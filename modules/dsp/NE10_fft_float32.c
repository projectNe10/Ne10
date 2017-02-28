/*
 *  Copyright 2013-16 ARM Limited and Contributors.
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
 *  DISCLAIMED. IN NO EVENT SHALL ARM LIMITED AND CONTRIBUTORS BE LIABLE FOR ANY
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

/*
 * This function calculates the FFT for power-of-two input sizes using an ordered, mixed
 * radix-4/8 DIT algorithm.
 *
 * At each stage, 'fstride' holds the number of butterfly "sections" to be processed,
 * while 'mstride' holds the number of butterflies to be performed in each section. After
 * radix-4 butterflies, for example, we quarter the the 'fstride' (number of sections) and
 * quadruple the 'mstride' (size of each section) for the next stage. The exception to
 * this is the first stage, in which 'mstride' does not apply (as it is implicitly 1).
 *
 * The algorithm first performs either a radix-8 or radix-4 pass, depending on the size
 * of the input/output (as dictated by the 'factors' array), and then continually applies
 * radix-4 butterflies to completion. The order in which results are stored after each
 * stage allows stages to load and store elements contiguously between iterations, and for
 * the final output order to be correct.
 */
static void ne10_mixed_radix_butterfly_float32_c (ne10_fft_cpx_float32_t *out,
        ne10_fft_cpx_float32_t *in,
        ne10_int32_t *factors,
        ne10_fft_cpx_float32_t *twiddles,
        ne10_fft_cpx_float32_t *buffer)
{
    ne10_int32_t stage_count = factors[0];
    ne10_int32_t fstride = factors[1];
    ne10_int32_t mstride = factors[(stage_count << 1) - 1];
    ne10_int32_t first_radix = factors[stage_count << 1];
    ne10_int32_t step, f_count, m_count;
    ne10_fft_cpx_float32_t *src = in;
    ne10_fft_cpx_float32_t *dst = out;
    ne10_fft_cpx_float32_t *out_final = out;
    ne10_fft_cpx_float32_t *tw, *tmp;
    const ne10_float32_t TW_81 = 0.70710678;

    ne10_fft_cpx_float32_t scratch[16];
    ne10_fft_cpx_float32_t scratch_in[8];
    ne10_fft_cpx_float32_t scratch_out[8];
    ne10_fft_cpx_float32_t scratch_tw[6];

    // The first stage (using hardcoded twiddles)
    if (first_radix == 8) // For our factoring this means nfft is of form 2^{odd}
    {
        for (f_count = 0; f_count < fstride; f_count++)
        {
            dst = &out[f_count * 8];

            // X[0] +/- X[4N/8]
            scratch_in[0].r = src[0].r + src[fstride * 4].r;
            scratch_in[0].i = src[0].i + src[fstride * 4].i;
            scratch_in[1].r = src[0].r - src[fstride * 4].r;
            scratch_in[1].i = src[0].i - src[fstride * 4].i;

            // X[N/8] +/- X[5N/8]
            scratch_in[2].r = src[fstride].r + src[fstride * 5].r;
            scratch_in[2].i = src[fstride].i + src[fstride * 5].i;
            scratch_in[3].r = src[fstride].r - src[fstride * 5].r;
            scratch_in[3].i = src[fstride].i - src[fstride * 5].i;

            // X[2N/8] +/- X[6N/8]
            scratch_in[4].r = src[fstride * 2].r + src[fstride * 6].r;
            scratch_in[4].i = src[fstride * 2].i + src[fstride * 6].i;
            scratch_in[5].r = src[fstride * 2].r - src[fstride * 6].r;
            scratch_in[5].i = src[fstride * 2].i - src[fstride * 6].i;

            // X[3N/8] +/- X[7N/8]
            scratch_in[6].r = src[fstride * 3].r + src[fstride * 7].r;
            scratch_in[6].i = src[fstride * 3].i + src[fstride * 7].i;
            scratch_in[7].r = src[fstride * 3].r - src[fstride * 7].r;
            scratch_in[7].i = src[fstride * 3].i - src[fstride * 7].i;


            scratch[0] = scratch_in[0]; // X[0] + X[4N/8]
            scratch[1] = scratch_in[1]; // X[0] - X[4N/8]
            scratch[2] = scratch_in[2]; // X[N/8] + X[5N/8]
            scratch[4] = scratch_in[4]; // X[2N/8] + X[6N/8]
            scratch[6] = scratch_in[6]; // X[3N/8] + X[7N/8]

            // (X[2N/8] - X[6N/8]) * -i
            scratch[5].r = scratch_in[5].i;
            scratch[5].i = -scratch_in[5].r;

            // (X[N/8] - X[5N/8]) * (TW_81 - TW_81i)
            scratch[3].r = (scratch_in[3].r + scratch_in[3].i) * TW_81;
            scratch[3].i = (scratch_in[3].i - scratch_in[3].r) * TW_81;

            // (X[3N/8] - X[7N/8]) * (TW_81 + TW_81i)
            scratch[7].r = (scratch_in[7].r - scratch_in[7].i) * TW_81;
            scratch[7].i = (scratch_in[7].i + scratch_in[7].r) * TW_81;

            // Combine the (X[0] +/- X[4N/8]) and (X[2N/8] +/- X[6N/8]) components
            scratch[8].r  = scratch[0].r + scratch[4].r;
            scratch[8].i  = scratch[0].i + scratch[4].i;
            scratch[9].r  = scratch[1].r + scratch[5].r;
            scratch[9].i  = scratch[1].i + scratch[5].i;
            scratch[10].r = scratch[0].r - scratch[4].r;
            scratch[10].i = scratch[0].i - scratch[4].i;
            scratch[11].r = scratch[1].r - scratch[5].r;
            scratch[11].i = scratch[1].i - scratch[5].i;

            // Combine the (X[N/8] +/- X[5N/8]) and (X[3N/8] +/- X[7N/8]) components
            scratch[12].r = scratch[2].r + scratch[6].r;
            scratch[12].i = scratch[2].i + scratch[6].i;
            scratch[13].r = scratch[3].r - scratch[7].r;
            scratch[13].i = scratch[3].i - scratch[7].i;
            scratch[14].r = scratch[2].r - scratch[6].r;
            scratch[14].i = scratch[2].i - scratch[6].i;
            scratch[15].r = scratch[3].r + scratch[7].r;
            scratch[15].i = scratch[3].i + scratch[7].i;

            // Combine the two combined components (for the full radix-8 butterfly)
            scratch_out[0].r = scratch[8].r  + scratch[12].r;
            scratch_out[0].i = scratch[8].i  + scratch[12].i;
            scratch_out[1].r = scratch[9].r  + scratch[13].r;
            scratch_out[1].i = scratch[9].i  + scratch[13].i;
            scratch_out[2].r = scratch[10].r + scratch[14].i;
            scratch_out[2].i = scratch[10].i - scratch[14].r;
            scratch_out[3].r = scratch[11].r + scratch[15].i;
            scratch_out[3].i = scratch[11].i - scratch[15].r;
            scratch_out[4].r = scratch[8].r  - scratch[12].r;
            scratch_out[4].i = scratch[8].i  - scratch[12].i;
            scratch_out[5].r = scratch[9].r  - scratch[13].r;
            scratch_out[5].i = scratch[9].i  - scratch[13].i;
            scratch_out[6].r = scratch[10].r - scratch[14].i;
            scratch_out[6].i = scratch[10].i + scratch[14].r;
            scratch_out[7].r = scratch[11].r - scratch[15].i;
            scratch_out[7].i = scratch[11].i + scratch[15].r;

            // Store the results
            dst[0] = scratch_out[0];
            dst[1] = scratch_out[1];
            dst[2] = scratch_out[2];
            dst[3] = scratch_out[3];
            dst[4] = scratch_out[4];
            dst[5] = scratch_out[5];
            dst[6] = scratch_out[6];
            dst[7] = scratch_out[7];

            src++;
        } // f_count

        // Update variables for the next stages
        step = fstride << 1; // For C2C, 1/4 of input size (fstride is nfft/8)
        stage_count--;
        fstride /= 4;
    }
    else // first_radix == 4, for our factoring this means nfft is of form 2^{even}
    {
        for (f_count = fstride; f_count; f_count--)
        {
            // Load the four input values for a radix-4 butterfly
            scratch_in[0] = src[0];           // X[0]
            scratch_in[1] = src[fstride * 1]; // X[N/4]
            scratch_in[2] = src[fstride * 2]; // X[2N/4]
            scratch_in[3] = src[fstride * 3]; // X[3N/4]

            // X[0] +/- X[2N/4]
            scratch[0].r = scratch_in[0].r + scratch_in[2].r;
            scratch[0].i = scratch_in[0].i + scratch_in[2].i;
            scratch[1].r = scratch_in[0].r - scratch_in[2].r;
            scratch[1].i = scratch_in[0].i - scratch_in[2].i;

            // X[N/4] +/- X[3N/4]
            scratch[2].r = scratch_in[1].r + scratch_in[3].r;
            scratch[2].i = scratch_in[1].i + scratch_in[3].i;
            scratch[3].r = scratch_in[1].r - scratch_in[3].r;
            scratch[3].i = scratch_in[1].i - scratch_in[3].i;

            // Combine the (X[0] +/- X[2N/4]) and (X[N/4] +/- X[3N/4]) components (for
            // the full radix-4 butterfly)
            scratch_out[0].r = scratch[0].r + scratch[2].r;
            scratch_out[0].i = scratch[0].i + scratch[2].i;
            scratch_out[1].r = scratch[1].r + scratch[3].i; // scratch[1] - i*scratch[3]
            scratch_out[1].i = scratch[1].i - scratch[3].r;
            scratch_out[2].r = scratch[0].r - scratch[2].r;
            scratch_out[2].i = scratch[0].i - scratch[2].i;
            scratch_out[3].r = scratch[1].r - scratch[3].i; // scratch[1] + i*scratch[3]
            scratch_out[3].i = scratch[1].i + scratch[3].r;

            // Store the results
            *dst++ = scratch_out[0];
            *dst++ = scratch_out[1];
            *dst++ = scratch_out[2];
            *dst++ = scratch_out[3];

            src++;
        } // f_count

        // Update variables for the next stages
        step = fstride; // For C2C, 1/4 of input size (fstride is nfft/4)
        stage_count--;
        fstride /= 4;
    }

    // The next stage should read the output of the first stage as input
    in = out;
    out = buffer;

    // Middle stages (after the first, excluding the last)
    for (; stage_count > 1; stage_count--)
    {
        src = in;
        for (f_count = 0; f_count < fstride; f_count++)
        {
            dst = &out[f_count * (mstride * 4)];
            tw = twiddles; // Reset the twiddle pointer for the next section
            for (m_count = mstride; m_count; m_count--)
            {
                // Load the three twiddles and four input values for a radix-4 butterfly
                scratch_tw[0] = tw[0];           // w^{k}
                scratch_tw[1] = tw[mstride * 1]; // w^{2k}
                scratch_tw[2] = tw[mstride * 2]; // w^{3k}
                scratch_in[0] = src[0];
                scratch_in[1] = src[step * 1];
                scratch_in[2] = src[step * 2];
                scratch_in[3] = src[step * 3];

                // Multiply input elements by their associated twiddles
                scratch[0] = scratch_in[0];
                scratch[1].r = scratch_in[1].r * scratch_tw[0].r - scratch_in[1].i * scratch_tw[0].i;
                scratch[1].i = scratch_in[1].i * scratch_tw[0].r + scratch_in[1].r * scratch_tw[0].i;
                scratch[2].r = scratch_in[2].r * scratch_tw[1].r - scratch_in[2].i * scratch_tw[1].i;
                scratch[2].i = scratch_in[2].i * scratch_tw[1].r + scratch_in[2].r * scratch_tw[1].i;
                scratch[3].r = scratch_in[3].r * scratch_tw[2].r - scratch_in[3].i * scratch_tw[2].i;
                scratch[3].i = scratch_in[3].i * scratch_tw[2].r + scratch_in[3].r * scratch_tw[2].i;

                // X[0] +/- X[2N/4]
                scratch[4].r = scratch[0].r + scratch[2].r;
                scratch[4].i = scratch[0].i + scratch[2].i;
                scratch[5].r = scratch[0].r - scratch[2].r;
                scratch[5].i = scratch[0].i - scratch[2].i;

                // X[N/4] +/- X[3N/4]
                scratch[6].r = scratch[1].r + scratch[3].r;
                scratch[6].i = scratch[1].i + scratch[3].i;
                scratch[7].r = scratch[1].r - scratch[3].r;
                scratch[7].i = scratch[1].i - scratch[3].i;

                // Combine the (X[0] +/- X[2N/4]) and (X[N/4] +/- X[3N/4]) components (for
                // the full radix-4 butterfly)
                scratch_out[0].r = scratch[4].r + scratch[6].r;
                scratch_out[0].i = scratch[4].i + scratch[6].i;
                scratch_out[1].r = scratch[5].r + scratch[7].i;
                scratch_out[1].i = scratch[5].i - scratch[7].r;
                scratch_out[2].r = scratch[4].r - scratch[6].r;
                scratch_out[2].i = scratch[4].i - scratch[6].i;
                scratch_out[3].r = scratch[5].r - scratch[7].i;
                scratch_out[3].i = scratch[5].i + scratch[7].r;

                // Store the results
                dst[0] = scratch_out[0];
                dst[mstride * 1] = scratch_out[1];
                dst[mstride * 2] = scratch_out[2];
                dst[mstride * 3] = scratch_out[3];

                tw++;
                src++;
                dst++;
            } // m_count
        } // f_count

        // Update variables for the next stages
        twiddles += mstride * 3;
        mstride *= 4;
        fstride /= 4;

        // Swap the input and output buffers for the next stage
        tmp = in;
        in = out;
        out = tmp;
    } // stage_count

    // The last stage
    if (stage_count)
    {
        src = in;

        // Always write to the final output buffer (if necessary, we can calculate this
        // in-place as the final stage reads and writes at the same offsets)
        dst = out_final;

        for (f_count = fstride; f_count; f_count--) // Note: for C2C, fstride = 1
        {
            tw = twiddles; // Reset the twiddle pointer for the next section
            for (m_count = mstride; m_count; m_count--)
            {
                // Load the three twiddles and four input values for a radix-4 butterfly
                scratch_tw[0] = tw[0];
                scratch_tw[1] = tw[mstride * 1];
                scratch_tw[2] = tw[mstride * 2];
                scratch_in[0] = src[0];
                scratch_in[1] = src[step * 1];
                scratch_in[2] = src[step * 2];
                scratch_in[3] = src[step * 3];

                // Multiply input elements by their associated twiddles
                scratch[0] = scratch_in[0];
                scratch[1].r = scratch_in[1].r * scratch_tw[0].r - scratch_in[1].i * scratch_tw[0].i;
                scratch[1].i = scratch_in[1].i * scratch_tw[0].r + scratch_in[1].r * scratch_tw[0].i;
                scratch[2].r = scratch_in[2].r * scratch_tw[1].r - scratch_in[2].i * scratch_tw[1].i;
                scratch[2].i = scratch_in[2].i * scratch_tw[1].r + scratch_in[2].r * scratch_tw[1].i;
                scratch[3].r = scratch_in[3].r * scratch_tw[2].r - scratch_in[3].i * scratch_tw[2].i;
                scratch[3].i = scratch_in[3].i * scratch_tw[2].r + scratch_in[3].r * scratch_tw[2].i;

                // X[0] +/- X[2N/4]
                scratch[4].r = scratch[0].r + scratch[2].r;
                scratch[4].i = scratch[0].i + scratch[2].i;
                scratch[5].r = scratch[0].r - scratch[2].r;
                scratch[5].i = scratch[0].i - scratch[2].i;

                // X[N/4] +/- X[3N/4]
                scratch[6].r = scratch[1].r + scratch[3].r;
                scratch[6].i = scratch[1].i + scratch[3].i;
                scratch[7].r = scratch[1].r - scratch[3].r;
                scratch[7].i = scratch[1].i - scratch[3].i;

                // Combine the (X[0] +/- X[2N/4]) and (X[N/4] +/- X[3N/4]) components (for
                // the full radix-4 butterfly)
                scratch_out[0].r = scratch[4].r + scratch[6].r;
                scratch_out[0].i = scratch[4].i + scratch[6].i;
                scratch_out[1].r = scratch[5].r + scratch[7].i;
                scratch_out[1].i = scratch[5].i - scratch[7].r;
                scratch_out[2].r = scratch[4].r - scratch[6].r;
                scratch_out[2].i = scratch[4].i - scratch[6].i;
                scratch_out[3].r = scratch[5].r - scratch[7].i;
                scratch_out[3].i = scratch[5].i + scratch[7].r;

                // Store the results
                dst[0] = scratch_out[0];
                dst[step * 1] = scratch_out[1];
                dst[step * 2] = scratch_out[2];
                dst[step * 3] = scratch_out[3];

                tw++;
                src++;
                dst++;
            } // m_count
        } // f_count
    } // last stage
}

/*
 * This function calculates the inverse FFT, and is very similar in structure to its
 * complement "ne10_mixed_radix_butterfly_float32_c".
 */
static void ne10_mixed_radix_butterfly_inverse_float32_c (ne10_fft_cpx_float32_t *out,
        ne10_fft_cpx_float32_t *in,
        ne10_int32_t *factors,
        ne10_fft_cpx_float32_t *twiddles,
        ne10_fft_cpx_float32_t *buffer)
{
    ne10_int32_t stage_count = factors[0];
    ne10_int32_t fstride = factors[1];
    ne10_int32_t mstride = factors[(stage_count << 1) - 1];
    ne10_int32_t first_radix = factors[stage_count << 1];
    ne10_float32_t one_by_nfft = (1.0f / (ne10_float32_t) (fstride * first_radix));
    ne10_int32_t step, f_count, m_count;
    ne10_fft_cpx_float32_t *src = in;
    ne10_fft_cpx_float32_t *dst = out;
    ne10_fft_cpx_float32_t *out_final = out;
    ne10_fft_cpx_float32_t *tw, *tmp;
    const ne10_float32_t TW_81 = 0.70710678;

    ne10_fft_cpx_float32_t scratch[16];
    ne10_fft_cpx_float32_t scratch_in[8];
    ne10_fft_cpx_float32_t scratch_out[8];
    ne10_fft_cpx_float32_t scratch_tw[6];

    // The first stage (using hardcoded twiddles)
    if (first_radix == 8) // nfft is of form 2^{odd}
    {
        for (f_count = 0; f_count < fstride; f_count++)
        {
            dst = &out[f_count * 8];

            // Prepare sums for the butterfly calculations
            scratch_in[0].r = src[0].r + src[fstride * 4].r;
            scratch_in[0].i = src[0].i + src[fstride * 4].i;
            scratch_in[1].r = src[0].r - src[fstride * 4].r;
            scratch_in[1].i = src[0].i - src[fstride * 4].i;
            scratch_in[2].r = src[fstride].r + src[fstride * 5].r;
            scratch_in[2].i = src[fstride].i + src[fstride * 5].i;
            scratch_in[3].r = src[fstride].r - src[fstride * 5].r;
            scratch_in[3].i = src[fstride].i - src[fstride * 5].i;
            scratch_in[4].r = src[fstride * 2].r + src[fstride * 6].r;
            scratch_in[4].i = src[fstride * 2].i + src[fstride * 6].i;
            scratch_in[5].r = src[fstride * 2].r - src[fstride * 6].r;
            scratch_in[5].i = src[fstride * 2].i - src[fstride * 6].i;
            scratch_in[6].r = src[fstride * 3].r + src[fstride * 7].r;
            scratch_in[6].i = src[fstride * 3].i + src[fstride * 7].i;
            scratch_in[7].r = src[fstride * 3].r - src[fstride * 7].r;
            scratch_in[7].i = src[fstride * 3].i - src[fstride * 7].i;

            // Multiply some of these by hardcoded radix-8 twiddles
            scratch[0] = scratch_in[0];
            scratch[1] = scratch_in[1];
            scratch[2] = scratch_in[2];
            scratch[3].r = (scratch_in[3].r - scratch_in[3].i) * TW_81;
            scratch[3].i = (scratch_in[3].i + scratch_in[3].r) * TW_81;
            scratch[4] = scratch_in[4];
            scratch[5].r = -scratch_in[5].i;
            scratch[5].i = scratch_in[5].r;
            scratch[6].r = scratch_in[6].r;
            scratch[6].i = scratch_in[6].i;
            scratch[7].r = (scratch_in[7].r + scratch_in[7].i) * TW_81;
            scratch[7].i = (scratch_in[7].i - scratch_in[7].r) * TW_81;

            // Combine the first set of pairs of these sums
            scratch[8].r = scratch[0].r  + scratch[4].r;
            scratch[8].i = scratch[0].i  + scratch[4].i;
            scratch[9].r = scratch[1].r  + scratch[5].r;
            scratch[9].i = scratch[1].i  + scratch[5].i;
            scratch[10].r = scratch[0].r - scratch[4].r;
            scratch[10].i = scratch[0].i - scratch[4].i;
            scratch[11].r = scratch[1].r - scratch[5].r;
            scratch[11].i = scratch[1].i - scratch[5].i;

            // Combine the second set of pairs of these sums
            scratch[12].r = scratch[2].r + scratch[6].r;
            scratch[12].i = scratch[2].i + scratch[6].i;
            scratch[13].r = scratch[3].r - scratch[7].r;
            scratch[13].i = scratch[3].i - scratch[7].i;
            scratch[14].r = scratch[2].r - scratch[6].r;
            scratch[14].i = scratch[2].i - scratch[6].i;
            scratch[15].r = scratch[3].r + scratch[7].r;
            scratch[15].i = scratch[3].i + scratch[7].i;

            // Combine these combined components (for the full radix-8 butterfly)
            scratch_out[0].r = scratch[8].r  + scratch[12].r;
            scratch_out[0].i = scratch[8].i  + scratch[12].i;
            scratch_out[1].r = scratch[9].r  + scratch[13].r;
            scratch_out[1].i = scratch[9].i  + scratch[13].i;
            scratch_out[2].r = scratch[10].r - scratch[14].i;
            scratch_out[2].i = scratch[10].i + scratch[14].r;
            scratch_out[3].r = scratch[11].r - scratch[15].i;
            scratch_out[3].i = scratch[11].i + scratch[15].r;
            scratch_out[4].r = scratch[8].r  - scratch[12].r;
            scratch_out[4].i = scratch[8].i  - scratch[12].i;
            scratch_out[5].r = scratch[9].r  - scratch[13].r;
            scratch_out[5].i = scratch[9].i  - scratch[13].i;
            scratch_out[6].r = scratch[10].r + scratch[14].i;
            scratch_out[6].i = scratch[10].i - scratch[14].r;
            scratch_out[7].r = scratch[11].r + scratch[15].i;
            scratch_out[7].i = scratch[11].i - scratch[15].r;

            // Store the results
            dst[0] = scratch_out[0];
            dst[1] = scratch_out[1];
            dst[2] = scratch_out[2];
            dst[3] = scratch_out[3];
            dst[4] = scratch_out[4];
            dst[5] = scratch_out[5];
            dst[6] = scratch_out[6];
            dst[7] = scratch_out[7];

            src++;
        } // f_count

        // Update variables for the next stages
        step = fstride << 1;
        stage_count--;
        fstride /= 4;

        if (stage_count == 0)
        {
            dst = out;
            for (f_count = 0; f_count < 8; f_count++)
            {
                dst[f_count].r *= one_by_nfft;
                dst[f_count].i *= one_by_nfft;
            }
        }
    }
    else // first_radix == 4, nfft is of form 2^{even}
    {
        for (f_count = fstride; f_count; f_count--)
        {
            // Load the four input values for a radix-4 butterfly
            scratch_in[0] = src[0];
            scratch_in[1] = src[fstride * 1];
            scratch_in[2] = src[fstride * 2];
            scratch_in[3] = src[fstride * 3];

            // Prepare the first set of sums for the butterfly calculations
            scratch[0].r = scratch_in[0].r + scratch_in[2].r;
            scratch[0].i = scratch_in[0].i + scratch_in[2].i;
            scratch[1].r = scratch_in[0].r - scratch_in[2].r;
            scratch[1].i = scratch_in[0].i - scratch_in[2].i;

            // Prepare the second set of sums for the butterfly calculations
            scratch[2].r = scratch_in[1].r + scratch_in[3].r;
            scratch[2].i = scratch_in[1].i + scratch_in[3].i;
            scratch[3].r = scratch_in[1].r - scratch_in[3].r;
            scratch[3].i = scratch_in[1].i - scratch_in[3].i;

            // Combine these sums (for the full radix-4 butterfly)
            scratch_out[0].r = scratch[0].r + scratch[2].r;
            scratch_out[0].i = scratch[0].i + scratch[2].i;
            scratch_out[1].r = scratch[1].r - scratch[3].i;
            scratch_out[1].i = scratch[1].i + scratch[3].r;
            scratch_out[2].r = scratch[0].r - scratch[2].r;
            scratch_out[2].i = scratch[0].i - scratch[2].i;
            scratch_out[3].r = scratch[1].r + scratch[3].i;
            scratch_out[3].i = scratch[1].i - scratch[3].r;

            // Store the results
            *dst++ = scratch_out[0];
            *dst++ = scratch_out[1];
            *dst++ = scratch_out[2];
            *dst++ = scratch_out[3];

            src++;
        } // f_count

        // Update variables for the next stages
        step = fstride;
        stage_count--;
        fstride /= 4;

        if (stage_count == 0)
        {
            dst = out;
            for (f_count = 0; f_count < 4; f_count++)
            {
                dst[f_count].r *= one_by_nfft;
                dst[f_count].i *= one_by_nfft;
            }
        }
    }

    // The next stage should read the output of the first stage as input
    in = out;
    out = buffer;

    // Middle stages (after the first, excluding the last)
    for (; stage_count > 1; stage_count--)
    {
        src = in;
        for (f_count = 0; f_count < fstride; f_count++)
        {
            dst = &out[f_count * (mstride * 4)];
            tw = twiddles;
            for (m_count = mstride; m_count ; m_count --)
            {
                // Load the three twiddles and four input values for a radix-4 butterfly
                scratch_tw[0] = tw[0];
                scratch_tw[1] = tw[mstride * 1];
                scratch_tw[2] = tw[mstride * 2];
                scratch_in[0] = src[0];
                scratch_in[1] = src[step * 1];
                scratch_in[2] = src[step * 2];
                scratch_in[3] = src[step * 3];

                // Multiply input elements by their associated twiddles
                scratch[0] = scratch_in[0];
                scratch[1].r = scratch_in[1].r * scratch_tw[0].r + scratch_in[1].i * scratch_tw[0].i;
                scratch[1].i = scratch_in[1].i * scratch_tw[0].r - scratch_in[1].r * scratch_tw[0].i;
                scratch[2].r = scratch_in[2].r * scratch_tw[1].r + scratch_in[2].i * scratch_tw[1].i;
                scratch[2].i = scratch_in[2].i * scratch_tw[1].r - scratch_in[2].r * scratch_tw[1].i;
                scratch[3].r = scratch_in[3].r * scratch_tw[2].r + scratch_in[3].i * scratch_tw[2].i;
                scratch[3].i = scratch_in[3].i * scratch_tw[2].r - scratch_in[3].r * scratch_tw[2].i;

                // Prepare the first set of sums for the butterfly calculations
                scratch[4].r = scratch[0].r + scratch[2].r;
                scratch[4].i = scratch[0].i + scratch[2].i;
                scratch[5].r = scratch[0].r - scratch[2].r;
                scratch[5].i = scratch[0].i - scratch[2].i;

                // Prepare the second set of sums for the butterfly calculations
                scratch[6].r = scratch[1].r + scratch[3].r;
                scratch[6].i = scratch[1].i + scratch[3].i;
                scratch[7].r = scratch[1].r - scratch[3].r;
                scratch[7].i = scratch[1].i - scratch[3].i;

                // Combine these sums (for the full radix-4 butterfly)
                scratch_out[0].r = scratch[4].r + scratch[6].r;
                scratch_out[0].i = scratch[4].i + scratch[6].i;
                scratch_out[1].r = scratch[5].r - scratch[7].i;
                scratch_out[1].i = scratch[5].i + scratch[7].r;
                scratch_out[2].r = scratch[4].r - scratch[6].r;
                scratch_out[2].i = scratch[4].i - scratch[6].i;
                scratch_out[3].r = scratch[5].r + scratch[7].i;
                scratch_out[3].i = scratch[5].i - scratch[7].r;

                // Store the results
                dst[0] = scratch_out[0];
                dst[mstride * 1] = scratch_out[1];
                dst[mstride * 2] = scratch_out[2];
                dst[mstride * 3] = scratch_out[3];

                tw++;
                src++;
                dst++;
            } // m_count
        } // f_count

        // Update variables for the next stages
        twiddles += mstride * 3;
        mstride *= 4;
        fstride /= 4;

        // Swap the input and output buffers for the next stage
        tmp = in;
        in = out;
        out = tmp;
    } // stage_count

    // The last stage
    if (stage_count)
    {
        src = in;

        // Always write to the final output buffer (if necessary, we can calculate this
        // in-place as the final stage reads and writes at the same offsets)
        dst = out_final;

        for (f_count = 0; f_count < fstride; f_count++)
        {
            tw = twiddles;
            for (m_count = mstride; m_count; m_count--)
            {
                // Load the three twiddles and four input values for a radix-4 butterfly
                scratch_tw[0] = tw[0];
                scratch_tw[1] = tw[mstride * 1];
                scratch_tw[2] = tw[mstride * 2];
                scratch_in[0] = src[0];
                scratch_in[1] = src[step * 1];
                scratch_in[2] = src[step * 2];
                scratch_in[3] = src[step * 3];

                // Multiply input elements by their associated twiddles
                scratch[0] = scratch_in[0];
                scratch[1].r = scratch_in[1].r * scratch_tw[0].r + scratch_in[1].i * scratch_tw[0].i;
                scratch[1].i = scratch_in[1].i * scratch_tw[0].r - scratch_in[1].r * scratch_tw[0].i;
                scratch[2].r = scratch_in[2].r * scratch_tw[1].r + scratch_in[2].i * scratch_tw[1].i;
                scratch[2].i = scratch_in[2].i * scratch_tw[1].r - scratch_in[2].r * scratch_tw[1].i;
                scratch[3].r = scratch_in[3].r * scratch_tw[2].r + scratch_in[3].i * scratch_tw[2].i;
                scratch[3].i = scratch_in[3].i * scratch_tw[2].r - scratch_in[3].r * scratch_tw[2].i;

                // Prepare the first set of sums for the butterfly calculations
                scratch[4].r = scratch[0].r + scratch[2].r;
                scratch[4].i = scratch[0].i + scratch[2].i;
                scratch[5].r = scratch[0].r - scratch[2].r;
                scratch[5].i = scratch[0].i - scratch[2].i;

                // Prepare the second set of sums for the butterfly calculations
                scratch[6].r = scratch[1].r + scratch[3].r;
                scratch[6].i = scratch[1].i + scratch[3].i;
                scratch[7].r = scratch[1].r - scratch[3].r;
                scratch[7].i = scratch[1].i - scratch[3].i;

                // Combine these sums (for the full radix-4 butterfly) and multiply by
                // (1 / nfft).
                scratch_out[0].r = (scratch[4].r + scratch[6].r) * one_by_nfft;
                scratch_out[0].i = (scratch[4].i + scratch[6].i) * one_by_nfft;
                scratch_out[1].r = (scratch[5].r - scratch[7].i) * one_by_nfft;
                scratch_out[1].i = (scratch[5].i + scratch[7].r) * one_by_nfft;
                scratch_out[2].r = (scratch[4].r - scratch[6].r) * one_by_nfft;
                scratch_out[2].i = (scratch[4].i - scratch[6].i) * one_by_nfft;
                scratch_out[3].r = (scratch[5].r + scratch[7].i) * one_by_nfft;
                scratch_out[3].i = (scratch[5].i - scratch[7].r) * one_by_nfft;

                // Store the results
                dst[0] = scratch_out[0];
                dst[step * 1] = scratch_out[1];
                dst[step * 2] = scratch_out[2];
                dst[step * 3] = scratch_out[3];

                tw++;
                src++;
                dst++;
            } // m_count
        } // f_count
    } // last stage
}

static void ne10_fft_split_r2c_1d_float32 (ne10_fft_cpx_float32_t *dst,
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

static void ne10_fft_split_c2r_1d_float32 (ne10_fft_cpx_float32_t *dst,
        const ne10_fft_cpx_float32_t *src,
        ne10_fft_cpx_float32_t *twiddles,
        ne10_int32_t ncfft)
{

    ne10_int32_t k;
    ne10_fft_cpx_float32_t fk, fnkc, fek, fok, tmp;


    dst[0].r = (src[0].r + src[ncfft].r) * 0.5f;
    dst[0].i = (src[0].r - src[ncfft].r) * 0.5f;

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

        dst[k].r = (fek.r + fok.r) * 0.5f;
        dst[k].i = (fek.i + fok.i) * 0.5f;

        dst[ncfft - k].r = (fek.r - fok.r) * 0.5f;
        dst[ncfft - k].i = (fok.i - fek.i) * 0.5f;
    }
}

/**
 * @defgroup C2C_FFT_IFFT Floating & Fixed Point Complex-to-Complex FFT
 * @ingroup groupDSPs
 * @{
 *
 * \par
 * A Fast Fourier Transform (FFT) is an efficient method of computing the
 * [Discrete Fourier Transform](https://en.wikipedia.org/wiki/Discrete_Fourier_transform) (DFT), or its inverse.
 * FFT operations are widely used, and have various applications in engineering, science and mathematics.
 *
 * \par Variants
 * This set of FFT functions implement the complex-to-complex 1D FFT -- and the complementary inverse transform (IFFT) -- for
 * various input sizes, with separate versions for floating point (`fft_c2c_1d_float32`) and fixed point
 * (`fft_c2c_1d_int32`, `fft_c2c_1d_int16`) data.
 *
 * \par Output
 * These functions operate out-of-place, using different buffers for their input and output. An additional temporary buffer is
 * also used internally to store intermediate results. The input and output buffers should be allocated by the user, and must
 * be of a size greater than or equal to `(fftSize * sizeof (ne10_fft_cpx_float32_t))` bytes, while the temporary buffer is
 * reserved during allocation of the operation's "configuration structure".
 *
 * \par Data format
 * Input and output buffers interleave the real and imaginary parts of each complex element, storing these contiguously.
 * <pre>Format: { Re[0], Im[0], Re[1], Im[1], Re[2], Im[2], ..., Re[fftSize - 2], Im[fftSize - 2], Re[fftSize - 1], Im[fftSize - 1] }</pre>
 *
 * \par Supported lengths
 * Internally, the functions primarily utilize a mixed radix 2/4 decimation-in-time algorithm, supporting input sizes of the form 2^N
 * (for integers N > 0). Some functions also support non-power-of-two input sizes using additional radices, and this is noted in the
 * relevant function descriptions where it applies.
 *
 * \par Example usage
 * A single precision floating point FFT/IFFT example code snippet follows.
 * <pre>
 * #include "NE10.h"
 * ...
 * {
 *     ne10_int32_t fftSize = 1024; // 2^N, N > 0 (e.g. 1024 at N = 10)
 *     ne10_fft_cfg_float32_t cfg = ne10_fft_alloc_c2c_float32 (fftSize);
 *     ne10_fft_cpx_float32_t *in  = malloc (fftSize * sizeof (ne10_fft_cpx_float32_t));
 *     ne10_fft_cpx_float32_t *out = malloc (fftSize * sizeof (ne10_fft_cpx_float32_t));
 *     ...
 *     // FFT
 *     ne10_fft_c2c_1d_float32_c (out, in, cfg, 0);
 *     ...
 *     // IFFT
 *     ne10_fft_c2c_1d_float32_c (out, in, cfg, 1);
 *     ...
 *     NE10_FREE (out);
 *     NE10_FREE (in);
 *     ne10_fft_destroy_c2c_float32 (cfg);
 * }
 * </pre>
 * The `ne10_fft_cfg_float32_t` variable `cfg` is a pointer to a configuration structure generated by `ne10_fft_alloc_c2c_float32`.
 * For different inputs of the same size, the same configuration structure can (and, indeed, should) be used. The contents of this
 * configuration structure include:
 * - `cfg->twiddles`: a pointer to a table of "twiddle factors" that are used in computing the FFT/IFFT
 * - `cfg->factors`: a buffer of "factors", which suggests to the core algorithm how the input can be broken down into smaller calculations
 * - `cfg->buffer`: a pointer to a temporary buffer used internally in calculations that is allocated when the configuration
 *                  structure is set up
 */

/**
 * @brief User-callable function to create a configuration structure for the FP32 C2C C FFT/IFFT.
 * @param[in]   nfft             input length
 * @retval      st               pointer to an FFT configuration structure (allocated with `malloc`), or `NULL` to indicate an error
 *
 * This function allocates and initialises an `ne10_fft_cfg_float32_t` configuration structure for
 * the C complex-to-complex single precision floating point FFT/IFFT. As part of this, it reserves
 * a buffer used internally by the FFT algorithm, factors the length of the FFT into simpler chunks,
 * and generates a "twiddle table" of coefficients used in the FFT "butterfly" calculations.
 */
ne10_fft_cfg_float32_t ne10_fft_alloc_c2c_float32_c (ne10_int32_t nfft)
{
    ne10_fft_cfg_float32_t st = NULL;
    ne10_uint32_t memneeded = sizeof (ne10_fft_state_float32_t)
                              + sizeof (ne10_int32_t) * (NE10_MAXFACTORS * 2) /* factors */
                              + sizeof (ne10_fft_cpx_float32_t) * nfft       /* twiddles */
                              + sizeof (ne10_fft_cpx_float32_t) * nfft         /* buffer */
                              + NE10_FFT_BYTE_ALIGNMENT;             /* 64-bit alignment */

    st = (ne10_fft_cfg_float32_t) NE10_MALLOC (memneeded);

    // Only backward FFT is scaled by default.
    st->is_forward_scaled = 0;
    st->is_backward_scaled = 1;

    if (st == NULL)
    {
        return st;
    }

    uintptr_t address = (uintptr_t) st + sizeof (ne10_fft_state_float32_t);
    NE10_BYTE_ALIGNMENT (address, NE10_FFT_BYTE_ALIGNMENT);
    st->factors = (ne10_int32_t*) address;
    st->twiddles = (ne10_fft_cpx_float32_t*) (st->factors + (NE10_MAXFACTORS * 2));
    st->buffer = st->twiddles + nfft;
    st->nfft = nfft;

    ne10_int32_t result;
    result = ne10_factor (nfft, st->factors, NE10_FACTOR_EIGHT_FIRST_STAGE);
    if (result == NE10_ERR)
    {
        NE10_FREE (st);
        return NULL;
    }

    // Disable radix 8 for generic FFTs (it doesn't work properly)
    {
        ne10_int32_t stage_count    = st->factors[0];
        ne10_int32_t algorithm_flag = st->factors[2 * (stage_count + 1)];

        if (algorithm_flag == NE10_FFT_ALG_ANY)
        {
            result = ne10_factor (st->nfft, st->factors, NE10_FACTOR_DEFAULT);
            if (result == NE10_ERR)
            {
                NE10_FREE (st);
                return NULL;
            }
        }
    }

    ne10_fft_generate_twiddles_float32 (st->twiddles, st->factors, nfft);

    return st;
}

/**
 * @brief Mixed radix-2/3/4/5 complex C FFT/IFFT of single precision floating point data.
 * @param[out]  *fout            pointer to the output buffer
 * @param[in]   *fin             pointer to the input buffer
 * @param[in]   cfg              pointer to the configuration structure
 * @param[in]   inverse_fft      whether this is an FFT or IFFT (0: FFT, 1: IFFT)
 *
 * This function implements a mixed radix-2/3/4/5 complex FFT/IFFT, supporting input lengths of the
 * form 2^N*3^M*5^K (N, M, K > 0). This is an out-of-place algorithm. For usage information, please
 * check test/test_suite_fft_float32.c.
 */
void ne10_fft_c2c_1d_float32_c (ne10_fft_cpx_float32_t *fout,
                                ne10_fft_cpx_float32_t *fin,
                                ne10_fft_cfg_float32_t cfg,
                                ne10_int32_t inverse_fft)
{
    ne10_int32_t stage_count = cfg->factors[0];
    ne10_int32_t algorithm_flag = cfg->factors[2 * (stage_count + 1)];

    assert ((algorithm_flag == NE10_FFT_ALG_DEFAULT)
            || (algorithm_flag == NE10_FFT_ALG_ANY));

    switch (algorithm_flag)
    {
    case NE10_FFT_ALG_DEFAULT:
        if (inverse_fft)
        {
            ne10_mixed_radix_butterfly_inverse_float32_c (fout, fin, cfg->factors, cfg->twiddles, cfg->buffer);
        }
        else
        {
            ne10_mixed_radix_butterfly_float32_c (fout, fin, cfg->factors, cfg->twiddles, cfg->buffer);
        }
        break;
    case NE10_FFT_ALG_ANY:
        if (inverse_fft)
        {
            ne10_mixed_radix_generic_butterfly_inverse_float32_c (fout, fin,
                    cfg->factors, cfg->twiddles, cfg->buffer, cfg->is_backward_scaled);
        }
        else
        {
            ne10_mixed_radix_generic_butterfly_float32_c (fout, fin,
                    cfg->factors, cfg->twiddles, cfg->buffer, cfg->is_forward_scaled);
        }
        break;
    }
}

/**
 * @}
 */ //end of C2C_FFT_IFFT group

/**
 * @defgroup R2C_FFT_IFFT Floating & Fixed Point Real-to-Complex FFT
 * @ingroup groupDSPs
 * @{
 *
 * \par
 * It is often the case in practical applications that full complex-to-complex FFT operations are unnecessary --
 * many applications deal exclusively with real signals. For this reason, Ne10 offers real-to-complex FFT and
 * complex-to-real IFFT operations.
 *
 * \par Output
 * These functions operate out-of-place, using different buffers for their input and output. An additional temporary buffer is
 * also used internally to store intermediate results. The input and output buffers should be allocated by the user, while the
 * temporary buffer is reserved during allocation of the operation's "configuration structure".
 *
 * \par Data format
 * Buffers of complex elements interleave the real and imaginary parts of each element, storing these contiguously, while buffers of
 * real elements simply hold values contiguously in memory as simple C arrays.
 * <pre>
 * Real-to-Complex (R2C) FFT:
 *     Input:  { Re[0], Re[1], Re[2], Re[3], Re[4], Re[5], ..., Re[fftSize - 4], Re[fftSize - 3], Re[fftSize - 2], Re[fftSize - 1] }
 *     Output: { Re[0], Im[0], Re[1], Im[1], Re[2], Im[2], ..., Re[(fftSize / 2) - 1], Im[(fftSize / 2) - 1], Re[(fftSize / 2)], Im[(fftSize / 2)] }
 * Complex-to-Real (C2R) IFFT:
 *     Input:  { Re[0], Im[0], Re[1], Im[1], Re[2], Im[2], ..., Re[(fftSize / 2) - 1], Im[(fftSize / 2) - 1], Re[(fftSize / 2)], Im[(fftSize / 2)] }
 *     Output: { Re[0], Re[1], Re[2], Re[3], Re[4], Re[5], ..., Re[fftSize - 4], Re[fftSize - 3], Re[fftSize - 2], Re[fftSize - 1] }
 * </pre>
 *
 * \par Supported lengths
 * These functions support input sizes of the form 2^N (for integers N > 0).
 *
 * \par Example usage
 * A single precision floating point FFT/IFFT example code snippet follows.
 * <pre>
 * #include "NE10.h"
 * ...
 * {
 *     ne10_int32_t fftSize = 1024; // 2^N, N > 0 (e.g. 1024 at N = 10)
 *     ne10_fft_cfg_float32_t cfg = ne10_fft_alloc_r2c_float32 (fftSize);
 *     ne10_float32_t *in = malloc (fftSize * sizeof (ne10_float32_t));
 *     ne10_fft_cpx_float32_t *out = malloc (fftSize * sizeof (ne10_fft_cpx_float32_t));
 *     ...
 *     // FFT
 *     ne10_fft_r2c_1d_float32_c (out, in, cfg);
 *     ...
 *     // IFFT
 *     ne10_fft_c2r_1d_float32_c (in, out, cfg);
 *     ...
 *     NE10_FREE (out);
 *     NE10_FREE (in);
 *     ne10_fft_destroy_r2c_float32 (cfg);
 * }
 * </pre>
 * The `ne10_fft_r2c_cfg_float32_t` variable `cfg` is a pointer to a configuration structure generated by `ne10_fft_alloc_r2c_float32`.
 * For different inputs of the same size, the same configuration structure can (and, indeed, should) be used. The contents of this
 * configuration structure include:
 * - `cfg->twiddles`: a pointer to a table of "twiddle factors" that are used in computing the FFT/IFFT
 * - `cfg->super_twiddles`: a pointer to a table of twiddle factors used in splitting complex and real components
 * - `cfg->factors`: a buffer of "factors", which suggests to the core algorithm how the input can be broken down into smaller calculations
 * - `cfg->buffer`: a pointer to a temporary buffer used internally in calculations that is allocated when the configuration
 *                  structure is set up
 */

// For NE10_UNROLL_LEVEL > 0, please refer to NE10_rfft_float.c
#if (NE10_UNROLL_LEVEL == 0)

/**
 * @brief User-callable function to create a configuration structure for the FP32 R2C/C2R FFT/IFFT.
 * @param[in]   nfft             input length
 * @retval      st               pointer to an FFT configuration structure (allocated with `malloc`), or `NULL` to indicate an error
 *
 * This function allocates and initialises an `ne10_fft_r2c_cfg_float32_t` configuration structure for
 * the FP32 real-to-complex and complex-to-real FFT/IFFT. As part of this, it reserves a buffer used
 * internally by the FFT algorithm, factors the length of the FFT into simpler chunks, and generates a
 * "twiddle table" of coefficients used in the FFT "butterfly" calculations.
 */
ne10_fft_r2c_cfg_float32_t ne10_fft_alloc_r2c_float32 (ne10_int32_t nfft)
{
    ne10_fft_r2c_cfg_float32_t st = NULL;
    ne10_int32_t ncfft = nfft >> 1;

    ne10_uint32_t memneeded = sizeof (ne10_fft_r2c_state_float32_t)
                              + sizeof (ne10_int32_t) * (NE10_MAXFACTORS * 2)        /* factors */
                              + sizeof (ne10_fft_cpx_float32_t) * ncfft              /* twiddle */
                              + sizeof (ne10_fft_cpx_float32_t) * (ncfft / 2) /* super twiddles */
                              + sizeof (ne10_fft_cpx_float32_t) * nfft                /* buffer */
                              + NE10_FFT_BYTE_ALIGNMENT;                    /* 64-bit alignment */

    st = (ne10_fft_r2c_cfg_float32_t) NE10_MALLOC (memneeded);

    if (st)
    {
        uintptr_t address = (uintptr_t) st + sizeof (ne10_fft_r2c_state_float32_t);
        NE10_BYTE_ALIGNMENT (address, NE10_FFT_BYTE_ALIGNMENT);
        st->factors = (ne10_int32_t*) address;
        st->twiddles = (ne10_fft_cpx_float32_t*) (st->factors + (NE10_MAXFACTORS * 2));
        st->super_twiddles = st->twiddles + ncfft;
        st->buffer = st->super_twiddles + (ncfft / 2);
        st->ncfft = ncfft;

        ne10_int32_t result = ne10_factor (ncfft, st->factors, NE10_FACTOR_EIGHT_FIRST_STAGE);
        if (result == NE10_ERR)
        {
            NE10_FREE (st);
            return NULL;
        }

        ne10_int32_t j, k;
        ne10_int32_t *factors = st->factors;
        ne10_fft_cpx_float32_t *twiddles = st->twiddles;
        ne10_int32_t stage_count = factors[0];
        ne10_int32_t fstride = factors[1];
        ne10_int32_t mstride;
        ne10_int32_t cur_radix;
        ne10_float32_t phase;
        const ne10_float32_t pi = NE10_PI;

        // Don't generate any twiddles for the first stage
        stage_count --;

        // Generate twiddles for the other stages
        for (; stage_count > 0; stage_count --)
        {
            cur_radix = factors[2 * stage_count];
            fstride /= cur_radix;
            mstride = factors[2 * stage_count + 1];
            for (j = 0; j < mstride; j++)
            {
                for (k = 1; k < cur_radix; k++) // phase = 1 when k = 0
                {
                    phase = -2 * pi * fstride * k * j / ncfft;
                    twiddles[mstride * (k - 1) + j].r = (ne10_float32_t) cos (phase);
                    twiddles[mstride * (k - 1) + j].i = (ne10_float32_t) sin (phase);
                }
            }
            twiddles += mstride * (cur_radix - 1);
        }

        twiddles = st->super_twiddles;
        for (j = 0; j < ncfft / 2; j++)
        {
            phase = -pi * ( (ne10_float32_t) (j + 1) / ncfft + 0.5f);
            twiddles->r = (ne10_float32_t) cos (phase);
            twiddles->i = (ne10_float32_t) sin (phase);
            twiddles++;
        }
    }

    return st;
}

/**
 * @brief Mixed radix-2/4 real-to-complex C FFT of single precision floating point data.
 * @param[out]  *fout            pointer to the output buffer
 * @param[in]   *fin             pointer to the input buffer
 * @param[in]   cfg              pointer to the configuration structure
 *
 * The function implements a mixed radix-2/4 real-to-complex FFT, supporting input lengths of
 * the form 2^N (N > 0). This is an out-of-place algorithm. For usage information, please check
 * test/test_suite_fft_float32.c.
 */
void ne10_fft_r2c_1d_float32_c (ne10_fft_cpx_float32_t *fout,
                                ne10_float32_t *fin,
                                ne10_fft_r2c_cfg_float32_t cfg)
{
    ne10_fft_cpx_float32_t * tmpbuf = cfg->buffer;

    ne10_mixed_radix_butterfly_float32_c (tmpbuf, (ne10_fft_cpx_float32_t*) fin, cfg->factors, cfg->twiddles, fout);
    ne10_fft_split_r2c_1d_float32 (fout, tmpbuf, cfg->super_twiddles, cfg->ncfft);
}

/**
 * @brief Mixed radix-2/4 complex-to-real C IFFT of single precision floating point data.
 * @param[out]  *fout            pointer to the output buffer
 * @param[in]   *fin             pointer to the input buffer
 * @param[in]   cfg              pointer to the configuration structure
 *
 * The function implements a mixed radix-2/4 complex-to-real IFFT, supporting input lengths of
 * the form 2^N (N > 0). This is an out-of-place algorithm. For usage information, please check
 * test/test_suite_fft_float32.c.
 */
void ne10_fft_c2r_1d_float32_c (ne10_float32_t *fout,
                                ne10_fft_cpx_float32_t *fin,
                                ne10_fft_r2c_cfg_float32_t cfg)
{
    ne10_fft_cpx_float32_t * tmpbuf1 = cfg->buffer;
    ne10_fft_cpx_float32_t * tmpbuf2 = cfg->buffer + cfg->ncfft;

    ne10_fft_split_c2r_1d_float32 (tmpbuf1, fin, cfg->super_twiddles, cfg->ncfft);
    ne10_mixed_radix_butterfly_inverse_float32_c ( (ne10_fft_cpx_float32_t*) fout, tmpbuf1, cfg->factors, cfg->twiddles, tmpbuf2);
}

/**
 * @} end of R2C_FFT_IFFT group
 */
#endif // NE10_UNROLL_LEVEL
