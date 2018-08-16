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

#include "hy3131.h"

#include "main.h" // for cube pin defs
#include "gpio.h"

// just wait some time to let setup and hold delays happen
static void spinloop(uint32_t times) {
    volatile uint32_t detimes = times;
    while (detimes--);
}

static void toggle_clock() {
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

static uint8_t recv_byte() {
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
}

// write a series of registers to the chip
void hy_write_regs(uint8_t start, uint8_t count, const uint8_t* data) {
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
}
