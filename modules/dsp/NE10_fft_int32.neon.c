/*
 *  Copyright 2013-16 ARM Limited and Contributors.
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
 * NE10 Library : dsp/NE10_fft_int32.neon.c
 */

#include <arm_neon.h>

#include "NE10_types.h"
#include "NE10_macros.h"
#include "NE10_fft.h"
#include "NE10_dsp.h"

static inline void ne10_fft4_forward_int32_unscaled (ne10_fft_cpx_int32_t * Fout,
        ne10_fft_cpx_int32_t * Fin)

{
    ne10_int32_t s0_r, s0_i, s1_r, s1_i, s2_r, s2_i;
    ne10_int32_t tmp_r, tmp_i;

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

static inline void ne10_fft4_backward_int32_unscaled (ne10_fft_cpx_int32_t * Fout,
        ne10_fft_cpx_int32_t * Fin)

{
    ne10_int32_t s0_r, s0_i, s1_r, s1_i, s2_r, s2_i;
    ne10_int32_t tmp_r, tmp_i;

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
static inline void ne10_fft8_forward_int32_unscaled (ne10_fft_cpx_int32_t * Fout,
        ne10_fft_cpx_int32_t * Fin)

{
    ne10_int32_t s0_r, s0_i, s1_r, s1_i, s2_r, s2_i, s3_r, s3_i, s4_r, s4_i, s5_r, s5_i, s6_r, s6_i, s7_r, s7_i;
    ne10_int32_t t0_r, t0_i, t1_r, t1_i, t2_r, t2_i, t3_r, t3_i, t4_r, t4_i, t5_r, t5_i;
    const ne10_int32_t TW_81 = 1518500249;

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

static inline void ne10_fft8_backward_int32_unscaled (ne10_fft_cpx_int32_t * Fout,
        ne10_fft_cpx_int32_t * Fin)

{
    ne10_int32_t s0_r, s0_i, s1_r, s1_i, s2_r, s2_i, s3_r, s3_i, s4_r, s4_i, s5_r, s5_i, s6_r, s6_i, s7_r, s7_i;
    ne10_int32_t t0_r, t0_i, t1_r, t1_i, t2_r, t2_i, t3_r, t3_i, t4_r, t4_i, t5_r, t5_i;
    const ne10_int32_t TW_81 = 1518500249;

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
static inline void ne10_fft8_forward_int32_scaled (ne10_fft_cpx_int32_t * Fout,
        ne10_fft_cpx_int32_t * Fin)

{
    ne10_int32_t s0_r, s0_i, s1_r, s1_i, s2_r, s2_i, s3_r, s3_i, s4_r, s4_i, s5_r, s5_i, s6_r, s6_i, s7_r, s7_i;
    ne10_int32_t t0_r, t0_i, t1_r, t1_i, t2_r, t2_i, t3_r, t3_i, t4_r, t4_i, t5_r, t5_i;
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
    ne10_int32_t s0_r, s0_i, s1_r, s1_i, s2_r, s2_i, s3_r, s3_i, s4_r, s4_i, s5_r, s5_i, s6_r, s6_i, s7_r, s7_i;
    ne10_int32_t t0_r, t0_i, t1_r, t1_i, t2_r, t2_i, t3_r, t3_i, t4_r, t4_i, t5_r, t5_i;
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

static void ne10_fft16_forward_int32_unscaled_neon (ne10_fft_cpx_int32_t * Fout,
        ne10_fft_cpx_int32_t * Fin,
        ne10_fft_cpx_int32_t * twiddles)
{
    ne10_fft_cpx_int32_t *tw1, *tw2, *tw3;

    // the first stage
    int32_t *p_src0, *p_src4, *p_src8, *p_src12;
    int32x4x2_t q2_in_0123, q2_in_4567, q2_in_89ab, q2_in_cdef;
    int32x4_t q_t0_r,  q_t0_i, q_t1_r,  q_t1_i, q_t2_r,  q_t2_i, q_t3_r, q_t3_i;
    int32x4_t q_out_r048c,  q_out_i048c, q_out_r159d,  q_out_i159d;
    int32x4_t q_out_r26ae,  q_out_i26ae, q_out_r37bf,  q_out_i37bf;
    p_src0 = (int32_t*) (& (Fin[0]));
    p_src4 = (int32_t*) (& (Fin[4]));
    p_src8 = (int32_t*) (& (Fin[8]));
    p_src12 = (int32_t*) (& (Fin[12]));
    q2_in_0123 = vld2q_s32 (p_src0);
    q2_in_4567 = vld2q_s32 (p_src4);
    q2_in_89ab = vld2q_s32 (p_src8);
    q2_in_cdef = vld2q_s32 (p_src12);

    q_t2_r = vsubq_s32 (q2_in_0123.val[0], q2_in_89ab.val[0]);
    q_t2_i = vsubq_s32 (q2_in_0123.val[1], q2_in_89ab.val[1]);
    q_t3_r = vaddq_s32 (q2_in_0123.val[0], q2_in_89ab.val[0]);
    q_t3_i = vaddq_s32 (q2_in_0123.val[1], q2_in_89ab.val[1]);

    q_t0_r = vaddq_s32 (q2_in_4567.val[0], q2_in_cdef.val[0]);
    q_t0_i = vaddq_s32 (q2_in_4567.val[1], q2_in_cdef.val[1]);
    q_t1_r = vsubq_s32 (q2_in_4567.val[0], q2_in_cdef.val[0]);
    q_t1_i = vsubq_s32 (q2_in_4567.val[1], q2_in_cdef.val[1]);

    q_out_r26ae = vsubq_s32 (q_t3_r, q_t0_r);
    q_out_i26ae = vsubq_s32 (q_t3_i, q_t0_i);
    q_out_r048c = vaddq_s32 (q_t3_r, q_t0_r);
    q_out_i048c = vaddq_s32 (q_t3_i, q_t0_i);
    q_out_r159d = vaddq_s32 (q_t2_r, q_t1_i);
    q_out_i159d = vsubq_s32 (q_t2_i, q_t1_r);
    q_out_r37bf = vsubq_s32 (q_t2_r, q_t1_i);
    q_out_i37bf = vaddq_s32 (q_t2_i, q_t1_r);

    // second stages
    int32_t *p_dst0, *p_dst1, *p_dst2, *p_dst3;
    int32_t *p_tw1, *p_tw2, *p_tw3;
    int32x4_t q_s0_r, q_s0_i, q_s1_r, q_s1_i, q_s2_r, q_s2_i;
    int32x4_t q_s3_r, q_s3_i, q_s4_r, q_s4_i, q_s5_r, q_s5_i;
    int32x4x2_t q2_tmp_0, q2_tmp_1, q2_tmp_2, q2_tmp_3;
    int32x4_t q_in_r0123, q_in_r4567, q_in_r89ab, q_in_rcdef;
    int32x4_t q_in_i0123, q_in_i4567, q_in_i89ab, q_in_icdef;
    int32x4x2_t q2_tw1, q2_tw2, q2_tw3;
    int32x4_t q_tmp0, q_tmp1, q_tmp2, q_tmp3, q_tmp4, q_tmp5;
    int32x4x2_t q2_out_0123, q2_out_4567, q2_out_89ab, q2_out_cdef;
    tw1 = twiddles;
    tw2 = twiddles + 4;
    tw3 = twiddles + 8;
    p_dst0 = (int32_t*) (&Fout[0]);
    p_dst1 = (int32_t*) (&Fout[4]);
    p_dst2 = (int32_t*) (&Fout[8]);
    p_dst3 = (int32_t*) (&Fout[12]);
    p_tw1 = (int32_t*) tw1;
    p_tw2 = (int32_t*) tw2;
    p_tw3 = (int32_t*) tw3;
    q2_tmp_0 = vzipq_s32 (q_out_r048c, q_out_r159d);
    q2_tmp_1 = vzipq_s32 (q_out_i048c, q_out_i159d);
    q2_tmp_2 = vzipq_s32 (q_out_r26ae, q_out_r37bf);
    q2_tmp_3 = vzipq_s32 (q_out_i26ae, q_out_i37bf);
    q_in_r0123 = vcombine_s32 (vget_low_s32 (q2_tmp_0.val[0]), vget_low_s32 (q2_tmp_2.val[0]));
    q_in_i0123 = vcombine_s32 (vget_low_s32 (q2_tmp_1.val[0]), vget_low_s32 (q2_tmp_3.val[0]));
    q_in_r4567 = vcombine_s32 (vget_high_s32 (q2_tmp_0.val[0]), vget_high_s32 (q2_tmp_2.val[0]));
    q_in_i4567 = vcombine_s32 (vget_high_s32 (q2_tmp_1.val[0]), vget_high_s32 (q2_tmp_3.val[0]));
    q_in_r89ab = vcombine_s32 (vget_low_s32 (q2_tmp_0.val[1]), vget_low_s32 (q2_tmp_2.val[1]));
    q_in_i89ab = vcombine_s32 (vget_low_s32 (q2_tmp_1.val[1]), vget_low_s32 (q2_tmp_3.val[1]));
    q_in_rcdef = vcombine_s32 (vget_high_s32 (q2_tmp_0.val[1]), vget_high_s32 (q2_tmp_2.val[1]));
    q_in_icdef = vcombine_s32 (vget_high_s32 (q2_tmp_1.val[1]), vget_high_s32 (q2_tmp_3.val[1]));
    q2_tw1 = vld2q_s32 (p_tw1);
    q2_tw2 = vld2q_s32 (p_tw2);
    q2_tw3 = vld2q_s32 (p_tw3);

    q_s0_r = vqrdmulhq_s32 (q_in_r4567, q2_tw1.val[0]);
    q_s0_i = vqrdmulhq_s32 (q_in_r4567, q2_tw1.val[1]);
    q_s1_r = vqrdmulhq_s32 (q_in_r89ab, q2_tw2.val[0]);
    q_s1_i = vqrdmulhq_s32 (q_in_r89ab, q2_tw2.val[1]);
    q_s2_r = vqrdmulhq_s32 (q_in_rcdef, q2_tw3.val[0]);
    q_s2_i = vqrdmulhq_s32 (q_in_rcdef, q2_tw3.val[1]);
    q_tmp0 = vqrdmulhq_s32 (q_in_i4567, q2_tw1.val[1]);
    q_tmp1 = vqrdmulhq_s32 (q_in_i4567, q2_tw1.val[0]);
    q_tmp2 = vqrdmulhq_s32 (q_in_i89ab, q2_tw2.val[1]);
    q_tmp3 = vqrdmulhq_s32 (q_in_i89ab, q2_tw2.val[0]);
    q_tmp4 = vqrdmulhq_s32 (q_in_icdef, q2_tw3.val[1]);
    q_tmp5 = vqrdmulhq_s32 (q_in_icdef, q2_tw3.val[0]);
    q_s0_r = vsubq_s32 (q_s0_r, q_tmp0);
    q_s0_i = vaddq_s32 (q_s0_i, q_tmp1);
    q_s1_r = vsubq_s32 (q_s1_r, q_tmp2);
    q_s1_i = vaddq_s32 (q_s1_i, q_tmp3);
    q_s2_r = vsubq_s32 (q_s2_r, q_tmp4);
    q_s2_i = vaddq_s32 (q_s2_i, q_tmp5);

    q_s5_r = vsubq_s32 (q_in_r0123, q_s1_r);
    q_s5_i = vsubq_s32 (q_in_i0123, q_s1_i);
    q2_out_0123.val[0] = vaddq_s32 (q_in_r0123, q_s1_r);
    q2_out_0123.val[1] = vaddq_s32 (q_in_i0123, q_s1_i);

    q_s3_r = vaddq_s32 (q_s0_r, q_s2_r);
    q_s3_i = vaddq_s32 (q_s0_i, q_s2_i);
    q_s4_r = vsubq_s32 (q_s0_r, q_s2_r);
    q_s4_i = vsubq_s32 (q_s0_i, q_s2_i);

    q2_out_89ab.val[0] = vsubq_s32 (q2_out_0123.val[0], q_s3_r);
    q2_out_89ab.val[1] = vsubq_s32 (q2_out_0123.val[1], q_s3_i);
    q2_out_0123.val[0] = vaddq_s32 (q2_out_0123.val[0], q_s3_r);
    q2_out_0123.val[1] = vaddq_s32 (q2_out_0123.val[1], q_s3_i);

    q2_out_4567.val[0] = vaddq_s32 (q_s5_r, q_s4_i);
    q2_out_4567.val[1] = vsubq_s32 (q_s5_i, q_s4_r);
    q2_out_cdef.val[0] = vsubq_s32 (q_s5_r, q_s4_i);
    q2_out_cdef.val[1] = vaddq_s32 (q_s5_i, q_s4_r);

    vst2q_s32 (p_dst0, q2_out_0123);
    vst2q_s32 (p_dst1, q2_out_4567);
    vst2q_s32 (p_dst2, q2_out_89ab);
    vst2q_s32 (p_dst3, q2_out_cdef);
}

static void ne10_fft16_backward_int32_unscaled_neon (ne10_fft_cpx_int32_t * Fout,
        ne10_fft_cpx_int32_t * Fin,
        ne10_fft_cpx_int32_t * twiddles)
{
    ne10_fft_cpx_int32_t *tw1, *tw2, *tw3;

    // the first stage
    int32_t *p_src0, *p_src4, *p_src8, *p_src12;
    int32x4x2_t q2_in_0123, q2_in_4567, q2_in_89ab, q2_in_cdef;
    int32x4_t q_t0_r,  q_t0_i, q_t1_r,  q_t1_i, q_t2_r,  q_t2_i, q_t3_r, q_t3_i;
    int32x4_t q_out_r048c,  q_out_i048c, q_out_r159d,  q_out_i159d;
    int32x4_t q_out_r26ae,  q_out_i26ae, q_out_r37bf,  q_out_i37bf;
    p_src0 = (int32_t*) (& (Fin[0]));
    p_src4 = (int32_t*) (& (Fin[4]));
    p_src8 = (int32_t*) (& (Fin[8]));
    p_src12 = (int32_t*) (& (Fin[12]));
    q2_in_0123 = vld2q_s32 (p_src0);
    q2_in_4567 = vld2q_s32 (p_src4);
    q2_in_89ab = vld2q_s32 (p_src8);
    q2_in_cdef = vld2q_s32 (p_src12);

    q_t2_r = vsubq_s32 (q2_in_0123.val[0], q2_in_89ab.val[0]);
    q_t2_i = vsubq_s32 (q2_in_0123.val[1], q2_in_89ab.val[1]);
    q_t3_r = vaddq_s32 (q2_in_0123.val[0], q2_in_89ab.val[0]);
    q_t3_i = vaddq_s32 (q2_in_0123.val[1], q2_in_89ab.val[1]);

    q_t0_r = vaddq_s32 (q2_in_4567.val[0], q2_in_cdef.val[0]);
    q_t0_i = vaddq_s32 (q2_in_4567.val[1], q2_in_cdef.val[1]);
    q_t1_r = vsubq_s32 (q2_in_4567.val[0], q2_in_cdef.val[0]);
    q_t1_i = vsubq_s32 (q2_in_4567.val[1], q2_in_cdef.val[1]);

    q_out_r26ae = vsubq_s32 (q_t3_r, q_t0_r);
    q_out_i26ae = vsubq_s32 (q_t3_i, q_t0_i);
    q_out_r048c = vaddq_s32 (q_t3_r, q_t0_r);
    q_out_i048c = vaddq_s32 (q_t3_i, q_t0_i);
    q_out_r159d = vsubq_s32 (q_t2_r, q_t1_i);
    q_out_i159d = vaddq_s32 (q_t2_i, q_t1_r);
    q_out_r37bf = vaddq_s32 (q_t2_r, q_t1_i);
    q_out_i37bf = vsubq_s32 (q_t2_i, q_t1_r);

    // second stages
    int32_t *p_dst0, *p_dst1, *p_dst2, *p_dst3;
    int32_t *p_tw1, *p_tw2, *p_tw3;
    int32x4_t q_s0_r, q_s0_i, q_s1_r, q_s1_i, q_s2_r, q_s2_i;
    int32x4_t q_s3_r, q_s3_i, q_s4_r, q_s4_i, q_s5_r, q_s5_i;
    int32x4x2_t q2_tmp_0, q2_tmp_1, q2_tmp_2, q2_tmp_3;
    int32x4_t q_in_r0123, q_in_r4567, q_in_r89ab, q_in_rcdef;
    int32x4_t q_in_i0123, q_in_i4567, q_in_i89ab, q_in_icdef;
    int32x4x2_t q2_tw1, q2_tw2, q2_tw3;
    int32x4_t q_tmp0, q_tmp1, q_tmp2, q_tmp3, q_tmp4, q_tmp5;
    int32x4x2_t q2_out_0123, q2_out_4567, q2_out_89ab, q2_out_cdef;
    tw1 = twiddles;
    tw2 = twiddles + 4;
    tw3 = twiddles + 8;
    p_dst0 = (int32_t*) (&Fout[0]);
    p_dst1 = (int32_t*) (&Fout[4]);
    p_dst2 = (int32_t*) (&Fout[8]);
    p_dst3 = (int32_t*) (&Fout[12]);
    p_tw1 = (int32_t*) tw1;
    p_tw2 = (int32_t*) tw2;
    p_tw3 = (int32_t*) tw3;
    q2_tmp_0 = vzipq_s32 (q_out_r048c, q_out_r159d);
    q2_tmp_1 = vzipq_s32 (q_out_i048c, q_out_i159d);
    q2_tmp_2 = vzipq_s32 (q_out_r26ae, q_out_r37bf);
    q2_tmp_3 = vzipq_s32 (q_out_i26ae, q_out_i37bf);
    q_in_r0123 = vcombine_s32 (vget_low_s32 (q2_tmp_0.val[0]), vget_low_s32 (q2_tmp_2.val[0]));
    q_in_i0123 = vcombine_s32 (vget_low_s32 (q2_tmp_1.val[0]), vget_low_s32 (q2_tmp_3.val[0]));
    q_in_r4567 = vcombine_s32 (vget_high_s32 (q2_tmp_0.val[0]), vget_high_s32 (q2_tmp_2.val[0]));
    q_in_i4567 = vcombine_s32 (vget_high_s32 (q2_tmp_1.val[0]), vget_high_s32 (q2_tmp_3.val[0]));
    q_in_r89ab = vcombine_s32 (vget_low_s32 (q2_tmp_0.val[1]), vget_low_s32 (q2_tmp_2.val[1]));
    q_in_i89ab = vcombine_s32 (vget_low_s32 (q2_tmp_1.val[1]), vget_low_s32 (q2_tmp_3.val[1]));
    q_in_rcdef = vcombine_s32 (vget_high_s32 (q2_tmp_0.val[1]), vget_high_s32 (q2_tmp_2.val[1]));
    q_in_icdef = vcombine_s32 (vget_high_s32 (q2_tmp_1.val[1]), vget_high_s32 (q2_tmp_3.val[1]));
    q2_tw1 = vld2q_s32 (p_tw1);
    q2_tw2 = vld2q_s32 (p_tw2);
    q2_tw3 = vld2q_s32 (p_tw3);

    q_s0_r = vqrdmulhq_s32 (q_in_r4567, q2_tw1.val[0]);
    q_s0_i = vqrdmulhq_s32 (q_in_i4567, q2_tw1.val[0]);
    q_s1_r = vqrdmulhq_s32 (q_in_r89ab, q2_tw2.val[0]);
    q_s1_i = vqrdmulhq_s32 (q_in_i89ab, q2_tw2.val[0]);
    q_s2_r = vqrdmulhq_s32 (q_in_rcdef, q2_tw3.val[0]);
    q_s2_i = vqrdmulhq_s32 (q_in_icdef, q2_tw3.val[0]);
    q_tmp0 = vqrdmulhq_s32 (q_in_i4567, q2_tw1.val[1]);
    q_tmp1 = vqrdmulhq_s32 (q_in_r4567, q2_tw1.val[1]);
    q_tmp2 = vqrdmulhq_s32 (q_in_i89ab, q2_tw2.val[1]);
    q_tmp3 = vqrdmulhq_s32 (q_in_r89ab, q2_tw2.val[1]);
    q_tmp4 = vqrdmulhq_s32 (q_in_icdef, q2_tw3.val[1]);
    q_tmp5 = vqrdmulhq_s32 (q_in_rcdef, q2_tw3.val[1]);
    q_s0_r = vaddq_s32 (q_s0_r, q_tmp0);
    q_s0_i = vsubq_s32 (q_s0_i, q_tmp1);
    q_s1_r = vaddq_s32 (q_s1_r, q_tmp2);
    q_s1_i = vsubq_s32 (q_s1_i, q_tmp3);
    q_s2_r = vaddq_s32 (q_s2_r, q_tmp4);
    q_s2_i = vsubq_s32 (q_s2_i, q_tmp5);

    q_s5_r = vsubq_s32 (q_in_r0123, q_s1_r);
    q_s5_i = vsubq_s32 (q_in_i0123, q_s1_i);
    q2_out_0123.val[0] = vaddq_s32 (q_in_r0123, q_s1_r);
    q2_out_0123.val[1] = vaddq_s32 (q_in_i0123, q_s1_i);

    q_s3_r = vaddq_s32 (q_s0_r, q_s2_r);
    q_s3_i = vaddq_s32 (q_s0_i, q_s2_i);
    q_s4_r = vsubq_s32 (q_s0_r, q_s2_r);
    q_s4_i = vsubq_s32 (q_s0_i, q_s2_i);

    q2_out_89ab.val[0] = vsubq_s32 (q2_out_0123.val[0], q_s3_r);
    q2_out_89ab.val[1] = vsubq_s32 (q2_out_0123.val[1], q_s3_i);
    q2_out_0123.val[0] = vaddq_s32 (q2_out_0123.val[0], q_s3_r);
    q2_out_0123.val[1] = vaddq_s32 (q2_out_0123.val[1], q_s3_i);

    q2_out_4567.val[0] = vsubq_s32 (q_s5_r, q_s4_i);
    q2_out_4567.val[1] = vaddq_s32 (q_s5_i, q_s4_r);
    q2_out_cdef.val[0] = vaddq_s32 (q_s5_r, q_s4_i);
    q2_out_cdef.val[1] = vsubq_s32 (q_s5_i, q_s4_r);

    vst2q_s32 (p_dst0, q2_out_0123);
    vst2q_s32 (p_dst1, q2_out_4567);
    vst2q_s32 (p_dst2, q2_out_89ab);
    vst2q_s32 (p_dst3, q2_out_cdef);
}

static void ne10_fft16_forward_int32_scaled_neon (ne10_fft_cpx_int32_t * Fout,
        ne10_fft_cpx_int32_t * Fin,
        ne10_fft_cpx_int32_t * twiddles)
{
    ne10_fft_cpx_int32_t *tw1, *tw2, *tw3;

    // the first stage
    int32_t *p_src0, *p_src4, *p_src8, *p_src12;
    int32x4x2_t q2_in_0123, q2_in_4567, q2_in_89ab, q2_in_cdef;
    int32x4_t q_t0_r,  q_t0_i, q_t1_r,  q_t1_i, q_t2_r,  q_t2_i, q_t3_r, q_t3_i;
    int32x4_t q_out_r048c,  q_out_i048c, q_out_r159d,  q_out_i159d;
    int32x4_t q_out_r26ae,  q_out_i26ae, q_out_r37bf,  q_out_i37bf;
    p_src0 = (int32_t*) (& (Fin[0]));
    p_src4 = (int32_t*) (& (Fin[4]));
    p_src8 = (int32_t*) (& (Fin[8]));
    p_src12 = (int32_t*) (& (Fin[12]));
    q2_in_0123 = vld2q_s32 (p_src0);
    q2_in_4567 = vld2q_s32 (p_src4);
    q2_in_89ab = vld2q_s32 (p_src8);
    q2_in_cdef = vld2q_s32 (p_src12);

    q_t2_r = vhsubq_s32 (q2_in_0123.val[0], q2_in_89ab.val[0]);
    q_t2_i = vhsubq_s32 (q2_in_0123.val[1], q2_in_89ab.val[1]);
    q_t3_r = vhaddq_s32 (q2_in_0123.val[0], q2_in_89ab.val[0]);
    q_t3_i = vhaddq_s32 (q2_in_0123.val[1], q2_in_89ab.val[1]);

    q_t0_r = vhaddq_s32 (q2_in_4567.val[0], q2_in_cdef.val[0]);
    q_t0_i = vhaddq_s32 (q2_in_4567.val[1], q2_in_cdef.val[1]);
    q_t1_r = vhsubq_s32 (q2_in_4567.val[0], q2_in_cdef.val[0]);
    q_t1_i = vhsubq_s32 (q2_in_4567.val[1], q2_in_cdef.val[1]);

    q_out_r26ae = vhsubq_s32 (q_t3_r, q_t0_r);
    q_out_i26ae = vhsubq_s32 (q_t3_i, q_t0_i);
    q_out_r048c = vhaddq_s32 (q_t3_r, q_t0_r);
    q_out_i048c = vhaddq_s32 (q_t3_i, q_t0_i);
    q_out_r159d = vhaddq_s32 (q_t2_r, q_t1_i);
    q_out_i159d = vhsubq_s32 (q_t2_i, q_t1_r);
    q_out_r37bf = vhsubq_s32 (q_t2_r, q_t1_i);
    q_out_i37bf = vhaddq_s32 (q_t2_i, q_t1_r);


    // second stages
    int32_t *p_dst0, *p_dst1, *p_dst2, *p_dst3;
    int32_t *p_tw1, *p_tw2, *p_tw3;
    int32x4_t q_s0_r, q_s0_i, q_s1_r, q_s1_i, q_s2_r, q_s2_i;
    int32x4_t q_s3_r, q_s3_i, q_s4_r, q_s4_i, q_s5_r, q_s5_i;
    int32x4x2_t q2_tmp_0, q2_tmp_1, q2_tmp_2, q2_tmp_3;
    int32x4_t q_in_r0123, q_in_r4567, q_in_r89ab, q_in_rcdef;
    int32x4_t q_in_i0123, q_in_i4567, q_in_i89ab, q_in_icdef;
    int32x4x2_t q2_tw1, q2_tw2, q2_tw3;
    int32x4_t q_tmp0, q_tmp1, q_tmp2, q_tmp3, q_tmp4, q_tmp5;
    int32x4x2_t q2_out_0123, q2_out_4567, q2_out_89ab, q2_out_cdef;
    tw1 = twiddles;
    tw2 = twiddles + 4;
    tw3 = twiddles + 8;
    p_dst0 = (int32_t*) (&Fout[0]);
    p_dst1 = (int32_t*) (&Fout[4]);
    p_dst2 = (int32_t*) (&Fout[8]);
    p_dst3 = (int32_t*) (&Fout[12]);
    p_tw1 = (int32_t*) tw1;
    p_tw2 = (int32_t*) tw2;
    p_tw3 = (int32_t*) tw3;
    q2_tmp_0 = vzipq_s32 (q_out_r048c, q_out_r159d);
    q2_tmp_1 = vzipq_s32 (q_out_i048c, q_out_i159d);
    q2_tmp_2 = vzipq_s32 (q_out_r26ae, q_out_r37bf);
    q2_tmp_3 = vzipq_s32 (q_out_i26ae, q_out_i37bf);
    q_in_r0123 = vcombine_s32 (vget_low_s32 (q2_tmp_0.val[0]), vget_low_s32 (q2_tmp_2.val[0]));
    q_in_i0123 = vcombine_s32 (vget_low_s32 (q2_tmp_1.val[0]), vget_low_s32 (q2_tmp_3.val[0]));
    q_in_r4567 = vcombine_s32 (vget_high_s32 (q2_tmp_0.val[0]), vget_high_s32 (q2_tmp_2.val[0]));
    q_in_i4567 = vcombine_s32 (vget_high_s32 (q2_tmp_1.val[0]), vget_high_s32 (q2_tmp_3.val[0]));
    q_in_r89ab = vcombine_s32 (vget_low_s32 (q2_tmp_0.val[1]), vget_low_s32 (q2_tmp_2.val[1]));
    q_in_i89ab = vcombine_s32 (vget_low_s32 (q2_tmp_1.val[1]), vget_low_s32 (q2_tmp_3.val[1]));
    q_in_rcdef = vcombine_s32 (vget_high_s32 (q2_tmp_0.val[1]), vget_high_s32 (q2_tmp_2.val[1]));
    q_in_icdef = vcombine_s32 (vget_high_s32 (q2_tmp_1.val[1]), vget_high_s32 (q2_tmp_3.val[1]));
    q2_tw1 = vld2q_s32 (p_tw1);
    q2_tw2 = vld2q_s32 (p_tw2);
    q2_tw3 = vld2q_s32 (p_tw3);

    q_s0_r = vqrdmulhq_s32 (q_in_r4567, q2_tw1.val[0]);
    q_s0_i = vqrdmulhq_s32 (q_in_r4567, q2_tw1.val[1]);
    q_s1_r = vqrdmulhq_s32 (q_in_r89ab, q2_tw2.val[0]);
    q_s1_i = vqrdmulhq_s32 (q_in_r89ab, q2_tw2.val[1]);
    q_s2_r = vqrdmulhq_s32 (q_in_rcdef, q2_tw3.val[0]);
    q_s2_i = vqrdmulhq_s32 (q_in_rcdef, q2_tw3.val[1]);
    q_tmp0 = vqrdmulhq_s32 (q_in_i4567, q2_tw1.val[1]);
    q_tmp1 = vqrdmulhq_s32 (q_in_i4567, q2_tw1.val[0]);
    q_tmp2 = vqrdmulhq_s32 (q_in_i89ab, q2_tw2.val[1]);
    q_tmp3 = vqrdmulhq_s32 (q_in_i89ab, q2_tw2.val[0]);
    q_tmp4 = vqrdmulhq_s32 (q_in_icdef, q2_tw3.val[1]);
    q_tmp5 = vqrdmulhq_s32 (q_in_icdef, q2_tw3.val[0]);

    q_s0_r = vsubq_s32 (q_s0_r, q_tmp0);
    q_s0_i = vaddq_s32 (q_s0_i, q_tmp1);
    q_s1_r = vsubq_s32 (q_s1_r, q_tmp2);
    q_s1_i = vaddq_s32 (q_s1_i, q_tmp3);
    q_s2_r = vsubq_s32 (q_s2_r, q_tmp4);
    q_s2_i = vaddq_s32 (q_s2_i, q_tmp5);

    q_s5_r = vhsubq_s32 (q_in_r0123, q_s1_r);
    q_s5_i = vhsubq_s32 (q_in_i0123, q_s1_i);
    q2_out_0123.val[0] = vhaddq_s32 (q_in_r0123, q_s1_r);
    q2_out_0123.val[1] = vhaddq_s32 (q_in_i0123, q_s1_i);

    q_s3_r = vhaddq_s32 (q_s0_r, q_s2_r);
    q_s3_i = vhaddq_s32 (q_s0_i, q_s2_i);
    q_s4_r = vhsubq_s32 (q_s0_r, q_s2_r);
    q_s4_i = vhsubq_s32 (q_s0_i, q_s2_i);

    q2_out_89ab.val[0] = vhsubq_s32 (q2_out_0123.val[0], q_s3_r);
    q2_out_89ab.val[1] = vhsubq_s32 (q2_out_0123.val[1], q_s3_i);
    q2_out_0123.val[0] = vhaddq_s32 (q2_out_0123.val[0], q_s3_r);
    q2_out_0123.val[1] = vhaddq_s32 (q2_out_0123.val[1], q_s3_i);

    q2_out_4567.val[0] = vhaddq_s32 (q_s5_r, q_s4_i);
    q2_out_4567.val[1] = vhsubq_s32 (q_s5_i, q_s4_r);
    q2_out_cdef.val[0] = vhsubq_s32 (q_s5_r, q_s4_i);
    q2_out_cdef.val[1] = vhaddq_s32 (q_s5_i, q_s4_r);

    vst2q_s32 (p_dst0, q2_out_0123);
    vst2q_s32 (p_dst1, q2_out_4567);
    vst2q_s32 (p_dst2, q2_out_89ab);
    vst2q_s32 (p_dst3, q2_out_cdef);
}

static void ne10_fft16_backward_int32_scaled_neon (ne10_fft_cpx_int32_t * Fout,
        ne10_fft_cpx_int32_t * Fin,
        ne10_fft_cpx_int32_t * twiddles)
{
    ne10_fft_cpx_int32_t *tw1, *tw2, *tw3;

    // the first stage
    int32_t *p_src0, *p_src4, *p_src8, *p_src12;
    int32x4x2_t q2_in_0123, q2_in_4567, q2_in_89ab, q2_in_cdef;
    int32x4_t q_t0_r,  q_t0_i, q_t1_r,  q_t1_i, q_t2_r,  q_t2_i, q_t3_r, q_t3_i;
    int32x4_t q_out_r048c,  q_out_i048c, q_out_r159d,  q_out_i159d;
    int32x4_t q_out_r26ae,  q_out_i26ae, q_out_r37bf,  q_out_i37bf;
    p_src0 = (int32_t*) (& (Fin[0]));
    p_src4 = (int32_t*) (& (Fin[4]));
    p_src8 = (int32_t*) (& (Fin[8]));
    p_src12 = (int32_t*) (& (Fin[12]));
    q2_in_0123 = vld2q_s32 (p_src0);
    q2_in_4567 = vld2q_s32 (p_src4);
    q2_in_89ab = vld2q_s32 (p_src8);
    q2_in_cdef = vld2q_s32 (p_src12);

    q_t2_r = vhsubq_s32 (q2_in_0123.val[0], q2_in_89ab.val[0]);
    q_t2_i = vhsubq_s32 (q2_in_0123.val[1], q2_in_89ab.val[1]);
    q_t3_r = vhaddq_s32 (q2_in_0123.val[0], q2_in_89ab.val[0]);
    q_t3_i = vhaddq_s32 (q2_in_0123.val[1], q2_in_89ab.val[1]);

    q_t0_r = vhaddq_s32 (q2_in_4567.val[0], q2_in_cdef.val[0]);
    q_t0_i = vhaddq_s32 (q2_in_4567.val[1], q2_in_cdef.val[1]);
    q_t1_r = vhsubq_s32 (q2_in_4567.val[0], q2_in_cdef.val[0]);
    q_t1_i = vhsubq_s32 (q2_in_4567.val[1], q2_in_cdef.val[1]);

    q_out_r26ae = vhsubq_s32 (q_t3_r, q_t0_r);
    q_out_i26ae = vhsubq_s32 (q_t3_i, q_t0_i);
    q_out_r048c = vhaddq_s32 (q_t3_r, q_t0_r);
    q_out_i048c = vhaddq_s32 (q_t3_i, q_t0_i);
    q_out_r159d = vhsubq_s32 (q_t2_r, q_t1_i);
    q_out_i159d = vhaddq_s32 (q_t2_i, q_t1_r);
    q_out_r37bf = vhaddq_s32 (q_t2_r, q_t1_i);
    q_out_i37bf = vhsubq_s32 (q_t2_i, q_t1_r);

    // second stages
    int32_t *p_dst0, *p_dst1, *p_dst2, *p_dst3;
    int32_t *p_tw1, *p_tw2, *p_tw3;
    int32x4_t q_s0_r, q_s0_i, q_s1_r, q_s1_i, q_s2_r, q_s2_i;
    int32x4_t q_s3_r, q_s3_i, q_s4_r, q_s4_i, q_s5_r, q_s5_i;
    int32x4x2_t q2_tmp_0, q2_tmp_1, q2_tmp_2, q2_tmp_3;
    int32x4_t q_in_r0123, q_in_r4567, q_in_r89ab, q_in_rcdef;
    int32x4_t q_in_i0123, q_in_i4567, q_in_i89ab, q_in_icdef;
    int32x4x2_t q2_tw1, q2_tw2, q2_tw3;
    int32x4_t q_tmp0, q_tmp1, q_tmp2, q_tmp3, q_tmp4, q_tmp5;
    int32x4x2_t q2_out_0123, q2_out_4567, q2_out_89ab, q2_out_cdef;
    tw1 = twiddles;
    tw2 = twiddles + 4;
    tw3 = twiddles + 8;
    p_dst0 = (int32_t*) (&Fout[0]);
    p_dst1 = (int32_t*) (&Fout[4]);
    p_dst2 = (int32_t*) (&Fout[8]);
    p_dst3 = (int32_t*) (&Fout[12]);
    p_tw1 = (int32_t*) tw1;
    p_tw2 = (int32_t*) tw2;
    p_tw3 = (int32_t*) tw3;
    q2_tmp_0 = vzipq_s32 (q_out_r048c, q_out_r159d);
    q2_tmp_1 = vzipq_s32 (q_out_i048c, q_out_i159d);
    q2_tmp_2 = vzipq_s32 (q_out_r26ae, q_out_r37bf);
    q2_tmp_3 = vzipq_s32 (q_out_i26ae, q_out_i37bf);
    q_in_r0123 = vcombine_s32 (vget_low_s32 (q2_tmp_0.val[0]), vget_low_s32 (q2_tmp_2.val[0]));
    q_in_i0123 = vcombine_s32 (vget_low_s32 (q2_tmp_1.val[0]), vget_low_s32 (q2_tmp_3.val[0]));
    q_in_r4567 = vcombine_s32 (vget_high_s32 (q2_tmp_0.val[0]), vget_high_s32 (q2_tmp_2.val[0]));
    q_in_i4567 = vcombine_s32 (vget_high_s32 (q2_tmp_1.val[0]), vget_high_s32 (q2_tmp_3.val[0]));
    q_in_r89ab = vcombine_s32 (vget_low_s32 (q2_tmp_0.val[1]), vget_low_s32 (q2_tmp_2.val[1]));
    q_in_i89ab = vcombine_s32 (vget_low_s32 (q2_tmp_1.val[1]), vget_low_s32 (q2_tmp_3.val[1]));
    q_in_rcdef = vcombine_s32 (vget_high_s32 (q2_tmp_0.val[1]), vget_high_s32 (q2_tmp_2.val[1]));
    q_in_icdef = vcombine_s32 (vget_high_s32 (q2_tmp_1.val[1]), vget_high_s32 (q2_tmp_3.val[1]));
    q2_tw1 = vld2q_s32 (p_tw1);
    q2_tw2 = vld2q_s32 (p_tw2);
    q2_tw3 = vld2q_s32 (p_tw3);

    q_s0_r = vqrdmulhq_s32 (q_in_r4567, q2_tw1.val[0]);
    q_s0_i = vqrdmulhq_s32 (q_in_i4567, q2_tw1.val[0]);
    q_s1_r = vqrdmulhq_s32 (q_in_r89ab, q2_tw2.val[0]);
    q_s1_i = vqrdmulhq_s32 (q_in_i89ab, q2_tw2.val[0]);
    q_s2_r = vqrdmulhq_s32 (q_in_rcdef, q2_tw3.val[0]);
    q_s2_i = vqrdmulhq_s32 (q_in_icdef, q2_tw3.val[0]);
    q_tmp0 = vqrdmulhq_s32 (q_in_i4567, q2_tw1.val[1]);
    q_tmp1 = vqrdmulhq_s32 (q_in_r4567, q2_tw1.val[1]);
    q_tmp2 = vqrdmulhq_s32 (q_in_i89ab, q2_tw2.val[1]);
    q_tmp3 = vqrdmulhq_s32 (q_in_r89ab, q2_tw2.val[1]);
    q_tmp4 = vqrdmulhq_s32 (q_in_icdef, q2_tw3.val[1]);
    q_tmp5 = vqrdmulhq_s32 (q_in_rcdef, q2_tw3.val[1]);
    q_s0_r = vaddq_s32 (q_s0_r, q_tmp0);
    q_s0_i = vsubq_s32 (q_s0_i, q_tmp1);
    q_s1_r = vaddq_s32 (q_s1_r, q_tmp2);
    q_s1_i = vsubq_s32 (q_s1_i, q_tmp3);
    q_s2_r = vaddq_s32 (q_s2_r, q_tmp4);
    q_s2_i = vsubq_s32 (q_s2_i, q_tmp5);

    q_s5_r = vhsubq_s32 (q_in_r0123, q_s1_r);
    q_s5_i = vhsubq_s32 (q_in_i0123, q_s1_i);
    q2_out_0123.val[0] = vhaddq_s32 (q_in_r0123, q_s1_r);
    q2_out_0123.val[1] = vhaddq_s32 (q_in_i0123, q_s1_i);

    q_s3_r = vhaddq_s32 (q_s0_r, q_s2_r);
    q_s3_i = vhaddq_s32 (q_s0_i, q_s2_i);
    q_s4_r = vhsubq_s32 (q_s0_r, q_s2_r);
    q_s4_i = vhsubq_s32 (q_s0_i, q_s2_i);

    q2_out_89ab.val[0] = vhsubq_s32 (q2_out_0123.val[0], q_s3_r);
    q2_out_89ab.val[1] = vhsubq_s32 (q2_out_0123.val[1], q_s3_i);
    q2_out_0123.val[0] = vhaddq_s32 (q2_out_0123.val[0], q_s3_r);
    q2_out_0123.val[1] = vhaddq_s32 (q2_out_0123.val[1], q_s3_i);

    q2_out_4567.val[0] = vhsubq_s32 (q_s5_r, q_s4_i);
    q2_out_4567.val[1] = vhaddq_s32 (q_s5_i, q_s4_r);
    q2_out_cdef.val[0] = vhaddq_s32 (q_s5_r, q_s4_i);
    q2_out_cdef.val[1] = vhsubq_s32 (q_s5_i, q_s4_r);

    vst2q_s32 (p_dst0, q2_out_0123);
    vst2q_s32 (p_dst1, q2_out_4567);
    vst2q_s32 (p_dst2, q2_out_89ab);
    vst2q_s32 (p_dst3, q2_out_cdef);
}

static void ne10_fft_split_r2c_1d_int32_neon (ne10_fft_cpx_int32_t *dst,
        const ne10_fft_cpx_int32_t *src,
        ne10_fft_cpx_int32_t *twiddles,
        ne10_int32_t ncfft,
        ne10_int32_t scaled_flag)
{
    ne10_int32_t count = ncfft / 2;
    ne10_fft_cpx_int32_t fpnk, fpk, f1k, f2k, tw, tdc;
    int32x4x2_t q2_fpk, q2_fpnk, q2_tw, q2_dst, q2_dst2;
    int32x4_t q_fpnk_r, q_fpnk_i;
    int32x4_t q_f1k_r, q_f1k_i, q_f2k_r, q_f2k_i;
    int32x4_t q_tw_r, q_tw_i;
    int32x4_t q_tmp0, q_tmp1, q_tmp2, q_tmp3;
    int32x4_t q_dst2_r, q_dst2_i;

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
            for (ne10_int32_t k = 1; k <= count ; k += 4)
            {
                int32_t* p_src  = (int32_t*) (& (src[k]));
                int32_t* p_src2  = (int32_t*) (& (src[ncfft - k - 3]));
                int32_t* p_twiddles  = (int32_t*) (& (twiddles[k - 1]));
                int32_t* p_dst  = (int32_t*) (& (dst[k]));
                int32_t* p_dst2  = (int32_t*) (& (dst[ncfft - k - 3]));

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
            for (ne10_int32_t k = 1; k <= count ; k += 4)
            {
                int32_t* p_src  = (int32_t*) (& (src[k]));
                int32_t* p_src2  = (int32_t*) (& (src[ncfft - k - 3]));
                int32_t* p_twiddles  = (int32_t*) (& (twiddles[k - 1]));
                int32_t* p_dst  = (int32_t*) (& (dst[k]));
                int32_t* p_dst2  = (int32_t*) (& (dst[ncfft - k - 3]));

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

        for (ne10_int32_t k = 1; k <= ncfft / 2 ; ++k)
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

    ne10_int32_t count = ncfft / 2;
    ne10_fft_cpx_int32_t fk, fnkc, fek, fok, tmp;
    int32x4x2_t q2_fk, q2_fnkc, q2_tw, q2_dst, q2_dst2;
    int32x4_t q_fnkc_r, q_fnkc_i;
    int32x4_t q_fek_r, q_fek_i, q_fok_r, q_fok_i;
    int32x4_t q_tmp0, q_tmp1, q_tmp2, q_tmp3;
    int32x4_t q_dst2_r, q_dst2_i;


    dst[0].r = src[0].r + src[ncfft].r;
    dst[0].i = src[0].r - src[ncfft].r;
    if (scaled_flag)
        NE10_F2I32_FIXDIV (dst[0], 2);
    if (count >= 4)
    {
        if (scaled_flag)
        {
            for (ne10_int32_t k = 1; k <= count ; k += 4)
            {
                int32_t* p_src  = (int32_t*) (& (src[k]));
                int32_t* p_src2  = (int32_t*) (& (src[ncfft - k - 3]));
                int32_t* p_twiddles  = (int32_t*) (& (twiddles[k - 1]));
                int32_t* p_dst  = (int32_t*) (& (dst[k]));
                int32_t* p_dst2  = (int32_t*) (& (dst[ncfft - k - 3]));

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

                q_dst2_r = vsubq_s32 (q_fek_r, q_fok_r);
                q_dst2_i = vsubq_s32 (q_fok_i, q_fek_i);
                q2_dst.val[0] = vaddq_s32 (q_fek_r, q_fok_r);
                q2_dst.val[1] = vaddq_s32 (q_fek_i, q_fok_i);
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
            for (ne10_int32_t k = 1; k <= count ; k += 4)
            {
                int32_t* p_src  = (int32_t*) (& (src[k]));
                int32_t* p_src2  = (int32_t*) (& (src[ncfft - k - 3]));
                int32_t* p_twiddles  = (int32_t*) (& (twiddles[k - 1]));
                int32_t* p_dst  = (int32_t*) (& (dst[k]));
                int32_t* p_dst2  = (int32_t*) (& (dst[ncfft - k - 3]));

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

                q_dst2_r = vsubq_s32 (q_fek_r, q_fok_r);
                q_dst2_i = vsubq_s32 (q_fok_i, q_fek_i);
                q2_dst.val[0] = vaddq_s32 (q_fek_r, q_fok_r);
                q2_dst.val[1] = vaddq_s32 (q_fek_i, q_fok_i);
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
        for (ne10_int32_t k = 1; k <= ncfft / 2; k++)
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

            dst[k].r = fek.r + fok.r;
            dst[k].i = fek.i + fok.i;

            dst[ncfft - k].r = fek.r - fok.r;
            dst[ncfft - k].i = fok.i - fek.i;
        }
    }
}

/**
 * @ingroup C2C_FFT_IFFT
 * Specific implementation of @ref ne10_fft_c2c_1d_int32 using NEON SIMD capabilities.
 */
void ne10_fft_c2c_1d_int32_neon (ne10_fft_cpx_int32_t *fout,
                                 ne10_fft_cpx_int32_t *fin,
                                 ne10_fft_cfg_int32_t cfg,
                                 ne10_int32_t inverse_fft,
                                 ne10_int32_t scaled_flag)
{
    // For input shorter than 15, fall back to c version.
    // We would not get much improvement from NEON for these cases.
    if (cfg->nfft < 15)
    {
        ne10_fft_c2c_1d_int32_c (fout, fin, cfg, inverse_fft, scaled_flag);
        return;
    }

    ne10_int32_t stage_count = cfg->factors[0];
    ne10_int32_t algorithm_flag = cfg->factors[2 * (stage_count + 1)];

    assert ((algorithm_flag == NE10_FFT_ALG_DEFAULT)
            || (algorithm_flag == NE10_FFT_ALG_ANY));

    // For NE10_FFT_ALG_ANY.
    // Function will return inside this branch.
    if (algorithm_flag == NE10_FFT_ALG_ANY)
    {
        if (inverse_fft)
        {
            ne10_mixed_radix_generic_butterfly_inverse_int32_neon (fout, fin,
                    cfg->factors, cfg->twiddles, cfg->buffer, scaled_flag);
        }
        else
        {
            ne10_mixed_radix_generic_butterfly_int32_neon (fout, fin,
                    cfg->factors, cfg->twiddles, cfg->buffer, scaled_flag);
        }
        return;
    }

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
 * @ingroup R2C_FFT_IFFT
 * Specific implementation of @ref ne10_fft_r2c_1d_int32 using NEON SIMD capabilities.
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
 * @ingroup R2C_FFT_IFFT
 * Specific implementation of @ref ne10_fft_c2r_1d_int32 using NEON SIMD capabilities.
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
