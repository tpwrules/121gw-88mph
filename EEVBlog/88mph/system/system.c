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

#include "system/system.h"

#include "system/job.h"
#include "system/timer.h"
#include "hardware/lcd.h"
#include "hardware/buttons.h"
#include "measurement/measurement.h"
#include "measurement/meas_modes.h"
#include "acquisition/acquisition.h"
#include "acquisition/reading.h"

void sys_main_loop(void) {
    __enable_irq();
    job_init();
    acq_init();
    meas_init();
    timer_init();

    // enable all the jobs so the system starts working
    // do this with interrupts disabled so we can ensure we get
    // a chance to turn them all on!
    __disable_irq();
    job_enable(JOB_10MS_TIMER);
    job_enable(JOB_SYSTEM);
    job_enable(JOB_MEASUREMENT);
    job_enable(JOB_ACQUISITION);
    __enable_irq();

    meas_set_mode(MEAS_MODE_VOLTS_DC);

    while (1) {
        // the main loop just sleeps
        // we trust an interrupt will arrive and wake us up
        // if there is something interesting to do
        __WFI();
    }
}

void sys_handle_job_system(void) {
    // the system job basically does the UI
    // and routes around measurements

    static button_t curr_button = BTN_NONE;
    static button_t curr_state = BTN_RELEASED;

    reading_t reading;
    bool got_new_reading = false;
    // get the latest reading
    while (meas_get_reading(&reading)) {
        got_new_reading = true;
    }

    // and put it on the screen
    if (got_new_reading) {
        lcd_put_reading(LCD_SCREEN_MAIN, reading);
        lcd_queue_update();
    }

    button_state_t new_state;
    button_t new_button = btn_get_new(&new_state);
    if (new_button != BTN_NONE) {
        curr_button = new_button;
        curr_state = new_state;
    }

    reading_t r = {
        // millicounts
        (((int32_t)curr_button) * 1000)+
        (((int32_t)curr_state) * 100000)+
        (((int32_t)btn_get_rsw()) * 1000000),
        0, // time_ms
        RDG_UNIT_NONE, // unit
        RDG_EXPONENT_NONE, // exponent
        RDG_DECIMAL_10000, // decimal
        RDG_KIND_MAIN // kind
    };
    lcd_put_reading(LCD_SCREEN_SUB, r);
}