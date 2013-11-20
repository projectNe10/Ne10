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

/*
 * NE10 Library : dsp/NE10_fft_float32.neon.c
 */

#include <arm_neon.h>

#include "NE10_types.h"
#include "NE10_macros.h"
#include "NE10_fft.h"

static void ne10_mixed_radix_butterfly_float32_neon (ne10_fft_cpx_float32_t * Fout,
        ne10_int32_t * factors,
        ne10_fft_cpx_float32_t * twiddles)
{
    ne10_int32_t i, j, mstride;
    ne10_int32_t stage_count;
    ne10_int32_t fstride;
    ne10_int32_t nfft;

    ne10_fft_cpx_float32_t tmp;
    ne10_fft_cpx_float32_t scratch[6];
    ne10_fft_cpx_float32_t *tw, *tw1, *tw2, *tw3;
    ne10_fft_cpx_float32_t * F;


    // the first stage
    stage_count = factors[0];
    fstride = factors[1];
    nfft = fstride * factors[2 * stage_count];

    switch (nfft)
    {
    case 2:
    {
        tmp.r = Fout[1].r;
        tmp.i = Fout[1].i;
        Fout[1].r = Fout[0].r - tmp.r;
        Fout[1].i = Fout[0].i - tmp.i;
        Fout[0].r = Fout[0].r + tmp.r;
        Fout[0].i = Fout[0].i + tmp.i;
        break;
    }
    case 4:
    {
        scratch[2].r = Fout[0].r - Fout[2].r;
        scratch[2].i = Fout[0].i - Fout[2].i;

        Fout[0].r += Fout[2].r;
        Fout[0].i += Fout[2].i;

        scratch[0].r = Fout[1].r + Fout[3].r;
        scratch[0].i = Fout[1].i + Fout[3].i;

        scratch[1].r = Fout[1].r - Fout[3].r;
        scratch[1].i = Fout[1].i - Fout[3].i;
        Fout[2].r = Fout[0].r - scratch[0].r;
        Fout[2].i = Fout[0].i - scratch[0].i;

        Fout[0].r += scratch[0].r;
        Fout[0].i += scratch[0].i;

        Fout[1].r = scratch[2].r + scratch[1].i;
        Fout[1].i = scratch[2].i - scratch[1].r;
        Fout[3].r = scratch[2].r - scratch[1].i;
        Fout[3].i = scratch[2].i + scratch[1].r;
        break;
    }
    case 8:
    {
        //radix 2/4, FFT length is 8
        ne10_butterfly_length8_float32_neon (Fout, factors, twiddles);
        break;
    }
    case 16:
    {
        //radix 4, FFT length is 16
        ne10_butterfly_length16_float32_neon (Fout, factors, twiddles);
        break;
    }
    default:
    {
        if (factors[2 * stage_count] == 2)
        {
            //radix 2/4, FFT length is 2^n (n is odd)
            ne10_butterfly_length_odd_power2_float32_neon (Fout, factors, twiddles);
        }
        else if (factors[2 * stage_count] == 4)
        {
            //radix 4, FFT length is 2^n (n is even)
            ne10_butterfly_length_even_power2_float32_neon (Fout, factors, twiddles);
        }
        break;
    }
    }
}

static void ne10_mixed_radix_butterfly_inverse_float32_neon (ne10_fft_cpx_float32_t * Fout,
        ne10_int32_t * factors,
        ne10_fft_cpx_float32_t * twiddles)
{
    ne10_int32_t i, j, mstride;
    ne10_int32_t stage_count;
    ne10_int32_t fstride;
    ne10_int32_t nfft;

    ne10_fft_cpx_float32_t tmp;
    ne10_fft_cpx_float32_t scratch[6];
    ne10_fft_cpx_float32_t *tw, *tw1, *tw2, *tw3;
    ne10_fft_cpx_float32_t * F;


    // the first stage
    stage_count = factors[0];
    fstride = factors[1];
    nfft = fstride * factors[2 * stage_count];

    switch (nfft)
    {
    case 2:
    {
        tmp.r = Fout[1].r;
        tmp.i = Fout[1].i;
        Fout[1].r = Fout[0].r - tmp.r;
        Fout[1].i = Fout[0].i - tmp.i;
        Fout[0].r = Fout[0].r + tmp.r;
        Fout[0].i = Fout[0].i + tmp.i;
        break;
    }
    case 4:
    {
        scratch[2].r = Fout[0].r - Fout[2].r;
        scratch[2].i = Fout[0].i - Fout[2].i;

        Fout[0].r += Fout[2].r;
        Fout[0].i += Fout[2].i;

        scratch[0].r = Fout[1].r + Fout[3].r;
        scratch[0].i = Fout[1].i + Fout[3].i;

        scratch[1].r = Fout[1].r - Fout[3].r;
        scratch[1].i = Fout[1].i - Fout[3].i;
        Fout[2].r = Fout[0].r - scratch[0].r;
        Fout[2].i = Fout[0].i - scratch[0].i;

        Fout[0].r += scratch[0].r;
        Fout[0].i += scratch[0].i;

        Fout[1].r = scratch[2].r - scratch[1].i;
        Fout[1].i = scratch[2].i + scratch[1].r;
        Fout[3].r = scratch[2].r + scratch[1].i;
        Fout[3].i = scratch[2].i - scratch[1].r;
        break;
    }
    case 8:
    {
        //radix 2/4, FFT length is 8
        ne10_butterfly_inverse_length8_float32_neon (Fout, factors, twiddles);
        break;
    }
    case 16:
    {
        //radix 4, FFT length is 16
        ne10_butterfly_inverse_length16_float32_neon (Fout, factors, twiddles);
        break;
    }
    default:
    {
        if (factors[2 * stage_count] == 2)
        {
            //radix 2/4, FFT length is 2^n (n is odd)
            ne10_butterfly_inverse_length_odd_power2_float32_neon (Fout, factors, twiddles);
        }
        else if (factors[2 * stage_count] == 4)
        {
            //radix 4, FFT length is 2^n (n is even)
            ne10_butterfly_inverse_length_even_power2_float32_neon (Fout, factors, twiddles);
        }
        break;
    }
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
    if (fin == fout)
    {
        /* NOTE: for an in-place FFT algorithm. It just performs an out-of-place FFT into a temp buffer */
        ne10_fft_cpx_float32_t * tmpbuf_ = (ne10_fft_cpx_float32_t*) NE10_MALLOC (sizeof (ne10_fft_cpx_float32_t) * nfft);

        // copy the data from input to output and bit reversal
        ne10_data_bitreversal_float32 (tmpbuf_, fin, 1, &factors[2]);

        if (inverse_fft)
            ne10_mixed_radix_butterfly_inverse_float32_neon (tmpbuf_, factors, twiddles);
        else
            ne10_mixed_radix_butterfly_float32_neon (tmpbuf_, factors, twiddles);

        memcpy (fout, tmpbuf_, sizeof (ne10_fft_cpx_float32_t) *nfft);
        NE10_FREE (tmpbuf_);
    }
    else
    {
        // copy the data from input to output and bit reversal
        ne10_data_bitreversal_float32 (fout, fin, 1, &factors[2]);

        if (inverse_fft)
            ne10_mixed_radix_butterfly_inverse_float32_neon (fout, factors, twiddles);
        else
            ne10_mixed_radix_butterfly_float32_neon (fout, factors, twiddles);
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

    // copy the data from input to output and bit reversal
    ne10_data_bitreversal_float32 (tmpbuf_, (ne10_fft_cpx_float32_t*) fin, 1, &factors[2]);
    ne10_mixed_radix_butterfly_float32_neon (tmpbuf_, factors, twiddles);

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

    // copy the data from input to output and bit reversal
    ne10_data_bitreversal_float32 ( (ne10_fft_cpx_float32_t*) fout, tmpbuf_, 1, &factors[2]);
    ne10_mixed_radix_butterfly_inverse_float32_neon ( (ne10_fft_cpx_float32_t*) fout, factors, twiddles);

    NE10_FREE (tmpbuf_);
}

/**
 * @} end of R2C_FFT_IFFT group
 */
