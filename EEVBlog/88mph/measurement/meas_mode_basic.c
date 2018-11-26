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

#include "measurement/meas_mode_basic.h"

#include "measurement/measurement.h"
#include "measurement/meas_modes.h"
#include "acquisition/acquisition.h"
#include "acquisition/reading.h"

void meas_mode_func_volts_dc(meas_event_t event, reading_t* reading) {
    switch (event) {
        case MEAS_EVENT_START: {
            // switch the acquisition engine to the correct mode
            acq_set_mode(ACQ_MODE_VOLTS_DC, ACQ_MODE_VOLTS_DC_SUBMODE_5d0000);
            break;
        }

        case MEAS_EVENT_NEW_ACQ: {
            // just pass it through
            meas_set_reading(0, *reading);
            break;
        }

        default: {
            // if we get stopped, we can rely on the next guy to 
            // switch acquisition mode and stuff correctly
            break;
        }
    }
}