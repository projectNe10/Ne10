/*
 *  Copyright 2015-16 ARM Limited
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
 * NE10 Library : dsp/NE10_fft_generic_int32.neonintrisic.h
 *
 * This file must be compiled by C++ toolchain because some functions are
 * written as template functions to make it easier for compiler to
 * reduce branch jump.
 */

#ifndef NE10_FFT_GENERIC_INT32_NEONINTRINSIC_H
#define NE10_FFT_GENERIC_INT32_NEONINTRINSIC_H

#include "NE10_types.h"
#include "NE10_macros.h"
#include "NE10_fft.neonintrinsic.h"
#include "NE10_fft_generic_int32.h"
#include <math.h>
#include <stdlib.h>

typedef int32x4x2_t CPLX;
typedef int32x4_t   REAL;
#define NE10_REAL_DUP_NEON_S32 vdupq_n_s32

#ifndef NE10_INLINE_ASM_OPT
#define NE10_CPLX_LOAD(PTR) vld2q_s32 ((ne10_int32_t*) (PTR))
#define NE10_CPLX_STORE(PTR,OUT) \
    do { \
        vst2q_s32 ((ne10_int32_t*) (PTR), OUT); \
    } while (0)
#else // NE10_INLINE_ASM_OPT
#ifndef __aarch64__
#error Currently, inline assembly optimizations are only available on AArch64.
#else // __aarch64__
template<class T>
inline static int32x4x2_t NE10_CPLX_LOAD(T *ptr)
{
    int32x4x2_t result;
    asm volatile (
        "ld2 {v0.4s, v1.4s}, [%[pin]] \n\t"
        "mov %[r].16b, v0.16b \n\t"
        "mov %[i].16b, v1.16b \n\t"
    : [r]"+w"(result.val[0]),
      [i]"+w"(result.val[1])
    : [pin]"r"(ptr)
    : "memory", "v0", "v1");
    return result;
}

template<class T>
inline static void NE10_CPLX_STORE(T *ptr, int32x4x2_t out)
{
    asm volatile (
        "mov v0.16b, %[r].16b  \n\t"
        "mov v1.16b, %[i].16b  \n\t"
        "st2 {v0.4s, v1.4s}, [%[pout]] \n\t"
    : [r]"+w"(out.val[0]),
      [i]"+w"(out.val[1])
    : [pout]"r"(ptr)
    : "memory", "v0", "v1");
}

#endif // __aarch64__
#endif // NE10_INLINE_ASM_OPT

template<>
inline CPLX NE10_CPX_LOAD_S<CPLX> (const CPLX *ptr)
{
    return NE10_CPLX_LOAD(ptr);
}

template<>
inline void NE10_CPX_STORE_S<CPLX> (CPLX *ptr, const CPLX out)
{
    NE10_CPLX_STORE (ptr, out);
}

template<>
inline void NE10_LOAD_BY_STEP<1, CPLX> (CPLX out[1],
        const CPLX *Fin,
        const ne10_int32_t)
{
    out[0] = NE10_CPX_LOAD_S (Fin);
}

template<>
inline void NE10_STORE_BY_STEP<1, CPLX> (CPLX *Fout,
        const CPLX out[1],
        const ne10_int32_t)
{
    NE10_CPX_STORE_S (Fout, out[0]);
}

static inline REAL NE10_S_MUL_NEON_S32 (const REAL vec,
        const ne10_int32_t scalar)
{
    REAL scalar_neon = NE10_REAL_DUP_NEON_S32 (scalar);
    REAL result = vqrdmulhq_s32 (scalar_neon, vec);
    return result;
}

static inline void NE10_CPX_MUL_NEON_S32 (CPLX &result, const CPLX A, const CPLX B)
{
        REAL ARBR = vqrdmulhq_s32 (A.val[0], B.val[0]);
        REAL ARBI = vqrdmulhq_s32 (A.val[0], B.val[1]);
        REAL AIBR = vqrdmulhq_s32 (A.val[1], B.val[0]);
        REAL AIBI = vqrdmulhq_s32 (A.val[1], B.val[1]);
        result.val[0] = ARBR - AIBI;
        result.val[1] = ARBI + AIBR;
}

template<int RADIX>
inline void NE10_LOAD_TW_AND_MUL (CPLX scratch_in[RADIX],
        const ne10_fft_cpx_int32_t *ptr_in,
        const ne10_int32_t step)
{
    CPLX scratch_tw;
    int32x2_t d2_tmp = vld1_s32 ((ne10_int32_t *)(ptr_in + (RADIX - 2) * step));

    scratch_tw.val[0] = NE10_REAL_DUP_NEON_S32 (d2_tmp[0]);
    scratch_tw.val[1] = NE10_REAL_DUP_NEON_S32 (d2_tmp[1]);
    NE10_CPX_MUL_NEON_S32 (scratch_in[RADIX - 1], scratch_in[RADIX - 1], scratch_tw);

    NE10_LOAD_TW_AND_MUL<RADIX - 1> (scratch_in, ptr_in, step);
}

template<>
inline void NE10_LOAD_TW_AND_MUL<1> (CPLX [1],
        const ne10_fft_cpx_int32_t *,
        const ne10_int32_t)
{
}

////////////////
// Conj inplace.
////////////////
template<>
inline void NE10_CONJ_S<CPLX> (CPLX &cplx)
{
    cplx.val[1] = -cplx.val[1];
}

template<>
inline void NE10_CONJ<1, CPLX> (CPLX in[1])
{
    NE10_CONJ_S<CPLX> (in[0]);
}

/////////////////////////////////////////////////////////////////////////////
// Scaling
// If Macro NE10_DSP_CFFT_SCALING is not defined, these functions do nothing.
/////////////////////////////////////////////////////////////////////////////
template<int RADIX, int SIZE = RADIX>
struct NE10_FFT_SCALING {
    inline void operator() (CPLX scratch_out[RADIX])
    {
#ifdef NE10_DSP_CFFT_SCALING
        const int32x4_t one_by_RADIX =
        {
            (ne10_int32_t) floor(1.0 / RADIX * NE10_F2I32_MAX + 0.5f),
            (ne10_int32_t) floor(1.0 / RADIX * NE10_F2I32_MAX + 0.5f),
            (ne10_int32_t) floor(1.0 / RADIX * NE10_F2I32_MAX + 0.5f),
            (ne10_int32_t) floor(1.0 / RADIX * NE10_F2I32_MAX + 0.5f)
        };
        scratch_out[SIZE - 1].val[0] = vqrdmulhq_s32 (scratch_out[SIZE - 1].val[0], one_by_RADIX);
        scratch_out[SIZE - 1].val[1] = vqrdmulhq_s32 (scratch_out[SIZE - 1].val[1], one_by_RADIX);
        NE10_FFT_SCALING<RADIX, SIZE - 1> () (scratch_out);
#endif
    }
};

template<int RADIX>
struct NE10_FFT_SCALING<RADIX, 1> {
    inline void operator () (CPLX scratch_out[1])
    {
#ifdef NE10_DSP_CFFT_SCALING
        const int32x4_t one_by_RADIX =
        {
            (ne10_int32_t) floor(1.0 / RADIX * NE10_F2I32_MAX + 0.5f),
            (ne10_int32_t) floor(1.0 / RADIX * NE10_F2I32_MAX + 0.5f),
            (ne10_int32_t) floor(1.0 / RADIX * NE10_F2I32_MAX + 0.5f),
            (ne10_int32_t) floor(1.0 / RADIX * NE10_F2I32_MAX + 0.5f)
        };
        scratch_out[0].val[0] = vqrdmulhq_s32 (scratch_out[0].val[0], one_by_RADIX);
        scratch_out[0].val[1] = vqrdmulhq_s32 (scratch_out[0].val[1], one_by_RADIX);
#endif
    }
};

inline void NE10_CPX_ADD_NEON_S32 (CPLX &result, const CPLX a, const CPLX b)
{
    result.val[0] = vaddq_s32 (a.val[0], b.val[0]);
    result.val[1] = vaddq_s32 (a.val[1], b.val[1]);
}

inline void NE10_CPX_SUB_NEON_S32 (CPLX &result, const CPLX a, const CPLX b)
{
    result.val[0] = vsubq_s32 (a.val[0], b.val[0]);
    result.val[1] = vsubq_s32 (a.val[1], b.val[1]);
}

inline REAL NE10_HALF (REAL src)
{
    const int32x4_t CONST_HALF_NEON = { -1, -1, -1, -1};
    src = vshlq_s32 (src, CONST_HALF_NEON);
    return src;
}

///////////////////
// FFT Kernel
// F: Forward
// C: Complex
// U: Unscaled
//////////////////
template<int RADIX>
inline void NE10_FFT_FCU_NEON_S32 (CPLX [RADIX], const CPLX [RADIX]);

template<>
inline void NE10_FFT_FCU_NEON_S32<2> (CPLX scratch_out[2],
        const CPLX scratch_in[2])
{
    NE10_CPX_ADD_NEON_S32 (scratch_out[0], scratch_in[0], scratch_in[1]);
    NE10_CPX_SUB_NEON_S32 (scratch_out[1], scratch_in[0], scratch_in[1]);
}

template<>
inline void NE10_FFT_FCU_NEON_S32<3> (CPLX Fout[3],
        const CPLX Fin[3])
{
    CPLX scratch[4];

    Fout[0] = Fin[0];
    Fout[1] = Fin[1];
    Fout[2] = Fin[2];

    scratch[1] = Fout[1];
    scratch[2] = Fout[2];

    NE10_CPX_ADD_NEON_S32 (scratch[3], scratch[1], scratch[2]);
    NE10_CPX_SUB_NEON_S32 (scratch[0], scratch[1], scratch[2]);

    Fout[1].val[0] = Fout[0].val[0] - NE10_HALF (scratch[3].val[0]);
    Fout[1].val[1] = Fout[0].val[1] - NE10_HALF (scratch[3].val[1]);

    scratch[0].val[0] = NE10_S_MUL_NEON_S32 (scratch[0].val[0], TW_3IN_S32);
    scratch[0].val[1] = NE10_S_MUL_NEON_S32 (scratch[0].val[1], TW_3IN_S32);

    Fout[0].val[0] += scratch[3].val[0];
    Fout[0].val[1] += scratch[3].val[1];

    Fout[2].val[0] = Fout[1].val[0] + scratch[0].val[1];
    Fout[2].val[1] = Fout[1].val[1] - scratch[0].val[0];

    Fout[1].val[0] -= scratch[0].val[1];
    Fout[1].val[1] += scratch[0].val[0];
}

template<>
inline void NE10_FFT_FCU_NEON_S32<4> (CPLX scratch_out[4],
        const CPLX scratch_in[4])
{
    CPLX scratch[4];

    NE10_CPX_ADD_NEON_S32 (scratch[0], scratch_in[0], scratch_in[2]);
    NE10_CPX_SUB_NEON_S32 (scratch[1], scratch_in[0], scratch_in[2]);
    NE10_CPX_ADD_NEON_S32 (scratch[2], scratch_in[1], scratch_in[3]);
    NE10_CPX_SUB_NEON_S32 (scratch[3], scratch_in[1], scratch_in[3]);

    NE10_CPX_SUB_NEON_S32 (scratch_out[2], scratch[0], scratch[2]);
    NE10_CPX_ADD_NEON_S32 (scratch_out[0], scratch[0], scratch[2]);

    scratch_out[1].val[0] = scratch[1].val[0] + scratch[3].val[1];
    scratch_out[1].val[1] = scratch[1].val[1] - scratch[3].val[0];
    scratch_out[3].val[0] = scratch[1].val[0] - scratch[3].val[1];
    scratch_out[3].val[1] = scratch[1].val[1] + scratch[3].val[0];
}

template<>
inline void NE10_FFT_FCU_NEON_S32<5> (CPLX Fout[5],
        const CPLX Fin[5])
{
    CPLX scratch[13], scratch_in[5];

    scratch_in[0] = Fin[0];
    scratch_in[1] = Fin[1];
    scratch_in[2] = Fin[2];
    scratch_in[3] = Fin[3];
    scratch_in[4] = Fin[4];

    scratch[0] = scratch_in[0];
    scratch[1] = scratch_in[1];
    scratch[2] = scratch_in[2];
    scratch[3] = scratch_in[3];
    scratch[4] = scratch_in[4];

    NE10_CPX_ADD_NEON_S32 (scratch[ 7], scratch[1], scratch[4]);
    NE10_CPX_SUB_NEON_S32 (scratch[10], scratch[1], scratch[4]);
    NE10_CPX_ADD_NEON_S32 (scratch[ 8], scratch[2], scratch[3]);
    NE10_CPX_SUB_NEON_S32 (scratch[ 9], scratch[2], scratch[3]);

    scratch_in[0].val[0] += scratch[7].val[0] + scratch[8].val[0];
    scratch_in[0].val[1] += scratch[7].val[1] + scratch[8].val[1];

    scratch[5].val[0] = scratch[0].val[0]
        + NE10_S_MUL_NEON_S32 (scratch[7].val[0], TW_5A_S32.r)
        + NE10_S_MUL_NEON_S32 (scratch[8].val[0], TW_5B_S32.r);
    scratch[5].val[1] = scratch[0].val[1]
        + NE10_S_MUL_NEON_S32 (scratch[7].val[1], TW_5A_S32.r)
        + NE10_S_MUL_NEON_S32 (scratch[8].val[1], TW_5B_S32.r);

    scratch[6].val[0] = NE10_S_MUL_NEON_S32 (scratch[10].val[1], TW_5A_S32.i)
        + NE10_S_MUL_NEON_S32 (scratch[9].val[1], TW_5B_S32.i);
    scratch[6].val[1] = -NE10_S_MUL_NEON_S32 (scratch[10].val[0], TW_5A_S32.i)
        - NE10_S_MUL_NEON_S32 (scratch[9].val[0], TW_5B_S32.i);

    NE10_CPX_SUB_NEON_S32 (scratch_in[1], scratch[5], scratch[6]);
    NE10_CPX_ADD_NEON_S32 (scratch_in[4], scratch[5], scratch[6]);

    scratch[11].val[0] = scratch[0].val[0]
        + NE10_S_MUL_NEON_S32 (scratch[7].val[0], TW_5B_S32.r)
        + NE10_S_MUL_NEON_S32 (scratch[8].val[0], TW_5A_S32.r);
    scratch[11].val[1] = scratch[0].val[1]
        + NE10_S_MUL_NEON_S32 (scratch[7].val[1], TW_5B_S32.r)
        + NE10_S_MUL_NEON_S32 (scratch[8].val[1], TW_5A_S32.r);

    scratch[12].val[0] = -NE10_S_MUL_NEON_S32 (scratch[10].val[1], TW_5B_S32.i)
        + NE10_S_MUL_NEON_S32 (scratch[9].val[1], TW_5A_S32.i);
    scratch[12].val[1] = NE10_S_MUL_NEON_S32 (scratch[10].val[0], TW_5B_S32.i)
        - NE10_S_MUL_NEON_S32 (scratch[9].val[0], TW_5A_S32.i);

    NE10_CPX_ADD_NEON_S32 (scratch_in[2], scratch[11], scratch[12]);
    NE10_CPX_SUB_NEON_S32 (scratch_in[3], scratch[11], scratch[12]);

    Fout[0] = scratch_in[0];
    Fout[1] = scratch_in[1];
    Fout[2] = scratch_in[2];
    Fout[3] = scratch_in[3];
    Fout[4] = scratch_in[4];
}

////////////////////////////////////
// Following are butterfly functions
////////////////////////////////////
template<ne10_int32_t RADIX, bool is_first_stage, bool is_inverse, bool is_scaled>
static __attribute__ ((noinline)) void ne10_radix_butterfly_int32_neon (
        CPLX *Fout,
        const CPLX *Fin,
        const ne10_fft_cpx_int32_t *twiddles,
        const ne10_int32_t fstride,
        const ne10_int32_t out_step,
        const ne10_int32_t nfft)
{
    PRINT_HIT;
    const ne10_int32_t in_step = nfft / RADIX;
    ne10_int32_t f_count;
    ne10_int32_t m_count;

    for (f_count = fstride; f_count > 0; f_count--)
    {
        for (m_count = out_step; m_count > 0; m_count--)
        {
            CPLX in[RADIX];
            CPLX out[RADIX];

            NE10_LOAD_BY_STEP<RADIX, CPLX> (in, Fin, in_step);

            if (is_inverse)
            {
                NE10_CONJ<RADIX> (in);
            }

            if (is_scaled)
            {
                NE10_FFT_SCALING<RADIX> () (in);
            }

            if (!is_first_stage)
            {
                NE10_LOAD_TW_AND_MUL<RADIX> (in, twiddles, out_step);
            }

            NE10_FFT_FCU_NEON_S32<RADIX> (out, in);

            if (is_inverse)
            {
                NE10_CONJ<RADIX> (out);
            }

            NE10_STORE_BY_STEP<RADIX, CPLX> (Fout, out, out_step);

            Fin++;

            if (!is_first_stage)
            {
                Fout++;
                twiddles++;
            }
            else
            {
                Fout += RADIX;
            }
        }
        if (!is_first_stage)
        {
            twiddles -= out_step;
            Fout += (RADIX - 1) * out_step;
        }
    }
}

template<bool is_inverse, bool is_scaled>
static void ne10_mixed_radix_generic_butterfly_int32_neon_impl (CPLX *Fout,
        const CPLX *Fin,
        const ne10_int32_t *factors,
        const ne10_fft_cpx_int32_t *twiddles,
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
        ne10_radix_butterfly_int32_neon<2, true, is_inverse, is_scaled> (Fout, Fin,
                NULL,
                fstride, 1, nfft);
        break;
    case 4:
        ne10_radix_butterfly_int32_neon<4, true, is_inverse, is_scaled> (Fout, Fin,
                NULL,
                fstride, 1, nfft);
        break;
    case 3:
        ne10_radix_butterfly_int32_neon<3, true, is_inverse, is_scaled> (Fout, Fin,
                NULL,
                fstride, 1, nfft);
        break;
    case 5:
        ne10_radix_butterfly_int32_neon<5, true, is_inverse, is_scaled> (Fout, Fin,
                NULL,
                fstride, 1, nfft);
        break;
    }

    stage_count--;
    if (!stage_count) // finish
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
            ne10_radix_butterfly_int32_neon<2, false, is_inverse, is_scaled> (Fout, buffer,
                    twiddles,
                    fstride, mstride, nfft);
            break;
        case 3:
            ne10_radix_butterfly_int32_neon<3, false, is_inverse, is_scaled> (Fout, buffer,
                    twiddles,
                    fstride, mstride, nfft);
            break;
        case 4:
            ne10_radix_butterfly_int32_neon<4, false, is_inverse, is_scaled> (Fout, buffer,
                    twiddles,
                    fstride, mstride, nfft);
            break;
        case 5:
            ne10_radix_butterfly_int32_neon<5, false, is_inverse, is_scaled> (Fout, buffer,
                    twiddles, fstride, mstride, nfft);
            break;
        } // switch (radix)

        twiddles += mstride * (radix - 1);
        mstride *= radix;

        stage_count--;
        radix = factors[ stage_count << 1 ];
    } // while (stage_count)
}

template<bool is_inverse, bool is_scaled>
static void ne10_c2c_1d_last_stage_neon (CPLX *Fout,
        const CPLX *Fin,
        const ne10_fft_cpx_int32_t *twiddles,
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

        for (m_count = out_step / NE10_FFT_PARA_LEVEL; m_count > 0; m_count--)
        {
            scratch_in[0] = NE10_CPLX_LOAD (Fin + 0);
            scratch_in[1] = NE10_CPLX_LOAD (Fin + 1);
            scratch_in[2] = NE10_CPLX_LOAD (Fin + 2);
            scratch_in[3] = NE10_CPLX_LOAD (Fin + 3);

            if (is_scaled)
            {
                NE10_FFT_SCALING<4> () (scratch_in);
            }

            // Transpose
            {
                float32x4x2_t scratch0, scratch_in0;
                float32x4x2_t scratch1, scratch_in1;
                float32x4x2_t scratch2, scratch_in2;
                float32x4x2_t scratch3, scratch_in3;

                scratch_in0.val[0] = vreinterpretq_f32_s32 (scratch_in[0].val[0]);
                scratch_in1.val[0] = vreinterpretq_f32_s32 (scratch_in[1].val[0]);
                scratch_in2.val[0] = vreinterpretq_f32_s32 (scratch_in[2].val[0]);
                scratch_in3.val[0] = vreinterpretq_f32_s32 (scratch_in[3].val[0]);
                scratch_in0.val[1] = vreinterpretq_f32_s32 (scratch_in[0].val[1]);
                scratch_in1.val[1] = vreinterpretq_f32_s32 (scratch_in[1].val[1]);
                scratch_in2.val[1] = vreinterpretq_f32_s32 (scratch_in[2].val[1]);
                scratch_in3.val[1] = vreinterpretq_f32_s32 (scratch_in[3].val[1]);

                NE10_RADIX4X4C_TRANSPOSE_NEON (scratch, scratch_in);

                scratch_in[0].val[0] = vreinterpretq_s32_f32 (scratch0.val[0]);
                scratch_in[1].val[0] = vreinterpretq_s32_f32 (scratch1.val[0]);
                scratch_in[2].val[0] = vreinterpretq_s32_f32 (scratch2.val[0]);
                scratch_in[3].val[0] = vreinterpretq_s32_f32 (scratch3.val[0]);
                scratch_in[0].val[1] = vreinterpretq_s32_f32 (scratch0.val[1]);
                scratch_in[1].val[1] = vreinterpretq_s32_f32 (scratch1.val[1]);
                scratch_in[2].val[1] = vreinterpretq_s32_f32 (scratch2.val[1]);
                scratch_in[3].val[1] = vreinterpretq_s32_f32 (scratch3.val[1]);
            }

            if (is_inverse)
            {
                NE10_CONJ<4, CPLX> (scratch_in);
            }

            // Not first stage
            {
                CPLX scratch_tw[3];

                scratch_tw[0] = NE10_CPLX_LOAD (twiddles + 0 * out_step);
                scratch_tw[1] = NE10_CPLX_LOAD (twiddles + 1 * out_step);
                scratch_tw[2] = NE10_CPLX_LOAD (twiddles + 2 * out_step);

                NE10_CPX_MUL_NEON_S32 (scratch_in[1], scratch_in[1], scratch_tw[0]);
                NE10_CPX_MUL_NEON_S32 (scratch_in[2], scratch_in[2], scratch_tw[1]);
                NE10_CPX_MUL_NEON_S32 (scratch_in[3], scratch_in[3], scratch_tw[2]);
            }

            NE10_FFT_FCU_NEON_S32<4> (scratch_out, scratch_in);

            if (is_inverse)
            {
                NE10_CONJ<4, CPLX> (scratch_out);
            }

            // Store.
            {
                ne10_fft_cpx_int32_t *Fout_cpx;
                Fout_cpx = (ne10_fft_cpx_int32_t *) Fout;

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
    const ne10_fft_cpx_int32_t *Fin_s  = (ne10_fft_cpx_int32_t *) Fin;
    ne10_fft_cpx_int32_t *Fout_s = (ne10_fft_cpx_int32_t *) Fout;
    for (m_count = out_step % 4; m_count > 0; m_count--)
    {
        ne10_fft_cpx_int32_t scratch_in[4];
        ne10_fft_cpx_int32_t scratch_tw[4];

        scratch_in[0] = Fin_s[0];
        scratch_in[1] = Fin_s[1];
        scratch_in[2] = Fin_s[2];
        scratch_in[3] = Fin_s[3];

        if (is_scaled)
        {
            scratch_in[0].r = scratch_in[0].r >> 2;
            scratch_in[1].r = scratch_in[1].r >> 2;
            scratch_in[2].r = scratch_in[2].r >> 2;
            scratch_in[3].r = scratch_in[3].r >> 2;

            scratch_in[0].i = scratch_in[0].i >> 2;
            scratch_in[1].i = scratch_in[1].i >> 2;
            scratch_in[2].i = scratch_in[2].i >> 2;
            scratch_in[3].i = scratch_in[3].i >> 2;
        }


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

        NE10_CPX_MUL_S32 (scratch_in[1], scratch_in[1], scratch_tw[0]);
        NE10_CPX_MUL_S32 (scratch_in[2], scratch_in[2], scratch_tw[1]);
        NE10_CPX_MUL_S32 (scratch_in[3], scratch_in[3], scratch_tw[2]);

        FFT_FCU<4> (scratch_in, scratch_in);

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

#endif
