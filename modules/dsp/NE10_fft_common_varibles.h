/*
 *  Copyright 2014 ARM Limited
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
 * NE10 Library : dsp/NE10_fft_common_varibles.h
 */

#include <NE10_types.h>

#ifndef NE10_FFT_COMMON_VARIBLES_H
#define NE10_FFT_COMMON_VARIBLES_H

///////////////////////////
// common varibles
///////////////////////////

/* Twiddles used in Radix-8 FFT */
const static ne10_float32_t TW_81_F32  =  0.70710678; // sqrt (2) / 2
const static ne10_float32_t TW_81N_F32 = -0.70710678; // - TW_81_F32

/* Twiddles used in Radix-5 FFT */
const static ne10_fft_cpx_float32_t TW_5A_F32 =
        {
             0.309016994374947, //   cos (2 * pi / 5)
            -0.951056516295154  // - sin (2 * pi / 5)
        };
const static ne10_fft_cpx_float32_t TW_5B_F32 =
        {
            -0.809016994374947, //   cos (4 * pi / 5)
            -0.587785252292473  // - sin (4 * pi / 5)
        };

/* Twiddles used in Radix-3 FFT */
const static ne10_float32_t TW_3I_F32  =   0.866025403784439; // sqrt (3) / 2
const static ne10_float32_t TW_3IN_F32 = - 0.866025403784439; // - TW_3IN_F32

#endif // NE10_FFT_COMMON_VARIBLES_H
