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
 *  DISCLAIMED. IN NO EVENT SHALL ARM LIMITED AND CONTRIBUTORS BE LIABLE FOR ANY
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
#include "NE10_dsp.h"

static inline void ne10_fft4_forward_float32 (ne10_fft_cpx_float32_t *out,
        ne10_fft_cpx_float32_t *in)
{
    ne10_float32_t s0_r, s0_i, s1_r, s1_i, s2_r, s2_i;
    ne10_float32_t tmp_r, tmp_i;

    // X[0] +/- X[2N/4]
    tmp_r = in[0].r + in[2].r;
    tmp_i = in[0].i + in[2].i;
    s2_r = in[0].r - in[2].r;
    s2_i = in[0].i - in[2].i;

    // X[N/4] +/- X[3N/4]
    s0_r = in[1].r + in[3].r;
    s0_i = in[1].i + in[3].i;
    s1_r = in[1].r - in[3].r;
    s1_i = in[1].i - in[3].i;

    // Combine the (X[0] +/- X[2N/4]) and (X[N/4] +/- X[3N/4]) components for the full
    // radix-4 butterfly, storing the results.
    out[2].r = tmp_r - s0_r;
    out[2].i = tmp_i - s0_i;
    out[0].r = tmp_r + s0_r;
    out[0].i = tmp_i + s0_i;
    out[1].r = s2_r + s1_i;
    out[1].i = s2_i - s1_r;
    out[3].r = s2_r - s1_i;
    out[3].i = s2_i + s1_r;
}

static inline void ne10_fft4_backward_float32 (ne10_fft_cpx_float32_t *out,
        ne10_fft_cpx_float32_t *in)
{
    ne10_float32_t s0_r, s0_i, s1_r, s1_i, s2_r, s2_i;
    ne10_float32_t tmp_r, tmp_i;

    // X[0] +/- X[2N/4]
    tmp_r = in[0].r + in[2].r;
    tmp_i = in[0].i + in[2].i;
    s2_r = in[0].r - in[2].r;
    s2_i = in[0].i - in[2].i;

    // X[N/4] +/- X[3N/4]
    s0_r = in[1].r + in[3].r;
    s0_i = in[1].i + in[3].i;
    s1_r = in[1].r - in[3].r;
    s1_i = in[1].i - in[3].i;

    // Combine the (X[0] +/- X[2N/4]) and (X[N/4] +/- X[3N/4]) components for the full
    // radix-4 butterfly, multiplying by (1 / nfft) and storing the results.
    out[2].r = (tmp_r - s0_r) * 0.25f;
    out[2].i = (tmp_i - s0_i) * 0.25f;
    out[0].r = (tmp_r + s0_r) * 0.25f;
    out[0].i = (tmp_i + s0_i) * 0.25f;
    out[1].r = (s2_r - s1_i) * 0.25f;
    out[1].i = (s2_i + s1_r) * 0.25f;
    out[3].r = (s2_r + s1_i) * 0.25f;
    out[3].i = (s2_i - s1_r) * 0.25f;
}


static inline void ne10_fft8_forward_float32 (ne10_fft_cpx_float32_t *out,
        ne10_fft_cpx_float32_t *in)
{
    ne10_float32_t s0_r, s0_i, s1_r, s1_i, s2_r, s2_i, s3_r, s3_i, s4_r, s4_i, s5_r, s5_i, s6_r, s6_i, s7_r, s7_i;
    ne10_float32_t t0_r, t0_i, t1_r, t1_i, t2_r, t2_i, t3_r, t3_i, t4_r, t4_i, t5_r, t5_i;
    const ne10_float32_t TW_81 = 0.70710678;

    // Prepare sums for the butterfly calculations
    s0_r = in[0].r + in[4].r;
    s0_i = in[0].i + in[4].i;
    s1_r = in[0].r - in[4].r;
    s1_i = in[0].i - in[4].i;
    s2_r = in[1].r + in[5].r;
    s2_i = in[1].i + in[5].i;
    s3_r = in[1].r - in[5].r;
    s3_i = in[1].i - in[5].i;
    s4_r = in[2].r + in[6].r;
    s4_i = in[2].i + in[6].i;
    s5_r = in[2].r - in[6].r;
    s5_i = in[2].i - in[6].i;
    s6_r = in[3].r + in[7].r;
    s6_i = in[3].i + in[7].i;
    s7_r = in[3].r - in[7].r;
    s7_i = in[3].i - in[7].i;

    // Combine the components without twiddles, storing the results.
    t0_r = s0_r - s4_r;
    t0_i = s0_i - s4_i;
    t1_r = s0_r + s4_r;
    t1_i = s0_i + s4_i;
    t2_r = s2_r + s6_r;
    t2_i = s2_i + s6_i;
    t3_r = s2_r - s6_r;
    t3_i = s2_i - s6_i;
    out[0].r = t1_r + t2_r;
    out[0].i = t1_i + t2_i;
    out[4].r = t1_r - t2_r;
    out[4].i = t1_i - t2_i;
    out[2].r = t0_r + t3_i;
    out[2].i = t0_i - t3_r;
    out[6].r = t0_r - t3_i;
    out[6].i = t0_i + t3_r;

    // Combine the components with twiddles (using hardcoded radix-8 twiddle values),
    // storing the results.
    t4_r = (s3_r + s3_i) * TW_81;
    t4_i = - (s3_r - s3_i) * TW_81;
    t5_r = (s7_r - s7_i) * TW_81;
    t5_i = (s7_r + s7_i) * TW_81;
    t0_r = s1_r - s5_i;
    t0_i = s1_i + s5_r;
    t1_r = s1_r + s5_i;
    t1_i = s1_i - s5_r;
    t2_r = t4_r - t5_r;
    t2_i = t4_i - t5_i;
    t3_r = t4_r + t5_r;
    t3_i = t4_i + t5_i;
    out[1].r = t1_r + t2_r;
    out[1].i = t1_i + t2_i;
    out[5].r = t1_r - t2_r;
    out[5].i = t1_i - t2_i;
    out[3].r = t0_r + t3_i;
    out[3].i = t0_i - t3_r;
    out[7].r = t0_r - t3_i;
    out[7].i = t0_i + t3_r;
}

static inline void ne10_fft8_backward_float32 (ne10_fft_cpx_float32_t *out,
        ne10_fft_cpx_float32_t *in)
{
    ne10_float32_t s0_r, s0_i, s1_r, s1_i, s2_r, s2_i, s3_r, s3_i, s4_r, s4_i, s5_r, s5_i, s6_r, s6_i, s7_r, s7_i;
    ne10_float32_t t0_r, t0_i, t1_r, t1_i, t2_r, t2_i, t3_r, t3_i, t4_r, t4_i, t5_r, t5_i;
    const ne10_float32_t TW_81 = 0.70710678;

    // Prepare sums for the butterfly calculations
    s0_r = in[0].r + in[4].r;
    s0_i = in[0].i + in[4].i;
    s1_r = in[0].r - in[4].r;
    s1_i = in[0].i - in[4].i;
    s2_r = in[1].r + in[5].r;
    s2_i = in[1].i + in[5].i;
    s3_r = in[1].r - in[5].r;
    s3_i = in[1].i - in[5].i;
    s4_r = in[2].r + in[6].r;
    s4_i = in[2].i + in[6].i;
    s5_r = in[2].r - in[6].r;
    s5_i = in[2].i - in[6].i;
    s6_r = in[3].r + in[7].r;
    s6_i = in[3].i + in[7].i;
    s7_r = in[3].r - in[7].r;
    s7_i = in[3].i - in[7].i;

    // Combine the components without twiddles, multiplying by (1 / nfft) and storing the
    // results.
    t0_r = s0_r - s4_r;
    t0_i = s0_i - s4_i;
    t1_r = s0_r + s4_r;
    t1_i = s0_i + s4_i;
    t2_r = s2_r + s6_r;
    t2_i = s2_i + s6_i;
    t3_r = s2_r - s6_r;
    t3_i = s2_i - s6_i;
    out[0].r = (t1_r + t2_r) * 0.125f;
    out[0].i = (t1_i + t2_i) * 0.125f;
    out[4].r = (t1_r - t2_r) * 0.125f;
    out[4].i = (t1_i - t2_i) * 0.125f;
    out[2].r = (t0_r - t3_i) * 0.125f;
    out[2].i = (t0_i + t3_r) * 0.125f;
    out[6].r = (t0_r + t3_i) * 0.125f;
    out[6].i = (t0_i - t3_r) * 0.125f;

    // Combine the components with twiddles (using hardcoded radix-8 twiddle values),
    // multiplying by (1 / nfft) and storing the results.
    t4_r = (s3_r - s3_i) * TW_81;
    t4_i = (s3_r + s3_i) * TW_81;
    t5_r = (s7_r + s7_i) * TW_81;
    t5_i = - (s7_r - s7_i) * TW_81;
    t0_r = s1_r + s5_i;
    t0_i = s1_i - s5_r;
    t1_r = s1_r - s5_i;
    t1_i = s1_i + s5_r;
    t2_r = t4_r - t5_r;
    t2_i = t4_i - t5_i;
    t3_r = t4_r + t5_r;
    t3_i = t4_i + t5_i;
    out[1].r = (t1_r + t2_r) * 0.125f;
    out[1].i = (t1_i + t2_i) * 0.125f;
    out[5].r = (t1_r - t2_r) * 0.125f;
    out[5].i = (t1_i - t2_i) * 0.125f;
    out[3].r = (t0_r - t3_i) * 0.125f;
    out[3].i = (t0_i + t3_r) * 0.125f;
    out[7].r = (t0_r + t3_i) * 0.125f;
    out[7].i = (t0_i - t3_r) * 0.125f;
}

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
    float32x4x2_t q2_out_0123, q2_out_4567, q2_out_89ab, q2_out_cdef;
    float32x4_t q_one_by_nfft;
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

    q_one_by_nfft = vdupq_n_f32 (0.0625f);
    q2_out_89ab.val[0] = vsubq_f32 (q2_out_0123.val[0], q_s3_r);
    q2_out_89ab.val[1] = vsubq_f32 (q2_out_0123.val[1], q_s3_i);
    q2_out_0123.val[0] = vaddq_f32 (q2_out_0123.val[0], q_s3_r);
    q2_out_0123.val[1] = vaddq_f32 (q2_out_0123.val[1], q_s3_i);

    q2_out_4567.val[0] = vsubq_f32 (q_s5_r, q_s4_i);
    q2_out_4567.val[1] = vaddq_f32 (q_s5_i, q_s4_r);
    q2_out_cdef.val[0] = vaddq_f32 (q_s5_r, q_s4_i);
    q2_out_cdef.val[1] = vsubq_f32 (q_s5_i, q_s4_r);

    q2_out_89ab.val[0] = vmulq_f32 (q2_out_89ab.val[0], q_one_by_nfft);
    q2_out_89ab.val[1] = vmulq_f32 (q2_out_89ab.val[1], q_one_by_nfft);
    q2_out_0123.val[0] = vmulq_f32 (q2_out_0123.val[0], q_one_by_nfft);
    q2_out_0123.val[1] = vmulq_f32 (q2_out_0123.val[1], q_one_by_nfft);
    q2_out_4567.val[0] = vmulq_f32 (q2_out_4567.val[0], q_one_by_nfft);
    q2_out_4567.val[1] = vmulq_f32 (q2_out_4567.val[1], q_one_by_nfft);
    q2_out_cdef.val[0] = vmulq_f32 (q2_out_cdef.val[0], q_one_by_nfft);
    q2_out_cdef.val[1] = vmulq_f32 (q2_out_cdef.val[1], q_one_by_nfft);

    vst2q_f32 (p_dst0, q2_out_0123);
    vst2q_f32 (p_dst1, q2_out_4567);
    vst2q_f32 (p_dst2, q2_out_89ab);
    vst2q_f32 (p_dst3, q2_out_cdef);
}

static inline void ne10_radix8x4_neon (ne10_fft_cpx_float32_t *out,
                                       ne10_fft_cpx_float32_t *in,
                                       ne10_int32_t stride)
{
    ne10_int32_t f_count;
    ne10_int32_t src_step = stride << 1; // ne10_fft_cpx_float32_t -> float32_t offset
    float32_t *p_src = (float32_t *) in;
    float32_t *p_dst = (float32_t *) out;
    const ne10_float32_t TW_81 = 0.70710678;
    const ne10_float32_t TW_81N = -0.70710678;

    float32x4x2_t q2_in0, q2_in1, q2_in2, q2_in3, q2_in4, q2_in5, q2_in6, q2_in7;
    float32x4_t q_sin0_r, q_sin0_i, q_sin1_r, q_sin1_i, q_sin2_r, q_sin2_i, q_sin3_r, q_sin3_i;
    float32x4_t q_sin4_r, q_sin4_i, q_sin5_r, q_sin5_i, q_sin6_r, q_sin6_i, q_sin7_r, q_sin7_i;
    float32x4_t q_s3_r, q_s3_i, q_s5_r, q_s5_i, q_s7_r, q_s7_i;
    float32x4_t q_s8_r, q_s8_i, q_s9_r, q_s9_i, q_s10_r, q_s10_i, q_s11_r, q_s11_i;
    float32x4_t q_s12_r, q_s12_i, q_s13_r, q_s13_i, q_s14_r, q_s14_i, q_s15_r, q_s15_i;
    float32x4_t q_out0_r, q_out0_i, q_out1_r, q_out1_i, q_out2_r, q_out2_i, q_out3_r, q_out3_i;
    float32x4_t q_out4_r, q_out4_i, q_out5_r, q_out5_i, q_out6_r, q_out6_i, q_out7_r, q_out7_i;
    float32x4x2_t q2_tmp0, q2_tmp1, q2_tmp2, q2_tmp3, q2_tmp4, q2_tmp5, q2_tmp6, q2_tmp7;
    float32x4x2_t q2_out0, q2_out1, q2_out2, q2_out3, q2_out4, q2_out5, q2_out6, q2_out7;
    float32x4_t q_tw_81, q_tw_81n;

    // This loop is unrolled four times, taking 16 NEON quadword registers of input to
    // process four radix-8 butterflies per loop iteration (in contrast to the single
    // radix-8 butterfly per iteration in the pure C counterpart).
    for (f_count = 0; f_count < stride; f_count += 4)
    {
        // Load the input values. Each q2_in* is a pair of two NEON quadword registers,
        // each member of which can hold two complex values (i.e. four float32 values).
        // Though we load four complex values at a time into the pairs here using VLD2Q,
        // the layout of the data ensures that we will not need to perform arithmetic
        // between values in the same vector.
        q2_in0 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in2 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in4 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in6 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in1 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in3 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in5 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in7 = vld2q_f32 (p_src);
        p_src += src_step;

        // Calculate sums for the butterfly calculations between the <X[0], X[4N/8]>,
        // <X[N/8], X[5N/8]>, <X[2N/8], X[6N/8]>, and <X[3N/8], X[7N/8]> components.
        q_sin0_r = vaddq_f32 (q2_in0.val[0], q2_in1.val[0]);
        q_sin0_i = vaddq_f32 (q2_in0.val[1], q2_in1.val[1]);
        q_sin1_r = vsubq_f32 (q2_in0.val[0], q2_in1.val[0]);
        q_sin1_i = vsubq_f32 (q2_in0.val[1], q2_in1.val[1]);
        q_sin2_r = vaddq_f32 (q2_in2.val[0], q2_in3.val[0]);
        q_sin2_i = vaddq_f32 (q2_in2.val[1], q2_in3.val[1]);
        q_sin3_r = vsubq_f32 (q2_in2.val[0], q2_in3.val[0]);
        q_sin3_i = vsubq_f32 (q2_in2.val[1], q2_in3.val[1]);
        q_sin4_r = vaddq_f32 (q2_in4.val[0], q2_in5.val[0]);
        q_sin4_i = vaddq_f32 (q2_in4.val[1], q2_in5.val[1]);
        q_sin5_r = vsubq_f32 (q2_in4.val[0], q2_in5.val[0]);
        q_sin5_i = vsubq_f32 (q2_in4.val[1], q2_in5.val[1]);
        q_sin6_r = vaddq_f32 (q2_in6.val[0], q2_in7.val[0]);
        q_sin6_i = vaddq_f32 (q2_in6.val[1], q2_in7.val[1]);
        q_sin7_r = vsubq_f32 (q2_in6.val[0], q2_in7.val[0]);
        q_sin7_i = vsubq_f32 (q2_in6.val[1], q2_in7.val[1]);

        // Multiply some of these by hardcoded radix-8 twiddles
        q_tw_81 = vdupq_n_f32 (TW_81);
        q_tw_81n = vdupq_n_f32 (TW_81N);
        q_s5_r = q_sin5_i;
        q_s5_i = vnegq_f32 (q_sin5_r);
        q_s3_r = vaddq_f32 (q_sin3_r, q_sin3_i);
        q_s3_i = vsubq_f32 (q_sin3_i, q_sin3_r);
        q_s7_r = vsubq_f32 (q_sin7_r, q_sin7_i);
        q_s7_i = vaddq_f32 (q_sin7_i, q_sin7_r);
        q_s3_r = vmulq_f32 (q_s3_r, q_tw_81);
        q_s3_i = vmulq_f32 (q_s3_i, q_tw_81);
        q_s7_r = vmulq_f32 (q_s7_r, q_tw_81n);
        q_s7_i = vmulq_f32 (q_s7_i, q_tw_81n);

        // Combine the first set of pairs of these sums
        q_s8_r = vaddq_f32 (q_sin0_r, q_sin4_r);
        q_s8_i = vaddq_f32 (q_sin0_i, q_sin4_i);
        q_s9_r = vaddq_f32 (q_sin1_r, q_s5_r);
        q_s9_i = vaddq_f32 (q_sin1_i, q_s5_i);
        q_s10_r = vsubq_f32 (q_sin0_r, q_sin4_r);
        q_s10_i = vsubq_f32 (q_sin0_i, q_sin4_i);
        q_s11_r = vsubq_f32 (q_sin1_r, q_s5_r);
        q_s11_i = vsubq_f32 (q_sin1_i, q_s5_i);

        // Combine the second set of pairs of these sums
        q_s12_r = vaddq_f32 (q_sin2_r, q_sin6_r);
        q_s12_i = vaddq_f32 (q_sin2_i, q_sin6_i);
        q_s13_r = vaddq_f32 (q_s3_r, q_s7_r);
        q_s13_i = vaddq_f32 (q_s3_i, q_s7_i);
        q_s14_r = vsubq_f32 (q_sin2_r, q_sin6_r);
        q_s14_i = vsubq_f32 (q_sin2_i, q_sin6_i);
        q_s15_r = vsubq_f32 (q_s3_r, q_s7_r);
        q_s15_i = vsubq_f32 (q_s3_i, q_s7_i);

        // Combine these combined components (for the full radix-8 butterfly)
        q_out4_r = vsubq_f32 (q_s8_r, q_s12_r);
        q_out4_i = vsubq_f32 (q_s8_i, q_s12_i);
        q_out5_r = vsubq_f32 (q_s9_r, q_s13_r);
        q_out5_i = vsubq_f32 (q_s9_i, q_s13_i);
        q_out0_r = vaddq_f32 (q_s8_r, q_s12_r);
        q_out0_i = vaddq_f32 (q_s8_i, q_s12_i);
        q_out1_r = vaddq_f32 (q_s9_r, q_s13_r);
        q_out1_i = vaddq_f32 (q_s9_i, q_s13_i);
        q_out2_r = vaddq_f32 (q_s10_r, q_s14_i);
        q_out2_i = vsubq_f32 (q_s10_i, q_s14_r);
        q_out3_r = vaddq_f32 (q_s11_r, q_s15_i);
        q_out3_i = vsubq_f32 (q_s11_i, q_s15_r);
        q_out6_r = vsubq_f32 (q_s10_r, q_s14_i);
        q_out6_i = vaddq_f32 (q_s10_i, q_s14_r);
        q_out7_r = vsubq_f32 (q_s11_r, q_s15_i);
        q_out7_i = vaddq_f32 (q_s11_i, q_s15_r);

        // Use VTRNQ and VCOMBINE to permute the vectors for contiguous stores. This is
        // necessary in this first stage as we wish to store the related calculated values
        // contiguously (as in the rolled pure C loop) even though they are not contiguous
        // in the vectors (instead, values that are from separate loop iterations in the C
        // version of the code are currently contiguous in vectors).
        q2_tmp0 = vtrnq_f32 (q_out0_r, q_out1_r);
        q2_tmp1 = vtrnq_f32 (q_out0_i, q_out1_i);
        q2_tmp2 = vtrnq_f32 (q_out2_r, q_out3_r);
        q2_tmp3 = vtrnq_f32 (q_out2_i, q_out3_i);
        q2_tmp4 = vtrnq_f32 (q_out4_r, q_out5_r);
        q2_tmp5 = vtrnq_f32 (q_out4_i, q_out5_i);
        q2_tmp6 = vtrnq_f32 (q_out6_r, q_out7_r);
        q2_tmp7 = vtrnq_f32 (q_out6_i, q_out7_i);
        q2_out0.val[0] = vcombine_f32 (vget_low_f32 (q2_tmp0.val[0]), vget_low_f32 (q2_tmp2.val[0]));
        q2_out0.val[1] = vcombine_f32 (vget_low_f32 (q2_tmp1.val[0]), vget_low_f32 (q2_tmp3.val[0]));
        q2_out2.val[0] = vcombine_f32 (vget_low_f32 (q2_tmp0.val[1]), vget_low_f32 (q2_tmp2.val[1]));
        q2_out2.val[1] = vcombine_f32 (vget_low_f32 (q2_tmp1.val[1]), vget_low_f32 (q2_tmp3.val[1]));
        q2_out4.val[0] = vcombine_f32 (vget_high_f32 (q2_tmp0.val[0]), vget_high_f32 (q2_tmp2.val[0]));
        q2_out4.val[1] = vcombine_f32 (vget_high_f32 (q2_tmp1.val[0]), vget_high_f32 (q2_tmp3.val[0]));
        q2_out6.val[0] = vcombine_f32 (vget_high_f32 (q2_tmp0.val[1]), vget_high_f32 (q2_tmp2.val[1]));
        q2_out6.val[1] = vcombine_f32 (vget_high_f32 (q2_tmp1.val[1]), vget_high_f32 (q2_tmp3.val[1]));
        q2_out1.val[0] = vcombine_f32 (vget_low_f32 (q2_tmp4.val[0]), vget_low_f32 (q2_tmp6.val[0]));
        q2_out1.val[1] = vcombine_f32 (vget_low_f32 (q2_tmp5.val[0]), vget_low_f32 (q2_tmp7.val[0]));
        q2_out3.val[0] = vcombine_f32 (vget_low_f32 (q2_tmp4.val[1]), vget_low_f32 (q2_tmp6.val[1]));
        q2_out3.val[1] = vcombine_f32 (vget_low_f32 (q2_tmp5.val[1]), vget_low_f32 (q2_tmp7.val[1]));
        q2_out5.val[0] = vcombine_f32 (vget_high_f32 (q2_tmp4.val[0]), vget_high_f32 (q2_tmp6.val[0]));
        q2_out5.val[1] = vcombine_f32 (vget_high_f32 (q2_tmp5.val[0]), vget_high_f32 (q2_tmp7.val[0]));
        q2_out7.val[0] = vcombine_f32 (vget_high_f32 (q2_tmp4.val[1]), vget_high_f32 (q2_tmp6.val[1]));
        q2_out7.val[1] = vcombine_f32 (vget_high_f32 (q2_tmp5.val[1]), vget_high_f32 (q2_tmp7.val[1]));

        // Store the results
        vst2q_f32 (p_dst, q2_out0);
        p_dst += 8;
        vst2q_f32 (p_dst, q2_out1);
        p_dst += 8;
        vst2q_f32 (p_dst, q2_out2);
        p_dst += 8;
        vst2q_f32 (p_dst, q2_out3);
        p_dst += 8;
        vst2q_f32 (p_dst, q2_out4);
        p_dst += 8;
        vst2q_f32 (p_dst, q2_out5);
        p_dst += 8;
        vst2q_f32 (p_dst, q2_out6);
        p_dst += 8;
        vst2q_f32 (p_dst, q2_out7);
        p_dst += 8;

        // Undo the p_src arithmetic from earlier in the loop, and add eight to skip past
        // the float32 values that have been used within this loop iteration.
        p_src = p_src - src_step * 8 + 8;
    } // f_count
}

static inline void ne10_radix4x4_without_twiddles_neon (ne10_fft_cpx_float32_t *out,
        ne10_fft_cpx_float32_t *in,
        ne10_int32_t stride)
{
    ne10_int32_t f_count;
    ne10_int32_t src_step = stride << 1; // ne10_fft_cpx_float32_t -> float32_t offset
    float32_t *p_src = (float32_t *) in;
    float32_t *p_dst = (float32_t *) out;

    float32x4x2_t q2_in0, q2_in1, q2_in2, q2_in3;
    float32x4_t q_s0_r, q_s0_i, q_s1_r, q_s1_i, q_s2_r, q_s2_i, q_s3_r, q_s3_i;
    float32x4_t q_out0_r, q_out0_i, q_out1_r, q_out1_i, q_out2_r, q_out2_i, q_out3_r, q_out3_i;
    float32x4x2_t q2_tmp0, q2_tmp1, q2_tmp2, q2_tmp3;
    float32x4x2_t q2_out0, q2_out1, q2_out2, q2_out3;

    // This loop is unrolled four times, taking 8 NEON quadword registers of input to
    // process four radix-4 butterflies per loop iteration.
    for (f_count = 0; f_count < stride; f_count += 4)
    {
        // Load the input values
        q2_in0 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in1 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in2 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in3 = vld2q_f32 (p_src);
        p_src += src_step;

        // Calculate sums for the butterfly calculations between the <X[0], X[2N/4]> and
        // <X[N/4], X[3N/4]> components.
        q_s0_r = vaddq_f32 (q2_in0.val[0], q2_in2.val[0]);
        q_s0_i = vaddq_f32 (q2_in0.val[1], q2_in2.val[1]);
        q_s1_r = vsubq_f32 (q2_in0.val[0], q2_in2.val[0]);
        q_s1_i = vsubq_f32 (q2_in0.val[1], q2_in2.val[1]);
        q_s2_r = vaddq_f32 (q2_in1.val[0], q2_in3.val[0]);
        q_s2_i = vaddq_f32 (q2_in1.val[1], q2_in3.val[1]);
        q_s3_r = vsubq_f32 (q2_in1.val[0], q2_in3.val[0]);
        q_s3_i = vsubq_f32 (q2_in1.val[1], q2_in3.val[1]);

        // Combine these sums (for the full radix-4 butterfly)
        q_out2_r = vsubq_f32 (q_s0_r, q_s2_r);
        q_out2_i = vsubq_f32 (q_s0_i, q_s2_i);
        q_out0_r = vaddq_f32 (q_s0_r, q_s2_r);
        q_out0_i = vaddq_f32 (q_s0_i, q_s2_i);
        q_out1_r = vaddq_f32 (q_s1_r, q_s3_i);
        q_out1_i = vsubq_f32 (q_s1_i, q_s3_r);
        q_out3_r = vsubq_f32 (q_s1_r, q_s3_i);
        q_out3_i = vaddq_f32 (q_s1_i, q_s3_r);

        // Permute the vectors for contiguous stores
        q2_tmp0 = vtrnq_f32 (q_out0_r, q_out1_r);
        q2_tmp1 = vtrnq_f32 (q_out0_i, q_out1_i);
        q2_tmp2 = vtrnq_f32 (q_out2_r, q_out3_r);
        q2_tmp3 = vtrnq_f32 (q_out2_i, q_out3_i);
        q2_out0.val[0] = vcombine_f32 (vget_low_f32 (q2_tmp0.val[0]), vget_low_f32 (q2_tmp2.val[0]));
        q2_out0.val[1] = vcombine_f32 (vget_low_f32 (q2_tmp1.val[0]), vget_low_f32 (q2_tmp3.val[0]));
        q2_out1.val[0] = vcombine_f32 (vget_low_f32 (q2_tmp0.val[1]), vget_low_f32 (q2_tmp2.val[1]));
        q2_out1.val[1] = vcombine_f32 (vget_low_f32 (q2_tmp1.val[1]), vget_low_f32 (q2_tmp3.val[1]));
        q2_out2.val[0] = vcombine_f32 (vget_high_f32 (q2_tmp0.val[0]), vget_high_f32 (q2_tmp2.val[0]));
        q2_out2.val[1] = vcombine_f32 (vget_high_f32 (q2_tmp1.val[0]), vget_high_f32 (q2_tmp3.val[0]));
        q2_out3.val[0] = vcombine_f32 (vget_high_f32 (q2_tmp0.val[1]), vget_high_f32 (q2_tmp2.val[1]));
        q2_out3.val[1] = vcombine_f32 (vget_high_f32 (q2_tmp1.val[1]), vget_high_f32 (q2_tmp3.val[1]));

        // Store the results
        vst2q_f32 (p_dst, q2_out0);
        p_dst += 8;
        vst2q_f32 (p_dst, q2_out1);
        p_dst += 8;
        vst2q_f32 (p_dst, q2_out2);
        p_dst += 8;
        vst2q_f32 (p_dst, q2_out3);
        p_dst += 8;

        // Adjust p_src for the next loop iteration
        p_src = p_src - src_step * 4 + 8;
    } // f_count
}

static inline void ne10_radix4x4_with_twiddles_neon (ne10_fft_cpx_float32_t *out,
        ne10_fft_cpx_float32_t *in,
        ne10_fft_cpx_float32_t *tw,
        ne10_int32_t src_stride,
        ne10_int32_t dst_stride,
        ne10_int32_t mstride)
{
    ne10_int32_t m_count;
    ne10_int32_t src_step = src_stride << 1; // ne10_fft_cpx_float32_t -> float32_t offsets
    ne10_int32_t dst_step = dst_stride << 1;
    ne10_int32_t tw_step  = mstride << 1;
    float32_t *p_src = (float32_t *) in;
    float32_t *p_dst = (float32_t *) out;
    float32_t *p_tw  = (float32_t *) tw;

    float32x4x2_t q2_in0, q2_in1, q2_in2, q2_in3;
    float32x4x2_t q2_tw0, q2_tw1, q2_tw2;
    float32x4_t q_s1_r, q_s1_i, q_s2_r, q_s2_i, q_s3_r, q_s3_i;
    float32x4_t q_s4_r, q_s4_i, q_s5_r, q_s5_i, q_s6_r, q_s6_i, q_s7_r, q_s7_i;
    float32x4x2_t q2_out0, q2_out1, q2_out2, q2_out3;

    // This loop is unrolled four times, taking 8 NEON quadword registers of input to
    // process four radix-4 butterflies per loop iteration.
    for (m_count = 0; m_count < mstride; m_count += 4)
    {
        // Load the input values
        q2_in0 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in1 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in2 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in3 = vld2q_f32 (p_src);
        p_src += src_step;

        // Load the twiddles
        q2_tw0 = vld2q_f32 (p_tw);
        p_tw += tw_step;
        q2_tw1 = vld2q_f32 (p_tw);
        p_tw += tw_step;
        q2_tw2 = vld2q_f32 (p_tw);

        // Multiply input elements by their associated twiddles
        q_s1_r = vmulq_f32 (q2_in1.val[0], q2_tw0.val[0]);
        q_s1_i = vmulq_f32 (q2_in1.val[1], q2_tw0.val[0]);
        q_s2_r = vmulq_f32 (q2_in2.val[0], q2_tw1.val[0]);
        q_s2_i = vmulq_f32 (q2_in2.val[1], q2_tw1.val[0]);
        q_s3_r = vmulq_f32 (q2_in3.val[0], q2_tw2.val[0]);
        q_s3_i = vmulq_f32 (q2_in3.val[1], q2_tw2.val[0]);
        q_s1_r = vmlsq_f32 (q_s1_r, q2_in1.val[1], q2_tw0.val[1]);
        q_s1_i = vmlaq_f32 (q_s1_i, q2_in1.val[0], q2_tw0.val[1]);
        q_s2_r = vmlsq_f32 (q_s2_r, q2_in2.val[1], q2_tw1.val[1]);
        q_s2_i = vmlaq_f32 (q_s2_i, q2_in2.val[0], q2_tw1.val[1]);
        q_s3_r = vmlsq_f32 (q_s3_r, q2_in3.val[1], q2_tw2.val[1]);
        q_s3_i = vmlaq_f32 (q_s3_i, q2_in3.val[0], q2_tw2.val[1]);

        // Calculate sums for the butterfly calculations between the <X[0], X[2N/4]> and
        // <X[N/4], X[3N/4]> components.
        q_s4_r = vaddq_f32 (q2_in0.val[0], q_s2_r);
        q_s4_i = vaddq_f32 (q2_in0.val[1], q_s2_i);
        q_s5_r = vsubq_f32 (q2_in0.val[0], q_s2_r);
        q_s5_i = vsubq_f32 (q2_in0.val[1], q_s2_i);
        q_s6_r = vaddq_f32 (q_s1_r, q_s3_r);
        q_s6_i = vaddq_f32 (q_s1_i, q_s3_i);
        q_s7_r = vsubq_f32 (q_s1_r, q_s3_r);
        q_s7_i = vsubq_f32 (q_s1_i, q_s3_i);

        // Combine these sums (for the full radix-4 butterfly)
        q2_out2.val[0] = vsubq_f32 (q_s4_r, q_s6_r);
        q2_out2.val[1] = vsubq_f32 (q_s4_i, q_s6_i);
        q2_out0.val[0] = vaddq_f32 (q_s4_r, q_s6_r);
        q2_out0.val[1] = vaddq_f32 (q_s4_i, q_s6_i);
        q2_out1.val[0] = vaddq_f32 (q_s5_r, q_s7_i);
        q2_out1.val[1] = vsubq_f32 (q_s5_i, q_s7_r);
        q2_out3.val[0] = vsubq_f32 (q_s5_r, q_s7_i);
        q2_out3.val[1] = vaddq_f32 (q_s5_i, q_s7_r);

        // Store the results
        vst2q_f32 (p_dst, q2_out0);
        p_dst += dst_step;
        vst2q_f32 (p_dst, q2_out1);
        p_dst += dst_step;
        vst2q_f32 (p_dst, q2_out2);
        p_dst += dst_step;
        vst2q_f32 (p_dst, q2_out3);
        p_dst += dst_step;

        // Undo the arithmetic we did to these variables earlier in the loop, and add
        // eight to skip past the float32 values processed within this loop iteration.
        p_src = p_src - src_step * 4 + 8;
        p_dst = p_dst - dst_step * 4 + 8;
        p_tw  = p_tw - tw_step * 2 + 8;
    } // m_count
}
static inline void ne10_radix8x4_inverse_neon (ne10_fft_cpx_float32_t *out,
        ne10_fft_cpx_float32_t *in,
        ne10_int32_t stride)
{
    ne10_int32_t f_count;
    ne10_int32_t src_step = stride << 1;
    float32_t *p_src = (float32_t *) in;
    float32_t *p_dst = (float32_t *) out;
    const ne10_float32_t TW_81 = 0.70710678;
    const ne10_float32_t TW_81N = -0.70710678;

    float32x4x2_t q2_in0, q2_in1, q2_in2, q2_in3, q2_in4, q2_in5, q2_in6, q2_in7;
    float32x4_t q_sin0_r, q_sin0_i, q_sin1_r, q_sin1_i, q_sin2_r, q_sin2_i, q_sin3_r, q_sin3_i;
    float32x4_t q_sin4_r, q_sin4_i, q_sin5_r, q_sin5_i, q_sin6_r, q_sin6_i, q_sin7_r, q_sin7_i;
    float32x4_t q_s3_r, q_s3_i, q_s5_r, q_s5_i, q_s7_r, q_s7_i;
    float32x4_t q_s8_r, q_s8_i, q_s9_r, q_s9_i, q_s10_r, q_s10_i, q_s11_r, q_s11_i;
    float32x4_t q_s12_r, q_s12_i, q_s13_r, q_s13_i, q_s14_r, q_s14_i, q_s15_r, q_s15_i;
    float32x4_t q_out0_r, q_out0_i, q_out1_r, q_out1_i, q_out2_r, q_out2_i, q_out3_r, q_out3_i;
    float32x4_t q_out4_r, q_out4_i, q_out5_r, q_out5_i, q_out6_r, q_out6_i, q_out7_r, q_out7_i;
    float32x4x2_t q2_tmp0, q2_tmp1, q2_tmp2, q2_tmp3, q2_tmp4, q2_tmp5, q2_tmp6, q2_tmp7;
    float32x4x2_t q2_out0, q2_out1, q2_out2, q2_out3, q2_out4, q2_out5, q2_out6, q2_out7;
    float32x4_t q_tw_81, q_tw_81n;

    // This loop is unrolled four times, taking 16 NEON quadword registers of input to
    // process four radix-8 butterflies per loop iteration.
    for (f_count = 0; f_count < stride; f_count += 4)
    {
        // Load the input values
        q2_in0 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in2 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in4 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in6 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in1 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in3 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in5 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in7 = vld2q_f32 (p_src);
        p_src += src_step;

        // Calculate sums for the butterfly calculations between the <X[0], X[4N/8]>,
        // <X[N/8], X[5N/8]>, <X[2N/8], X[6N/8]>, and <X[3N/8], X[7N/8]> components.
        q_sin0_r = vaddq_f32 (q2_in0.val[0], q2_in1.val[0]);
        q_sin0_i = vaddq_f32 (q2_in0.val[1], q2_in1.val[1]);
        q_sin1_r = vsubq_f32 (q2_in0.val[0], q2_in1.val[0]);
        q_sin1_i = vsubq_f32 (q2_in0.val[1], q2_in1.val[1]);
        q_sin2_r = vaddq_f32 (q2_in2.val[0], q2_in3.val[0]);
        q_sin2_i = vaddq_f32 (q2_in2.val[1], q2_in3.val[1]);
        q_sin3_r = vsubq_f32 (q2_in2.val[0], q2_in3.val[0]);
        q_sin3_i = vsubq_f32 (q2_in2.val[1], q2_in3.val[1]);
        q_sin4_r = vaddq_f32 (q2_in4.val[0], q2_in5.val[0]);
        q_sin4_i = vaddq_f32 (q2_in4.val[1], q2_in5.val[1]);
        q_sin5_r = vsubq_f32 (q2_in4.val[0], q2_in5.val[0]);
        q_sin5_i = vsubq_f32 (q2_in4.val[1], q2_in5.val[1]);
        q_sin6_r = vaddq_f32 (q2_in6.val[0], q2_in7.val[0]);
        q_sin6_i = vaddq_f32 (q2_in6.val[1], q2_in7.val[1]);
        q_sin7_r = vsubq_f32 (q2_in6.val[0], q2_in7.val[0]);
        q_sin7_i = vsubq_f32 (q2_in6.val[1], q2_in7.val[1]);

        // Multiply some of these by hardcoded radix-8 twiddles
        q_tw_81 = vdupq_n_f32 (TW_81);
        q_tw_81n = vdupq_n_f32 (TW_81N);
        q_s5_r = vnegq_f32 (q_sin5_i);
        q_s5_i = q_sin5_r;
        q_s3_r = vsubq_f32 (q_sin3_r, q_sin3_i);
        q_s3_i = vaddq_f32 (q_sin3_i, q_sin3_r);
        q_s7_r = vaddq_f32 (q_sin7_r, q_sin7_i);
        q_s7_i = vsubq_f32 (q_sin7_i, q_sin7_r);
        q_s3_r = vmulq_f32 (q_s3_r, q_tw_81);
        q_s3_i = vmulq_f32 (q_s3_i, q_tw_81);
        q_s7_r = vmulq_f32 (q_s7_r, q_tw_81n);
        q_s7_i = vmulq_f32 (q_s7_i, q_tw_81n);

        // Combine the first set of pairs of these sums
        q_s8_r = vaddq_f32 (q_sin0_r, q_sin4_r);
        q_s8_i = vaddq_f32 (q_sin0_i, q_sin4_i);
        q_s9_r = vaddq_f32 (q_sin1_r, q_s5_r);
        q_s9_i = vaddq_f32 (q_sin1_i, q_s5_i);
        q_s10_r = vsubq_f32 (q_sin0_r, q_sin4_r);
        q_s10_i = vsubq_f32 (q_sin0_i, q_sin4_i);
        q_s11_r = vsubq_f32 (q_sin1_r, q_s5_r);
        q_s11_i = vsubq_f32 (q_sin1_i, q_s5_i);

        // Combine the second set of pairs of these sums
        q_s12_r = vaddq_f32 (q_sin2_r, q_sin6_r);
        q_s12_i = vaddq_f32 (q_sin2_i, q_sin6_i);
        q_s13_r = vaddq_f32 (q_s3_r, q_s7_r);
        q_s13_i = vaddq_f32 (q_s3_i, q_s7_i);
        q_s14_r = vsubq_f32 (q_sin2_r, q_sin6_r);
        q_s14_i = vsubq_f32 (q_sin2_i, q_sin6_i);
        q_s15_r = vsubq_f32 (q_s3_r, q_s7_r);
        q_s15_i = vsubq_f32 (q_s3_i, q_s7_i);

        // Combine these combined components (for the full radix-8 butterfly)
        q_out4_r = vsubq_f32 (q_s8_r, q_s12_r);
        q_out4_i = vsubq_f32 (q_s8_i, q_s12_i);
        q_out5_r = vsubq_f32 (q_s9_r, q_s13_r);
        q_out5_i = vsubq_f32 (q_s9_i, q_s13_i);
        q_out0_r = vaddq_f32 (q_s8_r, q_s12_r);
        q_out0_i = vaddq_f32 (q_s8_i, q_s12_i);
        q_out1_r = vaddq_f32 (q_s9_r, q_s13_r);
        q_out1_i = vaddq_f32 (q_s9_i, q_s13_i);
        q_out2_r = vsubq_f32 (q_s10_r, q_s14_i);
        q_out2_i = vaddq_f32 (q_s10_i, q_s14_r);
        q_out3_r = vsubq_f32 (q_s11_r, q_s15_i);
        q_out3_i = vaddq_f32 (q_s11_i, q_s15_r);
        q_out6_r = vaddq_f32 (q_s10_r, q_s14_i);
        q_out6_i = vsubq_f32 (q_s10_i, q_s14_r);
        q_out7_r = vaddq_f32 (q_s11_r, q_s15_i);
        q_out7_i = vsubq_f32 (q_s11_i, q_s15_r);

        // Permute the vectors for contiguous stores
        q2_tmp0 = vtrnq_f32 (q_out0_r, q_out1_r);
        q2_tmp1 = vtrnq_f32 (q_out0_i, q_out1_i);
        q2_tmp2 = vtrnq_f32 (q_out2_r, q_out3_r);
        q2_tmp3 = vtrnq_f32 (q_out2_i, q_out3_i);
        q2_tmp4 = vtrnq_f32 (q_out4_r, q_out5_r);
        q2_tmp5 = vtrnq_f32 (q_out4_i, q_out5_i);
        q2_tmp6 = vtrnq_f32 (q_out6_r, q_out7_r);
        q2_tmp7 = vtrnq_f32 (q_out6_i, q_out7_i);
        q2_out0.val[0] = vcombine_f32 (vget_low_f32 (q2_tmp0.val[0]), vget_low_f32 (q2_tmp2.val[0]));
        q2_out0.val[1] = vcombine_f32 (vget_low_f32 (q2_tmp1.val[0]), vget_low_f32 (q2_tmp3.val[0]));
        q2_out2.val[0] = vcombine_f32 (vget_low_f32 (q2_tmp0.val[1]), vget_low_f32 (q2_tmp2.val[1]));
        q2_out2.val[1] = vcombine_f32 (vget_low_f32 (q2_tmp1.val[1]), vget_low_f32 (q2_tmp3.val[1]));
        q2_out4.val[0] = vcombine_f32 (vget_high_f32 (q2_tmp0.val[0]), vget_high_f32 (q2_tmp2.val[0]));
        q2_out4.val[1] = vcombine_f32 (vget_high_f32 (q2_tmp1.val[0]), vget_high_f32 (q2_tmp3.val[0]));
        q2_out6.val[0] = vcombine_f32 (vget_high_f32 (q2_tmp0.val[1]), vget_high_f32 (q2_tmp2.val[1]));
        q2_out6.val[1] = vcombine_f32 (vget_high_f32 (q2_tmp1.val[1]), vget_high_f32 (q2_tmp3.val[1]));
        q2_out1.val[0] = vcombine_f32 (vget_low_f32 (q2_tmp4.val[0]), vget_low_f32 (q2_tmp6.val[0]));
        q2_out1.val[1] = vcombine_f32 (vget_low_f32 (q2_tmp5.val[0]), vget_low_f32 (q2_tmp7.val[0]));
        q2_out3.val[0] = vcombine_f32 (vget_low_f32 (q2_tmp4.val[1]), vget_low_f32 (q2_tmp6.val[1]));
        q2_out3.val[1] = vcombine_f32 (vget_low_f32 (q2_tmp5.val[1]), vget_low_f32 (q2_tmp7.val[1]));
        q2_out5.val[0] = vcombine_f32 (vget_high_f32 (q2_tmp4.val[0]), vget_high_f32 (q2_tmp6.val[0]));
        q2_out5.val[1] = vcombine_f32 (vget_high_f32 (q2_tmp5.val[0]), vget_high_f32 (q2_tmp7.val[0]));
        q2_out7.val[0] = vcombine_f32 (vget_high_f32 (q2_tmp4.val[1]), vget_high_f32 (q2_tmp6.val[1]));
        q2_out7.val[1] = vcombine_f32 (vget_high_f32 (q2_tmp5.val[1]), vget_high_f32 (q2_tmp7.val[1]));

        // Store the results
        vst2q_f32 (p_dst, q2_out0);
        p_dst += 8;
        vst2q_f32 (p_dst, q2_out1);
        p_dst += 8;
        vst2q_f32 (p_dst, q2_out2);
        p_dst += 8;
        vst2q_f32 (p_dst, q2_out3);
        p_dst += 8;
        vst2q_f32 (p_dst, q2_out4);
        p_dst += 8;
        vst2q_f32 (p_dst, q2_out5);
        p_dst += 8;
        vst2q_f32 (p_dst, q2_out6);
        p_dst += 8;
        vst2q_f32 (p_dst, q2_out7);
        p_dst += 8;

        // Adjust p_src for the next loop iteration
        p_src = p_src - src_step * 8 + 8;
    } // f_count
}

static inline void ne10_radix4x4_inverse_without_twiddles_neon (ne10_fft_cpx_float32_t *out,
        ne10_fft_cpx_float32_t *in,
        ne10_int32_t stride)
{
    ne10_int32_t f_count;
    ne10_int32_t src_step = stride << 1;
    float32_t *p_src = (float32_t *) in;
    float32_t *p_dst = (float32_t *) out;

    float32x4x2_t q2_in0, q2_in1, q2_in2, q2_in3;
    float32x4_t q_s0_r, q_s0_i, q_s1_r, q_s1_i, q_s2_r, q_s2_i, q_s3_r, q_s3_i;
    float32x4_t q_out0_r, q_out0_i, q_out1_r, q_out1_i, q_out2_r, q_out2_i, q_out3_r, q_out3_i;
    float32x4x2_t q2_tmp0, q2_tmp1, q2_tmp2, q2_tmp3;
    float32x4x2_t q2_out0, q2_out1, q2_out2, q2_out3;

    // This loop is unrolled four times, taking 8 NEON quadword registers of input to
    // process four radix-4 butterflies per loop iteration.
    for (f_count = 0; f_count < stride; f_count += 4)
    {
        // Load the input values
        q2_in0 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in1 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in2 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in3 = vld2q_f32 (p_src);
        p_src += src_step;

        // Calculate sums for the butterfly calculations between the <X[0], X[2N/4]> and
        // <X[N/4], X[3N/4]> components.
        q_s0_r = vaddq_f32 (q2_in0.val[0], q2_in2.val[0]);
        q_s0_i = vaddq_f32 (q2_in0.val[1], q2_in2.val[1]);
        q_s1_r = vsubq_f32 (q2_in0.val[0], q2_in2.val[0]);
        q_s1_i = vsubq_f32 (q2_in0.val[1], q2_in2.val[1]);
        q_s2_r = vaddq_f32 (q2_in1.val[0], q2_in3.val[0]);
        q_s2_i = vaddq_f32 (q2_in1.val[1], q2_in3.val[1]);
        q_s3_r = vsubq_f32 (q2_in1.val[0], q2_in3.val[0]);
        q_s3_i = vsubq_f32 (q2_in1.val[1], q2_in3.val[1]);

        // Combine these sums (for the full radix-4 butterfly)
        q_out2_r = vsubq_f32 (q_s0_r, q_s2_r);
        q_out2_i = vsubq_f32 (q_s0_i, q_s2_i);
        q_out0_r = vaddq_f32 (q_s0_r, q_s2_r);
        q_out0_i = vaddq_f32 (q_s0_i, q_s2_i);
        q_out1_r = vsubq_f32 (q_s1_r, q_s3_i);
        q_out1_i = vaddq_f32 (q_s1_i, q_s3_r);
        q_out3_r = vaddq_f32 (q_s1_r, q_s3_i);
        q_out3_i = vsubq_f32 (q_s1_i, q_s3_r);

        // Permute the vectors for contiguous stores
        q2_tmp0 = vtrnq_f32 (q_out0_r, q_out1_r);
        q2_tmp1 = vtrnq_f32 (q_out0_i, q_out1_i);
        q2_tmp2 = vtrnq_f32 (q_out2_r, q_out3_r);
        q2_tmp3 = vtrnq_f32 (q_out2_i, q_out3_i);
        q2_out0.val[0] = vcombine_f32 (vget_low_f32 (q2_tmp0.val[0]), vget_low_f32 (q2_tmp2.val[0]));
        q2_out0.val[1] = vcombine_f32 (vget_low_f32 (q2_tmp1.val[0]), vget_low_f32 (q2_tmp3.val[0]));
        q2_out1.val[0] = vcombine_f32 (vget_low_f32 (q2_tmp0.val[1]), vget_low_f32 (q2_tmp2.val[1]));
        q2_out1.val[1] = vcombine_f32 (vget_low_f32 (q2_tmp1.val[1]), vget_low_f32 (q2_tmp3.val[1]));
        q2_out2.val[0] = vcombine_f32 (vget_high_f32 (q2_tmp0.val[0]), vget_high_f32 (q2_tmp2.val[0]));
        q2_out2.val[1] = vcombine_f32 (vget_high_f32 (q2_tmp1.val[0]), vget_high_f32 (q2_tmp3.val[0]));
        q2_out3.val[0] = vcombine_f32 (vget_high_f32 (q2_tmp0.val[1]), vget_high_f32 (q2_tmp2.val[1]));
        q2_out3.val[1] = vcombine_f32 (vget_high_f32 (q2_tmp1.val[1]), vget_high_f32 (q2_tmp3.val[1]));

        // Store the results
        vst2q_f32 (p_dst, q2_out0);
        p_dst += 8;
        vst2q_f32 (p_dst, q2_out1);
        p_dst += 8;
        vst2q_f32 (p_dst, q2_out2);
        p_dst += 8;
        vst2q_f32 (p_dst, q2_out3);
        p_dst += 8;

        // Adjust p_src for the next loop iteration
        p_src = p_src - src_step * 4 + 8;
    } // f_count
}

static inline void ne10_radix4x4_inverse_with_twiddles_neon (ne10_fft_cpx_float32_t *out,
        ne10_fft_cpx_float32_t *in,
        ne10_fft_cpx_float32_t *tw,
        ne10_int32_t src_stride,
        ne10_int32_t dst_stride,
        ne10_int32_t mstride)
{
    ne10_int32_t m_count;
    ne10_int32_t src_step = src_stride << 1;
    ne10_int32_t dst_step = dst_stride << 1;
    ne10_int32_t tw_step  = mstride << 1;
    float32_t *p_src = (float32_t *) in;
    float32_t *p_dst = (float32_t *) out;
    float32_t *p_tw  = (float32_t *) tw;

    float32x4x2_t q2_in0, q2_in1, q2_in2, q2_in3;
    float32x4x2_t q2_tw0, q2_tw1, q2_tw2;
    float32x4_t q_s1_r, q_s1_i, q_s2_r, q_s2_i, q_s3_r, q_s3_i;
    float32x4_t q_s4_r, q_s4_i, q_s5_r, q_s5_i, q_s6_r, q_s6_i, q_s7_r, q_s7_i;
    float32x4x2_t q2_out0, q2_out1, q2_out2, q2_out3;

    // This loop is unrolled four times, taking 8 NEON quadword registers of input to
    // process four radix-4 butterflies per loop iteration.
    for (m_count = 0; m_count < mstride; m_count += 4)
    {
        // Load the input values
        q2_in0 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in1 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in2 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in3 = vld2q_f32 (p_src);
        p_src += src_step;

        // Load the twiddles
        q2_tw0 = vld2q_f32 (p_tw);
        p_tw += tw_step;
        q2_tw1 = vld2q_f32 (p_tw);
        p_tw += tw_step;
        q2_tw2 = vld2q_f32 (p_tw);

        // Multiply input elements by their associated twiddles
        q_s1_r = vmulq_f32 (q2_in1.val[0], q2_tw0.val[0]);
        q_s1_i = vmulq_f32 (q2_in1.val[1], q2_tw0.val[0]);
        q_s2_r = vmulq_f32 (q2_in2.val[0], q2_tw1.val[0]);
        q_s2_i = vmulq_f32 (q2_in2.val[1], q2_tw1.val[0]);
        q_s3_r = vmulq_f32 (q2_in3.val[0], q2_tw2.val[0]);
        q_s3_i = vmulq_f32 (q2_in3.val[1], q2_tw2.val[0]);
        q_s1_r = vmlaq_f32 (q_s1_r, q2_in1.val[1], q2_tw0.val[1]);
        q_s1_i = vmlsq_f32 (q_s1_i, q2_in1.val[0], q2_tw0.val[1]);
        q_s2_r = vmlaq_f32 (q_s2_r, q2_in2.val[1], q2_tw1.val[1]);
        q_s2_i = vmlsq_f32 (q_s2_i, q2_in2.val[0], q2_tw1.val[1]);
        q_s3_r = vmlaq_f32 (q_s3_r, q2_in3.val[1], q2_tw2.val[1]);
        q_s3_i = vmlsq_f32 (q_s3_i, q2_in3.val[0], q2_tw2.val[1]);

        // Calculate sums for the butterfly calculations between the <X[0], X[2N/4]> and
        // <X[N/4], X[3N/4]> components.
        q_s4_r = vaddq_f32 (q2_in0.val[0], q_s2_r);
        q_s4_i = vaddq_f32 (q2_in0.val[1], q_s2_i);
        q_s5_r = vsubq_f32 (q2_in0.val[0], q_s2_r);
        q_s5_i = vsubq_f32 (q2_in0.val[1], q_s2_i);
        q_s6_r = vaddq_f32 (q_s1_r, q_s3_r);
        q_s6_i = vaddq_f32 (q_s1_i, q_s3_i);
        q_s7_r = vsubq_f32 (q_s1_r, q_s3_r);
        q_s7_i = vsubq_f32 (q_s1_i, q_s3_i);

        // Combine these sums (for the full radix-4 butterfly)
        q2_out2.val[0] = vsubq_f32 (q_s4_r, q_s6_r);
        q2_out2.val[1] = vsubq_f32 (q_s4_i, q_s6_i);
        q2_out0.val[0] = vaddq_f32 (q_s4_r, q_s6_r);
        q2_out0.val[1] = vaddq_f32 (q_s4_i, q_s6_i);
        q2_out1.val[0] = vsubq_f32 (q_s5_r, q_s7_i);
        q2_out1.val[1] = vaddq_f32 (q_s5_i, q_s7_r);
        q2_out3.val[0] = vaddq_f32 (q_s5_r, q_s7_i);
        q2_out3.val[1] = vsubq_f32 (q_s5_i, q_s7_r);

        // Store the results
        vst2q_f32 (p_dst, q2_out0);
        p_dst += dst_step;
        vst2q_f32 (p_dst, q2_out1);
        p_dst += dst_step;
        vst2q_f32 (p_dst, q2_out2);
        p_dst += dst_step;
        vst2q_f32 (p_dst, q2_out3);
        p_dst += dst_step;

        // Adjust p_src, p_dst, p_tw for the next loop iteration
        p_src = p_src - src_step * 4 + 8;
        p_dst = p_dst - dst_step * 4 + 8;
        p_tw = p_tw - tw_step * 2 + 8;
    } // m_count
}

static inline void ne10_radix4x4_inverse_with_twiddles_last_stage_neon (ne10_fft_cpx_float32_t *out,
        ne10_fft_cpx_float32_t *in,
        ne10_fft_cpx_float32_t *tw,
        ne10_int32_t src_stride,
        ne10_int32_t dst_stride,
        ne10_int32_t mstride,
        ne10_int32_t nfft)
{
    ne10_int32_t m_count;
    ne10_int32_t src_step = src_stride << 1;
    ne10_int32_t dst_step = dst_stride << 1;
    ne10_int32_t tw_step  = mstride << 1;
    float32_t *p_src = (float32_t *) in;
    float32_t *p_dst = (float32_t *) out;
    float32_t *p_tw  = (float32_t *) tw;
    ne10_float32_t one_by_nfft = (1.0f / (ne10_float32_t) nfft);

    float32x4x2_t q2_in0, q2_in1, q2_in2, q2_in3;
    float32x4x2_t q2_tw0, q2_tw1, q2_tw2;
    float32x4_t q_s1_r, q_s1_i, q_s2_r, q_s2_i, q_s3_r, q_s3_i;
    float32x4_t q_s4_r, q_s4_i, q_s5_r, q_s5_i, q_s6_r, q_s6_i, q_s7_r, q_s7_i;
    float32x4x2_t q2_out0, q2_out1, q2_out2, q2_out3;
    float32x4_t q_one_by_nfft = vdupq_n_f32 (one_by_nfft);

    // This loop is unrolled four times, taking 8 NEON quadword registers of input to
    // process four radix-4 butterflies per loop iteration.
    for (m_count = 0; m_count < mstride; m_count += 4)
    {
        // Load the input values
        q2_in0 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in1 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in2 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in3 = vld2q_f32 (p_src);
        p_src += src_step;

        // Load the twiddles
        q2_tw0 = vld2q_f32 (p_tw);
        p_tw += tw_step;
        q2_tw1 = vld2q_f32 (p_tw);
        p_tw += tw_step;
        q2_tw2 = vld2q_f32 (p_tw);

        // Multiply input elements by their associated twiddles
        q_s1_r = vmulq_f32 (q2_in1.val[0], q2_tw0.val[0]);
        q_s1_i = vmulq_f32 (q2_in1.val[1], q2_tw0.val[0]);
        q_s2_r = vmulq_f32 (q2_in2.val[0], q2_tw1.val[0]);
        q_s2_i = vmulq_f32 (q2_in2.val[1], q2_tw1.val[0]);
        q_s3_r = vmulq_f32 (q2_in3.val[0], q2_tw2.val[0]);
        q_s3_i = vmulq_f32 (q2_in3.val[1], q2_tw2.val[0]);
        q_s1_r = vmlaq_f32 (q_s1_r, q2_in1.val[1], q2_tw0.val[1]);
        q_s1_i = vmlsq_f32 (q_s1_i, q2_in1.val[0], q2_tw0.val[1]);
        q_s2_r = vmlaq_f32 (q_s2_r, q2_in2.val[1], q2_tw1.val[1]);
        q_s2_i = vmlsq_f32 (q_s2_i, q2_in2.val[0], q2_tw1.val[1]);
        q_s3_r = vmlaq_f32 (q_s3_r, q2_in3.val[1], q2_tw2.val[1]);
        q_s3_i = vmlsq_f32 (q_s3_i, q2_in3.val[0], q2_tw2.val[1]);

        // Calculate sums for the butterfly calculations between the <X[0], X[2N/4]> and
        // <X[N/4], X[3N/4]> components.
        q_s4_r = vaddq_f32 (q2_in0.val[0], q_s2_r);
        q_s4_i = vaddq_f32 (q2_in0.val[1], q_s2_i);
        q_s5_r = vsubq_f32 (q2_in0.val[0], q_s2_r);
        q_s5_i = vsubq_f32 (q2_in0.val[1], q_s2_i);
        q_s6_r = vaddq_f32 (q_s1_r, q_s3_r);
        q_s6_i = vaddq_f32 (q_s1_i, q_s3_i);
        q_s7_r = vsubq_f32 (q_s1_r, q_s3_r);
        q_s7_i = vsubq_f32 (q_s1_i, q_s3_i);

        // Combine these sums (for the full radix-4 butterfly)
        q2_out2.val[0] = vsubq_f32 (q_s4_r, q_s6_r);
        q2_out2.val[1] = vsubq_f32 (q_s4_i, q_s6_i);
        q2_out0.val[0] = vaddq_f32 (q_s4_r, q_s6_r);
        q2_out0.val[1] = vaddq_f32 (q_s4_i, q_s6_i);
        q2_out1.val[0] = vsubq_f32 (q_s5_r, q_s7_i);
        q2_out1.val[1] = vaddq_f32 (q_s5_i, q_s7_r);
        q2_out3.val[0] = vaddq_f32 (q_s5_r, q_s7_i);
        q2_out3.val[1] = vsubq_f32 (q_s5_i, q_s7_r);

        // Multiply by (1 / nfft)
        q2_out0.val[0] = vmulq_f32 (q2_out0.val[0], q_one_by_nfft);
        q2_out0.val[1] = vmulq_f32 (q2_out0.val[1], q_one_by_nfft);
        q2_out1.val[0] = vmulq_f32 (q2_out1.val[0], q_one_by_nfft);
        q2_out1.val[1] = vmulq_f32 (q2_out1.val[1], q_one_by_nfft);
        q2_out2.val[0] = vmulq_f32 (q2_out2.val[0], q_one_by_nfft);
        q2_out2.val[1] = vmulq_f32 (q2_out2.val[1], q_one_by_nfft);
        q2_out3.val[0] = vmulq_f32 (q2_out3.val[0], q_one_by_nfft);
        q2_out3.val[1] = vmulq_f32 (q2_out3.val[1], q_one_by_nfft);

        // Store the results
        vst2q_f32 (p_dst, q2_out0);
        p_dst += dst_step;
        vst2q_f32 (p_dst, q2_out1);
        p_dst += dst_step;
        vst2q_f32 (p_dst, q2_out2);
        p_dst += dst_step;
        vst2q_f32 (p_dst, q2_out3);
        p_dst += dst_step;

        // Adjust p_src, p_dst, p_tw for the next loop iteration
        p_src = p_src - src_step * 4 + 8;
        p_dst = p_dst - dst_step * 4 + 8;
        p_tw = p_tw - tw_step * 2 + 8;
    } // m_count
}

/*
 * This function calculates the FFT for power-of-two input sizes using an ordered, mixed
 * radix-4/8 DIT algorithm. It is very similar to its C counterpart in NE10_fft_float32.c,
 * "ne10_mixed_radix_butterfly_float32_c".
 */
void ne10_mixed_radix_fft_forward_float32_neon (ne10_fft_cpx_float32_t *out,
        ne10_fft_cpx_float32_t *in,
        ne10_int32_t *factors,
        ne10_fft_cpx_float32_t *twiddles,
        ne10_fft_cpx_float32_t *buffer)
{
    ne10_int32_t stage_count = factors[0];
    ne10_int32_t fstride = factors[1];
    ne10_int32_t mstride = factors[(stage_count << 1) - 1];
    ne10_int32_t first_radix = factors[stage_count << 1];
    ne10_int32_t step, f_count;
    ne10_fft_cpx_float32_t *src = in;
    ne10_fft_cpx_float32_t *dst = out;
    ne10_fft_cpx_float32_t *out_final = out;
    ne10_fft_cpx_float32_t *tw, *tmp;

    // The first stage (using hardcoded twiddles)
    if (first_radix == 2) // For radix-4 factoring, this means nfft is of form 2^{odd}
    {
        // Instead of performing a radix-2 butterfly as the factors array suggests,
        // we will instead perform a radix-8 butterfly.
        ne10_radix8x4_neon (dst, src, fstride / 4);

        // Update variables for the next stages
        step = fstride >> 1; // For C2C, 1/4 of input size (fstride is nfft/2)
        mstride *= 4;
        fstride /= 16;
        stage_count -= 2;
        twiddles += 6; // Skip the twiddles we no longer need (as we did radix 8, not 2)
    }
    else if (first_radix == 4) // For radix-4 factoring, this means nfft is of form 2^{even}
    {
        ne10_radix4x4_without_twiddles_neon (dst, src, fstride);

        // Update variables for the next stages
        step = fstride; // For C2C, 1/4 of input size (fstride is nfft/4)
        stage_count--;
        fstride /= 4;
    }

    // The next stage should read the output of the first stage as input
    in = out;
    out = buffer;

    // Middle stages (after the first, excluding the last)
    for (; stage_count > 1 ; stage_count--)
    {
        src = in;
        for (f_count = 0; f_count < fstride; f_count ++)
        {
            dst = &out[f_count * (mstride * 4)];
            tw = twiddles; // Reset the twiddle pointer for the next section
            ne10_radix4x4_with_twiddles_neon (dst, src, tw, step, mstride, mstride);
            src += mstride;
        } // f_count

        // Update variables for the next stages
        twiddles += mstride * 3;
        mstride *= 4;
        fstride /= 4;

        // Swap the input and output buffers for the next stage
        tmp = in;
        in = out;
        out = tmp;
    } // stage_count

    // The last stage
    if (stage_count)
    {
        src = in;

        // Always write to the final output buffer (if necessary, we can calculate this
        // in-place as the final stage reads and writes at the same offsets)
        dst = out_final;

        for (f_count = 0; f_count < fstride; f_count++)
        {
            tw = twiddles; // Reset the twiddle pointer for the next section
            ne10_radix4x4_with_twiddles_neon (dst, src, tw, step, step, mstride);
            src += mstride;
            dst += mstride;
        } // f_count
    } // last stage
}

/*
 * This function calculates the inverse FFT, and is very similar in structure to its
 * complement "ne10_mixed_radix_fft_forward_float32_neon".
 */
void ne10_mixed_radix_fft_backward_float32_neon (ne10_fft_cpx_float32_t *out,
        ne10_fft_cpx_float32_t *in,
        ne10_int32_t *factors,
        ne10_fft_cpx_float32_t *twiddles,
        ne10_fft_cpx_float32_t *buffer)
{
    ne10_int32_t stage_count = factors[0];
    ne10_int32_t fstride = factors[1];
    ne10_int32_t mstride = factors[(stage_count << 1) - 1];
    ne10_int32_t first_radix = factors[stage_count << 1];
    ne10_float32_t nfft = fstride * first_radix;
    ne10_int32_t step, f_count;
    ne10_fft_cpx_float32_t *src = in;
    ne10_fft_cpx_float32_t *dst = out;
    ne10_fft_cpx_float32_t *out_final = out;
    ne10_fft_cpx_float32_t *tw, *tmp;

    // The first stage (using hardcoded twiddles)
    if (first_radix == 2) // nfft is of form 2^{odd}
    {
        // Perform a radix-8 butterfly (rather than the "suggested" radix-2)
        ne10_radix8x4_inverse_neon (dst, src, fstride / 4);

        // Update variables for the next stages
        step = fstride >> 1;
        mstride *= 4;
        fstride /= 16;
        stage_count -= 2;
        twiddles += 6;
    }
    else if (first_radix == 4) // nfft is of form 2^{even}
    {
        ne10_radix4x4_inverse_without_twiddles_neon (dst, src, fstride);

        // Update variables for the next stages
        step = fstride;
        stage_count--;
        fstride /= 4;
    }

    // The next stage should read the output of the first stage as input
    in = out;
    out = buffer;

    // Middle stages (after the first, excluding the last)
    for (; stage_count > 1; stage_count--)
    {
        src = in;
        for (f_count = 0; f_count < fstride; f_count++)
        {
            dst = &out[f_count * (mstride * 4)];
            tw = twiddles;
            ne10_radix4x4_inverse_with_twiddles_neon (dst, src, tw, step, mstride, mstride);
            src += mstride;
        } // f_count

        // Update variables for the next stages
        twiddles += mstride * 3;
        mstride *= 4;
        fstride /= 4;

        // Swap the input and output buffers for the next stage
        tmp = in;
        in = out;
        out = tmp;
    } // stage_count

    // The last stage
    if (stage_count)
    {
        src = in;

        // Always write to the final output buffer (if necessary, we can calculate this
        // in-place as the final stage reads and writes at the same offsets)
        dst = out_final;

        for (f_count = 0; f_count < fstride; f_count++)
        {
            tw = twiddles;
            ne10_radix4x4_inverse_with_twiddles_last_stage_neon (dst, src, tw, step, step, mstride, nfft);
            src += mstride;
            dst += mstride;
        } // f_count
    } // last stage
}

/**
 * @addtogroup C2C_FFT_IFFT
 * @{
 */

/**
 * @brief Mixed radix-2/3/4/5 complex NEON FFT/IFFT of single precision floating point data.
 * @param[out]  *fout            pointer to the output buffer
 * @param[in]   *fin             pointer to the input buffer
 * @param[in]   cfg              pointer to the configuration structure
 * @param[in]   inverse_fft      whether this is an FFT or IFFT (0: FFT, 1: IFFT)
 *
 * This function implements a mixed radix-2/3/4/5 complex FFT/IFFT, supporting input lengths of the
 * form 2^N*3^M*5^K (N, M, K > 0, length >= 4). This is an out-of-place algorithm. For usage information,
 * please check test/test_suite_fft_float32.c.
 */
void ne10_fft_c2c_1d_float32_neon (ne10_fft_cpx_float32_t *fout,
                                   ne10_fft_cpx_float32_t *fin,
                                   ne10_fft_cfg_float32_t cfg,
                                   ne10_int32_t inverse_fft)
{
    // For input shorter than 15, fall back to c version.
    // We would not get much improvement from NEON for these cases.
    if (cfg->nfft < 15)
    {
        ne10_fft_c2c_1d_float32_c (fout, fin, cfg, inverse_fft);
        return;
    }

    ne10_int32_t stage_count = cfg->factors[0];
    ne10_int32_t algorithm_flag = cfg->factors[2 * (stage_count + 1)];

    assert ((algorithm_flag == NE10_FFT_ALG_24)
            || (algorithm_flag == NE10_FFT_ALG_ANY));

    // For NE10_FFT_ALG_ANY.
    // Function will return inside this branch.
    if (algorithm_flag == NE10_FFT_ALG_ANY)
    {
        if (inverse_fft)
        {
            ne10_mixed_radix_generic_butterfly_inverse_float32_neon (fout, fin,
                    cfg->factors, cfg->twiddles, cfg->buffer, cfg->is_backward_scaled);
        }
        else
        {
            ne10_mixed_radix_generic_butterfly_float32_neon (fout, fin,
                    cfg->factors, cfg->twiddles, cfg->buffer, cfg->is_forward_scaled);
        }
        return;
    }

    // Since function goes pass assertion and skips branch above, algorithm_flag
    // must be NE10_FFT_ALG_24.
    if (inverse_fft)
    {
        switch (cfg->nfft)
        {
        case 4:
            ne10_fft4_backward_float32 (fout, fin);
            break;
        case 8:
            ne10_fft8_backward_float32 (fout, fin);
            break;
        case 16:
            ne10_fft16_backward_float32_neon (fout, fin, cfg->twiddles);
            break;
        default:
            ne10_mixed_radix_fft_backward_float32_neon (fout, fin, cfg->factors, cfg->twiddles, cfg->buffer);
            break;
        }
    }
    else
    {
        switch (cfg->nfft)
        {
        case 4:
            ne10_fft4_forward_float32 (fout, fin);
            break;
        case 8:
            ne10_fft8_forward_float32 (fout, fin);
            break;
        case 16:
            ne10_fft16_forward_float32_neon (fout, fin, cfg->twiddles);
            break;
        default:
            ne10_mixed_radix_fft_forward_float32_neon (fout, fin, cfg->factors, cfg->twiddles, cfg->buffer);
            break;
        }
    }
}

/**
 * @}
 */ //end of C2C_FFT_IFFT group
