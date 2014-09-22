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
 * NE10 Library : dsp/NE10_fft.c
 */

#include "NE10_types.h"
#include "NE10_macros.h"
#include "NE10_fft.h"

/* factors buffer:
 * 0: stage number
 * 1: stride for the first stage
 * others: factors */
ne10_int32_t ne10_factor (ne10_int32_t n, ne10_int32_t * facbuf)
{
    ne10_int32_t p;
    ne10_int32_t i = 1;
    ne10_int32_t stage_num = 0;
    ne10_int32_t stride_max = n;

    /* factor out powers of 4, 2, 5, 3, and other */
    do
    {
        if ( (n % 4) == 0)
            p = 4;
        else if ( (n % 2) == 0)
            p = 2;
        else if ( (n % 5) == 0)
            p = 5;
        else if ( (n % 3) == 0)
            p = 3;
        else // stop factoring
            p = n;

        n /= p;
        facbuf[2 * i] = p;
        facbuf[2 * i + 1] = n;
        i++;
        stage_num++;
    }
    while (n > 1);
    facbuf[0] = stage_num;
    facbuf[1] = stride_max / p;

    if ( ((p % 2) == 0) && (stride_max >= 4) )
    {
        // last factor is 4 or 2
        // and nfft >= 4
        facbuf[2 * i] = NE10_FFT_ALG_24;
        return NE10_OK;
    }
    else // not support yet
    {
        facbuf[2 * i] = NE10_FFT_ALG_ANY;
        return NE10_ERR;
    }
}

ne10_fft_cpx_float32_t* ne10_fft_generate_twiddles_float32 (ne10_fft_cpx_float32_t * twiddles,
        const ne10_int32_t * factors,
        const ne10_int32_t nfft )
{
    ne10_int32_t i, j, k;
    ne10_fft_cpx_float32_t *tw;
    ne10_int32_t stage_count = factors[0];
    ne10_int32_t fstride = factors[1];
    ne10_int32_t mstride;
    ne10_int32_t cur_radix; // current radix

    const ne10_float64_t pi = NE10_PI;
    ne10_float32_t phase;

    // for first stage
    i = stage_count;
    cur_radix = factors[2 * i];
    if (cur_radix%2) // current radix is not 4 or 2
    {
        for (k = 0; k < cur_radix; k++)
        {
            phase = -2 * pi * k / cur_radix;
            twiddles[k].r = (ne10_float32_t) cos (phase);
            twiddles[k].i = (ne10_float32_t) sin (phase);
        }
        twiddles += cur_radix;
    }

    // for other stage
    for (i = stage_count - 1; i > 0; i--)
    {
        cur_radix = factors[2 * i];
        fstride /= cur_radix;
        mstride = factors[2 * i + 1];
        tw = twiddles;
        for (j = 0; j < mstride; j++)
        {
            for (k = 1; k < cur_radix; k++ ) // phase = 1 when k = 0
            {
                phase = -2 * pi * fstride * k * j / nfft;
                tw[mstride * ( k - 1 )].r = (ne10_float32_t) cos (phase);
                tw[mstride * ( k - 1 )].i = (ne10_float32_t) sin (phase);
            } // cur_radix
            tw ++;
        } // mstride
        twiddles += mstride * (cur_radix - 1);
    } // stage_count

    return twiddles;
}
