/*
 *  Copyright 2015-16 ARM Limited and Contributors.
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
 * NE10 Library : dsp/NE10_fft_generic_int32.cpp
 */

#include "NE10_types.h"
#include "NE10_macros.h"
#include "NE10_fft.h"
#include "NE10_fft_generic_int32.h"

/**
 * @brief Generic butterfly function for 32-bit fixed point.
 * @tparam RADIX            Radix of this stage. One among {2, 3, 4, 5 }
 * @tparam is_first_stage   Whether this stags is the first. If it is, multiplication
 * with twiddles is optimizied because all twiddles are \f$ 1 \f$.
 * @tparam is_inverse       Whether it is the inverse (backward) FFT.
 * @tparam is_scaled        Whether output of this stage is scaled. For fixed-point,
 * output is scaled by RADIX during each stage.
 * @param[out] Fout         Pointing to output buffer.
 * @param[in]  Fin          Pointing to input buffer.
 * @param[in]  twiddles     Pointint to twiddles buffer. If is_first_stage is set true,
 * twiddles will be ignored.
 * @param[in]  fstride      Number of butterfly groups.
 * @param[in]  out_step     Step between output data.
 * @param[in]  nfft         Size of the input array.
 */
template<int RADIX, bool is_first_stage, bool is_inverse, bool is_scaled>
inline void ne10_radix_butterfly_int32_c (ne10_fft_cpx_int32_t *Fout,
        const ne10_fft_cpx_int32_t *Fin,
        const ne10_fft_cpx_int32_t *twiddles,
        const ne10_int32_t fstride,
        const ne10_int32_t out_step,
        const ne10_int32_t nfft)
{
    const ne10_int32_t in_step = nfft / RADIX;
    ne10_int32_t f_count;
    ne10_int32_t m_count;

    for (f_count = fstride; f_count > 0; f_count--)
    {
        for (m_count = out_step; m_count > 0; m_count--)
        {
            ne10_fft_cpx_int32_t scratch_in[RADIX];
            ne10_fft_cpx_int32_t scratch_out[RADIX];

            // Load from input buffer.
            NE10_LOAD_BY_STEP <RADIX> (scratch_in, Fin, in_step);

            if (is_inverse)
            {
                // Conjugate all elements in scratch_in.
                NE10_CONJ<RADIX> (scratch_in);
            }

            if (is_scaled)
            {
                // All elements in scratch_in are divided by radix of this stage.
                NE10_SCALED<RADIX> (scratch_in, RADIX);
            }

            if (!is_first_stage)
            {
                // Multiply twiddles for all stages but the first one.
                ne10_fft_cpx_int32_t scratch_tw[RADIX - 1];
                ne10_fft_cpx_int32_t scratch[RADIX];

                // Load twiddles from twiddles buffer.
                NE10_LOAD_BY_STEP<RADIX - 1> (scratch_tw, twiddles, out_step);

                FFT_MUL_TW<RADIX> (scratch, scratch_in, scratch_tw);

                // Copy from temp buff scratch to scratch_in.
                NE10_LOAD_BY_STEP<RADIX> (scratch_in, scratch, 1);
            }

            // Radix -2, -3, -4 or -5 butterfly
            // From scratch_in to scratch_out.
            FFT_FCU<RADIX> (scratch_out, scratch_in);

            if (is_inverse)
            {
                // Conjugate all elements in scratch_out.
                NE10_CONJ<RADIX> (scratch_out);
            }

            // Store to output buffer.
            NE10_STORE_BY_STEP<RADIX> (Fout, scratch_out, out_step);

            // Update input, output and twiddles pointers.
            Fin++;
            if (!is_first_stage)
            {
                Fout++;
                twiddles++;
            }
            else
            {
                Fout += RADIX;
            }
        }
        if (!is_first_stage)
        {
            // Roll back twiddles.
            twiddles -= out_step;
            // Next output groups.
            Fout += (RADIX - 1) * out_step;
        }
    }
}

/**
 * @brief Generic butterfly function for 32-bit fixed point.
 * @tparam is_inverse       Whether it is the inverse (backward) FFT.
 * @tparam is_scaled        Whether output of this stage is scaled. For fixed-point,
 * output is scaled by RADIX during each stage.
 * @param[out] Fout         Pointing to output buffer.
 * @param[in]  Fin          Pointing to input buffer.
 * @param[in]  twiddles     Pointint to twiddles buffer.
 * @param[in]  radix        Radix of this stage.
 * @param[in]  in_step      Step between input data.
 * @param[in]  out_step     Step between output data.
 */
template<bool is_inverse, bool is_scaled>
static inline void ne10_radix_generic_butterfly_int32_c (ne10_fft_cpx_int32_t *Fout,
        const ne10_fft_cpx_int32_t *Fin,
        const ne10_fft_cpx_int32_t *twiddles,
        const ne10_int32_t radix,
        const ne10_int32_t in_step,
        const ne10_int32_t out_step)
{
    ne10_int32_t q, q1;
    ne10_int32_t f_count = in_step;

    ne10_fft_cpx_int32_t tmp;
    ne10_fft_cpx_int32_t *scratch;
    scratch = (ne10_fft_cpx_int32_t *) NE10_MALLOC (radix *
            sizeof (ne10_fft_cpx_int32_t));

    for (; f_count > 0; f_count--)
    {
        // load
        for (q1 = 0; q1 < radix; q1++)
        {
            scratch[q1] = Fin[in_step * q1];
            if (is_inverse)
            {
                scratch[q1].i = -scratch[q1].i;
            }
            if (is_scaled)
            {
                NE10_F2I32_FIXDIV (scratch[q1], radix);
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

/**
 * @brief Generic FFT function for 32-bit fixed point.
 * @tparam is_inverse       Whether it is the inverse (backward) FFT.
 * @tparam is_scaled        Whether output is scaled by size of FFT.
 * @param[out] Fout         Pointing to output buffer.
 * @param[in]  Fin          Pointing to input buffer.
 * @param[in]  factors      Pointing to factors buffer.
 * @param[in]  twiddles     Pointing to twiddles buffer.
 * @param[in]  buffer       Pointing to temp buffer so that Fin is not polluted.
 */
template<bool is_inverse, bool is_scaled>
inline void ne10_mixed_radix_generic_butterfly_int32_impl_c (ne10_fft_cpx_int32_t *Fout,
        const ne10_fft_cpx_int32_t *Fin,
        const ne10_int32_t *factors,
        const ne10_fft_cpx_int32_t *twiddles,
        ne10_fft_cpx_int32_t *buffer)
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

    // first stage
    switch (radix)
    {
    case 2:
        ne10_radix_butterfly_int32_c<2, true, is_inverse, is_scaled> (Fout, Fin,
                NULL, // Twiddles are not used for first stage.
                fstride, 1, nfft);
        break;
    case 4:
        ne10_radix_butterfly_int32_c<4, true, is_inverse, is_scaled> (Fout, Fin,
                NULL, // Same as above.
                fstride, 1, nfft);
        break;
    case 3:
        ne10_radix_butterfly_int32_c<3, true, is_inverse, is_scaled> (Fout, Fin,
                NULL, // Same as above.
                fstride, 1, nfft);
        break;
    case 5:
        ne10_radix_butterfly_int32_c<5, true, is_inverse, is_scaled> (Fout, Fin,
                NULL, // Same as above.
                fstride, 1, nfft);
        break;
    default:
        ne10_radix_generic_butterfly_int32_c<is_inverse, is_scaled> (Fout, Fin,
                twiddles, // Twiddles for butterfly.
                radix, fstride, 1);
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

    // other stages
    while (stage_count > 0)
    {
        ne10_swap_ptr (buffer, Fout);
        mstride *= radix;

        // update radix
        radix = factors[stage_count << 1];
        assert ((radix > 1) && (radix < 6));

        fstride /= radix;
        switch (radix)
        {
        case 2:
            ne10_radix_butterfly_int32_c<2, false, is_inverse, is_scaled> (Fout,
                    buffer, twiddles, fstride, mstride, nfft);
            break;
        case 3:
            ne10_radix_butterfly_int32_c<3, false, is_inverse, is_scaled> (Fout,
                    buffer, twiddles, fstride, mstride, nfft);
            break;
        case 4:
            ne10_radix_butterfly_int32_c<4, false, is_inverse, is_scaled> (Fout,
                    buffer, twiddles, fstride, mstride, nfft);
            break;
        case 5:
            ne10_radix_butterfly_int32_c<5, false, is_inverse, is_scaled> (Fout,
                    buffer, twiddles, fstride, mstride, nfft);
            break;
        } // switch (radix)

        twiddles += mstride * (radix - 1);
        stage_count--;
    } // while (stage_count)
}

/**
 * @brief Generic (forward) FFT function for 32-bit fixed point.
 * @param[out] Fout         Pointing to output buffer.
 * @param[in]  Fin          Pointing to input buffer.
 * @param[in]  factors      Pointing to factors buffer.
 * @param[in]  twiddles     Pointing to twiddles buffer.
 * @param[in]  buffer       Pointing to temp buffer so that Fin is not polluted.
 * @param[in]  is_scaled    Whether output is scaled by size of FFT.
 */
void ne10_mixed_radix_generic_butterfly_int32_c (ne10_fft_cpx_int32_t *Fout,
        const ne10_fft_cpx_int32_t *Fin,
        const ne10_int32_t *factors,
        const ne10_fft_cpx_int32_t *twiddles,
        ne10_fft_cpx_int32_t *buffer,
        const ne10_int32_t is_scaled)
{
    const bool is_inverse = false;
    if (is_scaled)
    {
        const bool is_scaled_flag = true;
        ne10_mixed_radix_generic_butterfly_int32_impl_c<is_inverse,
            is_scaled_flag> (Fout, Fin, factors, twiddles, buffer);
    }
    else
    {
        const bool is_scaled_flag = false;
        ne10_mixed_radix_generic_butterfly_int32_impl_c<is_inverse,
            is_scaled_flag> (Fout, Fin, factors, twiddles, buffer);
    }
}

/**
 * @brief Generic IFFT function for 32-bit fixed point.
 * @param[out] Fout         Pointing to output buffer.
 * @param[in]  Fin          Pointing to input buffer.
 * @param[in]  factors      Pointing to factors buffer.
 * @param[in]  twiddles     Pointing to twiddles buffer.
 * @param[in]  buffer       Pointing to temp buffer so that Fin is not polluted.
 * @param[in]  is_scaled    Whether output is scaled by size of FFT.
 */
void ne10_mixed_radix_generic_butterfly_inverse_int32_c (
        ne10_fft_cpx_int32_t *Fout,
        const ne10_fft_cpx_int32_t *Fin,
        const ne10_int32_t *factors,
        const ne10_fft_cpx_int32_t *twiddles,
        ne10_fft_cpx_int32_t *buffer,
        const ne10_int32_t is_scaled)
{
    const bool is_inverse = true;
    if (is_scaled)
    {
        const bool is_scaled_flag = true;
        ne10_mixed_radix_generic_butterfly_int32_impl_c<is_inverse,
            is_scaled_flag> (Fout, Fin, factors, twiddles, buffer);
    }
    else
    {
        const bool is_scaled_flag = false;
        ne10_mixed_radix_generic_butterfly_int32_impl_c<is_inverse,
            is_scaled_flag> (Fout, Fin, factors, twiddles, buffer);
    }
}
