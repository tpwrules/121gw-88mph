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

#ifndef SYSTEM_JOB_H
#define SYSTEM_JOB_H

#include <stdint.h>
#include <stdbool.h>
#include "stm32l1xx.h"

// this module deals with Jobs
// we attach specific jobs to interrupts we won't use, then use the NVIC
// to automatically prioritize them

// this module also sets priorities for all the interrupts we do use

typedef enum {
    // this one must be enabled/disabled through hy_enable_irq
    // its handler is in hardware/hy3131.c
    JOB_ACQUISITION = EXTI3_IRQn,
    JOB_10MS_TIMER = TIM6_IRQn,
    JOB_SYSTEM = USB_HP_IRQn
} job_t;

// configure the NVIC for everything, but don't enable any of the jobs
void job_init(void);

// disable all the jobs
void job_deinit(void);

// enable a specific job, so it can run if it is scheduled
// this un-schedules the job before enabling it
void job_enable(job_t job);

// disable a specific job, so it won't run even if it is scheduled
void job_disable(job_t job);

// schedule a specific job, so it will run if it scheduled and no
// higher priority job is running. this will only run the job once!
void job_schedule(job_t job);


// catch the interrupts so we can direct them to their jobs
// JOB_SYSTEM
void USB_HP_IRQHandler(void);

// JOB_10MS_TIMER
void TIM6_IRQHandler(void);

#endif