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

#ifndef ACQUISITION_READING_H
#define ACQUISITION_READING_H

#include <stdint.h>

// this file introduces the concept of a Reading

// a Reading is associated with a unit
// note that the table lcd_unit_icons is in this order
typedef enum {
    RDG_UNIT_NONE=0,
    RDG_UNIT_AMPS,
    RDG_UNIT_PERCENT,
    RDG_UNIT_FARADS,
    RDG_UNIT_HERTZ,
    RDG_UNIT_SECONDS,
    RDG_UNIT_OHMS,
    RDG_UNIT_VOLTS,
    RDG_UNIT_DEG_C,
    RDG_UNIT_DEG_F,
    RDG_UNIT_dB
} rdg_unit_t;

typedef struct {
    // the value of the reading
    // one count -> one least significant digit display
    // thus the reading can be 1000 times more precise
    int32_t millicounts;
    // the base unit of the reading
    rdg_unit_t unit;
    // the exponent of the reading
    // exponent 0 and millicounts 1000
    // shows 00001 on the display
    int8_t exponent;
} reading_t;

#endif