/*
 *  Copyright 2014-15 ARM Limited
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

    CPLX scratch[4];

    Fout[0] = Fin[0];
    Fout[1] = Fin[1];
    Fout[2] = Fin[2];

    scratch[1] = Fout[1];
    scratch[2] = Fout[2];

    NE10_CPX_ADD_NEON_F32 (scratch[3], scratch[1], scratch[2]);
    NE10_CPX_SUB_NEON_F32 (scratch[0], scratch[1], scratch[2]);

    Fout[1].val[0] = Fout[0].val[0] - scratch[3].val[0] * HALF_NEON_F32;
    Fout[1].val[1] = Fout[0].val[1] - scratch[3].val[1] * HALF_NEON_F32;

    scratch[0].val[0] = scratch[0].val[0] * TW_3IN_NEON_F32;
    scratch[0].val[1] = scratch[0].val[1] * TW_3IN_NEON_F32;

    Fout[0].val[0] += scratch[3].val[0];
    Fout[0].val[1] += scratch[3].val[1];

    Fout[2].val[0] = Fout[1].val[0] + scratch[0].val[1];
    Fout[2].val[1] = Fout[1].val[1] - scratch[0].val[0];

    Fout[1].val[0] -= scratch[0].val[1];
    Fout[1].val[1] += scratch[0].val[0];
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
    PRINT_HIT;

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
template<ne10_int32_t is_first_stage, ne10_int32_t is_inverse>
static void ne10_radix_2_butterfly_float32_neon (CPLX *Fout,
        const CPLX *Fin,
        const ne10_fft_cpx_float32_t *twiddles,
        const ne10_int32_t fstride,
        const ne10_int32_t out_step,
        const ne10_int32_t nfft)
{
    PRINT_HIT;

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

                if (is_first_stage == 1)
                {
                    NE10_FFT2_SCALING (out, one_by_fft_neon);
                }
            }

            NE10_CPLX_STORE (Fout + 0 * out_step, out[0]);
            NE10_CPLX_STORE (Fout + 1 * out_step, out[1]);

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
template<ne10_int32_t is_first_stage, ne10_int32_t is_inverse>
static void ne10_radix_4_butterfly_float32_neon (CPLX *Fout,
        const CPLX *Fin,
        const ne10_fft_cpx_float32_t *twiddles,
        const ne10_int32_t fstride,
        const ne10_int32_t out_step,
        const ne10_int32_t nfft)
{
    CPLX in[4];

    const ne10_int32_t in_step = nfft / 4;
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
                if (is_first_stage == 1)
                {
                    NE10_FFT4_SCALING (in, one_by_fft_neon);
                }
            }

            NE10_CPLX_STORE (Fout + 0 * out_step, in[0]);
            NE10_CPLX_STORE (Fout + 1 * out_step, in[1]);
            NE10_CPLX_STORE (Fout + 2 * out_step, in[2]);
            NE10_CPLX_STORE (Fout + 3 * out_step, in[3]);

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

template<ne10_int32_t is_first_stage, ne10_int32_t is_inverse>
static void ne10_radix_3_butterfly_float32_neon (CPLX *Fout,
        const CPLX *Fin,
        const ne10_fft_cpx_float32_t *twiddles,
        const ne10_int32_t fstride,
        const ne10_int32_t out_step,
        const ne10_int32_t nfft)
{
    CPLX in[3];
    CPLX out[3];

    const ne10_int32_t in_step = nfft / 3;
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

            if (is_inverse == 1)
            {
                NE10_FFT3_CONJ (in);
            }

            if (is_first_stage == 0)
            {
                NE10_LOAD_TW_AND_MUL (in[1], twiddles);
                NE10_LOAD_TW_AND_MUL (in[2], twiddles + out_step);
            }

            NE10_FFT3_FUC_NEON_F32 (out, in);

            if (is_inverse == 1)
            {
                NE10_FFT3_CONJ (out);
                if (is_first_stage == 1)
                {
                    NE10_FFT3_SCALING (out, one_by_fft_neon);
                }
            }

            NE10_CPLX_STORE (Fout + 0 * out_step, out[0]);
            NE10_CPLX_STORE (Fout + 1 * out_step, out[1]);
            NE10_CPLX_STORE (Fout + 2 * out_step, out[2]);

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

template<ne10_int32_t is_first_stage, ne10_int32_t is_inverse>
static void ne10_radix_5_butterfly_float32_neon (CPLX *Fout,
        const CPLX *Fin,
        const ne10_fft_cpx_float32_t *twiddles,
        const ne10_int32_t fstride,
        const ne10_int32_t out_step,
        const ne10_int32_t nfft)
{
    CPLX in[5];

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

            NE10_FFT5_FUC_INPLACE_NEON_F32 (in);

            if (is_inverse == 1)
            {
                NE10_FFT5_CONJ (in);
                if (is_first_stage == 1)
                {
                    NE10_FFT5_SCALING (in, one_by_fft_neon);
                }
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

template<ne10_int32_t is_first_stage, ne10_int32_t is_inverse>
static void ne10_radix_8_butterfly_float32_neon (CPLX *Fout,
        const CPLX *Fin,
        const ne10_fft_cpx_float32_t *twiddles,
        const ne10_int32_t fstride,
        const ne10_int32_t out_step,
        const ne10_int32_t nfft)
{
    PRINT_HIT;

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

template<ne10_int32_t is_inverse>
static void ne10_mixed_radix_generic_butterfly_float32_neon_impl (CPLX *Fout,
        const CPLX *Fin,
        const ne10_int32_t *factors,
        const ne10_fft_cpx_float32_t *twiddles,
        CPLX *buffer)
{
    PRINT_HIT;

    ne10_int32_t fstride, mstride, radix;
    ne10_int32_t stage_count;
    ne10_int32_t nfft;

    // init fstride, mstride, radix, nfft
    stage_count = factors[0];
    fstride = factors[1];
    mstride = 1;
    radix = factors[ stage_count << 1 ]; // radix of first stage
    nfft = fstride * radix;

    PRINT_HIT;

    // swap to make sure output to Fout
    if (stage_count % 2 == 0)
    {
        ne10_swap_ptr (buffer, Fout);
    }

    // first stage
    switch (radix)
    {
    case 2:
        PRINT_HIT;
        ne10_radix_2_butterfly_float32_neon<1, is_inverse> (Fout, Fin, NULL,
                fstride, 1, nfft);
        break;
    case 4:
        PRINT_HIT;
        ne10_radix_4_butterfly_float32_neon<1, is_inverse> (Fout, Fin, NULL,
                fstride, 1, nfft);
        break;
    case 3:
        PRINT_HIT;
        ne10_radix_3_butterfly_float32_neon<1, is_inverse> (Fout, Fin, NULL,
                fstride, 1, nfft);
        break;
    case 5:
        PRINT_HIT;
        ne10_radix_5_butterfly_float32_neon<1, is_inverse> (Fout, Fin, NULL,
                fstride, 1, nfft);
        break;
    case 8:
        PRINT_HIT;
        ne10_radix_8_butterfly_float32_neon<1, is_inverse> (Fout, Fin, NULL,
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
            ne10_radix_2_butterfly_float32_neon<0, is_inverse> (Fout, buffer,
                    twiddles, fstride, mstride, nfft);
            break;
        case 3:
            ne10_radix_3_butterfly_float32_neon<0, is_inverse> (Fout, buffer,
                    twiddles, fstride, mstride, nfft);
            break;
        case 4:
            ne10_radix_4_butterfly_float32_neon<0, is_inverse> (Fout, buffer,
                    twiddles, fstride, mstride, nfft);
            break;
        case 5:
            ne10_radix_5_butterfly_float32_neon<0, is_inverse> (Fout, buffer,
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
        const ne10_int32_t nfft)
{
#ifdef NE10_VERBOSE
    // Clear Fout
    int i;
    for (i = 0; i < nfft; i++)
    {
        ((ne10_fft_cpx_float32_t *) Fout)[i].r = 0.0;
        ((ne10_fft_cpx_float32_t *) Fout)[i].i = 0.0;
    }
#endif

    ne10_int32_t f_count;
    ne10_int32_t m_count;

    for (f_count = fstride; f_count > 0; f_count--)
    {
        CPLX scratch_in[4];
        CPLX scratch_out[4];

        for (m_count = out_step / NE10_FFT_PARA_LEVEL; m_count > 0; m_count--)
        {
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

            if (is_inverse)
            {
                NE10_FFT4_CONJ (scratch_in);
            }

            // Not first stage
            {
                CPLX scratch_tw[3];

                scratch_tw[0] = NE10_CPLX_LOAD (twiddles + 0 * out_step);
                scratch_tw[1] = NE10_CPLX_LOAD (twiddles + 1 * out_step);
                scratch_tw[2] = NE10_CPLX_LOAD (twiddles + 2 * out_step);

                NE10_FFT4_MUL_TW_NEON (scratch_in, scratch_in, scratch_tw);
            }

            NE10_FFT4_FUC_NEON_F32 (scratch_out, scratch_in);

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

void ne10_mixed_radix_generic_butterfly_float32_neon (
        ne10_fft_cpx_float32_t *Fout,
        const ne10_fft_cpx_float32_t *Fin,
        const ne10_int32_t *factors,
        const ne10_fft_cpx_float32_t *twiddles,
        ne10_fft_cpx_float32_t *buffer)
{
    PRINT_HIT;

    ne10_int32_t stage_count = factors[0];
    ne10_int32_t fstride = factors[1];
    ne10_int32_t radix = factors[stage_count << 1]; // radix of first stage

    // nfft below is not the actual length of FFT, it is 1/4 of the actual one
    // instead.
    ne10_int32_t nfft = fstride * radix;

    ne10_mixed_radix_generic_butterfly_float32_neon_impl<0> ((CPLX *) buffer,
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

    PRINT_HIT;
}

void ne10_mixed_radix_generic_butterfly_inverse_float32_neon (
        ne10_fft_cpx_float32_t *Fout,
        const ne10_fft_cpx_float32_t *Fin,
        const ne10_int32_t *factors,
        const ne10_fft_cpx_float32_t *twiddles,
        ne10_fft_cpx_float32_t *buffer)
{
    PRINT_HIT;

    ne10_int32_t stage_count = factors[0];
    ne10_int32_t fstride = factors[1];
    ne10_int32_t radix = factors[stage_count << 1]; // radix of first stage

    // nfft below is not the actual length of FFT, it is 1/4 of the actual one
    // instead.
    ne10_int32_t nfft = fstride * radix;

    ne10_mixed_radix_generic_butterfly_float32_neon_impl<1> ((CPLX *) buffer,
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

    PRINT_HIT;
}
