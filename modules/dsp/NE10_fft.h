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

/*
 * NE10 Library : dsp/NE10_fft.h
 */

#include "NE10_dsp.h"
#include "NE10_types.h"
#include "NE10_fft_common_varibles.h"
#include "NE10_fft_cplx_ops.h"
#include "NE10_fft_bfly.h"
#include "NE10_fft_debug_macro.h"

#ifndef NE10_FFT_H
#define NE10_FFT_H

///////////////////////////
// Internal macro define
///////////////////////////
#define NE10_FFT_BYTE_ALIGNMENT 8
#define NE10_INLINE inline static

/*
 * FFT Algorithm Flags
 *
 * These are used within Ne10 to decide, after factoring an FFT into stages, what
 * FFT algorithm should be used.
 *
 * - NE10_FFT_ALG_DEFAULT is a mixed radix 2/4 algorithm.
 * - NE10_FFT_ALG_ANY is designated specifically for non-power-of-two input sizes.
 */
#define NE10_FFT_ALG_DEFAULT  0
#define NE10_FFT_ALG_ANY      1

/*
 * FFT Factor Flags
 *
 * These are used within Ne10 to decide how an input FFT size should be factored into
 * stages (i.e. what radices should be used).
 *
 * - NE10_FACTOR_DEFAULT factors into 2, 3, 4, 5.
 * - NE10_FACTOR_EIGHT_FIRST_STAGE is NE10_FACTOR_DEFAULT with the extended ability to
 *   have a radix-8 initial stage.
 * - NE10_FACTOR_EIGHT factors into 2, 3, 4, 5, 8.
 */
#define NE10_FACTOR_DEFAULT             0
#define NE10_FACTOR_EIGHT_FIRST_STAGE   1
#define NE10_FACTOR_EIGHT               2

// Comment when do not want to scale output result
#define NE10_DSP_RFFT_SCALING
#define NE10_DSP_CFFT_SCALING

#define NE10_FFT_PARA_LEVEL 4

#ifdef __cplusplus
extern "C" {
#endif

///////////////////////////
// function prototypes:
///////////////////////////

    /*common fft functions */
    extern ne10_int32_t ne10_factor (ne10_int32_t n, ne10_int32_t * facbuf, ne10_int32_t ne10_factor_flags);

    extern ne10_fft_cpx_float32_t* ne10_fft_generate_twiddles_float32 (ne10_fft_cpx_float32_t * twiddles,
        const ne10_int32_t * factors,
        const ne10_int32_t nfft );

    extern ne10_fft_cpx_float32_t* ne10_fft_generate_twiddles_transposed_float32 (
        ne10_fft_cpx_float32_t * twiddles,
        const ne10_int32_t * factors,
        const ne10_int32_t nfft );

    extern ne10_fft_cpx_int32_t* ne10_fft_generate_twiddles_int32 (ne10_fft_cpx_int32_t * twiddles,
        const ne10_int32_t * factors,
        const ne10_int32_t nfft );

    /*common functions for float fft */
    extern void ne10_mixed_radix_generic_butterfly_float32_c (ne10_fft_cpx_float32_t * Fout,
            const ne10_fft_cpx_float32_t * Fin,
            const ne10_int32_t * factors,
            const ne10_fft_cpx_float32_t * twiddles,
            ne10_fft_cpx_float32_t * buffer,
            const ne10_int32_t is_scaled);

    extern void ne10_mixed_radix_generic_butterfly_inverse_float32_c (ne10_fft_cpx_float32_t * Fout,
            const ne10_fft_cpx_float32_t * Fin,
            const ne10_int32_t * factors,
            const ne10_fft_cpx_float32_t * twiddles,
            ne10_fft_cpx_float32_t * buffer,
            const ne10_int32_t is_scaled);

    extern void ne10_mixed_radix_generic_butterfly_float32_neon (ne10_fft_cpx_float32_t * Fout,
            const ne10_fft_cpx_float32_t * Fin,
            const ne10_int32_t * factors,
            const ne10_fft_cpx_float32_t * twiddles,
            ne10_fft_cpx_float32_t * buffer,
            const ne10_int32_t is_scaled);

    extern void ne10_mixed_radix_generic_butterfly_inverse_float32_neon (ne10_fft_cpx_float32_t * Fout,
            const ne10_fft_cpx_float32_t * Fin,
            const ne10_int32_t * factors,
            const ne10_fft_cpx_float32_t * twiddles,
            ne10_fft_cpx_float32_t * buffer,
            const ne10_int32_t is_scaled);

    extern void ne10_mixed_radix_fft_forward_float32_neon (ne10_fft_cpx_float32_t * Fout,
            ne10_fft_cpx_float32_t * Fin,
            ne10_int32_t * factors,
            ne10_fft_cpx_float32_t * twiddles,
            ne10_fft_cpx_float32_t * buffer)
    asm ("ne10_mixed_radix_fft_forward_float32_neon");

    extern void ne10_mixed_radix_fft_backward_float32_neon (ne10_fft_cpx_float32_t * Fout,
            ne10_fft_cpx_float32_t * Fin,
            ne10_int32_t * factors,
            ne10_fft_cpx_float32_t * twiddles,
            ne10_fft_cpx_float32_t * buffer)
    asm ("ne10_mixed_radix_fft_backward_float32_neon");

    /* common functions for fixed point fft */
    /* butterfly for int 32 */
    extern void ne10_mixed_radix_generic_butterfly_int32_c (ne10_fft_cpx_int32_t * Fout,
            const ne10_fft_cpx_int32_t * Fin,
            const ne10_int32_t * factors,
            const ne10_fft_cpx_int32_t * twiddles,
            ne10_fft_cpx_int32_t * buffer,
            const ne10_int32_t scaled_flag);

    extern void ne10_mixed_radix_generic_butterfly_inverse_int32_c (ne10_fft_cpx_int32_t * Fout,
            const ne10_fft_cpx_int32_t * Fin,
            const ne10_int32_t * factors,
            const ne10_fft_cpx_int32_t * twiddles,
            ne10_fft_cpx_int32_t * buffer,
            const ne10_int32_t scaled_flag);

    extern void ne10_mixed_radix_generic_butterfly_int32_neon (ne10_fft_cpx_int32_t * Fout,
            const ne10_fft_cpx_int32_t * Fin,
            const ne10_int32_t * factors,
            const ne10_fft_cpx_int32_t * twiddles,
            ne10_fft_cpx_int32_t * buffer,
            const ne10_int32_t scaled_flag);

    extern void ne10_mixed_radix_generic_butterfly_inverse_int32_neon (ne10_fft_cpx_int32_t * Fout,
            const ne10_fft_cpx_int32_t * Fin,
            const ne10_int32_t * factors,
            const ne10_fft_cpx_int32_t * twiddles,
            ne10_fft_cpx_int32_t * buffer,
            const ne10_int32_t scaled_flag);

    extern void ne10_mixed_radix_fft_forward_int32_unscaled_neon (ne10_fft_cpx_int32_t * Fout,
            ne10_fft_cpx_int32_t *fin,
            ne10_int32_t * factors,
            ne10_fft_cpx_int32_t * twiddles,
            ne10_fft_cpx_int32_t * buffer)
    asm ("ne10_mixed_radix_fft_forward_int32_unscaled_neon");
    extern void ne10_mixed_radix_fft_backward_int32_unscaled_neon (ne10_fft_cpx_int32_t * Fout,
            ne10_fft_cpx_int32_t *fin,
            ne10_int32_t * factors,
            ne10_fft_cpx_int32_t * twiddles,
            ne10_fft_cpx_int32_t * buffer)
    asm ("ne10_mixed_radix_fft_backward_int32_unscaled_neon");
    extern void ne10_mixed_radix_fft_forward_int32_scaled_neon (ne10_fft_cpx_int32_t * Fout,
            ne10_fft_cpx_int32_t *fin,
            ne10_int32_t * factors,
            ne10_fft_cpx_int32_t * twiddles,
            ne10_fft_cpx_int32_t * buffer)
    asm ("ne10_mixed_radix_fft_forward_int32_scaled_neon");
    extern void ne10_mixed_radix_fft_backward_int32_scaled_neon (ne10_fft_cpx_int32_t * Fout,
            ne10_fft_cpx_int32_t *fin,
            ne10_int32_t * factors,
            ne10_fft_cpx_int32_t * twiddles,
            ne10_fft_cpx_int32_t * buffer)
    asm ("ne10_mixed_radix_fft_backward_int32_scaled_neon");

    /* butterfly for int 16 */
    extern void ne10_mixed_radix_fft_forward_int16_unscaled_neon (ne10_fft_cpx_int16_t * Fout,
            ne10_fft_cpx_int16_t *fin,
            ne10_int32_t * factors,
            ne10_fft_cpx_int16_t * twiddles,
            ne10_fft_cpx_int16_t * buffer)
    asm ("ne10_mixed_radix_fft_forward_int16_unscaled_neon");
    extern void ne10_mixed_radix_fft_backward_int16_unscaled_neon (ne10_fft_cpx_int16_t * Fout,
            ne10_fft_cpx_int16_t *fin,
            ne10_int32_t * factors,
            ne10_fft_cpx_int16_t * twiddles,
            ne10_fft_cpx_int16_t * buffer)
    asm ("ne10_mixed_radix_fft_backward_int16_unscaled_neon");
    extern void ne10_mixed_radix_fft_forward_int16_scaled_neon (ne10_fft_cpx_int16_t * Fout,
            ne10_fft_cpx_int16_t *fin,
            ne10_int32_t * factors,
            ne10_fft_cpx_int16_t * twiddles,
            ne10_fft_cpx_int16_t * buffer)
    asm ("ne10_mixed_radix_fft_forward_int16_scaled_neon");
    extern void ne10_mixed_radix_fft_backward_int16_scaled_neon (ne10_fft_cpx_int16_t * Fout,
            ne10_fft_cpx_int16_t *fin,
            ne10_int32_t * factors,
            ne10_fft_cpx_int16_t * twiddles,
            ne10_fft_cpx_int16_t * buffer)
    asm ("ne10_mixed_radix_fft_backward_int16_scaled_neon");

#if (NE10_UNROLL_LEVEL > 0)
    extern void ne10_radix4_r2c_c (ne10_fft_cpx_float32_t *Fout,
            const ne10_fft_cpx_float32_t *Fin,
            const ne10_int32_t fstride,
            const ne10_int32_t mstride,
            const ne10_int32_t nfft);

    extern void ne10_radix4_c2r_c (ne10_fft_cpx_float32_t *Fout,
            const ne10_fft_cpx_float32_t *Fin,
            const ne10_int32_t fstride,
            const ne10_int32_t mstride,
            const ne10_int32_t nfft);

    extern void ne10_radix8_r2c_c (ne10_fft_cpx_float32_t *Fout,
            const ne10_fft_cpx_float32_t *Fin,
            const ne10_int32_t fstride,
            const ne10_int32_t mstride,
            const ne10_int32_t nfft);

    extern void ne10_radix8_c2r_c (ne10_fft_cpx_float32_t *Fout,
            const ne10_fft_cpx_float32_t *Fin,
            const ne10_int32_t fstride,
            const ne10_int32_t mstride,
            const ne10_int32_t nfft);
#endif // NE10_UNROLL_LEVEL

#ifdef __cplusplus
}
#endif

#endif
