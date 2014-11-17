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
 * NE10 Library : dsp/NE10_fft_generic_float32.c
 */

#include "NE10_types.h"
#include "NE10_macros.h"
#include "NE10_fft.h"

///////////////////////////////
// Multiply input with twiddles
///////////////////////////////
static inline void FFT2_MUL_TW (ne10_fft_cpx_float32_t scratch_out[2],
        const ne10_fft_cpx_float32_t scratch_in[2],
        const ne10_fft_cpx_float32_t scratch_tw[1])
{
    scratch_out[0] = scratch_in[0];
    NE10_CPX_MUL_F32 (scratch_out[1], scratch_in[1], scratch_tw[0]);
}

static inline void FFT3_MUL_TW (ne10_fft_cpx_float32_t scratch_out[3],
        const ne10_fft_cpx_float32_t scratch_in[3],
        const ne10_fft_cpx_float32_t scratch_tw[2])
{
    FFT2_MUL_TW (scratch_out, scratch_in, scratch_tw);
    NE10_CPX_MUL_F32 (scratch_out[2], scratch_in[2], scratch_tw[1]);
}

static inline void FFT4_MUL_TW (ne10_fft_cpx_float32_t scratch_out[4],
        const ne10_fft_cpx_float32_t scratch_in[4],
        const ne10_fft_cpx_float32_t scratch_tw[3])
{
    FFT3_MUL_TW (scratch_out, scratch_in, scratch_tw);
    NE10_CPX_MUL_F32 (scratch_out[3], scratch_in[3], scratch_tw[2]);
}

static inline void FFT5_MUL_TW (ne10_fft_cpx_float32_t scratch_out[5],
        const ne10_fft_cpx_float32_t scratch_in[5],
        const ne10_fft_cpx_float32_t scratch_tw[4])
{
    FFT4_MUL_TW (scratch_out, scratch_in, scratch_tw);
    NE10_CPX_MUL_F32 (scratch_out[4], scratch_in[4], scratch_tw[3]);
}

///////////////////
// FFT Kernel
// F: Forward
// C: Complex
// U: Unscaled
//////////////////
static inline void FFT2_FCU (ne10_fft_cpx_float32_t scratch_out[2],
        const ne10_fft_cpx_float32_t scratch_in[2])
{
    NE10_CPX_ADD (scratch_out[0], scratch_in[0], scratch_in[1]);
    NE10_CPX_SUB (scratch_out[1], scratch_in[0], scratch_in[1]);
}

static inline void FFT3_FCU (ne10_fft_cpx_float32_t Fout[3],
        const ne10_fft_cpx_float32_t Fin[3])
{
    ne10_fft_cpx_float32_t scratch[4];
    ne10_fft_cpx_float32_t scratch_in[3];

    scratch_in[0] = Fin[0];
    scratch_in[1] = Fin[1];
    scratch_in[2] = Fin[2];

    scratch[1] = scratch_in[1];
    scratch[2] = scratch_in[2];

    NE10_CPX_ADD (scratch[3], scratch[1], scratch[2]);
    NE10_CPX_SUB (scratch[0], scratch[1], scratch[2]);

    scratch_in[1].r = scratch_in[0].r - scratch[3].r * 0.5;
    scratch_in[1].i = scratch_in[0].i - scratch[3].i * 0.5;

    scratch[0].r *= -TW_3I_F32;
    scratch[0].i *= -TW_3I_F32;

    scratch_in[0].r += scratch[3].r;
    scratch_in[0].i += scratch[3].i;

    scratch_in[2].r = scratch_in[1].r + scratch[0].i;
    scratch_in[2].i = scratch_in[1].i - scratch[0].r;

    scratch_in[1].r -= scratch[0].i;
    scratch_in[1].i += scratch[0].r;

    Fout[0] = scratch_in[0];
    Fout[1] = scratch_in[1];
    Fout[2] = scratch_in[2];
}

static inline void FFT4_FCU (ne10_fft_cpx_float32_t scratch_out[4],
        const ne10_fft_cpx_float32_t scratch_in[4])
{
    ne10_fft_cpx_float32_t scratch[4];

    NE10_CPX_ADD (scratch[0], scratch_in[0], scratch_in[2]);
    NE10_CPX_SUB (scratch[1], scratch_in[0], scratch_in[2]);
    NE10_CPX_ADD (scratch[2], scratch_in[1], scratch_in[3]);
    NE10_CPX_SUB (scratch[3], scratch_in[1], scratch_in[3]);

    NE10_CPX_SUB (scratch_out[2], scratch[0], scratch[2]);
    NE10_CPX_ADD (scratch_out[0], scratch[0], scratch[2]);

    scratch_out[1].r = scratch[1].r + scratch[3].i;
    scratch_out[1].i = scratch[1].i - scratch[3].r;
    scratch_out[3].r = scratch[1].r - scratch[3].i;
    scratch_out[3].i = scratch[1].i + scratch[3].r;
}

static inline void FFT5_FCU (ne10_fft_cpx_float32_t Fout[5],
        const ne10_fft_cpx_float32_t Fin[5])
{
    ne10_fft_cpx_float32_t scratch[13], scratch_in[5];

    scratch_in[0] = Fin[0];
    scratch_in[1] = Fin[1];
    scratch_in[2] = Fin[2];
    scratch_in[3] = Fin[3];
    scratch_in[4] = Fin[4];

    scratch[0] = scratch_in[0];
    scratch[1] = scratch_in[1];
    scratch[2] = scratch_in[2];
    scratch[3] = scratch_in[3];
    scratch[4] = scratch_in[4];

    NE10_CPX_ADD (scratch[ 7], scratch[1], scratch[4]);
    NE10_CPX_SUB (scratch[10], scratch[1], scratch[4]);
    NE10_CPX_ADD (scratch[ 8], scratch[2], scratch[3]);
    NE10_CPX_SUB (scratch[ 9], scratch[2], scratch[3]);

    scratch_in[0].r += scratch[7].r + scratch[8].r;
    scratch_in[0].i += scratch[7].i + scratch[8].i;

    scratch[5].r = scratch[0].r
        + NE10_S_MUL (scratch[7].r, TW_5A_F32.r)
        + NE10_S_MUL (scratch[8].r, TW_5B_F32.r);
    scratch[5].i = scratch[0].i
        + NE10_S_MUL (scratch[7].i, TW_5A_F32.r)
        + NE10_S_MUL (scratch[8].i, TW_5B_F32.r);

    scratch[6].r = NE10_S_MUL (scratch[10].i, TW_5A_F32.i)
        + NE10_S_MUL (scratch[9].i, TW_5B_F32.i);
    scratch[6].i = -NE10_S_MUL (scratch[10].r, TW_5A_F32.i)
        - NE10_S_MUL (scratch[9].r, TW_5B_F32.i);

    NE10_CPX_SUB (scratch_in[1], scratch[5], scratch[6]);
    NE10_CPX_ADD (scratch_in[4], scratch[5], scratch[6]);

    scratch[11].r = scratch[0].r
        + NE10_S_MUL (scratch[7].r, TW_5B_F32.r)
        + NE10_S_MUL (scratch[8].r, TW_5A_F32.r);
    scratch[11].i = scratch[0].i
        + NE10_S_MUL (scratch[7].i, TW_5B_F32.r)
        + NE10_S_MUL (scratch[8].i, TW_5A_F32.r);

    scratch[12].r = -NE10_S_MUL (scratch[10].i, TW_5B_F32.i)
        + NE10_S_MUL (scratch[9].i, TW_5A_F32.i);
    scratch[12].i = NE10_S_MUL (scratch[10].r, TW_5B_F32.i)
        - NE10_S_MUL (scratch[9].r, TW_5A_F32.i);

    NE10_CPX_ADD (scratch_in[2], scratch[11], scratch[12]);
    NE10_CPX_SUB (scratch_in[3], scratch[11], scratch[12]);

    Fout[0] = scratch_in[0];
    Fout[1] = scratch_in[1];
    Fout[2] = scratch_in[2];
    Fout[3] = scratch_in[3];
    Fout[4] = scratch_in[4];
}

////////////////////////////////////
// Following are butterfly functions
////////////////////////////////////
static inline void ne10_radix_2_butterfly_forward_float32_c (ne10_fft_cpx_float32_t *Fout,
        const ne10_fft_cpx_float32_t *Fin,
        const ne10_fft_cpx_float32_t *twiddles,
        const ne10_int32_t fstride,
        const ne10_int32_t out_step,
        const ne10_int32_t nfft,
        const ne10_int32_t is_first_stage)
{
    ne10_fft_cpx_float32_t scratch_in[2];
    ne10_fft_cpx_float32_t scratch_out[2];

    const ne10_int32_t in_step = nfft / 2;
    ne10_int32_t f_count;
    ne10_int32_t m_count;

    for (f_count = fstride; f_count > 0; f_count--)
    {
        for (m_count = out_step; m_count > 0; m_count--)
        {
            scratch_in[0] = Fin[0 * in_step];
            scratch_in[1] = Fin[1 * in_step];

            if (!is_first_stage)
            {
                ne10_fft_cpx_float32_t scratch_tw[1];
                ne10_fft_cpx_float32_t scratch[2];

                scratch_tw[0] = twiddles[0 * out_step];

                FFT2_MUL_TW (scratch, scratch_in, scratch_tw);

                scratch_in[0] = scratch[0];
                scratch_in[1] = scratch[1];
            }

            FFT2_FCU (scratch_out, scratch_in);

            Fout[0 * out_step] = scratch_out[0];
            Fout[1 * out_step] = scratch_out[1];

            Fin++;

            if (!is_first_stage)
            {
                Fout++;
                twiddles++;
            }
            else
            {
                Fout += 2;
            }
        }
        if (!is_first_stage)
        {
            twiddles -= out_step;
            Fout += (2 - 1) * out_step;
        }
    }
}

static inline void ne10_radix_4_butterfly_forward_float32_c (ne10_fft_cpx_float32_t *Fout,
        const ne10_fft_cpx_float32_t *Fin,
        const ne10_fft_cpx_float32_t *twiddles,
        const ne10_int32_t fstride,
        const ne10_int32_t out_step,
        const ne10_int32_t nfft,
        const ne10_int32_t is_first_stage)
{
    ne10_fft_cpx_float32_t scratch_in[4];
    ne10_fft_cpx_float32_t scratch_out[4];

    const ne10_int32_t in_step = nfft / 4;
    ne10_int32_t f_count;
    ne10_int32_t m_count;

    for (f_count = fstride; f_count > 0; f_count--)
    {
        for (m_count = out_step; m_count > 0; m_count--)
        {
            scratch_in[0] = Fin[0 * in_step];
            scratch_in[1] = Fin[1 * in_step];
            scratch_in[2] = Fin[2 * in_step];
            scratch_in[3] = Fin[3 * in_step];

            if (!is_first_stage)
            {
                ne10_fft_cpx_float32_t scratch_tw[3];
                ne10_fft_cpx_float32_t scratch[4];

                scratch_tw[0] = twiddles[0 * out_step];
                scratch_tw[1] = twiddles[1 * out_step];
                scratch_tw[2] = twiddles[2 * out_step];

                FFT4_MUL_TW (scratch, scratch_in, scratch_tw);

                scratch_in[0] = scratch[0];
                scratch_in[1] = scratch[1];
                scratch_in[2] = scratch[2];
                scratch_in[3] = scratch[3];
            }

            FFT4_FCU (scratch_out, scratch_in);

            Fout[0 * out_step] = scratch_out[0];
            Fout[1 * out_step] = scratch_out[1];
            Fout[2 * out_step] = scratch_out[2];
            Fout[3 * out_step] = scratch_out[3];

            Fin++;

            if (!is_first_stage)
            {
                Fout++;
                twiddles++;
            }
            else
            {
                Fout += 4;
            }
        }
        if (!is_first_stage)
        {
            twiddles -= out_step;
            Fout += (4 - 1) * out_step;
        }
    }
}

static inline void ne10_radix_3_butterfly_forward_float32_c (ne10_fft_cpx_float32_t *Fout,
        const ne10_fft_cpx_float32_t *Fin,
        const ne10_fft_cpx_float32_t *twiddles,
        const ne10_int32_t fstride,
        const ne10_int32_t out_step,
        const ne10_int32_t nfft,
        const ne10_int32_t is_first_stage)
{
    ne10_fft_cpx_float32_t scratch_in[3];
    ne10_fft_cpx_float32_t scratch_out[3];

    const ne10_int32_t in_step = nfft / 3;
    ne10_int32_t f_count;
    ne10_int32_t m_count;

    for (f_count = fstride; f_count > 0; f_count--)
    {
        for (m_count = out_step; m_count > 0; m_count--)
        {
            scratch_in[0] = Fin[0 * in_step];
            scratch_in[1] = Fin[1 * in_step];
            scratch_in[2] = Fin[2 * in_step];

            if (!is_first_stage)
            {
                ne10_fft_cpx_float32_t scratch_tw[2];
                ne10_fft_cpx_float32_t scratch[3];

                scratch_tw[0] = twiddles[0 * out_step];
                scratch_tw[1] = twiddles[1 * out_step];

                FFT3_MUL_TW (scratch, scratch_in, scratch_tw);

                scratch_in[0] = scratch[0];
                scratch_in[1] = scratch[1];
                scratch_in[2] = scratch[2];
            }

            FFT3_FCU (scratch_out, scratch_in);

            Fout[0 * out_step] = scratch_out[0];
            Fout[1 * out_step] = scratch_out[1];
            Fout[2 * out_step] = scratch_out[2];

            Fin++;

            if (!is_first_stage)
            {
                Fout++;
                twiddles++;
            }
            else
            {
                Fout += 3;
            }
        }
        if (!is_first_stage)
        {
            twiddles -= out_step;
            Fout += (3 - 1) * out_step;
        }
    }
}

static inline void ne10_radix_5_butterfly_forward_float32_c (ne10_fft_cpx_float32_t *Fout,
        const ne10_fft_cpx_float32_t *Fin,
        const ne10_fft_cpx_float32_t *twiddles,
        const ne10_int32_t fstride,
        const ne10_int32_t out_step,
        const ne10_int32_t nfft,
        const ne10_int32_t is_first_stage)
{
    ne10_fft_cpx_float32_t scratch_in[5];
    ne10_fft_cpx_float32_t scratch_out[5];

    const ne10_int32_t in_step = nfft / 5;
    ne10_int32_t f_count;
    ne10_int32_t m_count;

    for (f_count = fstride; f_count > 0; f_count--)
    {
        for (m_count = out_step; m_count > 0; m_count--)
        {
            scratch_in[0] = Fin[0 * in_step];
            scratch_in[1] = Fin[1 * in_step];
            scratch_in[2] = Fin[2 * in_step];
            scratch_in[3] = Fin[3 * in_step];
            scratch_in[4] = Fin[4 * in_step];

            if (!is_first_stage)
            {
                ne10_fft_cpx_float32_t scratch_tw[4];
                ne10_fft_cpx_float32_t scratch[5];

                scratch_tw[0] = twiddles[0 * out_step];
                scratch_tw[1] = twiddles[1 * out_step];
                scratch_tw[2] = twiddles[2 * out_step];
                scratch_tw[3] = twiddles[3 * out_step];

                FFT5_MUL_TW (scratch, scratch_in, scratch_tw);

                scratch_in[0] = scratch[0];
                scratch_in[1] = scratch[1];
                scratch_in[2] = scratch[2];
                scratch_in[3] = scratch[3];
                scratch_in[4] = scratch[4];
            }

            FFT5_FCU (scratch_out, scratch_in);

            Fout[0 * out_step] = scratch_out[0];
            Fout[1 * out_step] = scratch_out[1];
            Fout[2 * out_step] = scratch_out[2];
            Fout[3 * out_step] = scratch_out[3];
            Fout[4 * out_step] = scratch_out[4];

            Fin++;

            if (!is_first_stage)
            {
                Fout++;
                twiddles++;
            }
            else
            {
                Fout += 5;
            }
        }
        if (!is_first_stage)
        {
            twiddles -= out_step;
            Fout += (5 - 1) * out_step;
        }
    }
}

static inline void ne10_radix_generic_butterfly_forward_float32_c (ne10_fft_cpx_float32_t *Fout,
        const ne10_fft_cpx_float32_t *Fin,
        const ne10_fft_cpx_float32_t *twiddles,
        const ne10_int32_t radix,
        const ne10_int32_t in_step,
        const ne10_int32_t out_step)
{
    ne10_int32_t q, q1;
    ne10_int32_t f_count = in_step;

    ne10_fft_cpx_float32_t tmp;
    ne10_fft_cpx_float32_t *scratch;
    scratch = (ne10_fft_cpx_float32_t *) NE10_MALLOC (radix *
            sizeof (ne10_fft_cpx_float32_t));

    for (; f_count > 0; f_count--)
    {
        // load
        for (q1 = 0; q1 < radix; q1++)
        {
            scratch[q1] = Fin[in_step * q1];
        } // q1

        // compute Fout[q1 * out_step] from definition
        for (q1 = 0; q1 < radix; q1++)
        {
            ne10_int32_t twidx = 0;
            Fout[q1 * out_step] = scratch[0];
            for (q = 1; q < radix; q++)
            {
                twidx += 1 * q1;
                if (twidx >= radix)
                {
                    twidx -= radix;
                }
                NE10_CPX_MUL_F32 (tmp, scratch[q], twiddles[twidx]);
                NE10_CPX_ADDTO (Fout[q1 * out_step], tmp);
            } // q
        } // q1

        Fout += radix;
        Fin++;
    }

    NE10_FREE (scratch);
}

void ne10_mixed_radix_generic_butterfly_float32_c (ne10_fft_cpx_float32_t *Fout,
        const ne10_fft_cpx_float32_t *Fin,
        const ne10_int32_t *factors,
        const ne10_fft_cpx_float32_t *twiddles,
        ne10_fft_cpx_float32_t *buffer)
{
    PRINT_HIT;
#ifdef NE10_VERBOSE
    {
        int i;
        printf ("factors = \n");
        for (i = 0; i < NE10_MAXFACTORS; i++)
        {
            printf ("[%d] = ( %d, %d )\n", i, factors[2 * i], factors[2 * i + 1]);
        }
    }
#endif

    ne10_int32_t fstride, mstride, radix;
    ne10_int32_t stage_count;
    ne10_int32_t nfft;

    // init fstride, mstride, radix, nfft
    stage_count = factors[0];
    fstride = factors[1];
    mstride = 1;
    radix = factors[stage_count << 1]; // radix of first stage
    nfft = fstride * radix;

    if (stage_count % 2 == 0)
    {
        ne10_swap_ptr (buffer, Fout);
    }

    PRINT_STAGE_INFO;
    PRINT_POINTERS_INFO (Fin, Fout, buffer, twiddles);
    // first stage
    switch (radix)
    {
    case 2:
        ne10_radix_2_butterfly_forward_float32_c (Fout, Fin, NULL, fstride, 1, nfft, 1);
        break;
    case 4:
        ne10_radix_4_butterfly_forward_float32_c (Fout, Fin, NULL, fstride, 1, nfft, 1);
        break;
    case 3:
        ne10_radix_3_butterfly_forward_float32_c (Fout, Fin, NULL, fstride, 1, nfft, 1);
        break;
    case 5:
        ne10_radix_5_butterfly_forward_float32_c (Fout, Fin, NULL, fstride, 1, nfft, 1);
        break;
    default:
        ne10_radix_generic_butterfly_forward_float32_c (Fout, Fin, twiddles, radix,
                fstride, 1);
        break;
    }

    stage_count--;
    if (!stage_count) // finish
    {
        return;
    }

    twiddles += radix;

    // other stges
    while (stage_count > 0)
    {
        ne10_swap_ptr (buffer, Fout);
        mstride *= radix;

        // update radix
        radix = factors[stage_count << 1];
        assert ((radix > 1) && (radix < 6));

        fstride /= radix;
        PRINT_STAGE_INFO;
        switch (radix)
        {
        case 2:
            ne10_radix_2_butterfly_forward_float32_c (Fout, buffer, twiddles, fstride,
                    mstride, nfft, 0);
            break;
        case 3:
            ne10_radix_3_butterfly_forward_float32_c (Fout, buffer, twiddles, fstride,
                    mstride, nfft, 0);
            break;
        case 4:
            ne10_radix_4_butterfly_forward_float32_c (Fout, buffer, twiddles, fstride,
                    mstride, nfft, 0);
            break;
        case 5:
            ne10_radix_5_butterfly_forward_float32_c (Fout, buffer, twiddles, fstride,
                    mstride, nfft, 0);
            break;
        } // switch (radix)

        twiddles += mstride * (radix - 1);

        stage_count--;
    } // while (stage_count)
}

void ne10_mixed_radix_generic_butterfly_inverse_float32_c (ne10_fft_cpx_float32_t *Fout,
        const ne10_fft_cpx_float32_t *Fin,
        const ne10_int32_t *factors,
        const ne10_fft_cpx_float32_t *twiddles,
        ne10_fft_cpx_float32_t *buffer)
{

}
