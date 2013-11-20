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
 * NE10 Library : dsp/NE10_fft_int32.c
 */

#include "NE10_types.h"
#include "NE10_macros.h"
#include "NE10_fft.h"


static void ne10_mixed_radix_butterfly_int32_c (ne10_fft_cpx_int32_t * Fout,
        ne10_int32_t * factors,
        ne10_fft_cpx_int32_t * twiddles)

{
    ne10_int32_t i, j, mstride;
    ne10_int32_t stage_count;
    ne10_int32_t fstride;

    ne10_fft_cpx_int32_t tmp;
    ne10_fft_cpx_int32_t scratch[6];
    ne10_fft_cpx_int32_t *tw, *tw1, *tw2, *tw3;
    ne10_fft_cpx_int32_t * F;


    // the first stage
    stage_count = factors[0];
    fstride = factors[1];
    if (factors[2 * stage_count] == 2) // length of FFT is 2^n (n is odd)
    {
        //fstride is nfft>>1
        for (i = 0; i < fstride; i++)
        {
            NE10_F2I32_FIXDIV (Fout[2 * i], 2);
            NE10_F2I32_FIXDIV (Fout[2 * i + 1], 2);
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
            NE10_F2I32_FIXDIV (Fout[4 * i], 4);
            NE10_F2I32_FIXDIV (Fout[4 * i + 1], 4);
            NE10_F2I32_FIXDIV (Fout[4 * i + 2], 4);
            NE10_F2I32_FIXDIV (Fout[4 * i + 3], 4);
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
                NE10_F2I32_FIXDIV (F[0], 4);
                NE10_F2I32_FIXDIV (F[mstride], 4);
                NE10_F2I32_FIXDIV (F[mstride * 2], 4);
                NE10_F2I32_FIXDIV (F[mstride * 3], 4);
                scratch[0].r = ( ( (ne10_int32_t) ( ( (ne10_int64_t) F[mstride].r * tw1->r) >> 32)) - ( (ne10_int32_t) ( ( (ne10_int64_t) F[mstride].i * tw1->i) >> 32))) << 1;
                scratch[0].i = ( ( (ne10_int32_t) ( ( (ne10_int64_t) F[mstride].r * tw1->i) >> 32)) + ( (ne10_int32_t) ( ( (ne10_int64_t) F[mstride].i * tw1->r) >> 32))) << 1;
                scratch[1].r = ( ( (ne10_int32_t) ( ( (ne10_int64_t) F[mstride * 2].r * tw2->r) >> 32)) - ( (ne10_int32_t) ( ( (ne10_int64_t) F[mstride * 2].i * tw2->i) >> 32))) << 1;
                scratch[1].i = ( ( (ne10_int32_t) ( ( (ne10_int64_t) F[mstride * 2].r * tw2->i) >> 32)) + ( (ne10_int32_t) ( ( (ne10_int64_t) F[mstride * 2].i * tw2->r) >> 32))) << 1;
                scratch[2].r = ( ( (ne10_int32_t) ( ( (ne10_int64_t) F[mstride * 3].r * tw3->r) >> 32)) - ( (ne10_int32_t) ( ( (ne10_int64_t) F[mstride * 3].i * tw3->i) >> 32))) << 1;
                scratch[2].i = ( ( (ne10_int32_t) ( ( (ne10_int64_t) F[mstride * 3].r * tw3->i) >> 32)) + ( (ne10_int32_t) ( ( (ne10_int64_t) F[mstride * 3].i * tw3->r) >> 32))) << 1;

                //C_SUB( scratch[5] , *F, scratch[1] );
                scratch[5].r = F->r - scratch[1].r;
                scratch[5].i = F->i - scratch[1].i;
                //C_ADDTO(*F, scratch[1]);
                F->r += scratch[1].r;
                F->i += scratch[1].i;
                //C_ADD( scratch[3] , scratch[0] , scratch[2] );
                scratch[3].r = scratch[0].r + scratch[2].r;
                scratch[3].i = scratch[0].i + scratch[2].i;
                //C_SUB( scratch[4] , scratch[0] , scratch[2] );
                //C_SUB( F[m2], *F, scratch[3] );
                scratch[4].r = scratch[0].r - scratch[2].r;
                scratch[4].i = scratch[0].i - scratch[2].i;
                F[mstride * 2].r = F->r - scratch[3].r;
                F[mstride * 2].i = F->i - scratch[3].i;
                //C_ADDTO( *F , scratch[3] );
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

static void ne10_mixed_radix_butterfly_inverse_int32_c (ne10_fft_cpx_int32_t * Fout,
        ne10_int32_t * factors,
        ne10_fft_cpx_int32_t * twiddles)

{
    ne10_int32_t i, j, mstride;
    ne10_int32_t stage_count;
    ne10_int32_t fstride;

    ne10_fft_cpx_int32_t tmp;
    ne10_fft_cpx_int32_t scratch[6];
    ne10_fft_cpx_int32_t *tw, *tw1, *tw2, *tw3;
    ne10_fft_cpx_int32_t * F;


    // the first stage
    stage_count = factors[0];
    fstride = factors[1];
    if (factors[2 * stage_count] == 2) // length of FFT is 2^n (n is odd)
    {
        //fstride is nfft>>1;
        for (i = 0; i < fstride; i++)
        {
            NE10_F2I32_FIXDIV (Fout[2 * i], 2);
            NE10_F2I32_FIXDIV (Fout[2 * i + 1], 2);
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
            NE10_F2I32_FIXDIV (Fout[4 * i], 4);
            NE10_F2I32_FIXDIV (Fout[4 * i + 1], 4);
            NE10_F2I32_FIXDIV (Fout[4 * i + 2], 4);
            NE10_F2I32_FIXDIV (Fout[4 * i + 3], 4);
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
                NE10_F2I32_FIXDIV (F[0], 4);
                NE10_F2I32_FIXDIV (F[mstride], 4);
                NE10_F2I32_FIXDIV (F[mstride * 2], 4);
                NE10_F2I32_FIXDIV (F[mstride * 3], 4);
                scratch[0].r = ( ( (ne10_int32_t) ( ( (ne10_int64_t) F[mstride].r * tw1->r) >> 32)) + ( (ne10_int32_t) ( ( (ne10_int64_t) F[mstride].i * tw1->i) >> 32))) << 1;
                scratch[0].i = ( ( (ne10_int32_t) ( ( (ne10_int64_t) F[mstride].i * tw1->r) >> 32)) - ( (ne10_int32_t) ( ( (ne10_int64_t) F[mstride].r * tw1->i) >> 32))) << 1;
                scratch[1].r = ( ( (ne10_int32_t) ( ( (ne10_int64_t) F[mstride * 2].r * tw2->r) >> 32)) + ( (ne10_int32_t) ( ( (ne10_int64_t) F[mstride * 2].i * tw2->i) >> 32))) << 1;
                scratch[1].i = ( ( (ne10_int32_t) ( ( (ne10_int64_t) F[mstride * 2].i * tw2->r) >> 32)) - ( (ne10_int32_t) ( ( (ne10_int64_t) F[mstride * 2].r * tw2->i) >> 32))) << 1;
                scratch[2].r = ( ( (ne10_int32_t) ( ( (ne10_int64_t) F[mstride * 3].r * tw3->r) >> 32)) + ( (ne10_int32_t) ( ( (ne10_int64_t) F[mstride * 3].i * tw3->i) >> 32))) << 1;
                scratch[2].i = ( ( (ne10_int32_t) ( ( (ne10_int64_t) F[mstride * 3].i * tw3->r) >> 32)) - ( (ne10_int32_t) ( ( (ne10_int64_t) F[mstride * 3].r * tw3->i) >> 32))) << 1;

                //C_SUB( scratch[5] , *F, scratch[1] );
                scratch[5].r = F->r - scratch[1].r;
                scratch[5].i = F->i - scratch[1].i;
                //C_ADDTO(*F, scratch[1]);
                F->r += scratch[1].r;
                F->i += scratch[1].i;
                //C_ADD( scratch[3] , scratch[0] , scratch[2] );
                scratch[3].r = scratch[0].r + scratch[2].r;
                scratch[3].i = scratch[0].i + scratch[2].i;
                //C_SUB( scratch[4] , scratch[0] , scratch[2] );
                //C_SUB( F[m2], *F, scratch[3] );
                scratch[4].r = scratch[0].r - scratch[2].r;
                scratch[4].i = scratch[0].i - scratch[2].i;
                F[mstride * 2].r = F->r - scratch[3].r;
                F[mstride * 2].i = F->i - scratch[3].i;
                //C_ADDTO( *F , scratch[3] );
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

void ne10_data_bitreversal_int32 (ne10_fft_cpx_int32_t * Fout,
                                  const ne10_fft_cpx_int32_t * f,
                                  ne10_int32_t fstride,
                                  ne10_int32_t * factors)
{
    const ne10_int32_t p = *factors++; /* the radix  */
    const ne10_int32_t m = *factors++; /* stage's fft length/p */
    const ne10_fft_cpx_int32_t * Fout_end = Fout + p * m;
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
            ne10_data_bitreversal_int32 (Fout, f, fstride * p, factors);
            f += fstride;
        }
        while ( (Fout += m) != Fout_end);
    }

}

void ne10_fft_split_r2c_1d_int32 (ne10_fft_cpx_int32_t *dst,
                                  const ne10_fft_cpx_int32_t *src,
                                  ne10_fft_cpx_int32_t *twiddles,
                                  ne10_int32_t ncfft)
{
    ne10_int32_t k;
    ne10_fft_cpx_int32_t fpnk, fpk, f1k, f2k, tw, tdc;

    tdc.r = src[0].r;
    tdc.i = src[0].i;
    NE10_F2I32_FIXDIV (tdc, 2);

    dst[0].r = tdc.r + tdc.i;
    dst[ncfft].r = tdc.r - tdc.i;
    dst[ncfft].i = dst[0].i = 0;

    for (k = 1; k <= ncfft / 2 ; ++k)
    {
        fpk    = src[k];
        fpnk.r =   src[ncfft - k].r;
        fpnk.i = - src[ncfft - k].i;
        NE10_F2I32_FIXDIV (fpk, 2);
        NE10_F2I32_FIXDIV (fpnk, 2);

        f1k.r = fpk.r + fpnk.r;
        f1k.i = fpk.i + fpnk.i;

        f2k.r = fpk.r - fpnk.r;
        f2k.i = fpk.i - fpnk.i;

        tw.r = ( ( (ne10_int32_t) ( ( (ne10_int64_t) f2k.r * (twiddles[k - 1]).r) >> 32)) - ( (ne10_int32_t) ( ( (ne10_int64_t) f2k.i * (twiddles[k - 1]).i) >> 32))) << 1;
        tw.i = ( ( (ne10_int32_t) ( ( (ne10_int64_t) f2k.r * (twiddles[k - 1]).i) >> 32)) + ( (ne10_int32_t) ( ( (ne10_int64_t) f2k.i * (twiddles[k - 1]).r) >> 32))) << 1;

        dst[k].r = (f1k.r + tw.r) >> 1;
        dst[k].i = (f1k.i + tw.i) >> 1;
        dst[ncfft - k].r = (f1k.r - tw.r) >> 1;
        dst[ncfft - k].i = (tw.i - f1k.i) >> 1;
    }
}

void ne10_fft_split_c2r_1d_int32 (ne10_fft_cpx_int32_t *dst,
                                  const ne10_fft_cpx_int32_t *src,
                                  ne10_fft_cpx_int32_t *twiddles,
                                  ne10_int32_t ncfft)
{

    ne10_int32_t k;
    ne10_fft_cpx_int32_t fk, fnkc, fek, fok, tmp;


    dst[0].r = src[0].r + src[ncfft].r;
    dst[0].i = src[0].r - src[ncfft].r;
    NE10_F2I32_FIXDIV (dst[0], 2);

    for (k = 1; k <= ncfft / 2; k++)
    {
        fk = src[k];
        fnkc.r = src[ncfft - k].r;
        fnkc.i = -src[ncfft - k].i;
        NE10_F2I32_FIXDIV (fk, 2);
        NE10_F2I32_FIXDIV (fnkc, 2);

        fek.r = fk.r + fnkc.r;
        fek.i = fk.i + fnkc.i;

        tmp.r = fk.r - fnkc.r;
        tmp.i = fk.i - fnkc.i;

        fok.r = ( ( (ne10_int32_t) ( ( (ne10_int64_t) tmp.r * (twiddles[k - 1]).r) >> 32)) + ( (ne10_int32_t) ( ( (ne10_int64_t) tmp.i * (twiddles[k - 1]).i) >> 32))) << 1;
        fok.i = ( ( (ne10_int32_t) ( ( (ne10_int64_t) tmp.i * (twiddles[k - 1]).r) >> 32)) - ( (ne10_int32_t) ( ( (ne10_int64_t) tmp.r * (twiddles[k - 1]).i) >> 32))) << 1;

        dst[k].r = fek.r + fok.r;
        dst[k].i = fek.i + fok.i;

        dst[ncfft - k].r = fek.r - fok.r;
        dst[ncfft - k].i = fok.i - fek.i;
    }
}

/**
 * @addtogroup C2C_FFT_IFFT
 * @{
 */

/**
 * @brief User-callable function to allocate all necessary storage space for the fft.
 * @param[in]   nfft             length of FFT
 * @return      st               point to the FFT config memory. This memory is allocated with malloc.
 * The function allocate all necessary storage space for the fft. It also factors out the length of FFT and generates the twiddle coeff.
 */
ne10_fft_cfg_int32_t ne10_fft_alloc_c2c_int32 (ne10_int32_t nfft)
{
    ne10_fft_cfg_int32_t st = NULL;
    ne10_uint32_t memneeded = sizeof (struct ne10_fft_state_int32_t)
                              + sizeof (ne10_fft_cpx_int32_t) * (nfft - 1); // + 3*MAXFACTORS); /* twiddle factors*/

    st = (ne10_fft_cfg_int32_t) NE10_MALLOC (memneeded);
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
        ne10_fft_cpx_int32_t *twiddles = st->twiddles;
        ne10_fft_cpx_int32_t *tw;
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
                tw->r = (ne10_int32_t) floor (0.5f + NE10_F2I32_MAX * cos (phase1));
                tw->i = (ne10_int32_t) floor (0.5f + NE10_F2I32_MAX * sin (phase1));
                (tw + m)->r = (ne10_int32_t) floor (0.5f + NE10_F2I32_MAX * cos (phase2));
                (tw + m)->i = (ne10_int32_t) floor (0.5f + NE10_F2I32_MAX * sin (phase2));
                (tw + m * 2)->r = (ne10_int32_t) floor (0.5f + NE10_F2I32_MAX * cos (phase3));
                (tw + m * 2)->i = (ne10_int32_t) floor (0.5f + NE10_F2I32_MAX * sin (phase3));
                tw++;
            }
            twiddles += m * 3;
        }

    }
    return st;
}

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
void ne10_fft_c2c_1d_int32_c (ne10_fft_cpx_int32_t *fout,
                              ne10_fft_cpx_int32_t *fin,
                              ne10_fft_cpx_int32_t *twiddles,
                              ne10_int32_t *factors,
                              ne10_int32_t nfft,
                              ne10_int32_t inverse_fft)
{
    if (fin == fout)
    {
        /* NOTE: for an in-place FFT algorithm. It just performs an out-of-place FFT into a temp buffer */
        ne10_fft_cpx_int32_t * tmpbuf_ = (ne10_fft_cpx_int32_t*) NE10_MALLOC (sizeof (ne10_fft_cpx_int32_t) * nfft);

        // copy the data from input to output and bit reversal
        ne10_data_bitreversal_int32 (tmpbuf_, fin, 1, &factors[2]);

        if (inverse_fft)
            ne10_mixed_radix_butterfly_inverse_int32_c (tmpbuf_, factors, twiddles);
        else
            ne10_mixed_radix_butterfly_int32_c (tmpbuf_, factors, twiddles);

        memcpy (fout, tmpbuf_, sizeof (ne10_fft_cpx_int32_t) *nfft);
        NE10_FREE (tmpbuf_);
    }
    else
    {
        // copy the data from input to output and bit reversal
        ne10_data_bitreversal_int32 (fout, fin, 1, &factors[2]);

        if (inverse_fft)
            ne10_mixed_radix_butterfly_inverse_int32_c (fout, factors, twiddles);
        else
            ne10_mixed_radix_butterfly_int32_c (fout, factors, twiddles);
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
 * @brief User-callable function to allocate all necessary storage space for the fft (r2c/c2r).
 * @param[in]   nfft             length of FFT
 * @return      st               point to the FFT config memory. This memory is allocated with malloc.
 * The function allocate all necessary storage space for the fft. It also factors out the length of FFT and generates the twiddle coeff.
 */
ne10_fft_r2c_cfg_int32_t ne10_fft_alloc_r2c_int32 (ne10_int32_t nfft)
{
    ne10_fft_r2c_cfg_int32_t st = NULL;
    ne10_int32_t ncfft = nfft >> 1;

    ne10_uint32_t memneeded = sizeof (struct ne10_fft_r2c_state_int32_t)
                              + sizeof (ne10_fft_cpx_int32_t) * ncfft  /* twiddle*/
                              + sizeof (ne10_fft_cpx_int32_t) * ncfft / 2; /* super twiddles*/

    st = (ne10_fft_r2c_cfg_int32_t) NE10_MALLOC (memneeded);
    st->twiddles = (ne10_fft_cpx_int32_t*) ( (ne10_int8_t*) st + sizeof (struct ne10_fft_r2c_state_int32_t));
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
        ne10_fft_cpx_int32_t *twiddles = st->twiddles;
        ne10_fft_cpx_int32_t *tw;
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
                tw->r = (ne10_int32_t) floor (0.5f + NE10_F2I32_MAX * cos (phase1));
                tw->i = (ne10_int32_t) floor (0.5f + NE10_F2I32_MAX * sin (phase1));
                (tw + m)->r = (ne10_int32_t) floor (0.5f + NE10_F2I32_MAX * cos (phase2));
                (tw + m)->i = (ne10_int32_t) floor (0.5f + NE10_F2I32_MAX * sin (phase2));
                (tw + m * 2)->r = (ne10_int32_t) floor (0.5f + NE10_F2I32_MAX * cos (phase3));
                (tw + m * 2)->i = (ne10_int32_t) floor (0.5f + NE10_F2I32_MAX * sin (phase3));
                tw++;
            }
            twiddles += m * 3;
        }

        tw = st->super_twiddles;
        for (i = 0; i < ncfft / 2; i++)
        {
            phase1 = -pi * ( (ne10_float32_t) (i + 1) / ncfft + 0.5f);
            tw->r = (ne10_int32_t) floor (0.5f + NE10_F2I32_MAX * cos (phase1));
            tw->i = (ne10_int32_t) floor (0.5f + NE10_F2I32_MAX * sin (phase1));
            tw++;
        }

    }
    return st;
}

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
void ne10_fft_r2c_1d_int32_c (ne10_fft_cpx_int32_t *fout,
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
    ne10_data_bitreversal_int32 (tmpbuf_, (ne10_fft_cpx_int32_t*) fin, 1, &factors[2]);
    ne10_mixed_radix_butterfly_int32_c (tmpbuf_, factors, twiddles);

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
void ne10_fft_c2r_1d_int32_c (ne10_int32_t *fout,
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

    // copy the data from input to output and bit reversal
    ne10_data_bitreversal_int32 ( (ne10_fft_cpx_int32_t*) fout, tmpbuf_, 1, &factors[2]);
    ne10_mixed_radix_butterfly_inverse_int32_c ( (ne10_fft_cpx_int32_t*) fout, factors, twiddles);

    NE10_FREE (tmpbuf_);
}

/**
 * @} end of R2C_FFT_IFFT group
 */
