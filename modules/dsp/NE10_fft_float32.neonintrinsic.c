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
 * NE10 Library : dsp/NE10_fft_float32.neon.c
 */

#include <arm_neon.h>

#include "NE10_types.h"
#include "NE10_macros.h"
#include "NE10_fft.h"
#include "NE10_dsp.h"

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
    Fout[2].r = (tmp_r - s0_r) * 0.25f;
    Fout[2].i = (tmp_i - s0_i) * 0.25f;
    Fout[0].r = (tmp_r + s0_r) * 0.25f;
    Fout[0].i = (tmp_i + s0_i) * 0.25f;

    Fout[1].r = (s2_r - s1_i) * 0.25f;
    Fout[1].i = (s2_i + s1_r) * 0.25f;
    Fout[3].r = (s2_r + s1_i) * 0.25f;
    Fout[3].i = (s2_i - s1_r) * 0.25f;
}


static inline void ne10_fft8_forward_float32 (ne10_fft_cpx_float32_t * Fout,
        ne10_fft_cpx_float32_t * Fin)
{
    ne10_float32_t s0_r, s0_i, s1_r, s1_i, s2_r, s2_i, s3_r, s3_i, s4_r, s4_i, s5_r, s5_i, s6_r, s6_i, s7_r, s7_i;
    ne10_float32_t t0_r, t0_i, t1_r, t1_i, t2_r, t2_i, t3_r, t3_i, t4_r, t4_i, t5_r, t5_i;
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
    Fout[0].r = (t1_r + t2_r) * 0.125f;
    Fout[0].i = (t1_i + t2_i) * 0.125f;
    Fout[4].r = (t1_r - t2_r) * 0.125f;
    Fout[4].i = (t1_i - t2_i) * 0.125f;
    Fout[2].r = (t0_r - t3_i) * 0.125f;
    Fout[2].i = (t0_i + t3_r) * 0.125f;
    Fout[6].r = (t0_r + t3_i) * 0.125f;
    Fout[6].i = (t0_i - t3_r) * 0.125f;

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
    Fout[1].r = (t1_r + t2_r) * 0.125f;
    Fout[1].i = (t1_i + t2_i) * 0.125f;
    Fout[5].r = (t1_r - t2_r) * 0.125f;
    Fout[5].i = (t1_i - t2_i) * 0.125f;
    Fout[3].r = (t0_r - t3_i) * 0.125f;
    Fout[3].i = (t0_i + t3_r) * 0.125f;
    Fout[7].r = (t0_r + t3_i) * 0.125f;
    Fout[7].i = (t0_i - t3_r) * 0.125f;
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

static inline void ne10_radix8x4_neon (ne10_fft_cpx_float32_t * Fout,
                                       ne10_fft_cpx_float32_t * Fin,
                                       ne10_int32_t stride)
{
    ne10_int32_t f_count;
    ne10_int32_t src_step = stride << 1;

    const ne10_float32_t TW_81 = 0.70710678;
    const ne10_float32_t TW_81N = -0.70710678;

    float32_t *p_src, *p_dst;
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

    p_src = (float32_t *) Fin;
    p_dst = (float32_t *) Fout;

    for (f_count = 0; f_count < stride; f_count += 4)
    {
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

        // radix 4 butterfly without twiddles
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

        // radix 2 butterfly
        q_s8_r = vaddq_f32 (q_sin0_r, q_sin4_r);
        q_s8_i = vaddq_f32 (q_sin0_i, q_sin4_i);
        q_s9_r = vaddq_f32 (q_sin1_r, q_s5_r);
        q_s9_i = vaddq_f32 (q_sin1_i, q_s5_i);
        q_s10_r = vsubq_f32 (q_sin0_r, q_sin4_r);
        q_s10_i = vsubq_f32 (q_sin0_i, q_sin4_i);
        q_s11_r = vsubq_f32 (q_sin1_r, q_s5_r);
        q_s11_i = vsubq_f32 (q_sin1_i, q_s5_i);

        // radix 2 butterfly
        q_s12_r = vaddq_f32 (q_sin2_r, q_sin6_r);
        q_s12_i = vaddq_f32 (q_sin2_i, q_sin6_i);
        q_s13_r = vaddq_f32 (q_s3_r, q_s7_r);
        q_s13_i = vaddq_f32 (q_s3_i, q_s7_i);
        q_s14_r = vsubq_f32 (q_sin2_r, q_sin6_r);
        q_s14_i = vsubq_f32 (q_sin2_i, q_sin6_i);
        q_s15_r = vsubq_f32 (q_s3_r, q_s7_r);
        q_s15_i = vsubq_f32 (q_s3_i, q_s7_i);

        // third result
        q_out4_r = vsubq_f32 (q_s8_r, q_s12_r);
        q_out4_i = vsubq_f32 (q_s8_i, q_s12_i);
        q_out5_r = vsubq_f32 (q_s9_r, q_s13_r);
        q_out5_i = vsubq_f32 (q_s9_i, q_s13_i);

        // first result
        q_out0_r = vaddq_f32 (q_s8_r, q_s12_r);
        q_out0_i = vaddq_f32 (q_s8_i, q_s12_i);
        q_out1_r = vaddq_f32 (q_s9_r, q_s13_r);
        q_out1_i = vaddq_f32 (q_s9_i, q_s13_i);

        // second result
        q_out2_r = vaddq_f32 (q_s10_r, q_s14_i);
        q_out2_i = vsubq_f32 (q_s10_i, q_s14_r);
        q_out3_r = vaddq_f32 (q_s11_r, q_s15_i);
        q_out3_i = vsubq_f32 (q_s11_i, q_s15_r);

        // forth result
        q_out6_r = vsubq_f32 (q_s10_r, q_s14_i);
        q_out6_i = vaddq_f32 (q_s10_i, q_s14_r);
        q_out7_r = vsubq_f32 (q_s11_r, q_s15_i);
        q_out7_i = vaddq_f32 (q_s11_i, q_s15_r);

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

        // store
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

        p_src = p_src - src_step * 8 + 8;
    } // f_count
}

static inline void ne10_radix4x4_without_twiddles_neon (ne10_fft_cpx_float32_t * Fout,
        ne10_fft_cpx_float32_t * Fin,
        ne10_int32_t stride)
{
    ne10_int32_t f_count;
    ne10_int32_t src_step = stride << 1;

    float32_t *p_src, *p_dst;
    float32x4x2_t q2_in0, q2_in1, q2_in2, q2_in3;
    float32x4_t q_s0_r, q_s0_i, q_s1_r, q_s1_i, q_s2_r, q_s2_i, q_s3_r, q_s3_i;
    float32x4_t q_out0_r, q_out0_i, q_out1_r, q_out1_i, q_out2_r, q_out2_i, q_out3_r, q_out3_i;
    float32x4x2_t q2_tmp0, q2_tmp1, q2_tmp2, q2_tmp3;
    float32x4x2_t q2_out0, q2_out1, q2_out2, q2_out3;

    p_src = (float32_t *) Fin;
    p_dst = (float32_t *) Fout;

    for (f_count = 0; f_count < stride; f_count += 4)
    {
        // load
        q2_in0 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in1 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in2 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in3 = vld2q_f32 (p_src);
        p_src += src_step;

        // radix 4 butterfly without twiddles
        q_s0_r = vaddq_f32 (q2_in0.val[0], q2_in2.val[0]);
        q_s0_i = vaddq_f32 (q2_in0.val[1], q2_in2.val[1]);
        q_s1_r = vsubq_f32 (q2_in0.val[0], q2_in2.val[0]);
        q_s1_i = vsubq_f32 (q2_in0.val[1], q2_in2.val[1]);
        q_s2_r = vaddq_f32 (q2_in1.val[0], q2_in3.val[0]);
        q_s2_i = vaddq_f32 (q2_in1.val[1], q2_in3.val[1]);
        q_s3_r = vsubq_f32 (q2_in1.val[0], q2_in3.val[0]);
        q_s3_i = vsubq_f32 (q2_in1.val[1], q2_in3.val[1]);

        // third result
        q_out2_r = vsubq_f32 (q_s0_r, q_s2_r);
        q_out2_i = vsubq_f32 (q_s0_i, q_s2_i);
        q_out0_r = vaddq_f32 (q_s0_r, q_s2_r);
        q_out0_i = vaddq_f32 (q_s0_i, q_s2_i);

        q_out1_r = vaddq_f32 (q_s1_r, q_s3_i);
        q_out1_i = vsubq_f32 (q_s1_i, q_s3_r);
        q_out3_r = vsubq_f32 (q_s1_r, q_s3_i);
        q_out3_i = vaddq_f32 (q_s1_i, q_s3_r);

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

        // store
        vst2q_f32 (p_dst, q2_out0);
        p_dst += 8;
        vst2q_f32 (p_dst, q2_out1);
        p_dst += 8;
        vst2q_f32 (p_dst, q2_out2);
        p_dst += 8;
        vst2q_f32 (p_dst, q2_out3);
        p_dst += 8;

        p_src = p_src - src_step * 4 + 8;
    }
}

static inline void ne10_radix4x4_with_twiddles_neon (ne10_fft_cpx_float32_t * Fout,
        ne10_fft_cpx_float32_t * Fin,
        ne10_fft_cpx_float32_t * tw,
        ne10_int32_t src_stride,
        ne10_int32_t dst_stride,
        ne10_int32_t mstride)
{
    ne10_int32_t m_count;
    ne10_int32_t src_step = src_stride << 1;
    ne10_int32_t dst_step = dst_stride << 1;
    ne10_int32_t tw_step = mstride << 1;

    float32_t *p_src, *p_dst, *p_tw;
    float32x4x2_t q2_in0, q2_in1, q2_in2, q2_in3;
    float32x4x2_t q2_tw0, q2_tw1, q2_tw2;
    float32x4_t q_s1_r, q_s1_i, q_s2_r, q_s2_i, q_s3_r, q_s3_i;
    float32x4_t q_s4_r, q_s4_i, q_s5_r, q_s5_i, q_s6_r, q_s6_i, q_s7_r, q_s7_i;
    float32x4x2_t q2_out0, q2_out1, q2_out2, q2_out3;

    p_src = (float32_t *) Fin;
    p_dst = (float32_t *) Fout;
    p_tw = (float32_t *) tw;

    for (m_count = 0; m_count < mstride; m_count += 4)
    {
        // load
        q2_in0 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in1 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in2 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in3 = vld2q_f32 (p_src);
        p_src += src_step;

        q2_tw0 = vld2q_f32 (p_tw);
        p_tw += tw_step;
        q2_tw1 = vld2q_f32 (p_tw);
        p_tw += tw_step;
        q2_tw2 = vld2q_f32 (p_tw);

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

        q_s4_r = vaddq_f32 (q2_in0.val[0], q_s2_r);
        q_s4_i = vaddq_f32 (q2_in0.val[1], q_s2_i);
        q_s5_r = vsubq_f32 (q2_in0.val[0], q_s2_r);
        q_s5_i = vsubq_f32 (q2_in0.val[1], q_s2_i);

        q_s6_r = vaddq_f32 (q_s1_r, q_s3_r);
        q_s6_i = vaddq_f32 (q_s1_i, q_s3_i);
        q_s7_r = vsubq_f32 (q_s1_r, q_s3_r);
        q_s7_i = vsubq_f32 (q_s1_i, q_s3_i);

        q2_out2.val[0] = vsubq_f32 (q_s4_r, q_s6_r);
        q2_out2.val[1] = vsubq_f32 (q_s4_i, q_s6_i);
        q2_out0.val[0] = vaddq_f32 (q_s4_r, q_s6_r);
        q2_out0.val[1] = vaddq_f32 (q_s4_i, q_s6_i);

        q2_out1.val[0] = vaddq_f32 (q_s5_r, q_s7_i);
        q2_out1.val[1] = vsubq_f32 (q_s5_i, q_s7_r);
        q2_out3.val[0] = vsubq_f32 (q_s5_r, q_s7_i);
        q2_out3.val[1] = vaddq_f32 (q_s5_i, q_s7_r);

        // store
        vst2q_f32 (p_dst, q2_out0);
        p_dst += dst_step;
        vst2q_f32 (p_dst, q2_out1);
        p_dst += dst_step;
        vst2q_f32 (p_dst, q2_out2);
        p_dst += dst_step;
        vst2q_f32 (p_dst, q2_out3);
        p_dst += dst_step;

        p_src = p_src - src_step * 4 + 8;
        p_dst = p_dst - dst_step * 4 + 8;
        p_tw = p_tw - tw_step * 2 + 8;
    }
}
static inline void ne10_radix8x4_inverse_neon (ne10_fft_cpx_float32_t * Fout,
        ne10_fft_cpx_float32_t * Fin,
        ne10_int32_t stride)
{
    ne10_int32_t f_count;
    ne10_int32_t src_step = stride << 1;

    const ne10_float32_t TW_81 = 0.70710678;
    const ne10_float32_t TW_81N = -0.70710678;

    float32_t *p_src, *p_dst;
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

    p_src = (float32_t *) Fin;
    p_dst = (float32_t *) Fout;

    for (f_count = 0; f_count < stride; f_count += 4)
    {
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

        // radix 4 butterfly without twiddles
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

        // radix 2 butterfly
        q_s8_r = vaddq_f32 (q_sin0_r, q_sin4_r);
        q_s8_i = vaddq_f32 (q_sin0_i, q_sin4_i);
        q_s9_r = vaddq_f32 (q_sin1_r, q_s5_r);
        q_s9_i = vaddq_f32 (q_sin1_i, q_s5_i);
        q_s10_r = vsubq_f32 (q_sin0_r, q_sin4_r);
        q_s10_i = vsubq_f32 (q_sin0_i, q_sin4_i);
        q_s11_r = vsubq_f32 (q_sin1_r, q_s5_r);
        q_s11_i = vsubq_f32 (q_sin1_i, q_s5_i);

        // radix 2 butterfly
        q_s12_r = vaddq_f32 (q_sin2_r, q_sin6_r);
        q_s12_i = vaddq_f32 (q_sin2_i, q_sin6_i);
        q_s13_r = vaddq_f32 (q_s3_r, q_s7_r);
        q_s13_i = vaddq_f32 (q_s3_i, q_s7_i);
        q_s14_r = vsubq_f32 (q_sin2_r, q_sin6_r);
        q_s14_i = vsubq_f32 (q_sin2_i, q_sin6_i);
        q_s15_r = vsubq_f32 (q_s3_r, q_s7_r);
        q_s15_i = vsubq_f32 (q_s3_i, q_s7_i);

        // third result
        q_out4_r = vsubq_f32 (q_s8_r, q_s12_r);
        q_out4_i = vsubq_f32 (q_s8_i, q_s12_i);
        q_out5_r = vsubq_f32 (q_s9_r, q_s13_r);
        q_out5_i = vsubq_f32 (q_s9_i, q_s13_i);

        // first result
        q_out0_r = vaddq_f32 (q_s8_r, q_s12_r);
        q_out0_i = vaddq_f32 (q_s8_i, q_s12_i);
        q_out1_r = vaddq_f32 (q_s9_r, q_s13_r);
        q_out1_i = vaddq_f32 (q_s9_i, q_s13_i);

        // second result
        q_out2_r = vsubq_f32 (q_s10_r, q_s14_i);
        q_out2_i = vaddq_f32 (q_s10_i, q_s14_r);
        q_out3_r = vsubq_f32 (q_s11_r, q_s15_i);
        q_out3_i = vaddq_f32 (q_s11_i, q_s15_r);

        // forth result
        q_out6_r = vaddq_f32 (q_s10_r, q_s14_i);
        q_out6_i = vsubq_f32 (q_s10_i, q_s14_r);
        q_out7_r = vaddq_f32 (q_s11_r, q_s15_i);
        q_out7_i = vsubq_f32 (q_s11_i, q_s15_r);

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

        // store
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

        p_src = p_src - src_step * 8 + 8;
    } // f_count
}

static inline void ne10_radix4x4_inverse_without_twiddles_neon (ne10_fft_cpx_float32_t * Fout,
        ne10_fft_cpx_float32_t * Fin,
        ne10_int32_t stride)
{
    ne10_int32_t f_count;
    ne10_int32_t src_step = stride << 1;

    float32_t *p_src, *p_dst;
    float32x4x2_t q2_in0, q2_in1, q2_in2, q2_in3;
    float32x4_t q_s0_r, q_s0_i, q_s1_r, q_s1_i, q_s2_r, q_s2_i, q_s3_r, q_s3_i;
    float32x4_t q_out0_r, q_out0_i, q_out1_r, q_out1_i, q_out2_r, q_out2_i, q_out3_r, q_out3_i;
    float32x4x2_t q2_tmp0, q2_tmp1, q2_tmp2, q2_tmp3;
    float32x4x2_t q2_out0, q2_out1, q2_out2, q2_out3;

    p_src = (float32_t *) Fin;
    p_dst = (float32_t *) Fout;

    for (f_count = 0; f_count < stride; f_count += 4)
    {
        // load
        q2_in0 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in1 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in2 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in3 = vld2q_f32 (p_src);
        p_src += src_step;

        // radix 4 butterfly without twiddles
        q_s0_r = vaddq_f32 (q2_in0.val[0], q2_in2.val[0]);
        q_s0_i = vaddq_f32 (q2_in0.val[1], q2_in2.val[1]);
        q_s1_r = vsubq_f32 (q2_in0.val[0], q2_in2.val[0]);
        q_s1_i = vsubq_f32 (q2_in0.val[1], q2_in2.val[1]);
        q_s2_r = vaddq_f32 (q2_in1.val[0], q2_in3.val[0]);
        q_s2_i = vaddq_f32 (q2_in1.val[1], q2_in3.val[1]);
        q_s3_r = vsubq_f32 (q2_in1.val[0], q2_in3.val[0]);
        q_s3_i = vsubq_f32 (q2_in1.val[1], q2_in3.val[1]);

        q_out2_r = vsubq_f32 (q_s0_r, q_s2_r);
        q_out2_i = vsubq_f32 (q_s0_i, q_s2_i);
        q_out0_r = vaddq_f32 (q_s0_r, q_s2_r);
        q_out0_i = vaddq_f32 (q_s0_i, q_s2_i);

        q_out1_r = vsubq_f32 (q_s1_r, q_s3_i);
        q_out1_i = vaddq_f32 (q_s1_i, q_s3_r);
        q_out3_r = vaddq_f32 (q_s1_r, q_s3_i);
        q_out3_i = vsubq_f32 (q_s1_i, q_s3_r);

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

        // store
        vst2q_f32 (p_dst, q2_out0);
        p_dst += 8;
        vst2q_f32 (p_dst, q2_out1);
        p_dst += 8;
        vst2q_f32 (p_dst, q2_out2);
        p_dst += 8;
        vst2q_f32 (p_dst, q2_out3);
        p_dst += 8;

        p_src = p_src - src_step * 4 + 8;
    }
}

static inline void ne10_radix4x4_inverse_with_twiddles_neon (ne10_fft_cpx_float32_t * Fout,
        ne10_fft_cpx_float32_t * Fin,
        ne10_fft_cpx_float32_t * tw,
        ne10_int32_t src_stride,
        ne10_int32_t dst_stride,
        ne10_int32_t mstride)
{
    ne10_int32_t m_count;
    ne10_int32_t src_step = src_stride << 1;
    ne10_int32_t dst_step = dst_stride << 1;
    ne10_int32_t tw_step = mstride << 1;

    float32_t *p_src, *p_dst, *p_tw;
    float32x4x2_t q2_in0, q2_in1, q2_in2, q2_in3;
    float32x4x2_t q2_tw0, q2_tw1, q2_tw2;
    float32x4_t q_s1_r, q_s1_i, q_s2_r, q_s2_i, q_s3_r, q_s3_i;
    float32x4_t q_s4_r, q_s4_i, q_s5_r, q_s5_i, q_s6_r, q_s6_i, q_s7_r, q_s7_i;
    float32x4x2_t q2_out0, q2_out1, q2_out2, q2_out3;

    p_src = (float32_t *) Fin;
    p_dst = (float32_t *) Fout;
    p_tw = (float32_t *) tw;

    for (m_count = 0; m_count < mstride; m_count += 4)
    {
        // load
        q2_in0 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in1 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in2 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in3 = vld2q_f32 (p_src);
        p_src += src_step;

        q2_tw0 = vld2q_f32 (p_tw);
        p_tw += tw_step;
        q2_tw1 = vld2q_f32 (p_tw);
        p_tw += tw_step;
        q2_tw2 = vld2q_f32 (p_tw);

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

        q_s4_r = vaddq_f32 (q2_in0.val[0], q_s2_r);
        q_s4_i = vaddq_f32 (q2_in0.val[1], q_s2_i);
        q_s5_r = vsubq_f32 (q2_in0.val[0], q_s2_r);
        q_s5_i = vsubq_f32 (q2_in0.val[1], q_s2_i);

        q_s6_r = vaddq_f32 (q_s1_r, q_s3_r);
        q_s6_i = vaddq_f32 (q_s1_i, q_s3_i);
        q_s7_r = vsubq_f32 (q_s1_r, q_s3_r);
        q_s7_i = vsubq_f32 (q_s1_i, q_s3_i);

        q2_out2.val[0] = vsubq_f32 (q_s4_r, q_s6_r);
        q2_out2.val[1] = vsubq_f32 (q_s4_i, q_s6_i);
        q2_out0.val[0] = vaddq_f32 (q_s4_r, q_s6_r);
        q2_out0.val[1] = vaddq_f32 (q_s4_i, q_s6_i);

        q2_out1.val[0] = vsubq_f32 (q_s5_r, q_s7_i);
        q2_out1.val[1] = vaddq_f32 (q_s5_i, q_s7_r);
        q2_out3.val[0] = vaddq_f32 (q_s5_r, q_s7_i);
        q2_out3.val[1] = vsubq_f32 (q_s5_i, q_s7_r);

        // store
        vst2q_f32 (p_dst, q2_out0);
        p_dst += dst_step;
        vst2q_f32 (p_dst, q2_out1);
        p_dst += dst_step;
        vst2q_f32 (p_dst, q2_out2);
        p_dst += dst_step;
        vst2q_f32 (p_dst, q2_out3);
        p_dst += dst_step;

        p_src = p_src - src_step * 4 + 8;
        p_dst = p_dst - dst_step * 4 + 8;
        p_tw = p_tw - tw_step * 2 + 8;
    }
}

static inline void ne10_radix4x4_inverse_with_twiddles_last_stage_neon (ne10_fft_cpx_float32_t * Fout,
        ne10_fft_cpx_float32_t * Fin,
        ne10_fft_cpx_float32_t * tw,
        ne10_int32_t src_stride,
        ne10_int32_t dst_stride,
        ne10_int32_t mstride,
        ne10_int32_t nfft)
{
    ne10_int32_t m_count;
    ne10_int32_t src_step = src_stride << 1;
    ne10_int32_t dst_step = dst_stride << 1;
    ne10_int32_t tw_step = mstride << 1;
    ne10_float32_t one_by_nfft = (1.0f / (ne10_float32_t) nfft);

    float32_t *p_src, *p_dst, *p_tw;
    float32x4x2_t q2_in0, q2_in1, q2_in2, q2_in3;
    float32x4x2_t q2_tw0, q2_tw1, q2_tw2;
    float32x4_t q_s1_r, q_s1_i, q_s2_r, q_s2_i, q_s3_r, q_s3_i;
    float32x4_t q_s4_r, q_s4_i, q_s5_r, q_s5_i, q_s6_r, q_s6_i, q_s7_r, q_s7_i;
    float32x4x2_t q2_out0, q2_out1, q2_out2, q2_out3;
    float32x4_t q_one_by_nfft = vdupq_n_f32 (one_by_nfft);

    p_src = (float32_t *) Fin;
    p_dst = (float32_t *) Fout;
    p_tw = (float32_t *) tw;

    for (m_count = 0; m_count < mstride; m_count += 4)
    {
        // load
        q2_in0 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in1 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in2 = vld2q_f32 (p_src);
        p_src += src_step;
        q2_in3 = vld2q_f32 (p_src);
        p_src += src_step;

        q2_tw0 = vld2q_f32 (p_tw);
        p_tw += tw_step;
        q2_tw1 = vld2q_f32 (p_tw);
        p_tw += tw_step;
        q2_tw2 = vld2q_f32 (p_tw);

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

        q_s4_r = vaddq_f32 (q2_in0.val[0], q_s2_r);
        q_s4_i = vaddq_f32 (q2_in0.val[1], q_s2_i);
        q_s5_r = vsubq_f32 (q2_in0.val[0], q_s2_r);
        q_s5_i = vsubq_f32 (q2_in0.val[1], q_s2_i);

        q_s6_r = vaddq_f32 (q_s1_r, q_s3_r);
        q_s6_i = vaddq_f32 (q_s1_i, q_s3_i);
        q_s7_r = vsubq_f32 (q_s1_r, q_s3_r);
        q_s7_i = vsubq_f32 (q_s1_i, q_s3_i);

        q2_out2.val[0] = vsubq_f32 (q_s4_r, q_s6_r);
        q2_out2.val[1] = vsubq_f32 (q_s4_i, q_s6_i);
        q2_out0.val[0] = vaddq_f32 (q_s4_r, q_s6_r);
        q2_out0.val[1] = vaddq_f32 (q_s4_i, q_s6_i);

        q2_out1.val[0] = vsubq_f32 (q_s5_r, q_s7_i);
        q2_out1.val[1] = vaddq_f32 (q_s5_i, q_s7_r);
        q2_out3.val[0] = vaddq_f32 (q_s5_r, q_s7_i);
        q2_out3.val[1] = vsubq_f32 (q_s5_i, q_s7_r);

        q2_out0.val[0] = vmulq_f32 (q2_out0.val[0], q_one_by_nfft);
        q2_out0.val[1] = vmulq_f32 (q2_out0.val[1], q_one_by_nfft);
        q2_out1.val[0] = vmulq_f32 (q2_out1.val[0], q_one_by_nfft);
        q2_out1.val[1] = vmulq_f32 (q2_out1.val[1], q_one_by_nfft);
        q2_out2.val[0] = vmulq_f32 (q2_out2.val[0], q_one_by_nfft);
        q2_out2.val[1] = vmulq_f32 (q2_out2.val[1], q_one_by_nfft);
        q2_out3.val[0] = vmulq_f32 (q2_out3.val[0], q_one_by_nfft);
        q2_out3.val[1] = vmulq_f32 (q2_out3.val[1], q_one_by_nfft);

        // store
        vst2q_f32 (p_dst, q2_out0);
        p_dst += dst_step;
        vst2q_f32 (p_dst, q2_out1);
        p_dst += dst_step;
        vst2q_f32 (p_dst, q2_out2);
        p_dst += dst_step;
        vst2q_f32 (p_dst, q2_out3);
        p_dst += dst_step;

        p_src = p_src - src_step * 4 + 8;
        p_dst = p_dst - dst_step * 4 + 8;
        p_tw = p_tw - tw_step * 2 + 8;
    }
}

void ne10_mixed_radix_fft_forward_float32_neon (ne10_fft_cpx_float32_t * Fout,
        ne10_fft_cpx_float32_t * Fin,
        ne10_int32_t * factors,
        ne10_fft_cpx_float32_t * twiddles,
        ne10_fft_cpx_float32_t * buffer)
{
    ne10_int32_t fstride, mstride, N;
    ne10_int32_t fstride1;
    ne10_int32_t f_count;
    ne10_int32_t stage_count;

    ne10_fft_cpx_float32_t   *Fin1, *Fout1;
    ne10_fft_cpx_float32_t   *Fout_ls = Fout;
    ne10_fft_cpx_float32_t   *Ftmp;
    ne10_fft_cpx_float32_t   *tw, *tw1;

    // init fstride, mstride, N
    stage_count = factors[0];
    fstride = factors[1];
    mstride = factors[ (stage_count << 1) - 1 ];
    N = factors[ stage_count << 1 ]; // radix

    // the first stage
    Fin1 = Fin;
    Fout1 = Fout;
    if (N == 2)   // length of FFT is 2^n (n is odd)
    {
        // radix 8
        N = fstride >> 1; // 1/4 of length of FFT
        tw = twiddles;
        fstride1 = fstride >> 2;

        ne10_radix8x4_neon (Fout, Fin, fstride1);

        tw += 6;
        mstride <<= 2;
        fstride >>= 4;
        stage_count -= 2;

        // swap
        Ftmp = buffer;
        buffer = Fout;
        Fout = Ftmp;
    }
    else if (N == 4)   // length of FFT is 2^n (n is even)
    {
        //fstride is nfft>>2
        ne10_radix4x4_without_twiddles_neon (Fout, Fin, fstride);

        N = fstride; // 1/4 of length of FFT

        // swap
        Ftmp = buffer;
        buffer = Fout;
        Fout = Ftmp;

        // update address for other stages
        stage_count--;
        tw = twiddles;
        fstride >>= 2;
        // end of first stage
    }


    // others but the last one
    for (; stage_count > 1 ; stage_count--)
    {
        Fin1 = buffer;
        for (f_count = 0; f_count < fstride; f_count ++)
        {
            Fout1 = & Fout[ f_count * mstride << 2 ];
            tw1 = tw;
            ne10_radix4x4_with_twiddles_neon (Fout1, Fin1, tw1, N, mstride, mstride);
            Fin1 += mstride;
        } // f_count
        tw += mstride * 3;
        mstride <<= 2;
        fstride >>= 2;

        // swap
        Ftmp = buffer;
        buffer = Fout;
        Fout = Ftmp;
    } // stage_count

    // the last one
    if (stage_count)
    {
        Fin1 = buffer;
        // if stage count is even, output to the input array
        Fout1 = Fout_ls;

        for (f_count = 0; f_count < fstride; f_count ++)
        {
            tw1 = tw;
            ne10_radix4x4_with_twiddles_neon (Fout1, Fin1, tw1, N, N, mstride);
            Fin1 += mstride;
            Fout1 += mstride;
        } // f_count
    } // last stage
}

void ne10_mixed_radix_fft_backward_float32_neon (ne10_fft_cpx_float32_t * Fout,
        ne10_fft_cpx_float32_t   * Fin,
        ne10_int32_t * factors,
        ne10_fft_cpx_float32_t * twiddles,
        ne10_fft_cpx_float32_t * buffer)
{
    ne10_int32_t fstride, mstride, N;
    ne10_int32_t fstride1;
    ne10_int32_t f_count;
    ne10_int32_t stage_count;
    ne10_int32_t nfft;

    ne10_fft_cpx_float32_t   *Fin1, *Fout1;
    ne10_fft_cpx_float32_t   *Fout_ls = Fout;
    ne10_fft_cpx_float32_t   *Ftmp;
    ne10_fft_cpx_float32_t   *tw, *tw1;

    // init fstride, mstride, N
    stage_count = factors[0];
    fstride = factors[1];
    mstride = factors[ (stage_count << 1) - 1 ];
    N = factors[ stage_count << 1 ]; // radix
    nfft = fstride * N;

    // the first stage
    Fin1 = Fin;
    Fout1 = Fout;
    if (N == 2)   // length of FFT is 2^n (n is odd)
    {
        // radix 8
        N = fstride >> 1; // 1/4 of length of FFT
        tw = twiddles;
        fstride1 = fstride >> 2;

        ne10_radix8x4_inverse_neon (Fout, Fin, fstride1);

        tw += 6;
        mstride <<= 2;
        fstride >>= 4;
        stage_count -= 2;

        // swap
        Ftmp = buffer;
        buffer = Fout;
        Fout = Ftmp;
    }
    else if (N == 4)   // length of FFT is 2^n (n is even)
    {
        //fstride is nfft>>2
        ne10_radix4x4_inverse_without_twiddles_neon (Fout, Fin, fstride);

        N = fstride; // 1/4 of length of FFT

        // swap
        Ftmp = buffer;
        buffer = Fout;
        Fout = Ftmp;

        // update address for other stages
        stage_count--;
        tw = twiddles;
        fstride >>= 2;
        // end of first stage
    }

    // others but the last one
    for (; stage_count > 1 ; stage_count--)
    {
        Fin1 = buffer;
        for (f_count = 0; f_count < fstride; f_count ++)
        {
            Fout1 = & Fout[ f_count * mstride << 2 ];
            tw1 = tw;
            ne10_radix4x4_inverse_with_twiddles_neon (Fout1, Fin1, tw1, N, mstride, mstride);
            Fin1 += mstride;
        } // f_count
        tw += mstride * 3;
        mstride <<= 2;
        fstride >>= 2;

        // swap
        Ftmp = buffer;
        buffer = Fout;
        Fout = Ftmp;
    } // stage_count

    // the last one
    if (stage_count)
    {
        Fin1 = buffer;
        // if stage count is even, output to the input array
        Fout1 = Fout_ls;

        for (f_count = 0; f_count < fstride; f_count ++)
        {
            tw1 = tw;
            ne10_radix4x4_inverse_with_twiddles_last_stage_neon (Fout1, Fin1, tw1, N, N, mstride, nfft);
            Fin1 += mstride;
            Fout1 += mstride;
        } // f_count
    } // last stage
}

/**
 * @addtogroup C2C_FFT_IFFT
 * @{
 */

/**
 * @brief Mixed radix-2/3/4/5 complex FFT/IFFT of float(32-bit) data.
 * @param[out]  *fout            point to the output buffer (out-of-place)
 * @param[in]   *fin             point to the input buffer (out-of-place)
 * @param[in]   cfg              point to the config struct
 * @param[in]   inverse_fft      the flag of IFFT, 0: FFT, 1: IFFT
 * @return none.
 * The function implements a mixed radix-2/3/4/5 complex FFT/IFFT. The length of 2^N*3^M*5^K(N,M,K are 1, 2, 3, 4, 5, 6 ....etc, and length >= 4) is supported.
 * Otherwise, this FFT is an out-of-place algorithm. When you want to get an in-place FFT, it creates a temp buffer as
 *  output buffer and then copies the temp buffer back to input buffer. For the usage of this function, please check test/test_suite_fft_float32.c
 */
void ne10_fft_c2c_1d_float32_neon (ne10_fft_cpx_float32_t *fout,
                                   ne10_fft_cpx_float32_t *fin,
                                   ne10_fft_cfg_float32_t cfg,
                                   ne10_int32_t inverse_fft)
{
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
                    cfg->factors, cfg->twiddles, cfg->buffer);
        }
        else
        {
            ne10_mixed_radix_generic_butterfly_float32_neon (fout, fin,
                    cfg->factors, cfg->twiddles, cfg->buffer);
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
