/*
 *  Copyright 2014-15 ARM Limited and Contributors.
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

/* license of Kiss FFT */
/*
Copyright (c) 2003-2010, Mark Borgerding

All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
    * Neither the author nor the names of any contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/*
 * NE10 Library : dsp/NE10_fft_cplx_ops.h
 */

#include "NE10_types.h"

#ifndef NE10_FFT_CPLX_OPS_H
#define NE10_FFT_CPLX_OPS_H

#ifdef __cplusplus
template<class T>
static inline void ne10_swap_ptr(T *&ptr_a, T *&ptr_b)
{
    ptr_a = (T *)((intptr_t)(ptr_a) ^ (intptr_t)(ptr_b));
    ptr_b = (T *)((intptr_t)(ptr_a) ^ (intptr_t)(ptr_b));
    ptr_a = (T *)((intptr_t)(ptr_a) ^ (intptr_t)(ptr_b));
}
#else
#define ne10_swap_ptr(X,Y) \
    do { \
        X = (void *)((intptr_t)(X) ^ (intptr_t)(Y)); \
        Y = (void *)((intptr_t)(X) ^ (intptr_t)(Y)); \
        X = (void *)((intptr_t)(X) ^ (intptr_t)(Y)); \
    } while (0)
#endif

// Multiply scalar X by scalar Y
#define NE10_S_MUL(X,Y) ((X) * (Y))

#define NE10_CPX_ADD(Z,A,B) \
    do { \
        Z.r = A.r + B.r; \
        Z.i = A.i + B.i; \
    } while (0)

#define NE10_CPX_SUB(Z,A,B) \
    do { \
        Z.r = A.r - B.r; \
        Z.i = A.i - B.i; \
    } while (0)

#define NE10_CPX_ADDTO(Z,X) NE10_CPX_ADD(Z,X,Z)

#define NE10_CPX_MUL_F32(Z,A,B) \
    do { \
        ne10_float32_t ARBR = A.r * B.r; \
        ne10_float32_t AIBI = A.i * B.i; \
        ne10_float32_t ARBI = A.r * B.i; \
        ne10_float32_t AIBR = A.i * B.r; \
        Z.r = ARBR - AIBI; \
        Z.i = AIBR + ARBI; \
    } while (0)

#define NE10_CPX_CONJ_MUL_F32(Z,A,B) \
    do { \
        ne10_float32_t ARBR = A.r * B.r; \
        ne10_float32_t AIBI = A.i * B.i; \
        ne10_float32_t ARBI = A.r * B.i; \
        ne10_float32_t AIBR = A.i * B.r; \
        Z.r = ARBR + AIBI; \
        Z.i = AIBR - ARBI; \
    } while (0)

#define NE10_CPX_MUL_TW_F32(Z,TW) \
    do { \
        ne10_fft_cpx_float32_t tmp; \
        NE10_CPX_MUL(tmp,Z,TW); \
        Z = tmp; \
    } while (0)

#define NE10_CPX_MUL_INV_TW_F32(Z,TW) \
    do { \
        ne10_fft_cpx_float32_t tmp; \
        NE10_CPX_CONJ_MUL(tmp,Z,TW); \
        Z = tmp; \
    } while (0)

#endif // NE10_FFT_CPLX_OPS_H
