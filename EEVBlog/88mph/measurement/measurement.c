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
    // our job is to handle all the acquisitions
    reading_t reading;
    
    while (acq_get_reading(&reading)) {
        // tell the new reading to the mode function
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

// must be power of 2!!
#define MEAS_READING_QUEUE_SIZE (32)

#define Q_MASK (MEAS_READING_QUEUE_SIZE-1)

static volatile reading_t queue[MEAS_READING_QUEUE_SIZE];
static volatile int q_head = 0;
static volatile int q_tail = 0;

// put a reading into the queue. if there is no space it's just dropped
void meas_put_reading(reading_t* reading) {
    // can be called from any job, so protect ourselves!
    __disable_irq();
    // buffer head cause it's volatile
    // but we know we can't get interrupted
    int q_h = q_head;
    if (((q_h+1)&Q_MASK) != q_tail) {
        // we have space
        queue[q_h] = *reading;
        q_head = (q_h+1) & Q_MASK;
    }
    __enable_irq();

    // the system job is certainly interested in this new measurement
    job_schedule(JOB_SYSTEM);
}

// get a reading from the queue. returns false if there is no reading to get.
// else puts the reading into reading and returns true
bool meas_get_reading(reading_t* reading) {
    // can be called from any job, so protect ourselves!
    __disable_irq();
    // buffer tail cause it's volatile
    // but we know we can't get interrupted
    int q_t = q_tail;
    bool there_is_a_reading = (q_head != q_t);
    if (there_is_a_reading) {
        *reading = queue[q_t];
        q_tail = (q_t+1) & Q_MASK;
    }
    __enable_irq();
    return there_is_a_reading;
}

// empty the queue of all readings
void meas_clear_readings(void) {
    // can be called from any job, so protect ourselves!
    __disable_irq();
    q_head = 0;
    q_tail = 0;
    __enable_irq();
}

void meas_mode_func_off(meas_event_t event, reading_t* reading) {
    if (event == MEAS_EVENT_START) {
        // make sure the acquisition engine is turned off if we're not
        // measuring anything
        acq_set_mode(ACQ_MODE_MISC, ACQ_MODE_MISC_SUBMODE_OFF);
    }
}