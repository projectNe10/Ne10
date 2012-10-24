/*
 *  Copyright 2012 ARM Limited
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

#include <stdio.h>

#include "NE10_dsp.h"

ne10_result_t ne10_init_dsp (ne10_int32_t is_NEON_available)
{
    if (NE10_OK == is_NEON_available)
    {
        ne10_radix4_butterfly_float = ne10_radix4_butterfly_float_neon;
        ne10_radix4_butterfly_inverse_float = ne10_radix4_butterfly_inverse_float_neon;
        ne10_rfft_float = ne10_rfft_float_neon;
    }
    else
    {
        ne10_radix4_butterfly_float = ne10_radix4_butterfly_float_c;
        ne10_radix4_butterfly_inverse_float = ne10_radix4_butterfly_inverse_float_c;
        ne10_rfft_float = ne10_rfft_float_c;
    }
    return NE10_OK;
}

// These are actual definitions of our function pointers that are declared in inc/NE10_dsp.h
void (*ne10_radix4_butterfly_float)(ne10_float32_t *pDst,
                     ne10_float32_t *pSrc,
                     ne10_uint16_t N,
                     ne10_float32_t *pCoef);

void (*ne10_radix4_butterfly_inverse_float)(ne10_float32_t *pDst,
                     ne10_float32_t *pSrc,
                     ne10_uint16_t N,
                     ne10_float32_t *pCoef);

void (*ne10_rfft_float)(
                     const ne10_rfft_instance_f32_t * S,
                     ne10_float32_t * pSrc,
                     ne10_float32_t * pDst,
                     ne10_float32_t * pTemp);
