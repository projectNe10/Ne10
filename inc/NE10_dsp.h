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
 * NE10 Library : inc/NE10_dsp.h
 */


#include <NE10_types.h>

#ifndef NE10_MATH_H
#define NE10_MATH_H

#ifdef __cplusplus
extern "C" {
#endif

///////////////////////////
// function prototypes:
///////////////////////////

/* fft functions*/

/* function pointers*/
extern void (*ne10_radix4_butterfly_float)(ne10_float32_t *pDst,
                     ne10_float32_t *pSrc,
                     ne10_uint16_t N,
                     ne10_float32_t *pCoef);

extern void (*ne10_radix4_butterfly_inverse_float)(ne10_float32_t *pDst,
                     ne10_float32_t *pSrc,
                     ne10_uint16_t N,
                     ne10_float32_t *pCoef);

extern void (*ne10_rfft_float)(
                     const ne10_rfft_instance_f32_t * S,
                     ne10_float32_t * pSrc,
                     ne10_float32_t * pDst);
/* init functions*/
extern ne10_result_t ne10_cfft_radix4_init_float(ne10_cfft_radix4_instance_f32_t * S,
                     ne10_uint16_t fftLen,
                     ne10_uint8_t ifftFlag);

extern ne10_result_t ne10_rfft_init_float(ne10_rfft_instance_f32_t * S,
                     ne10_cfft_radix4_instance_f32_t * S_CFFT,
                     ne10_uint32_t fftLen,
                     ne10_uint32_t ifftFlagR);
/* C version*/
extern void ne10_radix4_butterfly_float_c(ne10_float32_t *pDst,
                     ne10_float32_t *pSrc,
                     ne10_uint16_t N,
                     ne10_float32_t *pCoef);

extern void ne10_radix4_butterfly_inverse_float_c(ne10_float32_t *pDst,
                     ne10_float32_t *pSrc,
                     ne10_uint16_t N,
                     ne10_float32_t *pCoef);

extern void ne10_rfft_float_c(
                     const ne10_rfft_instance_f32_t * S,
                     ne10_float32_t * pSrc,
                     ne10_float32_t * pDst);


/* NEON version*/
extern void ne10_radix4_butterfly_float_neon(ne10_float32_t *pDst,
                     ne10_float32_t *pSrc,
                     ne10_uint16_t N,
                     ne10_float32_t *pCoef);

extern void ne10_radix4_butterfly_inverse_float_neon(ne10_float32_t *pDst,
                     ne10_float32_t *pSrc,
                     ne10_uint16_t N,
                     ne10_float32_t *pCoef);

extern void ne10_rfft_float_neon(
                     const ne10_rfft_instance_f32_t * S,
                     ne10_float32_t * pSrc,
                     ne10_float32_t * pDst);

#ifdef __cplusplus
}
#endif

#endif
