/*
 *  Copyright 2013 ARM Limited
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

#include <arm_neon.h>

#include "NE10.h"
#include "NE10_mask_table.h"

#define INTER_RESIZE_COEF_BITS (11)
#define INTER_RESIZE_COEF_SCALE (1 << INTER_RESIZE_COEF_BITS)
#define BITS (INTER_RESIZE_COEF_BITS*2)
#define DELTA (1 << (INTER_RESIZE_COEF_BITS*2 - 1))

void ne10_img_hresize_4channels_linear_neon (const unsigned char** src, int** dst, int count,
        const int* xofs, const short* alpha,
        int swidth, int dwidth, int cn, int xmin, int xmax)
{
    int dx, k;
    int dx0 = 0;

    int16x4x2_t alpha_vec;

    uint8x8_t dS0_vec, dS1_vec;
    int16x8_t qS0_vec, qS1_vec;
    int16x4_t dS0_0123, dS0_4567, dS1_0123, dS1_4567;

    int32x4_t qT0_vec, qT1_vec;

    int16x4_t dCoeff;
    dCoeff = vdup_n_s16 (INTER_RESIZE_COEF_SCALE);

    for (k = 0; k <= count - 2; k++)
    {
        const unsigned char *S0 = src[k], *S1 = src[k + 1];
        int *D0 = dst[k], *D1 = dst[k + 1];

        for (dx = dx0; dx < xmax; dx += 4)
        {
            int sx = xofs[dx];

            alpha_vec = vld2_s16 (&alpha[dx * 2]);

            dS0_vec = vld1_u8 (&S0[sx]);
            dS1_vec = vld1_u8 (&S1[sx]);

            qS0_vec = vreinterpretq_s16_u16 (vmovl_u8 (dS0_vec));
            qS1_vec = vreinterpretq_s16_u16 (vmovl_u8 (dS1_vec));

            dS0_0123 = vget_low_s16 (qS0_vec);
            dS0_4567 = vget_high_s16 (qS0_vec);
            dS1_0123 = vget_low_s16 (qS1_vec);
            dS1_4567 = vget_high_s16 (qS1_vec);

            qT0_vec = vmull_s16 (dS0_0123, alpha_vec.val[0]);
            qT1_vec = vmull_s16 (dS1_0123, alpha_vec.val[0]);
            qT0_vec = vmlal_s16 (qT0_vec, dS0_4567, alpha_vec.val[1]);
            qT1_vec = vmlal_s16 (qT1_vec, dS1_4567, alpha_vec.val[1]);

            vst1q_s32 (&D0[dx], qT0_vec);
            vst1q_s32 (&D1[dx], qT1_vec);
        }

        for (; dx < dwidth; dx += 4)
        {
            int sx = xofs[dx];

            dS0_vec = vld1_u8 (&S0[sx]);
            dS1_vec = vld1_u8 (&S1[sx]);

            qS0_vec = vreinterpretq_s16_u16 (vmovl_u8 (dS0_vec));
            qS1_vec = vreinterpretq_s16_u16 (vmovl_u8 (dS1_vec));

            dS0_0123 = vget_low_s16 (qS0_vec);
            dS1_0123 = vget_low_s16 (qS1_vec);

            qT0_vec = vmull_s16 (dS0_0123, dCoeff);
            qT1_vec = vmull_s16 (dS1_0123, dCoeff);

            vst1q_s32 (&D0[dx], qT0_vec);
            vst1q_s32 (&D1[dx], qT1_vec);
        }
    }

    for (; k < count; k++)
    {
        const unsigned char *S = src[k];
        int *D = dst[k];
        for (dx = 0; dx < xmax; dx += 4)
        {
            int sx = xofs[dx];

            alpha_vec = vld2_s16 (&alpha[dx * 2]);

            dS0_vec = vld1_u8 (&S[sx]);
            qS0_vec = vreinterpretq_s16_u16 (vmovl_u8 (dS0_vec));

            dS0_0123 = vget_low_s16 (qS0_vec);
            dS0_4567 = vget_high_s16 (qS0_vec);

            qT0_vec = vmull_s16 (dS0_0123, alpha_vec.val[0]);
            qT0_vec = vmlal_s16 (qT0_vec, dS0_4567, alpha_vec.val[1]);

            vst1q_s32 (&D[dx], qT0_vec);
        }

        for (; dx < dwidth; dx += 4)
        {
            int sx = xofs[dx];

            dS0_vec = vld1_u8 (&S[sx]);
            qS0_vec = vreinterpretq_s16_u16 (vmovl_u8 (dS0_vec));
            dS0_0123 = vget_low_s16 (qS0_vec);
            qT0_vec = vmull_s16 (dS0_0123, dCoeff);

            vst1q_s32 (&D[dx], qT0_vec);
        }
    }
}


void ne10_img_vresize_linear_neon (const int** src, unsigned char* dst, const short* beta, int width)
{
    const int *S0 = src[0], *S1 = src[1];

    int32x4_t qS0_0123, qS0_4567, qS1_0123, qS1_4567;
    int32x4_t qT_0123, qT_4567;
    int16x4_t dT_0123, dT_4567;
    uint16x8_t qT_01234567;
    uint8x8_t dT_01234567, dDst_01234567;

    int32x2_t dBeta;
    dBeta = vset_lane_s32 ( (int) (beta[0]), dBeta, 0);
    dBeta = vset_lane_s32 ( (int) (beta[1]), dBeta, 1);

    int32x4_t qDelta, qMin, qMax;
    qDelta = vdupq_n_s32 (DELTA);
    qMin = vdupq_n_s32 (0);
    qMax = vdupq_n_s32 (255);

    int x = 0;
    for (; x <= width - 8; x += 8)
    {
        qS0_0123 = vld1q_s32 (&S0[x]);
        qS0_4567 = vld1q_s32 (&S0[x + 4]);
        qS1_0123 = vld1q_s32 (&S1[x]);
        qS1_4567 = vld1q_s32 (&S1[x + 4]);

        qT_0123 = vmulq_lane_s32 (qS0_0123, dBeta, 0);
        qT_4567 = vmulq_lane_s32 (qS0_4567, dBeta, 0);
        qT_0123 = vmlaq_lane_s32 (qT_0123, qS1_0123, dBeta, 1);
        qT_4567 = vmlaq_lane_s32 (qT_4567, qS1_4567, dBeta, 1);

        qT_0123 = vaddq_s32 (qT_0123, qDelta);
        qT_4567 = vaddq_s32 (qT_4567, qDelta);

        qT_0123 = vshrq_n_s32 (qT_0123, BITS);
        qT_4567 = vshrq_n_s32 (qT_4567, BITS);

        qT_0123 = vmaxq_s32 (qT_0123, qMin);
        qT_4567 = vmaxq_s32 (qT_4567, qMin);
        qT_0123 = vminq_s32 (qT_0123, qMax);
        qT_4567 = vminq_s32 (qT_4567, qMax);

        dT_0123 = vmovn_s32 (qT_0123);
        dT_4567 = vmovn_s32 (qT_4567);
        qT_01234567 = vreinterpretq_u16_s16 (vcombine_s16 (dT_0123, dT_4567));
        dT_01234567 = vmovn_u16 (qT_01234567);

        vst1_u8 (&dst[x], dT_01234567);
    }

    if (x < width)
    {
        uint8x8_t dMask;
        dMask = vld1_u8 ( (uint8_t *) (&ne10_img_vresize_linear_mask_residual_table[ (width - x - 1)]));
        dDst_01234567 = vld1_u8 (&dst[x]);

        qS0_0123 = vld1q_s32 (&S0[x]);
        qS0_4567 = vld1q_s32 (&S0[x + 4]);
        qS1_0123 = vld1q_s32 (&S1[x]);
        qS1_4567 = vld1q_s32 (&S1[x + 4]);

        qT_0123 = vmulq_lane_s32 (qS0_0123, dBeta, 0);
        qT_4567 = vmulq_lane_s32 (qS0_4567, dBeta, 0);
        qT_0123 = vmlaq_lane_s32 (qT_0123, qS1_0123, dBeta, 1);
        qT_4567 = vmlaq_lane_s32 (qT_4567, qS1_4567, dBeta, 1);

        qT_0123 = vaddq_s32 (qT_0123, qDelta);
        qT_4567 = vaddq_s32 (qT_4567, qDelta);

        qT_0123 = vshrq_n_s32 (qT_0123, BITS);
        qT_4567 = vshrq_n_s32 (qT_4567, BITS);

        qT_0123 = vmaxq_s32 (qT_0123, qMin);
        qT_4567 = vmaxq_s32 (qT_4567, qMin);
        qT_0123 = vminq_s32 (qT_0123, qMax);
        qT_4567 = vminq_s32 (qT_4567, qMax);

        dT_0123 = vmovn_s32 (qT_0123);
        dT_4567 = vmovn_s32 (qT_4567);
        qT_01234567 = vreinterpretq_u16_s16 (vcombine_s16 (dT_0123, dT_4567));
        dT_01234567 = vmovn_u16 (qT_01234567);

        dMask = vbsl_u8 (dMask, dT_01234567, dDst_01234567);
        vst1_u8 (&dst[x], dMask);
    }
}


