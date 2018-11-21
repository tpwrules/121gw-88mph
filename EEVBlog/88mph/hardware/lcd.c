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

#include "stm32l1xx.h"

#include "lcd.h"
#include "lcd_tables.h"

uint32_t lcd_segment_buffer[8];

// transfer the LCD buffer to LCD RAM and trigger an update
void lcd_update() {
    // wait for any pending update to be completed
    while (LCD->SR & LCD_SR_UDR);
    // fill the LCD RAM with our buffer
    for (int i=0; i<8; i++) {
        LCD->RAM[i] = lcd_segment_buffer[i];
    }
    // and set the bit to trigger a new update
    LCD->SR |= LCD_SR_UDR;
}

// set a character on one of the LCD's 7 segment displays
void lcd_set_char(lcd_digit_t where, char c) {
    // first step: look up the char and get its segments
    uint8_t segs;
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