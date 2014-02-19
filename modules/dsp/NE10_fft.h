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

/*
 * NE10 Library : dsp/NE10_fft.h
 */


#include <NE10_types.h>

#ifndef NE10_FFT_H
#define NE10_FFT_H

#ifdef __cplusplus
extern "C" {
#endif

///////////////////////////
// function prototypes:
///////////////////////////

    /*common fft functions */

    /*common functions for float fft */
    extern void ne10_data_bitreversal_float32 (ne10_fft_cpx_float32_t * Fout,
            const ne10_fft_cpx_float32_t * f,
            ne10_int32_t fstride,
            ne10_int32_t * factors);
    extern void ne10_fft_split_r2c_1d_float32 (ne10_fft_cpx_float32_t *dst,
            const ne10_fft_cpx_float32_t *src,
            ne10_fft_cpx_float32_t *twiddles,
            ne10_int32_t ncfft);
    extern void ne10_fft_split_c2r_1d_float32 (ne10_fft_cpx_float32_t *dst,
            const ne10_fft_cpx_float32_t *src,
            ne10_fft_cpx_float32_t *twiddles,
            ne10_int32_t ncfft);
    extern void ne10_radix4_butterfly_forward_float32_neon (ne10_fft_cpx_float32_t * Fout,
            ne10_int32_t * factors,
            ne10_fft_cpx_float32_t * twiddles)
    asm ("ne10_radix4_butterfly_forward_float32_neon");
    extern void ne10_radix4_butterfly_backward_float32_neon (ne10_fft_cpx_float32_t * Fout,
            ne10_int32_t * factors,
            ne10_fft_cpx_float32_t * twiddles)
    asm ("ne10_radix4_butterfly_backward_float32_neon");

    extern void ne10_radix2_butterfly_forward_float32_neon (ne10_fft_cpx_float32_t * Fout,
            ne10_int32_t * factors,
            ne10_fft_cpx_float32_t * twiddles)
    asm ("ne10_radix2_butterfly_forward_float32_neon");
    extern void ne10_radix2_butterfly_backward_float32_neon (ne10_fft_cpx_float32_t * Fout,
            ne10_int32_t * factors,
            ne10_fft_cpx_float32_t * twiddles)
    asm ("ne10_radix2_butterfly_backward_float32_neon");

    extern void ne10_mixed_radix_butterfly_length_even_power2_float32_neon (ne10_fft_cpx_float32_t * Fout,
            ne10_int32_t * factors,
            ne10_fft_cpx_float32_t * twiddles)
    asm ("ne10_mixed_radix_butterfly_length_even_power2_float32_neon");
    extern void ne10_mixed_radix_butterfly_length_odd_power2_float32_neon (ne10_fft_cpx_float32_t * Fout,
            ne10_int32_t * factors,
            ne10_fft_cpx_float32_t * twiddles)
    asm ("ne10_mixed_radix_butterfly_length_odd_power2_float32_neon");

    extern void ne10_mixed_radix_butterfly_inverse_length_even_power2_float32_neon (ne10_fft_cpx_float32_t * Fout,
            ne10_int32_t * factors,
            ne10_fft_cpx_float32_t * twiddles)
    asm ("ne10_mixed_radix_butterfly_inverse_length_even_power2_float32_neon");
    extern void ne10_mixed_radix_butterfly_inverse_length_odd_power2_float32_neon (ne10_fft_cpx_float32_t * Fout,
            ne10_int32_t * factors,
            ne10_fft_cpx_float32_t * twiddles)
    asm ("ne10_mixed_radix_butterfly_inverse_length_odd_power2_float32_neon");

    /* common functions for fixed point fft */
    /* bit reversal for int 16 */
    extern void ne10_data_bitreversal_int16 (ne10_fft_cpx_int16_t * Fout,
            const ne10_fft_cpx_int16_t * f,
            ne10_int32_t fstride,
            ne10_int32_t * factors);
    extern void ne10_fft_split_r2c_1d_int16 (ne10_fft_cpx_int16_t *dst,
            const ne10_fft_cpx_int16_t *src,
            ne10_fft_cpx_int16_t *twiddles,
            ne10_int32_t ncfft);
    extern void ne10_fft_split_c2r_1d_int16 (ne10_fft_cpx_int16_t *dst,
            const ne10_fft_cpx_int16_t *src,
            ne10_fft_cpx_int16_t *twiddles,
            ne10_int32_t ncfft);

    /* butterfly for int 16 */
    extern void ne10_mixed_radix_butterfly_int16_neon (ne10_fft_cpx_int16_t * Fout,
            ne10_int32_t * factors,
            ne10_fft_cpx_int16_t * twiddles)
    asm ("ne10_mixed_radix_butterfly_int16_neon");
    extern void ne10_mixed_radix_butterfly_inverse_int16_neon (ne10_fft_cpx_int16_t * Fout,
            ne10_int32_t * factors,
            ne10_fft_cpx_int16_t * twiddles)
    asm ("ne10_mixed_radix_butterfly_inverse_int16_neon");

    /* bit reversal for int 32 */
    extern void ne10_data_bitreversal_int32 (ne10_fft_cpx_int32_t * Fout,
            const ne10_fft_cpx_int32_t * f,
            ne10_int32_t fstride,
            ne10_int32_t * factors);
    extern void ne10_fft_split_r2c_1d_int32 (ne10_fft_cpx_int32_t *dst,
            const ne10_fft_cpx_int32_t *src,
            ne10_fft_cpx_int32_t *twiddles,
            ne10_int32_t ncfft);
    extern void ne10_fft_split_c2r_1d_int32 (ne10_fft_cpx_int32_t *dst,
            const ne10_fft_cpx_int32_t *src,
            ne10_fft_cpx_int32_t *twiddles,
            ne10_int32_t ncfft);

    /* butterfly for int 32 */
    extern void ne10_mixed_radix_butterfly_forward_int32_unscaled_neon (ne10_fft_cpx_int32_t * Fout,
            ne10_int32_t * factors,
            ne10_fft_cpx_int32_t * twiddles)
    asm ("ne10_mixed_radix_butterfly_forward_int32_unscaled_neon");
    extern void ne10_mixed_radix_butterfly_backward_int32_unscaled_neon (ne10_fft_cpx_int32_t * Fout,
            ne10_int32_t * factors,
            ne10_fft_cpx_int32_t * twiddles)
    asm ("ne10_mixed_radix_butterfly_backward_int32_unscaled_neon");

    extern void ne10_mixed_radix_butterfly_forward_int32_scaled_neon (ne10_fft_cpx_int32_t * Fout,
            ne10_int32_t * factors,
            ne10_fft_cpx_int32_t * twiddles)
    asm ("ne10_mixed_radix_butterfly_forward_int32_scaled_neon");
    extern void ne10_mixed_radix_butterfly_backward_int32_scaled_neon (ne10_fft_cpx_int32_t * Fout,
            ne10_int32_t * factors,
            ne10_fft_cpx_int32_t * twiddles)
    asm ("ne10_mixed_radix_butterfly_backward_int32_scaled_neon");

    extern void ne10_radix4_butterfly_forward_int32_unscaled_neon (ne10_fft_cpx_int32_t * Fout,
            ne10_int32_t * factors,
            ne10_fft_cpx_int32_t * twiddles)
    asm ("ne10_radix4_butterfly_forward_int32_unscaled_neon");
    extern void ne10_radix4_butterfly_backward_int32_unscaled_neon (ne10_fft_cpx_int32_t * Fout,
            ne10_int32_t * factors,
            ne10_fft_cpx_int32_t * twiddles)
    asm ("ne10_radix4_butterfly_backward_int32_unscaled_neon");

    extern void ne10_radix2_butterfly_forward_int32_unscaled_neon (ne10_fft_cpx_int32_t * Fout,
            ne10_int32_t * factors,
            ne10_fft_cpx_int32_t * twiddles)
    asm ("ne10_radix2_butterfly_forward_int32_unscaled_neon");
    extern void ne10_radix2_butterfly_backward_int32_unscaled_neon (ne10_fft_cpx_int32_t * Fout,
            ne10_int32_t * factors,
            ne10_fft_cpx_int32_t * twiddles)
    asm ("ne10_radix2_butterfly_backward_int32_unscaled_neon");

    extern void ne10_radix4_butterfly_forward_int32_scaled_neon (ne10_fft_cpx_int32_t * Fout,
            ne10_int32_t * factors,
            ne10_fft_cpx_int32_t * twiddles)
    asm ("ne10_radix4_butterfly_forward_int32_scaled_neon");
    extern void ne10_radix4_butterfly_backward_int32_scaled_neon (ne10_fft_cpx_int32_t * Fout,
            ne10_int32_t * factors,
            ne10_fft_cpx_int32_t * twiddles)
    asm ("ne10_radix4_butterfly_backward_int32_scaled_neon");

    extern void ne10_radix2_butterfly_forward_int32_scaled_neon (ne10_fft_cpx_int32_t * Fout,
            ne10_int32_t * factors,
            ne10_fft_cpx_int32_t * twiddles)
    asm ("ne10_radix2_butterfly_forward_int32_scaled_neon");
    extern void ne10_radix2_butterfly_backward_int32_scaled_neon (ne10_fft_cpx_int32_t * Fout,
            ne10_int32_t * factors,
            ne10_fft_cpx_int32_t * twiddles)
    asm ("ne10_radix2_butterfly_backward_int32_scaled_neon");

#ifdef __cplusplus
}
#endif

#endif
