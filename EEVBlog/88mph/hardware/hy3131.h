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

#ifndef HARDWARE_HY3131_H
#define HARDWARE_HY3131_H

#include <stdint.h>
#include <stdbool.h>

// this file manages interacting with the HY3131 measurement chip

// register defines
// multibyte registers are all sign-extended little endian

// last AD1 and AD2/3 value
#define HY_REG_AD1_DATA (0x00) // and the next two
#define HY_REG_AD2_DATA (0x03) // and the next two
// last low-pass filter value
#define HY_REG_LPF_DATA (0x06) // and the next two
// last RMS computation value
#define HY_REG_RMS_DATA (0x09) // and the next four
// peak hold minimum value
#define HY_REG_PKHMIN (0x0E) // and the next two
// peak hold maximum value
#define HY_REG_PKHMAX (0x11) // and the next two
// frequency counter status
#define HY_REG_CTSTA (0x14)
#define HY_REG_CTSTA_CTBOV (0x01) // bit set if CTB overflows
// frequency counter registers
#define HY_REG_CTC (0x15) // and the next two
#define HY_REG_CTB (0x18) // and the next two
#define HY_REG_CTA (0x1B) // and the next two
// interrupt flag; 1 when specified interrupt happened
#define HY_REG_INTF (0x1E)
// interrupt enable; 1 to enable specified interrupt
#define HY_REG_INTE (0x1F)

// BORF: set in INTF when chip detects brown-out
// note that this does not cause an IRQ!
#define HY_REG_INTF_BORF (0x80)
// the other interrupts: enable in INTE, monitor in INTF
#define HY_REG_INT_RMS (0x10)
#define HY_REG_INT_LPF (0x08)
#define HY_REG_INT_AD1 (0x04)
#define HY_REG_INT_AD2 (0x02)
#define HY_REG_INT_CT (0x01)

void hy_init(void);
void hy_deinit(void);

// the EXTI3 interrupt is triggered when the chip asserts its IRQ
void EXTI3_IRQHandler(void);

// turn on and off processing the IRQ from the chip

// enable IRQs only if enable is true
// if enable is false, the IRQ state is not changed
void hy_enable_irq(bool enable);
// disable IRQs. previous IRQ state is returned
bool hy_disable_irq(void);


// read a series of registers from the chip
void hy_read_regs(uint8_t start, uint8_t count, uint8_t* data);
// write a series of registers to the chip
void hy_write_regs(uint8_t start, uint8_t count, const uint8_t* data);

#endif