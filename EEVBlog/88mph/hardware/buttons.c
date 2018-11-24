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

#include "hardware/gpio.h"

// storage and data regarding all the buttons

// data on the button to be stored in ROM
typedef struct {
    // pointer to button's current value in GPIO register
    // true = released
    // false = pressed
    volatile bool* hw_not_val;
    bool can_be_held; // true if the button should register a BTN_HELD state
    uint8_t debounce_pressed; // 10ms units for how long the button should be
                              // pressed until it is considered such
    uint8_t debounce_released; // 10ms units for how long the button should be
                               // released until it is considered such
} button_data_t;

// state of the button to be stored in RAM
typedef struct {
    bool last_val; // value of the button last time it was read
    // must be volatile since it's read out of the interrupt
    volatile button_state_t state;
    uint8_t debounce_timer;
    uint8_t held_timer;
} button_mem_t;

// same order as the typedef
static const button_data_t button_data[18] = {
    // top row, left to right
    // BTN_RANGE
    {GPIO_PINGET_BITBAND_ADDR(B_RANGE), true, 5, 5},
    // BTN_HOLD
    {GPIO_PINGET_BITBAND_ADDR(B_HOLD), true, 5, 5},
    // BTN_REL
    {GPIO_PINGET_BITBAND_ADDR(B_REL), true, 5, 5},
    // BTN_1ms_PEAK
    {GPIO_PINGET_BITBAND_ADDR(B_PEAK), true, 5, 5},

    // bottom row, left to right
    // BTN_MODE
    {GPIO_PINGET_BITBAND_ADDR(B_MODE), true, 5, 5},
    // BTN_MIN_MAX
    {GPIO_PINGET_BITBAND_ADDR(B_MINMAX), true, 5, 5},
    // BTN_MEM
    {GPIO_PINGET_BITBAND_ADDR(B_MEM), true, 5, 5},
    // BTN_SETUP
    {GPIO_PINGET_BITBAND_ADDR(B_SETUP), true, 5, 5},

    // if something is in the corresponding jack
    // BTN_JACKDET_mA
    {GPIO_PINGET_BITBAND_ADDR(B_FUSE_mA), false, 20, 20},
    // BTN_JACKDET_A
    {GPIO_PINGET_BITBAND_ADDR(B_FUSE_A), false, 20, 20},

    // range switch, left to right
    // BTN_RSW_LowZ
    {GPIO_PINGET_BITBAND_ADDR(R_LowZ), false, 10, 10},
    // BTN_RSW_V
    {GPIO_PINGET_BITBAND_ADDR(R_VOLTS), false, 10, 10},
    // BTN_RSW_mV
    {GPIO_PINGET_BITBAND_ADDR(R_mV_TEMP), false, 10, 10},
    // BTN_RSW_Hz
    {GPIO_PINGET_BITBAND_ADDR(R_Hz_Duty), false, 10, 10},
    // BTN_RSW_OHMS
    {GPIO_PINGET_BITBAND_ADDR(R_O_B_D_C), false, 10, 10},
    // BTN_RSW_VA
    {GPIO_PINGET_BITBAND_ADDR(R_VA), false, 10, 10},
    // BTN_RSW_uA
    {GPIO_PINGET_BITBAND_ADDR(R_uA), false, 10, 10},
    // BTN_RSW_A
    {GPIO_PINGET_BITBAND_ADDR(R_mA_A), false, 10, 10},
};

// also the same order as the typedef
static button_mem_t button_mem[18];

// called every 10ms to do all the magic
void btn_process(void) {
    for (int bi=0; bi<16; bi++) {
        const button_data_t* this_data = &button_data[bi];
        button_mem_t* this_mem = &button_mem[bi];

        bool curr_val = !(*this_data->hw_not_val);

        // handle the debouncing first
        if (curr_val == this_mem->last_val) {
            // figure out how far we should count for debouncing
            uint8_t timer_target = this_mem->last_val ?
                this_data->debounce_released : this_data->debounce_pressed;
            // count up the timer if it's not already there
            if (this_mem->debounce_timer < timer_target) {
                this_mem->debounce_timer++;
                // if it just reached the target, change the state
                if (this_mem->debounce_timer == timer_target) {
                    this_mem->state = curr_val ?
                        BTN_JUST_PRESSED : BTN_JUST_RELEASED;
                    this_mem->held_timer = 0;
                }
            }
        } else {
            // they don't match, the button is bouncing, so reset the timer
            this_mem->debounce_timer = 0;
        }

        // then the holding
        // this looks the same as the debouncing
        if (this_data->can_be_held && 
                ((this_mem->state & 0xFE) == BTN_PRESSED)) {
            if (this_mem->held_timer < BTN_HELD_TIME) {
                this_mem->held_timer++;
                if (this_mem->held_timer == BTN_HELD_TIME) {
                    this_mem->state = BTN_JUST_HELD;
                }
            }
        }

        this_mem->last_val = curr_val;
    }
}

// returns the first button that has a 'just' state
// or BTN_NONE if there is no button with a 'just' state
// also sets it to the corresponding not-'just' state
// only works for the buttony buttons, not the range switch
button_t btn_get_new(void) {
    button_t the_button = BTN_NONE;
    __disable_irq();
    for (int bi=0; bi<10; bi++) {
        button_state_t state = button_mem[bi].state;
        if (state & 1) { // is 'just'
            // turn off 'just'
            button_mem[bi].state &= 0xFE;
            the_button = bi+1; // +1 to make room for zero
            break;
        }
    }
    __enable_irq();
    return the_button;
}

// get the position of the range switch
// returns BTN_NONE if 0 or more than 1 switch position is selected
button_t btn_get_rsw(void) {
    button_t the_button = BTN_NONE;
    __disable_irq();
    for (int bi=10; bi<18; bi++) {
        button_state_t state = button_mem[bi].state;
        if (state & 1) { // is 'just'
            // turn off 'just'
            button_mem[bi].state &= 0xFE;
        }
        if ((state & 0xFE) == BTN_PRESSED) {
            if (the_button != BTN_NONE) {
                // multiple ranges selected, return NONE instead
                the_button = BTN_NONE;
                break;
            }
            the_button = bi+1; // +1 to make room for zero
            // we keep going through all the position to avoid a double
            // situation
        }
    }
    __enable_irq();
    return the_button;
}