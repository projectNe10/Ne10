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
 * NE10 Library : dsp/NE10_fft_int32.neon.c
 */

#include <arm_neon.h>

#include "NE10_types.h"
#include "NE10_macros.h"
#include "NE10_fft.h"


static inline void ne10_fft4_forward_int32_scaled (ne10_fft_cpx_int32_t * Fout,
        ne10_fft_cpx_int32_t * Fin)

{
    ne10_int32_t s0_r, s0_i, s1_r, s1_i, s2_r, s2_i;
    ne10_int32_t tmp_r, tmp_i;

    s2_r = (Fin[0].r - Fin[2].r) >> 2;
    s2_i = (Fin[0].i - Fin[2].i) >> 2;
    tmp_r = (Fin[0].r + Fin[2].r) >> 2;
    tmp_i = (Fin[0].i + Fin[2].i) >> 2;


    s0_r = (Fin[1].r + Fin[3].r) >> 2;
    s0_i = (Fin[1].i + Fin[3].i) >> 2;
    s1_r = (Fin[1].r - Fin[3].r) >> 2;
    s1_i = (Fin[1].i - Fin[3].i) >> 2;

    Fout[2].r = tmp_r - s0_r;
    Fout[2].i = tmp_i - s0_i;
    Fout[0].r = tmp_r + s0_r;
    Fout[0].i = tmp_i + s0_i;

    Fout[1].r = s2_r + s1_i;
    Fout[1].i = s2_i - s1_r;
    Fout[3].r = s2_r - s1_i;
    Fout[3].i = s2_i + s1_r;
}

static inline void ne10_fft4_backward_int32_scaled (ne10_fft_cpx_int32_t * Fout,
        ne10_fft_cpx_int32_t * Fin)

{
    ne10_int32_t s0_r, s0_i, s1_r, s1_i, s2_r, s2_i;
    ne10_int32_t tmp_r, tmp_i;

    s2_r = (Fin[0].r - Fin[2].r) >> 2;
    s2_i = (Fin[0].i - Fin[2].i) >> 2;
    tmp_r = (Fin[0].r + Fin[2].r) >> 2;
    tmp_i = (Fin[0].i + Fin[2].i) >> 2;

    s0_r = (Fin[1].r + Fin[3].r) >> 2;
    s0_i = (Fin[1].i + Fin[3].i) >> 2;
    s1_r = (Fin[1].r - Fin[3].r) >> 2;
    s1_i = (Fin[1].i - Fin[3].i) >> 2;

    Fout[2].r = tmp_r - s0_r;
    Fout[2].i = tmp_i - s0_i;
    Fout[0].r = tmp_r + s0_r;
    Fout[0].i = tmp_i + s0_i;

    Fout[1].r = s2_r - s1_i;
    Fout[1].i = s2_i + s1_r;
    Fout[3].r = s2_r + s1_i;
    Fout[3].i = s2_i - s1_r;
}

static inline void ne10_fft8_forward_int32_scaled (ne10_fft_cpx_int32_t * Fout,
        ne10_fft_cpx_int32_t * Fin)

{
    ne10_int32_t s0_r, s0_i, s1_r, s1_i, s2_r, s2_i, s3_r, s3_i, s4_r, s4_i, s5_r, s5_i, s6_r, s6_i, s7_r, s7_i;
    ne10_int32_t t0_r, t0_i, t1_r, t1_i, t2_r, t2_i, t3_r, t3_i, t4_r, t4_i, t5_r, t5_i;
    ne10_fft_cpx_int32_t * F;
    const ne10_int32_t TW_81 = 1518500249;

    s0_r = (Fin[0].r + Fin[4].r) >> 3;
    s0_i = (Fin[0].i + Fin[4].i) >> 3;
    s1_r = (Fin[0].r - Fin[4].r) >> 3;
    s1_i = (Fin[0].i - Fin[4].i) >> 3;
    s2_r = (Fin[1].r + Fin[5].r) >> 3;
    s2_i = (Fin[1].i + Fin[5].i) >> 3;
    s3_r = (Fin[1].r - Fin[5].r) >> 3;
    s3_i = (Fin[1].i - Fin[5].i) >> 3;
    s4_r = (Fin[2].r + Fin[6].r) >> 3;
    s4_i = (Fin[2].i + Fin[6].i) >> 3;
    s5_r = (Fin[2].r - Fin[6].r) >> 3;
    s5_i = (Fin[2].i - Fin[6].i) >> 3;
    s6_r = (Fin[3].r + Fin[7].r) >> 3;
    s6_i = (Fin[3].i + Fin[7].i) >> 3;
    s7_r = (Fin[3].r - Fin[7].r) >> 3;
    s7_i = (Fin[3].i - Fin[7].i) >> 3;

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

    t4_r = (ne10_int32_t) ( ( (ne10_int64_t) (s3_r + s3_i) * TW_81) >> 31);
    t4_i = - (ne10_int32_t) ( ( (ne10_int64_t) (s3_r - s3_i) * TW_81) >> 31);
    t5_r = (ne10_int32_t) ( ( (ne10_int64_t) (s7_r - s7_i) * TW_81) >> 31);
    t5_i = (ne10_int32_t) ( ( (ne10_int64_t) (s7_r + s7_i) * TW_81) >> 31);

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

static inline void ne10_fft8_backward_int32_scaled (ne10_fft_cpx_int32_t * Fout,
        ne10_fft_cpx_int32_t * Fin)

{
    ne10_int32_t i;
    ne10_int32_t s0_r, s0_i, s1_r, s1_i, s2_r, s2_i, s3_r, s3_i, s4_r, s4_i, s5_r, s5_i, s6_r, s6_i, s7_r, s7_i;
    ne10_int32_t t0_r, t0_i, t1_r, t1_i, t2_r, t2_i, t3_r, t3_i, t4_r, t4_i, t5_r, t5_i;
    ne10_fft_cpx_int32_t * F;
    const ne10_int32_t TW_81 = 1518500249;

    s0_r = (Fin[0].r + Fin[4].r) >> 3;
    s0_i = (Fin[0].i + Fin[4].i) >> 3;
    s1_r = (Fin[0].r - Fin[4].r) >> 3;
    s1_i = (Fin[0].i - Fin[4].i) >> 3;
    s2_r = (Fin[1].r + Fin[5].r) >> 3;
    s2_i = (Fin[1].i + Fin[5].i) >> 3;
    s3_r = (Fin[1].r - Fin[5].r) >> 3;
    s3_i = (Fin[1].i - Fin[5].i) >> 3;
    s4_r = (Fin[2].r + Fin[6].r) >> 3;
    s4_i = (Fin[2].i + Fin[6].i) >> 3;
    s5_r = (Fin[2].r - Fin[6].r) >> 3;
    s5_i = (Fin[2].i - Fin[6].i) >> 3;
    s6_r = (Fin[3].r + Fin[7].r) >> 3;
    s6_i = (Fin[3].i + Fin[7].i) >> 3;
    s7_r = (Fin[3].r - Fin[7].r) >> 3;
    s7_i = (Fin[3].i - Fin[7].i) >> 3;

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

    t4_r = (ne10_int32_t) ( ( (ne10_int64_t) (s3_r - s3_i) * TW_81) >> 31);
    t4_i = (ne10_int32_t) ( ( (ne10_int64_t) (s3_r + s3_i) * TW_81) >> 31);
    t5_r = (ne10_int32_t) ( ( (ne10_int64_t) (s7_r + s7_i) * TW_81) >> 31);
    t5_i = - (ne10_int32_t) ( ( (ne10_int64_t) (s7_r - s7_i) * TW_81) >> 31);

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



static void ne10_butterfly_length_odd_power2_int32_neon(ne10_fft_cpx_int32_t * Fout,
        ne10_int32_t * factors,
        ne10_fft_cpx_int32_t * twiddles)
{
    ne10_int32_t i, j, mstride;
    ne10_int32_t stage_count;
    ne10_int32_t fstride;

    ne10_fft_cpx_int32_t *tw, *tw1, *tw2, *tw3;
    ne10_fft_cpx_int32_t *F;


    stage_count = factors[0];
    fstride = factors[1];

    // the first stage
    //fstride is nfft>>1
    int32x4x4_t q4_in01, q4_out01;
    int32_t *p_src, *p_dst;
    for (i = 0; i < fstride; i += 4)
    {
        p_dst = p_src = (int32_t*)(&(Fout[i*2]));
        q4_in01 = vld4q_s32 (p_src);
        q4_in01.val[0] = vshrq_n_s32 (q4_in01.val[0], 1);
        q4_in01.val[1] = vshrq_n_s32 (q4_in01.val[1], 1);
        q4_in01.val[2] = vshrq_n_s32 (q4_in01.val[2], 1);
        q4_in01.val[3] = vshrq_n_s32 (q4_in01.val[3], 1);
        q4_out01.val[2] = vsubq_s32 (q4_in01.val[0], q4_in01.val[2]);
        q4_out01.val[3] = vsubq_s32 (q4_in01.val[1], q4_in01.val[3]);
        q4_out01.val[0] = vaddq_s32 (q4_in01.val[0], q4_in01.val[2]);
        q4_out01.val[1] = vaddq_s32 (q4_in01.val[1], q4_in01.val[3]);
        vst4q_s32 (p_dst, q4_out01);
    }

    // the second stage
    stage_count--;
    mstride = 2;//factors[2 * stage_count + 1];
    tw = twiddles;
    fstride = fstride >> 2;
    int32_t *p_src0, *p_src1, *p_src2, *p_src3, *p_dst0, *p_dst1, *p_dst2, *p_dst3;
    int32_t *p_src0_1, *p_src1_1, *p_src2_1, *p_src3_1, *p_dst0_1, *p_dst1_1, *p_dst2_1, *p_dst3_1;
    int32_t *p_tw1, *p_tw2, *p_tw3;
    int32x4x2_t q2_in_0123, q2_in_4567, q2_in_89ab, q2_in_cdef;
    int32x4_t q_in_r0189, q_in_i0189, q_in_r23ab, q_in_i23ab, q_in_r45cd, q_in_i45cd, q_in_r67ef, q_in_i67ef;
    int32x2x2_t q_tw1_01, q_tw2_23, q_tw3_45;
    int32x4_t q_tw1_r, q_tw1_i, q_tw2_r, q_tw2_i, q_tw3_r, q_tw3_i;
    int32x4_t q_s0_r, q_s0_i, q_s1_r, q_s1_i, q_s2_r, q_s2_i;
    int32x4_t q_s3_r, q_s3_i, q_s4_r, q_s4_i, q_s5_r, q_s5_i;
    int32x4x2_t q2_in_0, q2_in_1, q2_in_2, q2_in_3;
    int32x4x2_t q2_tw1, q2_tw2, q2_tw3;
    int32x4_t q_tmp0, q_tmp1, q_tmp2, q_tmp3, q_tmp4, q_tmp5;

    for (i = 0; i < fstride; i += 2)
    {
        F = &Fout[i * mstride * 4];
        tw1 = tw;
        tw2 = tw + mstride;
        tw3 = tw + mstride * 2;
        p_dst0 = p_src0 = (int32_t*)(&F[0]);
        p_dst1 = p_src1 = (int32_t*)(&F[mstride]);
        p_dst2 = p_src2 = (int32_t*)(&F[mstride * 2]);
        p_dst3 = p_src3 = (int32_t*)(&F[mstride * 3]);
        F = &Fout[(i + 1) * mstride * 4];
        p_dst0_1 = p_src0_1 = (int32_t*)(&F[0]);
        p_dst1_1 = p_src1_1 = (int32_t*)(&F[mstride]);
        p_dst2_1 = p_src2_1 = (int32_t*)(&F[mstride * 2]);
        p_dst3_1 = p_src3_1 = (int32_t*)(&F[mstride * 3]);
        p_tw1 = (int32_t*)tw1;
        p_tw2 = (int32_t*)tw2;
        p_tw3 = (int32_t*)tw3;

        q2_in_0123 = vld2q_s32(p_src0);
        q2_in_4567 = vld2q_s32(p_src2);
        q2_in_89ab = vld2q_s32(p_src0_1);
        q2_in_cdef = vld2q_s32(p_src2_1);
        q_tw1_01 = vld2_s32(p_tw1);
        q_tw2_23 = vld2_s32(p_tw2);
        q_tw3_45 = vld2_s32(p_tw3);
        q2_in_0123.val[0] = vshrq_n_s32 (q2_in_0123.val[0], 2);
        q2_in_0123.val[1] = vshrq_n_s32 (q2_in_0123.val[1], 2);
        q2_in_4567.val[0] = vshrq_n_s32 (q2_in_4567.val[0], 2);
        q2_in_4567.val[1] = vshrq_n_s32 (q2_in_4567.val[1], 2);
        q2_in_89ab.val[0] = vshrq_n_s32 (q2_in_89ab.val[0], 2);
        q2_in_89ab.val[1] = vshrq_n_s32 (q2_in_89ab.val[1], 2);
        q2_in_cdef.val[0] = vshrq_n_s32 (q2_in_cdef.val[0], 2);
        q2_in_cdef.val[1] = vshrq_n_s32 (q2_in_cdef.val[1], 2);
        q_in_r0189 = vcombine_s32 (vget_low_s32 (q2_in_0123.val[0]), vget_low_s32 (q2_in_89ab.val[0]));
        q_in_i0189 = vcombine_s32 (vget_low_s32 (q2_in_0123.val[1]), vget_low_s32 (q2_in_89ab.val[1]));
        q_in_r23ab = vcombine_s32 (vget_high_s32 (q2_in_0123.val[0]), vget_high_s32 (q2_in_89ab.val[0]));
        q_in_i23ab = vcombine_s32 (vget_high_s32 (q2_in_0123.val[1]), vget_high_s32 (q2_in_89ab.val[1]));
        q_in_r45cd = vcombine_s32 (vget_low_s32 (q2_in_4567.val[0]), vget_low_s32 (q2_in_cdef.val[0]));
        q_in_i45cd = vcombine_s32 (vget_low_s32 (q2_in_4567.val[1]), vget_low_s32 (q2_in_cdef.val[1]));
        q_in_r67ef = vcombine_s32 (vget_high_s32 (q2_in_4567.val[0]), vget_high_s32 (q2_in_cdef.val[0]));
        q_in_i67ef = vcombine_s32 (vget_high_s32 (q2_in_4567.val[1]), vget_high_s32 (q2_in_cdef.val[1]));
        q_tw1_r = vcombine_s32 (q_tw1_01.val[0], q_tw1_01.val[0]);
        q_tw1_i = vcombine_s32 (q_tw1_01.val[1], q_tw1_01.val[1]);
        q_tw2_r = vcombine_s32 (q_tw2_23.val[0], q_tw2_23.val[0]);
        q_tw2_i = vcombine_s32 (q_tw2_23.val[1], q_tw2_23.val[1]);
        q_tw3_r = vcombine_s32 (q_tw3_45.val[0], q_tw3_45.val[0]);
        q_tw3_i = vcombine_s32 (q_tw3_45.val[1], q_tw3_45.val[1]);

        q_s0_r = vqrdmulhq_s32 (q_in_r23ab, q_tw1_r);
        q_s0_i = vqrdmulhq_s32 (q_in_r23ab, q_tw1_i);
        q_s1_r = vqrdmulhq_s32 (q_in_r45cd, q_tw2_r);
        q_s1_i = vqrdmulhq_s32 (q_in_r45cd, q_tw2_i);
        q_s2_r = vqrdmulhq_s32 (q_in_r67ef, q_tw3_r);
        q_s2_i = vqrdmulhq_s32 (q_in_r67ef, q_tw3_i);
        q_tmp0 = vqrdmulhq_s32 (q_in_i23ab, q_tw1_i);
        q_tmp1 = vqrdmulhq_s32 (q_in_i23ab, q_tw1_r);
        q_tmp2 = vqrdmulhq_s32 (q_in_i45cd, q_tw2_i);
        q_tmp3 = vqrdmulhq_s32 (q_in_i45cd, q_tw2_r);
        q_tmp4 = vqrdmulhq_s32 (q_in_i67ef, q_tw3_i);
        q_tmp5 = vqrdmulhq_s32 (q_in_i67ef, q_tw3_r);
        q_s0_r = vqsubq_s32 (q_s0_r, q_tmp0);
        q_s0_i = vqaddq_s32 (q_s0_i, q_tmp1);
        q_s1_r = vqsubq_s32 (q_s1_r, q_tmp2);
        q_s1_i = vqaddq_s32 (q_s1_i, q_tmp3);
        q_s2_r = vqsubq_s32 (q_s2_r, q_tmp4);
        q_s2_i = vqaddq_s32 (q_s2_i, q_tmp5);

        q_s5_r = vqsubq_s32 (q_in_r0189, q_s1_r);
        q_s5_i = vqsubq_s32 (q_in_i0189, q_s1_i);
        q_in_r0189 = vqaddq_s32 (q_in_r0189, q_s1_r);
        q_in_i0189 = vqaddq_s32 (q_in_i0189, q_s1_i);

        q_s3_r = vqaddq_s32 (q_s0_r, q_s2_r);
        q_s3_i = vqaddq_s32 (q_s0_i, q_s2_i);
        q_s4_r = vqsubq_s32 (q_s0_r, q_s2_r);
        q_s4_i = vqsubq_s32 (q_s0_i, q_s2_i);

        q_in_r45cd = vqsubq_s32 (q_in_r0189, q_s3_r);
        q_in_i45cd = vqsubq_s32 (q_in_i0189, q_s3_i);
        q_in_r0189 = vqaddq_s32 (q_in_r0189, q_s3_r);
        q_in_i0189 = vqaddq_s32 (q_in_i0189, q_s3_i);

        q_in_r23ab = vqaddq_s32 (q_s5_r, q_s4_i);
        q_in_i23ab = vqsubq_s32 (q_s5_i, q_s4_r);
        q_in_r67ef = vqsubq_s32 (q_s5_r, q_s4_i);
        q_in_i67ef = vqaddq_s32 (q_s5_i, q_s4_r);

        q2_in_0123.val[0] = vcombine_s32 (vget_low_s32 (q_in_r0189), vget_low_s32 (q_in_r23ab));
        q2_in_0123.val[1] = vcombine_s32 (vget_low_s32 (q_in_i0189), vget_low_s32 (q_in_i23ab));
        q2_in_4567.val[0] = vcombine_s32 (vget_low_s32 (q_in_r45cd), vget_low_s32 (q_in_r67ef));
        q2_in_4567.val[1] = vcombine_s32 (vget_low_s32 (q_in_i45cd), vget_low_s32 (q_in_i67ef));
        q2_in_89ab.val[0] = vcombine_s32 (vget_high_s32 (q_in_r0189), vget_high_s32 (q_in_r23ab));
        q2_in_89ab.val[1] = vcombine_s32 (vget_high_s32 (q_in_i0189), vget_high_s32 (q_in_i23ab));
        q2_in_cdef.val[0] = vcombine_s32 (vget_high_s32 (q_in_r45cd), vget_high_s32 (q_in_r67ef));
        q2_in_cdef.val[1] = vcombine_s32 (vget_high_s32 (q_in_i45cd), vget_high_s32 (q_in_i67ef));
        vst2q_s32(p_dst0, q2_in_0123);
        vst2q_s32(p_dst2, q2_in_4567);
        vst2q_s32(p_dst0_1, q2_in_89ab);
        vst2q_s32(p_dst2_1, q2_in_cdef);
    }
    tw += mstride * 3;
    mstride <<= 2;

    // other stages
    stage_count--;
    for (; stage_count > 0; stage_count--)
    {
        fstride = fstride >> 2;
        for (i = 0; i < fstride; i++)
        {
            F = &Fout[i * mstride * 4];
            tw1 = tw;
            tw2 = tw + mstride;
            tw3 = tw + mstride * 2;
            for (j = 0; j < mstride; j += 4)
            {
                p_dst0 = p_src0 = (int32_t*)(&F[0]);
                p_dst1 = p_src1 = (int32_t*)(&F[mstride]);
                p_dst2 = p_src2 = (int32_t*)(&F[mstride * 2]);
                p_dst3 = p_src3 = (int32_t*)(&F[mstride * 3]);
                p_tw1 = (int32_t*)tw1;
                p_tw2 = (int32_t*)tw2;
                p_tw3 = (int32_t*)tw3;
                q2_in_0 = vld2q_s32(p_src0);
                q2_in_1 = vld2q_s32(p_src1);
                q2_in_2 = vld2q_s32(p_src2);
                q2_in_3 = vld2q_s32(p_src3);
                q2_tw1 = vld2q_s32(p_tw1);
                q2_tw2 = vld2q_s32(p_tw2);
                q2_tw3 = vld2q_s32(p_tw3);
                q2_in_0.val[0] = vshrq_n_s32 (q2_in_0.val[0], 2);
                q2_in_0.val[1] = vshrq_n_s32 (q2_in_0.val[1], 2);
                q2_in_1.val[0] = vshrq_n_s32 (q2_in_1.val[0], 2);
                q2_in_1.val[1] = vshrq_n_s32 (q2_in_1.val[1], 2);
                q2_in_2.val[0] = vshrq_n_s32 (q2_in_2.val[0], 2);
                q2_in_2.val[1] = vshrq_n_s32 (q2_in_2.val[1], 2);
                q2_in_3.val[0] = vshrq_n_s32 (q2_in_3.val[0], 2);
                q2_in_3.val[1] = vshrq_n_s32 (q2_in_3.val[1], 2);

                q_s0_r = vqrdmulhq_s32 (q2_in_1.val[0], q2_tw1.val[0]);
                q_s0_i = vqrdmulhq_s32 (q2_in_1.val[0], q2_tw1.val[1]);
                q_s1_r = vqrdmulhq_s32 (q2_in_2.val[0], q2_tw2.val[0]);
                q_s1_i = vqrdmulhq_s32 (q2_in_2.val[0], q2_tw2.val[1]);
                q_s2_r = vqrdmulhq_s32 (q2_in_3.val[0], q2_tw3.val[0]);
                q_s2_i = vqrdmulhq_s32 (q2_in_3.val[0], q2_tw3.val[1]);
                q_tmp0 = vqrdmulhq_s32 (q2_in_1.val[1], q2_tw1.val[1]);
                q_tmp1 = vqrdmulhq_s32 (q2_in_1.val[1], q2_tw1.val[0]);
                q_tmp2 = vqrdmulhq_s32 (q2_in_2.val[1], q2_tw2.val[1]);
                q_tmp3 = vqrdmulhq_s32 (q2_in_2.val[1], q2_tw2.val[0]);
                q_tmp4 = vqrdmulhq_s32 (q2_in_3.val[1], q2_tw3.val[1]);
                q_tmp5 = vqrdmulhq_s32 (q2_in_3.val[1], q2_tw3.val[0]);
                q_s0_r = vqsubq_s32 (q_s0_r, q_tmp0);
                q_s0_i = vqaddq_s32 (q_s0_i, q_tmp1);
                q_s1_r = vqsubq_s32 (q_s1_r, q_tmp2);
                q_s1_i = vqaddq_s32 (q_s1_i, q_tmp3);
                q_s2_r = vqsubq_s32 (q_s2_r, q_tmp4);
                q_s2_i = vqaddq_s32 (q_s2_i, q_tmp5);

                q_s5_r = vqsubq_s32 (q2_in_0.val[0], q_s1_r);
                q_s5_i = vqsubq_s32 (q2_in_0.val[1], q_s1_i);
                q2_in_0.val[0] = vqaddq_s32 (q2_in_0.val[0], q_s1_r);
                q2_in_0.val[1] = vqaddq_s32 (q2_in_0.val[1], q_s1_i);

                q_s3_r = vqaddq_s32 (q_s0_r, q_s2_r);
                q_s3_i = vqaddq_s32 (q_s0_i, q_s2_i);
                q_s4_r = vqsubq_s32 (q_s0_r, q_s2_r);
                q_s4_i = vqsubq_s32 (q_s0_i, q_s2_i);

                q2_in_2.val[0] = vqsubq_s32 (q2_in_0.val[0], q_s3_r);
                q2_in_2.val[1] = vqsubq_s32 (q2_in_0.val[1], q_s3_i);
                q2_in_0.val[0] = vqaddq_s32 (q2_in_0.val[0], q_s3_r);
                q2_in_0.val[1] = vqaddq_s32 (q2_in_0.val[1], q_s3_i);

                q2_in_1.val[0] = vqaddq_s32 (q_s5_r, q_s4_i);
                q2_in_1.val[1] = vqsubq_s32 (q_s5_i, q_s4_r);
                q2_in_3.val[0] = vqsubq_s32 (q_s5_r, q_s4_i);
                q2_in_3.val[1] = vqaddq_s32 (q_s5_i, q_s4_r);

                vst2q_s32(p_dst0, q2_in_0);
                vst2q_s32(p_dst1, q2_in_1);
                vst2q_s32(p_dst2, q2_in_2);
                vst2q_s32(p_dst3, q2_in_3);

                tw1 += 4;
                tw2 += 4;
                tw3 += 4;
                F += 4;
            }
        }
        tw += mstride * 3;
        mstride <<= 2;
    }
}

static void ne10_butterfly_length_even_power2_int32_neon(ne10_fft_cpx_int32_t * Fout,
        ne10_int32_t * factors,
        ne10_fft_cpx_int32_t * twiddles)
{
    ne10_int32_t i, j, mstride;
    ne10_int32_t stage_count;
    ne10_int32_t fstride;

    ne10_fft_cpx_int32_t *tw, *tw1, *tw2, *tw3;
    ne10_fft_cpx_int32_t *F;


    stage_count = factors[0];
    fstride = factors[1];

    // the first stage
    //fstride is nfft>>2
    for (i = 0; i < fstride; i += 2)
    {
        int32_t *p_src0, *p_src1, *p_src2, *p_src3;
        int32x4_t q_in0, q_in1, q_in2, q_in3;
        int32x4_t q_in_04, q_in_15, q_in_26, q_in_37;
        int32x4_t q_s0, q_s1, q_s2, q_s3, q_s4, q_s5;
        int32x4_t q_tmp0, q_tmp1, q_tmp2, q_tmp3;
        int32x4x2_t q2_tmp0, q2_tmp1;
        p_src0 = (int32_t*)(&(Fout[i*4]));
        p_src1 = (int32_t*)(&(Fout[i*4 + 2]));
        p_src2 = (int32_t*)(&(Fout[i*4 + 4]));
        p_src3 = (int32_t*)(&(Fout[i*4 + 6]));
        q_in0 = vld1q_s32 (p_src0);
        q_in1 = vld1q_s32 (p_src1);
        q_in2 = vld1q_s32 (p_src2);
        q_in3 = vld1q_s32 (p_src3);
        q_in0 = vshrq_n_s32 (q_in0, 2);
        q_in1 = vshrq_n_s32 (q_in1, 2);
        q_in2 = vshrq_n_s32 (q_in2, 2);
        q_in3 = vshrq_n_s32 (q_in3, 2);

        q_in_04 = vcombine_s32 (vget_low_s32 (q_in0), vget_low_s32 (q_in2));
        q_in_15 = vcombine_s32 (vget_high_s32 (q_in0), vget_high_s32 (q_in2));
        q_in_26 = vcombine_s32 (vget_low_s32 (q_in1), vget_low_s32 (q_in3));
        q_in_37 = vcombine_s32 (vget_high_s32 (q_in1), vget_high_s32 (q_in3));

        q_s2 = vqsubq_s32 (q_in_04, q_in_26);
        q_in_04 = vqaddq_s32 (q_in_04, q_in_26);
        q_s0 = vqaddq_s32 (q_in_15, q_in_37);
        q_s1 = vqsubq_s32 (q_in_15, q_in_37);
        q_in_26 = vqsubq_s32 (q_in_04, q_s0);
        q_in_04 = vqaddq_s32 (q_in_04, q_s0);
        q_s1 = vrev64q_s32 (q_s1);
        q_tmp0 = vqaddq_s32 (q_s2, q_s1);
        q_tmp1 = vqsubq_s32 (q_s2, q_s1);
        q_tmp3 = vrev64q_s32 (q_tmp1);
        q2_tmp1 = vtrnq_s32 (q_tmp0, q_tmp3);
        q_in_15 = q2_tmp1.val[0];
        q_in_37 = vrev64q_s32 (q2_tmp1.val[1]);
        q_in0 = vcombine_s32 (vget_low_s32 (q_in_04), vget_low_s32 (q_in_15));
        q_in1 = vcombine_s32 (vget_low_s32 (q_in_26), vget_low_s32 (q_in_37));
        q_in2 = vcombine_s32 (vget_high_s32 (q_in_04), vget_high_s32 (q_in_15));
        q_in3 = vcombine_s32 (vget_high_s32 (q_in_26), vget_high_s32 (q_in_37));

        vst1q_s32 (p_src0, q_in0);
        vst1q_s32 (p_src1, q_in1);
        vst1q_s32 (p_src2, q_in2);
        vst1q_s32 (p_src3, q_in3);
    }
    stage_count--;

    // other stages
    int32_t *p_src0, *p_src1, *p_src2, *p_src3, *p_dst0, *p_dst1, *p_dst2, *p_dst3;
    int32_t *p_tw1, *p_tw2, *p_tw3;
    int32x4_t q_s0_r, q_s0_i, q_s1_r, q_s1_i, q_s2_r, q_s2_i;
    int32x4_t q_s3_r, q_s3_i, q_s4_r, q_s4_i, q_s5_r, q_s5_i;
    int32x4x2_t q2_in_0, q2_in_1, q2_in_2, q2_in_3;
    int32x4x2_t q2_tw1, q2_tw2, q2_tw3;
    int32x4_t q_tmp0, q_tmp1, q_tmp2, q_tmp3, q_tmp4, q_tmp5;
    mstride = factors[2 * stage_count + 1];
    tw = twiddles;
    for (; stage_count > 0; stage_count--)
    {
        fstride = fstride >> 2;
        for (i = 0; i < fstride; i++)
        {
            F = &Fout[i * mstride * 4];
            tw1 = tw;
            tw2 = tw + mstride;
            tw3 = tw + mstride * 2;
            for (j = 0; j < mstride; j += 4)
            {
                p_dst0 = p_src0 = (int32_t*)(&F[0]);
                p_dst1 = p_src1 = (int32_t*)(&F[mstride]);
                p_dst2 = p_src2 = (int32_t*)(&F[mstride * 2]);
                p_dst3 = p_src3 = (int32_t*)(&F[mstride * 3]);
                p_tw1 = (int32_t*)tw1;
                p_tw2 = (int32_t*)tw2;
                p_tw3 = (int32_t*)tw3;
                q2_in_0 = vld2q_s32(p_src0);
                q2_in_1 = vld2q_s32(p_src1);
                q2_in_2 = vld2q_s32(p_src2);
                q2_in_3 = vld2q_s32(p_src3);
                q2_tw1 = vld2q_s32(p_tw1);
                q2_tw2 = vld2q_s32(p_tw2);
                q2_tw3 = vld2q_s32(p_tw3);
                q2_in_0.val[0] = vshrq_n_s32 (q2_in_0.val[0], 2);
                q2_in_0.val[1] = vshrq_n_s32 (q2_in_0.val[1], 2);
                q2_in_1.val[0] = vshrq_n_s32 (q2_in_1.val[0], 2);
                q2_in_1.val[1] = vshrq_n_s32 (q2_in_1.val[1], 2);
                q2_in_2.val[0] = vshrq_n_s32 (q2_in_2.val[0], 2);
                q2_in_2.val[1] = vshrq_n_s32 (q2_in_2.val[1], 2);
                q2_in_3.val[0] = vshrq_n_s32 (q2_in_3.val[0], 2);
                q2_in_3.val[1] = vshrq_n_s32 (q2_in_3.val[1], 2);

                q_s0_r = vqrdmulhq_s32 (q2_in_1.val[0], q2_tw1.val[0]);
                q_s0_i = vqrdmulhq_s32 (q2_in_1.val[0], q2_tw1.val[1]);
                q_s1_r = vqrdmulhq_s32 (q2_in_2.val[0], q2_tw2.val[0]);
                q_s1_i = vqrdmulhq_s32 (q2_in_2.val[0], q2_tw2.val[1]);
                q_s2_r = vqrdmulhq_s32 (q2_in_3.val[0], q2_tw3.val[0]);
                q_s2_i = vqrdmulhq_s32 (q2_in_3.val[0], q2_tw3.val[1]);
                q_tmp0 = vqrdmulhq_s32 (q2_in_1.val[1], q2_tw1.val[1]);
                q_tmp1 = vqrdmulhq_s32 (q2_in_1.val[1], q2_tw1.val[0]);
                q_tmp2 = vqrdmulhq_s32 (q2_in_2.val[1], q2_tw2.val[1]);
                q_tmp3 = vqrdmulhq_s32 (q2_in_2.val[1], q2_tw2.val[0]);
                q_tmp4 = vqrdmulhq_s32 (q2_in_3.val[1], q2_tw3.val[1]);
                q_tmp5 = vqrdmulhq_s32 (q2_in_3.val[1], q2_tw3.val[0]);
                q_s0_r = vqsubq_s32 (q_s0_r, q_tmp0);
                q_s0_i = vqaddq_s32 (q_s0_i, q_tmp1);
                q_s1_r = vqsubq_s32 (q_s1_r, q_tmp2);
                q_s1_i = vqaddq_s32 (q_s1_i, q_tmp3);
                q_s2_r = vqsubq_s32 (q_s2_r, q_tmp4);
                q_s2_i = vqaddq_s32 (q_s2_i, q_tmp5);

                q_s5_r = vqsubq_s32 (q2_in_0.val[0], q_s1_r);
                q_s5_i = vqsubq_s32 (q2_in_0.val[1], q_s1_i);
                q2_in_0.val[0] = vqaddq_s32 (q2_in_0.val[0], q_s1_r);
                q2_in_0.val[1] = vqaddq_s32 (q2_in_0.val[1], q_s1_i);

                q_s3_r = vqaddq_s32 (q_s0_r, q_s2_r);
                q_s3_i = vqaddq_s32 (q_s0_i, q_s2_i);
                q_s4_r = vqsubq_s32 (q_s0_r, q_s2_r);
                q_s4_i = vqsubq_s32 (q_s0_i, q_s2_i);

                q2_in_2.val[0] = vqsubq_s32 (q2_in_0.val[0], q_s3_r);
                q2_in_2.val[1] = vqsubq_s32 (q2_in_0.val[1], q_s3_i);
                q2_in_0.val[0] = vqaddq_s32 (q2_in_0.val[0], q_s3_r);
                q2_in_0.val[1] = vqaddq_s32 (q2_in_0.val[1], q_s3_i);

                q2_in_1.val[0] = vqaddq_s32 (q_s5_r, q_s4_i);
                q2_in_1.val[1] = vqsubq_s32 (q_s5_i, q_s4_r);
                q2_in_3.val[0] = vqsubq_s32 (q_s5_r, q_s4_i);
                q2_in_3.val[1] = vqaddq_s32 (q_s5_i, q_s4_r);

                vst2q_s32(p_dst0, q2_in_0);
                vst2q_s32(p_dst1, q2_in_1);
                vst2q_s32(p_dst2, q2_in_2);
                vst2q_s32(p_dst3, q2_in_3);

                tw1 += 4;
                tw2 += 4;
                tw3 += 4;
                F += 4;
            }
        }
        tw += mstride * 3;
        mstride <<= 2;
    }
}

static void ne10_mixed_radix_butterfly_forward_int32_scaled_neon (ne10_fft_cpx_int32_t * Fout,
        ne10_int32_t * factors,
        ne10_fft_cpx_int32_t * twiddles)
{
    ne10_int32_t stage_count;
    stage_count = factors[0];

    if (factors[2 * stage_count] == 2) // length of FFT is 2^n (n is odd)
    {
        //radix 2/4, FFT length is 2^n (n is odd)
        ne10_butterfly_length_odd_power2_int32_neon (Fout, factors, twiddles);
    }
    else if (factors[2 * stage_count] == 4) // length of FFT is 2^n (n is even)
    {
        //radix 4, FFT length is 2^n (n is even)
        ne10_butterfly_length_even_power2_int32_neon (Fout, factors, twiddles);
    }
}

static void ne10_butterfly_inverse_length_odd_power2_int32_neon(ne10_fft_cpx_int32_t * Fout,
        ne10_int32_t * factors,
        ne10_fft_cpx_int32_t * twiddles)
{
    ne10_int32_t i, j, mstride;
    ne10_int32_t stage_count;
    ne10_int32_t fstride;

    ne10_fft_cpx_int32_t *tw, *tw1, *tw2, *tw3;
    ne10_fft_cpx_int32_t *F;


    stage_count = factors[0];
    fstride = factors[1];

    // the first stage
    //fstride is nfft>>1
    int32x4x4_t q4_in01, q4_out01;
    int32_t *p_src, *p_dst;
    for (i = 0; i < fstride; i += 4)
    {
        p_dst = p_src = (int32_t*)(&(Fout[i*2]));
        q4_in01 = vld4q_s32 (p_src);
        q4_in01.val[0] = vshrq_n_s32 (q4_in01.val[0], 1);
        q4_in01.val[1] = vshrq_n_s32 (q4_in01.val[1], 1);
        q4_in01.val[2] = vshrq_n_s32 (q4_in01.val[2], 1);
        q4_in01.val[3] = vshrq_n_s32 (q4_in01.val[3], 1);
        q4_out01.val[2] = vsubq_s32 (q4_in01.val[0], q4_in01.val[2]);
        q4_out01.val[3] = vsubq_s32 (q4_in01.val[1], q4_in01.val[3]);
        q4_out01.val[0] = vaddq_s32 (q4_in01.val[0], q4_in01.val[2]);
        q4_out01.val[1] = vaddq_s32 (q4_in01.val[1], q4_in01.val[3]);
        vst4q_s32 (p_dst, q4_out01);
    }

    // the second stage
    stage_count--;
    mstride = 2;//factors[2 * stage_count + 1];
    tw = twiddles;
    fstride = fstride >> 2;
    int32_t *p_src0, *p_src1, *p_src2, *p_src3, *p_dst0, *p_dst1, *p_dst2, *p_dst3;
    int32_t *p_src0_1, *p_src1_1, *p_src2_1, *p_src3_1, *p_dst0_1, *p_dst1_1, *p_dst2_1, *p_dst3_1;
    int32_t *p_tw1, *p_tw2, *p_tw3;
    int32x4x2_t q2_in_0123, q2_in_4567, q2_in_89ab, q2_in_cdef;
    int32x4_t q_in_r0189, q_in_i0189, q_in_r23ab, q_in_i23ab, q_in_r45cd, q_in_i45cd, q_in_r67ef, q_in_i67ef;
    int32x2x2_t q_tw1_01, q_tw2_23, q_tw3_45;
    int32x4_t q_tw1_r, q_tw1_i, q_tw2_r, q_tw2_i, q_tw3_r, q_tw3_i;
    int32x4_t q_s0_r, q_s0_i, q_s1_r, q_s1_i, q_s2_r, q_s2_i;
    int32x4_t q_s3_r, q_s3_i, q_s4_r, q_s4_i, q_s5_r, q_s5_i;
    int32x4x2_t q2_in_0, q2_in_1, q2_in_2, q2_in_3;
    int32x4x2_t q2_tw1, q2_tw2, q2_tw3;
    int32x4_t q_tmp0, q_tmp1, q_tmp2, q_tmp3, q_tmp4, q_tmp5;

    for (i = 0; i < fstride; i += 2)
    {
        F = &Fout[i * mstride * 4];
        tw1 = tw;
        tw2 = tw + mstride;
        tw3 = tw + mstride * 2;
        p_dst0 = p_src0 = (int32_t*)(&F[0]);
        p_dst1 = p_src1 = (int32_t*)(&F[mstride]);
        p_dst2 = p_src2 = (int32_t*)(&F[mstride * 2]);
        p_dst3 = p_src3 = (int32_t*)(&F[mstride * 3]);
        F = &Fout[(i + 1) * mstride * 4];
        p_dst0_1 = p_src0_1 = (int32_t*)(&F[0]);
        p_dst1_1 = p_src1_1 = (int32_t*)(&F[mstride]);
        p_dst2_1 = p_src2_1 = (int32_t*)(&F[mstride * 2]);
        p_dst3_1 = p_src3_1 = (int32_t*)(&F[mstride * 3]);
        p_tw1 = (int32_t*)tw1;
        p_tw2 = (int32_t*)tw2;
        p_tw3 = (int32_t*)tw3;

        q2_in_0123 = vld2q_s32(p_src0);
        q2_in_4567 = vld2q_s32(p_src2);
        q2_in_89ab = vld2q_s32(p_src0_1);
        q2_in_cdef = vld2q_s32(p_src2_1);
        q_tw1_01 = vld2_s32(p_tw1);
        q_tw2_23 = vld2_s32(p_tw2);
        q_tw3_45 = vld2_s32(p_tw3);
        q2_in_0123.val[0] = vshrq_n_s32 (q2_in_0123.val[0], 2);
        q2_in_0123.val[1] = vshrq_n_s32 (q2_in_0123.val[1], 2);
        q2_in_4567.val[0] = vshrq_n_s32 (q2_in_4567.val[0], 2);
        q2_in_4567.val[1] = vshrq_n_s32 (q2_in_4567.val[1], 2);
        q2_in_89ab.val[0] = vshrq_n_s32 (q2_in_89ab.val[0], 2);
        q2_in_89ab.val[1] = vshrq_n_s32 (q2_in_89ab.val[1], 2);
        q2_in_cdef.val[0] = vshrq_n_s32 (q2_in_cdef.val[0], 2);
        q2_in_cdef.val[1] = vshrq_n_s32 (q2_in_cdef.val[1], 2);
        q_in_r0189 = vcombine_s32 (vget_low_s32 (q2_in_0123.val[0]), vget_low_s32 (q2_in_89ab.val[0]));
        q_in_i0189 = vcombine_s32 (vget_low_s32 (q2_in_0123.val[1]), vget_low_s32 (q2_in_89ab.val[1]));
        q_in_r23ab = vcombine_s32 (vget_high_s32 (q2_in_0123.val[0]), vget_high_s32 (q2_in_89ab.val[0]));
        q_in_i23ab = vcombine_s32 (vget_high_s32 (q2_in_0123.val[1]), vget_high_s32 (q2_in_89ab.val[1]));
        q_in_r45cd = vcombine_s32 (vget_low_s32 (q2_in_4567.val[0]), vget_low_s32 (q2_in_cdef.val[0]));
        q_in_i45cd = vcombine_s32 (vget_low_s32 (q2_in_4567.val[1]), vget_low_s32 (q2_in_cdef.val[1]));
        q_in_r67ef = vcombine_s32 (vget_high_s32 (q2_in_4567.val[0]), vget_high_s32 (q2_in_cdef.val[0]));
        q_in_i67ef = vcombine_s32 (vget_high_s32 (q2_in_4567.val[1]), vget_high_s32 (q2_in_cdef.val[1]));
        q_tw1_r = vcombine_s32 (q_tw1_01.val[0], q_tw1_01.val[0]);
        q_tw1_i = vcombine_s32 (q_tw1_01.val[1], q_tw1_01.val[1]);
        q_tw2_r = vcombine_s32 (q_tw2_23.val[0], q_tw2_23.val[0]);
        q_tw2_i = vcombine_s32 (q_tw2_23.val[1], q_tw2_23.val[1]);
        q_tw3_r = vcombine_s32 (q_tw3_45.val[0], q_tw3_45.val[0]);
        q_tw3_i = vcombine_s32 (q_tw3_45.val[1], q_tw3_45.val[1]);

        q_s0_r = vqrdmulhq_s32 (q_in_r23ab, q_tw1_r);
        q_s0_i = vqrdmulhq_s32 (q_in_i23ab, q_tw1_r);
        q_s1_r = vqrdmulhq_s32 (q_in_r45cd, q_tw2_r);
        q_s1_i = vqrdmulhq_s32 (q_in_i45cd, q_tw2_r);
        q_s2_r = vqrdmulhq_s32 (q_in_r67ef, q_tw3_r);
        q_s2_i = vqrdmulhq_s32 (q_in_i67ef, q_tw3_r);
        q_tmp0 = vqrdmulhq_s32 (q_in_i23ab, q_tw1_i);
        q_tmp1 = vqrdmulhq_s32 (q_in_r23ab, q_tw1_i);
        q_tmp2 = vqrdmulhq_s32 (q_in_i45cd, q_tw2_i);
        q_tmp3 = vqrdmulhq_s32 (q_in_r45cd, q_tw2_i);
        q_tmp4 = vqrdmulhq_s32 (q_in_i67ef, q_tw3_i);
        q_tmp5 = vqrdmulhq_s32 (q_in_r67ef, q_tw3_i);
        q_s0_r = vqaddq_s32 (q_s0_r, q_tmp0);
        q_s0_i = vqsubq_s32 (q_s0_i, q_tmp1);
        q_s1_r = vqaddq_s32 (q_s1_r, q_tmp2);
        q_s1_i = vqsubq_s32 (q_s1_i, q_tmp3);
        q_s2_r = vqaddq_s32 (q_s2_r, q_tmp4);
        q_s2_i = vqsubq_s32 (q_s2_i, q_tmp5);

        q_s5_r = vqsubq_s32 (q_in_r0189, q_s1_r);
        q_s5_i = vqsubq_s32 (q_in_i0189, q_s1_i);
        q_in_r0189 = vqaddq_s32 (q_in_r0189, q_s1_r);
        q_in_i0189 = vqaddq_s32 (q_in_i0189, q_s1_i);

        q_s3_r = vqaddq_s32 (q_s0_r, q_s2_r);
        q_s3_i = vqaddq_s32 (q_s0_i, q_s2_i);
        q_s4_r = vqsubq_s32 (q_s0_r, q_s2_r);
        q_s4_i = vqsubq_s32 (q_s0_i, q_s2_i);

        q_in_r45cd = vqsubq_s32 (q_in_r0189, q_s3_r);
        q_in_i45cd = vqsubq_s32 (q_in_i0189, q_s3_i);
        q_in_r0189 = vqaddq_s32 (q_in_r0189, q_s3_r);
        q_in_i0189 = vqaddq_s32 (q_in_i0189, q_s3_i);

        q_in_r23ab = vqsubq_s32 (q_s5_r, q_s4_i);
        q_in_i23ab = vqaddq_s32 (q_s5_i, q_s4_r);
        q_in_r67ef = vqaddq_s32 (q_s5_r, q_s4_i);
        q_in_i67ef = vqsubq_s32 (q_s5_i, q_s4_r);

        q2_in_0123.val[0] = vcombine_s32 (vget_low_s32 (q_in_r0189), vget_low_s32 (q_in_r23ab));
        q2_in_0123.val[1] = vcombine_s32 (vget_low_s32 (q_in_i0189), vget_low_s32 (q_in_i23ab));
        q2_in_4567.val[0] = vcombine_s32 (vget_low_s32 (q_in_r45cd), vget_low_s32 (q_in_r67ef));
        q2_in_4567.val[1] = vcombine_s32 (vget_low_s32 (q_in_i45cd), vget_low_s32 (q_in_i67ef));
        q2_in_89ab.val[0] = vcombine_s32 (vget_high_s32 (q_in_r0189), vget_high_s32 (q_in_r23ab));
        q2_in_89ab.val[1] = vcombine_s32 (vget_high_s32 (q_in_i0189), vget_high_s32 (q_in_i23ab));
        q2_in_cdef.val[0] = vcombine_s32 (vget_high_s32 (q_in_r45cd), vget_high_s32 (q_in_r67ef));
        q2_in_cdef.val[1] = vcombine_s32 (vget_high_s32 (q_in_i45cd), vget_high_s32 (q_in_i67ef));
        vst2q_s32(p_dst0, q2_in_0123);
        vst2q_s32(p_dst2, q2_in_4567);
        vst2q_s32(p_dst0_1, q2_in_89ab);
        vst2q_s32(p_dst2_1, q2_in_cdef);
    }
    tw += mstride * 3;
    mstride <<= 2;

    // other stages
    stage_count--;
    for (; stage_count > 0; stage_count--)
    {
        fstride = fstride >> 2;
        for (i = 0; i < fstride; i++)
        {
            F = &Fout[i * mstride * 4];
            tw1 = tw;
            tw2 = tw + mstride;
            tw3 = tw + mstride * 2;
            for (j = 0; j < mstride; j += 4)
            {
                p_dst0 = p_src0 = (int32_t*)(&F[0]);
                p_dst1 = p_src1 = (int32_t*)(&F[mstride]);
                p_dst2 = p_src2 = (int32_t*)(&F[mstride * 2]);
                p_dst3 = p_src3 = (int32_t*)(&F[mstride * 3]);
                p_tw1 = (int32_t*)tw1;
                p_tw2 = (int32_t*)tw2;
                p_tw3 = (int32_t*)tw3;
                q2_in_0 = vld2q_s32(p_src0);
                q2_in_1 = vld2q_s32(p_src1);
                q2_in_2 = vld2q_s32(p_src2);
                q2_in_3 = vld2q_s32(p_src3);
                q2_tw1 = vld2q_s32(p_tw1);
                q2_tw2 = vld2q_s32(p_tw2);
                q2_tw3 = vld2q_s32(p_tw3);
                q2_in_0.val[0] = vshrq_n_s32 (q2_in_0.val[0], 2);
                q2_in_0.val[1] = vshrq_n_s32 (q2_in_0.val[1], 2);
                q2_in_1.val[0] = vshrq_n_s32 (q2_in_1.val[0], 2);
                q2_in_1.val[1] = vshrq_n_s32 (q2_in_1.val[1], 2);
                q2_in_2.val[0] = vshrq_n_s32 (q2_in_2.val[0], 2);
                q2_in_2.val[1] = vshrq_n_s32 (q2_in_2.val[1], 2);
                q2_in_3.val[0] = vshrq_n_s32 (q2_in_3.val[0], 2);
                q2_in_3.val[1] = vshrq_n_s32 (q2_in_3.val[1], 2);

                q_s0_r = vqrdmulhq_s32 (q2_in_1.val[0], q2_tw1.val[0]);
                q_s0_i = vqrdmulhq_s32 (q2_in_1.val[1], q2_tw1.val[0]);
                q_s1_r = vqrdmulhq_s32 (q2_in_2.val[0], q2_tw2.val[0]);
                q_s1_i = vqrdmulhq_s32 (q2_in_2.val[1], q2_tw2.val[0]);
                q_s2_r = vqrdmulhq_s32 (q2_in_3.val[0], q2_tw3.val[0]);
                q_s2_i = vqrdmulhq_s32 (q2_in_3.val[1], q2_tw3.val[0]);
                q_tmp0 = vqrdmulhq_s32 (q2_in_1.val[1], q2_tw1.val[1]);
                q_tmp1 = vqrdmulhq_s32 (q2_in_1.val[0], q2_tw1.val[1]);
                q_tmp2 = vqrdmulhq_s32 (q2_in_2.val[1], q2_tw2.val[1]);
                q_tmp3 = vqrdmulhq_s32 (q2_in_2.val[0], q2_tw2.val[1]);
                q_tmp4 = vqrdmulhq_s32 (q2_in_3.val[1], q2_tw3.val[1]);
                q_tmp5 = vqrdmulhq_s32 (q2_in_3.val[0], q2_tw3.val[1]);
                q_s0_r = vqaddq_s32 (q_s0_r, q_tmp0);
                q_s0_i = vqsubq_s32 (q_s0_i, q_tmp1);
                q_s1_r = vqaddq_s32 (q_s1_r, q_tmp2);
                q_s1_i = vqsubq_s32 (q_s1_i, q_tmp3);
                q_s2_r = vqaddq_s32 (q_s2_r, q_tmp4);
                q_s2_i = vqsubq_s32 (q_s2_i, q_tmp5);

                q_s5_r = vqsubq_s32 (q2_in_0.val[0], q_s1_r);
                q_s5_i = vqsubq_s32 (q2_in_0.val[1], q_s1_i);
                q2_in_0.val[0] = vqaddq_s32 (q2_in_0.val[0], q_s1_r);
                q2_in_0.val[1] = vqaddq_s32 (q2_in_0.val[1], q_s1_i);

                q_s3_r = vqaddq_s32 (q_s0_r, q_s2_r);
                q_s3_i = vqaddq_s32 (q_s0_i, q_s2_i);
                q_s4_r = vqsubq_s32 (q_s0_r, q_s2_r);
                q_s4_i = vqsubq_s32 (q_s0_i, q_s2_i);

                q2_in_2.val[0] = vqsubq_s32 (q2_in_0.val[0], q_s3_r);
                q2_in_2.val[1] = vqsubq_s32 (q2_in_0.val[1], q_s3_i);
                q2_in_0.val[0] = vqaddq_s32 (q2_in_0.val[0], q_s3_r);
                q2_in_0.val[1] = vqaddq_s32 (q2_in_0.val[1], q_s3_i);

                q2_in_1.val[0] = vqsubq_s32 (q_s5_r, q_s4_i);
                q2_in_1.val[1] = vqaddq_s32 (q_s5_i, q_s4_r);
                q2_in_3.val[0] = vqaddq_s32 (q_s5_r, q_s4_i);
                q2_in_3.val[1] = vqsubq_s32 (q_s5_i, q_s4_r);

                vst2q_s32(p_dst0, q2_in_0);
                vst2q_s32(p_dst1, q2_in_1);
                vst2q_s32(p_dst2, q2_in_2);
                vst2q_s32(p_dst3, q2_in_3);

                tw1 += 4;
                tw2 += 4;
                tw3 += 4;
                F += 4;
            }
        }
        tw += mstride * 3;
        mstride <<= 2;
    }
}

static void ne10_butterfly_inverse_length_even_power2_int32_neon(ne10_fft_cpx_int32_t * Fout,
        ne10_int32_t * factors,
        ne10_fft_cpx_int32_t * twiddles)
{
    ne10_int32_t i, j, mstride;
    ne10_int32_t stage_count;
    ne10_int32_t fstride;

    ne10_fft_cpx_int32_t *tw, *tw1, *tw2, *tw3;
    ne10_fft_cpx_int32_t *F;


    stage_count = factors[0];
    fstride = factors[1];

    // the first stage
    //fstride is nfft>>2
    for (i = 0; i < fstride; i += 2)
    {
        int32_t *p_src0, *p_src1, *p_src2, *p_src3;
        int32x4_t q_in0, q_in1, q_in2, q_in3;
        int32x4_t q_in_04, q_in_15, q_in_26, q_in_37;
        int32x4_t q_s0, q_s1, q_s2, q_s3, q_s4, q_s5;
        int32x4_t q_tmp0, q_tmp1, q_tmp2, q_tmp3;
        int32x4x2_t q2_tmp0, q2_tmp1;
        p_src0 = (int32_t*)(&(Fout[i*4]));
        p_src1 = (int32_t*)(&(Fout[i*4 + 2]));
        p_src2 = (int32_t*)(&(Fout[i*4 + 4]));
        p_src3 = (int32_t*)(&(Fout[i*4 + 6]));
        q_in0 = vld1q_s32 (p_src0);
        q_in1 = vld1q_s32 (p_src1);
        q_in2 = vld1q_s32 (p_src2);
        q_in3 = vld1q_s32 (p_src3);
        q_in0 = vshrq_n_s32 (q_in0, 2);
        q_in1 = vshrq_n_s32 (q_in1, 2);
        q_in2 = vshrq_n_s32 (q_in2, 2);
        q_in3 = vshrq_n_s32 (q_in3, 2);

        q_in_04 = vcombine_s32 (vget_low_s32 (q_in0), vget_low_s32 (q_in2));
        q_in_15 = vcombine_s32 (vget_high_s32 (q_in0), vget_high_s32 (q_in2));
        q_in_26 = vcombine_s32 (vget_low_s32 (q_in1), vget_low_s32 (q_in3));
        q_in_37 = vcombine_s32 (vget_high_s32 (q_in1), vget_high_s32 (q_in3));

        q_s2 = vqsubq_s32 (q_in_04, q_in_26);
        q_in_04 = vqaddq_s32 (q_in_04, q_in_26);
        q_s0 = vqaddq_s32 (q_in_15, q_in_37);
        q_s1 = vqsubq_s32 (q_in_15, q_in_37);
        q_in_26 = vqsubq_s32 (q_in_04, q_s0);
        q_in_04 = vqaddq_s32 (q_in_04, q_s0);
        q_s1 = vrev64q_s32 (q_s1);
        q_tmp0 = vqsubq_s32 (q_s2, q_s1);
        q_tmp1 = vqaddq_s32 (q_s2, q_s1);
        q_tmp3 = vrev64q_s32 (q_tmp1);
        q2_tmp1 = vtrnq_s32 (q_tmp0, q_tmp3);
        q_in_15 = q2_tmp1.val[0];
        q_in_37 = vrev64q_s32 (q2_tmp1.val[1]);
        q_in0 = vcombine_s32 (vget_low_s32 (q_in_04), vget_low_s32 (q_in_15));
        q_in1 = vcombine_s32 (vget_low_s32 (q_in_26), vget_low_s32 (q_in_37));
        q_in2 = vcombine_s32 (vget_high_s32 (q_in_04), vget_high_s32 (q_in_15));
        q_in3 = vcombine_s32 (vget_high_s32 (q_in_26), vget_high_s32 (q_in_37));

        vst1q_s32 (p_src0, q_in0);
        vst1q_s32 (p_src1, q_in1);
        vst1q_s32 (p_src2, q_in2);
        vst1q_s32 (p_src3, q_in3);
    }
    stage_count--;

    // other stages
    int32_t *p_src0, *p_src1, *p_src2, *p_src3, *p_dst0, *p_dst1, *p_dst2, *p_dst3;
    int32_t *p_tw1, *p_tw2, *p_tw3;
    int32x4_t q_s0_r, q_s0_i, q_s1_r, q_s1_i, q_s2_r, q_s2_i;
    int32x4_t q_s3_r, q_s3_i, q_s4_r, q_s4_i, q_s5_r, q_s5_i;
    int32x4x2_t q2_in_0, q2_in_1, q2_in_2, q2_in_3;
    int32x4x2_t q2_tw1, q2_tw2, q2_tw3;
    int32x4_t q_tmp0, q_tmp1, q_tmp2, q_tmp3, q_tmp4, q_tmp5;
    mstride = factors[2 * stage_count + 1];
    tw = twiddles;
    for (; stage_count > 0; stage_count--)
    {
        fstride = fstride >> 2;
        for (i = 0; i < fstride; i++)
        {
            F = &Fout[i * mstride * 4];
            tw1 = tw;
            tw2 = tw + mstride;
            tw3 = tw + mstride * 2;
            for (j = 0; j < mstride; j += 4)
            {
                p_dst0 = p_src0 = (int32_t*)(&F[0]);
                p_dst1 = p_src1 = (int32_t*)(&F[mstride]);
                p_dst2 = p_src2 = (int32_t*)(&F[mstride * 2]);
                p_dst3 = p_src3 = (int32_t*)(&F[mstride * 3]);
                p_tw1 = (int32_t*)tw1;
                p_tw2 = (int32_t*)tw2;
                p_tw3 = (int32_t*)tw3;
                q2_in_0 = vld2q_s32(p_src0);
                q2_in_1 = vld2q_s32(p_src1);
                q2_in_2 = vld2q_s32(p_src2);
                q2_in_3 = vld2q_s32(p_src3);
                q2_tw1 = vld2q_s32(p_tw1);
                q2_tw2 = vld2q_s32(p_tw2);
                q2_tw3 = vld2q_s32(p_tw3);
                q2_in_0.val[0] = vshrq_n_s32 (q2_in_0.val[0], 2);
                q2_in_0.val[1] = vshrq_n_s32 (q2_in_0.val[1], 2);
                q2_in_1.val[0] = vshrq_n_s32 (q2_in_1.val[0], 2);
                q2_in_1.val[1] = vshrq_n_s32 (q2_in_1.val[1], 2);
                q2_in_2.val[0] = vshrq_n_s32 (q2_in_2.val[0], 2);
                q2_in_2.val[1] = vshrq_n_s32 (q2_in_2.val[1], 2);
                q2_in_3.val[0] = vshrq_n_s32 (q2_in_3.val[0], 2);
                q2_in_3.val[1] = vshrq_n_s32 (q2_in_3.val[1], 2);

                q_s0_r = vqrdmulhq_s32 (q2_in_1.val[0], q2_tw1.val[0]);
                q_s0_i = vqrdmulhq_s32 (q2_in_1.val[1], q2_tw1.val[0]);
                q_s1_r = vqrdmulhq_s32 (q2_in_2.val[0], q2_tw2.val[0]);
                q_s1_i = vqrdmulhq_s32 (q2_in_2.val[1], q2_tw2.val[0]);
                q_s2_r = vqrdmulhq_s32 (q2_in_3.val[0], q2_tw3.val[0]);
                q_s2_i = vqrdmulhq_s32 (q2_in_3.val[1], q2_tw3.val[0]);
                q_tmp0 = vqrdmulhq_s32 (q2_in_1.val[1], q2_tw1.val[1]);
                q_tmp1 = vqrdmulhq_s32 (q2_in_1.val[0], q2_tw1.val[1]);
                q_tmp2 = vqrdmulhq_s32 (q2_in_2.val[1], q2_tw2.val[1]);
                q_tmp3 = vqrdmulhq_s32 (q2_in_2.val[0], q2_tw2.val[1]);
                q_tmp4 = vqrdmulhq_s32 (q2_in_3.val[1], q2_tw3.val[1]);
                q_tmp5 = vqrdmulhq_s32 (q2_in_3.val[0], q2_tw3.val[1]);
                q_s0_r = vqaddq_s32 (q_s0_r, q_tmp0);
                q_s0_i = vqsubq_s32 (q_s0_i, q_tmp1);
                q_s1_r = vqaddq_s32 (q_s1_r, q_tmp2);
                q_s1_i = vqsubq_s32 (q_s1_i, q_tmp3);
                q_s2_r = vqaddq_s32 (q_s2_r, q_tmp4);
                q_s2_i = vqsubq_s32 (q_s2_i, q_tmp5);

                q_s5_r = vqsubq_s32 (q2_in_0.val[0], q_s1_r);
                q_s5_i = vqsubq_s32 (q2_in_0.val[1], q_s1_i);
                q2_in_0.val[0] = vqaddq_s32 (q2_in_0.val[0], q_s1_r);
                q2_in_0.val[1] = vqaddq_s32 (q2_in_0.val[1], q_s1_i);

                q_s3_r = vqaddq_s32 (q_s0_r, q_s2_r);
                q_s3_i = vqaddq_s32 (q_s0_i, q_s2_i);
                q_s4_r = vqsubq_s32 (q_s0_r, q_s2_r);
                q_s4_i = vqsubq_s32 (q_s0_i, q_s2_i);

                q2_in_2.val[0] = vqsubq_s32 (q2_in_0.val[0], q_s3_r);
                q2_in_2.val[1] = vqsubq_s32 (q2_in_0.val[1], q_s3_i);
                q2_in_0.val[0] = vqaddq_s32 (q2_in_0.val[0], q_s3_r);
                q2_in_0.val[1] = vqaddq_s32 (q2_in_0.val[1], q_s3_i);

                q2_in_1.val[0] = vqsubq_s32 (q_s5_r, q_s4_i);
                q2_in_1.val[1] = vqaddq_s32 (q_s5_i, q_s4_r);
                q2_in_3.val[0] = vqaddq_s32 (q_s5_r, q_s4_i);
                q2_in_3.val[1] = vqsubq_s32 (q_s5_i, q_s4_r);

                vst2q_s32(p_dst0, q2_in_0);
                vst2q_s32(p_dst1, q2_in_1);
                vst2q_s32(p_dst2, q2_in_2);
                vst2q_s32(p_dst3, q2_in_3);

                tw1 += 4;
                tw2 += 4;
                tw3 += 4;
                F += 4;
            }
        }
        tw += mstride * 3;
        mstride <<= 2;
    }
}

static void ne10_mixed_radix_butterfly_backward_int32_scaled_neon (ne10_fft_cpx_int32_t * Fout,
        ne10_int32_t * factors,
        ne10_fft_cpx_int32_t * twiddles)
{
    ne10_int32_t stage_count = factors[0];

    if (factors[2 * stage_count] == 2) // length of FFT is 2^n (n is odd)
    {
        //radix 2/4, FFT length is 2^n (n is odd)
        ne10_butterfly_inverse_length_odd_power2_int32_neon (Fout, factors, twiddles);
    }
    else if (factors[2 * stage_count] == 4) // length of FFT is 2^n (n is even)
    {
        //radix 4, FFT length is 2^n (n is even)
        ne10_butterfly_inverse_length_even_power2_int32_neon (Fout, factors, twiddles);
    }
}

/**
 * @addtogroup C2C_FFT_IFFT
 * @{
 */

/**
 * @brief Mixed radix-2/4 complex FFT/IFFT of 32-bit fixed point data.
 * @param[out]  *fout            point to the output buffer (out-of-place)
 * @param[in]   *fin             point to the input buffer (out-of-place)
 * @param[in]   *twiddles        point to the twiddle buffer
 * @param[in]   *factors         point to factors buffer. 0: stage number, 1: stride for the first stage, others: radix and stage's fft length/radix
 * @param[in]   nfft             length of FFT
 * @param[in]   inverse_fft      the flag of IFFT, 0: FFT, 1: IFFT
 * @return none.
 * The function implements a mixed radix-2/4 complex FFT/IFFT. The length of 2^N(N is 3, 4, 5, 6 ....etc) is supported.
 * Otherwise, this FFT is an out-of-place algorithm. When you want to get an in-place FFT, it creates a temp buffer as
 *  output buffer and then copies the temp buffer back to input buffer. For the usage of this function, please check test/test_suite_fft_int32.c
 */
void ne10_fft_c2c_1d_int32_scaled_neon (ne10_fft_cpx_int32_t *fout,
                                 ne10_fft_cpx_int32_t *fin,
                                 ne10_fft_cpx_int32_t *twiddles,
                                 ne10_int32_t *factors,
                                 ne10_int32_t nfft,
                                 ne10_int32_t inverse_fft)
{
    if (inverse_fft)
    {
        switch (nfft)
        {
        case 4:
            ne10_fft4_backward_int32_scaled (fout, fin);
            break;
        case 8:
            ne10_fft8_backward_int32_scaled (fout, fin);
            break;
        case 16:
            ne10_fft16_backward_int32_scaled_neon (fout, fin, twiddles);
            break;
        default:
            ne10_data_bitreversal_int32 (fout, fin, 1, &factors[2]);
            ne10_mixed_radix_butterfly_backward_int32_scaled_neon (fout, factors, twiddles);
            break;
        }
    }
    else
    {
        switch (nfft)
        {
        case 4:
            ne10_fft4_forward_int32_scaled (fout, fin);
            break;
        case 8:
            ne10_fft8_forward_int32_scaled (fout, fin);
            break;
        case 16:
            ne10_fft16_forward_int32_scaled_neon (fout, fin, twiddles);
            break;
        default:
            ne10_data_bitreversal_int32 (fout, fin, 1, &factors[2]);
            ne10_mixed_radix_butterfly_forward_int32_scaled_neon (fout, factors, twiddles);
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
 * @brief Mixed radix-2/4 FFT (real to complex) of int32 data.
 * @param[out]  *fout            point to the output buffer
 * @param[in]   *fin             point to the input buffer
 * @param[in]   *twiddles        point to the twiddle buffer
 * @param[in]   *super_twiddles  point to the twiddle buffer for data split
 * @param[in]   *factors         point to factors buffer. 0: stage number, 1: stride for the first stage, others: radix and stage's fft length/radix
 * @param[in]   nfft             length of FFT
 * @return none.
 * The function implements a mixed radix-2/4 FFT (real to complex). The length of 2^N(N is 2, 3, 4, 5, 6 ....etc) is supported.
 * Otherwise, we alloc a temp buffer(the size is same as input buffer) for storing intermedia.
 * For the usage of this function, please check test/test_suite_fft_int32.c
 */
void ne10_fft_r2c_1d_int32_scaled_neon (ne10_fft_cpx_int32_t *fout,
                                 ne10_int32_t *fin,
                                 ne10_fft_cpx_int32_t *twiddles,
                                 ne10_fft_cpx_int32_t *super_twiddles,
                                 ne10_int32_t *factors,
                                 ne10_int32_t nfft)
{
    ne10_int32_t ncfft = nfft >> 1;

    /* malloc a temp buffer for cfft */
    ne10_fft_cpx_int32_t * tmpbuf_ = (ne10_fft_cpx_int32_t*) NE10_MALLOC (sizeof (ne10_fft_cpx_int32_t) * ncfft);

    // copy the data from input to output and bit reversal
    ne10_fft_c2c_1d_int32_scaled_neon (tmpbuf_, (ne10_fft_cpx_int32_t*) fin, twiddles, factors, ncfft, 0);
    ne10_fft_split_r2c_1d_int32 (fout, tmpbuf_, super_twiddles, ncfft);

    NE10_FREE (tmpbuf_);
}

/**
 * @brief Mixed radix-2/4 IFFT (complex to real) of int32 data.
 * @param[out]  *fout            point to the output buffer
 * @param[in]   *fin             point to the input buffer
 * @param[in]   *twiddles        point to the twiddle buffer
 * @param[in]   *super_twiddles  point to the twiddle buffer for data split
 * @param[in]   *factors         point to factors buffer. 0: stage number, 1: stride for the first stage, others: radix and stage's fft length/radix
 * @param[in]   nfft             length of FFT
 * @return none.
 * The function implements a mixed radix-2/4 FFT (complex to real). The length of 2^N(N is 2, 3, 4, 5, 6 ....etc) is supported.
 * Otherwise, we alloc a temp buffer(the size is same as input buffer) for storing intermedia.
 * For the usage of this function, please check test/test_suite_fft_int32.c
 */
void ne10_fft_c2r_1d_int32_scaled_neon (ne10_int32_t *fout,
                                 ne10_fft_cpx_int32_t *fin,
                                 ne10_fft_cpx_int32_t *twiddles,
                                 ne10_fft_cpx_int32_t *super_twiddles,
                                 ne10_int32_t *factors,
                                 ne10_int32_t nfft)
{
    ne10_int32_t ncfft = nfft >> 1;

    /* malloc a temp buffer for split */
    ne10_fft_cpx_int32_t * tmpbuf_ = (ne10_fft_cpx_int32_t*) NE10_MALLOC (sizeof (ne10_fft_cpx_int32_t) * ncfft);

    ne10_fft_split_c2r_1d_int32 (tmpbuf_, fin, super_twiddles, ncfft);
    ne10_fft_c2c_1d_int32_scaled_neon ( (ne10_fft_cpx_int32_t*) fout, tmpbuf_, twiddles, factors, ncfft, 1);

    NE10_FREE (tmpbuf_);
}

/**
 * @} end of R2C_FFT_IFFT group
 */
