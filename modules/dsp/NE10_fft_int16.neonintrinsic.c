/*
 *  Copyright 2013-15 ARM Limited and Contributors.
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
 * NE10 Library : dsp/NE10_fft_int16.neon.c
 */

#include <arm_neon.h>

#include "NE10_types.h"
#include "NE10_macros.h"
#include "NE10_fft.h"

#define FFT4_FS_START \
    ne10_int16_t s0_r, s0_i, s1_r, s1_i, s2_r, s2_i; \
    ne10_int16_t tmp_r, tmp_i;


#define FFT4_FS \
    s2_r = Fin[0].r - Fin[2].r; \
    s2_i = Fin[0].i - Fin[2].i; \
    tmp_r = Fin[0].r + Fin[2].r; \
    tmp_i = Fin[0].i + Fin[2].i; \
    s0_r = Fin[1].r + Fin[3].r; \
    s0_i = Fin[1].i + Fin[3].i; \
    s1_r = Fin[1].r - Fin[3].r; \
    s1_i = Fin[1].i - Fin[3].i;

#define FFT4_FS_SCALED \
    s2_r = (Fin[0].r - Fin[2].r) >> 2; \
    s2_i = (Fin[0].i - Fin[2].i) >> 2; \
    tmp_r = (Fin[0].r + Fin[2].r) >> 2; \
    tmp_i = (Fin[0].i + Fin[2].i) >> 2; \
    s0_r = (Fin[1].r + Fin[3].r) >> 2; \
    s0_i = (Fin[1].i + Fin[3].i) >> 2; \
    s1_r = (Fin[1].r - Fin[3].r) >> 2; \
    s1_i = (Fin[1].i - Fin[3].i) >> 2;

#define FFT4_FWD_LS \
    Fout[2].r = tmp_r - s0_r; \
    Fout[2].i = tmp_i - s0_i; \
    Fout[0].r = tmp_r + s0_r; \
    Fout[0].i = tmp_i + s0_i; \
    Fout[1].r = s2_r + s1_i; \
    Fout[1].i = s2_i - s1_r; \
    Fout[3].r = s2_r - s1_i; \
    Fout[3].i = s2_i + s1_r;

#define FFT4_INV_LS \
    Fout[2].r = tmp_r - s0_r; \
    Fout[2].i = tmp_i - s0_i; \
    Fout[0].r = tmp_r + s0_r; \
    Fout[0].i = tmp_i + s0_i; \
    Fout[1].r = s2_r - s1_i; \
    Fout[1].i = s2_i + s1_r; \
    Fout[3].r = s2_r + s1_i; \
    Fout[3].i = s2_i - s1_r;

static inline void ne10_fft4_forward_int16_unscaled (ne10_fft_cpx_int16_t * Fout,
        ne10_fft_cpx_int16_t * Fin)

{
    FFT4_FS_START
    FFT4_FS
    FFT4_FWD_LS
}

static inline void ne10_fft4_backward_int16_unscaled (ne10_fft_cpx_int16_t * Fout,
        ne10_fft_cpx_int16_t * Fin)

{
    FFT4_FS_START
    FFT4_FS
    FFT4_INV_LS
}
static inline void ne10_fft4_forward_int16_scaled (ne10_fft_cpx_int16_t * Fout,
        ne10_fft_cpx_int16_t * Fin)

{
    FFT4_FS_START
    FFT4_FS_SCALED
    FFT4_FWD_LS
}

static inline void ne10_fft4_backward_int16_scaled (ne10_fft_cpx_int16_t * Fout,
        ne10_fft_cpx_int16_t * Fin)

{
    FFT4_FS_START
    FFT4_FS_SCALED
    FFT4_INV_LS
}

#define FFT8_FS_START \
    ne10_int16_t s0_r, s0_i, s1_r, s1_i, s2_r, s2_i, s3_r, s3_i, s4_r, s4_i, s5_r, s5_i, s6_r, s6_i, s7_r, s7_i; \
    ne10_int16_t t0_r, t0_i, t1_r, t1_i, t2_r, t2_i, t3_r, t3_i, t4_r, t4_i, t5_r, t5_i; \
    const ne10_int16_t TW_81 = 23169;

#define FFT8_FS \
    s0_r = Fin[0].r + Fin[4].r; \
    s0_i = Fin[0].i + Fin[4].i; \
    s1_r = Fin[0].r - Fin[4].r; \
    s1_i = Fin[0].i - Fin[4].i; \
    s2_r = Fin[1].r + Fin[5].r; \
    s2_i = Fin[1].i + Fin[5].i; \
    s3_r = Fin[1].r - Fin[5].r; \
    s3_i = Fin[1].i - Fin[5].i; \
    s4_r = Fin[2].r + Fin[6].r; \
    s4_i = Fin[2].i + Fin[6].i; \
    s5_r = Fin[2].r - Fin[6].r; \
    s5_i = Fin[2].i - Fin[6].i; \
    s6_r = Fin[3].r + Fin[7].r; \
    s6_i = Fin[3].i + Fin[7].i; \
    s7_r = Fin[3].r - Fin[7].r; \
    s7_i = Fin[3].i - Fin[7].i;

#define FFT8_FS_SCALED \
    s0_r = (Fin[0].r + Fin[4].r) >> 3; \
    s0_i = (Fin[0].i + Fin[4].i) >> 3; \
    s1_r = (Fin[0].r - Fin[4].r) >> 3; \
    s1_i = (Fin[0].i - Fin[4].i) >> 3; \
    s2_r = (Fin[1].r + Fin[5].r) >> 3; \
    s2_i = (Fin[1].i + Fin[5].i) >> 3; \
    s3_r = (Fin[1].r - Fin[5].r) >> 3; \
    s3_i = (Fin[1].i - Fin[5].i) >> 3; \
    s4_r = (Fin[2].r + Fin[6].r) >> 3; \
    s4_i = (Fin[2].i + Fin[6].i) >> 3; \
    s5_r = (Fin[2].r - Fin[6].r) >> 3; \
    s5_i = (Fin[2].i - Fin[6].i) >> 3; \
    s6_r = (Fin[3].r + Fin[7].r) >> 3; \
    s6_i = (Fin[3].i + Fin[7].i) >> 3; \
    s7_r = (Fin[3].r - Fin[7].r) >> 3; \
    s7_i = (Fin[3].i - Fin[7].i) >> 3;


#define FFT8_FWD_LS \
    t0_r = s0_r - s4_r; \
    t0_i = s0_i - s4_i; \
    t1_r = s0_r + s4_r; \
    t1_i = s0_i + s4_i; \
    t2_r = s2_r + s6_r; \
    t2_i = s2_i + s6_i; \
    t3_r = s2_r - s6_r; \
    t3_i = s2_i - s6_i; \
    Fout[0].r = t1_r + t2_r; \
    Fout[0].i = t1_i + t2_i; \
    Fout[4].r = t1_r - t2_r; \
    Fout[4].i = t1_i - t2_i; \
    Fout[2].r = t0_r + t3_i; \
    Fout[2].i = t0_i - t3_r; \
    Fout[6].r = t0_r - t3_i; \
    Fout[6].i = t0_i + t3_r; \
    t4_r = (ne10_int16_t) ( ( (NE10_F2I16_SAMPPROD) (s3_r + s3_i) * TW_81) >> NE10_F2I16_SHIFT); \
    t4_i = - (ne10_int16_t) ( ( (NE10_F2I16_SAMPPROD) (s3_r - s3_i) * TW_81) >> NE10_F2I16_SHIFT); \
    t5_r = (ne10_int16_t) ( ( (NE10_F2I16_SAMPPROD) (s7_r - s7_i) * TW_81) >> NE10_F2I16_SHIFT); \
    t5_i = (ne10_int16_t) ( ( (NE10_F2I16_SAMPPROD) (s7_r + s7_i) * TW_81) >> NE10_F2I16_SHIFT); \
    t0_r = s1_r - s5_i; \
    t0_i = s1_i + s5_r; \
    t1_r = s1_r + s5_i; \
    t1_i = s1_i - s5_r; \
    t2_r = t4_r - t5_r; \
    t2_i = t4_i - t5_i; \
    t3_r = t4_r + t5_r; \
    t3_i = t4_i + t5_i; \
    Fout[1].r = t1_r + t2_r; \
    Fout[1].i = t1_i + t2_i; \
    Fout[5].r = t1_r - t2_r; \
    Fout[5].i = t1_i - t2_i; \
    Fout[3].r = t0_r + t3_i; \
    Fout[3].i = t0_i - t3_r; \
    Fout[7].r = t0_r - t3_i; \
    Fout[7].i = t0_i + t3_r;

#define FFT8_INV_LS \
    t0_r = s0_r - s4_r; \
    t0_i = s0_i - s4_i; \
    t1_r = s0_r + s4_r; \
    t1_i = s0_i + s4_i; \
    t2_r = s2_r + s6_r; \
    t2_i = s2_i + s6_i; \
    t3_r = s2_r - s6_r; \
    t3_i = s2_i - s6_i; \
    Fout[0].r = t1_r + t2_r; \
    Fout[0].i = t1_i + t2_i; \
    Fout[4].r = t1_r - t2_r; \
    Fout[4].i = t1_i - t2_i; \
    Fout[2].r = t0_r - t3_i; \
    Fout[2].i = t0_i + t3_r; \
    Fout[6].r = t0_r + t3_i; \
    Fout[6].i = t0_i - t3_r; \
    t4_r = (ne10_int16_t) ( ( (NE10_F2I16_SAMPPROD) (s3_r - s3_i) * TW_81) >> NE10_F2I16_SHIFT); \
    t4_i = (ne10_int16_t) ( ( (NE10_F2I16_SAMPPROD) (s3_r + s3_i) * TW_81) >> NE10_F2I16_SHIFT); \
    t5_r = (ne10_int16_t) ( ( (NE10_F2I16_SAMPPROD) (s7_r + s7_i) * TW_81) >> NE10_F2I16_SHIFT); \
    t5_i = - (ne10_int16_t) ( ( (NE10_F2I16_SAMPPROD) (s7_r - s7_i) * TW_81) >> NE10_F2I16_SHIFT); \
    t0_r = s1_r + s5_i; \
    t0_i = s1_i - s5_r; \
    t1_r = s1_r - s5_i; \
    t1_i = s1_i + s5_r; \
    t2_r = t4_r - t5_r; \
    t2_i = t4_i - t5_i; \
    t3_r = t4_r + t5_r; \
    t3_i = t4_i + t5_i; \
    Fout[1].r = t1_r + t2_r; \
    Fout[1].i = t1_i + t2_i; \
    Fout[5].r = t1_r - t2_r; \
    Fout[5].i = t1_i - t2_i; \
    Fout[3].r = t0_r - t3_i; \
    Fout[3].i = t0_i + t3_r; \
    Fout[7].r = t0_r + t3_i; \
    Fout[7].i = t0_i - t3_r;

static inline void ne10_fft8_forward_int16_unscaled (ne10_fft_cpx_int16_t * Fout,
        ne10_fft_cpx_int16_t * Fin)

{
    FFT8_FS_START
    FFT8_FS
    FFT8_FWD_LS
}

static inline void ne10_fft8_backward_int16_unscaled (ne10_fft_cpx_int16_t * Fout,
        ne10_fft_cpx_int16_t * Fin)

{
    FFT8_FS_START
    FFT8_FS
    FFT8_INV_LS
}
static inline void ne10_fft8_forward_int16_scaled (ne10_fft_cpx_int16_t * Fout,
        ne10_fft_cpx_int16_t * Fin)

{
    FFT8_FS_START
    FFT8_FS_SCALED
    FFT8_FWD_LS
}

static inline void ne10_fft8_backward_int16_scaled (ne10_fft_cpx_int16_t * Fout,
        ne10_fft_cpx_int16_t * Fin)

{
    FFT8_FS_START
    FFT8_FS_SCALED
    FFT8_INV_LS
}

#define RADIX8x4_START \
    ne10_int32_t f_count; \
    ne10_int32_t src_step = stride << 1; \
    const ne10_int16_t TW_81 = 23169; \
    const ne10_int16_t TW_81N = -23169; \
    int16_t *p_src, *p_dst; \
    int16x4x2_t d2_in0, d2_in1, d2_in2, d2_in3, d2_in4, d2_in5, d2_in6, d2_in7; \
    int16x4_t d_sin0_r, d_sin0_i, d_sin1_r, d_sin1_i, d_sin2_r, d_sin2_i, d_sin3_r, d_sin3_i; \
    int16x4_t d_sin4_r, d_sin4_i, d_sin5_r, d_sin5_i, d_sin6_r, d_sin6_i, d_sin7_r, d_sin7_i; \
    int16x4_t d_s3_r, d_s3_i, d_s5_r, d_s5_i, d_s7_r, d_s7_i; \
    int16x4_t d_s8_r, d_s8_i, d_s9_r, d_s9_i, d_s10_r, d_s10_i, d_s11_r, d_s11_i; \
    int16x4_t d_s12_r, d_s12_i, d_s13_r, d_s13_i, d_s14_r, d_s14_i, d_s15_r, d_s15_i; \
    int16x4_t d_out0_r, d_out0_i, d_out1_r, d_out1_i, d_out2_r, d_out2_i, d_out3_r, d_out3_i; \
    int16x4_t d_out4_r, d_out4_i, d_out5_r, d_out5_i, d_out6_r, d_out6_i, d_out7_r, d_out7_i; \
    int16x4x2_t d2_out0, d2_out1, d2_out2, d2_out3, d2_out4, d2_out5, d2_out6, d2_out7; \
    int16x8x2_t q2_tmp0, q2_tmp1, q2_tmp2, q2_tmp3; \
    int32x4x2_t q2_tmp4, q2_tmp5, q2_tmp6, q2_tmp7; \
    int16x4_t d_tw_81, d_tw_81n; \
    p_src = (int16_t *) Fin; \
    p_dst = (int16_t *) Fout;


#define RADIX8x4_LOAD \
    d2_in0 = vld2_s16 (p_src); \
    p_src += src_step; \
    d2_in2 = vld2_s16 (p_src); \
    p_src += src_step; \
    d2_in4 = vld2_s16 (p_src); \
    p_src += src_step; \
    d2_in6 = vld2_s16 (p_src); \
    p_src += src_step; \
    d2_in1 = vld2_s16 (p_src); \
    p_src += src_step; \
    d2_in3 = vld2_s16 (p_src); \
    p_src += src_step; \
    d2_in5 = vld2_s16 (p_src); \
    p_src += src_step; \
    d2_in7 = vld2_s16 (p_src); \
    p_src += src_step;

#define RADIX8x4_STORE \
    q2_tmp0 = vtrnq_s16 (vcombine_s16(d_out0_r, d_out0_i), vcombine_s16(d_out1_r, d_out1_i)); \
    q2_tmp1 = vtrnq_s16 (vcombine_s16(d_out2_r, d_out2_i), vcombine_s16(d_out3_r, d_out3_i)); \
    q2_tmp2 = vtrnq_s16 (vcombine_s16(d_out4_r, d_out4_i), vcombine_s16(d_out5_r, d_out5_i)); \
    q2_tmp3 = vtrnq_s16 (vcombine_s16(d_out6_r, d_out6_i), vcombine_s16(d_out7_r, d_out7_i)); \
    q2_tmp4 = vtrnq_s32 (vreinterpretq_s32_s16(q2_tmp0.val[0]), vreinterpretq_s32_s16(q2_tmp1.val[0])); \
    q2_tmp5 = vtrnq_s32 (vreinterpretq_s32_s16(q2_tmp0.val[1]), vreinterpretq_s32_s16(q2_tmp1.val[1])); \
    q2_tmp6 = vtrnq_s32 (vreinterpretq_s32_s16(q2_tmp2.val[0]), vreinterpretq_s32_s16(q2_tmp3.val[0])); \
    q2_tmp7 = vtrnq_s32 (vreinterpretq_s32_s16(q2_tmp2.val[1]), vreinterpretq_s32_s16(q2_tmp3.val[1])); \
    d2_out0.val[0] = vget_low_s16 (vreinterpretq_s16_s32(q2_tmp4.val[0])); \
    d2_out0.val[1] = vget_high_s16 (vreinterpretq_s16_s32(q2_tmp4.val[0])); \
    d2_out1.val[0] = vget_low_s16 (vreinterpretq_s16_s32(q2_tmp6.val[0])); \
    d2_out1.val[1] = vget_high_s16 (vreinterpretq_s16_s32(q2_tmp6.val[0])); \
    d2_out2.val[0] = vget_low_s16 (vreinterpretq_s16_s32(q2_tmp5.val[0])); \
    d2_out2.val[1] = vget_high_s16 (vreinterpretq_s16_s32(q2_tmp5.val[0])); \
    d2_out3.val[0] = vget_low_s16 (vreinterpretq_s16_s32(q2_tmp7.val[0])); \
    d2_out3.val[1] = vget_high_s16 (vreinterpretq_s16_s32(q2_tmp7.val[0])); \
    d2_out4.val[0] = vget_low_s16 (vreinterpretq_s16_s32(q2_tmp4.val[1])); \
    d2_out4.val[1] = vget_high_s16 (vreinterpretq_s16_s32(q2_tmp4.val[1])); \
    d2_out5.val[0] = vget_low_s16 (vreinterpretq_s16_s32(q2_tmp6.val[1])); \
    d2_out5.val[1] = vget_high_s16 (vreinterpretq_s16_s32(q2_tmp6.val[1])); \
    d2_out6.val[0] = vget_low_s16 (vreinterpretq_s16_s32(q2_tmp5.val[1])); \
    d2_out6.val[1] = vget_high_s16 (vreinterpretq_s16_s32(q2_tmp5.val[1])); \
    d2_out7.val[0] = vget_low_s16 (vreinterpretq_s16_s32(q2_tmp7.val[1])); \
    d2_out7.val[1] = vget_high_s16 (vreinterpretq_s16_s32(q2_tmp7.val[1])); \
    vst2_s16 (p_dst, d2_out0); \
    p_dst += 8; \
    vst2_s16 (p_dst, d2_out1); \
    p_dst += 8; \
    vst2_s16 (p_dst, d2_out2); \
    p_dst += 8; \
    vst2_s16 (p_dst, d2_out3); \
    p_dst += 8; \
    vst2_s16 (p_dst, d2_out4); \
    p_dst += 8; \
    vst2_s16 (p_dst, d2_out5); \
    p_dst += 8; \
    vst2_s16 (p_dst, d2_out6); \
    p_dst += 8; \
    vst2_s16 (p_dst, d2_out7); \
    p_dst += 8; \
    p_src = p_src - src_step * 8 + 8;

#define RADIX8x4_FS_S0 \
    d_sin0_r = vadd_s16 (d2_in0.val[0], d2_in1.val[0]); \
    d_sin0_i = vadd_s16 (d2_in0.val[1], d2_in1.val[1]); \
    d_sin1_r = vsub_s16 (d2_in0.val[0], d2_in1.val[0]); \
    d_sin1_i = vsub_s16 (d2_in0.val[1], d2_in1.val[1]); \
    d_sin2_r = vadd_s16 (d2_in2.val[0], d2_in3.val[0]); \
    d_sin2_i = vadd_s16 (d2_in2.val[1], d2_in3.val[1]); \
    d_sin3_r = vsub_s16 (d2_in2.val[0], d2_in3.val[0]); \
    d_sin3_i = vsub_s16 (d2_in2.val[1], d2_in3.val[1]); \
    d_sin4_r = vadd_s16 (d2_in4.val[0], d2_in5.val[0]); \
    d_sin4_i = vadd_s16 (d2_in4.val[1], d2_in5.val[1]); \
    d_sin5_r = vsub_s16 (d2_in4.val[0], d2_in5.val[0]); \
    d_sin5_i = vsub_s16 (d2_in4.val[1], d2_in5.val[1]); \
    d_sin6_r = vadd_s16 (d2_in6.val[0], d2_in7.val[0]); \
    d_sin6_i = vadd_s16 (d2_in6.val[1], d2_in7.val[1]); \
    d_sin7_r = vsub_s16 (d2_in6.val[0], d2_in7.val[0]); \
    d_sin7_i = vsub_s16 (d2_in6.val[1], d2_in7.val[1]);

#define RADIX8x4_FWD_S357 \
    d_tw_81 = vdup_n_s16 (TW_81); \
    d_tw_81n = vdup_n_s16 (TW_81N); \
    d_s5_r = d_sin5_i; \
    d_s5_i = vneg_s16 (d_sin5_r); \
    d_s3_r = vadd_s16 (d_sin3_r, d_sin3_i); \
    d_s3_i = vsub_s16 (d_sin3_i, d_sin3_r); \
    d_s7_r = vsub_s16 (d_sin7_r, d_sin7_i); \
    d_s7_i = vadd_s16 (d_sin7_i, d_sin7_r); \
    d_s3_r = vqdmulh_s16 (d_s3_r, d_tw_81); \
    d_s3_i = vqdmulh_s16 (d_s3_i, d_tw_81); \
    d_s7_r = vqdmulh_s16 (d_s7_r, d_tw_81n); \
    d_s7_i = vqdmulh_s16 (d_s7_i, d_tw_81n);

#define RADIX8x4_INV_S357 \
    d_tw_81 = vdup_n_s16 (TW_81); \
    d_tw_81n = vdup_n_s16 (TW_81N); \
    d_s5_r = vneg_s16 (d_sin5_i); \
    d_s5_i = d_sin5_r; \
    d_s3_r = vsub_s16 (d_sin3_r, d_sin3_i); \
    d_s3_i = vadd_s16 (d_sin3_i, d_sin3_r); \
    d_s7_r = vadd_s16 (d_sin7_r, d_sin7_i); \
    d_s7_i = vsub_s16 (d_sin7_i, d_sin7_r); \
    d_s3_r = vqdmulh_s16 (d_s3_r, d_tw_81); \
    d_s3_i = vqdmulh_s16 (d_s3_i, d_tw_81); \
    d_s7_r = vqdmulh_s16 (d_s7_r, d_tw_81n); \
    d_s7_i = vqdmulh_s16 (d_s7_i, d_tw_81n);

#define RADIX8x4_LS_02 \
    d_s8_r = vadd_s16 (d_sin0_r, d_sin4_r); \
    d_s8_i = vadd_s16 (d_sin0_i, d_sin4_i); \
    d_s9_r = vadd_s16 (d_sin1_r, d_s5_r); \
    d_s9_i = vadd_s16 (d_sin1_i, d_s5_i); \
    d_s10_r = vsub_s16 (d_sin0_r, d_sin4_r); \
    d_s10_i = vsub_s16 (d_sin0_i, d_sin4_i); \
    d_s11_r = vsub_s16 (d_sin1_r, d_s5_r); \
    d_s11_i = vsub_s16 (d_sin1_i, d_s5_i); \
    d_s12_r = vadd_s16 (d_sin2_r, d_sin6_r); \
    d_s12_i = vadd_s16 (d_sin2_i, d_sin6_i); \
    d_s13_r = vadd_s16 (d_s3_r, d_s7_r); \
    d_s13_i = vadd_s16 (d_s3_i, d_s7_i); \
    d_s14_r = vsub_s16 (d_sin2_r, d_sin6_r); \
    d_s14_i = vsub_s16 (d_sin2_i, d_sin6_i); \
    d_s15_r = vsub_s16 (d_s3_r, d_s7_r); \
    d_s15_i = vsub_s16 (d_s3_i, d_s7_i); \
    d_out4_r = vsub_s16 (d_s8_r, d_s12_r); \
    d_out4_i = vsub_s16 (d_s8_i, d_s12_i); \
    d_out5_r = vsub_s16 (d_s9_r, d_s13_r); \
    d_out5_i = vsub_s16 (d_s9_i, d_s13_i); \
    d_out0_r = vadd_s16 (d_s8_r, d_s12_r); \
    d_out0_i = vadd_s16 (d_s8_i, d_s12_i); \
    d_out1_r = vadd_s16 (d_s9_r, d_s13_r); \
    d_out1_i = vadd_s16 (d_s9_i, d_s13_i);

#define RADIX8x4_FS_S0_SCALED \
    d_sin0_r = vhadd_s16 (d2_in0.val[0], d2_in1.val[0]); \
    d_sin0_i = vhadd_s16 (d2_in0.val[1], d2_in1.val[1]); \
    d_sin1_r = vhsub_s16 (d2_in0.val[0], d2_in1.val[0]); \
    d_sin1_i = vhsub_s16 (d2_in0.val[1], d2_in1.val[1]); \
    d_sin2_r = vhadd_s16 (d2_in2.val[0], d2_in3.val[0]); \
    d_sin2_i = vhadd_s16 (d2_in2.val[1], d2_in3.val[1]); \
    d_sin3_r = vhsub_s16 (d2_in2.val[0], d2_in3.val[0]); \
    d_sin3_i = vhsub_s16 (d2_in2.val[1], d2_in3.val[1]); \
    d_sin4_r = vhadd_s16 (d2_in4.val[0], d2_in5.val[0]); \
    d_sin4_i = vhadd_s16 (d2_in4.val[1], d2_in5.val[1]); \
    d_sin5_r = vhsub_s16 (d2_in4.val[0], d2_in5.val[0]); \
    d_sin5_i = vhsub_s16 (d2_in4.val[1], d2_in5.val[1]); \
    d_sin6_r = vhadd_s16 (d2_in6.val[0], d2_in7.val[0]); \
    d_sin6_i = vhadd_s16 (d2_in6.val[1], d2_in7.val[1]); \
    d_sin7_r = vhsub_s16 (d2_in6.val[0], d2_in7.val[0]); \
    d_sin7_i = vhsub_s16 (d2_in6.val[1], d2_in7.val[1]);

#define RADIX8x4_LS_02_SCALED \
    d_s8_r = vhadd_s16 (d_sin0_r, d_sin4_r); \
    d_s8_i = vhadd_s16 (d_sin0_i, d_sin4_i); \
    d_s9_r = vhadd_s16 (d_sin1_r, d_s5_r); \
    d_s9_i = vhadd_s16 (d_sin1_i, d_s5_i); \
    d_s10_r = vhsub_s16 (d_sin0_r, d_sin4_r); \
    d_s10_i = vhsub_s16 (d_sin0_i, d_sin4_i); \
    d_s11_r = vhsub_s16 (d_sin1_r, d_s5_r); \
    d_s11_i = vhsub_s16 (d_sin1_i, d_s5_i); \
    d_s12_r = vhadd_s16 (d_sin2_r, d_sin6_r); \
    d_s12_i = vhadd_s16 (d_sin2_i, d_sin6_i); \
    d_s13_r = vhadd_s16 (d_s3_r, d_s7_r); \
    d_s13_i = vhadd_s16 (d_s3_i, d_s7_i); \
    d_s14_r = vhsub_s16 (d_sin2_r, d_sin6_r); \
    d_s14_i = vhsub_s16 (d_sin2_i, d_sin6_i); \
    d_s15_r = vhsub_s16 (d_s3_r, d_s7_r); \
    d_s15_i = vhsub_s16 (d_s3_i, d_s7_i); \
    d_out4_r = vhsub_s16 (d_s8_r, d_s12_r); \
    d_out4_i = vhsub_s16 (d_s8_i, d_s12_i); \
    d_out5_r = vhsub_s16 (d_s9_r, d_s13_r); \
    d_out5_i = vhsub_s16 (d_s9_i, d_s13_i); \
    d_out0_r = vhadd_s16 (d_s8_r, d_s12_r); \
    d_out0_i = vhadd_s16 (d_s8_i, d_s12_i); \
    d_out1_r = vhadd_s16 (d_s9_r, d_s13_r); \
    d_out1_i = vhadd_s16 (d_s9_i, d_s13_i);


static inline void ne10_radix8x4_forward_unscaled_neon (ne10_fft_cpx_int16_t * Fout,
        ne10_fft_cpx_int16_t * Fin,
        ne10_int32_t stride)
{
    RADIX8x4_START

    for (f_count = 0; f_count < stride; f_count += 4)
    {
        RADIX8x4_LOAD
        RADIX8x4_FS_S0


        // radix 4 butterfly without twiddles
        RADIX8x4_FWD_S357
        RADIX8x4_LS_02

        d_out2_r = vadd_s16 (d_s10_r, d_s14_i);
        d_out2_i = vsub_s16 (d_s10_i, d_s14_r);
        d_out3_r = vadd_s16 (d_s11_r, d_s15_i);
        d_out3_i = vsub_s16 (d_s11_i, d_s15_r);
        d_out6_r = vsub_s16 (d_s10_r, d_s14_i);
        d_out6_i = vadd_s16 (d_s10_i, d_s14_r);
        d_out7_r = vsub_s16 (d_s11_r, d_s15_i);
        d_out7_i = vadd_s16 (d_s11_i, d_s15_r);

        RADIX8x4_STORE
    } // f_count
}

static inline void ne10_radix8x4_backward_unscaled_neon (ne10_fft_cpx_int16_t * Fout,
        ne10_fft_cpx_int16_t * Fin,
        ne10_int32_t stride)
{
    RADIX8x4_START

    for (f_count = 0; f_count < stride; f_count += 4)
    {
        RADIX8x4_LOAD
        RADIX8x4_FS_S0

        // radix 4 butterfly without twiddles
        RADIX8x4_INV_S357
        RADIX8x4_LS_02

        d_out2_r = vsub_s16 (d_s10_r, d_s14_i);
        d_out2_i = vadd_s16 (d_s10_i, d_s14_r);
        d_out3_r = vsub_s16 (d_s11_r, d_s15_i);
        d_out3_i = vadd_s16 (d_s11_i, d_s15_r);
        d_out6_r = vadd_s16 (d_s10_r, d_s14_i);
        d_out6_i = vsub_s16 (d_s10_i, d_s14_r);
        d_out7_r = vadd_s16 (d_s11_r, d_s15_i);
        d_out7_i = vsub_s16 (d_s11_i, d_s15_r);

        RADIX8x4_STORE
    } // f_count
}
static inline void ne10_radix8x4_forward_scaled_neon (ne10_fft_cpx_int16_t * Fout,
        ne10_fft_cpx_int16_t * Fin,
        ne10_int32_t stride)
{
    RADIX8x4_START

    for (f_count = 0; f_count < stride; f_count += 4)
    {
        RADIX8x4_LOAD
        RADIX8x4_FS_S0_SCALED

        // radix 4 butterfly without twiddles
        RADIX8x4_FWD_S357
        RADIX8x4_LS_02_SCALED

        d_out2_r = vhadd_s16 (d_s10_r, d_s14_i);
        d_out2_i = vhsub_s16 (d_s10_i, d_s14_r);
        d_out3_r = vhadd_s16 (d_s11_r, d_s15_i);
        d_out3_i = vhsub_s16 (d_s11_i, d_s15_r);
        d_out6_r = vhsub_s16 (d_s10_r, d_s14_i);
        d_out6_i = vhadd_s16 (d_s10_i, d_s14_r);
        d_out7_r = vhsub_s16 (d_s11_r, d_s15_i);
        d_out7_i = vhadd_s16 (d_s11_i, d_s15_r);

        RADIX8x4_STORE
    } // f_count
}

static inline void ne10_radix8x4_backward_scaled_neon (ne10_fft_cpx_int16_t * Fout,
        ne10_fft_cpx_int16_t * Fin,
        ne10_int32_t stride)
{
    RADIX8x4_START

    for (f_count = 0; f_count < stride; f_count += 4)
    {
        RADIX8x4_LOAD
        RADIX8x4_FS_S0_SCALED

        // radix 4 butterfly without twiddles
        RADIX8x4_INV_S357
        RADIX8x4_LS_02_SCALED

        d_out2_r = vhsub_s16 (d_s10_r, d_s14_i);
        d_out2_i = vhadd_s16 (d_s10_i, d_s14_r);
        d_out3_r = vhsub_s16 (d_s11_r, d_s15_i);
        d_out3_i = vhadd_s16 (d_s11_i, d_s15_r);
        d_out6_r = vhadd_s16 (d_s10_r, d_s14_i);
        d_out6_i = vhsub_s16 (d_s10_i, d_s14_r);
        d_out7_r = vhadd_s16 (d_s11_r, d_s15_i);
        d_out7_i = vhsub_s16 (d_s11_i, d_s15_r);

        RADIX8x4_STORE
    } // f_count
}

#define RADIX4x4_WITHOUT_TW_START \
    ne10_int32_t f_count; \
    ne10_int32_t src_step = stride << 1; \
    int16_t *p_src, *p_dst; \
    int16x4x2_t d2_in0, d2_in1, d2_in2, d2_in3; \
    int16x4_t d_s0_r, d_s0_i, d_s1_r, d_s1_i, d_s2_r, d_s2_i, d_s3_r, d_s3_i; \
    int16x4_t d_out0_r, d_out0_i, d_out1_r, d_out1_i, d_out2_r, d_out2_i, d_out3_r, d_out3_i; \
    int16x4x2_t d2_out0, d2_out1, d2_out2, d2_out3; \
    int16x8x2_t q2_tmp0, q2_tmp1; \
    int32x4x2_t q2_tmp2, q2_tmp3; \
    p_src = (int16_t *) Fin; \
    p_dst = (int16_t *) Fout;

#define RADIX4x4_WITHOUT_TW_LOAD \
    d2_in0 = vld2_s16 (p_src); \
    p_src += src_step; \
    d2_in1 = vld2_s16 (p_src); \
    p_src += src_step; \
    d2_in2 = vld2_s16 (p_src); \
    p_src += src_step; \
    d2_in3 = vld2_s16 (p_src); \
    p_src += src_step;

#define RADIX4x4_WITHOUT_TW_STORE \
    q2_tmp0 = vtrnq_s16 (vcombine_s16(d_out0_r, d_out0_i), vcombine_s16(d_out1_r, d_out1_i)); \
    q2_tmp1 = vtrnq_s16 (vcombine_s16(d_out2_r, d_out2_i), vcombine_s16(d_out3_r, d_out3_i)); \
    q2_tmp2 = vtrnq_s32 (vreinterpretq_s32_s16(q2_tmp0.val[0]), vreinterpretq_s32_s16(q2_tmp1.val[0])); \
    q2_tmp3 = vtrnq_s32 (vreinterpretq_s32_s16(q2_tmp0.val[1]), vreinterpretq_s32_s16(q2_tmp1.val[1])); \
    d2_out0.val[0] = vget_low_s16 (vreinterpretq_s16_s32(q2_tmp2.val[0])); \
    d2_out0.val[1] = vget_high_s16 (vreinterpretq_s16_s32(q2_tmp2.val[0])); \
    d2_out1.val[0] = vget_low_s16 (vreinterpretq_s16_s32(q2_tmp3.val[0])); \
    d2_out1.val[1] = vget_high_s16 (vreinterpretq_s16_s32(q2_tmp3.val[0])); \
    d2_out2.val[0] = vget_low_s16 (vreinterpretq_s16_s32(q2_tmp2.val[1])); \
    d2_out2.val[1] = vget_high_s16 (vreinterpretq_s16_s32(q2_tmp2.val[1])); \
    d2_out3.val[0] = vget_low_s16 (vreinterpretq_s16_s32(q2_tmp3.val[1])); \
    d2_out3.val[1] = vget_high_s16 (vreinterpretq_s16_s32(q2_tmp3.val[1])); \
    vst2_s16 (p_dst, d2_out0); \
    p_dst += 8; \
    vst2_s16 (p_dst, d2_out1); \
    p_dst += 8; \
    vst2_s16 (p_dst, d2_out2); \
    p_dst += 8; \
    vst2_s16 (p_dst, d2_out3); \
    p_dst += 8; \
    p_src = p_src - src_step * 4 + 8;

#define RADIX4x4_WITHOUT_TW_S0 \
        d_s0_r = vadd_s16 (d2_in0.val[0], d2_in2.val[0]); \
        d_s0_i = vadd_s16 (d2_in0.val[1], d2_in2.val[1]); \
        d_s1_r = vsub_s16 (d2_in0.val[0], d2_in2.val[0]); \
        d_s1_i = vsub_s16 (d2_in0.val[1], d2_in2.val[1]); \
        d_s2_r = vadd_s16 (d2_in1.val[0], d2_in3.val[0]); \
        d_s2_i = vadd_s16 (d2_in1.val[1], d2_in3.val[1]); \
        d_s3_r = vsub_s16 (d2_in1.val[0], d2_in3.val[0]); \
        d_s3_i = vsub_s16 (d2_in1.val[1], d2_in3.val[1]); \
        d_out2_r = vsub_s16 (d_s0_r, d_s2_r); \
        d_out2_i = vsub_s16 (d_s0_i, d_s2_i); \
        d_out0_r = vadd_s16 (d_s0_r, d_s2_r); \
        d_out0_i = vadd_s16 (d_s0_i, d_s2_i);

#define RADIX4x4_WITHOUT_TW_S0_SCALED \
        d_s0_r = vhadd_s16 (d2_in0.val[0], d2_in2.val[0]); \
        d_s0_i = vhadd_s16 (d2_in0.val[1], d2_in2.val[1]); \
        d_s1_r = vhsub_s16 (d2_in0.val[0], d2_in2.val[0]); \
        d_s1_i = vhsub_s16 (d2_in0.val[1], d2_in2.val[1]); \
        d_s2_r = vhadd_s16 (d2_in1.val[0], d2_in3.val[0]); \
        d_s2_i = vhadd_s16 (d2_in1.val[1], d2_in3.val[1]); \
        d_s3_r = vhsub_s16 (d2_in1.val[0], d2_in3.val[0]); \
        d_s3_i = vhsub_s16 (d2_in1.val[1], d2_in3.val[1]); \
        d_out2_r = vhsub_s16 (d_s0_r, d_s2_r); \
        d_out2_i = vhsub_s16 (d_s0_i, d_s2_i); \
        d_out0_r = vhadd_s16 (d_s0_r, d_s2_r); \
        d_out0_i = vhadd_s16 (d_s0_i, d_s2_i);


static inline void ne10_radix4x4_without_twiddles_forward_unscaled_neon (ne10_fft_cpx_int16_t * Fout,
        ne10_fft_cpx_int16_t * Fin,
        ne10_int32_t stride)
{
    RADIX4x4_WITHOUT_TW_START

    for (f_count = 0; f_count < stride; f_count += 4)
    {
        // load
        RADIX4x4_WITHOUT_TW_LOAD

        // radix 4 butterfly without twiddles
        RADIX4x4_WITHOUT_TW_S0

        d_out1_r = vadd_s16 (d_s1_r, d_s3_i);
        d_out1_i = vsub_s16 (d_s1_i, d_s3_r);
        d_out3_r = vsub_s16 (d_s1_r, d_s3_i);
        d_out3_i = vadd_s16 (d_s1_i, d_s3_r);

        RADIX4x4_WITHOUT_TW_STORE
    }
}

static inline void ne10_radix4x4_without_twiddles_backward_unscaled_neon (ne10_fft_cpx_int16_t * Fout,
        ne10_fft_cpx_int16_t * Fin,
        ne10_int32_t stride)
{
    RADIX4x4_WITHOUT_TW_START

    for (f_count = 0; f_count < stride; f_count += 4)
    {
        // load
        RADIX4x4_WITHOUT_TW_LOAD

        // radix 4 butterfly without twiddles
        RADIX4x4_WITHOUT_TW_S0

        d_out1_r = vsub_s16 (d_s1_r, d_s3_i);
        d_out1_i = vadd_s16 (d_s1_i, d_s3_r);
        d_out3_r = vadd_s16 (d_s1_r, d_s3_i);
        d_out3_i = vsub_s16 (d_s1_i, d_s3_r);

        RADIX4x4_WITHOUT_TW_STORE
    }
}

static inline void ne10_radix4x4_without_twiddles_forward_scaled_neon (ne10_fft_cpx_int16_t * Fout,
        ne10_fft_cpx_int16_t * Fin,
        ne10_int32_t stride)
{
    RADIX4x4_WITHOUT_TW_START

    for (f_count = 0; f_count < stride; f_count += 4)
    {
        // load
        RADIX4x4_WITHOUT_TW_LOAD

        // radix 4 butterfly without twiddles
        RADIX4x4_WITHOUT_TW_S0_SCALED

        d_out1_r = vhadd_s16 (d_s1_r, d_s3_i);
        d_out1_i = vhsub_s16 (d_s1_i, d_s3_r);
        d_out3_r = vhsub_s16 (d_s1_r, d_s3_i);
        d_out3_i = vhadd_s16 (d_s1_i, d_s3_r);

        RADIX4x4_WITHOUT_TW_STORE
    }
}

static inline void ne10_radix4x4_without_twiddles_backward_scaled_neon (ne10_fft_cpx_int16_t * Fout,
        ne10_fft_cpx_int16_t * Fin,
        ne10_int32_t stride)
{
    RADIX4x4_WITHOUT_TW_START

    for (f_count = 0; f_count < stride; f_count += 4)
    {
        // load
        RADIX4x4_WITHOUT_TW_LOAD

        // radix 4 butterfly without twiddles
        RADIX4x4_WITHOUT_TW_S0_SCALED

        d_out1_r = vhsub_s16 (d_s1_r, d_s3_i);
        d_out1_i = vhadd_s16 (d_s1_i, d_s3_r);
        d_out3_r = vhadd_s16 (d_s1_r, d_s3_i);
        d_out3_i = vhsub_s16 (d_s1_i, d_s3_r);

        RADIX4x4_WITHOUT_TW_STORE
    }
}

#define RADIX4x4_WITH_TW_START \
    ne10_int32_t m_count; \
    ne10_int32_t src_step = src_stride << 1; \
    ne10_int32_t dst_step = dst_stride << 1; \
    ne10_int32_t tw_step = mstride << 1; \
    int16_t *p_src, *p_dst, *p_tw; \
    int16x4x2_t d2_in0, d2_in1, d2_in2, d2_in3; \
    int16x4x2_t d2_tw0, d2_tw1, d2_tw2; \
    int16x4_t d_s1_r, d_s1_i, d_s2_r, d_s2_i, d_s3_r, d_s3_i; \
    int16x4_t d_tmp0, d_tmp1, d_tmp2, d_tmp3, d_tmp4, d_tmp5; \
    int16x4_t d_s4_r, d_s4_i, d_s5_r, d_s5_i, d_s6_r, d_s6_i, d_s7_r, d_s7_i; \
    int16x4x2_t d2_out0, d2_out1, d2_out2, d2_out3; \
    p_src = (int16_t *) Fin; \
    p_dst = (int16_t *) Fout; \
    p_tw = (int16_t *) tw;

#define RADIX4x4_WITH_TW_LOAD \
    d2_in0 = vld2_s16 (p_src); \
    p_src += src_step; \
    d2_in1 = vld2_s16 (p_src); \
    p_src += src_step; \
    d2_in2 = vld2_s16 (p_src); \
    p_src += src_step; \
    d2_in3 = vld2_s16 (p_src); \
    p_src += src_step; \
    d2_tw0 = vld2_s16 (p_tw); \
    p_tw += tw_step; \
    d2_tw1 = vld2_s16 (p_tw); \
    p_tw += tw_step; \
    d2_tw2 = vld2_s16 (p_tw); \
    d_s1_r = vqdmulh_s16 (d2_in1.val[0], d2_tw0.val[0]); \
    d_s1_i = vqdmulh_s16 (d2_in1.val[1], d2_tw0.val[0]); \
    d_s2_r = vqdmulh_s16 (d2_in2.val[0], d2_tw1.val[0]); \
    d_s2_i = vqdmulh_s16 (d2_in2.val[1], d2_tw1.val[0]); \
    d_s3_r = vqdmulh_s16 (d2_in3.val[0], d2_tw2.val[0]); \
    d_s3_i = vqdmulh_s16 (d2_in3.val[1], d2_tw2.val[0]); \
    d_tmp0 = vqdmulh_s16 (d2_in1.val[1], d2_tw0.val[1]); \
    d_tmp1 = vqdmulh_s16 (d2_in1.val[0], d2_tw0.val[1]); \
    d_tmp2 = vqdmulh_s16 (d2_in2.val[1], d2_tw1.val[1]); \
    d_tmp3 = vqdmulh_s16 (d2_in2.val[0], d2_tw1.val[1]); \
    d_tmp4 = vqdmulh_s16 (d2_in3.val[1], d2_tw2.val[1]); \
    d_tmp5 = vqdmulh_s16 (d2_in3.val[0], d2_tw2.val[1]);

#define RADIX4x4_WITH_TW_STORE \
    vst2_s16 (p_dst, d2_out0); \
    p_dst += dst_step; \
    vst2_s16 (p_dst, d2_out1); \
    p_dst += dst_step; \
    vst2_s16 (p_dst, d2_out2); \
    p_dst += dst_step; \
    vst2_s16 (p_dst, d2_out3); \
    p_dst += dst_step; \
    p_src = p_src - src_step * 4 + 8; \
    p_dst = p_dst - dst_step * 4 + 8; \
    p_tw = p_tw - tw_step * 2 + 8;

#define RADIX4x4_WITH_TW_S1_FWD \
    d_s1_r = vsub_s16 (d_s1_r, d_tmp0); \
    d_s1_i = vadd_s16 (d_s1_i, d_tmp1); \
    d_s2_r = vsub_s16 (d_s2_r, d_tmp2); \
    d_s2_i = vadd_s16 (d_s2_i, d_tmp3); \
    d_s3_r = vsub_s16 (d_s3_r, d_tmp4); \
    d_s3_i = vadd_s16 (d_s3_i, d_tmp5);

#define RADIX4x4_WITH_TW_S1_INV \
    d_s1_r = vadd_s16 (d_s1_r, d_tmp0); \
    d_s1_i = vsub_s16 (d_s1_i, d_tmp1); \
    d_s2_r = vadd_s16 (d_s2_r, d_tmp2); \
    d_s2_i = vsub_s16 (d_s2_i, d_tmp3); \
    d_s3_r = vadd_s16 (d_s3_r, d_tmp4); \
    d_s3_i = vsub_s16 (d_s3_i, d_tmp5);


#define RADIX4x4_WITH_TW_LS_02 \
    d_s4_r = vadd_s16 (d2_in0.val[0], d_s2_r); \
    d_s4_i = vadd_s16 (d2_in0.val[1], d_s2_i); \
    d_s5_r = vsub_s16 (d2_in0.val[0], d_s2_r); \
    d_s5_i = vsub_s16 (d2_in0.val[1], d_s2_i); \
    d_s6_r = vadd_s16 (d_s1_r, d_s3_r); \
    d_s6_i = vadd_s16 (d_s1_i, d_s3_i); \
    d_s7_r = vsub_s16 (d_s1_r, d_s3_r); \
    d_s7_i = vsub_s16 (d_s1_i, d_s3_i); \
    d2_out2.val[0] = vsub_s16 (d_s4_r, d_s6_r); \
    d2_out2.val[1] = vsub_s16 (d_s4_i, d_s6_i); \
    d2_out0.val[0] = vadd_s16 (d_s4_r, d_s6_r); \
    d2_out0.val[1] = vadd_s16 (d_s4_i, d_s6_i);

#define RADIX4x4_WITH_TW_LS_02_SCALED \
    d_s4_r = vhadd_s16 (d2_in0.val[0], d_s2_r); \
    d_s4_i = vhadd_s16 (d2_in0.val[1], d_s2_i); \
    d_s5_r = vhsub_s16 (d2_in0.val[0], d_s2_r); \
    d_s5_i = vhsub_s16 (d2_in0.val[1], d_s2_i); \
    d_s6_r = vhadd_s16 (d_s1_r, d_s3_r); \
    d_s6_i = vhadd_s16 (d_s1_i, d_s3_i); \
    d_s7_r = vhsub_s16 (d_s1_r, d_s3_r); \
    d_s7_i = vhsub_s16 (d_s1_i, d_s3_i); \
    d2_out2.val[0] = vhsub_s16 (d_s4_r, d_s6_r); \
    d2_out2.val[1] = vhsub_s16 (d_s4_i, d_s6_i); \
    d2_out0.val[0] = vhadd_s16 (d_s4_r, d_s6_r); \
    d2_out0.val[1] = vhadd_s16 (d_s4_i, d_s6_i);


static inline void ne10_radix4x4_with_twiddles_forward_unscaled_neon (ne10_fft_cpx_int16_t * Fout,
        ne10_fft_cpx_int16_t * Fin,
        ne10_fft_cpx_int16_t * tw,
        ne10_int32_t src_stride,
        ne10_int32_t dst_stride,
        ne10_int32_t mstride)
{
    RADIX4x4_WITH_TW_START

    for (m_count = 0; m_count < mstride; m_count += 4)
    {
        // load
        RADIX4x4_WITH_TW_LOAD
        RADIX4x4_WITH_TW_S1_FWD

        RADIX4x4_WITH_TW_LS_02

        d2_out1.val[0] = vadd_s16 (d_s5_r, d_s7_i);
        d2_out1.val[1] = vsub_s16 (d_s5_i, d_s7_r);
        d2_out3.val[0] = vsub_s16 (d_s5_r, d_s7_i);
        d2_out3.val[1] = vadd_s16 (d_s5_i, d_s7_r);

        // store
        RADIX4x4_WITH_TW_STORE
    }
}


static inline void ne10_radix4x4_with_twiddles_backward_unscaled_neon (ne10_fft_cpx_int16_t * Fout,
        ne10_fft_cpx_int16_t * Fin,
        ne10_fft_cpx_int16_t * tw,
        ne10_int32_t src_stride,
        ne10_int32_t dst_stride,
        ne10_int32_t mstride)
{
    RADIX4x4_WITH_TW_START

    for (m_count = 0; m_count < mstride; m_count += 4)
    {
        // load
        RADIX4x4_WITH_TW_LOAD
        RADIX4x4_WITH_TW_S1_INV

        RADIX4x4_WITH_TW_LS_02

        d2_out1.val[0] = vsub_s16 (d_s5_r, d_s7_i);
        d2_out1.val[1] = vadd_s16 (d_s5_i, d_s7_r);
        d2_out3.val[0] = vadd_s16 (d_s5_r, d_s7_i);
        d2_out3.val[1] = vsub_s16 (d_s5_i, d_s7_r);

        // store
        RADIX4x4_WITH_TW_STORE
    }
}



static inline void ne10_radix4x4_with_twiddles_forward_scaled_neon (ne10_fft_cpx_int16_t * Fout,
        ne10_fft_cpx_int16_t * Fin,
        ne10_fft_cpx_int16_t * tw,
        ne10_int32_t src_stride,
        ne10_int32_t dst_stride,
        ne10_int32_t mstride)
{
    RADIX4x4_WITH_TW_START

    for (m_count = 0; m_count < mstride; m_count += 4)
    {
        // load
        RADIX4x4_WITH_TW_LOAD
        RADIX4x4_WITH_TW_S1_FWD

        RADIX4x4_WITH_TW_LS_02_SCALED

        d2_out1.val[0] = vhadd_s16 (d_s5_r, d_s7_i);
        d2_out1.val[1] = vhsub_s16 (d_s5_i, d_s7_r);
        d2_out3.val[0] = vhsub_s16 (d_s5_r, d_s7_i);
        d2_out3.val[1] = vhadd_s16 (d_s5_i, d_s7_r);

        // store
        RADIX4x4_WITH_TW_STORE
    }
}

static inline void ne10_radix4x4_with_twiddles_backward_scaled_neon (ne10_fft_cpx_int16_t * Fout,
        ne10_fft_cpx_int16_t * Fin,
        ne10_fft_cpx_int16_t * tw,
        ne10_int32_t src_stride,
        ne10_int32_t dst_stride,
        ne10_int32_t mstride)
{
    RADIX4x4_WITH_TW_START

    for (m_count = 0; m_count < mstride; m_count += 4)
    {
        // load
        RADIX4x4_WITH_TW_LOAD
        RADIX4x4_WITH_TW_S1_INV

        RADIX4x4_WITH_TW_LS_02_SCALED

        d2_out1.val[0] = vhsub_s16 (d_s5_r, d_s7_i);
        d2_out1.val[1] = vhadd_s16 (d_s5_i, d_s7_r);
        d2_out3.val[0] = vhadd_s16 (d_s5_r, d_s7_i);
        d2_out3.val[1] = vhsub_s16 (d_s5_i, d_s7_r);

        // store
        RADIX4x4_WITH_TW_STORE
    }
}


#define ne10_mixed_radix_fft_forward_int16_neon(scaled) \
void ne10_mixed_radix_fft_forward_int16_##scaled##_neon (ne10_fft_cpx_int16_t * Fout, \
        ne10_fft_cpx_int16_t * Fin, \
        ne10_int32_t * factors, \
        ne10_fft_cpx_int16_t * twiddles, \
        ne10_fft_cpx_int16_t * buffer) \
{ \
    ne10_int32_t fstride, mstride, N; \
    ne10_int32_t fstride1; \
    ne10_int32_t f_count; \
    ne10_int32_t stage_count; \
 \
    ne10_fft_cpx_int16_t   *Fin1, *Fout1; \
    ne10_fft_cpx_int16_t   *Fout_ls = Fout; \
    ne10_fft_cpx_int16_t   *Ftmp; \
    ne10_fft_cpx_int16_t   *tw, *tw1; \
 \
    /* init fstride, mstride, N */ \
    stage_count = factors[0]; \
    fstride = factors[1]; \
    mstride = factors[ (stage_count << 1) - 1 ]; \
    N = factors[ stage_count << 1 ];  \
 \
    /* the first stage */ \
    Fin1 = Fin; \
    Fout1 = Fout; \
    if (N == 2) \
    { \
        N = fstride >> 1;\
        tw = twiddles; \
        fstride1 = fstride >> 2; \
        ne10_radix8x4_forward_##scaled##_neon (Fout, Fin, fstride1);\
 \
        tw += 6; \
        mstride <<= 2; \
        fstride >>= 4; \
        stage_count -= 2; \
 \
        Ftmp = Fin; \
        Fin = Fout; \
        Fout = Ftmp; \
    } \
    else if (N == 4) \
    { \
        ne10_radix4x4_without_twiddles_forward_##scaled##_neon (Fout, Fin, fstride); \
        N = fstride; \
        Ftmp = Fin; \
        Fin = Fout; \
        Fout = Ftmp; \
        /* update address for other stages*/ \
        stage_count--; \
        tw = twiddles; \
        fstride >>= 2; \
    } \
    /* others but the last one*/ \
    for (; stage_count > 1 ; stage_count--) \
    { \
        Fin1 = Fin; \
        for (f_count = 0; f_count < fstride; f_count ++) \
        { \
            Fout1 = & Fout[ f_count * mstride << 2 ]; \
            tw1 = tw; \
            ne10_radix4x4_with_twiddles_forward_##scaled##_neon (Fout1, Fin1, tw1, N, mstride, mstride); \
            Fin1 += mstride; \
        } \
        tw += mstride * 3; \
        mstride <<= 2; \
        Ftmp = Fin; \
        Fin = Fout; \
        Fout = Ftmp; \
        fstride >>= 2; \
    }\
    /* the last one*/ \
    if (stage_count) \
    { \
        Fin1 = Fin; \
        Fout1 = Fout_ls; \
        for (f_count = 0; f_count < fstride; f_count ++) \
        { \
            tw1 = tw; \
            ne10_radix4x4_with_twiddles_forward_##scaled##_neon (Fout1, Fin1, tw1, N, N, mstride); \
            Fin1 += mstride; \
            Fout1 += mstride; \
        } \
    } \
}

#define ne10_mixed_radix_fft_backward_int16_neon(scaled) \
void ne10_mixed_radix_fft_backward_int16_##scaled##_neon (ne10_fft_cpx_int16_t * Fout, \
        ne10_fft_cpx_int16_t * Fin, \
        ne10_int32_t * factors, \
        ne10_fft_cpx_int16_t * twiddles, \
        ne10_fft_cpx_int16_t * buffer) \
{ \
    ne10_int32_t fstride, mstride, N; \
    ne10_int32_t fstride1; \
    ne10_int32_t f_count; \
    ne10_int32_t stage_count; \
 \
    ne10_fft_cpx_int16_t   *Fin1, *Fout1; \
    ne10_fft_cpx_int16_t   *Fout_ls = Fout; \
    ne10_fft_cpx_int16_t   *Ftmp; \
    ne10_fft_cpx_int16_t   *tw, *tw1; \
 \
    /* init fstride, mstride, N */ \
    stage_count = factors[0]; \
    fstride = factors[1]; \
    mstride = factors[ (stage_count << 1) - 1 ]; \
    N = factors[ stage_count << 1 ];  \
 \
    /* the first stage */ \
    Fin1 = Fin; \
    Fout1 = Fout; \
    if (N == 2) \
    { \
        N = fstride >> 1;\
        tw = twiddles; \
        fstride1 = fstride >> 2; \
        ne10_radix8x4_backward_##scaled##_neon (Fout, Fin, fstride1);\
 \
        tw += 6; \
        mstride <<= 2; \
        fstride >>= 4; \
        stage_count -= 2; \
 \
        Ftmp = Fin; \
        Fin = Fout; \
        Fout = Ftmp; \
    } \
    else if (N == 4) \
    { \
        ne10_radix4x4_without_twiddles_backward_##scaled##_neon (Fout, Fin, fstride); \
        N = fstride; \
        Ftmp = Fin; \
        Fin = Fout; \
        Fout = Ftmp; \
        /* update address for other stages*/ \
        stage_count--; \
        tw = twiddles; \
        fstride >>= 2; \
    } \
    /* others but the last one*/ \
    for (; stage_count > 1 ; stage_count--) \
    { \
        Fin1 = Fin; \
        for (f_count = 0; f_count < fstride; f_count ++) \
        { \
            Fout1 = & Fout[ f_count * mstride << 2 ]; \
            tw1 = tw; \
            ne10_radix4x4_with_twiddles_backward_##scaled##_neon (Fout1, Fin1, tw1, N, mstride, mstride); \
            Fin1 += mstride; \
        } \
        tw += mstride * 3; \
        mstride <<= 2; \
        Ftmp = Fin; \
        Fin = Fout; \
        Fout = Ftmp; \
        fstride >>= 2; \
    }\
    /* the last one*/ \
    if (stage_count) \
    { \
        Fin1 = Fin; \
        Fout1 = Fout_ls; \
        for (f_count = 0; f_count < fstride; f_count ++) \
        { \
            tw1 = tw; \
            ne10_radix4x4_with_twiddles_backward_##scaled##_neon (Fout1, Fin1, tw1, N, N, mstride); \
            Fin1 += mstride; \
            Fout1 += mstride; \
        } \
    } \
}


ne10_mixed_radix_fft_forward_int16_neon (unscaled)
ne10_mixed_radix_fft_forward_int16_neon (scaled)
ne10_mixed_radix_fft_backward_int16_neon (unscaled)
ne10_mixed_radix_fft_backward_int16_neon (scaled)


static void ne10_fft_split_r2c_1d_int16_neon (ne10_fft_cpx_int16_t *dst,
        const ne10_fft_cpx_int16_t *src,
        ne10_fft_cpx_int16_t *twiddles,
        ne10_int32_t ncfft,
        ne10_int32_t scaled_flag)
{
    ne10_int32_t k;
    ne10_int32_t count = ncfft / 2;
    ne10_fft_cpx_int16_t fpnk, fpk, f1k, f2k, tw, tdc;
    int16x8x2_t q2_fpk, q2_fpnk, q2_tw, q2_dst, q2_dst2;
    int16x8_t q_fpnk_r, q_fpnk_i;
    int16x8_t q_f1k_r, q_f1k_i, q_f2k_r, q_f2k_i;
    int16x8_t q_tw_r, q_tw_i;
    int16x8_t q_tmp0, q_tmp1, q_tmp2, q_tmp3;
    int16x8_t q_dst2_r, q_dst2_i;
    int16_t *p_src, *p_src2, *p_dst, *p_dst2, *p_twiddles;

    tdc.r = src[0].r;
    tdc.i = src[0].i;

    if (scaled_flag)
        NE10_F2I16_FIXDIV (tdc, 2);

    dst[0].r = tdc.r + tdc.i;
    dst[ncfft].r = tdc.r - tdc.i;
    dst[ncfft].i = dst[0].i = 0;
    if (count >= 8)
    {

        if (scaled_flag)
        {
            for (k = 1; k <= count ; k += 8)
            {
                p_src  = (int16_t*) (& (src[k]));
                p_src2  = (int16_t*) (& (src[ncfft - k - 7]));
                p_twiddles  = (int16_t*) (& (twiddles[k - 1]));
                p_dst  = (int16_t*) (& (dst[k]));
                p_dst2  = (int16_t*) (& (dst[ncfft - k - 7]));

                q2_fpk  = vld2q_s16 (p_src);
                q2_fpnk = vld2q_s16 (p_src2);

                q2_tw = vld2q_s16 (p_twiddles);
                q2_fpnk.val[0] = vrev32q_s16 (q2_fpnk.val[0]);
                q2_fpnk.val[1] = vrev32q_s16 (q2_fpnk.val[1]);
                q2_fpnk.val[0] = vreinterpretq_s16_s32 (vrev64q_s32 (vreinterpretq_s32_s16 (q2_fpnk.val[0])));
                q2_fpnk.val[1] = vreinterpretq_s16_s32 (vrev64q_s32 (vreinterpretq_s32_s16 (q2_fpnk.val[1])));
                q_fpnk_r = vcombine_s16 (vget_high_s16 (q2_fpnk.val[0]), vget_low_s16 (q2_fpnk.val[0]));
                q_fpnk_i = vcombine_s16 (vget_high_s16 (q2_fpnk.val[1]), vget_low_s16 (q2_fpnk.val[1]));
                q_fpnk_i = vnegq_s16 (q_fpnk_i);

                q_f1k_r = vhaddq_s16 (q2_fpk.val[0], q_fpnk_r);
                q_f1k_i = vhaddq_s16 (q2_fpk.val[1], q_fpnk_i);

                q_f2k_r = vhsubq_s16 (q2_fpk.val[0], q_fpnk_r);
                q_f2k_i = vhsubq_s16 (q2_fpk.val[1], q_fpnk_i);

                q_tmp0 = vqdmulhq_s16 (q_f2k_r, q2_tw.val[0]);
                q_tmp1 = vqdmulhq_s16 (q_f2k_i, q2_tw.val[1]);
                q_tmp2 = vqdmulhq_s16 (q_f2k_r, q2_tw.val[1]);
                q_tmp3 = vqdmulhq_s16 (q_f2k_i, q2_tw.val[0]);
                q_tw_r = vsubq_s16 (q_tmp0, q_tmp1);
                q_tw_i = vaddq_s16 (q_tmp2, q_tmp3);

                q_dst2_r = vhsubq_s16 (q_f1k_r, q_tw_r);
                q_dst2_i = vhsubq_s16 (q_tw_i, q_f1k_i);
                q2_dst.val[0] = vhaddq_s16 (q_f1k_r, q_tw_r);
                q2_dst.val[1] = vhaddq_s16 (q_f1k_i, q_tw_i);
                q_dst2_r = vrev32q_s16 (q_dst2_r);
                q_dst2_i = vrev32q_s16 (q_dst2_i);
                q_dst2_r = vreinterpretq_s16_s32 (vrev64q_s32 (vreinterpretq_s32_s16 (q_dst2_r))) ;
                q_dst2_i = vreinterpretq_s16_s32 (vrev64q_s32 (vreinterpretq_s32_s16 (q_dst2_i)));
                q2_dst2.val[0] = vcombine_s16 (vget_high_s16 (q_dst2_r), vget_low_s16 (q_dst2_r));
                q2_dst2.val[1] = vcombine_s16 (vget_high_s16 (q_dst2_i), vget_low_s16 (q_dst2_i));
                vst2q_s16 (p_dst, q2_dst);
                vst2q_s16 (p_dst2, q2_dst2);

            }
        }
        else
        {
            for (k = 1; k <= count ; k += 8)
            {
                p_src  = (int16_t*) (& (src[k]));
                p_src2  = (int16_t*) (& (src[ncfft - k - 7]));
                p_twiddles  = (int16_t*) (& (twiddles[k - 1]));
                p_dst  = (int16_t*) (& (dst[k]));
                p_dst2  = (int16_t*) (& (dst[ncfft - k - 7]));

                q2_fpk  = vld2q_s16 (p_src);
                q2_fpnk = vld2q_s16 (p_src2);

                q2_tw = vld2q_s16 (p_twiddles);
                q2_fpnk.val[0] = vrev32q_s16 (q2_fpnk.val[0]);
                q2_fpnk.val[1] = vrev32q_s16 (q2_fpnk.val[1]);
                q2_fpnk.val[0] = vreinterpretq_s16_s32 (vrev64q_s32 (vreinterpretq_s32_s16 (q2_fpnk.val[0])));
                q2_fpnk.val[1] = vreinterpretq_s16_s32 (vrev64q_s32 (vreinterpretq_s32_s16 (q2_fpnk.val[1])));
                q_fpnk_r = vcombine_s16 (vget_high_s16 (q2_fpnk.val[0]), vget_low_s16 (q2_fpnk.val[0]));
                q_fpnk_i = vcombine_s16 (vget_high_s16 (q2_fpnk.val[1]), vget_low_s16 (q2_fpnk.val[1]));
                q_fpnk_i = vnegq_s16 (q_fpnk_i);

                q_f1k_r = vaddq_s16 (q2_fpk.val[0], q_fpnk_r);
                q_f1k_i = vaddq_s16 (q2_fpk.val[1], q_fpnk_i);

                q_f2k_r = vsubq_s16 (q2_fpk.val[0], q_fpnk_r);
                q_f2k_i = vsubq_s16 (q2_fpk.val[1], q_fpnk_i);

                q_tmp0 = vqdmulhq_s16 (q_f2k_r, q2_tw.val[0]);
                q_tmp1 = vqdmulhq_s16 (q_f2k_i, q2_tw.val[1]);
                q_tmp2 = vqdmulhq_s16 (q_f2k_r, q2_tw.val[1]);
                q_tmp3 = vqdmulhq_s16 (q_f2k_i, q2_tw.val[0]);
                q_tw_r = vsubq_s16 (q_tmp0, q_tmp1);
                q_tw_i = vaddq_s16 (q_tmp2, q_tmp3);

                q_dst2_r = vhsubq_s16 (q_f1k_r, q_tw_r);
                q_dst2_i = vhsubq_s16 (q_tw_i, q_f1k_i);
                q2_dst.val[0] = vhaddq_s16 (q_f1k_r, q_tw_r);
                q2_dst.val[1] = vhaddq_s16 (q_f1k_i, q_tw_i);
                q_dst2_r = vrev32q_s16 (q_dst2_r);
                q_dst2_i = vrev32q_s16 (q_dst2_i);
                q_dst2_r = vreinterpretq_s16_s32 (vrev64q_s32 (vreinterpretq_s32_s16 (q_dst2_r))) ;
                q_dst2_i = vreinterpretq_s16_s32 (vrev64q_s32 (vreinterpretq_s32_s16 (q_dst2_i)));
                q2_dst2.val[0] = vcombine_s16 (vget_high_s16 (q_dst2_r), vget_low_s16 (q_dst2_r));
                q2_dst2.val[1] = vcombine_s16 (vget_high_s16 (q_dst2_i), vget_low_s16 (q_dst2_i));
                vst2q_s16 (p_dst, q2_dst);
                vst2q_s16 (p_dst2, q2_dst2);

            }
        }
    }
    else
    {

        for (k = 1; k <= ncfft / 2 ; ++k)
        {
            fpk    = src[k];
            fpnk.r =   src[ncfft - k].r;
            fpnk.i = - src[ncfft - k].i;
            if (scaled_flag)
            {
                NE10_F2I16_FIXDIV (fpk, 2);
                NE10_F2I16_FIXDIV (fpnk, 2);
            }

            f1k.r = fpk.r + fpnk.r;
            f1k.i = fpk.i + fpnk.i;

            f2k.r = fpk.r - fpnk.r;
            f2k.i = fpk.i - fpnk.i;

            tw.r = (ne10_int16_t) ( ( (NE10_F2I16_SAMPPROD) f2k.r * (twiddles[k - 1]).r
                                      - (NE10_F2I16_SAMPPROD) f2k.i * (twiddles[k - 1]).i) >> NE10_F2I16_SHIFT);
            tw.i = (ne10_int16_t) ( ( (NE10_F2I16_SAMPPROD) f2k.r * (twiddles[k - 1]).i
                                      + (NE10_F2I16_SAMPPROD) f2k.i * (twiddles[k - 1]).r) >> NE10_F2I16_SHIFT);

            dst[k].r = (f1k.r + tw.r) >> 1;
            dst[k].i = (f1k.i + tw.i) >> 1;
            dst[ncfft - k].r = (f1k.r - tw.r) >> 1;
            dst[ncfft - k].i = (tw.i - f1k.i) >> 1;
        }
    }
}

static void ne10_fft_split_c2r_1d_int16_neon (ne10_fft_cpx_int16_t *dst,
        const ne10_fft_cpx_int16_t *src,
        ne10_fft_cpx_int16_t *twiddles,
        ne10_int32_t ncfft,
        ne10_int32_t scaled_flag)
{

    ne10_int32_t k;
    ne10_int32_t count = ncfft / 2;
    ne10_fft_cpx_int16_t fk, fnkc, fek, fok, tmp;
    int16x8x2_t q2_fk, q2_fnkc, q2_tw, q2_dst, q2_dst2;
    int16x8_t q_fnkc_r, q_fnkc_i;
    int16x8_t q_fek_r, q_fek_i, q_fok_r, q_fok_i;
    int16x8_t q_tmp0, q_tmp1, q_tmp2, q_tmp3;
    int16x8_t q_dst2_r, q_dst2_i;
    int16_t *p_src, *p_src2, *p_dst, *p_dst2, *p_twiddles;


    dst[0].r = src[0].r + src[ncfft].r;
    dst[0].i = src[0].r - src[ncfft].r;

    if (scaled_flag)
        NE10_F2I16_FIXDIV (dst[0], 2);
    if (count >= 8)
    {
        if (scaled_flag)
        {
            for (k = 1; k <= count ; k += 8)
            {
                p_src  = (int16_t*) (& (src[k]));
                p_src2  = (int16_t*) (& (src[ncfft - k - 7]));
                p_twiddles  = (int16_t*) (& (twiddles[k - 1]));
                p_dst  = (int16_t*) (& (dst[k]));
                p_dst2  = (int16_t*) (& (dst[ncfft - k - 7]));

                q2_fk  = vld2q_s16 (p_src);
                q2_fnkc = vld2q_s16 (p_src2);
                q2_tw = vld2q_s16 (p_twiddles);
                q2_fnkc.val[0] = vrev32q_s16 (q2_fnkc.val[0]);
                q2_fnkc.val[1] = vrev32q_s16 (q2_fnkc.val[1]);
                q2_fnkc.val[0] = vreinterpretq_s16_s32 (vrev64q_s32 (vreinterpretq_s32_s16 (q2_fnkc.val[0])));
                q2_fnkc.val[1] = vreinterpretq_s16_s32 (vrev64q_s32 (vreinterpretq_s32_s16 (q2_fnkc.val[1])));
                q_fnkc_r = vcombine_s16 (vget_high_s16 (q2_fnkc.val[0]), vget_low_s16 (q2_fnkc.val[0]));
                q_fnkc_i = vcombine_s16 (vget_high_s16 (q2_fnkc.val[1]), vget_low_s16 (q2_fnkc.val[1]));
                q_fnkc_i = vnegq_s16 (q_fnkc_i);

                q_fek_r = vhaddq_s16 (q2_fk.val[0], q_fnkc_r);
                q_fek_i = vhaddq_s16 (q2_fk.val[1], q_fnkc_i);
                q_tmp0 = vhsubq_s16 (q2_fk.val[0], q_fnkc_r);
                q_tmp1 = vhsubq_s16 (q2_fk.val[1], q_fnkc_i);

                q_fok_r = vqdmulhq_s16 (q_tmp0, q2_tw.val[0]);
                q_fok_i = vqdmulhq_s16 (q_tmp1, q2_tw.val[0]);
                q_tmp2 = vqdmulhq_s16 (q_tmp1, q2_tw.val[1]);
                q_tmp3 = vqdmulhq_s16 (q_tmp0, q2_tw.val[1]);
                q_fok_r = vaddq_s16 (q_fok_r, q_tmp2);
                q_fok_i = vsubq_s16 (q_fok_i, q_tmp3);

                q_dst2_r = vsubq_s16 (q_fek_r, q_fok_r);
                q_dst2_i = vsubq_s16 (q_fok_i, q_fek_i);
                q2_dst.val[0] = vaddq_s16 (q_fek_r, q_fok_r);
                q2_dst.val[1] = vaddq_s16 (q_fek_i, q_fok_i);
                q_dst2_r = vrev32q_s16 (q_dst2_r);
                q_dst2_i = vrev32q_s16 (q_dst2_i);
                q_dst2_r = vreinterpretq_s16_s32 (vrev64q_s32 (vreinterpretq_s32_s16 (q_dst2_r))) ;
                q_dst2_i = vreinterpretq_s16_s32 (vrev64q_s32 (vreinterpretq_s32_s16 (q_dst2_i)));
                q2_dst2.val[0] = vcombine_s16 (vget_high_s16 (q_dst2_r), vget_low_s16 (q_dst2_r));
                q2_dst2.val[1] = vcombine_s16 (vget_high_s16 (q_dst2_i), vget_low_s16 (q_dst2_i));
                vst2q_s16 (p_dst, q2_dst);
                vst2q_s16 (p_dst2, q2_dst2);

            }

        }
        else
        {
            for (k = 1; k <= count ; k += 8)
            {
                p_src  = (int16_t*) (& (src[k]));
                p_src2  = (int16_t*) (& (src[ncfft - k - 7]));
                p_twiddles  = (int16_t*) (& (twiddles[k - 1]));
                p_dst  = (int16_t*) (& (dst[k]));
                p_dst2  = (int16_t*) (& (dst[ncfft - k - 7]));

                q2_fk  = vld2q_s16 (p_src);
                q2_fnkc = vld2q_s16 (p_src2);
                q2_tw = vld2q_s16 (p_twiddles);
                q2_fnkc.val[0] = vrev32q_s16 (q2_fnkc.val[0]);
                q2_fnkc.val[1] = vrev32q_s16 (q2_fnkc.val[1]);
                q2_fnkc.val[0] = vreinterpretq_s16_s32 (vrev64q_s32 (vreinterpretq_s32_s16 (q2_fnkc.val[0])));
                q2_fnkc.val[1] = vreinterpretq_s16_s32 (vrev64q_s32 (vreinterpretq_s32_s16 (q2_fnkc.val[1])));
                q_fnkc_r = vcombine_s16 (vget_high_s16 (q2_fnkc.val[0]), vget_low_s16 (q2_fnkc.val[0]));
                q_fnkc_i = vcombine_s16 (vget_high_s16 (q2_fnkc.val[1]), vget_low_s16 (q2_fnkc.val[1]));
                q_fnkc_i = vnegq_s16 (q_fnkc_i);

                q_fek_r = vaddq_s16 (q2_fk.val[0], q_fnkc_r);
                q_fek_i = vaddq_s16 (q2_fk.val[1], q_fnkc_i);
                q_tmp0 = vsubq_s16 (q2_fk.val[0], q_fnkc_r);
                q_tmp1 = vsubq_s16 (q2_fk.val[1], q_fnkc_i);

                q_fok_r = vqdmulhq_s16 (q_tmp0, q2_tw.val[0]);
                q_fok_i = vqdmulhq_s16 (q_tmp1, q2_tw.val[0]);
                q_tmp2 = vqdmulhq_s16 (q_tmp1, q2_tw.val[1]);
                q_tmp3 = vqdmulhq_s16 (q_tmp0, q2_tw.val[1]);
                q_fok_r = vaddq_s16 (q_fok_r, q_tmp2);
                q_fok_i = vsubq_s16 (q_fok_i, q_tmp3);

                q_dst2_r = vsubq_s16 (q_fek_r, q_fok_r);
                q_dst2_i = vsubq_s16 (q_fok_i, q_fek_i);
                q2_dst.val[0] = vaddq_s16 (q_fek_r, q_fok_r);
                q2_dst.val[1] = vaddq_s16 (q_fek_i, q_fok_i);
                q_dst2_r = vrev32q_s16 (q_dst2_r);
                q_dst2_i = vrev32q_s16 (q_dst2_i);
                q_dst2_r = vreinterpretq_s16_s32 (vrev64q_s32 (vreinterpretq_s32_s16 (q_dst2_r))) ;
                q_dst2_i = vreinterpretq_s16_s32 (vrev64q_s32 (vreinterpretq_s32_s16 (q_dst2_i)));
                q2_dst2.val[0] = vcombine_s16 (vget_high_s16 (q_dst2_r), vget_low_s16 (q_dst2_r));
                q2_dst2.val[1] = vcombine_s16 (vget_high_s16 (q_dst2_i), vget_low_s16 (q_dst2_i));
                vst2q_s16 (p_dst, q2_dst);
                vst2q_s16 (p_dst2, q2_dst2);

            }
        }
    }
    else
    {

        for (k = 1; k <= ncfft / 2; k++)
        {
            fk = src[k];
            fnkc.r = src[ncfft - k].r;
            fnkc.i = -src[ncfft - k].i;
            if (scaled_flag)
            {
                NE10_F2I16_FIXDIV (fk, 2);
                NE10_F2I16_FIXDIV (fnkc, 2);
            }

            fek.r = fk.r + fnkc.r;
            fek.i = fk.i + fnkc.i;

            tmp.r = fk.r - fnkc.r;
            tmp.i = fk.i - fnkc.i;

            fok.r = (ne10_int16_t) ( ( (NE10_F2I16_SAMPPROD) tmp.r * (twiddles[k - 1]).r
                                       + (NE10_F2I16_SAMPPROD) tmp.i * (twiddles[k - 1]).i) >> NE10_F2I16_SHIFT);
            fok.i = (ne10_int16_t) ( ( (NE10_F2I16_SAMPPROD) tmp.i * (twiddles[k - 1]).r
                                       - (NE10_F2I16_SAMPPROD) tmp.r * (twiddles[k - 1]).i) >> NE10_F2I16_SHIFT);

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
 * @brief Mixed radix-2/4 complex FFT/IFFT of 16-bit fixed point data.
 * @param[out]  *fout            point to the output buffer (out-of-place)
 * @param[in]   *fin             point to the input buffer (out-of-place)
 * @param[in]   cfg              point to the config struct
 * @param[in]   inverse_fft      the flag of IFFT, 0: FFT, 1: IFFT
 * @param[in]   scaled_flag      scale flag, 0 unscaled, 1 scaled
 * @return none.
 * The function implements a mixed radix-2/4 complex FFT/IFFT. The length of 2^N(N is 1, 2, 3, 4, 5, 6 ....etc) is supported.
 * Otherwise, this FFT is an out-of-place algorithm. When you want to get an in-place FFT, it creates a temp buffer as
 *  output buffer and then copies the temp buffer back to input buffer. For the usage of this function, please check test/test_suite_fft_int16.c
 */
void ne10_fft_c2c_1d_int16_neon (ne10_fft_cpx_int16_t *fout,
                                 ne10_fft_cpx_int16_t *fin,
                                 ne10_fft_cfg_int16_t cfg,
                                 ne10_int32_t inverse_fft,
                                 ne10_int32_t scaled_flag)
{
    if (scaled_flag)
    {
        if (inverse_fft)
        {
            switch (cfg->nfft)
            {
            case 4:
                ne10_fft4_backward_int16_scaled (fout, fin);
                break;
            case 8:
                ne10_fft8_backward_int16_scaled (fout, fin);
                break;
            default:
                ne10_mixed_radix_fft_backward_int16_scaled_neon (fout, fin, cfg->factors, cfg->twiddles, cfg->buffer);
                break;
            }
        }
        else
        {
            switch (cfg->nfft)
            {
            case 4:
                ne10_fft4_forward_int16_scaled (fout, fin);
                break;
            case 8:
                ne10_fft8_forward_int16_scaled (fout, fin);
                break;
            default:
                ne10_mixed_radix_fft_forward_int16_scaled_neon (fout, fin, cfg->factors, cfg->twiddles, cfg->buffer);
                break;
            }
        }
    }
    else
    {
        if (inverse_fft)
        {
            switch (cfg->nfft)
            {
            case 4:
                ne10_fft4_backward_int16_unscaled (fout, fin);
                break;
            case 8:
                ne10_fft8_backward_int16_unscaled (fout, fin);
                break;
            default:
                ne10_mixed_radix_fft_backward_int16_unscaled_neon (fout, fin, cfg->factors, cfg->twiddles, cfg->buffer);
                break;
            }
        }
        else
        {
            switch (cfg->nfft)
            {
            case 4:
                ne10_fft4_forward_int16_unscaled (fout, fin);
                break;
            case 8:
                ne10_fft8_forward_int16_unscaled (fout, fin);
                break;
            default:
                ne10_mixed_radix_fft_forward_int16_unscaled_neon (fout, fin, cfg->factors, cfg->twiddles, cfg->buffer);
                break;
            }
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
 * @brief Mixed radix-2/4 real-to-complex NEON FFT of 16-bit fixed point data.
 * @param[out]  *fout            point to the output buffer
 * @param[in]   *fin             point to the input buffer
 * @param[in]   cfg              point to the config struct
 * @param[in]   scaled_flag      scale flag, 0 unscaled, 1 scaled
 * @return none.
 * The function implements a mixed radix-2/4 FFT (real to complex). The length of 2^N(N is 2, 3, 4, 5, 6 ....etc) is supported.
 * Otherwise, we alloc a temp buffer(the size is same as input buffer) for storing intermedia.
 * For the usage of this function, please check test/test_suite_fft_int16.c
 */
void ne10_fft_r2c_1d_int16_neon (ne10_fft_cpx_int16_t *fout,
                                 ne10_int16_t *fin,
                                 ne10_fft_r2c_cfg_int16_t cfg,
                                 ne10_int32_t scaled_flag)
{
    ne10_fft_cpx_int16_t * tmpbuf1 = cfg->buffer;
    ne10_fft_cpx_int16_t * tmpbuf2 = cfg->buffer + cfg->ncfft;
    ne10_fft_state_int16_t c2c_state;

    c2c_state.nfft = cfg->ncfft;
    c2c_state.factors = cfg->factors;
    c2c_state.twiddles = cfg->twiddles;
    c2c_state.buffer = tmpbuf2;

    ne10_fft_c2c_1d_int16_neon (tmpbuf1, (ne10_fft_cpx_int16_t*) fin, &c2c_state, 0, scaled_flag);
    ne10_fft_split_r2c_1d_int16_neon (fout, tmpbuf1,  cfg->super_twiddles, cfg->ncfft, scaled_flag);
}

/**
 * @brief Mixed radix-2/4 complex-to-real NEON IFFT of 16-bit fixed point data.
 * @param[out]  *fout            point to the output buffer
 * @param[in]   *fin             point to the input buffer
 * @param[in]   cfg              point to the config struct
 * @param[in]   scaled_flag      scale flag, 0 unscaled, 1 scaled
 * @return none.
 * The function implements a mixed radix-2/4 FFT (complex to real). The length of 2^N(N is 2, 3, 4, 5, 6 ....etc) is supported.
 * Otherwise, we alloc a temp buffer(the size is same as input buffer) for storing intermedia.
 * For the usage of this function, please check test/test_suite_fft_int16.c
 */
void ne10_fft_c2r_1d_int16_neon (ne10_int16_t *fout,
                                 ne10_fft_cpx_int16_t *fin,
                                 ne10_fft_r2c_cfg_int16_t cfg,
                                 ne10_int32_t scaled_flag)
{
    ne10_fft_cpx_int16_t * tmpbuf1 = cfg->buffer;
    ne10_fft_cpx_int16_t * tmpbuf2 = cfg->buffer + cfg->ncfft;
    ne10_fft_state_int16_t c2c_state;

    c2c_state.nfft = cfg->ncfft;
    c2c_state.factors = cfg->factors;
    c2c_state.twiddles = cfg->twiddles;
    c2c_state.buffer = tmpbuf2;

    ne10_fft_split_c2r_1d_int16_neon (tmpbuf1, fin, cfg->super_twiddles, cfg->ncfft, scaled_flag);
    ne10_fft_c2c_1d_int16_neon ( (ne10_fft_cpx_int16_t*) fout, tmpbuf1, &c2c_state, 1, scaled_flag);
}

/**
 * @} end of R2C_FFT_IFFT group
 */
