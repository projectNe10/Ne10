/*
 *  Copyright 2012-14 ARM Limited
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    * Neither the name of ARM Limited nor the
 *      names of its contributors may be used to endorse or promote products
 *      derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY ARM LIMITED AND CONTRIBUTORS "AS IS" AND
 *  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL ARM LIMITED BE LIABLE FOR ANY
 *  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

/* mask table for dsp module */
extern const ne10_uint32_t ne10_qMaskTable32[Q_MASK_TABLE_SIZE] asm ("ne10_qMaskTable32");
extern const ne10_uint32_t ne10_dMaskTable32[D_MASK_TABLE_SIZE] asm ("ne10_dMaskTable32");
extern const ne10_uint32_t ne10_divLookUpTable[DIV_LOOKUP_TABLE_SIZE] \
asm ("ne10_divLookUpTable");

/* mask table for imgproc module */
#define NE10_VRESIZE_LINEAR_MASK_TABLE_SIZE    7
extern const ne10_uint64_t ne10_img_vresize_linear_mask_residual_table[NE10_VRESIZE_LINEAR_MASK_TABLE_SIZE] \
asm ("ne10_vresize_mask_residual_table");

#endif

