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

// this controls which exponent lights up on the LCD
// note that the table lcd_exponent_icons is in this order
typedef enum {
    RDG_EXPONENT_NANO=0,
    RDG_EXPONENT_MICRO,
    RDG_EXPONENT_MILLI,
    RDG_EXPONENT_NONE, // no exponent
    RDG_EXPONENT_KILO,
    RDG_EXPONENT_MEGA
} rdg_exponent_t;

// this controls which decimal point lights up on the LCD
// note that the table lcd_decimal_points is in this order
typedef enum {
    RDG_DECIMAL_1d0000=0,
    RDG_DECIMAL_10d000,
    RDG_DECIMAL_100d00,
    RDG_DECIMAL_1000d0,
    RDG_DECIMAL_10000 // no decimal
} rdg_decimal_t;

// this controls the reading's kind, and thus its fate
typedef enum {
    RDG_KIND_MAIN, // main screen reading
} rdg_kind_t;

typedef struct {
    // the value of the reading
    // one count -> one least significant digit display
    // thus the reading can be 1000 times more precise in calculations
    int32_t millicounts;
    // the milliseconds the reading was taken at
    // used for logging purposes
    uint32_t time_ms;
    // the base unit of the reading
    rdg_unit_t unit;
    // the unit's exponent and decimal position
    // there is no requirement that these be normalized
    // i.e. 1000.0V and 1.0000kV are both allowed, but will have different
    // values here
    rdg_exponent_t exponent;
    rdg_decimal_t decimal;
    // what the reading's purpose is in life
    rdg_kind_t kind;
} reading_t;

#endif