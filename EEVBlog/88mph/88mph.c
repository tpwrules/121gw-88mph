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
#include <stdio.h>

#include "stm32l1xx_hal.h"

#include "hardware/lcd.h"
#include "hardware/hy3131.h"
#include "hardware/gpio.h"

#include "acquisition/reading.h"

#include "88mph.h"

void main_88mph(void) {
    // turn on measurement digital source
    GPIO_PINSET(HW_PWR_CTL);
    // turn on 4V analog source
    GPIO_PINSET(HW_PWR_CTL2);

    lcd_put_str(LCD_SCREEN_SUB, "hello");
    lcd_put_str(LCD_SCREEN_MAIN, "world");
    lcd_update();

    static const uint8_t regs[20] =
        {   0,   0,0x13,0x8A,   5,0x40,   0,0x4D,0x31,   1,
         0x22,   0,   0,0x90,0x28,0xA0,0x80,0xC7,   0,0x20};

    // set up DC5V mode in HY
    hy_write_regs(0x20, 20, regs);

    while (1) {
        // read AD1 value
        uint8_t ad1_buf[3];
        hy_read_regs(HY_REG_AD1_DATA, 3, ad1_buf);
        int32_t ad1 = ad1_buf[2] << 16 | ad1_buf[1] << 8 | ad1_buf[0];
        // do 24->32 bit sign extension
        if (ad1 & 0x800000) {
            ad1 |= 0xFF000000;
        }

        // coerce into reading for new function
        reading_t reading;
        reading.millicounts = (ad1 * 100)/6;
        reading.unit = RDG_UNIT_VOLTS;
        reading.exponent = 0;

        // now put it where it belongs
        lcd_put_reading(LCD_SCREEN_MAIN, reading);

        // flush screen changes and wait a bit before
        // reading sample register again
        lcd_update();
        HAL_Delay(100);
    }
}