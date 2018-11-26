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

// handle the measured values
// there are 4 available readings
// which one is which depends on the mode
void meas_set_reading(int which, reading_t reading);
// returns false if the reading isn't new
// if it is new, returns true and puts it in reading
bool meas_get_reading(int which, reading_t* reading);

void meas_mode_func_off(meas_event_t event, reading_t* reading);

#endif