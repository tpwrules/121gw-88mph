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

#ifndef MEASUREMENT_MEAS_MODES_H
#define MEASUREMENT_MEAS_MODES_H

#include <stdint.h>

#include "acquisition/reading.h"

// this file defines the measurement modes
// the corresponding .c has a table with function pointers to the mode handlers
// keep the ordering of these options the same!

typedef enum {
    MEAS_MODE_OFF=0,
    MEAS_MODE_VOLTS_DC
} meas_mode_t;

// we also need to define the mode function
typedef enum {
    // these are called from the system job
    // begin measuring, reading is null
    MEAS_EVENT_START=0,
    // end measuring, reading is null
    MEAS_EVENT_STOP,

    // these are called from the measurement job
    // the acquisition engine has a new reading
    MEAS_EVENT_NEW_ACQ
} meas_event_t;

typedef void (*meas_mode_func)(meas_event_t event, reading_t* reading);

extern const meas_mode_func meas_mode_funcs[2];


#endif