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
 * NE10 Library : dsp/NE10_fft_int16.neon.c
 */

#include <arm_neon.h>

#include "NE10_types.h"
#include "NE10_macros.h"
#include "NE10_fft.h"

static inline void ne10_fft4_forward_int16_unscaled (ne10_fft_cpx_int16_t * Fout,
        ne10_fft_cpx_int16_t * Fin)

{
    ne10_int16_t s0_r, s0_i, s1_r, s1_i, s2_r, s2_i;
    ne10_int16_t tmp_r, tmp_i;

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

static inline void ne10_fft4_backward_int16_unscaled (ne10_fft_cpx_int16_t * Fout,
        ne10_fft_cpx_int16_t * Fin)

{
    ne10_int16_t s0_r, s0_i, s1_r, s1_i, s2_r, s2_i;
    ne10_int16_t tmp_r, tmp_i;

    s2_r = Fin[0].r - Fin[2].r;
    s2_i = Fin[0].i - Fin[2].i;

    tmp_r = Fin[0].r + Fin[2].r;
    tmp_i = Fin[0].i + Fin[2].i;

    s0_r = Fin[1].r + Fin[3].r;
    s0_i = Fin[1].i + Fin[3].i;

    s1_r = Fin[1].r - Fin[3].r;
    s1_i = Fin[1].i - Fin[3].i;

    Fout[2].r = (tmp_r - s0_r) >> 2;
    Fout[2].i = (tmp_i - s0_i) >> 2;
    Fout[0].r = (tmp_r + s0_r) >> 2;
    Fout[0].i = (tmp_i + s0_i) >> 2;
    Fout[1].r = (s2_r - s1_i) >> 2;
    Fout[1].i = (s2_i + s1_r) >> 2;
    Fout[3].r = (s2_r + s1_i) >> 2;
    Fout[3].i = (s2_i - s1_r) >> 2;
}
static inline void ne10_fft4_forward_int16_scaled (ne10_fft_cpx_int16_t * Fout,
        ne10_fft_cpx_int16_t * Fin)

{
    ne10_int16_t s0_r, s0_i, s1_r, s1_i, s2_r, s2_i;
    ne10_int16_t tmp_r, tmp_i;

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

static inline void ne10_fft4_backward_int16_scaled (ne10_fft_cpx_int16_t * Fout,
        ne10_fft_cpx_int16_t * Fin)

{
    ne10_int16_t s0_r, s0_i, s1_r, s1_i, s2_r, s2_i;
    ne10_int16_t tmp_r, tmp_i;

    s2_r = (Fin[0].r - Fin[2].r) >> 2;
    s2_i = (Fin[0].i - Fin[2].i) >> 2;
    tmp_r = (Fin[0].r + Fin[2].r) >> 2;
    tmp_i = (Fin[0].i + Fin[2].i) >> 2;

    s0_r = (Fin[1].r + Fin[3].r) >> 2;
    s0_i = (Fin[1].i + Fin[3].i) >> 2;
    s1_r = (Fin[1].r - Fin[3].r) >> 2;
    s1_i = (Fin[1].i - Fin[3].i) >> 2;

    Fout[2].r = (tmp_r - s0_r) >> 2;
    Fout[2].i = (tmp_i - s0_i) >> 2;
    Fout[0].r = (tmp_r + s0_r) >> 2;
    Fout[0].i = (tmp_i + s0_i) >> 2;
    Fout[1].r = (s2_r - s1_i) >> 2;
    Fout[1].i = (s2_i + s1_r) >> 2;
    Fout[3].r = (s2_r + s1_i) >> 2;
    Fout[3].i = (s2_i - s1_r) >> 2;
}
static inline void ne10_fft8_forward_int16_unscaled (ne10_fft_cpx_int16_t * Fout,
        ne10_fft_cpx_int16_t * Fin)

{
    ne10_int16_t s0_r, s0_i, s1_r, s1_i, s2_r, s2_i, s3_r, s3_i, s4_r, s4_i, s5_r, s5_i, s6_r, s6_i, s7_r, s7_i;
    ne10_int16_t t0_r, t0_i, t1_r, t1_i, t2_r, t2_i, t3_r, t3_i, t4_r, t4_i, t5_r, t5_i;
    const ne10_int16_t TW_81 = 23169;

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

    t4_r = (ne10_int16_t) ( ( (NE10_F2I16_SAMPPROD) (s3_r + s3_i) * TW_81) >> NE10_F2I16_SHIFT);
    t4_i = - (ne10_int16_t) ( ( (NE10_F2I16_SAMPPROD) (s3_r - s3_i) * TW_81) >> NE10_F2I16_SHIFT);
    t5_r = (ne10_int16_t) ( ( (NE10_F2I16_SAMPPROD) (s7_r - s7_i) * TW_81) >> NE10_F2I16_SHIFT);
    t5_i = (ne10_int16_t) ( ( (NE10_F2I16_SAMPPROD) (s7_r + s7_i) * TW_81) >> NE10_F2I16_SHIFT);

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

static inline void ne10_fft8_backward_int16_unscaled (ne10_fft_cpx_int16_t * Fout,
        ne10_fft_cpx_int16_t * Fin)

{
    ne10_int16_t s0_r, s0_i, s1_r, s1_i, s2_r, s2_i, s3_r, s3_i, s4_r, s4_i, s5_r, s5_i, s6_r, s6_i, s7_r, s7_i;
    ne10_int16_t t0_r, t0_i, t1_r, t1_i, t2_r, t2_i, t3_r, t3_i, t4_r, t4_i, t5_r, t5_i;
    const ne10_int16_t TW_81 = 23169;

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
    Fout[0].r = (t1_r + t2_r) >> 3;
    Fout[0].i = (t1_i + t2_i) >> 3;
    Fout[4].r = (t1_r - t2_r) >> 3;
    Fout[4].i = (t1_i - t2_i) >> 3;
    Fout[2].r = (t0_r - t3_i) >> 3;
    Fout[2].i = (t0_i + t3_r) >> 3;
    Fout[6].r = (t0_r + t3_i) >> 3;
    Fout[6].i = (t0_i - t3_r) >> 3;

    t4_r = (ne10_int16_t) ( ( (NE10_F2I16_SAMPPROD) (s3_r - s3_i) * TW_81) >> NE10_F2I16_SHIFT);
    t4_i = (ne10_int16_t) ( ( (NE10_F2I16_SAMPPROD) (s3_r + s3_i) * TW_81) >> NE10_F2I16_SHIFT);
    t5_r = (ne10_int16_t) ( ( (NE10_F2I16_SAMPPROD) (s7_r + s7_i) * TW_81) >> NE10_F2I16_SHIFT);
    t5_i = - (ne10_int16_t) ( ( (NE10_F2I16_SAMPPROD) (s7_r - s7_i) * TW_81) >> NE10_F2I16_SHIFT);

    t0_r = s1_r + s5_i;
    t0_i = s1_i - s5_r;
    t1_r = s1_r - s5_i;
    t1_i = s1_i + s5_r;
    t2_r = t4_r - t5_r;
    t2_i = t4_i - t5_i;
    t3_r = t4_r + t5_r;
    t3_i = t4_i + t5_i;
    Fout[1].r = (t1_r + t2_r) >> 3;
    Fout[1].i = (t1_i + t2_i) >> 3;
    Fout[5].r = (t1_r - t2_r) >> 3;
    Fout[5].i = (t1_i - t2_i) >> 3;
    Fout[3].r = (t0_r - t3_i) >> 3;
    Fout[3].i = (t0_i + t3_r) >> 3;
    Fout[7].r = (t0_r + t3_i) >> 3;
    Fout[7].i = (t0_i - t3_r) >> 3;
}
static inline void ne10_fft8_forward_int16_scaled (ne10_fft_cpx_int16_t * Fout,
        ne10_fft_cpx_int16_t * Fin)

{
    ne10_int16_t s0_r, s0_i, s1_r, s1_i, s2_r, s2_i, s3_r, s3_i, s4_r, s4_i, s5_r, s5_i, s6_r, s6_i, s7_r, s7_i;
    ne10_int16_t t0_r, t0_i, t1_r, t1_i, t2_r, t2_i, t3_r, t3_i, t4_r, t4_i, t5_r, t5_i;
    const ne10_int16_t TW_81 = 23169;

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

    t4_r = (ne10_int16_t) ( ( (NE10_F2I16_SAMPPROD) (s3_r + s3_i) * TW_81) >> NE10_F2I16_SHIFT);
    t4_i = - (ne10_int16_t) ( ( (NE10_F2I16_SAMPPROD) (s3_r - s3_i) * TW_81) >> NE10_F2I16_SHIFT);
    t5_r = (ne10_int16_t) ( ( (NE10_F2I16_SAMPPROD) (s7_r - s7_i) * TW_81) >> NE10_F2I16_SHIFT);
    t5_i = (ne10_int16_t) ( ( (NE10_F2I16_SAMPPROD) (s7_r + s7_i) * TW_81) >> NE10_F2I16_SHIFT);

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

static inline void ne10_fft8_backward_int16_scaled (ne10_fft_cpx_int16_t * Fout,
        ne10_fft_cpx_int16_t * Fin)

{
    ne10_int16_t s0_r, s0_i, s1_r, s1_i, s2_r, s2_i, s3_r, s3_i, s4_r, s4_i, s5_r, s5_i, s6_r, s6_i, s7_r, s7_i;
    ne10_int16_t t0_r, t0_i, t1_r, t1_i, t2_r, t2_i, t3_r, t3_i, t4_r, t4_i, t5_r, t5_i;
    const ne10_int16_t TW_81 = 23169;

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
    Fout[0].r = (t1_r + t2_r) >> 3;
    Fout[0].i = (t1_i + t2_i) >> 3;
    Fout[4].r = (t1_r - t2_r) >> 3;
    Fout[4].i = (t1_i - t2_i) >> 3;
    Fout[2].r = (t0_r - t3_i) >> 3;
    Fout[2].i = (t0_i + t3_r) >> 3;
    Fout[6].r = (t0_r + t3_i) >> 3;
    Fout[6].i = (t0_i - t3_r) >> 3;

    t4_r = (ne10_int16_t) ( ( (NE10_F2I16_SAMPPROD) (s3_r - s3_i) * TW_81) >> NE10_F2I16_SHIFT);
    t4_i = (ne10_int16_t) ( ( (NE10_F2I16_SAMPPROD) (s3_r + s3_i) * TW_81) >> NE10_F2I16_SHIFT);
    t5_r = (ne10_int16_t) ( ( (NE10_F2I16_SAMPPROD) (s7_r + s7_i) * TW_81) >> NE10_F2I16_SHIFT);
    t5_i = - (ne10_int16_t) ( ( (NE10_F2I16_SAMPPROD) (s7_r - s7_i) * TW_81) >> NE10_F2I16_SHIFT);

    t0_r = s1_r + s5_i;
    t0_i = s1_i - s5_r;
    t1_r = s1_r - s5_i;
    t1_i = s1_i + s5_r;
    t2_r = t4_r - t5_r;
    t2_i = t4_i - t5_i;
    t3_r = t4_r + t5_r;
    t3_i = t4_i + t5_i;
    Fout[1].r = (t1_r + t2_r) >> 3;
    Fout[1].i = (t1_i + t2_i) >> 3;
    Fout[5].r = (t1_r - t2_r) >> 3;
    Fout[5].i = (t1_i - t2_i) >> 3;
    Fout[3].r = (t0_r - t3_i) >> 3;
    Fout[3].i = (t0_i + t3_r) >> 3;
    Fout[7].r = (t0_r + t3_i) >> 3;
    Fout[7].i = (t0_i - t3_r) >> 3;
}
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
 * @brief Mixed radix-2/4 FFT (real to complex) of int16 data.
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
 * @brief Mixed radix-2/4 IFFT (complex to real) of int16 data.
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
