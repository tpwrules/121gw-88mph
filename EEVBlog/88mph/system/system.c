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

#include "system/system.h"

#include "system/timer.h"
#include "hardware/lcd.h"
#include "hardware/buttons.h"
#include "acquisition/acquisition.h"
#include "acquisition/acq_modes.h"
#include "acquisition/reading.h"

void sys_main_loop(void) {
    __enable_irq();
    acq_init();
    timer_init();

    lcd_put_str(LCD_SCREEN_SUB, "hello");
    lcd_put_str(LCD_SCREEN_MAIN, "world");
    lcd_update();

    acq_set_mode(ACQ_MODE_VOLTS_DC, ACQ_MODE_VOLTS_DC_SUBMODE_5d0000);
    int submode = 0;

    button_t curr_button = BTN_NONE;

    while (1) {
        int new_submode = (HAL_GetTick()/1000)%4;
        if (submode != new_submode) {
            submode = new_submode;
            acq_set_submode(submode);
        }

        reading_t reading;

        if (acq_get_reading(0, &reading)) {
            lcd_put_reading(LCD_SCREEN_MAIN, reading);

            lcd_update();
        }

        button_t new_button = btn_get_new();
        if (new_button != BTN_NONE)
            curr_button = new_button;

        reading_t r;
        r.millicounts = ((int32_t)curr_button) * 1000;
        r.millicounts += ((int32_t)btn_get_rsw()) * 100000;
        lcd_put_reading(LCD_SCREEN_SUB, r);
    }
}