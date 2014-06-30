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
 * NE10 Library : dsp/NE10_fft_int32.neon.c
 */

#include <arm_neon.h>

#include "NE10_types.h"
#include "NE10_macros.h"
#include "NE10_fft.h"

#define FFT4_FS_START \
    ne10_int32_t s0_r, s0_i, s1_r, s1_i, s2_r, s2_i; \
    ne10_int32_t tmp_r, tmp_i;


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
    Fout[2].r = (tmp_r - s0_r) >> 2; \
    Fout[2].i = (tmp_i - s0_i) >> 2; \
    Fout[0].r = (tmp_r + s0_r) >> 2; \
    Fout[0].i = (tmp_i + s0_i) >> 2; \
    Fout[1].r = (s2_r - s1_i) >> 2; \
    Fout[1].i = (s2_i + s1_r) >> 2; \
    Fout[3].r = (s2_r + s1_i) >> 2; \
    Fout[3].i = (s2_i - s1_r) >> 2;

static inline void ne10_fft4_forward_int32_unscaled (ne10_fft_cpx_int32_t * Fout,
        ne10_fft_cpx_int32_t * Fin)

{
    FFT4_FS_START
    FFT4_FS
    FFT4_FWD_LS
}

static inline void ne10_fft4_backward_int32_unscaled (ne10_fft_cpx_int32_t * Fout,
        ne10_fft_cpx_int32_t * Fin)

{
    FFT4_FS_START
    FFT4_FS
    FFT4_INV_LS
}
static inline void ne10_fft4_forward_int32_scaled (ne10_fft_cpx_int32_t * Fout,
        ne10_fft_cpx_int32_t * Fin)

{
    FFT4_FS_START
    FFT4_FS_SCALED
    FFT4_FWD_LS
}

static inline void ne10_fft4_backward_int32_scaled (ne10_fft_cpx_int32_t * Fout,
        ne10_fft_cpx_int32_t * Fin)

{
    FFT4_FS_START
    FFT4_FS_SCALED
    FFT4_INV_LS
}

#define FFT8_FS_START \
    ne10_int32_t s0_r, s0_i, s1_r, s1_i, s2_r, s2_i, s3_r, s3_i, s4_r, s4_i, s5_r, s5_i, s6_r, s6_i, s7_r, s7_i; \
    ne10_int32_t t0_r, t0_i, t1_r, t1_i, t2_r, t2_i, t3_r, t3_i, t4_r, t4_i, t5_r, t5_i; \
    const ne10_int32_t TW_81 = 1518500249;

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
    t4_r = (ne10_int32_t) ( ( (ne10_int64_t) (s3_r + s3_i) * TW_81) >> 31); \
    t4_i = - (ne10_int32_t) ( ( (ne10_int64_t) (s3_r - s3_i) * TW_81) >> 31); \
    t5_r = (ne10_int32_t) ( ( (ne10_int64_t) (s7_r - s7_i) * TW_81) >> 31); \
    t5_i = (ne10_int32_t) ( ( (ne10_int64_t) (s7_r + s7_i) * TW_81) >> 31); \
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
    Fout[0].r = (t1_r + t2_r) >> 3; \
    Fout[0].i = (t1_i + t2_i) >> 3; \
    Fout[4].r = (t1_r - t2_r) >> 3; \
    Fout[4].i = (t1_i - t2_i) >> 3; \
    Fout[2].r = (t0_r - t3_i) >> 3; \
    Fout[2].i = (t0_i + t3_r) >> 3; \
    Fout[6].r = (t0_r + t3_i) >> 3; \
    Fout[6].i = (t0_i - t3_r) >> 3; \
    t4_r = (ne10_int32_t) ( ( (ne10_int64_t) (s3_r - s3_i) * TW_81) >> 31); \
    t4_i = (ne10_int32_t) ( ( (ne10_int64_t) (s3_r + s3_i) * TW_81) >> 31); \
    t5_r = (ne10_int32_t) ( ( (ne10_int64_t) (s7_r + s7_i) * TW_81) >> 31); \
    t5_i = - (ne10_int32_t) ( ( (ne10_int64_t) (s7_r - s7_i) * TW_81) >> 31); \
    t0_r = s1_r + s5_i; \
    t0_i = s1_i - s5_r; \
    t1_r = s1_r - s5_i; \
    t1_i = s1_i + s5_r; \
    t2_r = t4_r - t5_r; \
    t2_i = t4_i - t5_i; \
    t3_r = t4_r + t5_r; \
    t3_i = t4_i + t5_i; \
    Fout[1].r = (t1_r + t2_r) >> 3; \
    Fout[1].i = (t1_i + t2_i) >> 3; \
    Fout[5].r = (t1_r - t2_r) >> 3; \
    Fout[5].i = (t1_i - t2_i) >> 3; \
    Fout[3].r = (t0_r - t3_i) >> 3; \
    Fout[3].i = (t0_i + t3_r) >> 3; \
    Fout[7].r = (t0_r + t3_i) >> 3; \
    Fout[7].i = (t0_i - t3_r) >> 3;

static inline void ne10_fft8_forward_int32_unscaled (ne10_fft_cpx_int32_t * Fout,
        ne10_fft_cpx_int32_t * Fin)

{
    FFT8_FS_START
    FFT8_FS
    FFT8_FWD_LS
}

static inline void ne10_fft8_backward_int32_unscaled (ne10_fft_cpx_int32_t * Fout,
        ne10_fft_cpx_int32_t * Fin)

{
    FFT8_FS_START
    FFT8_FS
    FFT8_INV_LS
}
static inline void ne10_fft8_forward_int32_scaled (ne10_fft_cpx_int32_t * Fout,
        ne10_fft_cpx_int32_t * Fin)

{
    FFT8_FS_START
    FFT8_FS_SCALED
    FFT8_FWD_LS
}

static inline void ne10_fft8_backward_int32_scaled (ne10_fft_cpx_int32_t * Fout,
        ne10_fft_cpx_int32_t * Fin)

{
    FFT8_FS_START
    FFT8_FS_SCALED
    FFT8_INV_LS
}
#define FFT16_FS_START \
    ne10_fft_cpx_int32_t *tw1, *tw2, *tw3; \
    int32_t *p_src0, *p_src4, *p_src8, *p_src12; \
    int32x4x2_t q2_in_0123, q2_in_4567, q2_in_89ab, q2_in_cdef; \
    int32x4_t q_t0_r,  q_t0_i, q_t1_r,  q_t1_i, q_t2_r,  q_t2_i, q_t3_r, q_t3_i; \
    int32x4_t q_out_r048c,  q_out_i048c, q_out_r159d,  q_out_i159d; \
    int32x4_t q_out_r26ae,  q_out_i26ae, q_out_r37bf,  q_out_i37bf;

#define FFT16_LS_START \
    int32_t *p_dst0, *p_dst1, *p_dst2, *p_dst3; \
    int32_t *p_tw1, *p_tw2, *p_tw3; \
    int32x4_t q_s0_r, q_s0_i, q_s1_r, q_s1_i, q_s2_r, q_s2_i; \
    int32x4_t q_s3_r, q_s3_i, q_s4_r, q_s4_i, q_s5_r, q_s5_i; \
    int32x4x2_t q2_tmp_0, q2_tmp_1, q2_tmp_2, q2_tmp_3; \
    int32x4_t q_in_r0123, q_in_r4567, q_in_r89ab, q_in_rcdef; \
    int32x4_t q_in_i0123, q_in_i4567, q_in_i89ab, q_in_icdef; \
    int32x4x2_t q2_tw1, q2_tw2, q2_tw3; \
    int32x4_t q_tmp0, q_tmp1, q_tmp2, q_tmp3, q_tmp4, q_tmp5; \
    int32x4x2_t q2_out_0123, q2_out_4567, q2_out_89ab, q2_out_cdef;

#define FFT16_FS \
    p_src0 = (int32_t*) (& (Fin[0])); \
    p_src4 = (int32_t*) (& (Fin[4])); \
    p_src8 = (int32_t*) (& (Fin[8])); \
    p_src12 = (int32_t*) (& (Fin[12])); \
    q2_in_0123 = vld2q_s32 (p_src0); \
    q2_in_4567 = vld2q_s32 (p_src4); \
    q2_in_89ab = vld2q_s32 (p_src8); \
    q2_in_cdef = vld2q_s32 (p_src12); \
    q_t2_r = vsubq_s32 (q2_in_0123.val[0], q2_in_89ab.val[0]); \
    q_t2_i = vsubq_s32 (q2_in_0123.val[1], q2_in_89ab.val[1]); \
    q_t3_r = vaddq_s32 (q2_in_0123.val[0], q2_in_89ab.val[0]); \
    q_t3_i = vaddq_s32 (q2_in_0123.val[1], q2_in_89ab.val[1]); \
    q_t0_r = vaddq_s32 (q2_in_4567.val[0], q2_in_cdef.val[0]); \
    q_t0_i = vaddq_s32 (q2_in_4567.val[1], q2_in_cdef.val[1]); \
    q_t1_r = vsubq_s32 (q2_in_4567.val[0], q2_in_cdef.val[0]); \
    q_t1_i = vsubq_s32 (q2_in_4567.val[1], q2_in_cdef.val[1]); \
    q_out_r26ae = vsubq_s32 (q_t3_r, q_t0_r); \
    q_out_i26ae = vsubq_s32 (q_t3_i, q_t0_i); \
    q_out_r048c = vaddq_s32 (q_t3_r, q_t0_r); \
    q_out_i048c = vaddq_s32 (q_t3_i, q_t0_i);

#define FFT16_FS_SCALED \
    p_src0 = (int32_t*) (& (Fin[0])); \
    p_src4 = (int32_t*) (& (Fin[4])); \
    p_src8 = (int32_t*) (& (Fin[8])); \
    p_src12 = (int32_t*) (& (Fin[12])); \
    q2_in_0123 = vld2q_s32 (p_src0); \
    q2_in_4567 = vld2q_s32 (p_src4); \
    q2_in_89ab = vld2q_s32 (p_src8); \
    q2_in_cdef = vld2q_s32 (p_src12); \
    q_t2_r = vhsubq_s32 (q2_in_0123.val[0], q2_in_89ab.val[0]); \
    q_t2_i = vhsubq_s32 (q2_in_0123.val[1], q2_in_89ab.val[1]); \
    q_t3_r = vhaddq_s32 (q2_in_0123.val[0], q2_in_89ab.val[0]); \
    q_t3_i = vhaddq_s32 (q2_in_0123.val[1], q2_in_89ab.val[1]); \
    q_t0_r = vhaddq_s32 (q2_in_4567.val[0], q2_in_cdef.val[0]); \
    q_t0_i = vhaddq_s32 (q2_in_4567.val[1], q2_in_cdef.val[1]); \
    q_t1_r = vhsubq_s32 (q2_in_4567.val[0], q2_in_cdef.val[0]); \
    q_t1_i = vhsubq_s32 (q2_in_4567.val[1], q2_in_cdef.val[1]); \
    q_out_r26ae = vhsubq_s32 (q_t3_r, q_t0_r); \
    q_out_i26ae = vhsubq_s32 (q_t3_i, q_t0_i); \
    q_out_r048c = vhaddq_s32 (q_t3_r, q_t0_r); \
    q_out_i048c = vhaddq_s32 (q_t3_i, q_t0_i);

#define FFT16_LS_LOAD \
    tw1 = twiddles; \
    tw2 = twiddles + 4; \
    tw3 = twiddles + 8; \
    p_dst0 = (int32_t*) (&Fout[0]); \
    p_dst1 = (int32_t*) (&Fout[4]); \
    p_dst2 = (int32_t*) (&Fout[8]); \
    p_dst3 = (int32_t*) (&Fout[12]); \
    p_tw1 = (int32_t*) tw1; \
    p_tw2 = (int32_t*) tw2; \
    p_tw3 = (int32_t*) tw3; \
    q2_tmp_0 = vzipq_s32 (q_out_r048c, q_out_r159d); \
    q2_tmp_1 = vzipq_s32 (q_out_i048c, q_out_i159d); \
    q2_tmp_2 = vzipq_s32 (q_out_r26ae, q_out_r37bf); \
    q2_tmp_3 = vzipq_s32 (q_out_i26ae, q_out_i37bf); \
    q_in_r0123 = vcombine_s32 (vget_low_s32 (q2_tmp_0.val[0]), vget_low_s32 (q2_tmp_2.val[0])); \
    q_in_i0123 = vcombine_s32 (vget_low_s32 (q2_tmp_1.val[0]), vget_low_s32 (q2_tmp_3.val[0])); \
    q_in_r4567 = vcombine_s32 (vget_high_s32 (q2_tmp_0.val[0]), vget_high_s32 (q2_tmp_2.val[0])); \
    q_in_i4567 = vcombine_s32 (vget_high_s32 (q2_tmp_1.val[0]), vget_high_s32 (q2_tmp_3.val[0])); \
    q_in_r89ab = vcombine_s32 (vget_low_s32 (q2_tmp_0.val[1]), vget_low_s32 (q2_tmp_2.val[1])); \
    q_in_i89ab = vcombine_s32 (vget_low_s32 (q2_tmp_1.val[1]), vget_low_s32 (q2_tmp_3.val[1])); \
    q_in_rcdef = vcombine_s32 (vget_high_s32 (q2_tmp_0.val[1]), vget_high_s32 (q2_tmp_2.val[1])); \
    q_in_icdef = vcombine_s32 (vget_high_s32 (q2_tmp_1.val[1]), vget_high_s32 (q2_tmp_3.val[1])); \
    q2_tw1 = vld2q_s32 (p_tw1); \
    q2_tw2 = vld2q_s32 (p_tw2); \
    q2_tw3 = vld2q_s32 (p_tw3);

#define FFT16_FWD_LS \
    q_s0_r = vqrdmulhq_s32 (q_in_r4567, q2_tw1.val[0]); \
    q_s0_i = vqrdmulhq_s32 (q_in_r4567, q2_tw1.val[1]); \
    q_s1_r = vqrdmulhq_s32 (q_in_r89ab, q2_tw2.val[0]); \
    q_s1_i = vqrdmulhq_s32 (q_in_r89ab, q2_tw2.val[1]); \
    q_s2_r = vqrdmulhq_s32 (q_in_rcdef, q2_tw3.val[0]); \
    q_s2_i = vqrdmulhq_s32 (q_in_rcdef, q2_tw3.val[1]); \
    q_tmp0 = vqrdmulhq_s32 (q_in_i4567, q2_tw1.val[1]); \
    q_tmp1 = vqrdmulhq_s32 (q_in_i4567, q2_tw1.val[0]); \
    q_tmp2 = vqrdmulhq_s32 (q_in_i89ab, q2_tw2.val[1]); \
    q_tmp3 = vqrdmulhq_s32 (q_in_i89ab, q2_tw2.val[0]); \
    q_tmp4 = vqrdmulhq_s32 (q_in_icdef, q2_tw3.val[1]); \
    q_tmp5 = vqrdmulhq_s32 (q_in_icdef, q2_tw3.val[0]);

#define FFT16_INV_LS \
    q_s0_r = vqrdmulhq_s32 (q_in_r4567, q2_tw1.val[0]); \
    q_s0_i = vqrdmulhq_s32 (q_in_i4567, q2_tw1.val[0]); \
    q_s1_r = vqrdmulhq_s32 (q_in_r89ab, q2_tw2.val[0]); \
    q_s1_i = vqrdmulhq_s32 (q_in_i89ab, q2_tw2.val[0]); \
    q_s2_r = vqrdmulhq_s32 (q_in_rcdef, q2_tw3.val[0]); \
    q_s2_i = vqrdmulhq_s32 (q_in_icdef, q2_tw3.val[0]); \
    q_tmp0 = vqrdmulhq_s32 (q_in_i4567, q2_tw1.val[1]); \
    q_tmp1 = vqrdmulhq_s32 (q_in_r4567, q2_tw1.val[1]); \
    q_tmp2 = vqrdmulhq_s32 (q_in_i89ab, q2_tw2.val[1]); \
    q_tmp3 = vqrdmulhq_s32 (q_in_r89ab, q2_tw2.val[1]); \
    q_tmp4 = vqrdmulhq_s32 (q_in_icdef, q2_tw3.val[1]); \
    q_tmp5 = vqrdmulhq_s32 (q_in_rcdef, q2_tw3.val[1]);

#define FFT16_FWD_LS_S0 \
    q_s0_r = vsubq_s32 (q_s0_r, q_tmp0); \
    q_s0_i = vaddq_s32 (q_s0_i, q_tmp1); \
    q_s1_r = vsubq_s32 (q_s1_r, q_tmp2); \
    q_s1_i = vaddq_s32 (q_s1_i, q_tmp3); \
    q_s2_r = vsubq_s32 (q_s2_r, q_tmp4); \
    q_s2_i = vaddq_s32 (q_s2_i, q_tmp5);

#define FFT16_INV_LS_S0 \
    q_s0_r = vaddq_s32 (q_s0_r, q_tmp0); \
    q_s0_i = vsubq_s32 (q_s0_i, q_tmp1); \
    q_s1_r = vaddq_s32 (q_s1_r, q_tmp2); \
    q_s1_i = vsubq_s32 (q_s1_i, q_tmp3); \
    q_s2_r = vaddq_s32 (q_s2_r, q_tmp4); \
    q_s2_i = vsubq_s32 (q_s2_i, q_tmp5);

#define FFT16_LS_02 \
    q_s5_r = vsubq_s32 (q_in_r0123, q_s1_r); \
    q_s5_i = vsubq_s32 (q_in_i0123, q_s1_i); \
    q2_out_0123.val[0] = vaddq_s32 (q_in_r0123, q_s1_r); \
    q2_out_0123.val[1] = vaddq_s32 (q_in_i0123, q_s1_i); \
    q_s3_r = vaddq_s32 (q_s0_r, q_s2_r); \
    q_s3_i = vaddq_s32 (q_s0_i, q_s2_i); \
    q_s4_r = vsubq_s32 (q_s0_r, q_s2_r); \
    q_s4_i = vsubq_s32 (q_s0_i, q_s2_i); \
    q2_out_89ab.val[0] = vsubq_s32 (q2_out_0123.val[0], q_s3_r); \
    q2_out_89ab.val[1] = vsubq_s32 (q2_out_0123.val[1], q_s3_i); \
    q2_out_0123.val[0] = vaddq_s32 (q2_out_0123.val[0], q_s3_r); \
    q2_out_0123.val[1] = vaddq_s32 (q2_out_0123.val[1], q_s3_i);


#define FFT16_LS_02_SCALED \
    q_s5_r = vhsubq_s32 (q_in_r0123, q_s1_r); \
    q_s5_i = vhsubq_s32 (q_in_i0123, q_s1_i); \
    q2_out_0123.val[0] = vhaddq_s32 (q_in_r0123, q_s1_r); \
    q2_out_0123.val[1] = vhaddq_s32 (q_in_i0123, q_s1_i); \
    q_s3_r = vhaddq_s32 (q_s0_r, q_s2_r); \
    q_s3_i = vhaddq_s32 (q_s0_i, q_s2_i); \
    q_s4_r = vhsubq_s32 (q_s0_r, q_s2_r); \
    q_s4_i = vhsubq_s32 (q_s0_i, q_s2_i); \
    q2_out_89ab.val[0] = vhsubq_s32 (q2_out_0123.val[0], q_s3_r); \
    q2_out_89ab.val[1] = vhsubq_s32 (q2_out_0123.val[1], q_s3_i); \
    q2_out_0123.val[0] = vhaddq_s32 (q2_out_0123.val[0], q_s3_r); \
    q2_out_0123.val[1] = vhaddq_s32 (q2_out_0123.val[1], q_s3_i);

#define FFT16_ST \
    vst2q_s32 (p_dst0, q2_out_0123); \
    vst2q_s32 (p_dst1, q2_out_4567); \
    vst2q_s32 (p_dst2, q2_out_89ab); \
    vst2q_s32 (p_dst3, q2_out_cdef);

static void ne10_fft16_forward_int32_unscaled_neon (ne10_fft_cpx_int32_t * Fout,
        ne10_fft_cpx_int32_t * Fin,
        ne10_fft_cpx_int32_t * twiddles)
{
    // the first stage
    FFT16_FS_START
    FFT16_FS
    q_out_r159d = vaddq_s32 (q_t2_r, q_t1_i);
    q_out_i159d = vsubq_s32 (q_t2_i, q_t1_r);
    q_out_r37bf = vsubq_s32 (q_t2_r, q_t1_i);
    q_out_i37bf = vaddq_s32 (q_t2_i, q_t1_r);

    // second stages
    FFT16_LS_START
    FFT16_LS_LOAD
    FFT16_FWD_LS
    FFT16_FWD_LS_S0
    FFT16_LS_02

    q2_out_4567.val[0] = vaddq_s32 (q_s5_r, q_s4_i);
    q2_out_4567.val[1] = vsubq_s32 (q_s5_i, q_s4_r);
    q2_out_cdef.val[0] = vsubq_s32 (q_s5_r, q_s4_i);
    q2_out_cdef.val[1] = vaddq_s32 (q_s5_i, q_s4_r);

    FFT16_ST
}

static void ne10_fft16_backward_int32_unscaled_neon (ne10_fft_cpx_int32_t * Fout,
        ne10_fft_cpx_int32_t * Fin,
        ne10_fft_cpx_int32_t * twiddles)
{
    // the first stage
    FFT16_FS_START
    FFT16_FS
    q_out_r159d = vsubq_s32 (q_t2_r, q_t1_i);
    q_out_i159d = vaddq_s32 (q_t2_i, q_t1_r);
    q_out_r37bf = vaddq_s32 (q_t2_r, q_t1_i);
    q_out_i37bf = vsubq_s32 (q_t2_i, q_t1_r);

    // second stages
    FFT16_LS_START
    FFT16_LS_LOAD
    FFT16_INV_LS
    FFT16_INV_LS_S0
    FFT16_LS_02

    q2_out_4567.val[0] = vsubq_s32 (q_s5_r, q_s4_i);
    q2_out_4567.val[1] = vaddq_s32 (q_s5_i, q_s4_r);
    q2_out_cdef.val[0] = vaddq_s32 (q_s5_r, q_s4_i);
    q2_out_cdef.val[1] = vsubq_s32 (q_s5_i, q_s4_r);

    q2_out_89ab.val[0] = vshrq_n_s32 (q2_out_89ab.val[0], 4);
    q2_out_89ab.val[1] = vshrq_n_s32 (q2_out_89ab.val[1], 4);
    q2_out_0123.val[0] = vshrq_n_s32 (q2_out_0123.val[0], 4);
    q2_out_0123.val[1] = vshrq_n_s32 (q2_out_0123.val[1], 4);
    q2_out_4567.val[0] = vshrq_n_s32 (q2_out_4567.val[0], 4);
    q2_out_4567.val[1] = vshrq_n_s32 (q2_out_4567.val[1], 4);
    q2_out_cdef.val[0] = vshrq_n_s32 (q2_out_cdef.val[0], 4);
    q2_out_cdef.val[1] = vshrq_n_s32 (q2_out_cdef.val[1], 4);

    FFT16_ST
}

static void ne10_fft16_forward_int32_scaled_neon (ne10_fft_cpx_int32_t * Fout,
        ne10_fft_cpx_int32_t * Fin,
        ne10_fft_cpx_int32_t * twiddles)
{
    // the first stage
    FFT16_FS_START
    FFT16_FS_SCALED
    q_out_r159d = vhaddq_s32 (q_t2_r, q_t1_i);
    \
    q_out_i159d = vhsubq_s32 (q_t2_i, q_t1_r);
    \
    q_out_r37bf = vhsubq_s32 (q_t2_r, q_t1_i);
    \
    q_out_i37bf = vhaddq_s32 (q_t2_i, q_t1_r);
    \

    // second stages
    FFT16_LS_START
    FFT16_LS_LOAD
    FFT16_FWD_LS
    FFT16_FWD_LS_S0
    FFT16_LS_02_SCALED

    q2_out_4567.val[0] = vhaddq_s32 (q_s5_r, q_s4_i);
    q2_out_4567.val[1] = vhsubq_s32 (q_s5_i, q_s4_r);
    q2_out_cdef.val[0] = vhsubq_s32 (q_s5_r, q_s4_i);
    q2_out_cdef.val[1] = vhaddq_s32 (q_s5_i, q_s4_r);

    FFT16_ST
}

static void ne10_fft16_backward_int32_scaled_neon (ne10_fft_cpx_int32_t * Fout,
        ne10_fft_cpx_int32_t * Fin,
        ne10_fft_cpx_int32_t * twiddles)
{
    // the first stage
    FFT16_FS_START
    FFT16_FS_SCALED
    q_out_r159d = vhsubq_s32 (q_t2_r, q_t1_i);
    \
    q_out_i159d = vhaddq_s32 (q_t2_i, q_t1_r);
    \
    q_out_r37bf = vhaddq_s32 (q_t2_r, q_t1_i);
    \
    q_out_i37bf = vhsubq_s32 (q_t2_i, q_t1_r);

    // second stages
    FFT16_LS_START
    FFT16_LS_LOAD
    FFT16_INV_LS
    FFT16_INV_LS_S0
    FFT16_LS_02_SCALED

    q2_out_4567.val[0] = vhsubq_s32 (q_s5_r, q_s4_i);
    q2_out_4567.val[1] = vhaddq_s32 (q_s5_i, q_s4_r);
    q2_out_cdef.val[0] = vhaddq_s32 (q_s5_r, q_s4_i);
    q2_out_cdef.val[1] = vhsubq_s32 (q_s5_i, q_s4_r);

    q2_out_89ab.val[0] = vshrq_n_s32 (q2_out_89ab.val[0], 4);
    q2_out_89ab.val[1] = vshrq_n_s32 (q2_out_89ab.val[1], 4);
    q2_out_0123.val[0] = vshrq_n_s32 (q2_out_0123.val[0], 4);
    q2_out_0123.val[1] = vshrq_n_s32 (q2_out_0123.val[1], 4);
    q2_out_4567.val[0] = vshrq_n_s32 (q2_out_4567.val[0], 4);
    q2_out_4567.val[1] = vshrq_n_s32 (q2_out_4567.val[1], 4);
    q2_out_cdef.val[0] = vshrq_n_s32 (q2_out_cdef.val[0], 4);
    q2_out_cdef.val[1] = vshrq_n_s32 (q2_out_cdef.val[1], 4);

    FFT16_ST
}


#define RADIX8x4_START \
    ne10_int32_t f_count; \
    ne10_int32_t src_step = stride << 1; \
    const ne10_int32_t TW_81 = 1518500249; \
    const ne10_int32_t TW_81N = -1518500249; \
    int32_t *p_src, *p_dst; \
    int32x4x2_t q2_in0, q2_in1, q2_in2, q2_in3, q2_in4, q2_in5, q2_in6, q2_in7; \
    int32x4_t q_sin0_r, q_sin0_i, q_sin1_r, q_sin1_i, q_sin2_r, q_sin2_i, q_sin3_r, q_sin3_i; \
    int32x4_t q_sin4_r, q_sin4_i, q_sin5_r, q_sin5_i, q_sin6_r, q_sin6_i, q_sin7_r, q_sin7_i; \
    int32x4_t q_s3_r, q_s3_i, q_s5_r, q_s5_i, q_s7_r, q_s7_i; \
    int32x4_t q_s8_r, q_s8_i, q_s9_r, q_s9_i, q_s10_r, q_s10_i, q_s11_r, q_s11_i; \
    int32x4_t q_s12_r, q_s12_i, q_s13_r, q_s13_i, q_s14_r, q_s14_i, q_s15_r, q_s15_i; \
    int32x4_t q_out0_r, q_out0_i, q_out1_r, q_out1_i, q_out2_r, q_out2_i, q_out3_r, q_out3_i; \
    int32x4_t q_out4_r, q_out4_i, q_out5_r, q_out5_i, q_out6_r, q_out6_i, q_out7_r, q_out7_i; \
    int32x4x2_t q2_tmp0, q2_tmp1, q2_tmp2, q2_tmp3, q2_tmp4, q2_tmp5, q2_tmp6, q2_tmp7; \
    int32x4x2_t q2_out0, q2_out1, q2_out2, q2_out3, q2_out4, q2_out5, q2_out6, q2_out7; \
    int32x4_t q_tw_81, q_tw_81n; \
    p_src = (int32_t *) Fin; \
    p_dst = (int32_t *) Fout;


#define RADIX8x4_LOAD \
    q2_in0 = vld2q_s32 (p_src); \
    p_src += src_step; \
    q2_in2 = vld2q_s32 (p_src); \
    p_src += src_step; \
    q2_in4 = vld2q_s32 (p_src); \
    p_src += src_step; \
    q2_in6 = vld2q_s32 (p_src); \
    p_src += src_step; \
    q2_in1 = vld2q_s32 (p_src); \
    p_src += src_step; \
    q2_in3 = vld2q_s32 (p_src); \
    p_src += src_step; \
    q2_in5 = vld2q_s32 (p_src); \
    p_src += src_step; \
    q2_in7 = vld2q_s32 (p_src); \
    p_src += src_step;

#define RADIX8x4_STORE \
    q2_tmp0 = vtrnq_s32 (q_out0_r, q_out1_r); \
    q2_tmp1 = vtrnq_s32 (q_out0_i, q_out1_i); \
    q2_tmp2 = vtrnq_s32 (q_out2_r, q_out3_r); \
    q2_tmp3 = vtrnq_s32 (q_out2_i, q_out3_i); \
    q2_tmp4 = vtrnq_s32 (q_out4_r, q_out5_r); \
    q2_tmp5 = vtrnq_s32 (q_out4_i, q_out5_i); \
    q2_tmp6 = vtrnq_s32 (q_out6_r, q_out7_r); \
    q2_tmp7 = vtrnq_s32 (q_out6_i, q_out7_i); \
    q2_out0.val[0] = vcombine_s32 (vget_low_s32 (q2_tmp0.val[0]), vget_low_s32 (q2_tmp2.val[0])); \
    q2_out0.val[1] = vcombine_s32 (vget_low_s32 (q2_tmp1.val[0]), vget_low_s32 (q2_tmp3.val[0])); \
    q2_out2.val[0] = vcombine_s32 (vget_low_s32 (q2_tmp0.val[1]), vget_low_s32 (q2_tmp2.val[1])); \
    q2_out2.val[1] = vcombine_s32 (vget_low_s32 (q2_tmp1.val[1]), vget_low_s32 (q2_tmp3.val[1])); \
    q2_out4.val[0] = vcombine_s32 (vget_high_s32 (q2_tmp0.val[0]), vget_high_s32 (q2_tmp2.val[0])); \
    q2_out4.val[1] = vcombine_s32 (vget_high_s32 (q2_tmp1.val[0]), vget_high_s32 (q2_tmp3.val[0])); \
    q2_out6.val[0] = vcombine_s32 (vget_high_s32 (q2_tmp0.val[1]), vget_high_s32 (q2_tmp2.val[1])); \
    q2_out6.val[1] = vcombine_s32 (vget_high_s32 (q2_tmp1.val[1]), vget_high_s32 (q2_tmp3.val[1])); \
    q2_out1.val[0] = vcombine_s32 (vget_low_s32 (q2_tmp4.val[0]), vget_low_s32 (q2_tmp6.val[0])); \
    q2_out1.val[1] = vcombine_s32 (vget_low_s32 (q2_tmp5.val[0]), vget_low_s32 (q2_tmp7.val[0])); \
    q2_out3.val[0] = vcombine_s32 (vget_low_s32 (q2_tmp4.val[1]), vget_low_s32 (q2_tmp6.val[1])); \
    q2_out3.val[1] = vcombine_s32 (vget_low_s32 (q2_tmp5.val[1]), vget_low_s32 (q2_tmp7.val[1])); \
    q2_out5.val[0] = vcombine_s32 (vget_high_s32 (q2_tmp4.val[0]), vget_high_s32 (q2_tmp6.val[0])); \
    q2_out5.val[1] = vcombine_s32 (vget_high_s32 (q2_tmp5.val[0]), vget_high_s32 (q2_tmp7.val[0])); \
    q2_out7.val[0] = vcombine_s32 (vget_high_s32 (q2_tmp4.val[1]), vget_high_s32 (q2_tmp6.val[1])); \
    q2_out7.val[1] = vcombine_s32 (vget_high_s32 (q2_tmp5.val[1]), vget_high_s32 (q2_tmp7.val[1])); \
    vst2q_s32 (p_dst, q2_out0); \
    p_dst += 8; \
    vst2q_s32 (p_dst, q2_out1); \
    p_dst += 8; \
    vst2q_s32 (p_dst, q2_out2); \
    p_dst += 8; \
    vst2q_s32 (p_dst, q2_out3); \
    p_dst += 8; \
    vst2q_s32 (p_dst, q2_out4); \
    p_dst += 8; \
    vst2q_s32 (p_dst, q2_out5); \
    p_dst += 8; \
    vst2q_s32 (p_dst, q2_out6); \
    p_dst += 8; \
    vst2q_s32 (p_dst, q2_out7); \
    p_dst += 8; \
    p_src = p_src - src_step * 8 + 8;

#define RADIX8x4_FS_S0 \
    q_sin0_r = vaddq_s32 (q2_in0.val[0], q2_in1.val[0]); \
    q_sin0_i = vaddq_s32 (q2_in0.val[1], q2_in1.val[1]); \
    q_sin1_r = vsubq_s32 (q2_in0.val[0], q2_in1.val[0]); \
    q_sin1_i = vsubq_s32 (q2_in0.val[1], q2_in1.val[1]); \
    q_sin2_r = vaddq_s32 (q2_in2.val[0], q2_in3.val[0]); \
    q_sin2_i = vaddq_s32 (q2_in2.val[1], q2_in3.val[1]); \
    q_sin3_r = vsubq_s32 (q2_in2.val[0], q2_in3.val[0]); \
    q_sin3_i = vsubq_s32 (q2_in2.val[1], q2_in3.val[1]); \
    q_sin4_r = vaddq_s32 (q2_in4.val[0], q2_in5.val[0]); \
    q_sin4_i = vaddq_s32 (q2_in4.val[1], q2_in5.val[1]); \
    q_sin5_r = vsubq_s32 (q2_in4.val[0], q2_in5.val[0]); \
    q_sin5_i = vsubq_s32 (q2_in4.val[1], q2_in5.val[1]); \
    q_sin6_r = vaddq_s32 (q2_in6.val[0], q2_in7.val[0]); \
    q_sin6_i = vaddq_s32 (q2_in6.val[1], q2_in7.val[1]); \
    q_sin7_r = vsubq_s32 (q2_in6.val[0], q2_in7.val[0]); \
    q_sin7_i = vsubq_s32 (q2_in6.val[1], q2_in7.val[1]);

#define RADIX8x4_FWD_S357 \
    q_tw_81 = vdupq_n_s32 (TW_81); \
    q_tw_81n = vdupq_n_s32 (TW_81N); \
    q_s5_r = q_sin5_i; \
    q_s5_i = vnegq_s32 (q_sin5_r); \
    q_s3_r = vaddq_s32 (q_sin3_r, q_sin3_i); \
    q_s3_i = vsubq_s32 (q_sin3_i, q_sin3_r); \
    q_s7_r = vsubq_s32 (q_sin7_r, q_sin7_i); \
    q_s7_i = vaddq_s32 (q_sin7_i, q_sin7_r); \
    q_s3_r = vqdmulhq_s32 (q_s3_r, q_tw_81); \
    q_s3_i = vqdmulhq_s32 (q_s3_i, q_tw_81); \
    q_s7_r = vqdmulhq_s32 (q_s7_r, q_tw_81n); \
    q_s7_i = vqdmulhq_s32 (q_s7_i, q_tw_81n);

#define RADIX8x4_INV_S357 \
    q_tw_81 = vdupq_n_s32 (TW_81); \
    q_tw_81n = vdupq_n_s32 (TW_81N); \
    q_s5_r = vnegq_s32 (q_sin5_i); \
    q_s5_i = q_sin5_r; \
    q_s3_r = vsubq_s32 (q_sin3_r, q_sin3_i); \
    q_s3_i = vaddq_s32 (q_sin3_i, q_sin3_r); \
    q_s7_r = vaddq_s32 (q_sin7_r, q_sin7_i); \
    q_s7_i = vsubq_s32 (q_sin7_i, q_sin7_r); \
    q_s3_r = vqdmulhq_s32 (q_s3_r, q_tw_81); \
    q_s3_i = vqdmulhq_s32 (q_s3_i, q_tw_81); \
    q_s7_r = vqdmulhq_s32 (q_s7_r, q_tw_81n); \
    q_s7_i = vqdmulhq_s32 (q_s7_i, q_tw_81n);

#define RADIX8x4_LS_02 \
    q_s8_r = vaddq_s32 (q_sin0_r, q_sin4_r); \
    q_s8_i = vaddq_s32 (q_sin0_i, q_sin4_i); \
    q_s9_r = vaddq_s32 (q_sin1_r, q_s5_r); \
    q_s9_i = vaddq_s32 (q_sin1_i, q_s5_i); \
    q_s10_r = vsubq_s32 (q_sin0_r, q_sin4_r); \
    q_s10_i = vsubq_s32 (q_sin0_i, q_sin4_i); \
    q_s11_r = vsubq_s32 (q_sin1_r, q_s5_r); \
    q_s11_i = vsubq_s32 (q_sin1_i, q_s5_i); \
    q_s12_r = vaddq_s32 (q_sin2_r, q_sin6_r); \
    q_s12_i = vaddq_s32 (q_sin2_i, q_sin6_i); \
    q_s13_r = vaddq_s32 (q_s3_r, q_s7_r); \
    q_s13_i = vaddq_s32 (q_s3_i, q_s7_i); \
    q_s14_r = vsubq_s32 (q_sin2_r, q_sin6_r); \
    q_s14_i = vsubq_s32 (q_sin2_i, q_sin6_i); \
    q_s15_r = vsubq_s32 (q_s3_r, q_s7_r); \
    q_s15_i = vsubq_s32 (q_s3_i, q_s7_i); \
    q_out4_r = vsubq_s32 (q_s8_r, q_s12_r); \
    q_out4_i = vsubq_s32 (q_s8_i, q_s12_i); \
    q_out5_r = vsubq_s32 (q_s9_r, q_s13_r); \
    q_out5_i = vsubq_s32 (q_s9_i, q_s13_i); \
    q_out0_r = vaddq_s32 (q_s8_r, q_s12_r); \
    q_out0_i = vaddq_s32 (q_s8_i, q_s12_i); \
    q_out1_r = vaddq_s32 (q_s9_r, q_s13_r); \
    q_out1_i = vaddq_s32 (q_s9_i, q_s13_i);

#define RADIX8x4_FS_S0_SCALED \
    q_sin0_r = vhaddq_s32 (q2_in0.val[0], q2_in1.val[0]); \
    q_sin0_i = vhaddq_s32 (q2_in0.val[1], q2_in1.val[1]); \
    q_sin1_r = vhsubq_s32 (q2_in0.val[0], q2_in1.val[0]); \
    q_sin1_i = vhsubq_s32 (q2_in0.val[1], q2_in1.val[1]); \
    q_sin2_r = vhaddq_s32 (q2_in2.val[0], q2_in3.val[0]); \
    q_sin2_i = vhaddq_s32 (q2_in2.val[1], q2_in3.val[1]); \
    q_sin3_r = vhsubq_s32 (q2_in2.val[0], q2_in3.val[0]); \
    q_sin3_i = vhsubq_s32 (q2_in2.val[1], q2_in3.val[1]); \
    q_sin4_r = vhaddq_s32 (q2_in4.val[0], q2_in5.val[0]); \
    q_sin4_i = vhaddq_s32 (q2_in4.val[1], q2_in5.val[1]); \
    q_sin5_r = vhsubq_s32 (q2_in4.val[0], q2_in5.val[0]); \
    q_sin5_i = vhsubq_s32 (q2_in4.val[1], q2_in5.val[1]); \
    q_sin6_r = vhaddq_s32 (q2_in6.val[0], q2_in7.val[0]); \
    q_sin6_i = vhaddq_s32 (q2_in6.val[1], q2_in7.val[1]); \
    q_sin7_r = vhsubq_s32 (q2_in6.val[0], q2_in7.val[0]); \
    q_sin7_i = vhsubq_s32 (q2_in6.val[1], q2_in7.val[1]);

#define RADIX8x4_LS_02_SCALED \
    q_s8_r = vhaddq_s32 (q_sin0_r, q_sin4_r); \
    q_s8_i = vhaddq_s32 (q_sin0_i, q_sin4_i); \
    q_s9_r = vhaddq_s32 (q_sin1_r, q_s5_r); \
    q_s9_i = vhaddq_s32 (q_sin1_i, q_s5_i); \
    q_s10_r = vhsubq_s32 (q_sin0_r, q_sin4_r); \
    q_s10_i = vhsubq_s32 (q_sin0_i, q_sin4_i); \
    q_s11_r = vhsubq_s32 (q_sin1_r, q_s5_r); \
    q_s11_i = vhsubq_s32 (q_sin1_i, q_s5_i); \
    q_s12_r = vhaddq_s32 (q_sin2_r, q_sin6_r); \
    q_s12_i = vhaddq_s32 (q_sin2_i, q_sin6_i); \
    q_s13_r = vhaddq_s32 (q_s3_r, q_s7_r); \
    q_s13_i = vhaddq_s32 (q_s3_i, q_s7_i); \
    q_s14_r = vhsubq_s32 (q_sin2_r, q_sin6_r); \
    q_s14_i = vhsubq_s32 (q_sin2_i, q_sin6_i); \
    q_s15_r = vhsubq_s32 (q_s3_r, q_s7_r); \
    q_s15_i = vhsubq_s32 (q_s3_i, q_s7_i); \
    q_out4_r = vhsubq_s32 (q_s8_r, q_s12_r); \
    q_out4_i = vhsubq_s32 (q_s8_i, q_s12_i); \
    q_out5_r = vhsubq_s32 (q_s9_r, q_s13_r); \
    q_out5_i = vhsubq_s32 (q_s9_i, q_s13_i); \
    q_out0_r = vhaddq_s32 (q_s8_r, q_s12_r); \
    q_out0_i = vhaddq_s32 (q_s8_i, q_s12_i); \
    q_out1_r = vhaddq_s32 (q_s9_r, q_s13_r); \
    q_out1_i = vhaddq_s32 (q_s9_i, q_s13_i);


static inline void ne10_radix8x4_forward_unscaled_neon (ne10_fft_cpx_int32_t * Fout,
        ne10_fft_cpx_int32_t * Fin,
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

        q_out2_r = vaddq_s32 (q_s10_r, q_s14_i);
        q_out2_i = vsubq_s32 (q_s10_i, q_s14_r);
        q_out3_r = vaddq_s32 (q_s11_r, q_s15_i);
        q_out3_i = vsubq_s32 (q_s11_i, q_s15_r);
        q_out6_r = vsubq_s32 (q_s10_r, q_s14_i);
        q_out6_i = vaddq_s32 (q_s10_i, q_s14_r);
        q_out7_r = vsubq_s32 (q_s11_r, q_s15_i);
        q_out7_i = vaddq_s32 (q_s11_i, q_s15_r);

        RADIX8x4_STORE
    } // f_count
}

static inline void ne10_radix8x4_backward_unscaled_neon (ne10_fft_cpx_int32_t * Fout,
        ne10_fft_cpx_int32_t * Fin,
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

        q_out2_r = vsubq_s32 (q_s10_r, q_s14_i);
        q_out2_i = vaddq_s32 (q_s10_i, q_s14_r);
        q_out3_r = vsubq_s32 (q_s11_r, q_s15_i);
        q_out3_i = vaddq_s32 (q_s11_i, q_s15_r);
        q_out6_r = vaddq_s32 (q_s10_r, q_s14_i);
        q_out6_i = vsubq_s32 (q_s10_i, q_s14_r);
        q_out7_r = vaddq_s32 (q_s11_r, q_s15_i);
        q_out7_i = vsubq_s32 (q_s11_i, q_s15_r);

        RADIX8x4_STORE
    } // f_count
}
static inline void ne10_radix8x4_forward_scaled_neon (ne10_fft_cpx_int32_t * Fout,
        ne10_fft_cpx_int32_t * Fin,
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

        q_out2_r = vhaddq_s32 (q_s10_r, q_s14_i);
        q_out2_i = vhsubq_s32 (q_s10_i, q_s14_r);
        q_out3_r = vhaddq_s32 (q_s11_r, q_s15_i);
        q_out3_i = vhsubq_s32 (q_s11_i, q_s15_r);
        q_out6_r = vhsubq_s32 (q_s10_r, q_s14_i);
        q_out6_i = vhaddq_s32 (q_s10_i, q_s14_r);
        q_out7_r = vhsubq_s32 (q_s11_r, q_s15_i);
        q_out7_i = vhaddq_s32 (q_s11_i, q_s15_r);

        RADIX8x4_STORE
    } // f_count
}

static inline void ne10_radix8x4_backward_scaled_neon (ne10_fft_cpx_int32_t * Fout,
        ne10_fft_cpx_int32_t * Fin,
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

        q_out2_r = vhsubq_s32 (q_s10_r, q_s14_i);
        q_out2_i = vhaddq_s32 (q_s10_i, q_s14_r);
        q_out3_r = vhsubq_s32 (q_s11_r, q_s15_i);
        q_out3_i = vhaddq_s32 (q_s11_i, q_s15_r);
        q_out6_r = vhaddq_s32 (q_s10_r, q_s14_i);
        q_out6_i = vhsubq_s32 (q_s10_i, q_s14_r);
        q_out7_r = vhaddq_s32 (q_s11_r, q_s15_i);
        q_out7_i = vhsubq_s32 (q_s11_i, q_s15_r);

        RADIX8x4_STORE
    } // f_count
}

#define RADIX4x4_WITHOUT_TW_START \
    ne10_int32_t f_count; \
    ne10_int32_t src_step = stride << 1; \
    int32_t *p_src, *p_dst; \
    int32x4x2_t q2_in0, q2_in1, q2_in2, q2_in3; \
    int32x4_t q_s0_r, q_s0_i, q_s1_r, q_s1_i, q_s2_r, q_s2_i, q_s3_r, q_s3_i; \
    int32x4_t q_out0_r, q_out0_i, q_out1_r, q_out1_i, q_out2_r, q_out2_i, q_out3_r, q_out3_i; \
    int32x4x2_t q2_tmp0, q2_tmp1, q2_tmp2, q2_tmp3; \
    int32x4x2_t q2_out0, q2_out1, q2_out2, q2_out3; \
    p_src = (int32_t *) Fin; \
    p_dst = (int32_t *) Fout;

#define RADIX4x4_WITHOUT_TW_LOAD \
    q2_in0 = vld2q_s32 (p_src); \
    p_src += src_step; \
    q2_in1 = vld2q_s32 (p_src); \
    p_src += src_step; \
    q2_in2 = vld2q_s32 (p_src); \
    p_src += src_step; \
    q2_in3 = vld2q_s32 (p_src); \
    p_src += src_step;

#define RADIX4x4_WITHOUT_TW_STORE \
    q2_tmp0 = vtrnq_s32 (q_out0_r, q_out1_r); \
    q2_tmp1 = vtrnq_s32 (q_out0_i, q_out1_i); \
    q2_tmp2 = vtrnq_s32 (q_out2_r, q_out3_r); \
    q2_tmp3 = vtrnq_s32 (q_out2_i, q_out3_i); \
    q2_out0.val[0] = vcombine_s32 (vget_low_s32 (q2_tmp0.val[0]), vget_low_s32 (q2_tmp2.val[0])); \
    q2_out0.val[1] = vcombine_s32 (vget_low_s32 (q2_tmp1.val[0]), vget_low_s32 (q2_tmp3.val[0])); \
    q2_out1.val[0] = vcombine_s32 (vget_low_s32 (q2_tmp0.val[1]), vget_low_s32 (q2_tmp2.val[1])); \
    q2_out1.val[1] = vcombine_s32 (vget_low_s32 (q2_tmp1.val[1]), vget_low_s32 (q2_tmp3.val[1])); \
    q2_out2.val[0] = vcombine_s32 (vget_high_s32 (q2_tmp0.val[0]), vget_high_s32 (q2_tmp2.val[0])); \
    q2_out2.val[1] = vcombine_s32 (vget_high_s32 (q2_tmp1.val[0]), vget_high_s32 (q2_tmp3.val[0])); \
    q2_out3.val[0] = vcombine_s32 (vget_high_s32 (q2_tmp0.val[1]), vget_high_s32 (q2_tmp2.val[1])); \
    q2_out3.val[1] = vcombine_s32 (vget_high_s32 (q2_tmp1.val[1]), vget_high_s32 (q2_tmp3.val[1])); \
    vst2q_s32 (p_dst, q2_out0); \
    p_dst += 8; \
    vst2q_s32 (p_dst, q2_out1); \
    p_dst += 8; \
    vst2q_s32 (p_dst, q2_out2); \
    p_dst += 8; \
    vst2q_s32 (p_dst, q2_out3); \
    p_dst += 8; \
    p_src = p_src - src_step * 4 + 8;

#define RADIX4x4_WITHOUT_TW_S0 \
        q_s0_r = vaddq_s32 (q2_in0.val[0], q2_in2.val[0]); \
        q_s0_i = vaddq_s32 (q2_in0.val[1], q2_in2.val[1]); \
        q_s1_r = vsubq_s32 (q2_in0.val[0], q2_in2.val[0]); \
        q_s1_i = vsubq_s32 (q2_in0.val[1], q2_in2.val[1]); \
        q_s2_r = vaddq_s32 (q2_in1.val[0], q2_in3.val[0]); \
        q_s2_i = vaddq_s32 (q2_in1.val[1], q2_in3.val[1]); \
        q_s3_r = vsubq_s32 (q2_in1.val[0], q2_in3.val[0]); \
        q_s3_i = vsubq_s32 (q2_in1.val[1], q2_in3.val[1]); \
        q_out2_r = vsubq_s32 (q_s0_r, q_s2_r); \
        q_out2_i = vsubq_s32 (q_s0_i, q_s2_i); \
        q_out0_r = vaddq_s32 (q_s0_r, q_s2_r); \
        q_out0_i = vaddq_s32 (q_s0_i, q_s2_i);

#define RADIX4x4_WITHOUT_TW_S0_SCALED \
        q_s0_r = vhaddq_s32 (q2_in0.val[0], q2_in2.val[0]); \
        q_s0_i = vhaddq_s32 (q2_in0.val[1], q2_in2.val[1]); \
        q_s1_r = vhsubq_s32 (q2_in0.val[0], q2_in2.val[0]); \
        q_s1_i = vhsubq_s32 (q2_in0.val[1], q2_in2.val[1]); \
        q_s2_r = vhaddq_s32 (q2_in1.val[0], q2_in3.val[0]); \
        q_s2_i = vhaddq_s32 (q2_in1.val[1], q2_in3.val[1]); \
        q_s3_r = vhsubq_s32 (q2_in1.val[0], q2_in3.val[0]); \
        q_s3_i = vhsubq_s32 (q2_in1.val[1], q2_in3.val[1]); \
        q_out2_r = vhsubq_s32 (q_s0_r, q_s2_r); \
        q_out2_i = vhsubq_s32 (q_s0_i, q_s2_i); \
        q_out0_r = vhaddq_s32 (q_s0_r, q_s2_r); \
        q_out0_i = vhaddq_s32 (q_s0_i, q_s2_i);


static inline void ne10_radix4x4_without_twiddles_forward_unscaled_neon (ne10_fft_cpx_int32_t * Fout,
        ne10_fft_cpx_int32_t * Fin,
        ne10_int32_t stride)
{
    RADIX4x4_WITHOUT_TW_START

    for (f_count = 0; f_count < stride; f_count += 4)
    {
        // load
        RADIX4x4_WITHOUT_TW_LOAD

        // radix 4 butterfly without twiddles
        RADIX4x4_WITHOUT_TW_S0

        q_out1_r = vaddq_s32 (q_s1_r, q_s3_i);
        q_out1_i = vsubq_s32 (q_s1_i, q_s3_r);
        q_out3_r = vsubq_s32 (q_s1_r, q_s3_i);
        q_out3_i = vaddq_s32 (q_s1_i, q_s3_r);

        RADIX4x4_WITHOUT_TW_STORE
    }
}

static inline void ne10_radix4x4_without_twiddles_backward_unscaled_neon (ne10_fft_cpx_int32_t * Fout,
        ne10_fft_cpx_int32_t * Fin,
        ne10_int32_t stride)
{
    RADIX4x4_WITHOUT_TW_START

    for (f_count = 0; f_count < stride; f_count += 4)
    {
        // load
        RADIX4x4_WITHOUT_TW_LOAD

        // radix 4 butterfly without twiddles
        RADIX4x4_WITHOUT_TW_S0

        q_out1_r = vsubq_s32 (q_s1_r, q_s3_i);
        q_out1_i = vaddq_s32 (q_s1_i, q_s3_r);
        q_out3_r = vaddq_s32 (q_s1_r, q_s3_i);
        q_out3_i = vsubq_s32 (q_s1_i, q_s3_r);

        RADIX4x4_WITHOUT_TW_STORE
    }
}

static inline void ne10_radix4x4_without_twiddles_forward_scaled_neon (ne10_fft_cpx_int32_t * Fout,
        ne10_fft_cpx_int32_t * Fin,
        ne10_int32_t stride)
{
    RADIX4x4_WITHOUT_TW_START

    for (f_count = 0; f_count < stride; f_count += 4)
    {
        // load
        RADIX4x4_WITHOUT_TW_LOAD

        // radix 4 butterfly without twiddles
        RADIX4x4_WITHOUT_TW_S0_SCALED

        q_out1_r = vhaddq_s32 (q_s1_r, q_s3_i);
        q_out1_i = vhsubq_s32 (q_s1_i, q_s3_r);
        q_out3_r = vhsubq_s32 (q_s1_r, q_s3_i);
        q_out3_i = vhaddq_s32 (q_s1_i, q_s3_r);

        RADIX4x4_WITHOUT_TW_STORE
    }
}

static inline void ne10_radix4x4_without_twiddles_backward_scaled_neon (ne10_fft_cpx_int32_t * Fout,
        ne10_fft_cpx_int32_t * Fin,
        ne10_int32_t stride)
{
    RADIX4x4_WITHOUT_TW_START

    for (f_count = 0; f_count < stride; f_count += 4)
    {
        // load
        RADIX4x4_WITHOUT_TW_LOAD

        // radix 4 butterfly without twiddles
        RADIX4x4_WITHOUT_TW_S0_SCALED

        q_out1_r = vhsubq_s32 (q_s1_r, q_s3_i);
        q_out1_i = vhaddq_s32 (q_s1_i, q_s3_r);
        q_out3_r = vhaddq_s32 (q_s1_r, q_s3_i);
        q_out3_i = vhsubq_s32 (q_s1_i, q_s3_r);

        RADIX4x4_WITHOUT_TW_STORE
    }
}

#define RADIX4x4_WITH_TW_START \
    ne10_int32_t m_count; \
    ne10_int32_t src_step = src_stride << 1; \
    ne10_int32_t dst_step = dst_stride << 1; \
    ne10_int32_t tw_step = mstride << 1; \
    int32_t *p_src, *p_dst, *p_tw; \
    int32x4x2_t q2_in0, q2_in1, q2_in2, q2_in3; \
    int32x4x2_t q2_tw0, q2_tw1, q2_tw2; \
    int32x4_t q_s1_r, q_s1_i, q_s2_r, q_s2_i, q_s3_r, q_s3_i; \
    int32x4_t q_tmp0, q_tmp1, q_tmp2, q_tmp3, q_tmp4, q_tmp5; \
    int32x4_t q_s4_r, q_s4_i, q_s5_r, q_s5_i, q_s6_r, q_s6_i, q_s7_r, q_s7_i; \
    int32x4x2_t q2_out0, q2_out1, q2_out2, q2_out3; \
    p_src = (int32_t *) Fin; \
    p_dst = (int32_t *) Fout; \
    p_tw = (int32_t *) tw;

#define RADIX4x4_WITH_TW_LOAD \
    q2_in0 = vld2q_s32 (p_src); \
    p_src += src_step; \
    q2_in1 = vld2q_s32 (p_src); \
    p_src += src_step; \
    q2_in2 = vld2q_s32 (p_src); \
    p_src += src_step; \
    q2_in3 = vld2q_s32 (p_src); \
    p_src += src_step; \
    q2_tw0 = vld2q_s32 (p_tw); \
    p_tw += tw_step; \
    q2_tw1 = vld2q_s32 (p_tw); \
    p_tw += tw_step; \
    q2_tw2 = vld2q_s32 (p_tw); \
    q_s1_r = vqdmulhq_s32 (q2_in1.val[0], q2_tw0.val[0]); \
    q_s1_i = vqdmulhq_s32 (q2_in1.val[1], q2_tw0.val[0]); \
    q_s2_r = vqdmulhq_s32 (q2_in2.val[0], q2_tw1.val[0]); \
    q_s2_i = vqdmulhq_s32 (q2_in2.val[1], q2_tw1.val[0]); \
    q_s3_r = vqdmulhq_s32 (q2_in3.val[0], q2_tw2.val[0]); \
    q_s3_i = vqdmulhq_s32 (q2_in3.val[1], q2_tw2.val[0]); \
    q_tmp0 = vqdmulhq_s32 (q2_in1.val[1], q2_tw0.val[1]); \
    q_tmp1 = vqdmulhq_s32 (q2_in1.val[0], q2_tw0.val[1]); \
    q_tmp2 = vqdmulhq_s32 (q2_in2.val[1], q2_tw1.val[1]); \
    q_tmp3 = vqdmulhq_s32 (q2_in2.val[0], q2_tw1.val[1]); \
    q_tmp4 = vqdmulhq_s32 (q2_in3.val[1], q2_tw2.val[1]); \
    q_tmp5 = vqdmulhq_s32 (q2_in3.val[0], q2_tw2.val[1]);

#define RADIX4x4_WITH_TW_STORE \
    vst2q_s32 (p_dst, q2_out0); \
    p_dst += dst_step; \
    vst2q_s32 (p_dst, q2_out1); \
    p_dst += dst_step; \
    vst2q_s32 (p_dst, q2_out2); \
    p_dst += dst_step; \
    vst2q_s32 (p_dst, q2_out3); \
    p_dst += dst_step; \
    p_src = p_src - src_step * 4 + 8; \
    p_dst = p_dst - dst_step * 4 + 8; \
    p_tw = p_tw - tw_step * 2 + 8;

#define RADIX4x4_WITH_TW_S1_FWD \
    q_s1_r = vsubq_s32 (q_s1_r, q_tmp0); \
    q_s1_i = vaddq_s32 (q_s1_i, q_tmp1); \
    q_s2_r = vsubq_s32 (q_s2_r, q_tmp2); \
    q_s2_i = vaddq_s32 (q_s2_i, q_tmp3); \
    q_s3_r = vsubq_s32 (q_s3_r, q_tmp4); \
    q_s3_i = vaddq_s32 (q_s3_i, q_tmp5);

#define RADIX4x4_WITH_TW_S1_INV \
    q_s1_r = vaddq_s32 (q_s1_r, q_tmp0); \
    q_s1_i = vsubq_s32 (q_s1_i, q_tmp1); \
    q_s2_r = vaddq_s32 (q_s2_r, q_tmp2); \
    q_s2_i = vsubq_s32 (q_s2_i, q_tmp3); \
    q_s3_r = vaddq_s32 (q_s3_r, q_tmp4); \
    q_s3_i = vsubq_s32 (q_s3_i, q_tmp5);


#define RADIX4x4_WITH_TW_LS_02 \
    q_s4_r = vaddq_s32 (q2_in0.val[0], q_s2_r); \
    q_s4_i = vaddq_s32 (q2_in0.val[1], q_s2_i); \
    q_s5_r = vsubq_s32 (q2_in0.val[0], q_s2_r); \
    q_s5_i = vsubq_s32 (q2_in0.val[1], q_s2_i); \
    q_s6_r = vaddq_s32 (q_s1_r, q_s3_r); \
    q_s6_i = vaddq_s32 (q_s1_i, q_s3_i); \
    q_s7_r = vsubq_s32 (q_s1_r, q_s3_r); \
    q_s7_i = vsubq_s32 (q_s1_i, q_s3_i); \
    q2_out2.val[0] = vsubq_s32 (q_s4_r, q_s6_r); \
    q2_out2.val[1] = vsubq_s32 (q_s4_i, q_s6_i); \
    q2_out0.val[0] = vaddq_s32 (q_s4_r, q_s6_r); \
    q2_out0.val[1] = vaddq_s32 (q_s4_i, q_s6_i);

#define RADIX4x4_WITH_TW_LS_02_SCALED \
    q_s4_r = vhaddq_s32 (q2_in0.val[0], q_s2_r); \
    q_s4_i = vhaddq_s32 (q2_in0.val[1], q_s2_i); \
    q_s5_r = vhsubq_s32 (q2_in0.val[0], q_s2_r); \
    q_s5_i = vhsubq_s32 (q2_in0.val[1], q_s2_i); \
    q_s6_r = vhaddq_s32 (q_s1_r, q_s3_r); \
    q_s6_i = vhaddq_s32 (q_s1_i, q_s3_i); \
    q_s7_r = vhsubq_s32 (q_s1_r, q_s3_r); \
    q_s7_i = vhsubq_s32 (q_s1_i, q_s3_i); \
    q2_out2.val[0] = vhsubq_s32 (q_s4_r, q_s6_r); \
    q2_out2.val[1] = vhsubq_s32 (q_s4_i, q_s6_i); \
    q2_out0.val[0] = vhaddq_s32 (q_s4_r, q_s6_r); \
    q2_out0.val[1] = vhaddq_s32 (q_s4_i, q_s6_i);


static inline void ne10_radix4x4_with_twiddles_forward_unscaled_neon (ne10_fft_cpx_int32_t * Fout,
        ne10_fft_cpx_int32_t * Fin,
        ne10_fft_cpx_int32_t * tw,
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

        q2_out1.val[0] = vaddq_s32 (q_s5_r, q_s7_i);
        q2_out1.val[1] = vsubq_s32 (q_s5_i, q_s7_r);
        q2_out3.val[0] = vsubq_s32 (q_s5_r, q_s7_i);
        q2_out3.val[1] = vaddq_s32 (q_s5_i, q_s7_r);

        // store
        RADIX4x4_WITH_TW_STORE
    }
}


static inline void ne10_radix4x4_with_twiddles_backward_unscaled_neon (ne10_fft_cpx_int32_t * Fout,
        ne10_fft_cpx_int32_t * Fin,
        ne10_fft_cpx_int32_t * tw,
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

        q2_out1.val[0] = vsubq_s32 (q_s5_r, q_s7_i);
        q2_out1.val[1] = vaddq_s32 (q_s5_i, q_s7_r);
        q2_out3.val[0] = vaddq_s32 (q_s5_r, q_s7_i);
        q2_out3.val[1] = vsubq_s32 (q_s5_i, q_s7_r);

        // store
        RADIX4x4_WITH_TW_STORE
    }
}



static inline void ne10_radix4x4_with_twiddles_forward_scaled_neon (ne10_fft_cpx_int32_t * Fout,
        ne10_fft_cpx_int32_t * Fin,
        ne10_fft_cpx_int32_t * tw,
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

        q2_out1.val[0] = vhaddq_s32 (q_s5_r, q_s7_i);
        q2_out1.val[1] = vhsubq_s32 (q_s5_i, q_s7_r);
        q2_out3.val[0] = vhsubq_s32 (q_s5_r, q_s7_i);
        q2_out3.val[1] = vhaddq_s32 (q_s5_i, q_s7_r);

        // store
        RADIX4x4_WITH_TW_STORE
    }
}

static inline void ne10_radix4x4_with_twiddles_backward_scaled_neon (ne10_fft_cpx_int32_t * Fout,
        ne10_fft_cpx_int32_t * Fin,
        ne10_fft_cpx_int32_t * tw,
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

        q2_out1.val[0] = vhsubq_s32 (q_s5_r, q_s7_i);
        q2_out1.val[1] = vhaddq_s32 (q_s5_i, q_s7_r);
        q2_out3.val[0] = vhaddq_s32 (q_s5_r, q_s7_i);
        q2_out3.val[1] = vhsubq_s32 (q_s5_i, q_s7_r);

        // store
        RADIX4x4_WITH_TW_STORE
    }
}

static inline void ne10_radix4x4_with_twiddles_ls_backward_unscaled_neon (ne10_fft_cpx_int32_t * Fout,
        ne10_fft_cpx_int32_t * Fin,
        ne10_fft_cpx_int32_t * tw,
        ne10_int32_t src_stride,
        ne10_int32_t dst_stride,
        ne10_int32_t mstride,
        ne10_int32_t order)
{
    RADIX4x4_WITH_TW_START
    int32x4_t q_order = vdupq_n_s32 (order);
    q_order = vnegq_s32 (q_order);

    for (m_count = 0; m_count < mstride; m_count += 4)
    {
        // load
        RADIX4x4_WITH_TW_LOAD
        RADIX4x4_WITH_TW_S1_INV

        RADIX4x4_WITH_TW_LS_02

        q2_out1.val[0] = vsubq_s32 (q_s5_r, q_s7_i);
        q2_out1.val[1] = vaddq_s32 (q_s5_i, q_s7_r);
        q2_out3.val[0] = vaddq_s32 (q_s5_r, q_s7_i);
        q2_out3.val[1] = vsubq_s32 (q_s5_i, q_s7_r);

        q2_out2.val[0] = vrshlq_s32 (q2_out2.val[0], q_order);
        q2_out2.val[1] = vrshlq_s32 (q2_out2.val[1], q_order);
        q2_out0.val[0] = vrshlq_s32 (q2_out0.val[0], q_order);
        q2_out0.val[1] = vrshlq_s32 (q2_out0.val[1], q_order);
        q2_out1.val[0] = vrshlq_s32 (q2_out1.val[0], q_order);
        q2_out1.val[1] = vrshlq_s32 (q2_out1.val[1], q_order);
        q2_out3.val[0] = vrshlq_s32 (q2_out3.val[0], q_order);
        q2_out3.val[1] = vrshlq_s32 (q2_out3.val[1], q_order);

        // store
        RADIX4x4_WITH_TW_STORE
    }
}

static inline void ne10_radix4x4_with_twiddles_ls_backward_scaled_neon (ne10_fft_cpx_int32_t * Fout,
        ne10_fft_cpx_int32_t * Fin,
        ne10_fft_cpx_int32_t * tw,
        ne10_int32_t src_stride,
        ne10_int32_t dst_stride,
        ne10_int32_t mstride,
        ne10_int32_t order)
{
    RADIX4x4_WITH_TW_START
    int32x4_t q_order = vdupq_n_s32 (order);
    q_order = vnegq_s32 (q_order);

    for (m_count = 0; m_count < mstride; m_count += 4)
    {
        // load
        RADIX4x4_WITH_TW_LOAD
        RADIX4x4_WITH_TW_S1_INV

        RADIX4x4_WITH_TW_LS_02_SCALED

        q2_out1.val[0] = vhsubq_s32 (q_s5_r, q_s7_i);
        q2_out1.val[1] = vhaddq_s32 (q_s5_i, q_s7_r);
        q2_out3.val[0] = vhaddq_s32 (q_s5_r, q_s7_i);
        q2_out3.val[1] = vhsubq_s32 (q_s5_i, q_s7_r);

        q2_out2.val[0] = vrshlq_s32 (q2_out2.val[0], q_order);
        q2_out2.val[1] = vrshlq_s32 (q2_out2.val[1], q_order);
        q2_out0.val[0] = vrshlq_s32 (q2_out0.val[0], q_order);
        q2_out0.val[1] = vrshlq_s32 (q2_out0.val[1], q_order);
        q2_out1.val[0] = vrshlq_s32 (q2_out1.val[0], q_order);
        q2_out1.val[1] = vrshlq_s32 (q2_out1.val[1], q_order);
        q2_out3.val[0] = vrshlq_s32 (q2_out3.val[0], q_order);
        q2_out3.val[1] = vrshlq_s32 (q2_out3.val[1], q_order);

        // store
        RADIX4x4_WITH_TW_STORE
    }
}



#define ne10_mixed_radix_fft_forward_int32_neon(scaled) \
void ne10_mixed_radix_fft_forward_int32_##scaled##_neon (ne10_fft_cpx_int32_t * Fout, \
        ne10_fft_cpx_int32_t * Fin, \
        ne10_int32_t * factors, \
        ne10_fft_cpx_int32_t * twiddles, \
        ne10_fft_cpx_int32_t * buffer) \
{ \
    ne10_int32_t fstride, mstride, N; \
    ne10_int32_t fstride1; \
    ne10_int32_t f_count; \
    ne10_int32_t stage_count; \
 \
    ne10_fft_cpx_int32_t   *Fin1, *Fout1; \
    ne10_fft_cpx_int32_t   *Fout_ls = Fout; \
    ne10_fft_cpx_int32_t   *Ftmp; \
    ne10_fft_cpx_int32_t   *tw, *tw1; \
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
        Ftmp = buffer; \
        buffer = Fout; \
        Fout = Ftmp; \
    } \
    else if (N == 4) \
    { \
        ne10_radix4x4_without_twiddles_forward_##scaled##_neon (Fout, Fin, fstride); \
        N = fstride; \
        Ftmp = buffer; \
        buffer = Fout; \
        Fout = Ftmp; \
        /* update address for other stages*/ \
        stage_count--; \
        tw = twiddles; \
        fstride >>= 2; \
    } \
    /* others but the last one*/ \
    for (; stage_count > 1 ; stage_count--) \
    { \
        Fin1 = buffer; \
        for (f_count = 0; f_count < fstride; f_count ++) \
        { \
            Fout1 = & Fout[ f_count * mstride << 2 ]; \
            tw1 = tw; \
            ne10_radix4x4_with_twiddles_forward_##scaled##_neon (Fout1, Fin1, tw1, N, mstride, mstride); \
            Fin1 += mstride; \
        } \
        tw += mstride * 3; \
        mstride <<= 2; \
        Ftmp = buffer; \
        buffer = Fout; \
        Fout = Ftmp; \
        fstride >>= 2; \
    }\
    /* the last one*/ \
    if (stage_count) \
    { \
        Fin1 = buffer; \
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

#define ne10_mixed_radix_fft_backward_int32_neon(scaled) \
void ne10_mixed_radix_fft_backward_int32_##scaled##_neon (ne10_fft_cpx_int32_t * Fout, \
        ne10_fft_cpx_int32_t * Fin, \
        ne10_int32_t * factors, \
        ne10_fft_cpx_int32_t * twiddles, \
        ne10_fft_cpx_int32_t * buffer) \
{ \
    ne10_int32_t fstride, mstride, N; \
    ne10_int32_t fstride1; \
    ne10_int32_t f_count; \
    ne10_int32_t stage_count; \
    ne10_int32_t order; \
 \
    ne10_fft_cpx_int32_t   *Fin1, *Fout1; \
    ne10_fft_cpx_int32_t   *Fout_ls = Fout; \
    ne10_fft_cpx_int32_t   *Ftmp; \
    ne10_fft_cpx_int32_t   *tw, *tw1; \
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
        order = 2 * stage_count - 1; \
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
        Ftmp = buffer; \
        buffer = Fout; \
        Fout = Ftmp; \
    } \
    else if (N == 4) \
    { \
        order = 2 * stage_count; \
        ne10_radix4x4_without_twiddles_backward_##scaled##_neon (Fout, Fin, fstride); \
        N = fstride; \
        Ftmp = buffer; \
        buffer = Fout; \
        Fout = Ftmp; \
        /* update address for other stages*/ \
        stage_count--; \
        tw = twiddles; \
        fstride >>= 2; \
    } \
    /* others but the last one*/ \
    for (; stage_count > 1 ; stage_count--) \
    { \
        Fin1 = buffer; \
        for (f_count = 0; f_count < fstride; f_count ++) \
        { \
            Fout1 = & Fout[ f_count * mstride << 2 ]; \
            tw1 = tw; \
            ne10_radix4x4_with_twiddles_backward_##scaled##_neon (Fout1, Fin1, tw1, N, mstride, mstride); \
            Fin1 += mstride; \
        } \
        tw += mstride * 3; \
        mstride <<= 2; \
        Ftmp = buffer; \
        buffer = Fout; \
        Fout = Ftmp; \
        fstride >>= 2; \
    }\
    /* the last one*/ \
    if (stage_count) \
    { \
        Fin1 = buffer; \
        Fout1 = Fout_ls; \
        for (f_count = 0; f_count < fstride; f_count ++) \
        { \
            tw1 = tw; \
            ne10_radix4x4_with_twiddles_ls_backward_##scaled##_neon (Fout1, Fin1, tw1, N, N, mstride, order); \
            Fin1 += mstride; \
            Fout1 += mstride; \
        } \
    } \
}

ne10_mixed_radix_fft_forward_int32_neon (unscaled)
ne10_mixed_radix_fft_forward_int32_neon (scaled)
ne10_mixed_radix_fft_backward_int32_neon (unscaled)
ne10_mixed_radix_fft_backward_int32_neon (scaled)


static void ne10_fft_split_r2c_1d_int32_neon (ne10_fft_cpx_int32_t *dst,
        const ne10_fft_cpx_int32_t *src,
        ne10_fft_cpx_int32_t *twiddles,
        ne10_int32_t ncfft,
        ne10_int32_t scaled_flag)
{
    ne10_int32_t k;
    ne10_int32_t count = ncfft / 2;
    ne10_fft_cpx_int32_t fpnk, fpk, f1k, f2k, tw, tdc;
    int32x4x2_t q2_fpk, q2_fpnk, q2_tw, q2_dst, q2_dst2;
    int32x4_t q_fpnk_r, q_fpnk_i;
    int32x4_t q_f1k_r, q_f1k_i, q_f2k_r, q_f2k_i;
    int32x4_t q_tw_r, q_tw_i;
    int32x4_t q_tmp0, q_tmp1, q_tmp2, q_tmp3;
    int32x4_t q_dst2_r, q_dst2_i;
    int32_t *p_src, *p_src2, *p_dst, *p_dst2, *p_twiddles;

    tdc.r = src[0].r;
    tdc.i = src[0].i;

    if (scaled_flag)
        NE10_F2I32_FIXDIV (tdc, 2);

    dst[0].r = tdc.r + tdc.i;
    dst[ncfft].r = tdc.r - tdc.i;
    dst[ncfft].i = dst[0].i = 0;
    if (count >= 4)
    {

        if (scaled_flag)
        {
            for (k = 1; k <= count ; k += 4)
            {
                p_src  = (int32_t*) (& (src[k]));
                p_src2  = (int32_t*) (& (src[ncfft - k - 3]));
                p_twiddles  = (int32_t*) (& (twiddles[k - 1]));
                p_dst  = (int32_t*) (& (dst[k]));
                p_dst2  = (int32_t*) (& (dst[ncfft - k - 3]));

                q2_fpk  = vld2q_s32 (p_src);
                q2_fpnk = vld2q_s32 (p_src2);

                q2_tw = vld2q_s32 (p_twiddles);
                q2_fpnk.val[0] = vrev64q_s32 (q2_fpnk.val[0]);
                q2_fpnk.val[1] = vrev64q_s32 (q2_fpnk.val[1]);
                q_fpnk_r = vcombine_s32 (vget_high_s32 (q2_fpnk.val[0]), vget_low_s32 (q2_fpnk.val[0]));
                q_fpnk_i = vcombine_s32 (vget_high_s32 (q2_fpnk.val[1]), vget_low_s32 (q2_fpnk.val[1]));
                q_fpnk_i = vnegq_s32 (q_fpnk_i);

                q_f1k_r = vhaddq_s32 (q2_fpk.val[0], q_fpnk_r);
                q_f1k_i = vhaddq_s32 (q2_fpk.val[1], q_fpnk_i);

                q_f2k_r = vhsubq_s32 (q2_fpk.val[0], q_fpnk_r);
                q_f2k_i = vhsubq_s32 (q2_fpk.val[1], q_fpnk_i);

                q_tmp0 = vqdmulhq_s32 (q_f2k_r, q2_tw.val[0]);
                q_tmp1 = vqdmulhq_s32 (q_f2k_i, q2_tw.val[1]);
                q_tmp2 = vqdmulhq_s32 (q_f2k_r, q2_tw.val[1]);
                q_tmp3 = vqdmulhq_s32 (q_f2k_i, q2_tw.val[0]);
                q_tw_r = vsubq_s32 (q_tmp0, q_tmp1);
                q_tw_i = vaddq_s32 (q_tmp2, q_tmp3);

                q_dst2_r = vhsubq_s32 (q_f1k_r, q_tw_r);
                q_dst2_i = vhsubq_s32 (q_tw_i, q_f1k_i);
                q2_dst.val[0] = vhaddq_s32 (q_f1k_r, q_tw_r);
                q2_dst.val[1] = vhaddq_s32 (q_f1k_i, q_tw_i);
                q_dst2_r = vrev64q_s32 (q_dst2_r);
                q_dst2_i = vrev64q_s32 (q_dst2_i);
                q2_dst2.val[0] = vcombine_s32 (vget_high_s32 (q_dst2_r), vget_low_s32 (q_dst2_r));
                q2_dst2.val[1] = vcombine_s32 (vget_high_s32 (q_dst2_i), vget_low_s32 (q_dst2_i));
                vst2q_s32 (p_dst, q2_dst);
                vst2q_s32 (p_dst2, q2_dst2);

            }
        }
        else
        {
            for (k = 1; k <= count ; k += 4)
            {
                p_src  = (int32_t*) (& (src[k]));
                p_src2  = (int32_t*) (& (src[ncfft - k - 3]));
                p_twiddles  = (int32_t*) (& (twiddles[k - 1]));
                p_dst  = (int32_t*) (& (dst[k]));
                p_dst2  = (int32_t*) (& (dst[ncfft - k - 3]));

                q2_fpk  = vld2q_s32 (p_src);
                q2_fpnk = vld2q_s32 (p_src2);

                q2_tw = vld2q_s32 (p_twiddles);
                q2_fpnk.val[0] = vrev64q_s32 (q2_fpnk.val[0]);
                q2_fpnk.val[1] = vrev64q_s32 (q2_fpnk.val[1]);
                q_fpnk_r = vcombine_s32 (vget_high_s32 (q2_fpnk.val[0]), vget_low_s32 (q2_fpnk.val[0]));
                q_fpnk_i = vcombine_s32 (vget_high_s32 (q2_fpnk.val[1]), vget_low_s32 (q2_fpnk.val[1]));
                q_fpnk_i = vnegq_s32 (q_fpnk_i);

                q_f1k_r = vaddq_s32 (q2_fpk.val[0], q_fpnk_r);
                q_f1k_i = vaddq_s32 (q2_fpk.val[1], q_fpnk_i);

                q_f2k_r = vsubq_s32 (q2_fpk.val[0], q_fpnk_r);
                q_f2k_i = vsubq_s32 (q2_fpk.val[1], q_fpnk_i);

                q_tmp0 = vqdmulhq_s32 (q_f2k_r, q2_tw.val[0]);
                q_tmp1 = vqdmulhq_s32 (q_f2k_i, q2_tw.val[1]);
                q_tmp2 = vqdmulhq_s32 (q_f2k_r, q2_tw.val[1]);
                q_tmp3 = vqdmulhq_s32 (q_f2k_i, q2_tw.val[0]);
                q_tw_r = vsubq_s32 (q_tmp0, q_tmp1);
                q_tw_i = vaddq_s32 (q_tmp2, q_tmp3);

                q_dst2_r = vhsubq_s32 (q_f1k_r, q_tw_r);
                q_dst2_i = vhsubq_s32 (q_tw_i, q_f1k_i);
                q2_dst.val[0] = vhaddq_s32 (q_f1k_r, q_tw_r);
                q2_dst.val[1] = vhaddq_s32 (q_f1k_i, q_tw_i);
                q_dst2_r = vrev64q_s32 (q_dst2_r);
                q_dst2_i = vrev64q_s32 (q_dst2_i);
                q2_dst2.val[0] = vcombine_s32 (vget_high_s32 (q_dst2_r), vget_low_s32 (q_dst2_r));
                q2_dst2.val[1] = vcombine_s32 (vget_high_s32 (q_dst2_i), vget_low_s32 (q_dst2_i));
                vst2q_s32 (p_dst, q2_dst);
                vst2q_s32 (p_dst2, q2_dst2);

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
                NE10_F2I32_FIXDIV (fpk, 2);
                NE10_F2I32_FIXDIV (fpnk, 2);
            }

            f1k.r = fpk.r + fpnk.r;
            f1k.i = fpk.i + fpnk.i;

            f2k.r = fpk.r - fpnk.r;
            f2k.i = fpk.i - fpnk.i;

            tw.r = ( ( (ne10_int32_t) ( ( (NE10_F2I32_SAMPPROD) f2k.r * (twiddles[k - 1]).r) >> 32)) - ( (ne10_int32_t) ( ( (NE10_F2I32_SAMPPROD) f2k.i * (twiddles[k - 1]).i) >> 32))) << 1;
            tw.i = ( ( (ne10_int32_t) ( ( (NE10_F2I32_SAMPPROD) f2k.r * (twiddles[k - 1]).i) >> 32)) + ( (ne10_int32_t) ( ( (NE10_F2I32_SAMPPROD) f2k.i * (twiddles[k - 1]).r) >> 32))) << 1;

            dst[k].r = (f1k.r + tw.r) >> 1;
            dst[k].i = (f1k.i + tw.i) >> 1;
            dst[ncfft - k].r = (f1k.r - tw.r) >> 1;
            dst[ncfft - k].i = (tw.i - f1k.i) >> 1;
        }
    }
}

static void ne10_fft_split_c2r_1d_int32_neon (ne10_fft_cpx_int32_t *dst,
        const ne10_fft_cpx_int32_t *src,
        ne10_fft_cpx_int32_t *twiddles,
        ne10_int32_t ncfft,
        ne10_int32_t scaled_flag)
{

    ne10_int32_t k;
    ne10_int32_t count = ncfft / 2;
    ne10_fft_cpx_int32_t fk, fnkc, fek, fok, tmp;
    int32x4x2_t q2_fk, q2_fnkc, q2_tw, q2_dst, q2_dst2;
    int32x4_t q_fnkc_r, q_fnkc_i;
    int32x4_t q_fek_r, q_fek_i, q_fok_r, q_fok_i;
    int32x4_t q_tmp0, q_tmp1, q_tmp2, q_tmp3;
    int32x4_t q_dst2_r, q_dst2_i;
    int32_t *p_src, *p_src2, *p_dst, *p_dst2, *p_twiddles;


    dst[0].r = (src[0].r + src[ncfft].r) >> 1;
    dst[0].i = (src[0].r - src[ncfft].r) >> 1;
    if (scaled_flag)
        NE10_F2I32_FIXDIV (dst[0], 2);
    if (count >= 4)
    {
        if (scaled_flag)
        {
            for (k = 1; k <= count ; k += 4)
            {
                p_src  = (int32_t*) (& (src[k]));
                p_src2  = (int32_t*) (& (src[ncfft - k - 3]));
                p_twiddles  = (int32_t*) (& (twiddles[k - 1]));
                p_dst  = (int32_t*) (& (dst[k]));
                p_dst2  = (int32_t*) (& (dst[ncfft - k - 3]));

                q2_fk  = vld2q_s32 (p_src);
                q2_fnkc = vld2q_s32 (p_src2);
                q2_tw = vld2q_s32 (p_twiddles);
                q2_fnkc.val[0] = vrev64q_s32 (q2_fnkc.val[0]);
                q2_fnkc.val[1] = vrev64q_s32 (q2_fnkc.val[1]);
                q_fnkc_r = vcombine_s32 (vget_high_s32 (q2_fnkc.val[0]), vget_low_s32 (q2_fnkc.val[0]));
                q_fnkc_i = vcombine_s32 (vget_high_s32 (q2_fnkc.val[1]), vget_low_s32 (q2_fnkc.val[1]));
                q_fnkc_i = vnegq_s32 (q_fnkc_i);

                q_fek_r = vhaddq_s32 (q2_fk.val[0], q_fnkc_r);
                q_fek_i = vhaddq_s32 (q2_fk.val[1], q_fnkc_i);
                q_tmp0 = vhsubq_s32 (q2_fk.val[0], q_fnkc_r);
                q_tmp1 = vhsubq_s32 (q2_fk.val[1], q_fnkc_i);

                q_fok_r = vqdmulhq_s32 (q_tmp0, q2_tw.val[0]);
                q_fok_i = vqdmulhq_s32 (q_tmp1, q2_tw.val[0]);
                q_tmp2 = vqdmulhq_s32 (q_tmp1, q2_tw.val[1]);
                q_tmp3 = vqdmulhq_s32 (q_tmp0, q2_tw.val[1]);
                q_fok_r = vaddq_s32 (q_fok_r, q_tmp2);
                q_fok_i = vsubq_s32 (q_fok_i, q_tmp3);

                q_dst2_r = vhsubq_s32 (q_fek_r, q_fok_r);
                q_dst2_i = vhsubq_s32 (q_fok_i, q_fek_i);
                q2_dst.val[0] = vhaddq_s32 (q_fek_r, q_fok_r);
                q2_dst.val[1] = vhaddq_s32 (q_fek_i, q_fok_i);
                q_dst2_r = vrev64q_s32 (q_dst2_r);
                q_dst2_i = vrev64q_s32 (q_dst2_i);
                q2_dst2.val[0] = vcombine_s32 (vget_high_s32 (q_dst2_r), vget_low_s32 (q_dst2_r));
                q2_dst2.val[1] = vcombine_s32 (vget_high_s32 (q_dst2_i), vget_low_s32 (q_dst2_i));
                vst2q_s32 (p_dst, q2_dst);
                vst2q_s32 (p_dst2, q2_dst2);

            }

        }
        else
        {
            for (k = 1; k <= count ; k += 4)
            {
                p_src  = (int32_t*) (& (src[k]));
                p_src2  = (int32_t*) (& (src[ncfft - k - 3]));
                p_twiddles  = (int32_t*) (& (twiddles[k - 1]));
                p_dst  = (int32_t*) (& (dst[k]));
                p_dst2  = (int32_t*) (& (dst[ncfft - k - 3]));

                q2_fk  = vld2q_s32 (p_src);
                q2_fnkc = vld2q_s32 (p_src2);
                q2_tw = vld2q_s32 (p_twiddles);
                q2_fnkc.val[0] = vrev64q_s32 (q2_fnkc.val[0]);
                q2_fnkc.val[1] = vrev64q_s32 (q2_fnkc.val[1]);
                q_fnkc_r = vcombine_s32 (vget_high_s32 (q2_fnkc.val[0]), vget_low_s32 (q2_fnkc.val[0]));
                q_fnkc_i = vcombine_s32 (vget_high_s32 (q2_fnkc.val[1]), vget_low_s32 (q2_fnkc.val[1]));
                q_fnkc_i = vnegq_s32 (q_fnkc_i);

                q_fek_r = vaddq_s32 (q2_fk.val[0], q_fnkc_r);
                q_fek_i = vaddq_s32 (q2_fk.val[1], q_fnkc_i);
                q_tmp0 = vsubq_s32 (q2_fk.val[0], q_fnkc_r);
                q_tmp1 = vsubq_s32 (q2_fk.val[1], q_fnkc_i);

                q_fok_r = vqdmulhq_s32 (q_tmp0, q2_tw.val[0]);
                q_fok_i = vqdmulhq_s32 (q_tmp1, q2_tw.val[0]);
                q_tmp2 = vqdmulhq_s32 (q_tmp1, q2_tw.val[1]);
                q_tmp3 = vqdmulhq_s32 (q_tmp0, q2_tw.val[1]);
                q_fok_r = vaddq_s32 (q_fok_r, q_tmp2);
                q_fok_i = vsubq_s32 (q_fok_i, q_tmp3);

                q_dst2_r = vhsubq_s32 (q_fek_r, q_fok_r);
                q_dst2_i = vhsubq_s32 (q_fok_i, q_fek_i);
                q2_dst.val[0] = vhaddq_s32 (q_fek_r, q_fok_r);
                q2_dst.val[1] = vhaddq_s32 (q_fek_i, q_fok_i);
                q_dst2_r = vrev64q_s32 (q_dst2_r);
                q_dst2_i = vrev64q_s32 (q_dst2_i);
                q2_dst2.val[0] = vcombine_s32 (vget_high_s32 (q_dst2_r), vget_low_s32 (q_dst2_r));
                q2_dst2.val[1] = vcombine_s32 (vget_high_s32 (q_dst2_i), vget_low_s32 (q_dst2_i));
                vst2q_s32 (p_dst, q2_dst);
                vst2q_s32 (p_dst2, q2_dst2);

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
                NE10_F2I32_FIXDIV (fk, 2);
                NE10_F2I32_FIXDIV (fnkc, 2);
            }

            fek.r = fk.r + fnkc.r;
            fek.i = fk.i + fnkc.i;

            tmp.r = fk.r - fnkc.r;
            tmp.i = fk.i - fnkc.i;

            fok.r = ( ( (ne10_int32_t) ( ( (NE10_F2I32_SAMPPROD) tmp.r * (twiddles[k - 1]).r) >> 32)) + ( (ne10_int32_t) ( ( (NE10_F2I32_SAMPPROD) tmp.i * (twiddles[k - 1]).i) >> 32))) << 1;
            fok.i = ( ( (ne10_int32_t) ( ( (NE10_F2I32_SAMPPROD) tmp.i * (twiddles[k - 1]).r) >> 32)) - ( (ne10_int32_t) ( ( (NE10_F2I32_SAMPPROD) tmp.r * (twiddles[k - 1]).i) >> 32))) << 1;

            dst[k].r = (fek.r + fok.r) >> 1;
            dst[k].i = (fek.i + fok.i) >> 1;

            dst[ncfft - k].r = (fek.r - fok.r) >> 1;
            dst[ncfft - k].i = (fok.i - fek.i) >> 1;
        }
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
 * @param[in]   cfg              point to the config struct
 * @param[in]   inverse_fft      the flag of IFFT, 0: FFT, 1: IFFT
 * @param[in]   scaled_flag      scale flag, 0 unscaled, 1 scaled
 * @return none.
 * The function implements a mixed radix-2/4 complex FFT/IFFT. The length of 2^N(N is 3, 4, 5, 6 ....etc) is supported.
 * Otherwise, this FFT is an out-of-place algorithm. When you want to get an in-place FFT, it creates a temp buffer as
 *  output buffer and then copies the temp buffer back to input buffer. For the usage of this function, please check test/test_suite_fft_int32.c
 */

void ne10_fft_c2c_1d_int32_neon (ne10_fft_cpx_int32_t *fout,
                                 ne10_fft_cpx_int32_t *fin,
                                 ne10_fft_cfg_int32_t cfg,
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
                ne10_fft4_backward_int32_scaled (fout, fin);
                break;
            case 8:
                ne10_fft8_backward_int32_scaled (fout, fin);
                break;
            case 16:
                ne10_fft16_backward_int32_scaled_neon (fout, fin, cfg->twiddles);
                break;
            default:
                ne10_mixed_radix_fft_backward_int32_scaled_neon (fout, fin, cfg->factors, cfg->twiddles, cfg->buffer);
                break;
            }
        }
        else
        {
            switch (cfg->nfft)
            {
            case 4:
                ne10_fft4_forward_int32_scaled (fout, fin);
                break;
            case 8:
                ne10_fft8_forward_int32_scaled (fout, fin);
                break;
            case 16:
                ne10_fft16_forward_int32_scaled_neon (fout, fin, cfg->twiddles);
                break;
            default:
                ne10_mixed_radix_fft_forward_int32_scaled_neon (fout, fin, cfg->factors, cfg->twiddles, cfg->buffer);
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
                ne10_fft4_backward_int32_unscaled (fout, fin);
                break;
            case 8:
                ne10_fft8_backward_int32_unscaled (fout, fin);
                break;
            case 16:
                ne10_fft16_backward_int32_unscaled_neon (fout, fin, cfg->twiddles);
                break;
            default:
                ne10_mixed_radix_fft_backward_int32_unscaled_neon (fout, fin, cfg->factors, cfg->twiddles, cfg->buffer);
                break;
            }
        }
        else
        {
            switch (cfg->nfft)
            {
            case 4:
                ne10_fft4_forward_int32_unscaled (fout, fin);
                break;
            case 8:
                ne10_fft8_forward_int32_unscaled (fout, fin);
                break;
            case 16:
                ne10_fft16_forward_int32_unscaled_neon (fout, fin, cfg->twiddles);
                break;
            default:
                ne10_mixed_radix_fft_forward_int32_unscaled_neon (fout, fin, cfg->factors, cfg->twiddles, cfg->buffer);
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
 * @brief Mixed radix-2/4 FFT (real to complex) of int32 data.
 * @param[out]  *fout            point to the output buffer
 * @param[in]   *fin             point to the input buffer
 * @param[in]   cfg              point to the config struct
 * @param[in]   scaled_flag      scale flag, 0 unscaled, 1 scaled
 * @return none.
 * The function implements a mixed radix-2/4 FFT (real to complex). The length of 2^N(N is 2, 3, 4, 5, 6 ....etc) is supported.
 * Otherwise, we alloc a temp buffer(the size is same as input buffer) for storing intermedia.
 * For the usage of this function, please check test/test_suite_fft_int32.c
 */
void ne10_fft_r2c_1d_int32_neon (ne10_fft_cpx_int32_t *fout,
                                 ne10_int32_t *fin,
                                 ne10_fft_r2c_cfg_int32_t cfg,
                                 ne10_int32_t scaled_flag)
{
    ne10_fft_cpx_int32_t * tmpbuf1 = cfg->buffer;
    ne10_fft_cpx_int32_t * tmpbuf2 = cfg->buffer + cfg->ncfft;
    ne10_fft_state_int32_t c2c_state;

    c2c_state.nfft = cfg->ncfft;
    c2c_state.factors = cfg->factors;
    c2c_state.twiddles = cfg->twiddles;
    c2c_state.buffer = tmpbuf2;

    ne10_fft_c2c_1d_int32_neon (tmpbuf1, (ne10_fft_cpx_int32_t*) fin, &c2c_state, 0, scaled_flag);
    ne10_fft_split_r2c_1d_int32_neon (fout, tmpbuf1,  cfg->super_twiddles, cfg->ncfft, scaled_flag);
}

/**
 * @brief Mixed radix-2/4 IFFT (complex to real) of int32 data.
 * @param[out]  *fout            point to the output buffer
 * @param[in]   *fin             point to the input buffer
 * @param[in]   cfg              point to the config struct
 * @param[in]   scaled_flag      scale flag, 0 unscaled, 1 scaled
 * @return none.
 * The function implements a mixed radix-2/4 FFT (complex to real). The length of 2^N(N is 2, 3, 4, 5, 6 ....etc) is supported.
 * Otherwise, we alloc a temp buffer(the size is same as input buffer) for storing intermedia.
 * For the usage of this function, please check test/test_suite_fft_int32.c
 */
void ne10_fft_c2r_1d_int32_neon (ne10_int32_t *fout,
                                 ne10_fft_cpx_int32_t *fin,
                                 ne10_fft_r2c_cfg_int32_t cfg,
                                 ne10_int32_t scaled_flag)
{
    ne10_fft_cpx_int32_t * tmpbuf1 = cfg->buffer;
    ne10_fft_cpx_int32_t * tmpbuf2 = cfg->buffer + cfg->ncfft;
    ne10_fft_state_int32_t c2c_state;

    c2c_state.nfft = cfg->ncfft;
    c2c_state.factors = cfg->factors;
    c2c_state.twiddles = cfg->twiddles;
    c2c_state.buffer = tmpbuf2;

    ne10_fft_split_c2r_1d_int32_neon (tmpbuf1, fin, cfg->super_twiddles, cfg->ncfft, scaled_flag);
    ne10_fft_c2c_1d_int32_neon ( (ne10_fft_cpx_int32_t*) fout, tmpbuf1, &c2c_state, 1, scaled_flag);
}

/**
 * @} end of R2C_FFT_IFFT group
 */

