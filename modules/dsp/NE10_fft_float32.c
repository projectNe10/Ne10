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

/* license of Kiss FFT */
/*
Copyright (c) 2003-2010, Mark Borgerding

All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
    * Neither the author nor the names of any contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/*
 * NE10 Library : dsp/NE10_fft_float32.c
 */

#include "NE10_types.h"
#include "NE10_macros.h"
#include "NE10_fft.h"

static void ne10_mixed_radix_butterfly_float32_c (ne10_fft_cpx_float32_t * Fout,
        ne10_int32_t * factors,
        ne10_fft_cpx_float32_t * twiddles)
{
    ne10_int32_t i, j, mstride;
    ne10_int32_t stage_count;
    ne10_int32_t fstride;

    ne10_fft_cpx_float32_t tmp;
    ne10_fft_cpx_float32_t scratch[6];
    ne10_fft_cpx_float32_t *tw, *tw1, *tw2, *tw3;
    ne10_fft_cpx_float32_t * F;


    // the first stage
    stage_count = factors[0];
    fstride = factors[1];
    if (factors[2 * stage_count] == 2) // length of FFT is 2^n (n is odd)
    {
        //fstride is nfft>>1
        for (i = 0; i < fstride; i++)
        {
            tmp.r = Fout[2 * i + 1].r;
            tmp.i = Fout[2 * i + 1].i;
            Fout[2 * i + 1].r = Fout[2 * i].r - tmp.r;
            Fout[2 * i + 1].i = Fout[2 * i].i - tmp.i;
            Fout[2 * i].r = Fout[2 * i].r + tmp.r;
            Fout[2 * i].i = Fout[2 * i].i + tmp.i;
        }
    }
    else if (factors[2 * stage_count] == 4) // length of FFT is 2^n (n is even)
    {
        //fstride is nfft>>2
        for (i = 0; i < fstride; i++)
        {
            scratch[2].r = Fout[4 * i].r - Fout[4 * i + 2].r;
            scratch[2].i = Fout[4 * i].i - Fout[4 * i + 2].i;

            Fout[4 * i].r += Fout[4 * i + 2].r;
            Fout[4 * i].i += Fout[4 * i + 2].i;

            scratch[0].r = Fout[4 * i + 1].r + Fout[4 * i + 3].r;
            scratch[0].i = Fout[4 * i + 1].i + Fout[4 * i + 3].i;

            scratch[1].r = Fout[4 * i + 1].r - Fout[4 * i + 3].r;
            scratch[1].i = Fout[4 * i + 1].i - Fout[4 * i + 3].i;
            Fout[4 * i + 2].r = Fout[4 * i].r - scratch[0].r;
            Fout[4 * i + 2].i = Fout[4 * i].i - scratch[0].i;

            Fout[4 * i].r += scratch[0].r;
            Fout[4 * i].i += scratch[0].i;

            Fout[4 * i + 1].r = scratch[2].r + scratch[1].i;
            Fout[4 * i + 1].i = scratch[2].i - scratch[1].r;
            Fout[4 * i + 3].r = scratch[2].r - scratch[1].i;
            Fout[4 * i + 3].i = scratch[2].i + scratch[1].r;
        }
    }
    stage_count--;

    // other stages
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
            for (j = 0; j < mstride; j++)
            {
                scratch[0].r = F[mstride].r * tw1->r - F[mstride].i * tw1->i;
                scratch[0].i = F[mstride].r * tw1->i + F[mstride].i * tw1->r;
                scratch[1].r = F[mstride * 2].r * tw2->r - F[mstride * 2].i * tw2->i;
                scratch[1].i = F[mstride * 2].r * tw2->i + F[mstride * 2].i * tw2->r;
                scratch[2].r = F[mstride * 3].r * tw3->r - F[mstride * 3].i * tw3->i;
                scratch[2].i = F[mstride * 3].r * tw3->i + F[mstride * 3].i * tw3->r;

                scratch[5].r = F->r - scratch[1].r;
                scratch[5].i = F->i - scratch[1].i;
                F->r += scratch[1].r;
                F->i += scratch[1].i;

                scratch[3].r = scratch[0].r + scratch[2].r;
                scratch[3].i = scratch[0].i + scratch[2].i;
                scratch[4].r = scratch[0].r - scratch[2].r;
                scratch[4].i = scratch[0].i - scratch[2].i;

                F[mstride * 2].r = F->r - scratch[3].r;
                F[mstride * 2].i = F->i - scratch[3].i;
                F->r += scratch[3].r;
                F->i += scratch[3].i;

                F[mstride].r = scratch[5].r + scratch[4].i;
                F[mstride].i = scratch[5].i - scratch[4].r;
                F[mstride * 3].r = scratch[5].r - scratch[4].i;
                F[mstride * 3].i = scratch[5].i + scratch[4].r;

                tw1++;
                tw2++;
                tw3++;
                F++;
            }
        }
        tw += mstride * 3;
        mstride <<= 2;
    }

}

static void ne10_mixed_radix_butterfly_inverse_float32_c (ne10_fft_cpx_float32_t * Fout,
        ne10_int32_t * factors,
        ne10_fft_cpx_float32_t * twiddles)

{
    ne10_int32_t i, j, mstride;
    ne10_int32_t stage_count;
    ne10_int32_t fstride;

    ne10_fft_cpx_float32_t tmp;
    ne10_fft_cpx_float32_t scratch[6];
    ne10_fft_cpx_float32_t *tw, *tw1, *tw2, *tw3;
    ne10_fft_cpx_float32_t * F;


    // the first stage
    stage_count = factors[0];
    fstride = factors[1];
    if (factors[2 * stage_count] == 2) // length of FFT is 2^n (n is odd)
    {
        //fstride is nfft>>1;
        for (i = 0; i < fstride; i++)
        {
            tmp.r = Fout[2 * i + 1].r;
            tmp.i = Fout[2 * i + 1].i;
            Fout[2 * i + 1].r = Fout[2 * i].r - tmp.r;
            Fout[2 * i + 1].i = Fout[2 * i].i - tmp.i;
            Fout[2 * i].r = Fout[2 * i].r + tmp.r;
            Fout[2 * i].i = Fout[2 * i].i + tmp.i;
        }
    }
    else if (factors[2 * stage_count] == 4) // length of FFT is 2^n (n is even)
    {
        //fstride is nfft>>2
        for (i = 0; i < fstride; i++)
        {
            scratch[2].r = Fout[4 * i].r - Fout[4 * i + 2].r;
            scratch[2].i = Fout[4 * i].i - Fout[4 * i + 2].i;

            Fout[4 * i].r += Fout[4 * i + 2].r;
            Fout[4 * i].i += Fout[4 * i + 2].i;

            scratch[0].r = Fout[4 * i + 1].r + Fout[4 * i + 3].r;
            scratch[0].i = Fout[4 * i + 1].i + Fout[4 * i + 3].i;

            scratch[1].r = Fout[4 * i + 1].r - Fout[4 * i + 3].r;
            scratch[1].i = Fout[4 * i + 1].i - Fout[4 * i + 3].i;
            Fout[4 * i + 2].r = Fout[4 * i].r - scratch[0].r;
            Fout[4 * i + 2].i = Fout[4 * i].i - scratch[0].i;

            Fout[4 * i].r += scratch[0].r;
            Fout[4 * i].i += scratch[0].i;

            Fout[4 * i + 1].r = scratch[2].r - scratch[1].i;
            Fout[4 * i + 1].i = scratch[2].i + scratch[1].r;
            Fout[4 * i + 3].r = scratch[2].r + scratch[1].i;
            Fout[4 * i + 3].i = scratch[2].i - scratch[1].r;
        }
    }
    stage_count--;

    // other stages
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
            for (j = 0; j < mstride; j++)
            {
                scratch[0].r = F[mstride].r * tw1->r + F[mstride].i * tw1->i;
                scratch[0].i = F[mstride].i * tw1->r - F[mstride].r * tw1->i;
                scratch[1].r = F[mstride * 2].r * tw2->r + F[mstride * 2].i * tw2->i;
                scratch[1].i = F[mstride * 2].i * tw2->r - F[mstride * 2].r * tw2->i;
                scratch[2].r = F[mstride * 3].r * tw3->r + F[mstride * 3].i * tw3->i;
                scratch[2].i = F[mstride * 3].i * tw3->r - F[mstride * 3].r * tw3->i;

                scratch[5].r = F->r - scratch[1].r;
                scratch[5].i = F->i - scratch[1].i;
                F->r += scratch[1].r;
                F->i += scratch[1].i;

                scratch[3].r = scratch[0].r + scratch[2].r;
                scratch[3].i = scratch[0].i + scratch[2].i;
                scratch[4].r = scratch[0].r - scratch[2].r;
                scratch[4].i = scratch[0].i - scratch[2].i;

                F[mstride * 2].r = F->r - scratch[3].r;
                F[mstride * 2].i = F->i - scratch[3].i;
                F->r += scratch[3].r;
                F->i += scratch[3].i;

                F[mstride].r = scratch[5].r - scratch[4].i;
                F[mstride].i = scratch[5].i + scratch[4].r;
                F[mstride * 3].r = scratch[5].r + scratch[4].i;
                F[mstride * 3].i = scratch[5].i - scratch[4].r;

                tw1++;
                tw2++;
                tw3++;
                F++;
            }
        }
        tw += mstride * 3;
        mstride <<= 2;
    }
}

/* factors buffer:
 * 0: stage number
 * 1: stride for the first stage
 * others: factors */
static ne10_int32_t ne10_factor (ne10_int32_t n, ne10_int32_t * facbuf)
{
    ne10_int32_t p = 4;
    ne10_int32_t i = 1;
    ne10_int32_t stage_num = 0;
    ne10_int32_t stride_max = n;

    /* factor out powers of 4, powers of 2 */
    do
    {
        if ( (n % p) == 2)
            p = 2;
        else if (n % p)
        {
            return NE10_ERR;
        }

        n /= p;
        facbuf[2 * i] = p;
        facbuf[2 * i + 1] = n;
        i++;
        stage_num++;
    }
    while (n > 1);
    facbuf[0] = stage_num;
    facbuf[1] = stride_max / p;
    return NE10_OK;
}

void ne10_data_bitreversal_float32 (ne10_fft_cpx_float32_t * Fout,
                                    const ne10_fft_cpx_float32_t * f,
                                    ne10_int32_t fstride,
                                    ne10_int32_t * factors)
{
    const ne10_int32_t p = *factors++; /* the radix  */
    const ne10_int32_t m = *factors++; /* stage's fft length/p */
    const ne10_fft_cpx_float32_t * Fout_end = Fout + p * m;
    if (m == 1)
    {
        do
        {
            *Fout = *f;
            f += fstride;
        }
        while (++Fout != Fout_end);
    }
    else
    {
        do
        {
            ne10_data_bitreversal_float32 (Fout, f, fstride * p, factors);
            f += fstride;
        }
        while ( (Fout += m) != Fout_end);
    }

}

void ne10_fft_split_r2c_1d_float32 (ne10_fft_cpx_float32_t *dst,
                                    const ne10_fft_cpx_float32_t *src,
                                    ne10_fft_cpx_float32_t *twiddles,
                                    ne10_int32_t ncfft)
{
    ne10_int32_t k;
    ne10_fft_cpx_float32_t fpnk, fpk, f1k, f2k, tw, tdc;

    tdc.r = src[0].r;
    tdc.i = src[0].i;

    dst[0].r = tdc.r + tdc.i;
    dst[ncfft].r = tdc.r - tdc.i;
    dst[ncfft].i = dst[0].i = 0;

    for (k = 1; k <= ncfft / 2 ; ++k)
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

void ne10_fft_split_c2r_1d_float32 (ne10_fft_cpx_float32_t *dst,
                                    const ne10_fft_cpx_float32_t *src,
                                    ne10_fft_cpx_float32_t *twiddles,
                                    ne10_int32_t ncfft)
{

    ne10_int32_t k;
    ne10_fft_cpx_float32_t fk, fnkc, fek, fok, tmp;


    dst[0].r = src[0].r + src[ncfft].r;
    dst[0].i = src[0].r - src[ncfft].r;

    for (k = 1; k <= ncfft / 2; k++)
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

/**
 * @defgroup C2C_FFT_IFFT Float/Fixed point Complex FFT
 * @ingroup groupDSPs
 * @{
 *
 * \par
 * A Fast Fourier Transform(FFT) is an efficient algorithm to compute the Discrete Fourier Transform(DFT) and its inverse.
 * FFT is widely used for many applications in engineering, science and mathmatics.
 *
 * \par Function list
 * This set of functions implements complex one-dimensional FFT/IFFT with <code>2^N</code>(N>0) size. The function list is as follows:
 * -  fft_c2c_1d_float32
 * -  fft_c2c_1d_int32
 * -  fft_c2c_1d_int16
 *
 * \par
 * Note: The functions operate on out-of-place buffer which use different buffer for input and output. If the input and output is same,
 * we will allocate a temp buffer as output  buffer and then copy the output back to input buffer.
 *
 * \par The format of input and output:
 * The input and output have the same format as follows:
 * <pre> {real[0], imag[0], real[1], imag[1], real[2], imag[2].... real[fftSize-2], imag[fftSize-2], real[fftSize-1], imag[fftSize-1]} </pre>
 *
 * \par Lengths supported by the transform:
 * \par
 * Internally, the functions utilize a mixed radix 2/4 algorithm and the size of the FFT supported is the lengths
 * <code>2^N</code> (N is 1, 2, 3, 4, 5, 6, ......).
 *
 * \par Usage:
 * The basic usage of these functions is simple. We take float fft as an example and it looks like the code as follows.
 *
 * <pre>
 * #include "NE10.h"
 * ...
 * {
 *     fftSize = 2^N; //N is 1, 2, 3, 4, 5, 6....
 *     in = (ne10_fft_cpx_float32_t*) NE10_MALLOC (fftSize * sizeof (ne10_fft_cpx_float32_t));
 *     out = (ne10_fft_cpx_float32_t*) NE10_MALLOC (fftSize * sizeof (ne10_fft_cpx_float32_t));
 *     ne10_fft_cfg_float32_t cfg;
 *     ...
       cfg = ne10_fft_alloc_c2c_float32 (fftSize);
 *     ...
 *     //FFT
 *     ne10_fft_c2c_1d_float32_c (out, in, cfg->twiddles, cfg->factors, fftSize, 0);
 *     ...
 *     //IFFT
 *     ne10_fft_c2c_1d_float32_c (out, in, cfg->twiddles, cfg->factors, fftSize, 1);
 *     ...
 *     NE10_FREE (in);
 *     NE10_FREE (out);
 *     NE10_FREE (cfg);
 * }
 * </pre>
 *
 * Note:
 * \par
 * ne10_fft_cfg_float32_t cfg is the pointer which points to the buffer storing the twiddles and factors. It's generated in ne10_fft_alloc_c2c_float32(fftSize). If the fftSize is same, you needn't generate it again.
 * - cfg->twiddles
 *   \n This is pointer to the twiddle factor table.
 * - cfg->factors
 *   \n This is factors buffer: 0: stage number, 1: stride for the first stage, others: factors.
 *   \n For example, 128 could be split into 4x32, 4x8, 4x2, 2x1. The stage is 4, the stride of first stage is <code>128/2 = 64</code>. So that the factor buffer is[4, 64, 4, 32, 4, 8, 4, 2, 2, 1]
 *
 */

/**
 * @brief User-callable function to allocate all necessary storage space for the fft.
 * @param[in]   nfft             length of FFT
 * @return      st               point to the FFT config memory. This memory is allocated with malloc.
 * The function allocate all necessary storage space for the fft. It also factors out the length of FFT and generates the twiddle coeff.
 */
ne10_fft_cfg_float32_t ne10_fft_alloc_c2c_float32 (ne10_int32_t nfft)
{
    ne10_fft_cfg_float32_t st = NULL;
    ne10_uint32_t memneeded = sizeof (struct ne10_fft_state_float32_t)
                              + sizeof (ne10_fft_cpx_float32_t) * (nfft - 1); /* twiddle factors*/

    st = (ne10_fft_cfg_float32_t) NE10_MALLOC (memneeded);

    if (st)
    {
        ne10_int32_t result = ne10_factor (nfft, st->factors);
        if (result == NE10_ERR)
        {
            fprintf (stdout, "======ERROR, the length of FFT isn't support\n");
            NE10_FREE (st);
            return st;
        }

        ne10_int32_t i, j;
        ne10_int32_t *factors = st->factors;
        ne10_fft_cpx_float32_t *twiddles = st->twiddles;
        ne10_fft_cpx_float32_t *tw;
        ne10_int32_t stage_count = factors[0];
        ne10_int32_t fstride1 = factors[1];
        ne10_int32_t fstride2 = fstride1 * 2;
        ne10_int32_t fstride3 = fstride1 * 3;
        ne10_int32_t m;

        const ne10_float32_t pi = NE10_PI;
        ne10_float32_t phase1;
        ne10_float32_t phase2;
        ne10_float32_t phase3;

        for (i = stage_count - 1; i > 0; i--)
        {
            fstride1 >>= 2;
            fstride2 >>= 2;
            fstride3 >>= 2;
            m = factors[2 * i + 1];
            tw = twiddles;
            for (j = 0; j < m; j++)
            {
                phase1 = -2 * pi * fstride1 * j / nfft;
                phase2 = -2 * pi * fstride2 * j / nfft;
                phase3 = -2 * pi * fstride3 * j / nfft;
                tw->r = (ne10_float32_t) cos (phase1);
                tw->i = (ne10_float32_t) sin (phase1);
                (tw + m)->r = (ne10_float32_t) cos (phase2);
                (tw + m)->i = (ne10_float32_t) sin (phase2);
                (tw + m * 2)->r = (ne10_float32_t) cos (phase3);
                (tw + m * 2)->i = (ne10_float32_t) sin (phase3);
                tw++;
            }
            twiddles += m * 3;
        }

    }
    return st;
}

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
void ne10_fft_c2c_1d_float32_c (ne10_fft_cpx_float32_t *fout,
                                ne10_fft_cpx_float32_t *fin,
                                ne10_fft_cpx_float32_t *twiddles,
                                ne10_int32_t *factors,
                                ne10_int32_t nfft,
                                ne10_int32_t inverse_fft)
{
    // copy the data from input to output and bit reversal
    ne10_data_bitreversal_float32 (fout, fin, 1, &factors[2]);

    if (inverse_fft)
        ne10_mixed_radix_butterfly_inverse_float32_c (fout, factors, twiddles);
    else
        ne10_mixed_radix_butterfly_float32_c (fout, factors, twiddles);
}
/**
 * @}
 */ //end of C2C_FFT_IFFT group

/**
 * @defgroup R2C_FFT_IFFT Float/Fixed point Real2Complex FFT
 * @ingroup groupDSPs
 * @{
 *
 * \par
 * In Ne10 library, there has been complex FFT. But in many practical applications, signals are often real only.
 * So that we implement the real to complex FFT and complex to real IFFT based on complex FFT.
 *
 * \par Function list
 * This set of functions implements r2c one-dimensional FFT/c2r IFFT with <code>2^N</code>(N>1) size. The function list is as follows:
 * -  fft_r2c_1d_float32
 * -  fft_c2r_1d_float32
 * -  fft_r2c_1d_int32
 * -  fft_c2r_1d_int32
 * -  fft_r2c_1d_int16
 * -  fft_c2r_1d_int16
 *
 * \par
 * Note: The functions operate on out-of-place buffer which use different buffer for input and output. If the input and output is same,
 * we will allocate a temp buffer as output  buffer and then copy the output back to input buffer.
 *
 * \par The format of input and output:
 * - r2c FFT
 *   \n Input:{real[0], real[1],real[2],.... real[fftSize-2], real[fftSize-1]}. The length of input is fftSize real data.
 *   \n Output:{real[0], imag[0], real[1], imag[1], real[2], imag[2].... real[fftSize/2], imag[fftSize/2],}. The length of output should be fftSize complex data.
 *   For the reason that output[i] is the conjugate of output[fftSize-i], so that we give here output[0]~output[fftSize/2].
 * - c2r IFFT
 *   \n Input:{real[0], imag[0], real[1], imag[1], real[2], imag[2].... real[fftSize-1], imag[fftSize-1],} The length of input is fftSize complex data.
 *   \n Output:{real[0], real[1],real[2],.... real[fftSize-2], real[fftSize-1]}. The length of output is fftSize real data.
 *
 * \par Lengths supported by the transform:
 * \par
 * The r2c FFT is based on complex FFT, so that the FFT size supported is the length
 * <code>2^N</code> (N is 2, 3, 4, 5, 6, ......).
 *
 * \par Usage:
 * The basic usage of these functions is simple. We take float fft as an example and it looks like the code as follows.
 *
 * <pre>
 * #include "NE10.h"
 * ...
 * {
 *     fftSize = 2^N; //N is 2, 3, 4, 5, 6....
 *     in = (ne10_float32_t*) NE10_MALLOC (fftSize * sizeof (ne10_float32_t));
 *     out = (ne10_fft_cpx_float32_t*) NE10_MALLOC (fftSize * sizeof (ne10_fft_cpx_float32_t));
 *     ne10_fft_r2c_cfg_float32_t cfg;
 *     ...
       cfg = ne10_fft_alloc_r2c_float32 (fftSize);
 *     ...
 *     //FFT
 *     ne10_fft_r2c_1d_float32_c (out, in, cfg->twiddles, cfg->super_twiddles, cfg->factors, fftSize);
 *     ...
 *     //IFFT
 *     ne10_fft_c2r_1d_float32_c (in, out, cfg->twiddles, cfg->super_twiddles, cfg->factors, fftSize);
 *     ...
 *     NE10_FREE (cfg);
 *     NE10_FREE (in);
 *     NE10_FREE (out);
 * }
 * </pre>
 *
 * Note:
 * \par
 * ne10_fft_r2c_cfg_float32_t cfg is the pointer which points to the buffer storing the twiddles and factors. It's generated in ne10_fft_alloc_r2c_float32(fftSize). If the fftSize is same, you needn't generate it again.
 * - cfg->twiddles
 *   \n This is pointer to the twiddle factor table.
 * - cfg->super_twiddles
 *   \n This is pointer to the twiddle factor which for spliting complex and real.
 * - cfg->factors
 *   \n This is factors buffer: 0: stage number, 1: stride for the first stage, others: factors.
 *   \n For example, 128 could be split into 4x32, 4x8, 4x2, 2x1. The stage is 4, the stride of first stage is <code>128/2 = 64</code>. So that the factor buffer is[4, 64, 4, 32, 4, 8, 4, 2, 2, 1]
 *
 */

/**
 * @brief User-callable function to allocate all necessary storage space for the fft (r2c/c2r).
 * @param[in]   nfft             length of FFT
 * @return      st               point to the FFT config memory. This memory is allocated with malloc.
 * The function allocate all necessary storage space for the fft. It also factors out the length of FFT and generates the twiddle coeff.
 */
ne10_fft_r2c_cfg_float32_t ne10_fft_alloc_r2c_float32 (ne10_int32_t nfft)
{
    ne10_fft_r2c_cfg_float32_t st = NULL;
    ne10_int32_t ncfft = nfft >> 1;

    ne10_uint32_t memneeded = sizeof (struct ne10_fft_r2c_state_float32_t)
                              + sizeof (ne10_fft_cpx_float32_t) * ncfft  /* twiddle*/
                              + sizeof (ne10_fft_cpx_float32_t) * ncfft / 2; /* super twiddles*/

    st = (ne10_fft_r2c_cfg_float32_t) NE10_MALLOC (memneeded);
    st->twiddles = (ne10_fft_cpx_float32_t*) ( (ne10_int8_t*) st + sizeof (struct ne10_fft_r2c_state_float32_t));
    st->super_twiddles = st->twiddles + ncfft;

    if (st)
    {
        ne10_int32_t result = ne10_factor (ncfft, st->factors);
        if (result == NE10_ERR)
        {
            fprintf (stdout, "======ERROR, the length of FFT isn't support\n");
            NE10_FREE (st);
            return st;
        }

        ne10_int32_t i, j;
        ne10_int32_t *factors = st->factors;
        ne10_fft_cpx_float32_t *twiddles = st->twiddles;
        ne10_fft_cpx_float32_t *tw;
        ne10_int32_t stage_count = factors[0];
        ne10_int32_t fstride1 = factors[1];
        ne10_int32_t fstride2 = fstride1 * 2;
        ne10_int32_t fstride3 = fstride1 * 3;
        ne10_int32_t m;

        const ne10_float32_t pi = NE10_PI;
        ne10_float32_t phase1;
        ne10_float32_t phase2;
        ne10_float32_t phase3;

        for (i = stage_count - 1; i > 0; i--)
        {
            fstride1 >>= 2;
            fstride2 >>= 2;
            fstride3 >>= 2;
            m = factors[2 * i + 1];
            tw = twiddles;
            for (j = 0; j < m; j++)
            {
                phase1 = -2 * pi * fstride1 * j / ncfft;
                phase2 = -2 * pi * fstride2 * j / ncfft;
                phase3 = -2 * pi * fstride3 * j / ncfft;
                tw->r = (ne10_float32_t) cos (phase1);
                tw->i = (ne10_float32_t) sin (phase1);
                (tw + m)->r = (ne10_float32_t) cos (phase2);
                (tw + m)->i = (ne10_float32_t) sin (phase2);
                (tw + m * 2)->r = (ne10_float32_t) cos (phase3);
                (tw + m * 2)->i = (ne10_float32_t) sin (phase3);
                tw++;
            }
            twiddles += m * 3;
        }

        tw = st->super_twiddles;
        for (i = 0; i < ncfft / 2; i++)
        {
            phase1 = -pi * ( (ne10_float32_t) (i + 1) / ncfft + 0.5f);
            tw->r = (ne10_float32_t) cos (phase1);
            tw->i = (ne10_float32_t) sin (phase1);
            tw++;
        }

    }
    return st;
}

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
void ne10_fft_r2c_1d_float32_c (ne10_fft_cpx_float32_t *fout,
                                ne10_float32_t *fin,
                                ne10_fft_cpx_float32_t *twiddles,
                                ne10_fft_cpx_float32_t *super_twiddles,
                                ne10_int32_t *factors,
                                ne10_int32_t nfft)
{
    ne10_int32_t ncfft = nfft >> 1;

    /* malloc a temp buffer for cfft */
    ne10_fft_cpx_float32_t * tmpbuf_ = (ne10_fft_cpx_float32_t*) NE10_MALLOC (sizeof (ne10_fft_cpx_float32_t) * ncfft);

    ne10_fft_c2c_1d_float32_c (tmpbuf_, (ne10_fft_cpx_float32_t*) fin, twiddles, factors, ncfft, 0);
    ne10_fft_split_r2c_1d_float32 (fout, tmpbuf_, super_twiddles, ncfft);

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
void ne10_fft_c2r_1d_float32_c (ne10_float32_t *fout,
                                ne10_fft_cpx_float32_t *fin,
                                ne10_fft_cpx_float32_t *twiddles,
                                ne10_fft_cpx_float32_t *super_twiddles,
                                ne10_int32_t *factors,
                                ne10_int32_t nfft)
{
    ne10_int32_t ncfft = nfft >> 1;

    /* malloc a temp buffer for split */
    ne10_fft_cpx_float32_t * tmpbuf_ = (ne10_fft_cpx_float32_t*) NE10_MALLOC (sizeof (ne10_fft_cpx_float32_t) * ncfft);

    ne10_fft_split_c2r_1d_float32 (tmpbuf_, fin, super_twiddles, ncfft);
    ne10_fft_c2c_1d_float32_c ( (ne10_fft_cpx_float32_t*) fout, tmpbuf_, twiddles, factors, ncfft, 1);

    NE10_FREE (tmpbuf_);
}

/**
 * @} end of R2C_FFT_IFFT group
 */
