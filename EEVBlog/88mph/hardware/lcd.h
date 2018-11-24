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

#include "acquisition/reading.h"

extern uint32_t lcd_segment_buffer[8];

// define the digits on the screen for lcd_set_char
// this enum is in the order defined in lcd_7seg_segments
typedef enum {
    LCD_DIGIT_SS_10000=0,
    LCD_DIGIT_SS_1000=1,
    LCD_DIGIT_SS_100=2,
    LCD_DIGIT_SS_10=3,
    LCD_DIGIT_SS_1=4,

    LCD_DIGIT_MS_10000=5,
    LCD_DIGIT_MS_1000=6,
    LCD_DIGIT_MS_100=7,
    LCD_DIGIT_MS_10=8,
    LCD_DIGIT_MS_1=9
} lcd_digit_t;

// define the main and sub screens for the various put functions
typedef enum {
    LCD_SCREEN_SUB=0,
    LCD_SCREEN_MAIN=1
} lcd_screen_t;

// update the LCD from the segment buffer
void lcd_update(void);

// turn on and off segments
#define LCD_SEGON(seg) \
    (lcd_segment_buffer[LCD_RAM(seg)] |= LCD_SMASK_ON(seg))
#define LCD_SEGOFF(seg) \
    (lcd_segment_buffer[LCD_RAM(seg)] &= LCD_SMASK_OFF(seg))
#define LCD_SEGSET(seg, val)\
    (val ? LCD_SEGON(seg) : LCD_SEGOFF(seg))

// clear all the units and powers on the selected screen
void lcd_clear_units_powers(lcd_screen_t which);

// set a character on one of the LCD's 7 segment displays
void lcd_set_char(lcd_digit_t where, char c);
// write a string to a screen
// any longer than 5 chars is truncated
// any shorter is set to space
void lcd_put_str(lcd_screen_t which, char* s);
// put a reading on a screen
// automatically sets the units and powers accordingly
void lcd_put_reading(lcd_screen_t which, reading_t reading);

#endif
