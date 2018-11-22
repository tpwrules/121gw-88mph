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

// pointers to the acquisition mode functions

#include <stdint.h>
#include "acq_modes.h"

#include "acquisition.h"
#include "acq_mode_basic.h"

const acq_mode_func acq_mode_funcs[2] = {
    // ACQ_MODE_MISC
    acq_mode_func_misc,
    // ACQ_MODE_VOLTS_DC
    acq_mode_func_volts_dc
};