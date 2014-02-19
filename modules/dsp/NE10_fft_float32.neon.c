/*
 *  Copyright 2013-14 ARM Limited
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
 * NE10 Library : dsp/NE10_fft_float32.neon.c
 */

#include <arm_neon.h>

#include "NE10_types.h"
#include "NE10_macros.h"
#include "NE10_fft.h"

static inline void ne10_fft4_forward_float32 (ne10_fft_cpx_float32_t * Fout,
        ne10_fft_cpx_float32_t * Fin)

{
    ne10_float32_t s0_r, s0_i, s1_r, s1_i, s2_r, s2_i;
    ne10_float32_t tmp_r, tmp_i;

    s2_r = Fin[0].r - Fin[2].r;
    s2_i = Fin[0].i - Fin[2].i;

    tmp_r = Fin[0].r + Fin[2].r;
    tmp_i = Fin[0].i + Fin[2].i;

    s0_r = Fin[1].r + Fin[3].r;
    s0_i = Fin[1].i + Fin[3].i;

    s1_r = Fin[1].r - Fin[3].r;
    s1_i = Fin[1].i - Fin[3].i;
    Fout[2].r = tmp_r - s0_r;
    Fout[2].i = tmp_i - s0_i;
    Fout[0].r = tmp_r + s0_r;
    Fout[0].i = tmp_i + s0_i;

    Fout[1].r = s2_r + s1_i;
    Fout[1].i = s2_i - s1_r;
    Fout[3].r = s2_r - s1_i;
    Fout[3].i = s2_i + s1_r;
}

static inline void ne10_fft4_backward_float32 (ne10_fft_cpx_float32_t * Fout,
        ne10_fft_cpx_float32_t * Fin)

{
    ne10_float32_t s0_r, s0_i, s1_r, s1_i, s2_r, s2_i;
    ne10_float32_t tmp_r, tmp_i;

    s2_r = Fin[0].r - Fin[2].r;
    s2_i = Fin[0].i - Fin[2].i;

    tmp_r = Fin[0].r + Fin[2].r;
    tmp_i = Fin[0].i + Fin[2].i;

    s0_r = Fin[1].r + Fin[3].r;
    s0_i = Fin[1].i + Fin[3].i;

    s1_r = Fin[1].r - Fin[3].r;
    s1_i = Fin[1].i - Fin[3].i;
    Fout[2].r = tmp_r - s0_r;
    Fout[2].i = tmp_i - s0_i;
    Fout[0].r = tmp_r + s0_r;
    Fout[0].i = tmp_i + s0_i;

    Fout[1].r = s2_r - s1_i;
    Fout[1].i = s2_i + s1_r;
    Fout[3].r = s2_r + s1_i;
    Fout[3].i = s2_i - s1_r;
}

static inline void ne10_fft8_forward_float32 (ne10_fft_cpx_float32_t * Fout,
        ne10_fft_cpx_float32_t * Fin)

{
    ne10_float32_t s0_r, s0_i, s1_r, s1_i, s2_r, s2_i, s3_r, s3_i, s4_r, s4_i, s5_r, s5_i, s6_r, s6_i, s7_r, s7_i;
    ne10_float32_t t0_r, t0_i, t1_r, t1_i, t2_r, t2_i, t3_r, t3_i, t4_r, t4_i, t5_r, t5_i;
    ne10_fft_cpx_float32_t * F;
    const ne10_float32_t TW_81 = 0.70710678;

    s0_r = Fin[0].r + Fin[4].r;
    s0_i = Fin[0].i + Fin[4].i;
    s1_r = Fin[0].r - Fin[4].r;
    s1_i = Fin[0].i - Fin[4].i;
    s2_r = Fin[1].r + Fin[5].r;
    s2_i = Fin[1].i + Fin[5].i;
    s3_r = Fin[1].r - Fin[5].r;
    s3_i = Fin[1].i - Fin[5].i;
    s4_r = Fin[2].r + Fin[6].r;
    s4_i = Fin[2].i + Fin[6].i;
    s5_r = Fin[2].r - Fin[6].r;
    s5_i = Fin[2].i - Fin[6].i;
    s6_r = Fin[3].r + Fin[7].r;
    s6_i = Fin[3].i + Fin[7].i;
    s7_r = Fin[3].r - Fin[7].r;
    s7_i = Fin[3].i - Fin[7].i;

    t0_r = s0_r - s4_r;
    t0_i = s0_i - s4_i;
    t1_r = s0_r + s4_r;
    t1_i = s0_i + s4_i;
    t2_r = s2_r + s6_r;
    t2_i = s2_i + s6_i;
    t3_r = s2_r - s6_r;
    t3_i = s2_i - s6_i;
    Fout[0].r = t1_r + t2_r;
    Fout[0].i = t1_i + t2_i;
    Fout[4].r = t1_r - t2_r;
    Fout[4].i = t1_i - t2_i;
    Fout[2].r = t0_r + t3_i;
    Fout[2].i = t0_i - t3_r;
    Fout[6].r = t0_r - t3_i;
    Fout[6].i = t0_i + t3_r;

    t4_r =  (s3_r + s3_i) * TW_81;
    t4_i = -(s3_r - s3_i) * TW_81;
    t5_r =  (s7_r - s7_i) * TW_81;
    t5_i =  (s7_r + s7_i) * TW_81;

    t0_r = s1_r - s5_i;
    t0_i = s1_i + s5_r;
    t1_r = s1_r + s5_i;
    t1_i = s1_i - s5_r;
    t2_r = t4_r - t5_r;
    t2_i = t4_i - t5_i;
    t3_r = t4_r + t5_r;
    t3_i = t4_i + t5_i;
    Fout[1].r = t1_r + t2_r;
    Fout[1].i = t1_i + t2_i;
    Fout[5].r = t1_r - t2_r;
    Fout[5].i = t1_i - t2_i;
    Fout[3].r = t0_r + t3_i;
    Fout[3].i = t0_i - t3_r;
    Fout[7].r = t0_r - t3_i;
    Fout[7].i = t0_i + t3_r;
}

static inline void ne10_fft8_backward_float32 (ne10_fft_cpx_float32_t * Fout,
        ne10_fft_cpx_float32_t * Fin)

{
    ne10_float32_t s0_r, s0_i, s1_r, s1_i, s2_r, s2_i, s3_r, s3_i, s4_r, s4_i, s5_r, s5_i, s6_r, s6_i, s7_r, s7_i;
    ne10_float32_t t0_r, t0_i, t1_r, t1_i, t2_r, t2_i, t3_r, t3_i, t4_r, t4_i, t5_r, t5_i;
    ne10_fft_cpx_float32_t * F;
    const ne10_float32_t TW_81 = 0.70710678;

    s0_r = Fin[0].r + Fin[4].r;
    s0_i = Fin[0].i + Fin[4].i;
    s1_r = Fin[0].r - Fin[4].r;
    s1_i = Fin[0].i - Fin[4].i;
    s2_r = Fin[1].r + Fin[5].r;
    s2_i = Fin[1].i + Fin[5].i;
    s3_r = Fin[1].r - Fin[5].r;
    s3_i = Fin[1].i - Fin[5].i;
    s4_r = Fin[2].r + Fin[6].r;
    s4_i = Fin[2].i + Fin[6].i;
    s5_r = Fin[2].r - Fin[6].r;
    s5_i = Fin[2].i - Fin[6].i;
    s6_r = Fin[3].r + Fin[7].r;
    s6_i = Fin[3].i + Fin[7].i;
    s7_r = Fin[3].r - Fin[7].r;
    s7_i = Fin[3].i - Fin[7].i;

    t0_r = s0_r - s4_r;
    t0_i = s0_i - s4_i;
    t1_r = s0_r + s4_r;
    t1_i = s0_i + s4_i;
    t2_r = s2_r + s6_r;
    t2_i = s2_i + s6_i;
    t3_r = s2_r - s6_r;
    t3_i = s2_i - s6_i;
    Fout[0].r = t1_r + t2_r;
    Fout[0].i = t1_i + t2_i;
    Fout[4].r = t1_r - t2_r;
    Fout[4].i = t1_i - t2_i;
    Fout[2].r = t0_r - t3_i;
    Fout[2].i = t0_i + t3_r;
    Fout[6].r = t0_r + t3_i;
    Fout[6].i = t0_i - t3_r;

    t4_r = (s3_r - s3_i) * TW_81;
    t4_i = (s3_r + s3_i) * TW_81;
    t5_r = (s7_r + s7_i) * TW_81;
    t5_i = -(s7_r - s7_i) * TW_81;

    t0_r = s1_r + s5_i;
    t0_i = s1_i - s5_r;
    t1_r = s1_r - s5_i;
    t1_i = s1_i + s5_r;
    t2_r = t4_r - t5_r;
    t2_i = t4_i - t5_i;
    t3_r = t4_r + t5_r;
    t3_i = t4_i + t5_i;
    Fout[1].r = t1_r + t2_r;
    Fout[1].i = t1_i + t2_i;
    Fout[5].r = t1_r - t2_r;
    Fout[5].i = t1_i - t2_i;
    Fout[3].r = t0_r - t3_i;
    Fout[3].i = t0_i + t3_r;
    Fout[7].r = t0_r + t3_i;
    Fout[7].i = t0_i - t3_r;
}

static inline ne10_data_bitreversal_butterfly4_forward_float32 (ne10_fft_cpx_float32_t * out,
        ne10_fft_cpx_float32_t * in,
        ne10_int32_t fstride,
        ne10_int32_t stride1)
{
    ne10_float32_t t0_r, t0_i, t1_r, t1_i, t2_r, t2_i, t3_r, t3_i, t4_r, t4_i, t5_r, t5_i;
    t2_r = in[0].r - in[fstride * 2].r;
    t2_i = in[0].i - in[fstride * 2].i;
    t3_r = in[0].r + in[fstride * 2].r;
    t3_i = in[0].i + in[fstride * 2].i;
    t0_r = in[fstride].r + in[fstride * 3].r;
    t0_i = in[fstride].i + in[fstride * 3].i;
    t1_r = in[fstride].r - in[fstride * 3].r;
    t1_i = in[fstride].i - in[fstride * 3].i;
    out[2].r = t3_r - t0_r;
    out[2].i = t3_i - t0_i;
    out[0].r = t3_r + t0_r;
    out[0].i = t3_i + t0_i;
    out[1].r = t2_r + t1_i;
    out[1].i = t2_i - t1_r;
    out[3].r = t2_r - t1_i;
    out[3].i = t2_i + t1_r;
}

static inline ne10_data_bitreversal_butterfly4_backward_float32 (ne10_fft_cpx_float32_t * out,
        ne10_fft_cpx_float32_t * in,
        ne10_int32_t fstride,
        ne10_int32_t stride1)
{
    ne10_float32_t t0_r, t0_i, t1_r, t1_i, t2_r, t2_i, t3_r, t3_i, t4_r, t4_i, t5_r, t5_i;
    t2_r = in[0].r - in[fstride * 2].r;
    t2_i = in[0].i - in[fstride * 2].i;
    t3_r = in[0].r + in[fstride * 2].r;
    t3_i = in[0].i + in[fstride * 2].i;
    t0_r = in[fstride].r + in[fstride * 3].r;
    t0_i = in[fstride].i + in[fstride * 3].i;
    t1_r = in[fstride].r - in[fstride * 3].r;
    t1_i = in[fstride].i - in[fstride * 3].i;
    out[2].r = t3_r - t0_r;
    out[2].i = t3_i - t0_i;
    out[0].r = t3_r + t0_r;
    out[0].i = t3_i + t0_i;
    out[1].r = t2_r - t1_i;
    out[1].i = t2_i + t1_r;
    out[3].r = t2_r + t1_i;
    out[3].i = t2_i - t1_r;
}

#define  ne10_data_bitreversal64_butterfly4_float32(inverse) \
static void ne10_data_bitreversal64_butterfly4_##inverse##_float32 (ne10_fft_cpx_float32_t * Fout, \
        ne10_fft_cpx_float32_t * Fin) \
{ \
    ne10_int32_t i, p; \
    ne10_int32_t fstride; \
    ne10_int32_t stride1; \
    ne10_int32_t t0_r, t0_i, t1_r, t1_i, t2_r, t2_i, t3_r, t3_i, t4_r, t4_i, t5_r, t5_i; \
    ne10_fft_cpx_float32_t * F; \
    ne10_fft_cpx_float32_t * in; \
    fstride = 16; \
    F = Fout; \
    in = Fin; \
    stride1 = fstride >> 2; \
    for (i = 0; i < 4; i++) \
    { \
        in = &Fin[i]; \
        for (p = 0; p < 4; p++) \
        { \
            ne10_data_bitreversal_butterfly4_##inverse##_float32 (F, in, fstride, stride1); \
            in += stride1; \
            F += 4; \
        } \
    } \
}

#define  ne10_data_bitreversal256_butterfly4_float32(inverse) \
static void ne10_data_bitreversal256_butterfly4_##inverse##_float32 (ne10_fft_cpx_float32_t * Fout, \
        ne10_fft_cpx_float32_t * Fin) \
{ \
    ne10_int32_t i, j, p; \
    ne10_int32_t fstride; \
    ne10_int32_t stride1; \
    ne10_int32_t stride2; \
    ne10_int32_t t0_r, t0_i, t1_r, t1_i, t2_r, t2_i, t3_r, t3_i, t4_r, t4_i, t5_r, t5_i; \
    ne10_fft_cpx_float32_t * F; \
    ne10_fft_cpx_float32_t * in; \
    fstride = 64; \
    F = Fout; \
    in = Fin; \
    stride1 = fstride >> 2; \
    stride2 = stride1 >> 2; \
    for (j = 0; j < 4; j++) \
    { \
        for (i = 0; i < 4; i++) \
        { \
            in = &Fin[j + i * stride2]; \
            for (p = 0; p < 4; p++) \
            { \
                ne10_data_bitreversal_butterfly4_##inverse##_float32 (F, in, fstride, stride1); \
                in += stride1; \
                F += 4; \
            } \
        } \
    } \
}

#define  ne10_data_bitreversal1024_butterfly4_float32(inverse) \
static void ne10_data_bitreversal1024_butterfly4_##inverse##_float32 (ne10_fft_cpx_float32_t * Fout, \
        ne10_fft_cpx_float32_t * Fin) \
{ \
    ne10_int32_t i, j, k, p; \
    ne10_int32_t fstride; \
    ne10_int32_t stride1; \
    ne10_int32_t stride2; \
    ne10_int32_t stride3; \
    ne10_int32_t t0_r, t0_i, t1_r, t1_i, t2_r, t2_i, t3_r, t3_i, t4_r, t4_i, t5_r, t5_i; \
    ne10_fft_cpx_float32_t * F; \
    ne10_fft_cpx_float32_t * in; \
    fstride = 256; \
    F = Fout; \
    in = Fin; \
    stride1 = fstride >> 2; \
    stride2 = stride1 >> 2; \
    stride3 = stride2 >> 2; \
    for (k = 0; k < 4; k++) \
    { \
        for (j = 0; j < 4; j++) \
        { \
            for (i = 0; i < 4; i++) \
            { \
                in = &Fin[k + j * stride3 + i * stride2]; \
                for (p = 0; p < 4; p++) \
                { \
                    ne10_data_bitreversal_butterfly4_##inverse##_float32 (F, in, fstride, stride1); \
                    in += stride1; \
                    F += 4; \
                } \
            } \
        } \
    } \
}

#define  ne10_data_bitreversal4096_butterfly4_float32(inverse) \
static void ne10_data_bitreversal4096_butterfly4_##inverse##_float32 (ne10_fft_cpx_float32_t * Fout, \
        ne10_fft_cpx_float32_t * Fin) \
{ \
    ne10_int32_t i, j, k, l, p; \
    ne10_int32_t fstride; \
    ne10_int32_t stride1; \
    ne10_int32_t stride2; \
    ne10_int32_t stride3; \
    ne10_int32_t stride4; \
    ne10_int32_t t0_r, t0_i, t1_r, t1_i, t2_r, t2_i, t3_r, t3_i, t4_r, t4_i, t5_r, t5_i; \
    ne10_fft_cpx_float32_t * F; \
    ne10_fft_cpx_float32_t * in; \
    fstride = 1024; \
    F = Fout; \
    in = Fin; \
    stride1 = fstride >> 2; \
    stride2 = stride1 >> 2; \
    stride3 = stride2 >> 2; \
    stride4 = stride3 >> 2; \
    for (l = 0; l < 4; l++) \
    { \
        for (k = 0; k < 4; k++) \
        { \
            for (j = 0; j < 4; j++) \
            { \
                for (i = 0; i < 4; i++) \
                { \
                    in = &Fin[l + k*stride4 + j * stride3 + i * stride2]; \
                    for (p = 0; p < 4; p++) \
                    { \
                        ne10_data_bitreversal_butterfly4_##inverse##_float32 (F, in, fstride, stride1); \
                        in += stride1; \
                        F += 4; \
                    } \
                } \
            } \
        } \
    } \
}

#define  ne10_butterfly_length_even_power2_float32_neon(inverse) \
static void ne10_butterfly_##inverse##_length_even_power2_float32_neon (ne10_fft_cpx_float32_t * Fout, \
        ne10_fft_cpx_float32_t * Fin, \
        ne10_int32_t * factors, \
        ne10_fft_cpx_float32_t * twiddles) \
{ \
    ne10_int32_t fstride = factors[1]; \
    if (fstride == 16) \
        ne10_data_bitreversal64_butterfly4_##inverse##_float32 (Fout, Fin); \
    else if (fstride == 64) \
        ne10_data_bitreversal256_butterfly4_##inverse##_float32 (Fout, Fin); \
    else if (fstride == 256) \
        ne10_data_bitreversal1024_butterfly4_##inverse##_float32 (Fout, Fin); \
    else if (fstride == 1024) \
        ne10_data_bitreversal4096_butterfly4_##inverse##_float32 (Fout, Fin); \
    ne10_radix4_butterfly_##inverse##_float32_neon (Fout, factors, twiddles);\
}

ne10_data_bitreversal64_butterfly4_float32 (forward)
ne10_data_bitreversal64_butterfly4_float32 (backward)
ne10_data_bitreversal256_butterfly4_float32 (forward)
ne10_data_bitreversal256_butterfly4_float32 (backward)
ne10_data_bitreversal1024_butterfly4_float32 (forward)
ne10_data_bitreversal1024_butterfly4_float32 (backward)
ne10_data_bitreversal4096_butterfly4_float32 (forward)
ne10_data_bitreversal4096_butterfly4_float32 (backward)

ne10_butterfly_length_even_power2_float32_neon (forward)
ne10_butterfly_length_even_power2_float32_neon (backward)

static inline ne10_data_bitreversal_butterfly2_float32_neon (ne10_fft_cpx_float32_t * out,
        ne10_fft_cpx_float32_t * in,
        ne10_int32_t fstride,
        ne10_int32_t stride1)
{
    float32x2_t d_in0_0, d_in0_1;
    float32x2_t d_in1_0, d_in1_1;
    float32x2_t d_in2_0, d_in2_1;
    float32x2_t d_in3_0, d_in3_1;
    float32x4_t q_in01_0, q_in01_1, q_in23_0, q_in23_1;
    float32x4_t q_out01_0, q_out01_1, q_out23_0, q_out23_1;
    /* load loop */
    d_in0_0 = vld1_f32 ( (float32_t*) (&in[0]));
    d_in0_1 = vld1_f32 ( (float32_t*) (&in[fstride]));
    d_in1_0 = vld1_f32 ( (float32_t*) (&in[stride1]));
    d_in1_1 = vld1_f32 ( (float32_t*) (&in[stride1 + fstride]));
    d_in2_0 = vld1_f32 ( (float32_t*) (&in[stride1 * 2]));
    d_in2_1 = vld1_f32 ( (float32_t*) (&in[stride1 * 2 + fstride]));
    d_in3_0 = vld1_f32 ( (float32_t*) (&in[stride1 * 3]));
    d_in3_1 = vld1_f32 ( (float32_t*) (&in[stride1 * 3 + fstride]));
    /* calculate loop */
    q_in01_0 = vcombine_f32 (d_in0_0, d_in1_0);
    q_in01_1 = vcombine_f32 (d_in0_1, d_in1_1);
    q_in23_0 = vcombine_f32 (d_in2_0, d_in3_0);
    q_in23_1 = vcombine_f32 (d_in2_1, d_in3_1);
    q_out01_0 = vaddq_f32 (q_in01_0, q_in01_1);
    q_out01_1 = vsubq_f32 (q_in01_0, q_in01_1);
    q_out23_0 = vaddq_f32 (q_in23_0, q_in23_1);
    q_out23_1 = vsubq_f32 (q_in23_0, q_in23_1);
    /* store loop */
    vst1q_f32 ( (float32_t*) (&out[0]), vcombine_f32 (vget_low_f32 (q_out01_0), vget_low_f32 (q_out01_1)));
    vst1q_f32 ( (float32_t*) (&out[2]), vcombine_f32 (vget_high_f32 (q_out01_0), vget_high_f32 (q_out01_1)));
    vst1q_f32 ( (float32_t*) (&out[4]), vcombine_f32 (vget_low_f32 (q_out23_0), vget_low_f32 (q_out23_1)));
    vst1q_f32 ( (float32_t*) (&out[6]), vcombine_f32 (vget_high_f32 (q_out23_0), vget_high_f32 (q_out23_1)));
}

static void ne10_data_bitreversal32_float32_neon (ne10_fft_cpx_float32_t * Fout,
        ne10_fft_cpx_float32_t * Fin)
{
    ne10_int32_t i;
    ne10_int32_t fstride;

    ne10_fft_cpx_float32_t * F;
    ne10_fft_cpx_float32_t * in;
    ne10_int32_t stride1;
    ne10_int32_t stride2;

    // get the input, resort, calculate the first stage
    fstride = 16;

    F = Fout;
    in = Fin;
    stride1 = fstride >> 2;
    stride2 = stride1 >> 2;
    for (i = 0; i < 4; i++)
    {
        ne10_data_bitreversal_butterfly2_float32_neon (F, in, fstride, stride1);
        F += 8;
        in += stride2;
    }
}

static void ne10_data_bitreversal128_float32_neon (ne10_fft_cpx_float32_t * Fout,
        ne10_fft_cpx_float32_t * Fin)
{
    ne10_int32_t i, j;
    ne10_int32_t fstride;
    ne10_int32_t stride1;
    ne10_int32_t stride2;

    ne10_fft_cpx_float32_t * F;
    ne10_fft_cpx_float32_t * in;

    // get the input, resort, calculate the first stage
    fstride = 64;
    F = Fout;
    stride1 = fstride >> 2;
    stride2 = stride1 >> 2;
    for (j = 0; j < 4; j++)
    {
        in = &Fin[j];
        for (i = 0; i < 4; i++)
        {
            ne10_data_bitreversal_butterfly2_float32_neon (F, in, fstride, stride1);
            F += 8;
            in += stride2;
        }
    }
}


static void ne10_data_bitreversal512_float32_neon (ne10_fft_cpx_float32_t * Fout,
        ne10_fft_cpx_float32_t * Fin)
{
    ne10_int32_t i, j, k;
    ne10_int32_t fstride;
    ne10_int32_t stride1;
    ne10_int32_t stride2;
    ne10_int32_t stride3;

    ne10_fft_cpx_float32_t * F;
    ne10_fft_cpx_float32_t * in;

    // get the input, resort, calculate the first stage
    fstride = 256;

    F = Fout;
    stride1 = fstride >> 2;
    stride2 = stride1 >> 2;
    stride3 = stride2 >> 2;
    for (k = 0; k < 4; k++)
    {
        for (j = 0; j < 4; j++)
        {
            in = &Fin[k + j * stride3];
            for (i = 0; i < 4; i++)
            {
                ne10_data_bitreversal_butterfly2_float32_neon (F, in, fstride, stride1);
                F += 8;
                in += stride2;
            }
        }
    }
}
static void ne10_data_bitreversal2048_float32_neon (ne10_fft_cpx_float32_t * Fout,
        ne10_fft_cpx_float32_t * Fin)
{
    ne10_int32_t i, j, k, l;
    ne10_int32_t fstride;
    ne10_int32_t stride1;
    ne10_int32_t stride2;
    ne10_int32_t stride3;
    ne10_int32_t stride4;

    ne10_fft_cpx_float32_t * F;
    ne10_fft_cpx_float32_t * in;

    // get the input, resort, calculate the first stage
    fstride = 1024;

    F = Fout;
    stride1 = fstride >> 2;
    stride2 = stride1 >> 2;
    stride3 = stride2 >> 2;
    stride4 = stride3 >> 2;
    for (l = 0; l < 4; l++)
    {
        for (k = 0; k < 4; k++)
        {
            for (j = 0; j < 4; j++)
            {
                in = &Fin[l + k * stride4 + j * stride3];
                for (i = 0; i < 4; i++)
                {
                    ne10_data_bitreversal_butterfly2_float32_neon (F, in, fstride, stride1);
                    F += 8;
                    in += stride2;
                }
            }
        }
    }
}


#define  ne10_butterfly_length_odd_power2_float32_neon(inverse) \
static void ne10_butterfly_##inverse##_length_odd_power2_float32_neon (ne10_fft_cpx_float32_t * Fout, \
        ne10_fft_cpx_float32_t * Fin, \
        ne10_int32_t * factors, \
        ne10_fft_cpx_float32_t * twiddles) \
{ \
    ne10_int32_t fstride = factors[1]; \
    ne10_int32_t i; \
    if (fstride == 16) \
        ne10_data_bitreversal32_float32_neon (Fout, Fin); \
    else if (fstride == 64) \
        ne10_data_bitreversal128_float32_neon (Fout, Fin); \
    else if (fstride == 256) \
        ne10_data_bitreversal512_float32_neon (Fout, Fin); \
    else if (fstride == 1024) \
        ne10_data_bitreversal2048_float32_neon (Fout, Fin); \
    ne10_radix2_butterfly_##inverse##_float32_neon (Fout, factors, twiddles); \
}

ne10_butterfly_length_odd_power2_float32_neon (forward)
ne10_butterfly_length_odd_power2_float32_neon (backward)

static void ne10_fft16_forward_float32_neon (ne10_fft_cpx_float32_t * Fout,
        ne10_fft_cpx_float32_t * Fin,
        ne10_fft_cpx_float32_t * twiddles)
{
    ne10_fft_cpx_float32_t *tw1, *tw2, *tw3;

    // the first stage
    float32_t *p_src0, *p_src4, *p_src8, *p_src12;
    float32x4x2_t q2_in_0123, q2_in_4567, q2_in_89ab, q2_in_cdef;
    float32x4_t q_t0_r,  q_t0_i, q_t1_r,  q_t1_i, q_t2_r,  q_t2_i, q_t3_r, q_t3_i;
    float32x4_t q_out_r048c,  q_out_i048c, q_out_r159d,  q_out_i159d;
    float32x4_t q_out_r26ae,  q_out_i26ae, q_out_r37bf,  q_out_i37bf;
    p_src0 = (float32_t*) (& (Fin[0]));
    p_src4 = (float32_t*) (& (Fin[4]));
    p_src8 = (float32_t*) (& (Fin[8]));
    p_src12 = (float32_t*) (& (Fin[12]));
    q2_in_0123 = vld2q_f32 (p_src0);
    q2_in_4567 = vld2q_f32 (p_src4);
    q2_in_89ab = vld2q_f32 (p_src8);
    q2_in_cdef = vld2q_f32 (p_src12);

    q_t2_r = vsubq_f32 (q2_in_0123.val[0], q2_in_89ab.val[0]);
    q_t2_i = vsubq_f32 (q2_in_0123.val[1], q2_in_89ab.val[1]);
    q_t3_r = vaddq_f32 (q2_in_0123.val[0], q2_in_89ab.val[0]);
    q_t3_i = vaddq_f32 (q2_in_0123.val[1], q2_in_89ab.val[1]);

    q_t0_r = vaddq_f32 (q2_in_4567.val[0], q2_in_cdef.val[0]);
    q_t0_i = vaddq_f32 (q2_in_4567.val[1], q2_in_cdef.val[1]);
    q_t1_r = vsubq_f32 (q2_in_4567.val[0], q2_in_cdef.val[0]);
    q_t1_i = vsubq_f32 (q2_in_4567.val[1], q2_in_cdef.val[1]);

    q_out_r26ae = vsubq_f32 (q_t3_r, q_t0_r);
    q_out_i26ae = vsubq_f32 (q_t3_i, q_t0_i);
    q_out_r048c = vaddq_f32 (q_t3_r, q_t0_r);
    q_out_i048c = vaddq_f32 (q_t3_i, q_t0_i);
    q_out_r159d = vaddq_f32 (q_t2_r, q_t1_i);
    q_out_i159d = vsubq_f32 (q_t2_i, q_t1_r);
    q_out_r37bf = vsubq_f32 (q_t2_r, q_t1_i);
    q_out_i37bf = vaddq_f32 (q_t2_i, q_t1_r);

    // second stages
    float32_t *p_dst0, *p_dst1, *p_dst2, *p_dst3;
    float32_t *p_tw1, *p_tw2, *p_tw3;
    float32x4_t q_s0_r, q_s0_i, q_s1_r, q_s1_i, q_s2_r, q_s2_i;
    float32x4_t q_s3_r, q_s3_i, q_s4_r, q_s4_i, q_s5_r, q_s5_i;
    float32x4x2_t q2_tmp_0, q2_tmp_1, q2_tmp_2, q2_tmp_3;
    float32x4_t q_in_r0123, q_in_r4567, q_in_r89ab, q_in_rcdef;
    float32x4_t q_in_i0123, q_in_i4567, q_in_i89ab, q_in_icdef;
    float32x4x2_t q2_tw1, q2_tw2, q2_tw3;
    float32x4_t q_tmp0, q_tmp1, q_tmp2, q_tmp3, q_tmp4, q_tmp5;
    float32x4x2_t q2_out_0123, q2_out_4567, q2_out_89ab, q2_out_cdef;
    tw1 = twiddles;
    tw2 = twiddles + 4;
    tw3 = twiddles + 8;
    p_dst0 = (float32_t*) (&Fout[0]);
    p_dst1 = (float32_t*) (&Fout[4]);
    p_dst2 = (float32_t*) (&Fout[8]);
    p_dst3 = (float32_t*) (&Fout[12]);
    p_tw1 = (float32_t*) tw1;
    p_tw2 = (float32_t*) tw2;
    p_tw3 = (float32_t*) tw3;
    q2_tmp_0 = vzipq_f32 (q_out_r048c, q_out_r159d);
    q2_tmp_1 = vzipq_f32 (q_out_i048c, q_out_i159d);
    q2_tmp_2 = vzipq_f32 (q_out_r26ae, q_out_r37bf);
    q2_tmp_3 = vzipq_f32 (q_out_i26ae, q_out_i37bf);
    q_in_r0123 = vcombine_f32 (vget_low_f32 (q2_tmp_0.val[0]), vget_low_f32 (q2_tmp_2.val[0]));
    q_in_i0123 = vcombine_f32 (vget_low_f32 (q2_tmp_1.val[0]), vget_low_f32 (q2_tmp_3.val[0]));
    q_in_r4567 = vcombine_f32 (vget_high_f32 (q2_tmp_0.val[0]), vget_high_f32 (q2_tmp_2.val[0]));
    q_in_i4567 = vcombine_f32 (vget_high_f32 (q2_tmp_1.val[0]), vget_high_f32 (q2_tmp_3.val[0]));
    q_in_r89ab = vcombine_f32 (vget_low_f32 (q2_tmp_0.val[1]), vget_low_f32 (q2_tmp_2.val[1]));
    q_in_i89ab = vcombine_f32 (vget_low_f32 (q2_tmp_1.val[1]), vget_low_f32 (q2_tmp_3.val[1]));
    q_in_rcdef = vcombine_f32 (vget_high_f32 (q2_tmp_0.val[1]), vget_high_f32 (q2_tmp_2.val[1]));
    q_in_icdef = vcombine_f32 (vget_high_f32 (q2_tmp_1.val[1]), vget_high_f32 (q2_tmp_3.val[1]));
    q2_tw1 = vld2q_f32 (p_tw1);
    q2_tw2 = vld2q_f32 (p_tw2);
    q2_tw3 = vld2q_f32 (p_tw3);

    q_s0_r = vmulq_f32 (q_in_r4567, q2_tw1.val[0]);
    q_s0_i = vmulq_f32 (q_in_r4567, q2_tw1.val[1]);
    q_s1_r = vmulq_f32 (q_in_r89ab, q2_tw2.val[0]);
    q_s1_i = vmulq_f32 (q_in_r89ab, q2_tw2.val[1]);
    q_s2_r = vmulq_f32 (q_in_rcdef, q2_tw3.val[0]);
    q_s2_i = vmulq_f32 (q_in_rcdef, q2_tw3.val[1]);
    q_s0_r = vmlsq_f32 (q_s0_r, q_in_i4567, q2_tw1.val[1]);
    q_s0_i = vmlaq_f32 (q_s0_i, q_in_i4567, q2_tw1.val[0]);
    q_s1_r = vmlsq_f32 (q_s1_r, q_in_i89ab, q2_tw2.val[1]);
    q_s1_i = vmlaq_f32 (q_s1_i, q_in_i89ab, q2_tw2.val[0]);
    q_s2_r = vmlsq_f32 (q_s2_r, q_in_icdef, q2_tw3.val[1]);
    q_s2_i = vmlaq_f32 (q_s2_i, q_in_icdef, q2_tw3.val[0]);


    q_s5_r = vsubq_f32 (q_in_r0123, q_s1_r);
    q_s5_i = vsubq_f32 (q_in_i0123, q_s1_i);
    q2_out_0123.val[0] = vaddq_f32 (q_in_r0123, q_s1_r);
    q2_out_0123.val[1] = vaddq_f32 (q_in_i0123, q_s1_i);

    q_s3_r = vaddq_f32 (q_s0_r, q_s2_r);
    q_s3_i = vaddq_f32 (q_s0_i, q_s2_i);
    q_s4_r = vsubq_f32 (q_s0_r, q_s2_r);
    q_s4_i = vsubq_f32 (q_s0_i, q_s2_i);

    q2_out_89ab.val[0] = vsubq_f32 (q2_out_0123.val[0], q_s3_r);
    q2_out_89ab.val[1] = vsubq_f32 (q2_out_0123.val[1], q_s3_i);
    q2_out_0123.val[0] = vaddq_f32 (q2_out_0123.val[0], q_s3_r);
    q2_out_0123.val[1] = vaddq_f32 (q2_out_0123.val[1], q_s3_i);

    q2_out_4567.val[0] = vaddq_f32 (q_s5_r, q_s4_i);
    q2_out_4567.val[1] = vsubq_f32 (q_s5_i, q_s4_r);
    q2_out_cdef.val[0] = vsubq_f32 (q_s5_r, q_s4_i);
    q2_out_cdef.val[1] = vaddq_f32 (q_s5_i, q_s4_r);

    vst2q_f32 (p_dst0, q2_out_0123);
    vst2q_f32 (p_dst1, q2_out_4567);
    vst2q_f32 (p_dst2, q2_out_89ab);
    vst2q_f32 (p_dst3, q2_out_cdef);
}

static void ne10_fft16_backward_float32_neon (ne10_fft_cpx_float32_t * Fout,
        ne10_fft_cpx_float32_t * Fin,
        ne10_fft_cpx_float32_t * twiddles)
{
    ne10_float32_t i, j;
    ne10_fft_cpx_float32_t *tw1, *tw2, *tw3;

    // the first stage
    float32_t *p_src0, *p_src4, *p_src8, *p_src12;
    float32x4x2_t q2_in_0123, q2_in_4567, q2_in_89ab, q2_in_cdef;
    float32x4_t q_t0_r,  q_t0_i, q_t1_r,  q_t1_i, q_t2_r,  q_t2_i, q_t3_r, q_t3_i;
    float32x4_t q_out_r048c,  q_out_i048c, q_out_r159d,  q_out_i159d;
    float32x4_t q_out_r26ae,  q_out_i26ae, q_out_r37bf,  q_out_i37bf;
    p_src0 = (float32_t*) (& (Fin[0]));
    p_src4 = (float32_t*) (& (Fin[4]));
    p_src8 = (float32_t*) (& (Fin[8]));
    p_src12 = (float32_t*) (& (Fin[12]));
    q2_in_0123 = vld2q_f32 (p_src0);
    q2_in_4567 = vld2q_f32 (p_src4);
    q2_in_89ab = vld2q_f32 (p_src8);
    q2_in_cdef = vld2q_f32 (p_src12);

    q_t2_r = vsubq_f32 (q2_in_0123.val[0], q2_in_89ab.val[0]);
    q_t2_i = vsubq_f32 (q2_in_0123.val[1], q2_in_89ab.val[1]);
    q_t3_r = vaddq_f32 (q2_in_0123.val[0], q2_in_89ab.val[0]);
    q_t3_i = vaddq_f32 (q2_in_0123.val[1], q2_in_89ab.val[1]);

    q_t0_r = vaddq_f32 (q2_in_4567.val[0], q2_in_cdef.val[0]);
    q_t0_i = vaddq_f32 (q2_in_4567.val[1], q2_in_cdef.val[1]);
    q_t1_r = vsubq_f32 (q2_in_4567.val[0], q2_in_cdef.val[0]);
    q_t1_i = vsubq_f32 (q2_in_4567.val[1], q2_in_cdef.val[1]);

    q_out_r26ae = vsubq_f32 (q_t3_r, q_t0_r);
    q_out_i26ae = vsubq_f32 (q_t3_i, q_t0_i);
    q_out_r048c = vaddq_f32 (q_t3_r, q_t0_r);
    q_out_i048c = vaddq_f32 (q_t3_i, q_t0_i);
    q_out_r159d = vsubq_f32 (q_t2_r, q_t1_i);
    q_out_i159d = vaddq_f32 (q_t2_i, q_t1_r);
    q_out_r37bf = vaddq_f32 (q_t2_r, q_t1_i);
    q_out_i37bf = vsubq_f32 (q_t2_i, q_t1_r);

    // second stages
    float32_t *p_dst0, *p_dst1, *p_dst2, *p_dst3;
    float32_t *p_tw1, *p_tw2, *p_tw3;
    float32x4_t q_s0_r, q_s0_i, q_s1_r, q_s1_i, q_s2_r, q_s2_i;
    float32x4_t q_s3_r, q_s3_i, q_s4_r, q_s4_i, q_s5_r, q_s5_i;
    float32x4x2_t q2_tmp_0, q2_tmp_1, q2_tmp_2, q2_tmp_3;
    float32x4_t q_in_r0123, q_in_r4567, q_in_r89ab, q_in_rcdef;
    float32x4_t q_in_i0123, q_in_i4567, q_in_i89ab, q_in_icdef;
    float32x4x2_t q2_tw1, q2_tw2, q2_tw3;
    float32x4_t q_tmp0, q_tmp1, q_tmp2, q_tmp3, q_tmp4, q_tmp5;
    float32x4x2_t q2_out_0123, q2_out_4567, q2_out_89ab, q2_out_cdef;
    tw1 = twiddles;
    tw2 = twiddles + 4;
    tw3 = twiddles + 8;
    p_dst0 = (float32_t*) (&Fout[0]);
    p_dst1 = (float32_t*) (&Fout[4]);
    p_dst2 = (float32_t*) (&Fout[8]);
    p_dst3 = (float32_t*) (&Fout[12]);
    p_tw1 = (float32_t*) tw1;
    p_tw2 = (float32_t*) tw2;
    p_tw3 = (float32_t*) tw3;
    q2_tmp_0 = vzipq_f32 (q_out_r048c, q_out_r159d);
    q2_tmp_1 = vzipq_f32 (q_out_i048c, q_out_i159d);
    q2_tmp_2 = vzipq_f32 (q_out_r26ae, q_out_r37bf);
    q2_tmp_3 = vzipq_f32 (q_out_i26ae, q_out_i37bf);
    q_in_r0123 = vcombine_f32 (vget_low_f32 (q2_tmp_0.val[0]), vget_low_f32 (q2_tmp_2.val[0]));
    q_in_i0123 = vcombine_f32 (vget_low_f32 (q2_tmp_1.val[0]), vget_low_f32 (q2_tmp_3.val[0]));
    q_in_r4567 = vcombine_f32 (vget_high_f32 (q2_tmp_0.val[0]), vget_high_f32 (q2_tmp_2.val[0]));
    q_in_i4567 = vcombine_f32 (vget_high_f32 (q2_tmp_1.val[0]), vget_high_f32 (q2_tmp_3.val[0]));
    q_in_r89ab = vcombine_f32 (vget_low_f32 (q2_tmp_0.val[1]), vget_low_f32 (q2_tmp_2.val[1]));
    q_in_i89ab = vcombine_f32 (vget_low_f32 (q2_tmp_1.val[1]), vget_low_f32 (q2_tmp_3.val[1]));
    q_in_rcdef = vcombine_f32 (vget_high_f32 (q2_tmp_0.val[1]), vget_high_f32 (q2_tmp_2.val[1]));
    q_in_icdef = vcombine_f32 (vget_high_f32 (q2_tmp_1.val[1]), vget_high_f32 (q2_tmp_3.val[1]));
    q2_tw1 = vld2q_f32 (p_tw1);
    q2_tw2 = vld2q_f32 (p_tw2);
    q2_tw3 = vld2q_f32 (p_tw3);

    q_s0_r = vmulq_f32 (q_in_r4567, q2_tw1.val[0]);
    q_s0_i = vmulq_f32 (q_in_i4567, q2_tw1.val[0]);
    q_s1_r = vmulq_f32 (q_in_r89ab, q2_tw2.val[0]);
    q_s1_i = vmulq_f32 (q_in_i89ab, q2_tw2.val[0]);
    q_s2_r = vmulq_f32 (q_in_rcdef, q2_tw3.val[0]);
    q_s2_i = vmulq_f32 (q_in_icdef, q2_tw3.val[0]);
    q_s0_r = vmlaq_f32 (q_s0_r, q_in_i4567, q2_tw1.val[1]);
    q_s0_i = vmlsq_f32 (q_s0_i, q_in_r4567, q2_tw1.val[1]);
    q_s1_r = vmlaq_f32 (q_s1_r, q_in_i89ab, q2_tw2.val[1]);
    q_s1_i = vmlsq_f32 (q_s1_i, q_in_r89ab, q2_tw2.val[1]);
    q_s2_r = vmlaq_f32 (q_s2_r, q_in_icdef, q2_tw3.val[1]);
    q_s2_i = vmlsq_f32 (q_s2_i, q_in_rcdef, q2_tw3.val[1]);

    q_s5_r = vsubq_f32 (q_in_r0123, q_s1_r);
    q_s5_i = vsubq_f32 (q_in_i0123, q_s1_i);
    q2_out_0123.val[0] = vaddq_f32 (q_in_r0123, q_s1_r);
    q2_out_0123.val[1] = vaddq_f32 (q_in_i0123, q_s1_i);

    q_s3_r = vaddq_f32 (q_s0_r, q_s2_r);
    q_s3_i = vaddq_f32 (q_s0_i, q_s2_i);
    q_s4_r = vsubq_f32 (q_s0_r, q_s2_r);
    q_s4_i = vsubq_f32 (q_s0_i, q_s2_i);

    q2_out_89ab.val[0] = vsubq_f32 (q2_out_0123.val[0], q_s3_r);
    q2_out_89ab.val[1] = vsubq_f32 (q2_out_0123.val[1], q_s3_i);
    q2_out_0123.val[0] = vaddq_f32 (q2_out_0123.val[0], q_s3_r);
    q2_out_0123.val[1] = vaddq_f32 (q2_out_0123.val[1], q_s3_i);

    q2_out_4567.val[0] = vsubq_f32 (q_s5_r, q_s4_i);
    q2_out_4567.val[1] = vaddq_f32 (q_s5_i, q_s4_r);
    q2_out_cdef.val[0] = vaddq_f32 (q_s5_r, q_s4_i);
    q2_out_cdef.val[1] = vsubq_f32 (q_s5_i, q_s4_r);

    vst2q_f32 (p_dst0, q2_out_0123);
    vst2q_f32 (p_dst1, q2_out_4567);
    vst2q_f32 (p_dst2, q2_out_89ab);
    vst2q_f32 (p_dst3, q2_out_cdef);
}

static void ne10_mixed_radix_butterfly_forward_float32_neon (ne10_fft_cpx_float32_t * Fout,
        ne10_int32_t * factors,
        ne10_fft_cpx_float32_t * twiddles)
{
    ne10_int32_t stage_count;

    // the first stage
    stage_count = factors[0];
    if (factors[2 * stage_count] == 2)
    {
        //radix 2/4, FFT length is 2^n (n is odd)
        ne10_mixed_radix_butterfly_length_odd_power2_float32_neon (Fout, factors, twiddles);
    }
    else if (factors[2 * stage_count] == 4)
    {
        //radix 4, FFT length is 2^n (n is even)
        ne10_mixed_radix_butterfly_length_even_power2_float32_neon (Fout, factors, twiddles);
    }
}

static void ne10_mixed_radix_butterfly_backward_float32_neon (ne10_fft_cpx_float32_t * Fout,
        ne10_int32_t * factors,
        ne10_fft_cpx_float32_t * twiddles)
{
    ne10_int32_t stage_count;

    stage_count = factors[0];
    if (factors[2 * stage_count] == 2)
    {
        //radix 2/4, FFT length is 2^n (n is odd)
        ne10_mixed_radix_butterfly_inverse_length_odd_power2_float32_neon (Fout, factors, twiddles);
    }
    else if (factors[2 * stage_count] == 4)
    {
        //radix 4, FFT length is 2^n (n is even)
        ne10_mixed_radix_butterfly_inverse_length_even_power2_float32_neon (Fout, factors, twiddles);
    }
}


void ne10_fft_split_r2c_1d_float32_neon (ne10_fft_cpx_float32_t *dst,
        const ne10_fft_cpx_float32_t *src,
        ne10_fft_cpx_float32_t *twiddles,
        ne10_int32_t ncfft)
{
    ne10_int32_t k;
    ne10_int32_t count = ncfft / 2;
    ne10_fft_cpx_float32_t fpnk, fpk, f1k, f2k, tw, tdc;
    float32x4x2_t q2_fpk, q2_fpnk, q2_tw, q2_dst, q2_dst2;
    float32x4_t q_fpnk_r, q_fpnk_i;
    float32x4_t q_f1k_r, q_f1k_i, q_f2k_r, q_f2k_i;
    float32x4_t q_tw_r, q_tw_i;
    float32x4_t q_tmp0, q_tmp1, q_tmp2, q_tmp3, q_val;
    float32x4_t q_dst_r, q_dst_i, q_dst2_r, q_dst2_i;
    float32_t *p_src, *p_src2, *p_dst, *p_dst2, *p_twiddles;

    tdc.r = src[0].r;
    tdc.i = src[0].i;

    dst[0].r = tdc.r + tdc.i;
    dst[ncfft].r = tdc.r - tdc.i;
    dst[ncfft].i = dst[0].i = 0;

    if (count >= 4)
    {
        for (k = 1; k <= count ; k += 4)
        {
            p_src  = (float32_t*) (& (src[k]));
            p_src2  = (float32_t*) (& (src[ncfft - k - 3]));
            p_twiddles  = (float32_t*) (& (twiddles[k - 1]));
            p_dst  = (float32_t*) (& (dst[k]));
            p_dst2  = (float32_t*) (& (dst[ncfft - k - 3]));

            q2_fpk  = vld2q_f32 (p_src);
            q2_fpnk = vld2q_f32 (p_src2);
            q2_tw = vld2q_f32 (p_twiddles);
            q2_fpnk.val[0] = vrev64q_f32 (q2_fpnk.val[0]);
            q2_fpnk.val[1] = vrev64q_f32 (q2_fpnk.val[1]);
            q_fpnk_r = vcombine_f32 (vget_high_f32 (q2_fpnk.val[0]), vget_low_f32 (q2_fpnk.val[0]));
            q_fpnk_i = vcombine_f32 (vget_high_f32 (q2_fpnk.val[1]), vget_low_f32 (q2_fpnk.val[1]));
            q_fpnk_i = vnegq_f32 (q_fpnk_i);

            q_f1k_r = vaddq_f32 (q2_fpk.val[0], q_fpnk_r);
            q_f1k_i = vaddq_f32 (q2_fpk.val[1], q_fpnk_i);

            q_f2k_r = vsubq_f32 (q2_fpk.val[0], q_fpnk_r);
            q_f2k_i = vsubq_f32 (q2_fpk.val[1], q_fpnk_i);

            q_tmp0 = vmulq_f32 (q_f2k_r, q2_tw.val[0]);
            q_tmp1 = vmulq_f32 (q_f2k_i, q2_tw.val[1]);
            q_tmp2 = vmulq_f32 (q_f2k_r, q2_tw.val[1]);
            q_tmp3 = vmulq_f32 (q_f2k_i, q2_tw.val[0]);
            q_tw_r = vsubq_f32 (q_tmp0, q_tmp1);
            q_tw_i = vaddq_f32 (q_tmp2, q_tmp3);

            q_val = vdupq_n_f32 (0.5f);
            q_dst2_r = vsubq_f32 (q_f1k_r, q_tw_r);
            q_dst2_i = vsubq_f32 (q_tw_i, q_f1k_i);
            q_dst_r = vaddq_f32 (q_f1k_r, q_tw_r);
            q_dst_i = vaddq_f32 (q_f1k_i, q_tw_i);
            q_dst2_r = vmulq_f32 (q_dst2_r, q_val);
            q_dst2_i = vmulq_f32 (q_dst2_i, q_val);
            q2_dst.val[0] = vmulq_f32 (q_dst_r, q_val);
            q2_dst.val[1] = vmulq_f32 (q_dst_i, q_val);
            q_dst2_r = vrev64q_f32 (q_dst2_r);
            q_dst2_i = vrev64q_f32 (q_dst2_i);
            q2_dst2.val[0] = vcombine_f32 (vget_high_f32 (q_dst2_r), vget_low_f32 (q_dst2_r));
            q2_dst2.val[1] = vcombine_f32 (vget_high_f32 (q_dst2_i), vget_low_f32 (q_dst2_i));
            vst2q_f32 (p_dst, q2_dst);
            vst2q_f32 (p_dst2, q2_dst2);

        }
    }
    else
    {
        for (k = 1; k <= count ; k++)
        {
            fpk    = src[k];
            fpnk.r =   src[ncfft - k].r;
            fpnk.i = - src[ncfft - k].i;

            f1k.r = fpk.r + fpnk.r;
            f1k.i = fpk.i + fpnk.i;

            f2k.r = fpk.r - fpnk.r;
            f2k.i = fpk.i - fpnk.i;

            tw.r = f2k.r * (twiddles[k - 1]).r - f2k.i * (twiddles[k - 1]).i;
            tw.i = f2k.r * (twiddles[k - 1]).i + f2k.i * (twiddles[k - 1]).r;

            dst[k].r = (f1k.r + tw.r) * 0.5f;
            dst[k].i = (f1k.i + tw.i) * 0.5f;
            dst[ncfft - k].r = (f1k.r - tw.r) * 0.5f;
            dst[ncfft - k].i = (tw.i - f1k.i) * 0.5f;
        }
    }
}

void ne10_fft_split_c2r_1d_float32_neon (ne10_fft_cpx_float32_t *dst,
        const ne10_fft_cpx_float32_t *src,
        ne10_fft_cpx_float32_t *twiddles,
        ne10_int32_t ncfft)
{

    ne10_int32_t k;
    ne10_int32_t count = ncfft / 2;
    ne10_fft_cpx_float32_t fk, fnkc, fek, fok, tmp;
    float32x4x2_t q2_fk, q2_fnkc, q2_tw, q2_dst, q2_dst2;
    float32x4_t q_fnkc_r, q_fnkc_i;
    float32x4_t q_fek_r, q_fek_i, q_fok_r, q_fok_i;
    float32x4_t q_tw_r, q_tw_i;
    float32x4_t q_tmp0, q_tmp1, q_tmp2, q_tmp3;
    float32x4_t q_dst_r, q_dst_i, q_dst2_r, q_dst2_i;
    float32_t *p_src, *p_src2, *p_dst, *p_dst2, *p_twiddles;

    dst[0].r = src[0].r + src[ncfft].r;
    dst[0].i = src[0].r - src[ncfft].r;

    if (count >= 4)
    {
        for (k = 1; k <= count ; k += 4)
        {
            p_src  = (float32_t*) (& (src[k]));
            p_src2  = (float32_t*) (& (src[ncfft - k - 3]));
            p_twiddles  = (float32_t*) (& (twiddles[k - 1]));
            p_dst  = (float32_t*) (& (dst[k]));
            p_dst2  = (float32_t*) (& (dst[ncfft - k - 3]));

            q2_fk  = vld2q_f32 (p_src);
            q2_fnkc = vld2q_f32 (p_src2);
            q2_tw = vld2q_f32 (p_twiddles);
            q2_fnkc.val[0] = vrev64q_f32 (q2_fnkc.val[0]);
            q2_fnkc.val[1] = vrev64q_f32 (q2_fnkc.val[1]);
            q_fnkc_r = vcombine_f32 (vget_high_f32 (q2_fnkc.val[0]), vget_low_f32 (q2_fnkc.val[0]));
            q_fnkc_i = vcombine_f32 (vget_high_f32 (q2_fnkc.val[1]), vget_low_f32 (q2_fnkc.val[1]));
            q_fnkc_i = vnegq_f32 (q_fnkc_i);

            q_fek_r = vaddq_f32 (q2_fk.val[0], q_fnkc_r);
            q_fek_i = vaddq_f32 (q2_fk.val[1], q_fnkc_i);

            q_tmp0 = vsubq_f32 (q2_fk.val[0], q_fnkc_r);
            q_tmp1 = vsubq_f32 (q2_fk.val[1], q_fnkc_i);

            q_fok_r = vmulq_f32 (q_tmp0, q2_tw.val[0]);
            q_fok_i = vmulq_f32 (q_tmp1, q2_tw.val[0]);
            q_tmp2 = vmulq_f32 (q_tmp1, q2_tw.val[1]);
            q_tmp3 = vmulq_f32 (q_tmp0, q2_tw.val[1]);
            q_fok_r = vaddq_f32 (q_fok_r, q_tmp2);
            q_fok_i = vsubq_f32 (q_fok_i, q_tmp3);

            q_dst2_r = vsubq_f32 (q_fek_r, q_fok_r);
            q_dst2_i = vsubq_f32 (q_fok_i, q_fek_i);
            q2_dst.val[0] = vaddq_f32 (q_fek_r, q_fok_r);
            q2_dst.val[1] = vaddq_f32 (q_fek_i, q_fok_i);
            q_dst2_r = vrev64q_f32 (q_dst2_r);
            q_dst2_i = vrev64q_f32 (q_dst2_i);
            q2_dst2.val[0] = vcombine_f32 (vget_high_f32 (q_dst2_r), vget_low_f32 (q_dst2_r));
            q2_dst2.val[1] = vcombine_f32 (vget_high_f32 (q_dst2_i), vget_low_f32 (q_dst2_i));
            vst2q_f32 (p_dst, q2_dst);
            vst2q_f32 (p_dst2, q2_dst2);

        }
    }
    else
    {
        for (k = 1; k <= count ; k++)
        {
            fk = src[k];
            fnkc.r = src[ncfft - k].r;
            fnkc.i = -src[ncfft - k].i;

            fek.r = fk.r + fnkc.r;
            fek.i = fk.i + fnkc.i;

            tmp.r = fk.r - fnkc.r;
            tmp.i = fk.i - fnkc.i;

            fok.r = tmp.r * twiddles[k - 1].r + tmp.i * twiddles[k - 1].i;
            fok.i = tmp.i * twiddles[k - 1].r - tmp.r * twiddles[k - 1].i;

            dst[k].r = fek.r + fok.r;
            dst[k].i = fek.i + fok.i;

            dst[ncfft - k].r = fek.r - fok.r;
            dst[ncfft - k].i = fok.i - fek.i;
        }
    }
}

/**
 * @addtogroup C2C_FFT_IFFT
 * @{
 */

/**
 * @brief Mixed radix-2/4 complex FFT/IFFT of float(32-bit) data.
 * @param[out]  *fout            point to the output buffer (out-of-place)
 * @param[in]   *fin             point to the input buffer (out-of-place)
 * @param[in]   *twiddles        point to the twiddle buffer
 * @param[in]   *factors         point to factors buffer. 0: stage number, 1: stride for the first stage, others: radix and stage's fft length/radix
 * @param[in]   nfft             length of FFT
 * @param[in]   inverse_fft      the flag of IFFT, 0: FFT, 1: IFFT
 * @return none.
 * The function implements a mixed radix-2/4 complex FFT/IFFT. The length of 2^N(N is 1, 2, 3, 4, 5, 6 ....etc) is supported.
 * Otherwise, this FFT is an out-of-place algorithm. When you want to get an in-place FFT, it creates a temp buffer as
 *  output buffer and then copies the temp buffer back to input buffer. For the usage of this function, please check test/test_suite_fft_float32.c
 */
void ne10_fft_c2c_1d_float32_neon (ne10_fft_cpx_float32_t *fout,
                                   ne10_fft_cpx_float32_t *fin,
                                   ne10_fft_cpx_float32_t *twiddles,
                                   ne10_int32_t *factors,
                                   ne10_int32_t nfft,
                                   ne10_int32_t inverse_fft)
{
    if (inverse_fft)
    {
        switch (nfft)
        {
        case 4:
            ne10_fft4_backward_float32 (fout, fin);
            break;
        case 8:
            ne10_fft8_backward_float32 (fout, fin);
            break;
        case 16:
            ne10_fft16_backward_float32_neon (fout, fin, twiddles);
            break;
        case 32:
        case 128:
        case 512:
        case 2048:
            ne10_butterfly_backward_length_odd_power2_float32_neon (fout, fin, factors, twiddles);
            break;
        case 64:
        case 256:
        case 1024:
        case 4096:
            ne10_butterfly_backward_length_even_power2_float32_neon (fout, fin, factors, twiddles);
            break;
        default:
            ne10_data_bitreversal_float32 (fout, fin, 1, &factors[2]);
            ne10_mixed_radix_butterfly_backward_float32_neon (fout, factors, twiddles);
            break;
        }
    }
    else
    {
        switch (nfft)
        {
        case 4:
            ne10_fft4_forward_float32 (fout, fin);
            break;
        case 8:
            ne10_fft8_forward_float32 (fout, fin);
            break;
        case 16:
            ne10_fft16_forward_float32_neon (fout, fin, twiddles);
            break;
        case 32:
        case 128:
        case 512:
        case 2048:
            ne10_butterfly_forward_length_odd_power2_float32_neon (fout, fin, factors, twiddles);
            break;
        case 64:
        case 256:
        case 1024:
        case 4096:
            ne10_butterfly_forward_length_even_power2_float32_neon (fout, fin, factors, twiddles);
            break;
        default:
            ne10_data_bitreversal_float32 (fout, fin, 1, &factors[2]);
            ne10_mixed_radix_butterfly_forward_float32_neon (fout, factors, twiddles);
            break;
        }
    }
}

/**
 * @}
 */ //end of C2C_FFT_IFFT group

/**
 * @addtogroup R2C_FFT_IFFT
 * @{
 */

/**
 * @brief Mixed radix-2/4 FFT (real to complex) of float(32-bit) data.
 * @param[out]  *fout            point to the output buffer
 * @param[in]   *fin             point to the input buffer
 * @param[in]   *twiddles        point to the twiddle buffer
 * @param[in]   *super_twiddles  point to the twiddle buffer for data split
 * @param[in]   *factors         point to factors buffer. 0: stage number, 1: stride for the first stage, others: radix and stage's fft length/radix
 * @param[in]   nfft             length of FFT
 * @return none.
 * The function implements a mixed radix-2/4 FFT (real to complex). The length of 2^N(N is 2, 3, 4, 5, 6 ....etc) is supported.
 * Otherwise, we alloc a temp buffer(the size is same as input buffer) for storing intermedia.
 * For the usage of this function, please check test/test_suite_fft_float32.c
 */
void ne10_fft_r2c_1d_float32_neon (ne10_fft_cpx_float32_t *fout,
                                   ne10_float32_t *fin,
                                   ne10_fft_cpx_float32_t *twiddles,
                                   ne10_fft_cpx_float32_t *super_twiddles,
                                   ne10_int32_t *factors,
                                   ne10_int32_t nfft)
{
    ne10_int32_t ncfft = nfft >> 1;

    /* malloc a temp buffer for cfft */
    ne10_fft_cpx_float32_t * tmpbuf_ = (ne10_fft_cpx_float32_t*) NE10_MALLOC (sizeof (ne10_fft_cpx_float32_t) * ncfft);

    ne10_fft_c2c_1d_float32_neon (tmpbuf_, (ne10_fft_cpx_float32_t*) fin, twiddles, factors, ncfft, 0);
    ne10_fft_split_r2c_1d_float32_neon (fout, tmpbuf_, super_twiddles, ncfft);

    NE10_FREE (tmpbuf_);
}

/**
 * @brief Mixed radix-2/4 IFFT (complex to real) of float(32-bit) data.
 * @param[out]  *fout            point to the output buffer
 * @param[in]   *fin             point to the input buffer
 * @param[in]   *twiddles        point to the twiddle buffer
 * @param[in]   *super_twiddles  point to the twiddle buffer for data split
 * @param[in]   *factors         point to factors buffer. 0: stage number, 1: stride for the first stage, others: radix and stage's fft length/radix
 * @param[in]   nfft             length of FFT
 * @return none.
 * The function implements a mixed radix-2/4 FFT (complex to real). The length of 2^N(N is 2, 3, 4, 5, 6 ....etc) is supported.
 * Otherwise, we alloc a temp buffer(the size is same as input buffer) for storing intermedia.
 * For the usage of this function, please check test/test_suite_fft_float32.c
 */
void ne10_fft_c2r_1d_float32_neon (ne10_float32_t *fout,
                                   ne10_fft_cpx_float32_t *fin,
                                   ne10_fft_cpx_float32_t *twiddles,
                                   ne10_fft_cpx_float32_t *super_twiddles,
                                   ne10_int32_t *factors,
                                   ne10_int32_t nfft)
{
    ne10_int32_t ncfft = nfft >> 1;

    /* malloc a temp buffer for split */
    ne10_fft_cpx_float32_t * tmpbuf_ = (ne10_fft_cpx_float32_t*) NE10_MALLOC (sizeof (ne10_fft_cpx_float32_t) * ncfft);

    ne10_fft_split_c2r_1d_float32_neon (tmpbuf_, fin, super_twiddles, ncfft);
    ne10_fft_c2c_1d_float32_neon ( (ne10_fft_cpx_float32_t*) fout, tmpbuf_, twiddles, factors, ncfft, 1);

    NE10_FREE (tmpbuf_);
}

/**
 * @} end of R2C_FFT_IFFT group
 */
