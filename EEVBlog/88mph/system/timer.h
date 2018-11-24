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

#ifndef SYSTEM_TIMER_H
#define SYSTEM_TIMER_H

#include <stdint.h>

// this file handles the two system timers
// 1ms and 10ms

void timer_init(void);
void timer_deinit(void);

// callback for 1ms timer
void HAL_SYSTICK_Callback(void);
// callback for 10ms timer
void TIM3_IRQHandler(void);

// number of milliseconds since timer was inited
extern volatile uint32_t timer_1ms_ticks;
// number of 10 millisecond periods since timer was inited
extern volatile uint32_t timer_10ms_ticks;

#endif