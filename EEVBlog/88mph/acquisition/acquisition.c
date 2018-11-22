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

// this file handles the acquisition engine

#include <stdint.h>

#include "stm32l1xx_hal.h"

#include "hardware/hy3131.h"
#include "hardware/gpio.h"

#include "acquisition.h"

// turn on the acquisition engine
void acq_init() {
    // power up the digital supply for the measurement
    GPIO_PINSET(HW_PWR_CTL);
    // turn on the 4V analog supply
    GPIO_PINSET(HW_PWR_CTL2);
}

// turn off the acquisition engine
void acq_deinit() {
    // turn off analog supply
    GPIO_PINRST(HW_PWR_CTL2);
    // and then digital supply
    GPIO_PINRST(HW_PWR_CTL);
}