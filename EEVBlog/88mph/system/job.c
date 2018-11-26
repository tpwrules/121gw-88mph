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
#include <stdbool.h>
#include "stm32l1xx.h"

#include "system/job.h"

void job_init(void) {
    // first, disable all the jobs
    job_deinit();

    // now configure the job priorities
    __disable_irq();
    // priority numbers go from 0 to 15, and 0 is the most important
    // the 1ms SYSTICK timer is the most important, because we want accurate
    // timing and it doesn't do much
    NVIC_SetPriority(SysTick_IRQn, 0);

    // next we want the 10ms timer. it's pretty fast and doesn't do much
    // it's handled by TIM6
    NVIC_SetPriority(JOB_10MS_TIMER, 1);

    // after the timers, handling acquisition is the most important
    NVIC_SetPriority(JOB_ACQUISITION, 5);
    // measurement is closely related to acquisition
    NVIC_SetPriority(JOB_MEASUREMENT, 7);

    // then the system job
    // it keeps the UI responsive
    NVIC_SetPriority(JOB_SYSTEM, 10);

    __enable_irq();
}

// disable all the jobs
void job_deinit(void) {
    // disable job-specific IRQs
    // all at once, so things don't get weird
    __disable_irq();
    job_disable(JOB_SYSTEM);
    job_disable(JOB_10MS_TIMER);
    job_disable(JOB_MEASUREMENT);
    __enable_irq();
}

// enable a specific job, so it can run if it is scheduled
// this un-schedules the job before enabling it
void job_enable(job_t job) {
    // un-pend the IRQ to de-schedule
    NVIC_ClearPendingIRQ((IRQn_Type)job);
    // now enable it so it can run
    NVIC_EnableIRQ((IRQn_Type)job);
}

// resume a specific job by enabling it but not un-scheduling it
// only resumes if resume is true. otherwise, does nothing
void job_resume(job_t job, bool resume) {
    if (resume) {
        NVIC_EnableIRQ((IRQn_Type)job);
    }
}

// disable a specific job, so it won't run even if it is scheduled
// returns whether or not the job was previously enabled
// (to be used with resume)
bool job_disable(job_t job) {
    __disable_irq();
    // this horrifying expression was mostly copied and pasted from CMSIS
    bool was_enabled = 
        NVIC->ISER[(((uint32_t)(int32_t)job) >> 5UL)] &
        (uint32_t)(1UL << (((uint32_t)(int32_t)job) & 0x1FUL));
    NVIC_DisableIRQ((IRQn_Type)job);
    __enable_irq();
    return was_enabled;
}

// schedule a specific job, so it will run if it scheduled and no
// higher priority job is running. this will only run the job once!
void job_schedule(job_t job) {
    NVIC_SetPendingIRQ((IRQn_Type)job);
}

// catch the interrupts so we can direct them to their jobs 

// JOB_SYSTEM
#include "system/system.h"
void USB_HP_IRQHandler(void) {
    sys_handle_job_system();
}

// JOB_10MS_TIMER
#include "system/timer.h"
void TIM6_IRQHandler(void) {
    timer_handle_job_10ms_timer();
}

// JOB_MEASUREMENT
#include "measurement/measurement.h"
void USB_LP_IRQHandler(void) {
    meas_handle_job_measurement();
}