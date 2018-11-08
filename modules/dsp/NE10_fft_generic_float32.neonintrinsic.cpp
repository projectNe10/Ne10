/*
 *  Copyright 2014-16 ARM Limited and Contributors.
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
 * NE10 Library : dsp/NE10_fft_generic_float32.neonintrisic.cpp
 *
 * This file must be compiled by C++ toolchain because some functions are
 * written as template functions to make it easier for compiler to
 * reduce branch jump.
 */

#include "NE10_types.h"
#include "NE10_macros.h"
#include "NE10_fft.neonintrinsic.h"
#include "NE10_fft_generic_float32.h"
#include <stdlib.h>

typedef float32x4x2_t CPLX;
typedef float32x4_t   REAL;
#define NE10_REAL_DUP_NEON_F32 vdupq_n_f32
#define NE10_CPLX_LOAD(PTR) vld2q_f32 ((ne10_float32_t*) (PTR))
#define NE10_CPLX_STORE(PTR,OUT) \
    do { \
        vst2q_f32 ((ne10_float32_t*) (PTR), OUT); \
    } while (0)

static inline void NE10_LOAD_TW_AND_MUL (CPLX &scratch_in,
        const ne10_fft_cpx_float32_t *ptr_in)
{
    CPLX scratch_tw;
    float32x2_t d2_tmp = vld1_f32 ((ne10_float32_t *)ptr_in);
    scratch_tw.val[0] = NE10_REAL_DUP_NEON_F32 (d2_tmp[0]);
    scratch_tw.val[1] = NE10_REAL_DUP_NEON_F32 (d2_tmp[1]);
    NE10_CPX_MUL_NEON_F32 (scratch_in, scratch_in, scratch_tw);
}

static inline REAL NE10_S_MUL_NEON_F32 (const REAL vec,
        const ne10_float32_t scalar)
{
    REAL scalar_neon = NE10_REAL_DUP_NEON_F32 (scalar);
    REAL result = scalar_neon * vec;
    return result;
}

static inline REAL NE10_S_MLA_NEON_F32 (const REAL dst,
        const REAL src,
        const ne10_float32_t scalar)
{
    REAL scalar_neon = NE10_REAL_DUP_NEON_F32 (scalar);
    return vmlaq_f32 (dst, src, scalar_neon);
}

static inline REAL NE10_S_MLS_NEON_F32 (const REAL dst,
        const REAL src,
        const ne10_float32_t scalar)
{
    REAL scalar_neon = NE10_REAL_DUP_NEON_F32 (scalar);
    return vmlsq_f32 (dst, src, scalar_neon);
}

///////////////////////////////
// Multiply input with twiddles
///////////////////////////////
static inline void NE10_FFT2_MUL_TW_NEON (CPLX scratch_out[2],
        const CPLX scratch_in[2],
        const CPLX scratch_tw[1])
{
    scratch_out[0] = scratch_in[0];
    NE10_CPX_MUL_NEON_F32 (scratch_out[1], scratch_in[1], scratch_tw[0]);
}

static inline void NE10_FFT3_MUL_TW_NEON (CPLX scratch_out[3],
        const CPLX scratch_in[3],
        const CPLX scratch_tw[2])
{
    NE10_FFT2_MUL_TW_NEON (scratch_out, scratch_in, scratch_tw);
    NE10_CPX_MUL_NEON_F32 (scratch_out[2], scratch_in[2], scratch_tw[1]);
}

static inline void NE10_FFT4_MUL_TW_NEON (CPLX scratch_out[4],
        const CPLX scratch_in[4],
        const CPLX scratch_tw[3])
{
    NE10_FFT3_MUL_TW_NEON (scratch_out, scratch_in, scratch_tw);
    NE10_CPX_MUL_NEON_F32 (scratch_out[3], scratch_in[3], scratch_tw[2]);
}

static inline void NE10_FFT5_MUL_TW_NEON (CPLX scratch_out[5],
        const CPLX scratch_in[5],
        const CPLX scratch_tw[4])
{
    NE10_FFT4_MUL_TW_NEON (scratch_out, scratch_in, scratch_tw);
    NE10_CPX_MUL_NEON_F32 (scratch_out[4], scratch_in[4], scratch_tw[3]);
}

////////////////
// Conj inplace.
////////////////
static inline void NE10_FFT2_CONJ (CPLX scratch_out[2])
{
    scratch_out[0].val[1] = -scratch_out[0].val[1];
    scratch_out[1].val[1] = -scratch_out[1].val[1];
}

static inline void NE10_FFT3_CONJ (CPLX scratch_out[3])
{
    NE10_FFT2_CONJ (scratch_out);
    scratch_out[2].val[1] = -scratch_out[2].val[1];
}

static inline void NE10_FFT4_CONJ (CPLX scratch_out[4])
{
    NE10_FFT3_CONJ (scratch_out);
    scratch_out[3].val[1] = -scratch_out[3].val[1];
}

static inline void NE10_FFT5_CONJ (CPLX scratch_out[5])
{
    NE10_FFT4_CONJ (scratch_out);
    scratch_out[4].val[1] = -scratch_out[4].val[1];
}

static inline void NE10_FFT8_CONJ (CPLX scratch_out[8])
{
    NE10_FFT5_CONJ (scratch_out);
    scratch_out[5].val[1] = -scratch_out[5].val[1];
    scratch_out[6].val[1] = -scratch_out[6].val[1];
    scratch_out[7].val[1] = -scratch_out[7].val[1];
}

/////////////////////////////////////////////////////////////////////////////
// Scaling
// If Macro NE10_DSP_CFFT_SCALING is not defined, these functions do nothing.
/////////////////////////////////////////////////////////////////////////////
static inline void NE10_FFT2_SCALING (CPLX scratch_out[2],
        const REAL one_by_fft_neon)
{
#ifdef NE10_DSP_CFFT_SCALING
    scratch_out[0].val[0] *= one_by_fft_neon;
    scratch_out[0].val[1] *= one_by_fft_neon;
    scratch_out[1].val[0] *= one_by_fft_neon;
    scratch_out[1].val[1] *= one_by_fft_neon;
#endif
}

static inline void NE10_FFT3_SCALING (CPLX scratch_out[3],
        const REAL one_by_fft_neon)
{
#ifdef NE10_DSP_CFFT_SCALING
    NE10_FFT2_SCALING (scratch_out, one_by_fft_neon);
    scratch_out[2].val[0] *= one_by_fft_neon;
    scratch_out[2].val[1] *= one_by_fft_neon;
#endif
}

static inline void NE10_FFT4_SCALING (CPLX scratch_out[4],
        const REAL one_by_fft_neon)
{
#ifdef NE10_DSP_CFFT_SCALING
    NE10_FFT3_SCALING (scratch_out, one_by_fft_neon);
    scratch_out[3].val[0] *= one_by_fft_neon;
    scratch_out[3].val[1] *= one_by_fft_neon;
#endif
}

static inline void NE10_FFT5_SCALING (CPLX scratch_out[5],
        const REAL one_by_fft_neon)
{
#ifdef NE10_DSP_CFFT_SCALING
    NE10_FFT4_SCALING (scratch_out, one_by_fft_neon);
    scratch_out[4].val[0] *= one_by_fft_neon;
    scratch_out[4].val[1] *= one_by_fft_neon;
#endif
}

static inline void NE10_FFT8_SCALING (CPLX scratch_out[8],
        const REAL one_by_fft_neon)
{
#ifdef NE10_DSP_CFFT_SCALING
    NE10_FFT5_SCALING (scratch_out, one_by_fft_neon);
    scratch_out[5].val[0] *= one_by_fft_neon;
    scratch_out[5].val[1] *= one_by_fft_neon;
    scratch_out[6].val[0] *= one_by_fft_neon;
    scratch_out[6].val[1] *= one_by_fft_neon;
    scratch_out[7].val[0] *= one_by_fft_neon;
    scratch_out[7].val[1] *= one_by_fft_neon;
#endif
}

///////////////////
// FFT Kernel
// F: Forward
// C: Complex
// U: Unscaled
//////////////////
static inline void NE10_FFT2_FUC_NEON_F32 (CPLX scratch_out[2],
        const CPLX scratch_in[2])
{
    NE10_CPX_ADD_NEON_F32 (scratch_out[0], scratch_in[0], scratch_in[1]);
    NE10_CPX_SUB_NEON_F32 (scratch_out[1], scratch_in[0], scratch_in[1]);
}

static inline void NE10_FFT3_FUC_NEON_F32 (CPLX Fout[3],
        const CPLX Fin[3])
{
    const float32x4_t TW_3IN_NEON_F32 = vdupq_n_f32 (TW_3IN_F32);
    const float32x4_t HALF_NEON_F32 = vdupq_n_f32 (0.5f);

    NE10_CPX_ADD_NEON_F32 (Fout[2], Fin[1], Fin[2]);
    NE10_CPX_SUB_NEON_F32 (Fout[0], Fin[1], Fin[2]);

    Fout[1].val[0] = Fin[0].val[0] - Fout[2].val[0] * HALF_NEON_F32;
    Fout[1].val[1] = Fin[0].val[1] - Fout[2].val[1] * HALF_NEON_F32;

    Fout[0].val[0] = Fout[0].val[0] * TW_3IN_NEON_F32;
    Fout[0].val[1] = Fout[0].val[1] * TW_3IN_NEON_F32;
}

static inline void NE10_FFT4_FUC_NEON_F32 (CPLX scratch_out[4],
        const CPLX scratch_in[4])
{
    CPLX scratch[4];

    NE10_CPX_ADD_NEON_F32 (scratch[0], scratch_in[0], scratch_in[2]);
    NE10_CPX_SUB_NEON_F32 (scratch[1], scratch_in[0], scratch_in[2]);
    NE10_CPX_ADD_NEON_F32 (scratch[2], scratch_in[1], scratch_in[3]);
    NE10_CPX_SUB_NEON_F32 (scratch[3], scratch_in[1], scratch_in[3]);

    NE10_CPX_SUB_NEON_F32 (scratch_out[2], scratch[0], scratch[2]);
    NE10_CPX_ADD_NEON_F32 (scratch_out[0], scratch[0], scratch[2]);

    scratch_out[1].val[0] = scratch[1].val[0] + scratch[3].val[1];
    scratch_out[1].val[1] = scratch[1].val[1] - scratch[3].val[0];
    scratch_out[3].val[0] = scratch[1].val[0] - scratch[3].val[1];
    scratch_out[3].val[1] = scratch[1].val[1] + scratch[3].val[0];
}

static inline void NE10_FFT4_FUC_INPLACE_NEON_F32 (CPLX scratch_out[4])
{
    CPLX scratch[4];

    NE10_CPX_ADD_NEON_F32 (scratch[0], scratch_out[0], scratch_out[2]);
    NE10_CPX_SUB_NEON_F32 (scratch[1], scratch_out[0], scratch_out[2]);
    NE10_CPX_ADD_NEON_F32 (scratch[2], scratch_out[1], scratch_out[3]);
    NE10_CPX_SUB_NEON_F32 (scratch[3], scratch_out[1], scratch_out[3]);

    NE10_CPX_SUB_NEON_F32 (scratch_out[2], scratch[0], scratch[2]);
    NE10_CPX_ADD_NEON_F32 (scratch_out[0], scratch[0], scratch[2]);

    scratch_out[1].val[0] = scratch[1].val[0] + scratch[3].val[1];
    scratch_out[1].val[1] = scratch[1].val[1] - scratch[3].val[0];
    scratch_out[3].val[0] = scratch[1].val[0] - scratch[3].val[1];
    scratch_out[3].val[1] = scratch[1].val[1] + scratch[3].val[0];
}

static inline void NE10_FFT5_FUC_INPLACE_NEON_F32 (CPLX Fout[5])
{
    CPLX s[6];

    NE10_CPX_ADD_NEON_F32 (s[1], Fout[1], Fout[4]);
    NE10_CPX_ADD_NEON_F32 (s[2], Fout[2], Fout[3]);

    s[0] = Fout[0];
    s[5] = Fout[0];

    Fout[0].val[0] = Fout[0].val[0] + s[1].val[0] + s[2].val[0];
    Fout[0].val[1] = Fout[0].val[1] + s[1].val[1] + s[2].val[1];

    s[0].val[0] = NE10_S_MLA_NEON_F32 (s[0].val[0], s[1].val[0], TW_5A_F32.r);
    s[0].val[1] = NE10_S_MLA_NEON_F32 (s[0].val[1], s[1].val[1], TW_5A_F32.r);
    s[5].val[0] = NE10_S_MLA_NEON_F32 (s[5].val[0], s[1].val[0], TW_5B_F32.r);
    s[5].val[1] = NE10_S_MLA_NEON_F32 (s[5].val[1], s[1].val[1], TW_5B_F32.r);

    s[0].val[0] = NE10_S_MLA_NEON_F32 (s[0].val[0], s[2].val[0], TW_5B_F32.r);
    s[0].val[1] = NE10_S_MLA_NEON_F32 (s[0].val[1], s[2].val[1], TW_5B_F32.r);
    s[5].val[0] = NE10_S_MLA_NEON_F32 (s[5].val[0], s[2].val[0], TW_5A_F32.r);
    s[5].val[1] = NE10_S_MLA_NEON_F32 (s[5].val[1], s[2].val[1], TW_5A_F32.r);

    NE10_CPX_SUB_NEON_F32 (s[4], Fout[1], Fout[4]);
    NE10_CPX_SUB_NEON_F32 (s[3], Fout[2], Fout[3]);

    s[1].val[0] = NE10_S_MUL_NEON_F32 (s[4].val[1], TW_5A_F32.i);
    s[1].val[1] = -NE10_S_MUL_NEON_F32 (s[4].val[0], TW_5A_F32.i);
    s[2].val[0] = -NE10_S_MUL_NEON_F32 (s[4].val[1], TW_5B_F32.i);
    s[2].val[1] = NE10_S_MUL_NEON_F32 (s[4].val[0], TW_5B_F32.i);

    s[1].val[0] = NE10_S_MLA_NEON_F32 (s[1].val[0], s[3].val[1], TW_5B_F32.i);
    s[1].val[1] = NE10_S_MLS_NEON_F32 (s[1].val[1], s[3].val[0], TW_5B_F32.i);
    s[2].val[0] = NE10_S_MLA_NEON_F32 (s[2].val[0], s[3].val[1], TW_5A_F32.i);
    s[2].val[1] = NE10_S_MLS_NEON_F32 (s[2].val[1], s[3].val[0], TW_5A_F32.i);

    NE10_CPX_SUB_NEON_F32 (Fout[1], s[0], s[1]);
    NE10_CPX_ADD_NEON_F32 (Fout[4], s[0], s[1]);
    NE10_CPX_ADD_NEON_F32 (Fout[2], s[5], s[2]);
    NE10_CPX_SUB_NEON_F32 (Fout[3], s[5], s[2]);
}

#define NE10_BUTTERFLY_INDEX_NEON_F32(OUT,IN,OUT_I,OUT_J,IN_I,IN_J) \
    do { \
        NE10_CPX_ADD_NEON_F32 (OUT[OUT_I],IN[IN_I],IN[IN_J]); \
        NE10_CPX_SUB_NEON_F32 (OUT[OUT_J],IN[IN_I],IN[IN_J]); \
    } while (0)

static inline void NE10_FFT8_FUC_NEON_F32 (CPLX out[8],
        const CPLX in[8])
{
    CPLX s[8];
    const static ne10_fft_cpx_float32_t TW_8[4] =
    {
        {  1.00000,  0.00000 },
        {  0.70711, -0.70711 },
        {  0.00000, -1.00000 },
        { -0.70711, -0.70711 },
    };

    // STAGE - 1
    // in -> s
    {
        NE10_BUTTERFLY_INDEX_NEON_F32 (s, in, 0, 4, 0, 4);
        NE10_BUTTERFLY_INDEX_NEON_F32 (s, in, 1, 5, 1, 5);
        NE10_BUTTERFLY_INDEX_NEON_F32 (s, in, 2, 6, 2, 6);
        NE10_BUTTERFLY_INDEX_NEON_F32 (s, in, 3, 7, 3, 7);
    }

    // STAGE - 2
    // s -> out
    {
        // TW
#define NE10_CPX_MUL_TW8_NEON_F32(OUT,TW_8_TABLE,OUT_I,TW_J) \
        do { \
            ne10_fft_cpx_float32_t TW_TMP = TW_8_TABLE[TW_J]; \
            CPLX TW_TMP_NEON; \
            TW_TMP_NEON.val[0] = NE10_REAL_DUP_NEON_F32 (TW_TMP.r); \
            TW_TMP_NEON.val[1] = NE10_REAL_DUP_NEON_F32 (TW_TMP.i); \
            NE10_CPX_MUL_NEON_F32 (OUT[OUT_I],OUT[OUT_I],TW_TMP_NEON); \
        } while (0)

        NE10_CPX_MUL_TW8_NEON_F32 (s, TW_8, 4, 0);
        NE10_CPX_MUL_TW8_NEON_F32 (s, TW_8, 5, 1);
        NE10_CPX_MUL_TW8_NEON_F32 (s, TW_8, 6, 2);
        NE10_CPX_MUL_TW8_NEON_F32 (s, TW_8, 7, 3);

        NE10_BUTTERFLY_INDEX_NEON_F32 (out, s, 0, 2, 0, 2);
        NE10_BUTTERFLY_INDEX_NEON_F32 (out, s, 1, 3, 1, 3);
        NE10_BUTTERFLY_INDEX_NEON_F32 (out, s, 4, 6, 4, 6);
        NE10_BUTTERFLY_INDEX_NEON_F32 (out, s, 5, 7, 5, 7);
    }
    // STAGE - 3
    // out -> s
    {
        // TW
        NE10_CPX_MUL_TW8_NEON_F32 (out, TW_8, 2, 0);
        NE10_CPX_MUL_TW8_NEON_F32 (out, TW_8, 3, 2);
        NE10_CPX_MUL_TW8_NEON_F32 (out, TW_8, 6, 0);
        NE10_CPX_MUL_TW8_NEON_F32 (out, TW_8, 7, 2);
#undef NE10_CPX_MUL_TW8_NEON_F32

        NE10_BUTTERFLY_INDEX_NEON_F32 (s, out, 0, 4, 0, 1);
        NE10_BUTTERFLY_INDEX_NEON_F32 (s, out, 2, 6, 2, 3);
        NE10_BUTTERFLY_INDEX_NEON_F32 (s, out, 1, 5, 4, 5);
        NE10_BUTTERFLY_INDEX_NEON_F32 (s, out, 3, 7, 6, 7);
    }

    out[0] = s[0];
    out[1] = s[1];
    out[2] = s[2];
    out[3] = s[3];
    out[4] = s[4];
    out[5] = s[5];
    out[6] = s[6];
    out[7] = s[7];
}

////////////////////////////////////
// Following are butterfly functions
////////////////////////////////////
template<ne10_int32_t is_first_stage, ne10_int32_t is_inverse, bool is_scaled>
static void ne10_radix_2_butterfly_float32_neon (CPLX *Fout,
        const CPLX *Fin,
        const ne10_fft_cpx_float32_t *twiddles,
        const ne10_int32_t fstride,
        const ne10_int32_t out_step,
        const ne10_int32_t nfft)
{
    CPLX in[2];
    CPLX out[2];

    const ne10_int32_t in_step = nfft / 2;
    ne10_int32_t f_count;
    ne10_int32_t m_count;

    const REAL one_by_fft_neon = NE10_REAL_DUP_NEON_F32 (0.25 / nfft);

    for (f_count = fstride; f_count > 0; f_count--)
    {
        for (m_count = out_step; m_count > 0; m_count--)
        {
#ifndef NE10_INLINE_ASM_OPT
            in[0] = NE10_CPLX_LOAD (Fin + 0 * in_step);
            in[1] = NE10_CPLX_LOAD (Fin + 1 * in_step);

            if (is_inverse == 1)
            {
                NE10_FFT2_CONJ (in);
            }

            if (is_first_stage == 0)
            {
                NE10_LOAD_TW_AND_MUL (in[1], twiddles);
            }

            NE10_FFT2_FUC_NEON_F32 (out, in);

            if (is_inverse == 1)
            {
                NE10_FFT2_CONJ (out);

                if (is_scaled)
                {
                    NE10_FFT2_SCALING (out, one_by_fft_neon);
                }
            }

            NE10_CPLX_STORE (Fout + 0 * out_step, out[0]);
            NE10_CPLX_STORE (Fout + 1 * out_step, out[1]);
#else // NE10_INLINE_ASM_OPT
#ifndef __aarch64__
#error Currently, inline assembly optimizations are only available on AArch64.
#else // __aarch64__
            asm volatile (
                "ld2 {v0.4s, v1.4s}, [%[pin0]] \n\t"
                "ld2 {v2.4s, v3.4s}, [%[pin1]] \n\t"
            :
            : [pin0]"r"(Fin),
              [pin1]"r"(Fin + in_step)
            : "memory", "v0", "v1", "v2", "v3");

            if (is_inverse == 1)
            {
                asm volatile (
                    "fneg v1.4s, v1.4s \n\t"
                    "fneg v3.4s, v3.4s \n\t"
                :
                :
                : "v0", "v1", "v2", "v3");
            }

            if (is_first_stage == 0)
            {
                asm volatile (
                    "ld1 {v4.1d}, [%[ptw]] \n\t" // tw0

                    "fmul v14.4s,  v2.4s,  v4.s[1] \n\t" // RI
                    "fmul  v2.4s,  v2.4s,  v4.s[0] \n\t" // RR
                    "fmls  v2.4s,  v3.4s,  v4.s[1] \n\t" // RR - II
                    "fmul v15.4s,  v3.4s,  v4.s[0] \n\t" // IR
                    "fadd  v3.4s, v14.4s, v15.4s \n\t" // RI + IR
                :
                : [ptw]"r"(twiddles)
                : "memory", "v0", "v1", "v2", "v3", "v4", "v14", "v15");
            }

            asm volatile (
                "fadd  v4.4s, v0.4s, v2.4s \n\t"
                "fadd  v5.4s, v1.4s, v3.4s \n\t"
                "fsub  v6.4s, v0.4s, v2.4s \n\t"
                "fsub  v7.4s, v1.4s, v3.4s \n\t"
            :
            :
            : "memory",
              "v0", "v1", "v2", "v3", // in
              "v4", "v5", "v6", "v7"); // out

            if (is_inverse == 1)
            {
                asm volatile (
                    "fneg v5.4s, v5.4s \n\t"
                    "fneg v7.4s, v7.4s \n\t"
                :
                :
                : "v4", "v5", "v6", "v7");
            }

            if (is_scaled == 1)
            {
                asm volatile (
                    "fmul v4.4s, v4.4s, %[one_by_nfft].4s \n\t"
                    "fmul v5.4s, v5.4s, %[one_by_nfft].4s \n\t"
                    "fmul v6.4s, v6.4s, %[one_by_nfft].4s \n\t"
                    "fmul v7.4s, v7.4s, %[one_by_nfft].4s \n\t"
                :
                : [one_by_nfft]"w"(one_by_fft_neon)
                : "v4", "v5", "v6", "v7");
            }

            asm volatile (
                "st2 {v4.4s, v5.4s}, [%[pout0]] \n\t"
                "st2 {v6.4s, v7.4s}, [%[pout1]] \n\t"
            :
            : [pout0]"r"(Fout),
              [pout1]"r"(Fout + out_step)
            : "memory", "v4", "v5", "v6", "v7");
#endif // __aarch64__
#endif // NE10_INLINE_ASM_OPT

            Fin++;

            if (is_first_stage == 0)
            {
                Fout++;
                twiddles++;
            }
            else
            {
                Fout += 2;
            }
        }
        if (is_first_stage == 0)
        {
            twiddles -= out_step;
            Fout += (2 - 1) * out_step;
        }
    }
}
template<ne10_int32_t is_first_stage, ne10_int32_t is_inverse, bool is_scaled>
static void ne10_radix_4_butterfly_float32_neon (CPLX *Fout,
        const CPLX *Fin,
        const ne10_fft_cpx_float32_t *twiddles,
        const ne10_int32_t fstride,
        const ne10_int32_t out_step,
        const ne10_int32_t nfft)
{
    CPLX in[4];
    #ifdef NE10_INLINE_ASM_OPT
    CPLX s[4];
    #endif

    const ne10_int32_t in_step = nfft / 4;
    ne10_int32_t f_count;
    ne10_int32_t m_count;

    const REAL one_by_fft_neon = NE10_REAL_DUP_NEON_F32 (0.25 / nfft);

    for (f_count = fstride; f_count > 0; f_count--)
    {
        for (m_count = out_step; m_count > 0; m_count--)
        {
#ifndef NE10_INLINE_ASM_OPT
            in[0] = NE10_CPLX_LOAD (Fin + 0 * in_step);
            in[1] = NE10_CPLX_LOAD (Fin + 1 * in_step);
            in[2] = NE10_CPLX_LOAD (Fin + 2 * in_step);
            in[3] = NE10_CPLX_LOAD (Fin + 3 * in_step);

            if (is_inverse == 1)
            {
                NE10_FFT4_CONJ (in);
            }

            if (is_first_stage == 0)
            {
                NE10_LOAD_TW_AND_MUL (in[1], twiddles);
                NE10_LOAD_TW_AND_MUL (in[2], twiddles + out_step);
                NE10_LOAD_TW_AND_MUL (in[3], twiddles + out_step * 2);
            }

            NE10_FFT4_FUC_INPLACE_NEON_F32 (in);

            if (is_inverse == 1)
            {
                NE10_FFT4_CONJ (in);
            }
            if (is_scaled)
            {
                NE10_FFT4_SCALING (in, one_by_fft_neon);
            }

            NE10_CPLX_STORE (Fout + 0 * out_step, in[0]);
            NE10_CPLX_STORE (Fout + 1 * out_step, in[1]);
            NE10_CPLX_STORE (Fout + 2 * out_step, in[2]);
            NE10_CPLX_STORE (Fout + 3 * out_step, in[3]);
#else // NE10_INLINE_ASM_OPT
#ifndef __aarch64__
#error Currently, inline assembly optimizations are only available on AArch64.
#else // __aarch64__
#define NEON_REGISTERS_C2C_R4 \
            "v0", "v1", \
            "v2", "v3", \
            "v4", "v5", \
            "v6", "v7"
#define NEON_REGISTERS_C2C_TW_R4 \
            "v8", "v9", \
            "v10", "v11", \
            "v12", "v13"

            asm volatile (
                "ld2 {v0.4s, v1.4s}, [%[pin0]] \n\t" // in[0]
                "ld2 {v2.4s, v3.4s}, [%[pin1]] \n\t" // in[1]
                "ld2 {v4.4s, v5.4s}, [%[pin2]] \n\t" // in[2]
                "ld2 {v6.4s, v7.4s}, [%[pin3]] \n\t" // in[3]
            :
            : [pin0]"r"(Fin),
              [pin1]"r"(Fin + in_step),
              [pin2]"r"(Fin + in_step * 2),
              [pin3]"r"(Fin + in_step * 3)
            : "memory", NEON_REGISTERS_C2C_R4);

            if (is_inverse == 1)
            {
                asm volatile (
                    "fneg v1.4s, v1.4s \n\t"
                    "fneg v3.4s, v3.4s \n\t"
                    "fneg v5.4s, v5.4s \n\t"
                    "fneg v7.4s, v7.4s \n\t"
                :
                :
                : NEON_REGISTERS_C2C_R4);
            }

            if (is_first_stage == 0)
            {
                asm volatile (
                    "ld1 { v8.1d}, [%[ptw0]] \n\t" // tw0
                    "ld1 { v9.1d}, [%[ptw1]] \n\t" // tw1
                    "ld1 {v10.1d}, [%[ptw2]] \n\t" // tw2

                    "fmul v14.4s,  v2.4s,  v8.s[1] \n\t" // RI
                    "fmul  v2.4s,  v2.4s,  v8.s[0] \n\t" // RR
                    "fmls  v2.4s,  v3.4s,  v8.s[1] \n\t" // RR - II
                    "fmul v15.4s,  v3.4s,  v8.s[0] \n\t" // IR
                    "fadd  v3.4s, v14.4s, v15.4s \n\t" // RI + IR

                    "fmul v14.4s,  v4.4s, v9.s[1] \n\t" // RI
                    "fmul  v4.4s,  v4.4s, v9.s[0] \n\t" // RR
                    "fmls  v4.4s,  v5.4s, v9.s[1] \n\t" // RR - II
                    "fmul v15.4s,  v5.4s, v9.s[0] \n\t" // IR
                    "fadd  v5.4s, v14.4s, v15.4s \n\t" // RI + IR

                    "fmul v14.4s,  v6.4s, v10.s[1] \n\t" // RI
                    "fmul  v6.4s,  v6.4s, v10.s[0] \n\t" // RR
                    "fmls  v6.4s,  v7.4s, v10.s[1] \n\t" // RR - II
                    "fmul v15.4s,  v7.4s, v10.s[0] \n\t" // IR
                    "fadd  v7.4s, v14.4s, v15.4s \n\t" // RI + IR
                :
                : [ptw0]"r"(twiddles),
                  [ptw1]"r"(twiddles + out_step),
                  [ptw2]"r"(twiddles + out_step * 2)
                : "memory", NEON_REGISTERS_C2C_R4,
                  NEON_REGISTERS_C2C_TW_R4, "v14", "v15");
            }

            asm volatile (
                "fadd %[s0r].4s, v0.4s, v4.4s \n\t"
                "fadd %[s0i].4s, v1.4s, v5.4s \n\t"

                "fsub %[s1r].4s, v0.4s, v4.4s \n\t"
                "fsub %[s1i].4s, v1.4s, v5.4s \n\t"

                "fadd %[s2r].4s, v2.4s, v6.4s \n\t"
                "fadd %[s2i].4s, v3.4s, v7.4s \n\t"

                "fsub %[s3r].4s, v2.4s, v6.4s \n\t"
                "fsub %[s3i].4s, v3.4s, v7.4s \n\t"
            : [s0r]"+w"(s[0].val[0]),
              [s0i]"+w"(s[0].val[1]),
              [s1r]"+w"(s[1].val[0]),
              [s1i]"+w"(s[1].val[1]),
              [s2r]"+w"(s[2].val[0]),
              [s2i]"+w"(s[2].val[1]),
              [s3r]"+w"(s[3].val[0]),
              [s3i]"+w"(s[3].val[1])
            :
            : NEON_REGISTERS_C2C_R4);

            asm volatile (
                "fadd v0.4s, %[s0r].4s, %[s2r].4s \n\t"
                "fadd v1.4s, %[s0i].4s, %[s2i].4s \n\t"
                "fsub v4.4s, %[s0r].4s, %[s2r].4s \n\t"
                "fsub v5.4s, %[s0i].4s, %[s2i].4s \n\t"

                "fadd v2.4s, %[s1r].4s, %[s3i].4s \n\t"
                "fsub v3.4s, %[s1i].4s, %[s3r].4s \n\t"
                "fsub v6.4s, %[s1r].4s, %[s3i].4s \n\t"
                "fadd v7.4s, %[s1i].4s, %[s3r].4s \n\t"
            :
            : [s0r]"w"(s[0].val[0]),
              [s0i]"w"(s[0].val[1]),
              [s1r]"w"(s[1].val[0]),
              [s1i]"w"(s[1].val[1]),
              [s2r]"w"(s[2].val[0]),
              [s2i]"w"(s[2].val[1]),
              [s3r]"w"(s[3].val[0]),
              [s3i]"w"(s[3].val[1])
            : NEON_REGISTERS_C2C_R4);

            if (is_inverse == 1)
            {
                asm volatile (
                    "fneg v1.4s, v1.4s \n\t"
                    "fneg v3.4s, v3.4s \n\t"
                    "fneg v5.4s, v5.4s \n\t"
                    "fneg v7.4s, v7.4s \n\t"
                :
                :
                : NEON_REGISTERS_C2C_R4);
            }

            if (is_scaled)
            {
                asm volatile (
                    "fmul v0.4s, v0.4s, %[one_by_nfft].4s \n\t"
                    "fmul v1.4s, v1.4s, %[one_by_nfft].4s \n\t"
                    "fmul v2.4s, v2.4s, %[one_by_nfft].4s \n\t"
                    "fmul v3.4s, v3.4s, %[one_by_nfft].4s \n\t"
                    "fmul v4.4s, v4.4s, %[one_by_nfft].4s \n\t"
                    "fmul v5.4s, v5.4s, %[one_by_nfft].4s \n\t"
                    "fmul v6.4s, v6.4s, %[one_by_nfft].4s \n\t"
                    "fmul v7.4s, v7.4s, %[one_by_nfft].4s \n\t"
                :
                : [one_by_nfft]"w"(one_by_fft_neon)
                : NEON_REGISTERS_C2C_R4);
            }

            asm volatile (
                "st2 {v0.4s, v1.4s}, [%[pout0]] \n\t"
                "st2 {v2.4s, v3.4s}, [%[pout1]] \n\t"
                "st2 {v4.4s, v5.4s}, [%[pout2]] \n\t"
                "st2 {v6.4s, v7.4s}, [%[pout3]] \n\t"
            :
            : [pout0]"r"(Fout),
              [pout1]"r"(Fout + out_step),
              [pout2]"r"(Fout + out_step * 2),
              [pout3]"r"(Fout + out_step * 3)
            : NEON_REGISTERS_C2C_R4);
#endif // __aarch64__
#endif // NE10_INLINE_ASM_OPT
            Fin++;

            if (is_first_stage == 0)
            {
                Fout++;
                twiddles++;
            }
            else
            {
                Fout += 4;
            }
        }
        if (is_first_stage == 0)
        {
            twiddles -= out_step;
            Fout += (4 - 1) * out_step;
        }
    }
}

template<ne10_int32_t is_first_stage, ne10_int32_t is_inverse, bool is_scaled>
static void ne10_radix_3_butterfly_float32_neon (CPLX *Fout,
        const CPLX *Fin,
        const ne10_fft_cpx_float32_t *twiddles,
        const ne10_int32_t fstride,
        const ne10_int32_t out_step,
        const ne10_int32_t nfft)
{
    CPLX in[3];
    CPLX out[3];
    CPLX s[4];

    const ne10_int32_t in_step = nfft / 3;
    ne10_int32_t f_count;
    ne10_int32_t m_count;

    const REAL one_by_fft_neon = NE10_REAL_DUP_NEON_F32 (0.25 / nfft);
    const float32x4_t TW_3IN_NEON_F32 = vdupq_n_f32 (TW_3IN_F32);
    const float32x4_t HALF_NEON_F32 = vdupq_n_f32 (0.5f);

    for (f_count = fstride; f_count > 0; f_count--)
    {
        for (m_count = out_step; m_count > 0; m_count--)
        {
#ifndef NE10_INLINE_ASM_OPT
            in[0] = NE10_CPLX_LOAD (Fin + 0 * in_step);
            in[1] = NE10_CPLX_LOAD (Fin + 1 * in_step);
            in[2] = NE10_CPLX_LOAD (Fin + 2 * in_step);

            if (is_inverse == 1)
            {
                NE10_FFT3_CONJ (in);
            }

            if (is_first_stage == 0)
            {
                NE10_LOAD_TW_AND_MUL (in[1], twiddles);
                NE10_LOAD_TW_AND_MUL (in[2], twiddles + out_step);
            }

            NE10_CPX_ADD_NEON_F32 (s[2], in[1], in[2]);
            NE10_CPX_SUB_NEON_F32 (s[0], in[1], in[2]);
            s[3] = in[0];

            s[1].val[0] = - s[2].val[0] * HALF_NEON_F32;
            s[1].val[1] = - s[2].val[1] * HALF_NEON_F32;

            s[1].val[0] += s[3].val[0];
            s[1].val[1] += s[3].val[1];
            s[0].val[0] *= TW_3IN_NEON_F32;
            s[0].val[1] *= TW_3IN_NEON_F32;

            out[0].val[0] = s[3].val[0] + s[2].val[0];
            out[0].val[1] = s[3].val[1] + s[2].val[1];
            out[1].val[0] = s[1].val[0] - s[0].val[1];
            out[1].val[1] = s[1].val[1] + s[0].val[0];
            out[2].val[0] = s[1].val[0] + s[0].val[1];
            out[2].val[1] = s[1].val[1] - s[0].val[0];

            if (is_inverse == 1)
            {
                NE10_FFT3_CONJ (out);
            }
            if (is_scaled)
            {
                NE10_FFT3_SCALING (out, one_by_fft_neon);
            }

            NE10_CPLX_STORE (Fout + 0 * out_step, out[0]);
            NE10_CPLX_STORE (Fout + 1 * out_step, out[1]);
            NE10_CPLX_STORE (Fout + 2 * out_step, out[2]);
#else // NE10_INLINE_ASM_OPT
#ifndef __aarch64__
#error Currently, inline assembly optimizations are only available on AArch64.
#else // __aarch64__
            asm volatile (
                "ld2 {v0.4s, v1.4s}, [%[pin0]] \n\t" // in[0]
                "ld2 {v2.4s, v3.4s}, [%[pin1]] \n\t" // in[1]
                "ld2 {v4.4s, v5.4s}, [%[pin2]] \n\t" // in[2]
            :
            : [pin0]"r"(Fin),
              [pin1]"r"(Fin + in_step),
              [pin2]"r"(Fin + in_step * 2)
            : "memory", "v0", "v1", "v2", "v3", "v4", "v5");

            if (is_inverse == 1)
            {
                asm volatile (
                    "fneg v1.4s, v1.4s \n\t"
                    "fneg v3.4s, v3.4s \n\t"
                    "fneg v5.4s, v5.4s \n\t"
                :
                :
                : "v1", "v3", "v5");
            }

            if (is_first_stage == 0)
            {
                asm volatile (
                    // Load twiddles.
                    "ld1 {v6.1d}, [%[ptw0]] \n\t" // tw0
                    "ld1 {v7.1d}, [%[ptw1]] \n\t" // tw1
                    // in[1] = in[1] * tw[0]
                    "fmul v10.4s,  v2.4s,  v6.s[1] \n\t" // RI
                    "fmul  v2.4s,  v2.4s,  v6.s[0] \n\t" // RR
                    "fmls  v2.4s,  v3.4s,  v6.s[1] \n\t" // RR - II
                    "fmul v11.4s,  v3.4s,  v6.s[0] \n\t" // IR
                    "fadd  v3.4s, v10.4s, v11.4s \n\t" // RI + IR
                    // in[2] = in[2] * tw[1]
                    "fmul v10.4s,  v4.4s,  v7.s[1] \n\t" // RI
                    "fmul  v4.4s,  v4.4s,  v7.s[0] \n\t" // RR
                    "fmls  v4.4s,  v5.4s,  v7.s[1] \n\t" // RR - II
                    "fmul v11.4s,  v5.4s,  v7.s[0] \n\t" // IR
                    "fadd  v5.4s, v10.4s, v11.4s \n\t" // RI + IR
                :
                : [ptw0]"r"(twiddles),
                  [ptw1]"r"(twiddles + out_step)
                : "memory", "v0", "v1", "v2", "v3", "v4", "v5",
                  "v6", "v7", "v8", "v9",
                  "v10", "v11");
            }

            asm volatile (
                "fadd %[s2r].4s, v2.4s, v4.4s \n\t"
                "fadd %[s2i].4s, v3.4s, v5.4s \n\t"

                "fsub %[s0r].4s, v2.4s, v4.4s \n\t"
                "fsub %[s0i].4s, v3.4s, v5.4s \n\t"

                "mov %[s3r].16b, v0.16b \n\t"
                "mov %[s3i].16b, v1.16b \n\t"
            : [s0r]"+w"(s[0].val[0]),
              [s0i]"+w"(s[0].val[1]),
              [s2r]"+w"(s[2].val[0]),
              [s2i]"+w"(s[2].val[1]),
              [s3r]"+w"(s[3].val[0]),
              [s3i]"+w"(s[3].val[1])
            :
            : "v0", "v1", "v2", "v3", "v4", "v5");

            s[1].val[0] = - s[2].val[0] * HALF_NEON_F32;
            s[1].val[1] = - s[2].val[1] * HALF_NEON_F32;

            s[1].val[0] += s[3].val[0];
            s[1].val[1] += s[3].val[1];
            s[0].val[0] *= TW_3IN_NEON_F32;
            s[0].val[1] *= TW_3IN_NEON_F32;

            // out[0] - {v0.4s, v1.4s}
            // out[1] - {v2.4s, v3.4s}
            // out[2] - {v4.4s, v5.4s}
            asm volatile (
                "fadd v0.4s, %[s3r].4s, %[s2r].4s \n\t"
                "fadd v1.4s, %[s3i].4s, %[s2i].4s \n\t"
                "fsub v2.4s, %[s1r].4s, %[s0i].4s \n\t"
                "fadd v3.4s, %[s1i].4s, %[s0r].4s \n\t"
                "fadd v4.4s, %[s1r].4s, %[s0i].4s \n\t"
                "fsub v5.4s, %[s1i].4s, %[s0r].4s \n\t"
            :
            : [s0r]"w"(s[0].val[0]),
              [s0i]"w"(s[0].val[1]),
              [s1r]"w"(s[1].val[0]),
              [s1i]"w"(s[1].val[1]),
              [s2r]"w"(s[2].val[0]),
              [s2i]"w"(s[2].val[1]),
              [s3r]"w"(s[3].val[0]),
              [s3i]"w"(s[3].val[1])
            : "v0", "v1", "v2", "v3", "v4", "v5");

            if (is_inverse == 1)
            {
                asm volatile (
                    "fneg v1.4s, v1.4s \n\t"
                    "fneg v3.4s, v3.4s \n\t"
                    "fneg v5.4s, v5.4s \n\t"
                :
                :
                : "v1", "v3", "v5");
            }

            if (is_scaled)
            {
                asm volatile (
                    "fmul v0.4s, v0.4s, %[one_by_nfft].4s \n\t"
                    "fmul v1.4s, v1.4s, %[one_by_nfft].4s \n\t"
                    "fmul v2.4s, v2.4s, %[one_by_nfft].4s \n\t"
                    "fmul v3.4s, v3.4s, %[one_by_nfft].4s \n\t"
                    "fmul v4.4s, v4.4s, %[one_by_nfft].4s \n\t"
                    "fmul v5.4s, v5.4s, %[one_by_nfft].4s \n\t"
                :
                : [one_by_nfft]"w"(one_by_fft_neon)
                : "v0", "v1", "v2", "v3", "v4", "v5");
            }

            asm volatile (
                "st2 {v0.4s, v1.4s}, [%[pout0]] \n\t"
                "st2 {v2.4s, v3.4s}, [%[pout1]] \n\t"
                "st2 {v4.4s, v5.4s}, [%[pout2]] \n\t"
            :
            : [pout0]"r"(Fout),
              [pout1]"r"(Fout + out_step),
              [pout2]"r"(Fout + 2 * out_step)
            : "memory", "v0", "v1", "v2", "v3", "v4", "v5");
#endif // __aarch64__
#endif // NE10_INLINE_ASM_OPT

            Fin++;

            if (is_first_stage == 0)
            {
                Fout++;
                twiddles++;
            }
            else
            {
                Fout += 3;
            }
        }
        if (is_first_stage == 0)
        {
            twiddles -= out_step;
            Fout += (3 - 1) * out_step;
        }
    }
}

template<ne10_int32_t is_first_stage, ne10_int32_t is_inverse, bool is_scaled>
static void ne10_radix_5_butterfly_float32_neon (CPLX *Fout,
        const CPLX *Fin,
        const ne10_fft_cpx_float32_t *twiddles,
        const ne10_int32_t fstride,
        const ne10_int32_t out_step,
        const ne10_int32_t nfft)
{
    CPLX in[5];
    CPLX s[6];

    const ne10_int32_t in_step = nfft / 5;
    ne10_int32_t f_count;
    ne10_int32_t m_count;

    const REAL one_by_fft_neon = NE10_REAL_DUP_NEON_F32 (0.25 / nfft);

    for (f_count = fstride; f_count > 0; f_count--)
    {
        for (m_count = out_step; m_count > 0; m_count--)
        {
            in[0] = NE10_CPLX_LOAD (Fin + 0 * in_step);
            in[1] = NE10_CPLX_LOAD (Fin + 1 * in_step);
            in[2] = NE10_CPLX_LOAD (Fin + 2 * in_step);
            in[3] = NE10_CPLX_LOAD (Fin + 3 * in_step);
            in[4] = NE10_CPLX_LOAD (Fin + 4 * in_step);

            if (is_inverse == 1)
            {
                NE10_FFT5_CONJ (in);
            }

            if (is_first_stage == 0)
            {
                NE10_LOAD_TW_AND_MUL (in[1], twiddles);
                NE10_LOAD_TW_AND_MUL (in[2], twiddles + out_step);
                NE10_LOAD_TW_AND_MUL (in[3], twiddles + out_step * 2);
                NE10_LOAD_TW_AND_MUL (in[4], twiddles + out_step * 3);
            }

            NE10_CPX_ADD_NEON_F32 (s[1], in[1], in[4]);
            NE10_CPX_ADD_NEON_F32 (s[2], in[2], in[3]);

            s[0] = in[0];
            s[5] = in[0];

            in[0].val[0] = in[0].val[0] + s[1].val[0] + s[2].val[0];
            in[0].val[1] = in[0].val[1] + s[1].val[1] + s[2].val[1];

            s[0].val[0] = NE10_S_MLA_NEON_F32 (s[0].val[0], s[1].val[0], TW_5A_F32.r);
            s[0].val[1] = NE10_S_MLA_NEON_F32 (s[0].val[1], s[1].val[1], TW_5A_F32.r);
            s[5].val[0] = NE10_S_MLA_NEON_F32 (s[5].val[0], s[1].val[0], TW_5B_F32.r);
            s[5].val[1] = NE10_S_MLA_NEON_F32 (s[5].val[1], s[1].val[1], TW_5B_F32.r);

            s[0].val[0] = NE10_S_MLA_NEON_F32 (s[0].val[0], s[2].val[0], TW_5B_F32.r);
            s[0].val[1] = NE10_S_MLA_NEON_F32 (s[0].val[1], s[2].val[1], TW_5B_F32.r);
            s[5].val[0] = NE10_S_MLA_NEON_F32 (s[5].val[0], s[2].val[0], TW_5A_F32.r);
            s[5].val[1] = NE10_S_MLA_NEON_F32 (s[5].val[1], s[2].val[1], TW_5A_F32.r);

            NE10_CPX_SUB_NEON_F32 (s[4], in[1], in[4]);
            NE10_CPX_SUB_NEON_F32 (s[3], in[2], in[3]);

            s[1].val[0] = NE10_S_MUL_NEON_F32 (s[4].val[1], TW_5A_F32.i);
            s[1].val[1] = -NE10_S_MUL_NEON_F32 (s[4].val[0], TW_5A_F32.i);
            s[2].val[0] = -NE10_S_MUL_NEON_F32 (s[4].val[1], TW_5B_F32.i);
            s[2].val[1] = NE10_S_MUL_NEON_F32 (s[4].val[0], TW_5B_F32.i);

            s[1].val[0] = NE10_S_MLA_NEON_F32 (s[1].val[0], s[3].val[1], TW_5B_F32.i);
            s[1].val[1] = NE10_S_MLS_NEON_F32 (s[1].val[1], s[3].val[0], TW_5B_F32.i);
            s[2].val[0] = NE10_S_MLA_NEON_F32 (s[2].val[0], s[3].val[1], TW_5A_F32.i);
            s[2].val[1] = NE10_S_MLS_NEON_F32 (s[2].val[1], s[3].val[0], TW_5A_F32.i);

            NE10_CPX_SUB_NEON_F32 (in[1], s[0], s[1]);
            NE10_CPX_ADD_NEON_F32 (in[4], s[0], s[1]);
            NE10_CPX_ADD_NEON_F32 (in[2], s[5], s[2]);
            NE10_CPX_SUB_NEON_F32 (in[3], s[5], s[2]);

            if (is_inverse == 1)
            {
                NE10_FFT5_CONJ (in);
            }
            if (is_scaled)
            {
                NE10_FFT5_SCALING (in, one_by_fft_neon);
            }

            NE10_CPLX_STORE (Fout + 0 * out_step, in[0]);
            NE10_CPLX_STORE (Fout + 1 * out_step, in[1]);
            NE10_CPLX_STORE (Fout + 2 * out_step, in[2]);
            NE10_CPLX_STORE (Fout + 3 * out_step, in[3]);
            NE10_CPLX_STORE (Fout + 4 * out_step, in[4]);

            Fin++;

            if (is_first_stage == 0)
            {
                Fout++;
                twiddles++;
            }
            else
            {
                Fout += 5;
            }
        }
        if (is_first_stage == 0)
        {
            twiddles -= out_step;
            Fout += (5 - 1) * out_step;
        }
    }
}

template<ne10_int32_t is_first_stage, ne10_int32_t is_inverse, bool is_scaled>
static void ne10_radix_8_butterfly_float32_neon (CPLX *Fout,
        const CPLX *Fin,
        const ne10_fft_cpx_float32_t *,
        const ne10_int32_t fstride,
        const ne10_int32_t out_step,
        const ne10_int32_t nfft)
{
    CPLX in[8];
    CPLX out[8];

    const ne10_int32_t in_step = nfft / 8;
    ne10_int32_t f_count;
    ne10_int32_t m_count;

    const REAL one_by_fft_neon = NE10_REAL_DUP_NEON_F32 (0.25 / nfft);

    for (f_count = fstride; f_count > 0; f_count--)
    {
        for (m_count = out_step; m_count > 0; m_count--)
        {
            in[0] = NE10_CPLX_LOAD (Fin + 0 * in_step);
            in[1] = NE10_CPLX_LOAD (Fin + 1 * in_step);
            in[2] = NE10_CPLX_LOAD (Fin + 2 * in_step);
            in[3] = NE10_CPLX_LOAD (Fin + 3 * in_step);
            in[4] = NE10_CPLX_LOAD (Fin + 4 * in_step);
            in[5] = NE10_CPLX_LOAD (Fin + 5 * in_step);
            in[6] = NE10_CPLX_LOAD (Fin + 6 * in_step);
            in[7] = NE10_CPLX_LOAD (Fin + 7 * in_step);

            if (is_inverse == 1)
            {
                NE10_FFT8_CONJ (in);
            }

            NE10_FFT8_FUC_NEON_F32 (out, in);

            if (is_inverse == 1)
            {
                NE10_FFT8_CONJ (out);
            }
            if (is_scaled)
            {
                NE10_FFT8_SCALING (out, one_by_fft_neon);
            }

            NE10_CPLX_STORE (Fout + 0 * out_step, out[0]);
            NE10_CPLX_STORE (Fout + 1 * out_step, out[1]);
            NE10_CPLX_STORE (Fout + 2 * out_step, out[2]);
            NE10_CPLX_STORE (Fout + 3 * out_step, out[3]);
            NE10_CPLX_STORE (Fout + 4 * out_step, out[4]);
            NE10_CPLX_STORE (Fout + 5 * out_step, out[5]);
            NE10_CPLX_STORE (Fout + 6 * out_step, out[6]);
            NE10_CPLX_STORE (Fout + 7 * out_step, out[7]);

            Fin++;
            Fout += 8;
        }
    }
}

template<ne10_int32_t is_inverse, bool is_scaled>
static void ne10_mixed_radix_generic_butterfly_float32_neon_impl (CPLX *Fout,
        const CPLX *Fin,
        const ne10_int32_t *factors,
        const ne10_fft_cpx_float32_t *twiddles,
        CPLX *buffer)
{
    ne10_int32_t fstride, mstride, radix;
    ne10_int32_t stage_count;
    ne10_int32_t nfft;

    // init fstride, mstride, radix, nfft
    stage_count = factors[0];
    fstride = factors[1];
    mstride = 1;
    radix = factors[ stage_count << 1 ]; // radix of first stage
    nfft = fstride * radix;

    // swap to make sure output to Fout
    if (stage_count % 2 == 0)
    {
        ne10_swap_ptr (buffer, Fout);
    }

    // first stage
    switch (radix)
    {
    case 2:
        ne10_radix_2_butterfly_float32_neon<1, is_inverse, is_scaled> (Fout, Fin, NULL,
                fstride, 1, nfft);
        break;
    case 4:
        ne10_radix_4_butterfly_float32_neon<1, is_inverse, is_scaled> (Fout, Fin, NULL,
                fstride, 1, nfft);
        break;
    case 3:
        ne10_radix_3_butterfly_float32_neon<1, is_inverse, is_scaled> (Fout, Fin, NULL,
                fstride, 1, nfft);
        break;
    case 5:
        ne10_radix_5_butterfly_float32_neon<1, is_inverse, is_scaled> (Fout, Fin, NULL,
                fstride, 1, nfft);
        break;
    case 8:
        ne10_radix_8_butterfly_float32_neon<1, is_inverse, is_scaled> (Fout, Fin, NULL,
                fstride, 1, nfft);
        break;
    }

    stage_count--;
    if (! stage_count) // finish
    {
        return;
    }

    mstride *= radix;

    // update radix
    if (radix % 2)
    {
        twiddles += radix;
    }
    radix = factors[ stage_count << 1 ];

    // other stages
    while (stage_count > 0)
    {

        // radix of first stage, should be one of {2,3,5,4}
        assert ((radix > 1) && (radix < 6));

        ne10_swap_ptr (buffer, Fout);

        fstride /= radix;
        switch (radix)
        {
        case 2:
            ne10_radix_2_butterfly_float32_neon<0, is_inverse, false> (Fout, buffer,
                    twiddles, fstride, mstride, nfft);
            break;
        case 3:
            ne10_radix_3_butterfly_float32_neon<0, is_inverse, false> (Fout, buffer,
                    twiddles, fstride, mstride, nfft);
            break;
        case 4:
            ne10_radix_4_butterfly_float32_neon<0, is_inverse, false> (Fout, buffer,
                    twiddles, fstride, mstride, nfft);
            break;
        case 5:
            ne10_radix_5_butterfly_float32_neon<0, is_inverse, false> (Fout, buffer,
                    twiddles, fstride, mstride, nfft);
            break;
        } // switch (radix)

        twiddles += mstride * (radix - 1);
        mstride *= radix;

        stage_count--;
        radix = factors[ stage_count << 1 ];
    } // while (stage_count)
}

template<ne10_int32_t is_inverse>
static void ne10_c2c_1d_last_stage_neon (CPLX *Fout,
        const CPLX *Fin,
        const ne10_fft_cpx_float32_t *twiddles,
        const ne10_int32_t fstride,
        const ne10_int32_t out_step,
        const ne10_int32_t)
{
    ne10_int32_t f_count;
    ne10_int32_t m_count;

    for (f_count = fstride; f_count > 0; f_count--)
    {
        CPLX scratch_in[4];
        CPLX scratch_out[4];
        CPLX scratch[4];

        for (m_count = out_step / NE10_FFT_PARA_LEVEL; m_count > 0; m_count--)
        {
#ifndef NE10_INLINE_ASM_OPT
            scratch_in[0] = NE10_CPLX_LOAD (Fin + 0);
            scratch_in[1] = NE10_CPLX_LOAD (Fin + 1);
            scratch_in[2] = NE10_CPLX_LOAD (Fin + 2);
            scratch_in[3] = NE10_CPLX_LOAD (Fin + 3);

            // Transpose
            {
                CPLX scratch0, scratch_in0;
                CPLX scratch1, scratch_in1;
                CPLX scratch2, scratch_in2;
                CPLX scratch3, scratch_in3;

                scratch_in0 = scratch_in[0];
                scratch_in1 = scratch_in[1];
                scratch_in2 = scratch_in[2];
                scratch_in3 = scratch_in[3];

                NE10_RADIX4X4C_TRANSPOSE_NEON (scratch, scratch_in);

                scratch_in[0] = scratch0;
                scratch_in[1] = scratch1;
                scratch_in[2] = scratch2;
                scratch_in[3] = scratch3;
            }
#else // NE10_INLINE_ASM_OPT
#ifndef __aarch64__
#error Currently, inline assembly optimizations are only available on AArch64.
#else // __aarch64__
            const float *pin = (const float *) Fin;
            asm volatile (
                "ld2 {v0.4s, v1.4s}, [%[pin]], %[offset] \n\t"
                "ld2 {v2.4s, v3.4s}, [%[pin]], %[offset] \n\t"
                "ld2 {v4.4s, v5.4s}, [%[pin]], %[offset] \n\t"
                "ld2 {v6.4s, v7.4s}, [%[pin]] \n\t"

                // NE10_RADIX4X4C_TRANSPOSE_NEON (q2_in,q2_out);
                "trn1 v8.4s, v0.4s, v2.4s \n\t"
                "trn2 v9.4s, v0.4s, v2.4s \n\t"
                "trn1 v10.4s, v4.4s, v6.4s \n\t"
                "trn2 v11.4s, v4.4s, v6.4s \n\t"

                "trn1 %[in0r].2d, v8.2d, v10.2d \n\t"
                "trn1 %[in1r].2d, v9.2d, v11.2d \n\t"
                "trn2 %[in2r].2d, v8.2d, v10.2d \n\t"
                "trn2 %[in3r].2d, v9.2d, v11.2d \n\t"

                "trn1 v8.4s, v1.4s, v3.4s \n\t"
                "trn2 v9.4s, v1.4s, v3.4s \n\t"
                "trn1 v10.4s, v5.4s, v7.4s \n\t"
                "trn2 v11.4s, v5.4s, v7.4s \n\t"

                "trn1 %[in0i].2d, v8.2d, v10.2d \n\t"
                "trn1 %[in1i].2d, v9.2d, v11.2d \n\t"
                "trn2 %[in2i].2d, v8.2d, v10.2d \n\t"
                "trn2 %[in3i].2d, v9.2d, v11.2d \n\t"
            : [in0r]"+w"(scratch_in[0].val[0]),
              [in0i]"+w"(scratch_in[0].val[1]),
              [in1r]"+w"(scratch_in[1].val[0]),
              [in1i]"+w"(scratch_in[1].val[1]),
              [in2r]"+w"(scratch_in[2].val[0]),
              [in2i]"+w"(scratch_in[2].val[1]),
              [in3r]"+w"(scratch_in[3].val[0]),
              [in3i]"+w"(scratch_in[3].val[1]),
              [pin]"+r"(pin)
            : [offset]"r"(32)
            : "memory", "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7",
              "v8", "v9", "v10", "v11");
#endif // __aarch64__
#endif // NE10_INLINE_ASM_OPT

            if (is_inverse)
            {
                NE10_FFT4_CONJ (scratch_in);
            }

            // Not first stage
            {
#ifndef NE10_INLINE_ASM_OPT
                CPLX scratch_tw[3];

                scratch_tw[0] = NE10_CPLX_LOAD (twiddles + 0 * out_step);
                scratch_tw[1] = NE10_CPLX_LOAD (twiddles + 1 * out_step);
                scratch_tw[2] = NE10_CPLX_LOAD (twiddles + 2 * out_step);

                NE10_FFT4_MUL_TW_NEON (scratch_in, scratch_in, scratch_tw);
#else // NE10_INLINE_ASM_OPT
#ifndef __aarch64__
#error Currently, inline assembly optimizations are only available on AArch64.
#else // __aarch64__
                const float *tw = (const float *)twiddles;
                asm volatile (
                    "ld2 {v0.4s, v1.4s}, [%[tw]], %[offset] \n\t"
                    "ld2 {v2.4s, v3.4s}, [%[tw]], %[offset] \n\t"
                    "ld2 {v4.4s, v5.4s}, [%[tw]] \n\t"

                    "fmul       v6.4s, %[in1r].4s,  v1.4s \n\t" // RI
                    "fmul  %[in1r].4s, %[in1r].4s,  v0.4s \n\t" // RR
                    "fmls  %[in1r].4s, %[in1i].4s,  v1.4s \n\t" // RR - II
                    "fmul       v7.4s, %[in1i].4s,  v0.4s \n\t" // IR
                    "fadd  %[in1i].4s,      v6.4s,  v7.4s \n\t" // RI + IR

                    "fmul       v6.4s, %[in2r].4s,  v3.4s \n\t" // RI
                    "fmul  %[in2r].4s, %[in2r].4s,  v2.4s \n\t" // RR
                    "fmls  %[in2r].4s, %[in2i].4s,  v3.4s \n\t" // RR - II
                    "fmul       v7.4s, %[in2i].4s,  v2.4s \n\t" // IR
                    "fadd  %[in2i].4s,      v6.4s,  v7.4s \n\t" // RI + IR

                    "fmul       v6.4s, %[in3r].4s,  v5.4s \n\t" // RI
                    "fmul  %[in3r].4s, %[in3r].4s,  v4.4s \n\t" // RR
                    "fmls  %[in3r].4s, %[in3i].4s,  v5.4s \n\t" // RR - II
                    "fmul       v7.4s, %[in3i].4s,  v4.4s \n\t" // IR
                    "fadd  %[in3i].4s,      v6.4s,  v7.4s \n\t" // RI + IR
                : [tw]"+r"(tw),
                  [in1r]"+w"(scratch_in[1].val[0]),
                  [in1i]"+w"(scratch_in[1].val[1]),
                  [in2r]"+w"(scratch_in[2].val[0]),
                  [in2i]"+w"(scratch_in[2].val[1]),
                  [in3r]"+w"(scratch_in[3].val[0]),
                  [in3i]"+w"(scratch_in[3].val[1])
                : [offset]"r"(out_step * 8)
                : "memory", "v0", "v1", "v2", "v3", "v4", "v5",
                  "v6", "v7");
#endif // __aarch64__
#endif // NE10_INLINE_ASM_OPT

            }

            NE10_CPX_ADD_NEON_F32 (scratch[0], scratch_in[0], scratch_in[2]);
            NE10_CPX_SUB_NEON_F32 (scratch[1], scratch_in[0], scratch_in[2]);
            NE10_CPX_ADD_NEON_F32 (scratch[2], scratch_in[1], scratch_in[3]);
            NE10_CPX_SUB_NEON_F32 (scratch[3], scratch_in[1], scratch_in[3]);

#ifndef NE10_INLINE_ASM_OPT
            NE10_CPX_ADD_NEON_F32 (scratch_out[0], scratch[0], scratch[2]);
            NE10_CPX_SUB_NEON_F32 (scratch_out[2], scratch[0], scratch[2]);

            scratch_out[1].val[0] = scratch[1].val[0] + scratch[3].val[1];
            scratch_out[1].val[1] = scratch[1].val[1] - scratch[3].val[0];
            scratch_out[3].val[0] = scratch[1].val[0] - scratch[3].val[1];
            scratch_out[3].val[1] = scratch[1].val[1] + scratch[3].val[0];

            if (is_inverse == 1)
            {
                NE10_FFT4_CONJ (scratch_out);
            }

            // Store.
            {
                ne10_fft_cpx_float32_t *Fout_cpx;
                Fout_cpx = (ne10_fft_cpx_float32_t *) Fout;

                NE10_CPLX_STORE (Fout_cpx + 0 * out_step, scratch_out[0]);
                NE10_CPLX_STORE (Fout_cpx + 1 * out_step, scratch_out[1]);
                NE10_CPLX_STORE (Fout_cpx + 2 * out_step, scratch_out[2]);
                NE10_CPLX_STORE (Fout_cpx + 3 * out_step, scratch_out[3]);
            }
#else // NE10_INLINE_ASM_OPT
#ifndef __aarch64__
#error Currently, inline assembly optimizations are only available on AArch64.
#else // __aarch64__
            asm volatile (
                "fadd v0.4s, %[s0r].4s, %[s2r].4s \n\t"
                "fadd v1.4s, %[s0i].4s, %[s2i].4s \n\t"
                "fsub v4.4s, %[s0r].4s, %[s2r].4s \n\t"
                "fsub v5.4s, %[s0i].4s, %[s2i].4s \n\t"
                "fadd v2.4s, %[s1r].4s, %[s3i].4s \n\t"
                "fsub v3.4s, %[s1i].4s, %[s3r].4s \n\t"
                "fsub v6.4s, %[s1r].4s, %[s3i].4s \n\t"
                "fadd v7.4s, %[s1i].4s, %[s3r].4s \n\t"
            :
            : [s0r]"w"(scratch[0].val[0]),
              [s0i]"w"(scratch[0].val[1]),
              [s1r]"w"(scratch[1].val[0]),
              [s1i]"w"(scratch[1].val[1]),
              [s2r]"w"(scratch[2].val[0]),
              [s2i]"w"(scratch[2].val[1]),
              [s3r]"w"(scratch[3].val[0]),
              [s3i]"w"(scratch[3].val[1])
            : "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7");

            if (is_inverse == 1)
            {
                asm volatile (
                    "fneg v1.4s, v1.4s \n\t"
                    "fneg v3.4s, v3.4s \n\t"
                    "fneg v5.4s, v5.4s \n\t"
                    "fneg v7.4s, v7.4s \n\t"
                :
                :
                : "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7");
            }

            float *pout = (float *) Fout;
            asm volatile (
                "st2 {v0.4s, v1.4s}, [%[pout]], %[offset] \n\t"
                "st2 {v2.4s, v3.4s}, [%[pout]], %[offset] \n\t"
                "st2 {v4.4s, v5.4s}, [%[pout]], %[offset] \n\t"
                "st2 {v6.4s, v7.4s}, [%[pout]] \n\t"
            : [pout]"+r"(pout)
            : [offset]"r"(out_step * 8)
            : "memory", "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7");
#endif // __aarch64__
#endif // NE10_INLINE_ASM_OPT
            Fin += 4;
            Fout += 1;
            twiddles += 4;
        }
    }

    ne10_int32_t left_over = out_step % 4;
    if (left_over == 0)
    {
        return;
    }

    // Left over.
    const ne10_fft_cpx_float32_t *Fin_s  = (ne10_fft_cpx_float32_t *) Fin;
    ne10_fft_cpx_float32_t *Fout_s = (ne10_fft_cpx_float32_t *) Fout;
    for (m_count = out_step % 4; m_count > 0; m_count--)
    {
        ne10_fft_cpx_float32_t scratch_in[4];
        ne10_fft_cpx_float32_t scratch_tw[4];

        scratch_in[0] = Fin_s[0];
        scratch_in[1] = Fin_s[1];
        scratch_in[2] = Fin_s[2];
        scratch_in[3] = Fin_s[3];

        if (is_inverse)
        {
            scratch_in[0].i = -scratch_in[0].i;
            scratch_in[1].i = -scratch_in[1].i;
            scratch_in[2].i = -scratch_in[2].i;
            scratch_in[3].i = -scratch_in[3].i;
        }

        scratch_tw[0] = twiddles[0 * out_step];
        scratch_tw[1] = twiddles[1 * out_step];
        scratch_tw[2] = twiddles[2 * out_step];

        FFT4_MUL_TW (scratch_in, scratch_in, scratch_tw);

        FFT4_FCU_INPLACE (scratch_in);

        if (is_inverse)
        {
            scratch_in[0].i = -scratch_in[0].i;
            scratch_in[1].i = -scratch_in[1].i;
            scratch_in[2].i = -scratch_in[2].i;
            scratch_in[3].i = -scratch_in[3].i;
        }

        Fout_s[0 * out_step] = scratch_in[0];
        Fout_s[1 * out_step] = scratch_in[1];
        Fout_s[2 * out_step] = scratch_in[2];
        Fout_s[3 * out_step] = scratch_in[3];

        Fin_s += 4;
        Fout_s += 1;
        twiddles += 1;
    }
}

typedef void (*NE10_MIXED_RADIX_FUNC) (CPLX*, const CPLX *, const ne10_int32_t *,
        const ne10_fft_cpx_float32_t *, CPLX *);

void ne10_mixed_radix_generic_butterfly_float32_neon (
        ne10_fft_cpx_float32_t *Fout,
        const ne10_fft_cpx_float32_t *Fin,
        const ne10_int32_t *factors,
        const ne10_fft_cpx_float32_t *twiddles,
        ne10_fft_cpx_float32_t *buffer,
        const ne10_int32_t is_scaled)
{
    ne10_int32_t stage_count = factors[0];
    ne10_int32_t fstride = factors[1];
    ne10_int32_t radix = factors[stage_count << 1]; // radix of first stage

    NE10_MIXED_RADIX_FUNC ne10_mixed_radix_impl = NULL;

    // nfft below is not the actual length of FFT, it is 1/4 of the actual one
    // instead.
    ne10_int32_t nfft = fstride * radix;

    if (is_scaled)
    {
        ne10_mixed_radix_impl = ne10_mixed_radix_generic_butterfly_float32_neon_impl<0, true>;
    }
    else
    {
        ne10_mixed_radix_impl = ne10_mixed_radix_generic_butterfly_float32_neon_impl<0, false>;
    }

    ne10_mixed_radix_impl ((CPLX *) buffer,
            (const CPLX *) Fin, // From Fin to buffer
            factors,
            twiddles,
            (CPLX *) Fout); // Fout is "buffer" for these stages.

    ne10_c2c_1d_last_stage_neon<0> ((CPLX *) Fout,
            (const CPLX *) buffer, // From buffer to Fout
            twiddles + nfft,
            1, // out_step == fstride == 1
            nfft, // in_step == mstride == nfft
            nfft * 4); // Actual length of FFT
}

void ne10_mixed_radix_generic_butterfly_inverse_float32_neon (
        ne10_fft_cpx_float32_t *Fout,
        const ne10_fft_cpx_float32_t *Fin,
        const ne10_int32_t *factors,
        const ne10_fft_cpx_float32_t *twiddles,
        ne10_fft_cpx_float32_t *buffer,
        const ne10_int32_t is_scaled)
{
    ne10_int32_t stage_count = factors[0];
    ne10_int32_t fstride = factors[1];
    ne10_int32_t radix = factors[stage_count << 1]; // radix of first stage

    NE10_MIXED_RADIX_FUNC ne10_mixed_radix_impl = NULL;

    // nfft below is not the actual length of FFT, it is 1/4 of the actual one
    // instead.
    ne10_int32_t nfft = fstride * radix;

    if (is_scaled)
    {
        ne10_mixed_radix_impl = ne10_mixed_radix_generic_butterfly_float32_neon_impl<1, true>;
    }
    else
    {
        ne10_mixed_radix_impl = ne10_mixed_radix_generic_butterfly_float32_neon_impl<1, false>;
    }

    ne10_mixed_radix_impl ((CPLX *) buffer,
            (const CPLX *) Fin, // From Fin to buffer
            factors,
            twiddles,
            (CPLX *) Fout); // Fout is "buffer" for these stages.

    ne10_c2c_1d_last_stage_neon<1> ((CPLX *) Fout,
            (const CPLX *) buffer, // From buffer to Fout
            twiddles + nfft,
            1, // out_step == fstride == 1
            nfft, // in_step == mstride == nfft
            nfft * 4); // Actual length of FFT
}
