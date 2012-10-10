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
