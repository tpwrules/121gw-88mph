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

static void check_irq_line(void) {
    // the EXTI interrupt line is edge-sensitive
    // so if we turn on interrupts while the HY has already asserted
    // the interrupt line, we will never catch it
    // also, communicating with the HY will wiggle the DO line and trigger
    // spurious interrupts

    __disable_irq();
    // clear pending and perhaps fake EXTI interrupt
    EXTI->PR = EXTI_PR_PR3;
    // clear pending NVIC interrupt that would have happened as a result
    NVIC_ClearPendingIRQ((IRQn_Type)JOB_ACQUISITION);
    // check if the interrupt line is asserted
    if (GPIO_PINGET(HY_DO)) {
        // if it is, manually assert the interrupt in EXTI
        // as if it had seen the edge
        EXTI->SWIER = EXTI_SWIER_SWIER3;
    }
    __enable_irq();
}

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

    // finally, clear the interrupt mask inside the chip
    // this will also properly set up the interrupt state
    uint8_t mask = 0;
    hy_write_regs(HY_REG_INTE, 1, &mask);
}

void hy_deinit(void) {
    job_disable(JOB_ACQUISITION);
}


// chip interrupt is connected to EXTI3
void EXTI3_IRQHandler(void) {
    // acknowledge this interrupt in EXTI
    EXTI->PR = EXTI_PR_PR3;

    // it's time to do the job, probably because the HY bothered us
    acq_handle_job_acquisition();
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
    bool acq_enabled = job_disable(JOB_ACQUISITION);

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

    // clear spurious interrupts, but listen to the HY if it wants us
    check_irq_line();
    // configure the job how it was
    job_resume(JOB_ACQUISITION, acq_enabled);
}

// write a series of registers to the chip
void hy_write_regs(uint8_t start, uint8_t count, const uint8_t* data) {
    // we have to turn off interrupts while doing this
    // because the chip will be wiggling DO and making spurious interrupts
    // all over the place
    bool acq_enabled = job_disable(JOB_ACQUISITION);

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

    // clear spurious interrupts, but listen to the HY if it wants us
    check_irq_line();
    // configure the job how it was
    job_resume(JOB_ACQUISITION, acq_enabled);
}
