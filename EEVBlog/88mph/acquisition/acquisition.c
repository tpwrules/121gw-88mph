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

// this file handles the acquisition engine

#include <stdint.h>
#include <stdbool.h>
#include "stm32l1xx.h"

#include "acquisition/acquisition.h"

#include "acquisition/acq_modes.h"
#include "system/job.h"
#include "hardware/hy3131.h"
#include "hardware/gpio.h"

static acq_mode_func curr_acq_mode_func = 0;
static volatile uint8_t curr_int_mask = 0;

// turn on the acquisition engine
void acq_init(void) {
    // power up the digital supply for the measurement
    GPIO_PINSET(HW_PWR_CTL);
    // turn on the 4V analog supply
    GPIO_PINSET(HW_PWR_CTL2);
    // give the HY3131 a bit of time to power up
    // who knows if this is necessary, but it feels good
    HAL_Delay(10);
    // initialize it
    hy_init();
    // switch to the 'off' mode manually
    // cause there should be no previous mode func to call
    curr_acq_mode_func = acq_mode_funcs[ACQ_MODE_MISC];
    curr_acq_mode_func(ACQ_EVENT_START, (int64_t)ACQ_MODE_MISC_SUBMODE_OFF);
}

// turn off the acquisition engine
void acq_deinit(void) {
    // stop the current acquisition by switching to 'off'
    acq_set_mode(ACQ_MODE_MISC, ACQ_MODE_MISC_SUBMODE_OFF);
    // and cancel out the acq function
    curr_acq_mode_func = 0;
    // stop the HY3131
    hy_deinit();
    // turn off analog supply
    GPIO_PINRST(HW_PWR_CTL2);
    // and then digital supply
    GPIO_PINRST(HW_PWR_CTL);
}

// called when the HY3131 triggers an interrupt
// it's okay if there's actually nothing to do
void acq_process_hy_int(void) {
    uint8_t regbuf[5];

    // read which interrupts are pending
    // this also clears the pending interrupts
    uint8_t which_ints;
    hy_read_regs(HY_REG_INTF, 1, &which_ints);
    // only handle pending interrupts which are enabled
    which_ints &= curr_int_mask;
    if (which_ints & HY_REG_INT_AD1) {
        // read the 24 bit AD1 register
        hy_read_regs(HY_REG_AD1_DATA, 3, regbuf);
        int32_t val = regbuf[2] << 16 | regbuf[1] << 8 | regbuf[0];
        // sign extend 24 bits to 32
        if (val & 0x800000) {
            val |= 0xFF000000;
        }
        // tell the current acquisition mode about it
        curr_acq_mode_func(ACQ_EVENT_NEW_AD1, val);
    }
}

void acq_set_int_mask(uint8_t mask) {
    // disable interrupts around this so curr_int_mask isn't wrong
    bool prev = hy_disable_irq();
    curr_int_mask = mask;
    // the caller probably is changing the int mask because they've reconfigured
    // the chip, so clear pending interrupts from the chip first
    uint8_t whatever;
    hy_read_regs(HY_REG_INTF, 1, &whatever);
    // now enable the interrupts the user wanted
    hy_write_regs(HY_REG_INTE, 1, &mask);
    // and start listening for them
    hy_enable_irq(prev);
}

void acq_set_mode(acq_mode_t mode, acq_submode_t submode) {
    // turn off the current mode
    curr_acq_mode_func(ACQ_EVENT_STOP, 0);
    // figure out which mode func goes with this mode
    curr_acq_mode_func = acq_mode_funcs[mode];
    // and start it up
    curr_acq_mode_func(ACQ_EVENT_START, (int64_t)submode);
}

void acq_set_submode(acq_submode_t submode) {
    curr_acq_mode_func(ACQ_EVENT_SET_SUBMODE, (int64_t)submode);
}

static reading_t curr_readings[4];
static bool is_reading_new[4] = {false, false, false, false};

void acq_set_reading(int which, reading_t reading) {
    // don't bother to turn off interrupts cause there's nothing
    // higher priority that would interrupt this
    curr_readings[which] = reading;
    is_reading_new[which] = true;
    // the system job is likely interested in the new reading
    job_schedule(JOB_SYSTEM);
}

bool acq_get_reading(int which, reading_t* reading) {
    // it might get set from the hy interrupt
    __disable_irq();
    bool its_new = is_reading_new[which];
    if (its_new) {
        is_reading_new[which] = false;
        *reading = curr_readings[which];
    }
    __enable_irq();
    return its_new;
}

// misc mode handler
void acq_mode_func_misc(acq_event_t event, int64_t value) {
    // for now, all this mode should be doing is turning off
    hy_disable_irq();
    acq_set_int_mask(0);
}