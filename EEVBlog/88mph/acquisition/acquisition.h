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

#ifndef ACQUISITION_ACQUISITION_H
#define ACQUISITION_ACQUISITION_H

#include <stdint.h>
#include <stdbool.h>

#include "acquisition/reading.h"
#include "acquisition/acq_modes.h"

// turn on the acquisition engine
void acq_init(void);
// turn off the acquisition engine
void acq_deinit(void);

// do the acquisition job
// check the HY3131 and calculate new acquisitions
void acq_handle_job_acquisition(void);

// set the HY interrupt mask register
void acq_set_int_mask(uint8_t mask);


// set acquisition state
void acq_set_mode(acq_mode_t mode, acq_submode_t submode);
void acq_set_submode(acq_submode_t submode);

// there is a queue of acquired values, effectively between the acquisition
// job and the measurement job
// put a reading into the queue. if there is no space it's just dropped
void acq_put_reading(reading_t* reading);
// get a reading from the queue. returns false if there is no reading to get.
// else puts the reading into reading and returns true
bool acq_get_reading(reading_t* reading);
// empty the queue of all readings
void acq_clear_readings(void);

void acq_mode_func_misc(acq_event_t event, int64_t value);

#endif