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
 * NE10 Library : dsp/NE10_fft_int16.neon.c
 */

#include "NE10_types.h"
#include "NE10_macros.h"
#include "NE10_fft.h"

/**
 * @addtogroup C2C_FFT_IFFT
 * @{
 */

/**
 * @brief Mixed radix-2/4 complex FFT/IFFT of 16-bit fixed point data.
 * @param[out]  *fout            point to the output buffer (out-of-place)
 * @param[in]   *fin             point to the input buffer (out-of-place)
 * @param[in]   *twiddles        point to the twiddle buffer
 * @param[in]   *factors         point to factors buffer. 0: stage number, 1: stride for the first stage, others: radix and stage's fft length/radix
 * @param[in]   nfft             length of FFT
 * @param[in]   inverse_fft      the flag of IFFT, 0: FFT, 1: IFFT
 * @return none.
 * The function implements a mixed radix-2/4 complex FFT/IFFT. The length of 2^N(N is 1, 2, 3, 4, 5, 6 ....etc) is supported.
 * Otherwise, this FFT is an out-of-place algorithm. When you want to get an in-place FFT, it creates a temp buffer as
 *  output buffer and then copies the temp buffer back to input buffer. For the usage of this function, please check test/test_suite_fft_int16.c
 */

void ne10_fft_c2c_1d_int16_neon (ne10_fft_cpx_int16_t *fout,
                                 ne10_fft_cpx_int16_t *fin,
                                 ne10_fft_cpx_int16_t *twiddles,
                                 ne10_int32_t *factors,
                                 ne10_int32_t nfft,
                                 ne10_int32_t inverse_fft)
{
    if (fin == fout)
    {
        /* NOTE: for an in-place FFT algorithm. It just performs an out-of-place FFT into a temp buffer */
        ne10_fft_cpx_int16_t * tmpbuf_ = (ne10_fft_cpx_int16_t*) NE10_MALLOC (sizeof (ne10_fft_cpx_int16_t) * nfft);

        // copy the data from input to output and bit reversal
        ne10_data_bitreversal_int16 (tmpbuf_, fin, 1, &factors[2]);

        if (inverse_fft)
            ne10_mixed_radix_butterfly_inverse_int16_neon (tmpbuf_, factors, twiddles);
        else
            ne10_mixed_radix_butterfly_int16_neon (tmpbuf_, factors, twiddles);

        memcpy (fout, tmpbuf_, sizeof (ne10_fft_cpx_int16_t) *nfft);
        NE10_FREE (tmpbuf_);
    }
    else
    {
        // copy the data from input to output and bit reversal
        ne10_data_bitreversal_int16 (fout, fin, 1, &factors[2]);

        if (inverse_fft)
            ne10_mixed_radix_butterfly_inverse_int16_neon (fout, factors, twiddles);
        else
            ne10_mixed_radix_butterfly_int16_neon (fout, factors, twiddles);
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
 * @brief Mixed radix-2/4 FFT (real to complex) of int16 data.
 * @param[out]  *fout            point to the output buffer
 * @param[in]   *fin             point to the input buffer
 * @param[in]   *twiddles        point to the twiddle buffer
 * @param[in]   *super_twiddles  point to the twiddle buffer for data split
 * @param[in]   *factors         point to factors buffer. 0: stage number, 1: stride for the first stage, others: radix and stage's fft length/radix
 * @param[in]   nfft             length of FFT
 * @return none.
 * The function implements a mixed radix-2/4 FFT (real to complex). The length of 2^N(N is 2, 3, 4, 5, 6 ....etc) is supported.
 * Otherwise, we alloc a temp buffer(the size is same as input buffer) for storing intermedia.
 * For the usage of this function, please check test/test_suite_fft_int16.c
 */
void ne10_fft_r2c_1d_int16_neon (ne10_fft_cpx_int16_t *fout,
                                 ne10_int16_t *fin,
                                 ne10_fft_cpx_int16_t *twiddles,
                                 ne10_fft_cpx_int16_t *super_twiddles,
                                 ne10_int32_t *factors,
                                 ne10_int32_t nfft)
{
    ne10_int32_t ncfft = nfft >> 1;

    /* malloc a temp buffer for cfft */
    ne10_fft_cpx_int16_t * tmpbuf_ = (ne10_fft_cpx_int16_t*) NE10_MALLOC (sizeof (ne10_fft_cpx_int16_t) * ncfft);

    // copy the data from input to output and bit reversal
    ne10_data_bitreversal_int16 (tmpbuf_, (ne10_fft_cpx_int16_t*) fin, 1, &factors[2]);
    ne10_mixed_radix_butterfly_int16_neon (tmpbuf_, factors, twiddles);

    ne10_fft_split_r2c_1d_int16 (fout, tmpbuf_, super_twiddles, ncfft);

    NE10_FREE (tmpbuf_);
}

/**
 * @brief Mixed radix-2/4 IFFT (complex to real) of int16 data.
 * @param[out]  *fout            point to the output buffer
 * @param[in]   *fin             point to the input buffer
 * @param[in]   *twiddles        point to the twiddle buffer
 * @param[in]   *super_twiddles  point to the twiddle buffer for data split
 * @param[in]   *factors         point to factors buffer. 0: stage number, 1: stride for the first stage, others: radix and stage's fft length/radix
 * @param[in]   nfft             length of FFT
 * @return none.
 * The function implements a mixed radix-2/4 FFT (complex to real). The length of 2^N(N is 2, 3, 4, 5, 6 ....etc) is supported.
 * Otherwise, we alloc a temp buffer(the size is same as input buffer) for storing intermedia.
 * For the usage of this function, please check test/test_suite_fft_int16.c
 */
void ne10_fft_c2r_1d_int16_neon (ne10_int16_t *fout,
                                 ne10_fft_cpx_int16_t *fin,
                                 ne10_fft_cpx_int16_t *twiddles,
                                 ne10_fft_cpx_int16_t *super_twiddles,
                                 ne10_int32_t *factors,
                                 ne10_int32_t nfft)
{
    ne10_int32_t ncfft = nfft >> 1;

    /* malloc a temp buffer for split */
    ne10_fft_cpx_int16_t * tmpbuf_ = (ne10_fft_cpx_int16_t*) NE10_MALLOC (sizeof (ne10_fft_cpx_int16_t) * ncfft);

    ne10_fft_split_c2r_1d_int16 (tmpbuf_, fin, super_twiddles, ncfft);

    // copy the data from input to output and bit reversal
    ne10_data_bitreversal_int16 ( (ne10_fft_cpx_int16_t*) fout, tmpbuf_, 1, &factors[2]);
    ne10_mixed_radix_butterfly_inverse_int16_neon ( (ne10_fft_cpx_int16_t*) fout, factors, twiddles);

    NE10_FREE (tmpbuf_);
}

/**
 * @} end of R2C_FFT_IFFT group
 */
