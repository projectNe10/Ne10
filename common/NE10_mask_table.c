/*
 *  Copyright 2012 ARM Limited
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/*
 * NE10 Library : common/NE10_mask_table.c
 */

#include "NE10_mask_table.h"

const ne10_uint32_t ne10_qMaskTable32[MaskTableSize32] =
{
        0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000,
        0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x00000000,
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000,
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF
};
const ne10_uint32_t ne10_dMaskTable32[6] =
{
        0x00000000, 0x00000000,
        0xFFFFFFFF, 0x00000000,
        0xFFFFFFFF, 0xFFFFFFFF,

};


