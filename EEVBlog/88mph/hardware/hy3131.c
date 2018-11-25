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

#include "hardware/hy3131.h"

#include "hardware/gpio.h"
#include "system/job.h"
#include "acquisition/acquisition.h"

void hy_init(void) {
    // set up the interrupt handler
    // the chip DO line is connected to PF3, so it's on the EXTI3 line

    // disable the job so interrupts start off disabled
    job_disable(JOB_ACQUISITION);

    // configure EXTI3 to monitor the F pin
    MODIFY_REG(SYSCFG->EXTICR[0], // this EXTICR starts at 0 unlike the docs >_>
        SYSCFG_EXTICR1_EXTI3, 
        SYSCFG_EXTICR1_EXTI3_PF);

    // enable the rising edge interrupt
    SET_BIT(EXTI->RTSR, EXTI_RTSR_TR3);

    // and unmask the interrupt so the NVIC sees it
    SET_BIT(EXTI->IMR, EXTI_IMR_MR3);
}

void hy_deinit(void) {
    hy_enable_irq(false);
}


static volatile bool irq_enabled = false;

// chip interrupt is connected to EXTI3
void EXTI3_IRQHandler(void) {
    // acknowledge this interrupt in EXTI
    EXTI->PR = EXTI_PR_PR3;

    // it's time to do the job, probably because the HY bothered us
    acq_handle_job_acquisition();
}

void hy_enable_irq(bool enable) {
    if (!enable) return;

    __disable_irq();
    irq_enabled = true;
    // clear any pending interrupt in the EXTI
    EXTI->PR = EXTI_PR_PR3;
    // enable the job so we get notified again
    job_enable(JOB_ACQUISITION);
    // check to see if the chip currently is trying to interrupt us
    if (GPIO_PINGET(HY_DO)) {
        // if it is, the input is edge-triggered and we just cleared it,
        // so we have to trigger the interrupt in software since the
        // input won't see the edge again
        job_schedule(JOB_ACQUISITION);
    }
    __enable_irq();
}

bool hy_disable_irq(void) {
    __disable_irq();
    bool previous = irq_enabled;
    irq_enabled = false;
    // disable the job so we don't get bothered
    job_disable(JOB_ACQUISITION);
    __enable_irq();
    return previous;
}


// just wait some time to let setup and hold delays happen
static void spinloop(uint32_t times) {
    volatile uint32_t detimes = times;
    while (detimes--);
}

static void toggle_clock(void) {
    spinloop(1);
    GPIO_PINSET(HY_CK);
    spinloop(1);
    GPIO_PINRST(HY_CK);
}

static void send_byte(uint8_t byte) {
    for (int bit=0; bit<8; bit++) {
        GPIO_PINCHG(HY_DI, byte & 0x80);
        byte <<= 1;
        toggle_clock();
    }
}

static uint8_t recv_byte(void) {
    uint8_t byte = 0;
    for (int bit=0; bit<8; bit++) {
        // toggle clock before reading bit because there is a 1 bit
        // turnaround time between sending and receiving
        toggle_clock();
        byte <<= 1;
        byte |= GPIO_PINGET(HY_DO);
    }
    return byte;
}

// read a series of registers from the chip
void hy_read_regs(uint8_t start, uint8_t count, uint8_t* data) {
    // we have to turn off interrupts while doing this
    // because the chip will be wiggling DO and making spurious interrupts
    // all over the place
    bool prev = hy_disable_irq();

    // assert chip select
    GPIO_PINRST(HY_CS);

    // send address and read mode bit
    send_byte(start << 1 | 1);

    // shift out zeros
    GPIO_PINRST(HY_DI);

    // and now receive the data
    for (int ri = 0; ri < count; ri++) {
        data[ri] = recv_byte();
    }

    // deassert chip select to finish off 
    GPIO_PINSET(HY_CS);
    // wait for DO to stabilize once CS is deasserted
    spinloop(1);
    // and configure interrupts how they were
    hy_enable_irq(prev);
}

// write a series of registers to the chip
void hy_write_regs(uint8_t start, uint8_t count, const uint8_t* data) {
    // we have to turn off interrupts while doing this
    // because the chip will be wiggling DO and making spurious interrupts
    // all over the place
    bool prev = hy_disable_irq();

    // assert chip select
    GPIO_PINRST(HY_CS);

    // send address and write mode bit
    send_byte(start << 1 | 0);

    // now send the data
    for (int ri = 0; ri < count; ri++) {
        send_byte(data[ri]);
    }

    // deassert chip select and data out to finish off 
    GPIO_PINSET(HY_CS);
    GPIO_PINRST(HY_DI);
    // wait for DO to stabilize once CS is deasserted
    spinloop(1);
    // and configure interrupts how they were
    hy_enable_irq(prev);
}
