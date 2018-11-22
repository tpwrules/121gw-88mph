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

#ifndef ACQUISITION_ACQ_MODES_H
#define ACQUISITION_ACQ_MODES_H

#include <stdint.h>

// this file defines the acquisition modes and submodes
// the corresponding .c has a table with function pointers to the mode handlers
// keep the ordering of these options the same!

typedef enum {
    ACQ_MODE_MISC=0,
    ACQ_MODE_VOLTS_DC
} acq_mode_t;

// each mode starts its submodes at 0!!!
typedef enum {
    ACQ_MODE_MISC_SUBMODE_OFF=0,

    ACQ_MODE_VOLTS_DC_SUBMODE_5d000=0,
} acq_submode_t;

// we also need to define the mode function
typedef enum {
    // these are called from main thread
    // begin acquiring, value is initial submode
    ACQ_EVENT_START=0,
    // stop acquiring, value is meaningless
    ACQ_EVENT_STOP,
    // switch submodes, value is new submode
    ACQ_EVENT_SET_SUBMODE,

    // these are called from interrupt!
    // watch out
    // new measurement available, value is new measurement
    ACQ_EVENT_NEW_AD1
} acq_event_t;

typedef void (*acq_mode_func)(acq_event_t event, int64_t value);

extern const acq_mode_func acq_mode_funcs[2];

#endif