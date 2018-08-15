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

#ifndef HARDWARE_LCD_H
#define HARDWARE_LCD_H

/*
    This file contains functions to perform tasks on the LCD.
*/

#include <stdint.h>
#include <stdbool.h>

#include "lcd_segments.h"

extern uint32_t lcd_segment_buffer[8];

// update the LCD from the segment buffer
void lcd_update();

// turn on and off segments
#define LCD_SEGON(seg) \
    (lcd_segment_buffer[LCD_RAM(seg)] |= LCD_SMASK_ON(seg))
#define LCD_SEGOFF(seg) \
    (lcd_segment_buffer[LCD_RAM(seg)] &= LCD_SMASK_OFF(seg))
#define LCD_SEGSET(seg, val)\
    (val ? LCD_SEGON(seg) : LCD_SEGOFF(seg))

// set a character on the LCD
// 0-4: sub screen, left to right
// 5-9: main screen, left to right
void lcd_set_char(uint8_t where, char c);
// write a string to a screen
// 0: main, 1: sub
void lcd_put_str(bool on_subscreen, char* s);

#endif
