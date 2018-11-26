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

#ifndef MEASUREMENT_MEASUREMENT_H
#define MEASUREMENT_MEASUREMENT_H

#include <stdint.h>
#include <stdbool.h>

#include "measurement/meas_modes.h"
#include "acquisition/reading.h"

// turn on the measurement engine
void meas_init(void);
// turn off the measurement engine
void meas_deinit(void);

// do the measurement job
void meas_handle_job_measurement(void);

// set the measurement mode
void meas_set_mode(meas_mode_t mode);

// there is a queue of measured values, effectively between the measurement
// job and the system job
// put a reading into the queue. if there is no space it's just dropped
void meas_put_reading(reading_t* reading);
// get a reading from the queue. returns false if there is no reading to get.
// else puts the reading into reading and returns true
bool meas_get_reading(reading_t* reading);
// empty the queue of all readings
void meas_clear_readings(void);

void meas_mode_func_off(meas_event_t event, reading_t* reading);

#endif