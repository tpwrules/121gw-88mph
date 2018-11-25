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

#include "hardware/lcd.h"

#include "hardware/lcd_segments.h"
#include "hardware/lcd_tables.h"

#include "acquisition/reading.h"

uint32_t lcd_segment_buffer[8];

static volatile bool lcd_needs_updating = false;

void lcd_queue_update(void) {
    lcd_needs_updating = true;
}

// try and update the display every 10ms
// called by the 10ms timer routine
void lcd_10ms_update_if_necessary(void) {
    // if the LCD is currently updating, we can't touch it
    // so return and it'll get done next interrupt
    if (LCD->SR & LCD_SR_UDR)
        return;
    // fill the LCD RAM with our buffer
    for (int i=0; i<8; i++) {
        LCD->RAM[i] = lcd_segment_buffer[i];
    }
    // and set the bit to trigger a new update
    LCD->SR |= LCD_SR_UDR;
    lcd_needs_updating = false;
}

// turn off all the units on the selected screen
void lcd_clear_units_powers(lcd_screen_t which) {
    for (int i=0; i<11; i++) {
        uint8_t seg = lcd_unit_icons[which][i];
        if (seg != SEG_NONE)
            LCD_SEGOFF(seg);
    }

    for (int i=0; i<6; i++) {
        uint8_t seg = lcd_exponent_icons[which][i];
        if (seg != SEG_NONE)
            LCD_SEGOFF(seg);
    }

    for (int i=0; i<4; i++) {
        LCD_SEGOFF(lcd_decimal_points[which][i]);
    }
}

// set a character on one of the LCD's 7 segment displays
void lcd_set_char(lcd_digit_t where, char c) {
    // first step: look up the char and get its segments
    uint8_t segs; // in xGFEDCBA order
    if (c >= 'A' && c <= 'Z') {
        segs = lcd_7seg_font[c-'A'+10];
    } else if (c >= 'a' && c <= 'z') {
        segs = lcd_7seg_font[c-'a'+10];
    } else if (c >= '0' && c <= '9') {
        segs = lcd_7seg_font[c-'0'];
    } else {
        switch (c) {
            case '=':
                segs = 0x48;
                break;
            case '-':
                segs = 0x40;
                break;
            case ' ':
            default:
                segs = 0x00;
        }
    }

    // now set them accordingly
    for (int i=0; i<7; i++) {
        LCD_SEGSET(lcd_7seg_segments[(uint8_t)where][i],
            segs & 1);
        segs >>= 1;
    }
}

// write a string to a screen
// any longer than 5 chars is truncated
// any shorter is set to space
void lcd_put_str(lcd_screen_t which, char* s) {
    lcd_digit_t where = 
        which == LCD_SCREEN_SUB ? LCD_DIGIT_SS_10000 : LCD_DIGIT_MS_10000;
    
    for (int i=0; i<5; i++) {
        char c = *s;
        lcd_set_char(where++, c);
        if (c)
            s++;
    }
}

// put a reading on a screen
// automatically sets the units and powers accordingly
void lcd_put_reading(lcd_screen_t which, reading_t reading) {
    // to do: somehow cache the current unit and exponent so that
    // we don't have to turn off and on the segments each time

    // but for now, we gotta
    lcd_clear_units_powers(which);

    // round reading to display size
    int val = (reading.millicounts + 500)/1000;

    // set negative sign
    if (which == LCD_SCREEN_MAIN) {
        LCD_SEGSET(SEG_MS_NEGATIVE, val < 0);
    } else {
        LCD_SEGSET(SEG_SS_NEGATIVE, val < 0);
    }

    val = (val < 0) ? -val : val;

    // put each digit in its place
    lcd_digit_t place = 
        which == LCD_SCREEN_SUB ? LCD_DIGIT_SS_1 : LCD_DIGIT_MS_1;
    for (int di=0; di<5; di++) {
        lcd_set_char(place--, (val % 10)+'0');
        val /= 10;
    }

    // turn on other icons if they exist
    uint8_t seg = lcd_unit_icons[which][reading.unit];
    if (seg != SEG_NONE) {
        LCD_SEGON(seg);
    }

    seg = lcd_exponent_icons[which][reading.exponent];
    if (seg != SEG_NONE) {
        LCD_SEGON(seg);
    }

    seg = lcd_decimal_points[which][reading.decimal];
    if (seg != SEG_NONE) {
        LCD_SEGON(seg);
    }
}