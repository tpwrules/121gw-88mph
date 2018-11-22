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
#include "acq_mode_basic.h"
#include "acq_modes.h"
#include "acquisition.h"

#include "reading.h"
#include "hardware/hy3131.h"

// registers for volts dc
// first dimension is submode, second is register

static const uint8_t volts_dc_regs[1][20] = {
    // 5.000V
    {   0,   0,0x13,0x8A,   5,0x40,   0,0x4D,0x31,   1,
         0x22,   0,   0,0x90,0x28,0xA0,0x80,0xC7,   0,0x20}
};

void acq_mode_func_volts_dc(acq_event_t event, int64_t value) {
    switch (event) {
        case ACQ_EVENT_START:
        case ACQ_EVENT_SET_SUBMODE: {
            // starting and setting submodes is the same:
            // program new register set into the HY3131
            int submode = (int)value;
            hy_write_regs(0x20, 20,
                &volts_dc_regs[submode][0]);
            break;
        }

        case ACQ_EVENT_NEW_AD1: {
            int32_t ad1 = (int32_t)value;
            // ad1 is already nice and sign extended
            // all we need to do is put it into a reading
            reading_t reading;
            // approximately calibrate by dividing by 60
            reading.millicounts = (ad1*100)/6;
            reading.unit = RDG_UNIT_VOLTS;
            // in the future this will depend on the submode
            reading.exponent = 0;
            // tell the new reading to the rest of the acquisition engine
            acq_set_reading(0, reading);
            break;
        }

        // prolly should at least be turning off the interrupts at stop
        case ACQ_EVENT_STOP:
        default: {
            break;
        }
    }
}