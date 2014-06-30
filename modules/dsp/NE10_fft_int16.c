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
 * NE10 Library : dsp/NE10_fft_int16.c
 */

#include "NE10_types.h"
#include "NE10_macros.h"
#include "NE10_fft.h"


static void ne10_mixed_radix_butterfly_int16_c (ne10_fft_cpx_int16_t * Fout,
        ne10_fft_cpx_int16_t   * Fin,
        ne10_int32_t * factors,
        ne10_fft_cpx_int16_t * twiddles,
        ne10_fft_cpx_int16_t * buffer,
        ne10_int32_t scaled_flag)
{
    ne10_int32_t fstride, mstride, N;
    ne10_int32_t fstride1;
    ne10_int32_t f_count, m_count;
    ne10_int32_t stage_count;

    ne10_fft_cpx_int16_t   scratch_in[8];
    ne10_fft_cpx_int16_t   scratch_out[8];
    ne10_fft_cpx_int16_t   scratch[16];
    ne10_fft_cpx_int16_t   scratch_tw[6];

    ne10_fft_cpx_int16_t   *Fin1, *Fin2, *Fout1, *Fout2;
    ne10_fft_cpx_int16_t   *Fout_ls = Fout;
    ne10_fft_cpx_int16_t   *Ftmp;
    ne10_fft_cpx_int16_t   *tw, *tw1, *tw2;
    const ne10_int32_t TW_81 = 23169;
    const ne10_int32_t TW_81N = -23169;



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

        Fin1 = Fin;
        for (f_count = 0; f_count < fstride1; f_count ++)
        {
            Fout1 = & Fout[ f_count * 8 ];
            // load
            if (scaled_flag == 1)
            {
                NE10_F2I16_FIXDIV (Fin1[0], 8);
                NE10_F2I16_FIXDIV (Fin1[0 + fstride], 8);
                NE10_F2I16_FIXDIV (Fin1[fstride1], 8);
                NE10_F2I16_FIXDIV (Fin1[fstride1 + fstride], 8);
                NE10_F2I16_FIXDIV (Fin1[fstride1 * 2], 8);
                NE10_F2I16_FIXDIV (Fin1[fstride1 * 2 + fstride], 8);
                NE10_F2I16_FIXDIV (Fin1[fstride1 * 3], 8);
                NE10_F2I16_FIXDIV (Fin1[fstride1 * 3 + fstride], 8);
            }
            scratch_in[0].r = Fin1[0].r + Fin1[0 + fstride].r;
            scratch_in[0].i = Fin1[0].i + Fin1[0 + fstride].i;
            scratch_in[1].r = Fin1[0].r - Fin1[0 + fstride].r;
            scratch_in[1].i = Fin1[0].i - Fin1[0 + fstride].i;
            scratch_in[2].r = Fin1[fstride1].r + Fin1[fstride1 + fstride].r;
            scratch_in[2].i = Fin1[fstride1].i + Fin1[fstride1 + fstride].i;
            scratch_in[3].r = Fin1[fstride1].r - Fin1[fstride1 + fstride].r;
            scratch_in[3].i = Fin1[fstride1].i - Fin1[fstride1 + fstride].i;
            scratch_in[4].r = Fin1[fstride1 * 2].r + Fin1[fstride1 * 2 + fstride].r;
            scratch_in[4].i = Fin1[fstride1 * 2].i + Fin1[fstride1 * 2 + fstride].i;
            scratch_in[5].r = Fin1[fstride1 * 2].r - Fin1[fstride1 * 2 + fstride].r;
            scratch_in[5].i = Fin1[fstride1 * 2].i - Fin1[fstride1 * 2 + fstride].i;
            scratch_in[6].r = Fin1[fstride1 * 3].r + Fin1[fstride1 * 3 + fstride].r;
            scratch_in[6].i = Fin1[fstride1 * 3].i + Fin1[fstride1 * 3 + fstride].i;
            scratch_in[7].r = Fin1[fstride1 * 3].r - Fin1[fstride1 * 3 + fstride].r;
            scratch_in[7].i = Fin1[fstride1 * 3].i - Fin1[fstride1 * 3 + fstride].i;

            // radix 4 butterfly without twiddles
            scratch[0] = scratch_in[0];
            scratch[1] = scratch_in[1];

            scratch[2] = scratch_in[2];
            scratch[3].r = (ne10_int16_t) ( ( (NE10_F2I16_SAMPPROD) (scratch_in[3].r + scratch_in[3].i) * TW_81) >> NE10_F2I16_SHIFT);
            scratch[3].i = (ne10_int16_t) ( ( (NE10_F2I16_SAMPPROD) (scratch_in[3].i - scratch_in[3].r) * TW_81) >> NE10_F2I16_SHIFT);

            scratch[4] = scratch_in[4];
            scratch[5].r = scratch_in[5].i;
            scratch[5].i = -scratch_in[5].r;

            scratch[6].r = scratch_in[6].r;
            scratch[6].i = scratch_in[6].i;
            scratch[7].r = (ne10_int16_t) ( ( (NE10_F2I16_SAMPPROD) (scratch_in[7].r - scratch_in[7].i) * TW_81N) >> NE10_F2I16_SHIFT);
            scratch[7].i = (ne10_int16_t) ( ( (NE10_F2I16_SAMPPROD) (scratch_in[7].i + scratch_in[7].r) * TW_81N) >> NE10_F2I16_SHIFT);

            // radix 2 butterfly
            scratch[8].r = scratch[0].r + scratch[4].r;
            scratch[8].i = scratch[0].i + scratch[4].i;
            scratch[9].r = scratch[1].r + scratch[5].r;
            scratch[9].i = scratch[1].i + scratch[5].i;

            scratch[10].r = scratch[0].r - scratch[4].r;
            scratch[10].i = scratch[0].i - scratch[4].i;
            scratch[11].r = scratch[1].r - scratch[5].r;
            scratch[11].i = scratch[1].i - scratch[5].i;

            // radix 2 butterfly
            scratch[12].r = scratch[2].r + scratch[6].r;
            scratch[12].i = scratch[2].i + scratch[6].i;
            scratch[13].r = scratch[3].r + scratch[7].r;
            scratch[13].i = scratch[3].i + scratch[7].i;

            scratch[14].r = scratch[2].r - scratch[6].r;
            scratch[14].i = scratch[2].i - scratch[6].i;
            scratch[15].r = scratch[3].r - scratch[7].r;
            scratch[15].i = scratch[3].i - scratch[7].i;

            // third result
            scratch_out[4].r = scratch[8].r - scratch[12].r;
            scratch_out[4].i = scratch[8].i - scratch[12].i;
            scratch_out[5].r = scratch[9].r - scratch[13].r;
            scratch_out[5].i = scratch[9].i - scratch[13].i;

            // first result
            scratch_out[0].r = scratch[8].r + scratch[12].r;
            scratch_out[0].i = scratch[8].i + scratch[12].i;
            scratch_out[1].r = scratch[9].r + scratch[13].r;
            scratch_out[1].i = scratch[9].i + scratch[13].i;

            // second result
            scratch_out[2].r = scratch[10].r + scratch[14].i;
            scratch_out[2].i = scratch[10].i - scratch[14].r;
            scratch_out[3].r = scratch[11].r + scratch[15].i;
            scratch_out[3].i = scratch[11].i - scratch[15].r;

            // forth result
            scratch_out[6].r = scratch[10].r - scratch[14].i;
            scratch_out[6].i = scratch[10].i + scratch[14].r;
            scratch_out[7].r = scratch[11].r - scratch[15].i;
            scratch_out[7].i = scratch[11].i + scratch[15].r;

            // store
            Fout1[0] = scratch_out[0];
            Fout1[1] = scratch_out[1];
            Fout1[2] = scratch_out[2];
            Fout1[3] = scratch_out[3];
            Fout1[4] = scratch_out[4];
            Fout1[5] = scratch_out[5];
            Fout1[6] = scratch_out[6];
            Fout1[7] = scratch_out[7];

            Fin1 += 1;
        } // f_count
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
        for (f_count = fstride; f_count ; f_count --)
        {
            // load
            scratch_in[0] = *Fin1;
            Fin2 = Fin1 + fstride;
            scratch_in[1] = *Fin2;
            Fin2 = Fin2 + fstride;
            scratch_in[2] = *Fin2;
            Fin2 = Fin2 + fstride;
            scratch_in[3] = *Fin2;

            // radix 4 butterfly without twiddles
            if (scaled_flag == 1)
            {
                NE10_F2I16_FIXDIV (scratch_in[0], 4);
                NE10_F2I16_FIXDIV (scratch_in[1], 4);
                NE10_F2I16_FIXDIV (scratch_in[2], 4);
                NE10_F2I16_FIXDIV (scratch_in[3], 4);
            }

            // radix 2 butterfly
            scratch[0].r = scratch_in[0].r + scratch_in[2].r;
            scratch[0].i = scratch_in[0].i + scratch_in[2].i;

            scratch[1].r = scratch_in[0].r - scratch_in[2].r;
            scratch[1].i = scratch_in[0].i - scratch_in[2].i;

            // radix 2 butterfly
            scratch[2].r = scratch_in[1].r + scratch_in[3].r;
            scratch[2].i = scratch_in[1].i + scratch_in[3].i;

            scratch[3].r = scratch_in[1].r - scratch_in[3].r;
            scratch[3].i = scratch_in[1].i - scratch_in[3].i;

            // third result
            scratch_out[2].r = scratch[0].r - scratch[2].r;
            scratch_out[2].i = scratch[0].i - scratch[2].i;

            // first result
            scratch_out[0].r = scratch[0].r + scratch[2].r;
            scratch_out[0].i = scratch[0].i + scratch[2].i;

            // second result
            scratch_out[1].r = scratch[1].r + scratch[3].i;
            scratch_out[1].i = scratch[1].i - scratch[3].r;

            // forth result
            scratch_out[3].r = scratch[1].r - scratch[3].i;
            scratch_out[3].i = scratch[1].i + scratch[3].r;

            // store
            * Fout1 ++ = scratch_out[0];
            * Fout1 ++ = scratch_out[1];
            * Fout1 ++ = scratch_out[2];
            * Fout1 ++ = scratch_out[3];

            Fin1++;
        } // f_count

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
            for (m_count = mstride; m_count ; m_count --)
            {
                // load
                scratch_tw[0] = *tw1;
                tw2 = tw1 + mstride;
                scratch_tw[1] = *tw2;
                tw2 += mstride;
                scratch_tw[2] = *tw2;
                scratch_in[0] = * Fin1;
                Fin2 = Fin1 + N;
                scratch_in[1] = * Fin2;
                Fin2 += N;
                scratch_in[2] = * Fin2;
                Fin2 += N;
                scratch_in[3] = * Fin2;
                if (scaled_flag == 1)
                {
                    NE10_F2I16_FIXDIV (scratch_in[0], 4);
                    NE10_F2I16_FIXDIV (scratch_in[1], 4);
                    NE10_F2I16_FIXDIV (scratch_in[2], 4);
                    NE10_F2I16_FIXDIV (scratch_in[3], 4);
                }

                // radix 4 butterfly with twiddles

                scratch[0] = scratch_in[0];
                scratch[1].r = (ne10_int16_t) ( ( (NE10_F2I16_SAMPPROD) scratch_in[1].r * scratch_tw[0].r
                                                  - (NE10_F2I16_SAMPPROD) scratch_in[1].i * scratch_tw[0].i) >> NE10_F2I16_SHIFT);
                scratch[1].i = (ne10_int16_t) ( ( (NE10_F2I16_SAMPPROD) scratch_in[1].i * scratch_tw[0].r
                                                  + (NE10_F2I16_SAMPPROD) scratch_in[1].r * scratch_tw[0].i) >> NE10_F2I16_SHIFT);

                scratch[2].r = (ne10_int16_t) ( ( (NE10_F2I16_SAMPPROD) scratch_in[2].r * scratch_tw[1].r
                                                  - (NE10_F2I16_SAMPPROD) scratch_in[2].i * scratch_tw[1].i) >> NE10_F2I16_SHIFT);
                scratch[2].i = (ne10_int16_t) ( ( (NE10_F2I16_SAMPPROD) scratch_in[2].i * scratch_tw[1].r
                                                  + (NE10_F2I16_SAMPPROD) scratch_in[2].r * scratch_tw[1].i) >> NE10_F2I16_SHIFT);

                scratch[3].r = (ne10_int16_t) ( ( (NE10_F2I16_SAMPPROD) scratch_in[3].r * scratch_tw[2].r
                                                  - (NE10_F2I16_SAMPPROD) scratch_in[3].i * scratch_tw[2].i) >> NE10_F2I16_SHIFT);
                scratch[3].i = (ne10_int16_t) ( ( (NE10_F2I16_SAMPPROD) scratch_in[3].i * scratch_tw[2].r
                                                  + (NE10_F2I16_SAMPPROD) scratch_in[3].r * scratch_tw[2].i) >> NE10_F2I16_SHIFT);

                // radix 2 butterfly
                scratch[4].r = scratch[0].r + scratch[2].r;
                scratch[4].i = scratch[0].i + scratch[2].i;

                scratch[5].r = scratch[0].r - scratch[2].r;
                scratch[5].i = scratch[0].i - scratch[2].i;

                // radix 2 butterfly
                scratch[6].r = scratch[1].r + scratch[3].r;
                scratch[6].i = scratch[1].i + scratch[3].i;

                scratch[7].r = scratch[1].r - scratch[3].r;
                scratch[7].i = scratch[1].i - scratch[3].i;

                // third result
                scratch_out[2].r = scratch[4].r - scratch[6].r;
                scratch_out[2].i = scratch[4].i - scratch[6].i;

                // first result
                scratch_out[0].r = scratch[4].r + scratch[6].r;
                scratch_out[0].i = scratch[4].i + scratch[6].i;

                // second result
                scratch_out[1].r = scratch[5].r + scratch[7].i;
                scratch_out[1].i = scratch[5].i - scratch[7].r;

                // forth result
                scratch_out[3].r = scratch[5].r - scratch[7].i;
                scratch_out[3].i = scratch[5].i + scratch[7].r;

                // store
                *Fout1 = scratch_out[0];
                Fout2 = Fout1 + mstride;
                *Fout2 = scratch_out[1];
                Fout2 += mstride;
                *Fout2 = scratch_out[2];
                Fout2 += mstride;
                *Fout2 = scratch_out[3];

                tw1++;
                Fin1 ++;
                Fout1 ++;
            } // m_count
        } // f_count
        tw += mstride * 3;
        mstride <<= 2;
        // swap
        Ftmp = buffer;
        buffer = Fout;
        Fout = Ftmp;
        fstride >>= 2;
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
            for (m_count = mstride; m_count ; m_count --)
            {
                // load
                scratch_tw[0] = *tw1;
                tw2 = tw1 + mstride;
                scratch_tw[1] = *tw2;
                tw2 += mstride;
                scratch_tw[2] = *tw2;
                scratch_in[0] = * Fin1;
                Fin2 = Fin1 + N;
                scratch_in[1] = * Fin2;
                Fin2 += N;
                scratch_in[2] = * Fin2;
                Fin2 += N;
                scratch_in[3] = * Fin2;
                if (scaled_flag == 1)
                {
                    NE10_F2I16_FIXDIV (scratch_in[0], 4);
                    NE10_F2I16_FIXDIV (scratch_in[1], 4);
                    NE10_F2I16_FIXDIV (scratch_in[2], 4);
                    NE10_F2I16_FIXDIV (scratch_in[3], 4);
                }

                // radix 4 butterfly with twiddles

                scratch[0] = scratch_in[0];
                scratch[1].r = (ne10_int16_t) ( ( (NE10_F2I16_SAMPPROD) scratch_in[1].r * scratch_tw[0].r
                                                  - (NE10_F2I16_SAMPPROD) scratch_in[1].i * scratch_tw[0].i) >> NE10_F2I16_SHIFT);
                scratch[1].i = (ne10_int16_t) ( ( (NE10_F2I16_SAMPPROD) scratch_in[1].i * scratch_tw[0].r
                                                  + (NE10_F2I16_SAMPPROD) scratch_in[1].r * scratch_tw[0].i) >> NE10_F2I16_SHIFT);

                scratch[2].r = (ne10_int16_t) ( ( (NE10_F2I16_SAMPPROD) scratch_in[2].r * scratch_tw[1].r
                                                  - (NE10_F2I16_SAMPPROD) scratch_in[2].i * scratch_tw[1].i) >> NE10_F2I16_SHIFT);
                scratch[2].i = (ne10_int16_t) ( ( (NE10_F2I16_SAMPPROD) scratch_in[2].i * scratch_tw[1].r
                                                  + (NE10_F2I16_SAMPPROD) scratch_in[2].r * scratch_tw[1].i) >> NE10_F2I16_SHIFT);

                scratch[3].r = (ne10_int16_t) ( ( (NE10_F2I16_SAMPPROD) scratch_in[3].r * scratch_tw[2].r
                                                  - (NE10_F2I16_SAMPPROD) scratch_in[3].i * scratch_tw[2].i) >> NE10_F2I16_SHIFT);
                scratch[3].i = (ne10_int16_t) ( ( (NE10_F2I16_SAMPPROD) scratch_in[3].i * scratch_tw[2].r
                                                  + (NE10_F2I16_SAMPPROD) scratch_in[3].r * scratch_tw[2].i) >> NE10_F2I16_SHIFT);

                // radix 2 butterfly
                scratch[4].r = scratch[0].r + scratch[2].r;
                scratch[4].i = scratch[0].i + scratch[2].i;

                scratch[5].r = scratch[0].r - scratch[2].r;
                scratch[5].i = scratch[0].i - scratch[2].i;

                // radix 2 butterfly
                scratch[6].r = scratch[1].r + scratch[3].r;
                scratch[6].i = scratch[1].i + scratch[3].i;

                scratch[7].r = scratch[1].r - scratch[3].r;
                scratch[7].i = scratch[1].i - scratch[3].i;

                // third result
                scratch_out[2].r = scratch[4].r - scratch[6].r;
                scratch_out[2].i = scratch[4].i - scratch[6].i;

                // first result
                scratch_out[0].r = scratch[4].r + scratch[6].r;
                scratch_out[0].i = scratch[4].i + scratch[6].i;

                // second result
                scratch_out[1].r = scratch[5].r + scratch[7].i;
                scratch_out[1].i = scratch[5].i - scratch[7].r;

                // forth result
                scratch_out[3].r = scratch[5].r - scratch[7].i;
                scratch_out[3].i = scratch[5].i + scratch[7].r;

                // store
                *Fout1 = scratch_out[0];
                Fout2 = Fout1 + N;
                *Fout2 = scratch_out[1];
                Fout2 += N;
                *Fout2 = scratch_out[2];
                Fout2 += N;
                *Fout2 = scratch_out[3];

                tw1 ++;
                Fin1 ++;
                Fout1 ++;
            } // m_count
        } // f_count
    } // last stage
}

static void ne10_mixed_radix_butterfly_inverse_int16_c (ne10_fft_cpx_int16_t * Fout,
        ne10_fft_cpx_int16_t   * Fin,
        ne10_int32_t * factors,
        ne10_fft_cpx_int16_t * twiddles,
        ne10_fft_cpx_int16_t * buffer,
        ne10_int32_t scaled_flag)
{
    ne10_int32_t fstride, mstride, N;
    ne10_int32_t fstride1;
    ne10_int32_t f_count, m_count;
    ne10_int32_t stage_count;
    ne10_int32_t order;

    ne10_fft_cpx_int16_t   scratch_in[8];
    ne10_fft_cpx_int16_t   scratch_out[8];
    ne10_fft_cpx_int16_t   scratch[16];
    ne10_fft_cpx_int16_t   scratch_tw[6];

    ne10_fft_cpx_int16_t   *Fin1, *Fin2, *Fout1, *Fout2;
    ne10_fft_cpx_int16_t   *Fout_ls = Fout;
    ne10_fft_cpx_int16_t   *Ftmp;
    ne10_fft_cpx_int16_t   *tw, *tw1, *tw2;
    const ne10_int32_t TW_81 = 23169;
    const ne10_int32_t TW_81N = -23169;

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
        order = 2 * stage_count - 1;
        N = fstride >> 1; // 1/4 of length of FFT
        tw = twiddles;
        fstride1 = fstride >> 2;

        Fin1 = Fin;
        for (f_count = 0; f_count < fstride1; f_count ++)
        {
            Fout1 = & Fout[ f_count * 8 ];
            // load
            if (scaled_flag == 1)
            {
                NE10_F2I16_FIXDIV (Fin1[0], 8);
                NE10_F2I16_FIXDIV (Fin1[0 + fstride], 8);
                NE10_F2I16_FIXDIV (Fin1[fstride1], 8);
                NE10_F2I16_FIXDIV (Fin1[fstride1 + fstride], 8);
                NE10_F2I16_FIXDIV (Fin1[fstride1 * 2], 8);
                NE10_F2I16_FIXDIV (Fin1[fstride1 * 2 + fstride], 8);
                NE10_F2I16_FIXDIV (Fin1[fstride1 * 3], 8);
                NE10_F2I16_FIXDIV (Fin1[fstride1 * 3 + fstride], 8);
            }

            scratch_in[0].r = Fin1[0].r + Fin1[0 + fstride].r;
            scratch_in[0].i = Fin1[0].i + Fin1[0 + fstride].i;
            scratch_in[1].r = Fin1[0].r - Fin1[0 + fstride].r;
            scratch_in[1].i = Fin1[0].i - Fin1[0 + fstride].i;
            scratch_in[2].r = Fin1[fstride1].r + Fin1[fstride1 + fstride].r;
            scratch_in[2].i = Fin1[fstride1].i + Fin1[fstride1 + fstride].i;
            scratch_in[3].r = Fin1[fstride1].r - Fin1[fstride1 + fstride].r;
            scratch_in[3].i = Fin1[fstride1].i - Fin1[fstride1 + fstride].i;
            scratch_in[4].r = Fin1[fstride1 * 2].r + Fin1[fstride1 * 2 + fstride].r;
            scratch_in[4].i = Fin1[fstride1 * 2].i + Fin1[fstride1 * 2 + fstride].i;
            scratch_in[5].r = Fin1[fstride1 * 2].r - Fin1[fstride1 * 2 + fstride].r;
            scratch_in[5].i = Fin1[fstride1 * 2].i - Fin1[fstride1 * 2 + fstride].i;
            scratch_in[6].r = Fin1[fstride1 * 3].r + Fin1[fstride1 * 3 + fstride].r;
            scratch_in[6].i = Fin1[fstride1 * 3].i + Fin1[fstride1 * 3 + fstride].i;
            scratch_in[7].r = Fin1[fstride1 * 3].r - Fin1[fstride1 * 3 + fstride].r;
            scratch_in[7].i = Fin1[fstride1 * 3].i - Fin1[fstride1 * 3 + fstride].i;

            // radix 4 butterfly with twiddles

            scratch[0] = scratch_in[0];
            scratch[1] = scratch_in[1];

            scratch[2] = scratch_in[2];
            scratch[3].r = (ne10_int16_t) ( ( (NE10_F2I16_SAMPPROD) (scratch_in[3].r - scratch_in[3].i) * TW_81) >> NE10_F2I16_SHIFT);
            scratch[3].i = (ne10_int16_t) ( ( (NE10_F2I16_SAMPPROD) (scratch_in[3].i + scratch_in[3].r) * TW_81) >> NE10_F2I16_SHIFT);

            scratch[4] = scratch_in[4];
            scratch[5].r = -scratch_in[5].i;
            scratch[5].i = scratch_in[5].r;

            scratch[6].r = scratch_in[6].r;
            scratch[6].i = scratch_in[6].i;
            scratch[7].r = (ne10_int16_t) ( ( (NE10_F2I16_SAMPPROD) (scratch_in[7].r + scratch_in[7].i) * TW_81N) >> NE10_F2I16_SHIFT);
            scratch[7].i = (ne10_int16_t) ( ( (NE10_F2I16_SAMPPROD) (scratch_in[7].i - scratch_in[7].r) * TW_81N) >> NE10_F2I16_SHIFT);

            // radix 2 butterfly
            scratch[8].r = scratch[0].r + scratch[4].r;
            scratch[8].i = scratch[0].i + scratch[4].i;
            scratch[9].r = scratch[1].r + scratch[5].r;
            scratch[9].i = scratch[1].i + scratch[5].i;

            scratch[10].r = scratch[0].r - scratch[4].r;
            scratch[10].i = scratch[0].i - scratch[4].i;
            scratch[11].r = scratch[1].r - scratch[5].r;
            scratch[11].i = scratch[1].i - scratch[5].i;

            // radix 2 butterfly
            scratch[12].r = scratch[2].r + scratch[6].r;
            scratch[12].i = scratch[2].i + scratch[6].i;
            scratch[13].r = scratch[3].r + scratch[7].r;
            scratch[13].i = scratch[3].i + scratch[7].i;

            scratch[14].r = scratch[2].r - scratch[6].r;
            scratch[14].i = scratch[2].i - scratch[6].i;
            scratch[15].r = scratch[3].r - scratch[7].r;
            scratch[15].i = scratch[3].i - scratch[7].i;

            // third result
            scratch_out[4].r = scratch[8].r - scratch[12].r;
            scratch_out[4].i = scratch[8].i - scratch[12].i;
            scratch_out[5].r = scratch[9].r - scratch[13].r;
            scratch_out[5].i = scratch[9].i - scratch[13].i;

            // first result
            scratch_out[0].r = scratch[8].r + scratch[12].r;
            scratch_out[0].i = scratch[8].i + scratch[12].i;
            scratch_out[1].r = scratch[9].r + scratch[13].r;
            scratch_out[1].i = scratch[9].i + scratch[13].i;

            // second result
            scratch_out[2].r = scratch[10].r - scratch[14].i;
            scratch_out[2].i = scratch[10].i + scratch[14].r;
            scratch_out[3].r = scratch[11].r - scratch[15].i;
            scratch_out[3].i = scratch[11].i + scratch[15].r;

            // forth result
            scratch_out[6].r = scratch[10].r + scratch[14].i;
            scratch_out[6].i = scratch[10].i - scratch[14].r;
            scratch_out[7].r = scratch[11].r + scratch[15].i;
            scratch_out[7].i = scratch[11].i - scratch[15].r;

            // store
            Fout1[0] = scratch_out[0];
            Fout1[1] = scratch_out[1];
            Fout1[2] = scratch_out[2];
            Fout1[3] = scratch_out[3];
            Fout1[4] = scratch_out[4];
            Fout1[5] = scratch_out[5];
            Fout1[6] = scratch_out[6];
            Fout1[7] = scratch_out[7];

            Fin1 += 1;
        } // f_count
        tw += 6;
        mstride <<= 2;
        fstride >>= 4;
        stage_count -= 2;

        if (stage_count == 0)
        {
            for (f_count = 0; f_count < 8; f_count++)
            {
                Fout[f_count].r >>= order;
                Fout[f_count].i >>= order;
            }
        }

        // swap
        Ftmp = buffer;
        buffer = Fout;
        Fout = Ftmp;
    }
    else if (N == 4)   // length of FFT is 2^n (n is even)
    {
        //fstride is nfft>>2
        order = 2 * stage_count;
        for (f_count = fstride; f_count ; f_count --)
        {
            // load
            scratch_in[0] = *Fin1;
            Fin2 = Fin1 + fstride;
            scratch_in[1] = *Fin2;
            Fin2 = Fin2 + fstride;
            scratch_in[2] = *Fin2;
            Fin2 = Fin2 + fstride;
            scratch_in[3] = *Fin2;

            // radix 4 butterfly without twiddles
            if (scaled_flag == 1)
            {
                NE10_F2I16_FIXDIV (scratch_in[0], 4);
                NE10_F2I16_FIXDIV (scratch_in[1], 4);
                NE10_F2I16_FIXDIV (scratch_in[2], 4);
                NE10_F2I16_FIXDIV (scratch_in[3], 4);
            }

            // radix 2 butterfly
            scratch[0].r = scratch_in[0].r + scratch_in[2].r;
            scratch[0].i = scratch_in[0].i + scratch_in[2].i;

            scratch[1].r = scratch_in[0].r - scratch_in[2].r;
            scratch[1].i = scratch_in[0].i - scratch_in[2].i;

            // radix 2 butterfly
            scratch[2].r = scratch_in[1].r + scratch_in[3].r;
            scratch[2].i = scratch_in[1].i + scratch_in[3].i;

            scratch[3].r = scratch_in[1].r - scratch_in[3].r;
            scratch[3].i = scratch_in[1].i - scratch_in[3].i;

            // third result
            scratch_out[2].r = scratch[0].r - scratch[2].r;
            scratch_out[2].i = scratch[0].i - scratch[2].i;

            // first result
            scratch_out[0].r = scratch[0].r + scratch[2].r;
            scratch_out[0].i = scratch[0].i + scratch[2].i;

            // second result
            scratch_out[1].r = scratch[1].r - scratch[3].i;
            scratch_out[1].i = scratch[1].i + scratch[3].r;

            // forth result
            scratch_out[3].r = scratch[1].r + scratch[3].i;
            scratch_out[3].i = scratch[1].i - scratch[3].r;

            // store
            * Fout1 ++ = scratch_out[0];
            * Fout1 ++ = scratch_out[1];
            * Fout1 ++ = scratch_out[2];
            * Fout1 ++ = scratch_out[3];

            Fin1++;
        } // f_count

        N = fstride; // 1/4 of length of FFT
        // update address for other stages
        stage_count--;
        tw = twiddles;
        fstride >>= 2;

        if (stage_count == 0)
        {
            for (f_count = 0; f_count < 4; f_count++)
            {
                Fout[f_count].r >>= order;
                Fout[f_count].i >>= order;
            }
        }

        // swap
        Ftmp = buffer;
        buffer = Fout;
        Fout = Ftmp;

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
            for (m_count = mstride; m_count ; m_count --)
            {
                // load
                scratch_tw[0] = *tw1;
                tw2 = tw1 + mstride;
                scratch_tw[1] = *tw2;
                tw2 += mstride;
                scratch_tw[2] = *tw2;
                scratch_in[0] = * Fin1;
                Fin2 = Fin1 + N;
                scratch_in[1] = * Fin2;
                Fin2 += N;
                scratch_in[2] = * Fin2;
                Fin2 += N;
                scratch_in[3] = * Fin2;
                if (scaled_flag == 1)
                {
                    NE10_F2I16_FIXDIV (scratch_in[0], 4);
                    NE10_F2I16_FIXDIV (scratch_in[1], 4);
                    NE10_F2I16_FIXDIV (scratch_in[2], 4);
                    NE10_F2I16_FIXDIV (scratch_in[3], 4);
                }

                // radix 4 butterfly with twiddles

                scratch[0] = scratch_in[0];
                scratch[1].r = (ne10_int16_t) ( ( (NE10_F2I16_SAMPPROD) scratch_in[1].r * scratch_tw[0].r
                                                  + (NE10_F2I16_SAMPPROD) scratch_in[1].i * scratch_tw[0].i) >> NE10_F2I16_SHIFT);
                scratch[1].i = (ne10_int16_t) ( ( (NE10_F2I16_SAMPPROD) scratch_in[1].i * scratch_tw[0].r
                                                  - (NE10_F2I16_SAMPPROD) scratch_in[1].r * scratch_tw[0].i) >> NE10_F2I16_SHIFT);

                scratch[2].r = (ne10_int16_t) ( ( (NE10_F2I16_SAMPPROD) scratch_in[2].r * scratch_tw[1].r
                                                  + (NE10_F2I16_SAMPPROD) scratch_in[2].i * scratch_tw[1].i) >> NE10_F2I16_SHIFT);
                scratch[2].i = (ne10_int16_t) ( ( (NE10_F2I16_SAMPPROD) scratch_in[2].i * scratch_tw[1].r
                                                  - (NE10_F2I16_SAMPPROD) scratch_in[2].r * scratch_tw[1].i) >> NE10_F2I16_SHIFT);

                scratch[3].r = (ne10_int16_t) ( ( (NE10_F2I16_SAMPPROD) scratch_in[3].r * scratch_tw[2].r
                                                  + (NE10_F2I16_SAMPPROD) scratch_in[3].i * scratch_tw[2].i) >> NE10_F2I16_SHIFT);
                scratch[3].i = (ne10_int16_t) ( ( (NE10_F2I16_SAMPPROD) scratch_in[3].i * scratch_tw[2].r
                                                  - (NE10_F2I16_SAMPPROD) scratch_in[3].r * scratch_tw[2].i) >> NE10_F2I16_SHIFT);

                // radix 2 butterfly
                scratch[4].r = scratch[0].r + scratch[2].r;
                scratch[4].i = scratch[0].i + scratch[2].i;

                scratch[5].r = scratch[0].r - scratch[2].r;
                scratch[5].i = scratch[0].i - scratch[2].i;

                // radix 2 butterfly
                scratch[6].r = scratch[1].r + scratch[3].r;
                scratch[6].i = scratch[1].i + scratch[3].i;

                scratch[7].r = scratch[1].r - scratch[3].r;
                scratch[7].i = scratch[1].i - scratch[3].i;

                // third result
                scratch_out[2].r = scratch[4].r - scratch[6].r;
                scratch_out[2].i = scratch[4].i - scratch[6].i;

                // first result
                scratch_out[0].r = scratch[4].r + scratch[6].r;
                scratch_out[0].i = scratch[4].i + scratch[6].i;

                // second result
                scratch_out[1].r = scratch[5].r - scratch[7].i;
                scratch_out[1].i = scratch[5].i + scratch[7].r;

                // forth result
                scratch_out[3].r = scratch[5].r + scratch[7].i;
                scratch_out[3].i = scratch[5].i - scratch[7].r;

                // store
                *Fout1 = scratch_out[0];
                Fout2 = Fout1 + mstride;
                *Fout2 = scratch_out[1];
                Fout2 += mstride;
                *Fout2 = scratch_out[2];
                Fout2 += mstride;
                *Fout2 = scratch_out[3];

                tw1++;
                Fin1 ++;
                Fout1 ++;
            } // m_count
        } // f_count
        tw += mstride * 3;
        mstride <<= 2;
        // swap
        Ftmp = buffer;
        buffer = Fout;
        Fout = Ftmp;
        fstride >>= 2;
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
            for (m_count = mstride; m_count ; m_count --)
            {
                // load
                scratch_tw[0] = *tw1;
                tw2 = tw1 + mstride;
                scratch_tw[1] = *tw2;
                tw2 += mstride;
                scratch_tw[2] = *tw2;
                scratch_in[0] = * Fin1;
                Fin2 = Fin1 + N;
                scratch_in[1] = * Fin2;
                Fin2 += N;
                scratch_in[2] = * Fin2;
                Fin2 += N;
                scratch_in[3] = * Fin2;
                if (scaled_flag == 1)
                {
                    NE10_F2I16_FIXDIV (scratch_in[0], 4);
                    NE10_F2I16_FIXDIV (scratch_in[1], 4);
                    NE10_F2I16_FIXDIV (scratch_in[2], 4);
                    NE10_F2I16_FIXDIV (scratch_in[3], 4);
                }

                // radix 4 butterfly with twiddles

                scratch[0] = scratch_in[0];
                scratch[1].r = (ne10_int16_t) ( ( (NE10_F2I16_SAMPPROD) scratch_in[1].r * scratch_tw[0].r
                                                  + (NE10_F2I16_SAMPPROD) scratch_in[1].i * scratch_tw[0].i) >> NE10_F2I16_SHIFT);
                scratch[1].i = (ne10_int16_t) ( ( (NE10_F2I16_SAMPPROD) scratch_in[1].i * scratch_tw[0].r
                                                  - (NE10_F2I16_SAMPPROD) scratch_in[1].r * scratch_tw[0].i) >> NE10_F2I16_SHIFT);

                scratch[2].r = (ne10_int16_t) ( ( (NE10_F2I16_SAMPPROD) scratch_in[2].r * scratch_tw[1].r
                                                  + (NE10_F2I16_SAMPPROD) scratch_in[2].i * scratch_tw[1].i) >> NE10_F2I16_SHIFT);
                scratch[2].i = (ne10_int16_t) ( ( (NE10_F2I16_SAMPPROD) scratch_in[2].i * scratch_tw[1].r
                                                  - (NE10_F2I16_SAMPPROD) scratch_in[2].r * scratch_tw[1].i) >> NE10_F2I16_SHIFT);

                scratch[3].r = (ne10_int16_t) ( ( (NE10_F2I16_SAMPPROD) scratch_in[3].r * scratch_tw[2].r
                                                  + (NE10_F2I16_SAMPPROD) scratch_in[3].i * scratch_tw[2].i) >> NE10_F2I16_SHIFT);
                scratch[3].i = (ne10_int16_t) ( ( (NE10_F2I16_SAMPPROD) scratch_in[3].i * scratch_tw[2].r
                                                  - (NE10_F2I16_SAMPPROD) scratch_in[3].r * scratch_tw[2].i) >> NE10_F2I16_SHIFT);

                // radix 2 butterfly
                scratch[4].r = scratch[0].r + scratch[2].r;
                scratch[4].i = scratch[0].i + scratch[2].i;

                scratch[5].r = scratch[0].r - scratch[2].r;
                scratch[5].i = scratch[0].i - scratch[2].i;

                // radix 2 butterfly
                scratch[6].r = scratch[1].r + scratch[3].r;
                scratch[6].i = scratch[1].i + scratch[3].i;

                scratch[7].r = scratch[1].r - scratch[3].r;
                scratch[7].i = scratch[1].i - scratch[3].i;

                // third result
                scratch_out[2].r = (scratch[4].r - scratch[6].r) >> order;
                scratch_out[2].i = (scratch[4].i - scratch[6].i) >> order;

                // first result
                scratch_out[0].r = (scratch[4].r + scratch[6].r) >> order;
                scratch_out[0].i = (scratch[4].i + scratch[6].i) >> order;

                // second result
                scratch_out[1].r = (scratch[5].r - scratch[7].i) >> order;
                scratch_out[1].i = (scratch[5].i + scratch[7].r) >> order;

                // forth result
                scratch_out[3].r = (scratch[5].r + scratch[7].i) >> order;
                scratch_out[3].i = (scratch[5].i - scratch[7].r) >> order;

                // store
                *Fout1 = scratch_out[0];
                Fout2 = Fout1 + N;
                *Fout2 = scratch_out[1];
                Fout2 += N;
                *Fout2 = scratch_out[2];
                Fout2 += N;
                *Fout2 = scratch_out[3];

                tw1 ++;
                Fin1 ++;
                Fout1 ++;
            } // m_count
        } // f_count
    } // last stage
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

static void ne10_fft_split_r2c_1d_int16 (ne10_fft_cpx_int16_t *dst,
        const ne10_fft_cpx_int16_t *src,
        ne10_fft_cpx_int16_t *twiddles,
        ne10_int32_t ncfft,
        ne10_int32_t scaled_flag)
{
    ne10_int32_t k;
    ne10_fft_cpx_int16_t fpnk, fpk, f1k, f2k, tw, tdc;

    tdc.r = src[0].r;
    tdc.i = src[0].i;

    if (scaled_flag)
        NE10_F2I16_FIXDIV (tdc, 2);

    dst[0].r = tdc.r + tdc.i;
    dst[ncfft].r = tdc.r - tdc.i;
    dst[ncfft].i = dst[0].i = 0;

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

static void ne10_fft_split_c2r_1d_int16 (ne10_fft_cpx_int16_t *dst,
        const ne10_fft_cpx_int16_t *src,
        ne10_fft_cpx_int16_t *twiddles,
        ne10_int32_t ncfft,
        ne10_int32_t scaled_flag)
{

    ne10_int32_t k;
    ne10_fft_cpx_int16_t fk, fnkc, fek, fok, tmp;


    dst[0].r = (src[0].r + src[ncfft].r) >> 1;
    dst[0].i = (src[0].r - src[ncfft].r) >> 1;

    if (scaled_flag)
        NE10_F2I16_FIXDIV (dst[0], 2);

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

        dst[k].r = (fek.r + fok.r) >> 1;
        dst[k].i = (fek.i + fok.i) >> 1;

        dst[ncfft - k].r = (fek.r - fok.r) >> 1;
        dst[ncfft - k].i = (fok.i - fek.i) >> 1;
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
ne10_fft_cfg_int16_t ne10_fft_alloc_c2c_int16 (ne10_int32_t nfft)
{
    ne10_fft_cfg_int16_t st = NULL;
    ne10_uint32_t memneeded = sizeof (ne10_fft_state_int16_t)
                              + sizeof (ne10_int32_t) * (NE10_MAXFACTORS * 2) /* factors */
                              + sizeof (ne10_fft_cpx_int16_t) * (nfft)        /* twiddle */
                              + sizeof (ne10_fft_cpx_int16_t) * nfft;         /* buffer */

    st = (ne10_fft_cfg_int16_t) NE10_MALLOC (memneeded);

    if (st)
    {
        st->factors = (ne10_int32_t*) ( (ne10_int8_t*) st + sizeof (ne10_fft_state_int16_t));
        st->twiddles = (ne10_fft_cpx_int16_t*) (st->factors + (NE10_MAXFACTORS * 2));
        st->buffer = st->twiddles + nfft;
        st->nfft = nfft;

        ne10_int32_t result = ne10_factor (nfft, st->factors);
        if (result == NE10_ERR)
        {
            NE10_FREE (st);
            return st;
        }

        ne10_int32_t i, j;
        ne10_int32_t *factors = st->factors;
        ne10_fft_cpx_int16_t *twiddles = st->twiddles;
        ne10_fft_cpx_int16_t *tw;
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
                tw->r = (ne10_int16_t) floor (0.5f + NE10_F2I16_MAX * cos (phase1));
                tw->i = (ne10_int16_t) floor (0.5f + NE10_F2I16_MAX * sin (phase1));
                (tw + m)->r = (ne10_int16_t) floor (0.5f + NE10_F2I16_MAX * cos (phase2));
                (tw + m)->i = (ne10_int16_t) floor (0.5f + NE10_F2I16_MAX * sin (phase2));
                (tw + m * 2)->r = (ne10_int16_t) floor (0.5f + NE10_F2I16_MAX * cos (phase3));
                (tw + m * 2)->i = (ne10_int16_t) floor (0.5f + NE10_F2I16_MAX * sin (phase3));
                tw++;
            }
            twiddles += m * 3;
        }

    }
    return st;
}

/**
 * @brief Mixed radix-2/4 complex FFT/IFFT of 16-bit fixed point data.
 * @param[out]  *fout            point to the output buffer (out-of-place)
 * @param[in]   *fin             point to the input buffer (out-of-place)
 * @param[in]   cfg              point to the config struct
 * @param[in]   inverse_fft      the flag of IFFT, 0: FFT, 1: IFFT
 * @param[in]   scaled_flag      scale flag, 0 unscaled, 1 scaled
 * @return none.
 * The function implements a mixed radix-2/4 complex FFT/IFFT. The length of 2^N(N is 2, 3, 4, 5, 6 ....etc) is supported.
 * Otherwise, this FFT is an out-of-place algorithm. For the usage of this function, please check test/test_suite_fft_int16.c
 */
void ne10_fft_c2c_1d_int16_c (ne10_fft_cpx_int16_t *fout,
                              ne10_fft_cpx_int16_t *fin,
                              ne10_fft_cfg_int16_t cfg,
                              ne10_int32_t inverse_fft,
                              ne10_int32_t scaled_flag)
{
    if (inverse_fft)
        ne10_mixed_radix_butterfly_inverse_int16_c (fout, fin, cfg->factors, cfg->twiddles, cfg->buffer, scaled_flag);
    else
        ne10_mixed_radix_butterfly_int16_c (fout, fin, cfg->factors, cfg->twiddles, cfg->buffer, scaled_flag);
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
ne10_fft_r2c_cfg_int16_t ne10_fft_alloc_r2c_int16 (ne10_int32_t nfft)
{
    ne10_fft_r2c_cfg_int16_t st = NULL;
    ne10_int32_t ncfft = nfft >> 1;

    ne10_uint32_t memneeded = sizeof (ne10_fft_r2c_state_int16_t)
                              + sizeof (ne10_int32_t) * (NE10_MAXFACTORS * 2) /* factors */
                              + sizeof (ne10_fft_cpx_int16_t) * ncfft         /* twiddle*/
                              + sizeof (ne10_fft_cpx_int16_t) * ncfft / 2     /* super twiddles*/
                              + sizeof (ne10_fft_cpx_int32_t) * nfft;         /* buffer*/

    st = (ne10_fft_r2c_cfg_int16_t) NE10_MALLOC (memneeded);

    if (st)
    {
        st->factors = (ne10_int32_t*) ( (ne10_int8_t*) st + sizeof (ne10_fft_r2c_state_int16_t));
        st->twiddles = (ne10_fft_cpx_int16_t*) (st->factors + (NE10_MAXFACTORS * 2));
        st->super_twiddles = st->twiddles + ncfft;
        st->buffer = st->super_twiddles + (ncfft / 2);
        st->ncfft = ncfft;

        ne10_int32_t result = ne10_factor (ncfft, st->factors);
        if (result == NE10_ERR)
        {
            NE10_FREE (st);
            return st;
        }

        ne10_int32_t i, j;
        ne10_int32_t *factors = st->factors;
        ne10_fft_cpx_int16_t *twiddles = st->twiddles;
        ne10_fft_cpx_int16_t *tw;
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
                tw->r = (ne10_int16_t) floor (0.5f + NE10_F2I16_MAX * cos (phase1));
                tw->i = (ne10_int16_t) floor (0.5f + NE10_F2I16_MAX * sin (phase1));
                (tw + m)->r = (ne10_int16_t) floor (0.5f + NE10_F2I16_MAX * cos (phase2));
                (tw + m)->i = (ne10_int16_t) floor (0.5f + NE10_F2I16_MAX * sin (phase2));
                (tw + m * 2)->r = (ne10_int16_t) floor (0.5f + NE10_F2I16_MAX * cos (phase3));
                (tw + m * 2)->i = (ne10_int16_t) floor (0.5f + NE10_F2I16_MAX * sin (phase3));
                tw++;
            }
            twiddles += m * 3;
        }

        tw = st->super_twiddles;
        for (i = 0; i < ncfft / 2; i++)
        {
            phase1 = -pi * ( (ne10_float32_t) (i + 1) / ncfft + 0.5f);
            tw->r = (ne10_int16_t) floor (0.5f + NE10_F2I16_MAX * cos (phase1));
            tw->i = (ne10_int16_t) floor (0.5f + NE10_F2I16_MAX * sin (phase1));
            tw++;
        }

    }
    return st;
}

/**
 * @brief Mixed radix-2/4 FFT (real to complex) of int16 data.
 * @param[out]  *fout            point to the output buffer
 * @param[in]   *fin             point to the input buffer
 * @param[in]   cfg              point to the config struct
 * @param[in]   scaled_flag      scale flag, 0 unscaled, 1 scaled
 * @return none.
 * The function implements a mixed radix-2/4 FFT (real to complex). The length of 2^N(N is 3, 4, 5, 6 ....etc) is supported.
 * Otherwise, we alloc a temp buffer(the size is same as input buffer) for storing intermedia.
 * For the usage of this function, please check test/test_suite_fft_int16.c
 */
void ne10_fft_r2c_1d_int16_c (ne10_fft_cpx_int16_t *fout,
                              ne10_int16_t *fin,
                              ne10_fft_r2c_cfg_int16_t cfg,
                              ne10_int32_t scaled_flag)
{
    ne10_fft_cpx_int16_t * tmpbuf = cfg->buffer;

    ne10_mixed_radix_butterfly_int16_c (tmpbuf, (ne10_fft_cpx_int16_t*) fin, cfg->factors, cfg->twiddles, fout, scaled_flag);
    ne10_fft_split_r2c_1d_int16 (fout, tmpbuf, cfg->super_twiddles, cfg->ncfft, scaled_flag);
}

/**
 * @brief Mixed radix-2/4 IFFT (complex to real) of int16 data.
 * @param[out]  *fout            point to the output buffer
 * @param[in]   *fin             point to the input buffer
 * @param[in]   cfg              point to the config struct
 * @param[in]   scaled_flag      scale flag, 0 unscaled, 1 scaled
 * @return none.
 * The function implements a mixed radix-2/4 FFT (complex to real). The length of 2^N(N is 3, 4, 5, 6 ....etc) is supported.
 * Otherwise, we alloc a temp buffer(the size is same as input buffer) for storing intermedia.
 * For the usage of this function, please check test/test_suite_fft_int16.c
 */
void ne10_fft_c2r_1d_int16_c (ne10_int16_t *fout,
                              ne10_fft_cpx_int16_t *fin,
                              ne10_fft_r2c_cfg_int16_t cfg,
                              ne10_int32_t scaled_flag)
{
    ne10_fft_cpx_int16_t * tmpbuf1 = cfg->buffer;
    ne10_fft_cpx_int16_t * tmpbuf2 = cfg->buffer + cfg->ncfft;

    ne10_fft_split_c2r_1d_int16 (tmpbuf1, fin, cfg->super_twiddles, cfg->ncfft, scaled_flag);
    ne10_mixed_radix_butterfly_inverse_int16_c ( (ne10_fft_cpx_int16_t*) fout, tmpbuf1, cfg->factors, cfg->twiddles, tmpbuf2, scaled_flag);
}

/**
 * @} end of R2C_FFT_IFFT group
 */
