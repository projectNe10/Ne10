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
#include "NE10_fft_generic_float32.h"

////////////////////////////////////
// Following are butterfly functions
////////////////////////////////////
static inline void ne10_radix_2_butterfly_float32_c (ne10_fft_cpx_float32_t *Fout,
        const ne10_fft_cpx_float32_t *Fin,
        const ne10_fft_cpx_float32_t *twiddles,
        const ne10_int32_t fstride,
        const ne10_int32_t out_step,
        const ne10_int32_t nfft,
        const ne10_int32_t is_first_stage,
        const ne10_int32_t is_inverse)
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

            if (is_inverse)
            {
                scratch_in[0].i = -scratch_in[0].i;
                scratch_in[1].i = -scratch_in[1].i;
            }

#ifdef NE10_DSP_CFFT_SCALING
            if (is_inverse && is_first_stage)
            {
                const ne10_float32_t one_by_nfft = 1.0 / nfft;

                scratch_in[0].r *= one_by_nfft;
                scratch_in[0].i *= one_by_nfft;
                scratch_in[1].r *= one_by_nfft;
                scratch_in[1].i *= one_by_nfft;
            }
#endif

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

            if (is_inverse)
            {
                scratch_out[0].i = -scratch_out[0].i;
                scratch_out[1].i = -scratch_out[1].i;
            }

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

static inline void ne10_radix_4_butterfly_float32_c (ne10_fft_cpx_float32_t *Fout,
        const ne10_fft_cpx_float32_t *Fin,
        const ne10_fft_cpx_float32_t *twiddles,
        const ne10_int32_t fstride,
        const ne10_int32_t out_step,
        const ne10_int32_t nfft,
        const ne10_int32_t is_first_stage,
        const ne10_int32_t is_inverse)
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

            if (is_inverse)
            {
                scratch_in[0].i = -scratch_in[0].i;
                scratch_in[1].i = -scratch_in[1].i;
                scratch_in[2].i = -scratch_in[2].i;
                scratch_in[3].i = -scratch_in[3].i;
            }

#ifdef NE10_DSP_CFFT_SCALING
            if (is_inverse && is_first_stage)
            {
                const ne10_float32_t one_by_nfft = 1.0 / nfft;

                scratch_in[0].r *= one_by_nfft;
                scratch_in[0].i *= one_by_nfft;
                scratch_in[1].r *= one_by_nfft;
                scratch_in[1].i *= one_by_nfft;
                scratch_in[2].r *= one_by_nfft;
                scratch_in[2].i *= one_by_nfft;
                scratch_in[3].r *= one_by_nfft;
                scratch_in[3].i *= one_by_nfft;
            }
#endif

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

            if (is_inverse)
            {
                scratch_out[0].i = -scratch_out[0].i;
                scratch_out[1].i = -scratch_out[1].i;
                scratch_out[2].i = -scratch_out[2].i;
                scratch_out[3].i = -scratch_out[3].i;
            }

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

static inline void ne10_radix_8_butterfly_float32_c (ne10_fft_cpx_float32_t *Fout,
        const ne10_fft_cpx_float32_t *Fin,
        const ne10_fft_cpx_float32_t *twiddles,
        const ne10_int32_t fstride,
        const ne10_int32_t out_step,
        const ne10_int32_t nfft,
        const ne10_int32_t is_first_stage,
        const ne10_int32_t is_inverse)
{
    assert (is_first_stage == 1);

    ne10_fft_cpx_float32_t scratch_in[8];
    ne10_fft_cpx_float32_t scratch_out[8];

    const ne10_int32_t in_step = nfft / 8;
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
            scratch_in[5] = Fin[5 * in_step];
            scratch_in[6] = Fin[6 * in_step];
            scratch_in[7] = Fin[7 * in_step];

            if (is_inverse)
            {
                scratch_in[0].i = -scratch_in[0].i;
                scratch_in[1].i = -scratch_in[1].i;
                scratch_in[2].i = -scratch_in[2].i;
                scratch_in[3].i = -scratch_in[3].i;
                scratch_in[4].i = -scratch_in[4].i;
                scratch_in[5].i = -scratch_in[5].i;
                scratch_in[6].i = -scratch_in[6].i;
                scratch_in[7].i = -scratch_in[7].i;
            }

#ifdef NE10_DSP_CFFT_SCALING
            if (is_inverse)
            {
                const ne10_float32_t one_by_nfft = 1.0 / nfft;

                scratch_in[0].r *= one_by_nfft;
                scratch_in[0].i *= one_by_nfft;
                scratch_in[1].r *= one_by_nfft;
                scratch_in[1].i *= one_by_nfft;
                scratch_in[2].r *= one_by_nfft;
                scratch_in[2].i *= one_by_nfft;
                scratch_in[3].r *= one_by_nfft;
                scratch_in[3].i *= one_by_nfft;
                scratch_in[4].r *= one_by_nfft;
                scratch_in[4].i *= one_by_nfft;
                scratch_in[5].r *= one_by_nfft;
                scratch_in[5].i *= one_by_nfft;
                scratch_in[6].r *= one_by_nfft;
                scratch_in[6].i *= one_by_nfft;
                scratch_in[7].r *= one_by_nfft;
                scratch_in[7].i *= one_by_nfft;
            }
#endif

            FFT8_FCU (scratch_out, scratch_in);

            if (is_inverse)
            {
                scratch_out[0].i = -scratch_out[0].i;
                scratch_out[1].i = -scratch_out[1].i;
                scratch_out[2].i = -scratch_out[2].i;
                scratch_out[3].i = -scratch_out[3].i;
                scratch_out[4].i = -scratch_out[4].i;
                scratch_out[5].i = -scratch_out[5].i;
                scratch_out[6].i = -scratch_out[6].i;
                scratch_out[7].i = -scratch_out[7].i;
            }

            Fout[0*out_step] = scratch_out[0];
            Fout[1*out_step] = scratch_out[1];
            Fout[2*out_step] = scratch_out[2];
            Fout[3*out_step] = scratch_out[3];
            Fout[4*out_step] = scratch_out[4];
            Fout[5*out_step] = scratch_out[5];
            Fout[6*out_step] = scratch_out[6];
            Fout[7*out_step] = scratch_out[7];

            Fin++;
            Fout += 8;
        }
    }
}

static inline void ne10_radix_3_butterfly_float32_c (ne10_fft_cpx_float32_t *Fout,
        const ne10_fft_cpx_float32_t *Fin,
        const ne10_fft_cpx_float32_t *twiddles,
        const ne10_int32_t fstride,
        const ne10_int32_t out_step,
        const ne10_int32_t nfft,
        const ne10_int32_t is_first_stage,
        const ne10_int32_t is_inverse)
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

            if (is_inverse)
            {
                scratch_in[0].i = -scratch_in[0].i;
                scratch_in[1].i = -scratch_in[1].i;
                scratch_in[2].i = -scratch_in[2].i;
            }

#ifdef NE10_DSP_CFFT_SCALING
            if (is_first_stage && is_inverse)
            {
                const ne10_float32_t one_by_nfft = 1.0 / nfft;

                scratch_in[0].r *= one_by_nfft;
                scratch_in[0].i *= one_by_nfft;
                scratch_in[1].r *= one_by_nfft;
                scratch_in[1].i *= one_by_nfft;
                scratch_in[2].r *= one_by_nfft;
                scratch_in[2].i *= one_by_nfft;
            }
#endif

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

            if (is_inverse)
            {
                scratch_out[0].i = -scratch_out[0].i;
                scratch_out[1].i = -scratch_out[1].i;
                scratch_out[2].i = -scratch_out[2].i;
            }

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

static inline void ne10_radix_5_butterfly_float32_c (ne10_fft_cpx_float32_t *Fout,
        const ne10_fft_cpx_float32_t *Fin,
        const ne10_fft_cpx_float32_t *twiddles,
        const ne10_int32_t fstride,
        const ne10_int32_t out_step,
        const ne10_int32_t nfft,
        const ne10_int32_t is_first_stage,
        const ne10_int32_t is_inverse)
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

            if (is_inverse)
            {
                scratch_in[0].i = -scratch_in[0].i;
                scratch_in[1].i = -scratch_in[1].i;
                scratch_in[2].i = -scratch_in[2].i;
                scratch_in[3].i = -scratch_in[3].i;
                scratch_in[4].i = -scratch_in[4].i;
            }

#ifdef NE10_DSP_CFFT_SCALING
            if (is_first_stage && is_inverse)
            {
                const ne10_float32_t one_by_nfft = 1.0 / nfft;

                scratch_in[0].r *= one_by_nfft;
                scratch_in[0].i *= one_by_nfft;
                scratch_in[1].r *= one_by_nfft;
                scratch_in[1].i *= one_by_nfft;
                scratch_in[2].r *= one_by_nfft;
                scratch_in[2].i *= one_by_nfft;
                scratch_in[3].r *= one_by_nfft;
                scratch_in[3].i *= one_by_nfft;
                scratch_in[4].r *= one_by_nfft;
                scratch_in[4].i *= one_by_nfft;
            }
#endif

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

            if (is_inverse)
            {
                scratch_out[0].i = -scratch_out[0].i;
                scratch_out[1].i = -scratch_out[1].i;
                scratch_out[2].i = -scratch_out[2].i;
                scratch_out[3].i = -scratch_out[3].i;
                scratch_out[4].i = -scratch_out[4].i;
            }

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

static inline void ne10_radix_generic_butterfly_float32_c (ne10_fft_cpx_float32_t *Fout,
        const ne10_fft_cpx_float32_t *Fin,
        const ne10_fft_cpx_float32_t *twiddles,
        const ne10_int32_t radix,
        const ne10_int32_t in_step,
        const ne10_int32_t out_step,
        const ne10_int32_t is_inverse)
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
            if (is_inverse)
            {
                scratch[q1].i = -scratch[q1].i;
#ifdef NE10_DSP_CFFT_SCALING
                const ne10_float32_t one_by_nfft = 1.0 / (radix * in_step);
                scratch[q1].r *= one_by_nfft;
                scratch[q1].i *= one_by_nfft;
#endif
            }
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
            if (is_inverse)
            {
                Fout[q1 * out_step].i = -Fout[q1 * out_step].i;
            }
        } // q1

        Fout += radix;
        Fin++;
    }

    NE10_FREE (scratch);
}

static inline void ne10_mixed_radix_generic_butterfly_float32_impl_c (ne10_fft_cpx_float32_t *Fout,
        const ne10_fft_cpx_float32_t *Fin,
        const ne10_int32_t *factors,
        const ne10_fft_cpx_float32_t *twiddles,
        ne10_fft_cpx_float32_t *buffer,
        const ne10_int32_t is_inverse)
{
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
        ne10_radix_2_butterfly_float32_c (Fout, Fin, NULL, fstride, 1, nfft, 1, is_inverse);
        break;
    case 4:
        ne10_radix_4_butterfly_float32_c (Fout, Fin, NULL, fstride, 1, nfft, 1, is_inverse);
        break;
    case 3:
        ne10_radix_3_butterfly_float32_c (Fout, Fin, NULL, fstride, 1, nfft, 1, is_inverse);
        break;
    case 5:
        ne10_radix_5_butterfly_float32_c (Fout, Fin, NULL, fstride, 1, nfft, 1, is_inverse);
        break;
        break;
    case 8:
        ne10_radix_8_butterfly_float32_c (Fout, Fin, NULL, fstride, 1, nfft, 1, is_inverse);
    default:
        ne10_radix_generic_butterfly_float32_c (Fout, Fin, twiddles, radix,
                fstride, 1, is_inverse);
        break;
    }

    stage_count--;
    if (!stage_count) // finish
    {
        return;
    }

    if (radix % 2)
    {
        twiddles += radix;
    }

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
            ne10_radix_2_butterfly_float32_c (Fout, buffer, twiddles, fstride,
                    mstride, nfft, 0, is_inverse);
            break;
        case 3:
            ne10_radix_3_butterfly_float32_c (Fout, buffer, twiddles, fstride,
                    mstride, nfft, 0, is_inverse);
            break;
        case 4:
            ne10_radix_4_butterfly_float32_c (Fout, buffer, twiddles, fstride,
                    mstride, nfft, 0, is_inverse);
            break;
        case 5:
            ne10_radix_5_butterfly_float32_c (Fout, buffer, twiddles, fstride,
                    mstride, nfft, 0, is_inverse);
            break;
        } // switch (radix)

        twiddles += mstride * (radix - 1);

        stage_count--;
    } // while (stage_count)
}

void ne10_mixed_radix_generic_butterfly_float32_c (ne10_fft_cpx_float32_t *Fout,
        const ne10_fft_cpx_float32_t *Fin,
        const ne10_int32_t *factors,
        const ne10_fft_cpx_float32_t *twiddles,
        ne10_fft_cpx_float32_t *buffer)
{
    ne10_mixed_radix_generic_butterfly_float32_impl_c (Fout, Fin, factors, twiddles, buffer, 0);
}

void ne10_mixed_radix_generic_butterfly_inverse_float32_c (ne10_fft_cpx_float32_t *Fout,
        const ne10_fft_cpx_float32_t *Fin,
        const ne10_int32_t *factors,
        const ne10_fft_cpx_float32_t *twiddles,
        ne10_fft_cpx_float32_t *buffer)
{
    ne10_mixed_radix_generic_butterfly_float32_impl_c (Fout, Fin, factors, twiddles, buffer, 1);
}
