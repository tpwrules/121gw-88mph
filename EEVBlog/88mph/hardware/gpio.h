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

#ifndef HARDWARE_GPIO_H
#define HARDWARE_GPIO_H

// this file just has some GPIO-related macros
// because I'm not so fond of the HAL ones
// it's designed to work with cube-generated pin names

#include "stm32l1xx_hal.h"

// set a pin to 1
#define GPIO_PINSET(pin) \
    do {pin ## _GPIO_Port->BSRR = (pin ## _Pin);} while(0)
// reset a pin to 0
#define GPIO_PINRST(pin) \
    do {pin ## _GPIO_Port->BSRR = (pin ## _Pin) << 16;} while(0)
// change pin state depending on predicate
#define GPIO_PINCHG(pin, predicate) \
    do {pin ## _GPIO_Port->BSRR = \
        (pin ## _Pin) << ((predicate) ? 0 : 16);} while(0)

// return 1 or 0 depending on pin state
#define GPIO_PINGET(pin) \
        ((pin ## _GPIO_Port->IDR & (pin ## _Pin)) ? 1 : 0)

#endif