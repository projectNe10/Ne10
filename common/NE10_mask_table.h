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
 * NE10 Library : common/NE10_mask_table.h
 */

#include "NE10_types.h"
#ifndef _ARM_MASK_TABLE_H
#define _ARM_MASK_TABLE_H

#define Q_MASK_TABLE_SIZE        20
#define D_MASK_TABLE_SIZE        6
#define DIV_LOOKUP_TABLE_SIZE    255

extern const ne10_uint32_t ne10_qMaskTable32[Q_MASK_TABLE_SIZE];
extern const ne10_uint32_t ne10_dMaskTable32[D_MASK_TABLE_SIZE];
extern const ne10_uint32_t ne10_divLookUpTable[DIV_LOOKUP_TABLE_SIZE];
#endif

