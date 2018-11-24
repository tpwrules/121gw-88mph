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

#ifndef HARDWARE_LCD_SEGMENTS_H
#define HARDWARE_LCD_SEGMENTS_H

/*
    This file defines the segments on the 121GW's LCD.
    Each definition encodes a RAM location and bit index.

    LCD_SMASK_ON(SEGn) -> 1 where this segment is, 0 otherwise
    LCD_SMASK_OFF(SEGn) -> 0 where this segment is, 1 otherwise
    LCD_RAM(SEGn) -> 0 to 7, the segment's address in LCD RAM
    LCD_BIT(SEGn) -> 0 to 31, the segment's bit index
*/

// pack a ram address and bit index
#define _LCD_SEGDEF(ram, bit) ((uint8_t)(((ram & 7) << 5) | bit))

// extract the above in useful ways
#define LCD_RAM(seg) ((uint8_t)((seg & 0xE0) >> 5))
#define LCD_BIT(seg) ((uint8_t)((seg & 0x1F)))

#define LCD_SMASK_ON(seg) ((uint32_t)(1 << LCD_BIT(seg)))
#define LCD_SMASK_OFF(seg) ((uint32_t)(~LCD_SMASK_ON(seg)))


// the hardware doesn't support a segment here, so this can be used
// as a nonexistent segment sentinel
// however, the hardware technically doesn't want you to turn it on
#define SEG_NONE                  (0xFF)

// screen icons, in no particular order
#define SEG_ICON_REL              _LCD_SEGDEF(0, 0)
#define SEG_ICON_MEM              _LCD_SEGDEF(0, 16)
#define SEG_ICON_TEST             _LCD_SEGDEF(0, 29)
#define SEG_ICON_LOWZ             _LCD_SEGDEF(0, 30)
#define SEG_ICON_BT               _LCD_SEGDEF(0, 31)
#define SEG_ICON_DANGER           _LCD_SEGDEF(1, 7)
#define SEG_ICON_LPF              _LCD_SEGDEF(2, 31)
#define SEG_ICON_CONTINUITY       _LCD_SEGDEF(4, 31)
#define SEG_ICON_APO              _LCD_SEGDEF(5, 7)
#define SEG_ICON_ONE_OF_1ms_PEAK  _LCD_SEGDEF(6, 16)
#define SEG_ICON_DIODE            _LCD_SEGDEF(6, 31)
#define SEG_ICON_AUTO_RANGE       _LCD_SEGDEF(7, 0)
#define SEG_ICON_BATT             _LCD_SEGDEF(7, 7)

// min/max
#define SEG_ICON_MIN              _LCD_SEGDEF(6, 30)
#define SEG_ICON_MAX              _LCD_SEGDEF(4, 30)
#define SEG_ICON_AVG              _LCD_SEGDEF(2, 30)

// hold
#define SEG_ICON_AUTO_OF_HOLD     _LCD_SEGDEF(2, 16)
#define SEG_ICON_HOLD             _LCD_SEGDEF(4, 16)

// main screen segments

// the digits
// negative indicator
#define SEG_MS_NEGATIVE           _LCD_SEGDEF(3, 7)
// leftmost digit
#define SEG_MS_10000_A            _LCD_SEGDEF(7, 6)
#define SEG_MS_10000_B            _LCD_SEGDEF(7, 5)
#define SEG_MS_10000_C            _LCD_SEGDEF(3, 5)
#define SEG_MS_10000_D            _LCD_SEGDEF(1, 6)
#define SEG_MS_10000_E            _LCD_SEGDEF(3, 6)
#define SEG_MS_10000_F            _LCD_SEGDEF(5, 6)
#define SEG_MS_10000_G            _LCD_SEGDEF(5, 5)
// and the decimal point right of it
#define SEG_MS_POINT_d0000        _LCD_SEGDEF(1, 5)

#define SEG_MS_1000_A             _LCD_SEGDEF(7, 4)
#define SEG_MS_1000_B             _LCD_SEGDEF(7, 3)
#define SEG_MS_1000_C             _LCD_SEGDEF(3, 3)
#define SEG_MS_1000_D             _LCD_SEGDEF(1, 4)
#define SEG_MS_1000_E             _LCD_SEGDEF(3, 4)
#define SEG_MS_1000_F             _LCD_SEGDEF(5, 4)
#define SEG_MS_1000_G             _LCD_SEGDEF(5, 3)
#define SEG_MS_POINT_d000         _LCD_SEGDEF(1, 3)

#define SEG_MS_100_A              _LCD_SEGDEF(7, 2)
#define SEG_MS_100_B              _LCD_SEGDEF(7, 1)
#define SEG_MS_100_C              _LCD_SEGDEF(3, 1)
#define SEG_MS_100_D              _LCD_SEGDEF(1, 2)
#define SEG_MS_100_E              _LCD_SEGDEF(3, 2)
#define SEG_MS_100_F              _LCD_SEGDEF(5, 2)
#define SEG_MS_100_G              _LCD_SEGDEF(5, 1)
#define SEG_MS_POINT_d00          _LCD_SEGDEF(1, 1)

#define SEG_MS_10_A               _LCD_SEGDEF(6, 15)
#define SEG_MS_10_B               _LCD_SEGDEF(6, 14)
#define SEG_MS_10_C               _LCD_SEGDEF(2, 14)
#define SEG_MS_10_D               _LCD_SEGDEF(0, 15)
#define SEG_MS_10_E               _LCD_SEGDEF(2, 15)
#define SEG_MS_10_F               _LCD_SEGDEF(4, 15)
#define SEG_MS_10_G               _LCD_SEGDEF(4, 14)
#define SEG_MS_POINT_d0           _LCD_SEGDEF(0, 14)

// rightmost
#define SEG_MS_1_A                _LCD_SEGDEF(6, 13)
#define SEG_MS_1_B                _LCD_SEGDEF(6, 12)
#define SEG_MS_1_C                _LCD_SEGDEF(2, 12)
#define SEG_MS_1_D                _LCD_SEGDEF(0, 13)
#define SEG_MS_1_E                _LCD_SEGDEF(2, 13)
#define SEG_MS_1_F                _LCD_SEGDEF(4, 13)
#define SEG_MS_1_G                _LCD_SEGDEF(4, 12)

// main screen modes
#define SEG_MS_DC                 _LCD_SEGDEF(5, 0)
#define SEG_MS_PLUS_OF_DC_AC      _LCD_SEGDEF(3, 0)
#define SEG_MS_AC                 _LCD_SEGDEF(1, 0)
#define SEG_MS_AMPS               _LCD_SEGDEF(6, 10)
#define SEG_MS_AMPS_NEXT_TO_F     _LCD_SEGDEF(6, 9)
#define SEG_MS_DUTY_PERCENT       _LCD_SEGDEF(0, 17)
#define SEG_MS_FARADS             _LCD_SEGDEF(4, 9)
#define SEG_MS_HERTZ              _LCD_SEGDEF(4, 8)
#define SEG_MS_mSEC               _LCD_SEGDEF(6, 0)
#define SEG_MS_OHMS               _LCD_SEGDEF(2, 8)
#define SEG_MS_VOLTS              _LCD_SEGDEF(4, 10)
#define SEG_MS_DEG_C              _LCD_SEGDEF(4, 11)
#define SEG_MS_DEG_F              _LCD_SEGDEF(6, 11)

// main screen units
#define SEG_MS_MEGA               _LCD_SEGDEF(0, 11)
#define SEG_MS_KILO               _LCD_SEGDEF(0, 8)
#define SEG_MS_MILLI              _LCD_SEGDEF(2, 10)
#define SEG_MS_MICRO              _LCD_SEGDEF(0, 9)
#define SEG_MS_NANO               _LCD_SEGDEF(2, 9)


// subscreen segments

// the digits
// negative indicator
#define SEG_SS_NEGATIVE           _LCD_SEGDEF(4, 29)
// leftmost digit
#define SEG_SS_10000_A            _LCD_SEGDEF(6, 28)
#define SEG_SS_10000_B            _LCD_SEGDEF(6, 27)
#define SEG_SS_10000_C            _LCD_SEGDEF(2, 27)
#define SEG_SS_10000_D            _LCD_SEGDEF(0, 28)
#define SEG_SS_10000_E            _LCD_SEGDEF(2, 28)
#define SEG_SS_10000_F            _LCD_SEGDEF(4, 28)
#define SEG_SS_10000_G            _LCD_SEGDEF(4, 27)
// and the decimal point right of it
#define SEG_SS_POINT_d0000        _LCD_SEGDEF(0, 27)

#define SEG_SS_1000_A             _LCD_SEGDEF(7, 8)
#define SEG_SS_1000_B             _LCD_SEGDEF(6, 25)
#define SEG_SS_1000_C             _LCD_SEGDEF(2, 25)
#define SEG_SS_1000_D             _LCD_SEGDEF(1, 8)
#define SEG_SS_1000_E             _LCD_SEGDEF(3, 8)
#define SEG_SS_1000_F             _LCD_SEGDEF(5, 8)
#define SEG_SS_1000_G             _LCD_SEGDEF(4, 25)
#define SEG_SS_POINT_d000         _LCD_SEGDEF(0, 25)

#define SEG_SS_100_A              _LCD_SEGDEF(6, 24)
#define SEG_SS_100_B              _LCD_SEGDEF(6, 23)
#define SEG_SS_100_C              _LCD_SEGDEF(2, 23)
#define SEG_SS_100_D              _LCD_SEGDEF(0, 24)
#define SEG_SS_100_E              _LCD_SEGDEF(2, 24)
#define SEG_SS_100_F              _LCD_SEGDEF(4, 24)
#define SEG_SS_100_G              _LCD_SEGDEF(4, 23)
#define SEG_SS_POINT_d00          _LCD_SEGDEF(0, 23)

#define SEG_SS_10_A               _LCD_SEGDEF(6, 22)
#define SEG_SS_10_B               _LCD_SEGDEF(6, 21)
#define SEG_SS_10_C               _LCD_SEGDEF(2, 21)
#define SEG_SS_10_D               _LCD_SEGDEF(0, 22)
#define SEG_SS_10_E               _LCD_SEGDEF(2, 22)
#define SEG_SS_10_F               _LCD_SEGDEF(4, 22)
#define SEG_SS_10_G               _LCD_SEGDEF(4, 21)
#define SEG_SS_POINT_d0           _LCD_SEGDEF(0, 21)

// rightmost
#define SEG_SS_1_A                _LCD_SEGDEF(6, 20)
#define SEG_SS_1_B                _LCD_SEGDEF(6, 19)
#define SEG_SS_1_C                _LCD_SEGDEF(2, 19)
#define SEG_SS_1_D                _LCD_SEGDEF(0, 20)
#define SEG_SS_1_E                _LCD_SEGDEF(2, 20)
#define SEG_SS_1_F                _LCD_SEGDEF(4, 20)
#define SEG_SS_1_G                _LCD_SEGDEF(4, 19)

// subscren modes
#define SEG_SS_AC                 _LCD_SEGDEF(6, 29)
#define SEG_SS_AMPS               _LCD_SEGDEF(2, 18)
#define SEG_SS_dB                 _LCD_SEGDEF(0, 19)
#define SEG_SS_DC                 _LCD_SEGDEF(2, 29)
#define SEG_SS_HERTZ              _LCD_SEGDEF(6, 17)
#define SEG_SS_nS                 _LCD_SEGDEF(4, 18)
#define SEG_SS_OHMS               _LCD_SEGDEF(6, 18)
#define SEG_SS_VOLTS              _LCD_SEGDEF(2, 17)

// subscreen units
#define SEG_SS_KILO               _LCD_SEGDEF(4, 17)
#define SEG_SS_MILLI              _LCD_SEGDEF(0, 18)

// the bargraph

// the 25 bars on the graph, left to right
#define SEG_BG_B1                 _LCD_SEGDEF(2, 1)
#define SEG_BG_B2                 _LCD_SEGDEF(4, 1)
#define SEG_BG_B3                 _LCD_SEGDEF(6, 1)
#define SEG_BG_B4                 _LCD_SEGDEF(6, 2)
#define SEG_BG_B5                 _LCD_SEGDEF(4, 2)
#define SEG_BG_B6                 _LCD_SEGDEF(2, 2)
#define SEG_BG_B7                 _LCD_SEGDEF(0, 2)
#define SEG_BG_B8                 _LCD_SEGDEF(0, 3)
#define SEG_BG_B9                 _LCD_SEGDEF(2, 3)
#define SEG_BG_B10                _LCD_SEGDEF(4, 3)
#define SEG_BG_B11                _LCD_SEGDEF(6, 3)
#define SEG_BG_B12                _LCD_SEGDEF(6, 4)
#define SEG_BG_B13                _LCD_SEGDEF(4, 4)
#define SEG_BG_B14                _LCD_SEGDEF(2, 4)
#define SEG_BG_B15                _LCD_SEGDEF(0, 4)
#define SEG_BG_B16                _LCD_SEGDEF(0, 5)
#define SEG_BG_B17                _LCD_SEGDEF(2, 5)
#define SEG_BG_B18                _LCD_SEGDEF(4, 5)
#define SEG_BG_B19                _LCD_SEGDEF(6, 5)
#define SEG_BG_B20                _LCD_SEGDEF(6, 6)
#define SEG_BG_B21                _LCD_SEGDEF(4, 6)
#define SEG_BG_B22                _LCD_SEGDEF(2, 6)
#define SEG_BG_B23                _LCD_SEGDEF(0, 6)
#define SEG_BG_B24                _LCD_SEGDEF(0, 7)
#define SEG_BG_B25                _LCD_SEGDEF(2, 7)

// the rest of the bargraph segments
#define SEG_BG_SCALE              _LCD_SEGDEF(0, 1)
#define SEG_BG_SCALE_5            _LCD_SEGDEF(0, 12)
#define SEG_BG_SCALE_10           _LCD_SEGDEF(0, 10)

#define SEG_BG_RANGE_5            _LCD_SEGDEF(4, 7)
#define SEG_BG_RANGE_0_FOR_50     _LCD_SEGDEF(6, 7)
#define SEG_BG_RANGE_0_FOR_500    _LCD_SEGDEF(2, 11)
#define SEG_BG_RANGE_1000         _LCD_SEGDEF(6, 8)

#define SEG_BG_NEGATIVE           _LCD_SEGDEF(4, 0)
#define SEG_BG_POSITIVE           _LCD_SEGDEF(2, 0)

#endif
