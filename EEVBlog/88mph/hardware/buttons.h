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

#ifndef HARDWARE_BUTTONS_H
#define HARDWARE_BUTTONS_H

// this file handles reading the buttons, debouncing them, and
// determining held-ness

// how long a button is pressed before it's considered held
// in 10ms units, max 255
#define BTN_HELD_TIME (100) // 1 second

// JUST_ means the state is new
// when read, it goes to the corresponding non-JUST state
// do not change this!!
typedef enum {
    // finger off the button
    BTN_RELEASED = 0,
    BTN_JUST_RELEASED = 1,

    // finger on the button
    BTN_PRESSED = 2,
    BTN_JUST_PRESSED = 3,

    // finger on the button for at least BTN_HELD_TIME
    // (if the button can be held)
    BTN_HELD = 4,
    BTN_JUST_HELD = 5
} button_state_t;

// the buttons! finally
// do not change the order!!
// the rest of the data is in a table in buttons.c
typedef enum {
    // nothing was pressed or held
    BTN_NONE = 0,

    // top row, left to right
    BTN_RANGE,
    BTN_HOLD,
    BTN_REL,
    BTN_1ms_PEAK,

    // bottom row, left to right
    BTN_MODE,
    BTN_MIN_MAX,
    BTN_MEM,
    BTN_SETUP,

    // if something is in the corresponding jack
    BTN_JACKDET_mA,
    BTN_JACKDET_A,

    // range switch, left to right
    BTN_RSW_LowZ,
    BTN_RSW_V,
    BTN_RSW_mV,
    BTN_RSW_Hz,
    BTN_RSW_OHMS,
    BTN_RSW_VA,
    BTN_RSW_uA,
    BTN_RSW_A
} button_t;

// called every 10ms to do all the magic
void btn_process(void);

// returns the first button that has a 'just' state
// or BTN_NONE if there is no button with a 'just' state
// also sets it to the corresponding not-'just' state
// only works for the buttony buttons, not the range switch
button_t btn_get_new(void);

// get the position of the range switch
// returns BTN_NONE if 0 or more than 1 switch position is selected
button_t btn_get_rsw(void);

#endif