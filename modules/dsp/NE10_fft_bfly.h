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

/*
 * NE10 Library : dsp/NE10_fft_bfly.h
 */

#include "NE10_types.h"
#include "NE10_fft_cplx_ops.h"
#include "NE10_fft_common_varibles.h"

#ifndef NE10_FFT_BFLY_H
#define NE10_FFT_BFLY_H

// R2C FFT size==4
// In[4]    R[0],R[1],R[2],R[3]
// OUT[4]   R[0],R[1],I[1],R[2]
#define NE10_FFT_R2C_4R_RCR(OUT,IN) \
    do { \
        ne10_float32_t SCRATCH [2]; \
        SCRATCH[0] = IN[0] + IN[2]; \
        SCRATCH[1] = IN[1] + IN[3]; \
        OUT[0] = SCRATCH[0] + SCRATCH[1]; \
        OUT[1] = IN[0] - IN[2]; \
        OUT[2] = IN[3] - IN[1]; \
        OUT[3] = SCRATCH[0] - SCRATCH[1]; \
    } while (0)

// C2R FFT size==4  - inversed of R2C FFT
// In[4]    R[0],R[1],I[1],R[2]
// OUT[4]   R[0],R[1],R[2],R[3]
#define NE10_FFT_C2R_RCR_4R(OUT,IN) \
    do { \
        ne10_float32_t SCRATCH [4]; \
        SCRATCH[0] =(IN[0] + IN[3]); \
        SCRATCH[1] =(IN[0] - IN[3]); \
        SCRATCH[2] = IN[1] + IN[1]; \
        SCRATCH[3] = IN[2] + IN[2]; \
        OUT[0] = SCRATCH[0] + SCRATCH[2]; \
        OUT[1] = SCRATCH[1] - SCRATCH[3]; \
        OUT[2] = SCRATCH[0] - SCRATCH[2]; \
        OUT[3] = SCRATCH[1] + SCRATCH[3]; \
    } while (0)

// R2C FFT size==4
// In[4]    R[0],R[1],R[2],R[3]
// OUT[4]   R[0],I[0],R[1],I[1]
#define NE10_FFT_R2C_4R_CC(OUT,IN) \
    do { \
        ne10_float32_t SCRATCH [2]; \
        ne10_float32_t TMP [2]; \
        SCRATCH[0] = (IN[3] - IN[1]) * TW_81N_F32; \
        SCRATCH[1] = (IN[3] + IN[1]) * TW_81N_F32; \
        OUT[0] = IN[0] + SCRATCH[0]; \
        OUT[2] = IN[0] - SCRATCH[0]; \
        OUT[1] = SCRATCH[1] - IN[2]; \
        OUT[3] = SCRATCH[1] + IN[2]; \
    } while (0)

// C2R FFT size==4  - inversed of R2C FFT
// In[4]    R[0],I[0],R[1],I[1]
// OUT[4]   R[0],R[1],R[2],R[3]
#define NE10_FFT_C2R_CC_4R(OUT,IN) \
    do { \
        ne10_float32_t SCRATCH [4]; \
        OUT[0] = ( IN[0] + IN[2]); \
        OUT[2] = (-IN[1] + IN[3]); \
        OUT[0] = OUT[0] + OUT[0]; \
        OUT[2] = OUT[2] + OUT[2]; \
        SCRATCH[0] = (IN[0] - IN[2]); \
        SCRATCH[1] = (IN[1] + IN[3]); \
        SCRATCH[2] = (SCRATCH[0] + SCRATCH[1]); \
        SCRATCH[3] = (SCRATCH[0] - SCRATCH[1]); \
        OUT[3] = SCRATCH[2] / TW_81N_F32; \
        OUT[1] = SCRATCH[3] / TW_81_F32; \
    } while (0)

// R2C FFT size==4
// In[4]    R[0],I[0],R[1],I[1]
// OUT[4]   R[0],I[0],R[1],I[1]
#define NE10_FFT_R2C_CC_CC(OUT,IN) \
    do { \
        ne10_fft_cpx_float32_t TMP[4]; \
        ne10_float32_t TMP_SWAP; \
        NE10_CPX_ADD (TMP[0], IN[0], IN[2]); \
        NE10_CPX_SUB (TMP[1], IN[0], IN[2]); \
        NE10_CPX_ADD (TMP[2], IN[1], IN[3]); \
        NE10_CPX_SUB (TMP[3], IN[1], IN[3]); \
        TMP_SWAP = TMP[3].i; \
        TMP[3].i = - TMP[3].r; \
        TMP[3].r = TMP_SWAP; \
        OUT[0].r = TMP[0].r + TMP[2].r; \
        OUT[0].i = TMP[0].i + TMP[2].i; \
        OUT[2].r =   TMP[0].r - TMP[2].r; \
        OUT[2].i = -(TMP[0].i - TMP[2].i); \
        OUT[1].r = TMP[1].r + TMP[3].r; \
        OUT[1].i = TMP[1].i + TMP[3].i; \
        OUT[3].r =   TMP[1].r - TMP[3].r; \
        OUT[3].i = -(TMP[1].i - TMP[3].i); \
    } while (0)

// C2R FFT size==4  - inversed of R2C FFT
// In[4]    R[0],I[0],R[1],I[1]
// OUT[4]   R[0],I[0],R[1],I[1]
#define NE10_FFT_C2R_CC_CC(OUT,IN) \
    do { \
        ne10_fft_cpx_float32_t SCRATCH[4]; \
        SCRATCH[0].r = (IN[0].r + IN[1].r); \
        SCRATCH[2].r = (IN[0].r - IN[1].r); \
        SCRATCH[2].i = (IN[0].i + IN[1].i); \
        SCRATCH[0].i = (IN[0].i - IN[1].i); \
        SCRATCH[1].r = (IN[2].r + IN[3].r); \
        SCRATCH[3].i = (IN[2].r - IN[3].r); \
        SCRATCH[3].r = (IN[2].i + IN[3].i) * -1.0f; \
        SCRATCH[1].i = (IN[2].i - IN[3].i); \
        OUT[0].r = (SCRATCH[0].r + SCRATCH[1].r); \
        OUT[2].r = (SCRATCH[0].r - SCRATCH[1].r); \
        OUT[0].i = (SCRATCH[0].i + SCRATCH[1].i); \
        OUT[2].i = (SCRATCH[0].i - SCRATCH[1].i); \
        OUT[1].r = (SCRATCH[2].r + SCRATCH[3].r); \
        OUT[3].r = (SCRATCH[2].r - SCRATCH[3].r); \
        OUT[1].i = (SCRATCH[2].i + SCRATCH[3].i); \
        OUT[3].i = (SCRATCH[2].i - SCRATCH[3].i); \
    } while (0)

#endif // NE10_FFT_BFLY_H
