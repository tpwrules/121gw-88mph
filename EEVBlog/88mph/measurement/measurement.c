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

#include "measurement/measurement.h"

#include "acquisition/acquisition.h"
#include "measurement/meas_modes.h"
#include "system/job.h"

static meas_mode_func curr_meas_mode_func = 0;

// turn on the measurement engine
void meas_init(void) {
    // switch to the 'off' mode manually
    // cause there should be no previous mode func to call
    curr_meas_mode_func = meas_mode_funcs[MEAS_MODE_OFF];
    curr_meas_mode_func(MEAS_EVENT_START, NULL);
}

// turn off the measurement engine
void meas_deinit(void) {
    // stop the current measurement by switching to 'off'
    meas_set_mode(MEAS_MODE_OFF);
    // and cancel out the meas function
    curr_meas_mode_func = 0;
}

// do the measurement job
void meas_handle_job_measurement(void) {
    // fetch new acquisition
    reading_t reading;
    if (acq_get_reading(0, &reading)) {
        // and give it to the mode function if it's new
        curr_meas_mode_func(MEAS_EVENT_NEW_ACQ, &reading);
    }
}

// set the measurement mode
void meas_set_mode(meas_mode_t mode) {
    // turn off the current mode
    curr_meas_mode_func(MEAS_EVENT_STOP, NULL);
    // figure out which mode func goes with this mode
    curr_meas_mode_func = meas_mode_funcs[mode];
    // and start it up
    curr_meas_mode_func(MEAS_EVENT_START, NULL);
}

static reading_t curr_readings[4];
static bool is_reading_new[4] = {false, false, false, false};

void meas_set_reading(int which, reading_t reading) {
    // don't bother to turn off interrupts cause there's nothing
    // higher priority that would interrupt this
    curr_readings[which] = reading;
    is_reading_new[which] = true;
    // the system job is likely interested in the new measurement
    job_schedule(JOB_SYSTEM);
}

bool meas_get_reading(int which, reading_t* reading) {
    // stop the measurement job fiddling with this measurement
    __disable_irq();
    bool its_new = is_reading_new[which];
    if (its_new) {
        is_reading_new[which] = false;
        *reading = curr_readings[which];
    }
    __enable_irq();
    return its_new;
}

void meas_mode_func_off(meas_event_t event, reading_t* reading) {
    if (event == MEAS_EVENT_START) {
        // make sure the acquisition engine is turned off if we're not
        // measuring anything
        acq_set_mode(ACQ_MODE_MISC, ACQ_MODE_MISC_SUBMODE_OFF);
    }
}