/*****************************************************************************
 *  Copyright 2018 Thomas Watson                                             *
 *                                                                           *
 *  This file is a part of 88mph: https://github.com/tpwrules/121gw-88mph/   *
 *                                                                           *
 *  Licensed under the Apache License, Version 2.0 (the "License");          *
 *  you may not use this file except in compliance with the License.         *
 *  You may obtain a copy of the License at                                  *
 *                                                                           *
 *      http://www.apache.org/licenses/LICENSE-2.0                           *
 *                                                                           *
 *  Unless required by applicable law or agreed to in writing, software      *
 *  distributed under the License is distributed on an "AS IS" BASIS,        *
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. *
 *  See the License for the specific language governing permissions and      *
 *  limitations under the License.                                           *
 *****************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include "stm32l1xx.h"

#include "hardware/buttons.h"

#include "system/job.h"
#include "hardware/gpio.h"

// storage and data regarding all the buttons

// data on the button to be stored in ROM
typedef struct {
    // pointer to button's current value in GPIO register
    // true = released
    // false = pressed
    volatile bool* hw_not_val;
    bool can_be_held; // true if the button should register a BTN_HELD state
    uint8_t debounce_time; // 10ms units for how long the button's value should
                           // remain constant before the state changes
} button_data_t;

// state of the button to be stored in RAM
typedef struct {
    bool last_val; // value of the button last time it was read
    // these must be volatile since they're touched outside the interrupt
    volatile button_state_t state;
    volatile bool state_is_new;
    // these count down towards zero
    uint8_t debounce_timer;
    uint8_t held_timer;
} button_mem_t;

// same order as the typedef
static const button_data_t button_data[18] = {
    // top row, left to right
    // BTN_RANGE
    {GPIO_PINGET_BITBAND_ADDR(B_RANGE), true, 5},
    // BTN_HOLD
    {GPIO_PINGET_BITBAND_ADDR(B_HOLD), true, 5},
    // BTN_REL
    {GPIO_PINGET_BITBAND_ADDR(B_REL), true, 5},
    // BTN_1ms_PEAK
    {GPIO_PINGET_BITBAND_ADDR(B_PEAK), true, 5},

    // bottom row, left to right
    // BTN_MODE
    {GPIO_PINGET_BITBAND_ADDR(B_MODE), true, 5},
    // BTN_MIN_MAX
    {GPIO_PINGET_BITBAND_ADDR(B_MINMAX), true, 5},
    // BTN_MEM
    {GPIO_PINGET_BITBAND_ADDR(B_MEM), true, 5},
    // BTN_SETUP
    {GPIO_PINGET_BITBAND_ADDR(B_SETUP), true, 5},

    // if something is in the corresponding jack
    // BTN_JACKDET_mA
    {GPIO_PINGET_BITBAND_ADDR(B_FUSE_mA), false, 20},
    // BTN_JACKDET_A
    {GPIO_PINGET_BITBAND_ADDR(B_FUSE_A), false, 20},

    // range switch, left to right
    // BTN_RSW_LowZ
    {GPIO_PINGET_BITBAND_ADDR(R_LowZ), false, 10},
    // BTN_RSW_V
    {GPIO_PINGET_BITBAND_ADDR(R_VOLTS), false, 10},
    // BTN_RSW_mV
    {GPIO_PINGET_BITBAND_ADDR(R_mV_TEMP), false, 10},
    // BTN_RSW_Hz
    {GPIO_PINGET_BITBAND_ADDR(R_Hz_Duty), false, 10},
    // BTN_RSW_OHMS
    {GPIO_PINGET_BITBAND_ADDR(R_O_B_D_C), false, 10},
    // BTN_RSW_VA
    {GPIO_PINGET_BITBAND_ADDR(R_VA), false, 10},
    // BTN_RSW_uA
    {GPIO_PINGET_BITBAND_ADDR(R_uA), false, 10},
    // BTN_RSW_A
    {GPIO_PINGET_BITBAND_ADDR(R_mA_A), false, 10},
};

// also the same order as the typedef
static button_mem_t button_mem[18];

// called every 10ms to do all the magic
void btn_process(void) {
    bool some_state_was_updated = false;

    for (int bi=0; bi<18; bi++) {
        const button_data_t* this_data = &button_data[bi];
        button_mem_t* this_mem = &button_mem[bi];

        bool curr_val = !(*this_data->hw_not_val);

        // handle the debouncing first
        if (curr_val == this_mem->last_val) {
            // count down the timer if it's not already there
            if (this_mem->debounce_timer) {
                this_mem->debounce_timer--;
                // if it just reached zero, change the state
                if (this_mem->debounce_timer == 0) {
                    this_mem->state = curr_val ?
                        BTN_PRESSED : BTN_RELEASED;
                    this_mem->state_is_new = true;
                    this_mem->held_timer = BTN_HELD_TIME;
                    some_state_was_updated = true;
                }
            }
        } else {
            // they don't match, the button is bouncing, so reset the timer
            this_mem->debounce_timer = this_data->debounce_time;
        }

        this_mem->last_val = curr_val;

        // then the holding
        if (this_data->can_be_held && 
                (this_mem->state == BTN_PRESSED)) {
            if (this_mem->held_timer) {
                this_mem->held_timer--;
                if (this_mem->held_timer == 0) {
                    this_mem->state = BTN_HELD;
                    this_mem->state_is_new = true;
                    some_state_was_updated = true;
                }
            }
        }
    }

    // the system job cares about button state changes
    if (some_state_was_updated) {
        // schedule it so it can do something in response
        job_schedule(JOB_SYSTEM);
    }
}

// looks at the non-range-switch buttons and returns the first one whose state
// is new. it then clears the new flag and stores the state in new_state
// if there is no such button, returns BTN_NONE and does not change new_state
button_t btn_get_new(button_state_t* new_state) {
    button_t the_button = BTN_NONE;
    __disable_irq();
    for (int bi=0; bi<10; bi++) {
        if (button_mem[bi].state_is_new) {
            button_mem[bi].state_is_new = false;
            *new_state = button_mem[bi].state;
            the_button = bi+1; // +1 to make room for BTN_NONE
            break;
        }
    }
    __enable_irq();
    return the_button;
}

// returns the state of the corresponding button and clears the new flag
// always returns BTN_RELEASED for BTN_NONE
button_state_t btn_get_state(button_t button) {
    if (button == BTN_NONE) {
        return BTN_RELEASED;
    }

    // -1 to account for BTN_NONE
    uint8_t bi = (uint8_t)button - 1;

    __disable_irq();
    button_state_t state = button_mem[bi].state;
    button_mem[bi].state_is_new = false;
    __enable_irq();
    return state;
}

// get the position of the range switch
// returns BTN_NONE if 0 or more than 1 switch position is selected
button_t btn_get_rsw(void) {
    button_t the_button = BTN_NONE;
    __disable_irq();
    for (int bi=10; bi<18; bi++) {
        button_state_t state = button_mem[bi].state;
        // we don't care about clearing the new flag since there's no way
        // to read it
        if (state == BTN_PRESSED) {
            if (the_button != BTN_NONE) {
                // multiple ranges selected, return NONE instead
                the_button = BTN_NONE;
                break;
            }
            the_button = bi+1; // +1 to make room for BTN_NONE
            // we keep going through all the positions so we can check that
            // not more than one is pressed
        }
    }
    __enable_irq();
    return the_button;
}