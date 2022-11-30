/*
 *  Copyright 2012-16 ARM Limited and Contributors.
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
 * NE10 Library : inc/NE10_dsp.h
 */


#include "NE10_types.h"

#ifndef NE10_DSP_H
#define NE10_DSP_H

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @defgroup C2C_FFT_IFFT Complex-to-Complex FFT (Floating & Fixed Point)
 * @ingroup groupDSPs
 * @{
 *
 * \par
 * A [Fast Fourier Transform (FFT)](https://en.wikipedia.org/wiki/Fast_Fourier_transform) is an efficient method
 * of computing the [Discrete Fourier Transform (DFT)](https://en.wikipedia.org/wiki/Discrete_Fourier_transform),
 * or its inverse. This set of functions implement complex-to-complex 1D FFTs, and the complementary
 * inverse transforms (IFFTs).
 *
 * \par
 * Where scaling controls are not provided, these functions scale the data by \f$1/N\f$ in the inverse transformation only. This
 * is a common convention, but may contrast with typical usage in some fields, where this scale factor may instead be applied in
 * the forwards transformation only, or where a scale factor of \f$1/\sqrt{N}\f$ may be applied in both transformations.
 *
 * \par
 * Separate versions are provided for floating point (FP32) and fixed point (Q31, Q15) data, each with
 * a plain C implementation, a NEON implementation, and a function pointer to select between these at runtime (see @ref ne10_init). More detailed
 * descriptions of these functions can be found alongside the relevant function pointers:
 *
 * \par
 * - FP32: @ref ne10_fft_c2c_1d_float32
 * - INT32 (Q31): @ref ne10_fft_c2c_1d_int32
 * - INT16 (Q15): @ref ne10_fft_c2c_1d_int16
 *
 * \par Supported lengths
 * Internally, the functions primarily utilize a mixed radix 2/4 decimation-in-time algorithm, supporting input sizes of the form 2<sup>N</sup>
 * (for integers N > 0). Some functions also support non-power-of-two input sizes using additional radices, and this is noted in the
 * relevant function descriptions where it applies.
 *
 * \par Memory layout
 * These functions operate out-of-place, using different buffers for their input and output. An additional temporary buffer is
 * also used internally to store intermediate results. The input and output buffers should be allocated by the user, and must
 * be of a size greater than or equal to `(fftSize * sizeof(ne10_fft_cpx_float32_t))` bytes, while the temporary buffer is
 * reserved during allocation of a "configuration structure" (for instance, via @ref ne10_fft_alloc_c2c_float32).
 *
 * \par
 * Input and output buffers interleave the real and imaginary parts of each complex element, storing these contiguously.
 * <pre>Format: { Re[0], Im[0], Re[1], Im[1], Re[2], Im[2], ..., Re[fftSize - 2], Im[fftSize - 2], Re[fftSize - 1], Im[fftSize - 1] }</pre>
 *
 *
 * \par Example usage
 * A single precision floating point FFT/IFFT example code snippet follows.
 *
 * \par
 * ```c
 * #include "NE10.h"
 *
 * ...
 *
 * {
 *     ne10_int32_t fftSize = 1024;
 *     ne10_fft_cfg_float32_t cfg = ne10_fft_alloc_c2c_float32_neon(fftSize);          // Allocate a configuration structure for C2C FP32 NEON FFTs of size fftSize
 *     ne10_fft_cpx_float32_t *in  = malloc(fftSize * sizeof(ne10_fft_cpx_float32_t)); // Allocate an input array of samples
 *     ne10_fft_cpx_float32_t *out = malloc(fftSize * sizeof(ne10_fft_cpx_float32_t)); // Allocate an output array of samples
 *
 *     ...
 *
 *     // FFT
 *     ne10_fft_c2c_1d_float32_neon(out, in, cfg, 0); // Call the FP32 C2C NEON implementation directly
 *
 *     ...
 *
 *     // IFFT
 *     ne10_fft_c2c_1d_float32_neon(out, in, cfg, 1); // Call the FP32 C2C NEON implementation directly (reusing the configuration structure!)
 *
 *     ...
 *
 *     NE10_FREE(out);                    // Free the allocated input array
 *     NE10_FREE(in);                     // Free the allocated output array
 *     ne10_fft_destroy_c2c_float32(cfg); // Free the allocated configuration structure
 * }
 * ```
 *
 * \par
 * The @ref ne10_fft_cfg_float32_t variable `cfg` is a pointer to a configuration structure. **For different inputs of the same size,
 * the same configuration structure can** (and should, where possible) **be reused**.
 *
 * \par
 * The contents of these configuration structures include:
 *
 * \par
 * - `cfg->twiddles`: a pointer to a table of "twiddle factors" that are used in computing the FFT/IFFT
 * - `cfg->factors`: a buffer of "factors", which suggests to the core algorithm how the input can be broken down into smaller calculations
 * - `cfg->buffer`: a pointer to a temporary buffer used internally in calculations that is allocated when the configuration
 *                  structure is set up
 */

    /**
     * @brief Creates a configuration structure for @ref ne10_fft_c2c_1d_float32.
     *
     * @param[in]   nfft             input length
     * @retval      st               pointer to an FFT configuration structure (allocated with `malloc`), or `NULL` to indicate an error
     *
     * Allocates and initialises an @ref ne10_fft_cfg_float32_t configuration structure for
     * the complex-to-complex single precision floating point FFT/IFFT. As part of this, it reserves
     * a buffer used internally by the FFT algorithm, factors the length of the FFT into simpler chunks,
     * and generates a "twiddle table" of coefficients used in the FFT "butterfly" calculations.
     * Points to @ref ne10_fft_alloc_c2c_float32_c or @ref ne10_fft_alloc_c2c_float32_neon.
     *
     * To free the returned structure, call @ref ne10_fft_destroy_c2c_float32.
     */
    extern ne10_fft_cfg_float32_t (*ne10_fft_alloc_c2c_float32) (ne10_int32_t nfft);
    extern ne10_fft_cfg_float32_t ne10_fft_alloc_c2c_float32_c (ne10_int32_t nfft);
    extern ne10_fft_cfg_float32_t ne10_fft_alloc_c2c_float32_neon (ne10_int32_t nfft);

    /**
     * @brief Mixed radix-2/3/4/5 complex FFT/IFFT of single precision floating point data.
     *
     * @param[out]  *fout            pointer to the output buffer
     * @param[in]   *fin             pointer to the input buffer
     * @param[in]   cfg              pointer to the configuration structure (allocated via a variant of @ref ne10_fft_alloc_c2c_float32)
     * @param[in]   inverse_fft      whether this is an FFT or IFFT (0: FFT, 1: IFFT)
     *
     * Implements a mixed radix-2/3/4/5 complex FFT/IFFT, supporting input lengths of the
     * form 2<sup>N</sup>*3<sup>M</sup>*5<sup>K</sup> (N > 0). Points to
     * @ref ne10_fft_c2c_1d_float32_c or @ref ne10_fft_c2c_1d_float32_neon, the
     * latter of which has the additional constraint that `fftSize % 4 == 0` must
     * hold if fftSize > 2. This is an out-of-place algorithm. For usage
     * information, please check test/test_suite_fft_float32.c.
     */
    extern void (*ne10_fft_c2c_1d_float32) (ne10_fft_cpx_float32_t *fout,
                                            ne10_fft_cpx_float32_t *fin,
                                            ne10_fft_cfg_float32_t cfg,
                                            ne10_int32_t inverse_fft);
    extern void ne10_fft_c2c_1d_float32_c (ne10_fft_cpx_float32_t *fout,
                                           ne10_fft_cpx_float32_t *fin,
                                           ne10_fft_cfg_float32_t cfg,
                                           ne10_int32_t inverse_fft);
    extern void ne10_fft_c2c_1d_float32_neon (ne10_fft_cpx_float32_t *fout,
            ne10_fft_cpx_float32_t *fin,
            ne10_fft_cfg_float32_t cfg,
            ne10_int32_t inverse_fft);

    extern void ne10_fft_destroy_c2c_float32 (ne10_fft_cfg_float32_t);

    /**
     * @brief Creates a configuration structure for @ref ne10_fft_c2c_1d_int32.
     *
     * @param[in]   nfft             input length
     * @retval      st               pointer to an FFT configuration structure (allocated with `malloc`), or `NULL` to indicate an error
     *
     * Allocates and initialises an @ref ne10_fft_cfg_int32_t configuration structure for
     * the complex-to-complex 32-bit integer fixed point FFT/IFFT. As part of this, it reserves a
     * buffer used internally by the FFT algorithm, factors the length of the FFT into simpler chunks,
     * and generates a "twiddle table" of coefficients used in the FFT "butterfly" calculations.
     * Points to @ref ne10_fft_alloc_c2c_int32_c or @ref ne10_fft_alloc_c2c_int32_neon.
     *
     * To free the returned structure, call @ref ne10_fft_destroy_c2c_int32.
     */
    extern ne10_fft_cfg_int32_t (*ne10_fft_alloc_c2c_int32) (ne10_int32_t nfft);
    extern ne10_fft_cfg_int32_t ne10_fft_alloc_c2c_int32_c (ne10_int32_t nfft);
    extern ne10_fft_cfg_int32_t ne10_fft_alloc_c2c_int32_neon (ne10_int32_t nfft);

    /**
     * @brief Mixed radix-2/4 complex FFT/IFFT of 32-bit fixed point Q31 data.
     *
     * @param[out]  *fout            pointer to the output buffer
     * @param[in]   *fin             pointer to the input buffer
     * @param[in]   cfg              pointer to the configuration structure (allocated via a variant of @ref ne10_fft_alloc_c2c_int32)
     * @param[in]   inverse_fft      whether this is an FFT or IFFT (0: FFT, 1: IFFT)
     * @param[in]   scaled_flag      scale flag (0: unscaled, 1: scaled)
     *
     * Implements a mixed radix-2/4 complex FFT/IFFT, supporting input lengths of the
     * form 2<sup>N</sup> (N > 0). Points to @ref ne10_fft_c2c_1d_int32_c or
     * @ref ne10_fft_c2c_1d_int32_neon. This is an out-of-place algorithm.
     * For usage information, please check test/test_suite_fft_int32.c.
     */
    extern void (*ne10_fft_c2c_1d_int32) (ne10_fft_cpx_int32_t *fout,
                                          ne10_fft_cpx_int32_t *fin,
                                          ne10_fft_cfg_int32_t cfg,
                                          ne10_int32_t inverse_fft,
                                          ne10_int32_t scaled_flag);
    extern void ne10_fft_c2c_1d_int32_c (ne10_fft_cpx_int32_t *fout,
                                         ne10_fft_cpx_int32_t *fin,
                                         ne10_fft_cfg_int32_t cfg,
                                         ne10_int32_t inverse_fft,
                                         ne10_int32_t scaled_flag);
    extern void ne10_fft_c2c_1d_int32_neon (ne10_fft_cpx_int32_t *fout,
                                            ne10_fft_cpx_int32_t *fin,
                                            ne10_fft_cfg_int32_t cfg,
                                            ne10_int32_t inverse_fft,
                                            ne10_int32_t scaled_flag);

    extern void ne10_fft_destroy_c2c_int32 (ne10_fft_cfg_int32_t);


    extern ne10_fft_cfg_int16_t ne10_fft_alloc_c2c_int16 (ne10_int32_t nfft);

    /**
     * @brief Mixed radix-2/4 complex FFT/IFFT of 16-bit fixed point Q15 data.
     * @param[out]  *fout            pointer to the output buffer
     * @param[in]   *fin             pointer to the input buffer
     * @param[in]   cfg              pointer to the configuration structure (allocated via @ref ne10_fft_alloc_c2c_int16)
     * @param[in]   inverse_fft      whether this is an FFT or IFFT (0: FFT, 1: IFFT)
     * @param[in]   scaled_flag      scale flag (0: unscaled, 1: scaled)
     *
     * Implements a mixed radix-2/4 complex FFT/IFFT, supporting input lengths of the
     * form 2<sup>N</sup> (N > 0). Points to @ref ne10_fft_c2c_1d_int16_c or
     * @ref ne10_fft_c2c_1d_int16_neon. This is an out-of-place algorithm.
     * For usage information, please check test/test_suite_fft_int16.c.
     */
    extern void (*ne10_fft_c2c_1d_int16) (ne10_fft_cpx_int16_t *fout,
                                          ne10_fft_cpx_int16_t *fin,
                                          ne10_fft_cfg_int16_t cfg,
                                          ne10_int32_t inverse_fft,
                                          ne10_int32_t scaled_flag);
    extern void ne10_fft_c2c_1d_int16_c (ne10_fft_cpx_int16_t *fout,
                                         ne10_fft_cpx_int16_t *fin,
                                         ne10_fft_cfg_int16_t cfg,
                                         ne10_int32_t inverse_fft,
                                         ne10_int32_t scaled_flag);
    extern void ne10_fft_c2c_1d_int16_neon (ne10_fft_cpx_int16_t *fout,
                                            ne10_fft_cpx_int16_t *fin,
                                            ne10_fft_cfg_int16_t cfg,
                                            ne10_int32_t inverse_fft,
                                            ne10_int32_t scaled_flag);

    extern void ne10_fft_destroy_c2c_int16 (ne10_fft_cfg_int16_t);
/** @} */ // C2C_FFT_IFFT

/**
 * @ingroup groupDSPs
 * @defgroup R2C_FFT_IFFT Real-to-Complex FFT (Floating & Fixed Point)
 * @{
 *
 * \par
 * In practical applications, it is often the case that full @link C2C_FFT_IFFT complex-to-complex
 * FFT operations@endlink are unnecessary; many applications deal exclusively with real signals. For this
 * reason, Ne10 provides real-to-complex and complex-to-real 1D FFT/IFFT operations.
 *
 * \par
 * As with their complex-to-complex counterparts, in the absence of scaling controls within the function
 * signature these functions scale the data by \f$1/N\f$ in the inverse transformation only.
 *
 * \par
 * Three variants of these FFT/IFFT functions are provided, operating on FP32, Q31, and Q15 data types.
 * Each has a plain C implementation, a NEON implementation, and a function pointer to select between
 * these at runtime (see @ref ne10_init). More detailed descriptions of these functions can be found
 * alongside the relevant function pointers:
 *
 * \par
 * - FP32: @ref ne10_fft_r2c_1d_float32 and @ref ne10_fft_c2r_1d_float32
 * - INT32 (Q31): @ref ne10_fft_r2c_1d_int32 and @ref ne10_fft_c2r_1d_int32
 * - INT16 (Q15): @ref ne10_fft_r2c_1d_int16 and @ref ne10_fft_c2r_1d_int16
 *
 * \par Supported lengths
 * These functions support input sizes of the form 2<sup>N</sup> (for integers N > 0).
 *
 * \par Memory layout
 * These functions operate out-of-place, using different buffers for their input and output. An additional temporary buffer is
 * also used internally to store intermediate results. The input and output buffers should be allocated by the user, while the
 * temporary buffer is reserved during allocation of a "configuration structure" (for instance, via @ref ne10_fft_alloc_r2c_float32).
 *
 * \par
 * Buffers of complex elements interleave the real and imaginary parts of each element, storing these contiguously, while buffers of
 * real elements simply hold values contiguously in memory as simple C arrays. As real-to-complex Fourier transforms display
 * conjugate symmetry, outputs from the forwards transform will be only `(fftSize / 2) + 1` complex elements long. Similarly, inputs
 * to the inverse transform need only be this long.
 * <pre>
 * Real-to-Complex (R2C) FFT:
 *     Input:  { Re[0], Re[1], Re[2], Re[3], Re[4], Re[5], ..., Re[fftSize - 4], Re[fftSize - 3], Re[fftSize - 2], Re[fftSize - 1] }
 *     Output: { Re[0], Im[0], Re[1], Im[1], Re[2], Im[2], ..., Re[(fftSize / 2) - 1], Im[(fftSize / 2) - 1], Re[(fftSize / 2)], Im[(fftSize / 2)] }
 * Complex-to-Real (C2R) IFFT:
 *     Input:  { Re[0], Im[0], Re[1], Im[1], Re[2], Im[2], ..., Re[(fftSize / 2) - 1], Im[(fftSize / 2) - 1], Re[(fftSize / 2)], Im[(fftSize / 2)] }
 *     Output: { Re[0], Re[1], Re[2], Re[3], Re[4], Re[5], ..., Re[fftSize - 4], Re[fftSize - 3], Re[fftSize - 2], Re[fftSize - 1] }
 * </pre>
 *
 * \par Example usage
 * A single precision floating point FFT/IFFT example code snippet follows.
 *
 * \par
 * ```c
 * #include "NE10.h"
 *
 * ...
 *
 * {
 *     ne10_int32_t fftSize = 1024;
 *     ne10_fft_r2c_cfg_float32_t cfg = ne10_fft_alloc_r2c_float32(fftSize);           // Allocate a configuration structure for R2C/C2R FP32 FFTs of size fftSize
 *     ne10_float32_t *in          = malloc(fftSize * sizeof(ne10_float32_t));         // Allocate an input array of samples
 *     ne10_fft_cpx_float32_t *out = malloc(fftSize * sizeof(ne10_fft_cpx_float32_t)); // Allocate an (oversized) output array of samples
 *
 *     ...
 *
 *     // FFT
 *     ne10_fft_r2c_1d_float32_neon(out, in, cfg); // Call the FP32 R2C NEON implementation directly
 *
 *     ...
 *
 *     // IFFT
 *     ne10_fft_c2r_1d_float32_neon(in, out, cfg); // Call the FP32 C2R NEON implementation directly (reusing the configuration structure!)
 *
 *     ...
 *
 *     NE10_FREE(out);                    // Free the allocated input array
 *     NE10_FREE(in);                     // Free the allocated output array
 *     ne10_fft_destroy_r2c_float32(cfg); // Free the allocated configuration structure
 * }
 * ```
 *
 * \par
 * The @ref ne10_fft_r2c_cfg_float32_t variable `cfg` is a pointer to a configuration structure. **For different inputs of the same size,
 * the same configuration structure can** (and should, where possible) **be reused**.
 *
 * \par
 * The contents of these configuration structures include:
 *
 * \par
 * - `cfg->twiddles`: a pointer to a table of "twiddle factors" that are used in computing the FFT/IFFT
 * - `cfg->super_twiddles`: a pointer to a table of twiddle factors used in splitting complex and real components
 * - `cfg->factors`: a buffer of "factors", which suggests to the core algorithm how the input can be broken down into smaller calculations
 * - `cfg->buffer`: a pointer to a temporary buffer used internally in calculations that is allocated when the configuration
 *                  structure is set up
 */

    extern ne10_fft_r2c_cfg_float32_t ne10_fft_alloc_r2c_float32 (ne10_int32_t nfft);

    /**
     * @brief Mixed radix-2/4 real-to-complex FFT of single precision floating point data.
     *
     * @param[out]  *fout            pointer to the output buffer
     * @param[in]   *fin             pointer to the input buffer
     * @param[in]   cfg              pointer to the configuration structure (allocated via @ref ne10_fft_alloc_r2c_float32)
     *
     * The function implements a mixed radix-2/4 real-to-complex FFT, supporting input lengths of
     * the form 2<sup>N</sup> (N > 0). Points to @ref ne10_fft_r2c_1d_float32_c or
     * @ref ne10_fft_r2c_1d_float32_neon. This is an out-of-place algorithm.
     * For usage information, please check test/test_suite_fft_float32.c.
     *
     * For the inverse transform (IFFT), call @ref ne10_fft_c2r_1d_float32.
     */
    extern void (*ne10_fft_r2c_1d_float32) (ne10_fft_cpx_float32_t *fout,
                                            ne10_float32_t *fin,
                                            ne10_fft_r2c_cfg_float32_t cfg);
    extern void ne10_fft_r2c_1d_float32_c (ne10_fft_cpx_float32_t *fout,
                                           ne10_float32_t *fin,
                                           ne10_fft_r2c_cfg_float32_t cfg);
    extern void ne10_fft_r2c_1d_float32_neon (ne10_fft_cpx_float32_t *fout,
            ne10_float32_t *fin,
            ne10_fft_r2c_cfg_float32_t cfg);

    /**
     * @brief Mixed radix-2/4 complex-to-real IFFT of single precision floating point data.
     *
     * @param[out]  *fout            pointer to the output buffer
     * @param[in]   *fin             pointer to the input buffer
     * @param[in]   cfg              pointer to the configuration structure (allocated via @ref ne10_fft_alloc_r2c_float32)
     *
     * The function implements a mixed radix-2/4 complex-to-real IFFT, supporting input lengths of
     * the form 2<sup>N</sup> (N > 0). Points to @ref ne10_fft_c2r_1d_float32_c or
     * @ref ne10_fft_c2r_1d_float32_neon. This is an out-of-place algorithm.
     * For usage information, please check test/test_suite_fft_float32.c.
     */
    extern void (*ne10_fft_c2r_1d_float32) (ne10_float32_t *fout,
                                            ne10_fft_cpx_float32_t *fin,
                                            ne10_fft_r2c_cfg_float32_t cfg);
    extern void ne10_fft_c2r_1d_float32_c (ne10_float32_t *fout,
                                           ne10_fft_cpx_float32_t *fin,
                                           ne10_fft_r2c_cfg_float32_t cfg);
    extern void ne10_fft_c2r_1d_float32_neon (ne10_float32_t *fout,
                                              ne10_fft_cpx_float32_t *fin,
                                              ne10_fft_r2c_cfg_float32_t cfg);

    extern void ne10_fft_destroy_r2c_float32 (ne10_fft_r2c_cfg_float32_t);


    extern ne10_fft_r2c_cfg_int32_t ne10_fft_alloc_r2c_int32 (ne10_int32_t nfft);

    /**
     * @brief Mixed radix-2/4 real-to-complex FFT of 32-bit fixed point Q31 data.
     *
     * @param[out]  *fout            pointer to the output buffer
     * @param[in]   *fin             pointer to the input buffer
     * @param[in]   cfg              pointer to the configuration structure (allocated via @ref ne10_fft_alloc_r2c_int32)
     * @param[in]   scaled_flag      scale flag (0: unscaled, 1: scaled)
     *
     * The function implements a mixed radix-2/4 real-to-complex FFT, supporting input lengths of
     * the form 2<sup>N</sup> (N > 0). Points to @ref ne10_fft_r2c_1d_int32_c or
     * @ref ne10_fft_r2c_1d_int32_neon. This is an out-of-place algorithm.
     * For usage information, please check test/test_suite_fft_int32.c.
     *
     * For the inverse transform (IFFT), call @ref ne10_fft_c2r_1d_int32.
     */
    extern void (*ne10_fft_r2c_1d_int32) (ne10_fft_cpx_int32_t *fout,
                                          ne10_int32_t *fin,
                                          ne10_fft_r2c_cfg_int32_t cfg,
                                          ne10_int32_t scaled_flag);
    extern void ne10_fft_r2c_1d_int32_c (ne10_fft_cpx_int32_t *fout,
                                         ne10_int32_t *fin,
                                         ne10_fft_r2c_cfg_int32_t cfg,
                                         ne10_int32_t scaled_flag);
    extern void ne10_fft_r2c_1d_int32_neon (ne10_fft_cpx_int32_t *fout,
                                            ne10_int32_t *fin,
                                            ne10_fft_r2c_cfg_int32_t cfg,
                                            ne10_int32_t scaled_flag);

    /**
     * @brief Mixed radix-2/4 complex-to-real IFFT of 32-bit fixed point Q31 data.
     *
     * @param[out]  *fout            pointer to the output buffer
     * @param[in]   *fin             pointer to the input buffer
     * @param[in]   cfg              pointer to the configuration structure (allocated via @ref ne10_fft_alloc_r2c_int32)
     * @param[in]   scaled_flag      scale flag (0: unscaled, 1: scaled)
     *
     * The function implements a mixed radix-2/4 complex-to-real IFFT, supporting input lengths of
     * the form 2<sup>N</sup> (N > 0). Points to @ref ne10_fft_c2r_1d_int32_c or
     * @ref ne10_fft_c2r_1d_int32_neon. This is an out-of-place algorithm.
     * For usage information, please check test/test_suite_fft_int32.c.
     */
    extern void (*ne10_fft_c2r_1d_int32) (ne10_int32_t *fout,
                                          ne10_fft_cpx_int32_t *fin,
                                          ne10_fft_r2c_cfg_int32_t cfg,
                                          ne10_int32_t scaled_flag);
    extern void ne10_fft_c2r_1d_int32_c (ne10_int32_t *fout,
                                         ne10_fft_cpx_int32_t *fin,
                                         ne10_fft_r2c_cfg_int32_t cfg,
                                         ne10_int32_t scaled_flag);
    extern void ne10_fft_c2r_1d_int32_neon (ne10_int32_t *fout,
                                            ne10_fft_cpx_int32_t *fin,
                                            ne10_fft_r2c_cfg_int32_t cfg,
                                            ne10_int32_t scaled_flag);

    extern void ne10_fft_destroy_r2c_int32 (ne10_fft_r2c_cfg_int32_t);


    extern ne10_fft_r2c_cfg_int16_t ne10_fft_alloc_r2c_int16 (ne10_int32_t nfft);

    /**
     * @brief Mixed radix-2/4 real-to-complex FFT of 16-bit fixed point Q15 data.
     *
     * @param[out]  *fout            pointer to the output buffer
     * @param[in]   *fin             pointer to the input buffer
     * @param[in]   cfg              pointer to the configuration structure (allocated via @ref ne10_fft_alloc_r2c_int16)
     * @param[in]   scaled_flag      scale flag (0: unscaled, 1: scaled)
     *
     * The function implements a mixed radix-2/4 real-to-complex FFT, supporting input lengths of
     * the form 2<sup>N</sup> (N > 0). Points to @ref ne10_fft_r2c_1d_int16_c or
     * @ref ne10_fft_r2c_1d_int16_neon. This is an out-of-place algorithm.
     * For usage information, please check test/test_suite_fft_int16.c.
     *
     * For the inverse transform (IFFT), call @ref ne10_fft_c2r_1d_int16.
     */
    extern void (*ne10_fft_r2c_1d_int16) (ne10_fft_cpx_int16_t *fout,
                                          ne10_int16_t *fin,
                                          ne10_fft_r2c_cfg_int16_t cfg,
                                          ne10_int32_t scaled_flag);
    extern void ne10_fft_r2c_1d_int16_c (ne10_fft_cpx_int16_t *fout,
                                         ne10_int16_t *fin,
                                         ne10_fft_r2c_cfg_int16_t cfg,
                                         ne10_int32_t scaled_flag);
    extern void ne10_fft_r2c_1d_int16_neon (ne10_fft_cpx_int16_t *fout,
                                            ne10_int16_t *fin,
                                            ne10_fft_r2c_cfg_int16_t cfg,
                                            ne10_int32_t scaled_flag);

    /**
     * @brief Mixed radix-2/4 complex-to-real IFFT of 16-bit fixed point Q15 data.
     *
     * @param[out]  *fout            pointer to the output buffer
     * @param[in]   *fin             pointer to the input buffer
     * @param[in]   cfg              pointer to the configuration structure (allocated via @ref ne10_fft_alloc_r2c_int16)
     * @param[in]   scaled_flag      scale flag (0: unscaled, 1: scaled)
     *
     * The function implements a mixed radix-2/4 complex-to-real IFFT, supporting input lengths of
     * the form 2<sup>N</sup> (N > 0). Points to @ref ne10_fft_c2r_1d_int16_c or
     * @ref ne10_fft_c2r_1d_int16_neon. This is an out-of-place algorithm.
     * For usage information, please check test/test_suite_fft_int16.c.
     */
    extern void (*ne10_fft_c2r_1d_int16) (ne10_int16_t *fout,
                                          ne10_fft_cpx_int16_t *fin,
                                          ne10_fft_r2c_cfg_int16_t cfg,
                                          ne10_int32_t scaled_flag);
    extern void ne10_fft_c2r_1d_int16_c (ne10_int16_t *fout,
                                         ne10_fft_cpx_int16_t *fin,
                                         ne10_fft_r2c_cfg_int16_t cfg,
                                         ne10_int32_t scaled_flag);
    extern void ne10_fft_c2r_1d_int16_neon (ne10_int16_t *fout,
                                            ne10_fft_cpx_int16_t *fin,
                                            ne10_fft_r2c_cfg_int16_t cfg,
                                            ne10_int32_t scaled_flag);

    extern void ne10_fft_destroy_r2c_int16 (ne10_fft_r2c_cfg_int16_t);
/** @} */ // R2C_FFT_IFFT

    /**
     * @addtogroup FIR
     * @{
     */
    extern ne10_result_t ne10_fir_init_float (ne10_fir_instance_f32_t * S,
            ne10_uint16_t numTaps,
            ne10_float32_t * pCoeffs,
            ne10_float32_t * pState,
            ne10_uint32_t blockSize);

    /**
     * @param[in]  *S points to an instance of the floating-point FIR filter structure.
     * @param[in]  *pSrc points to the block of input data.
     * @param[out] *pDst points to the block of output data.
     * @param[in]  blockSize number of samples to process per call.
     *
     * Points to @ref ne10_fir_float_c or @ref ne10_fir_float_neon.
     */
    extern void (*ne10_fir_float) (const ne10_fir_instance_f32_t * S,
                                   ne10_float32_t * pSrc,
                                   ne10_float32_t * pDst,
                                   ne10_uint32_t blockSize);
    extern void ne10_fir_float_c (const ne10_fir_instance_f32_t * S,
                                  ne10_float32_t * pSrc,
                                  ne10_float32_t * pDst,
                                  ne10_uint32_t blockSize);
#ifdef ENABLE_NE10_FIR_FLOAT_NEON
    /**
     * Specific implementation of @ref ne10_fir_float using NEON SIMD capabilities.
     */
    extern void ne10_fir_float_neon (const ne10_fir_instance_f32_t * S,
                                     ne10_float32_t * pSrc,
                                     ne10_float32_t * pDst,
                                     ne10_uint32_t blockSize)
    asm ("ne10_fir_float_neon");
#endif // ENABLE_NE10_FIR_FLOAT_NEON
    /** @} */ // FIR

    /**
     * @addtogroup FIR_Decimate
     * @{
     */
    extern ne10_result_t ne10_fir_decimate_init_float (ne10_fir_decimate_instance_f32_t * S,
            ne10_uint16_t numTaps,
            ne10_uint8_t M,
            ne10_float32_t * pCoeffs,
            ne10_float32_t * pState,
            ne10_uint32_t blockSize);

    /**
     * @brief Processing function for the floating-point FIR decimator.
     *
     * @param[in] *S        points to an instance of the floating-point FIR decimator structure.
     * @param[in] *pSrc     points to the block of input data.
     * @param[out] *pDst    points to the block of output data.
     * @param[in] blockSize number of input samples to process per call.
     *
     * Points to @ref ne10_fir_decimate_float_c or @ref ne10_fir_decimate_float_neon.
     */
    extern void (*ne10_fir_decimate_float) (const ne10_fir_decimate_instance_f32_t * S,
                                            ne10_float32_t * pSrc,
                                            ne10_float32_t * pDst,
                                            ne10_uint32_t blockSize);
    extern void ne10_fir_decimate_float_c (const ne10_fir_decimate_instance_f32_t * S,
                                           ne10_float32_t * pSrc,
                                           ne10_float32_t * pDst,
                                           ne10_uint32_t blockSize);
#ifdef ENABLE_NE10_FIR_DECIMATE_FLOAT_NEON
    /**
     * Specific implementation of @ref ne10_fir_decimate_float using NEON SIMD capabilities.
     */
    extern void ne10_fir_decimate_float_neon (const ne10_fir_decimate_instance_f32_t * S,
            ne10_float32_t *pSrc,
            ne10_float32_t *pDst,
            ne10_uint32_t blockSize) asm ("ne10_fir_decimate_float_neon");

#endif // ENABLE_NE10_FIR_DECIMATE_FLOAT_NEON
    /** @} */ // FIR_decimate

    /**
     * @addtogroup FIR_Interpolate
     * @{
     */
    extern ne10_result_t ne10_fir_interpolate_init_float (ne10_fir_interpolate_instance_f32_t * S,
            ne10_uint8_t L,
            ne10_uint16_t numTaps,
            ne10_float32_t * pCoeffs,
            ne10_float32_t * pState,
            ne10_uint32_t blockSize);

    /**
     * @brief Processing function for the floating-point FIR interpolator.
     *
     * @param[in] *S        points to an instance of the floating-point FIR interpolator structure.
     * @param[in] *pSrc     points to the block of input data.
     * @param[out] *pDst    points to the block of output data.
     * @param[in] blockSize number of input samples to process per call.
     *
     * Points to @ref ne10_fir_interpolate_float_c or @ref ne10_fir_interpolate_float_neon.
     */
    extern void (*ne10_fir_interpolate_float) (const ne10_fir_interpolate_instance_f32_t * S,
            ne10_float32_t * pSrc,
            ne10_float32_t * pDst,
            ne10_uint32_t blockSize);
    extern void ne10_fir_interpolate_float_c (const ne10_fir_interpolate_instance_f32_t * S,
            ne10_float32_t * pSrc,
            ne10_float32_t * pDst,
            ne10_uint32_t blockSize);
#ifdef ENABLE_NE10_FIR_INTERPOLATE_FLOAT_NEON
    /**
     * Specific implementation of @ref ne10_fir_interpolate_float using NEON SIMD capabilities.
     */
    extern void ne10_fir_interpolate_float_neon (const ne10_fir_interpolate_instance_f32_t * S,
            ne10_float32_t *pSrc,
            ne10_float32_t *pDst,
            ne10_uint32_t blockSize) asm ("ne10_fir_interpolate_float_neon");
#endif // ENABLE_NE10_FIR_INTERPOLATE_FLOAT_NEON
    /** @} */ // FIR_interpolate

    /**
     * @addtogroup FIR_Lattice
     * @{
     */
    extern ne10_result_t ne10_fir_lattice_init_float (ne10_fir_lattice_instance_f32_t * S,
            ne10_uint16_t numStages,
            ne10_float32_t * pCoeffs,
            ne10_float32_t * pState);

    /**
     * @brief Processing function for the floating-point FIR lattice filter.
     *
     * @param[in]  *S        points to an instance of the floating-point FIR lattice structure.
     * @param[in]  *pSrc     points to the block of input data.
     * @param[out] *pDst     points to the block of output data
     * @param[in]  blockSize number of samples to process.
     *
     * Points to @ref ne10_fir_lattice_float_c or @ref ne10_fir_lattice_float_neon.
     */
    extern void (*ne10_fir_lattice_float) (const ne10_fir_lattice_instance_f32_t * S,
                                           ne10_float32_t * pSrc,
                                           ne10_float32_t * pDst,
                                           ne10_uint32_t blockSize);
    extern void ne10_fir_lattice_float_c (const ne10_fir_lattice_instance_f32_t * S,
                                          ne10_float32_t * pSrc,
                                          ne10_float32_t * pDst,
                                          ne10_uint32_t blockSize);
#ifdef ENABLE_NE10_FIR_LATTICE_FLOAT_NEON
    /**
     * Specific implementation of @ref ne10_fir_lattice_float using NEON SIMD capabilities.
     */
    extern void ne10_fir_lattice_float_neon (const ne10_fir_lattice_instance_f32_t * S,
            ne10_float32_t * pSrc,
            ne10_float32_t * pDst,
            ne10_uint32_t blockSize) asm ("ne10_fir_lattice_float_neon");
#endif // ENABLE_NE10_FIR_LATTICE_FLOAT_NEON
    /** @} */ // FIR_Lattice

    /**
     * @addtogroup FIR_Sparse
     * @{
     */
    extern ne10_result_t ne10_fir_sparse_init_float (ne10_fir_sparse_instance_f32_t * S,
            ne10_uint16_t numTaps,
            ne10_float32_t * pCoeffs,
            ne10_float32_t * pState,
            ne10_int32_t * pTapDelay,
            ne10_uint16_t maxDelay,
            ne10_uint32_t blockSize);

    /**
     * @brief Processing function for the floating-point sparse FIR filter.
     *
     * @param[in]  *S          points to an instance of the floating-point sparse FIR structure.
     * @param[in]  *pSrc       points to the block of input data.
     * @param[out] *pDst       points to the block of output data
     * @param[in]  *pScratchIn points to a temporary buffer of size blockSize.
     * @param[in]  blockSize   number of input samples to process per call.
     *
     * Points to @ref ne10_fir_sparse_float_c or @ref ne10_fir_sparse_float_neon.
     */
    extern void (*ne10_fir_sparse_float) (ne10_fir_sparse_instance_f32_t * S,
                                          ne10_float32_t * pSrc,
                                          ne10_float32_t * pDst,
                                          ne10_float32_t * pScratchIn,
                                          ne10_uint32_t blockSize);
    extern void ne10_fir_sparse_float_c (ne10_fir_sparse_instance_f32_t * S,
                                         ne10_float32_t * pSrc,
                                         ne10_float32_t * pDst,
                                         ne10_float32_t * pScratchIn,
                                         ne10_uint32_t blockSize);
#ifdef ENABLE_NE10_FIR_SPARSE_FLOAT_NEON
    /**
     * Specific implementation of @ref ne10_fir_sparse_float using NEON SIMD capabilities.
     */
    extern void ne10_fir_sparse_float_neon (ne10_fir_sparse_instance_f32_t * S,
                                            ne10_float32_t * pSrc,
                                            ne10_float32_t * pDst,
                                            ne10_float32_t * pScratch,
                                            ne10_uint32_t blockSize)
    asm ("ne10_fir_sparse_float_neon");
#endif // ENABLE_NE10_FIR_SPARSE_FLOAT_NEON
    /** @} */ // FIR_sparse


    /**
     * @addtogroup IIR_Lattice
     * @{
     */
    extern ne10_result_t ne10_iir_lattice_init_float (ne10_iir_lattice_instance_f32_t * S,
            ne10_uint16_t numStages,
            ne10_float32_t * pkCoeffs,
            ne10_float32_t * pvCoeffs,
            ne10_float32_t * pState,
            ne10_uint32_t blockSize);

    /**
     * @brief Processing function for the floating-point IIR lattice filter.
     *
     * @param[in] *S points to an instance of the floating-point IIR lattice structure.
     * @param[in] *pSrc points to the block of input data.
     * @param[out] *pDst points to the block of output data.
     * @param[in] blockSize number of samples to process.
     *
     * Points to @ref ne10_iir_lattice_float_c or @ref ne10_iir_lattice_float_neon.
     */
    extern void (*ne10_iir_lattice_float) (const ne10_iir_lattice_instance_f32_t * S,
                                           ne10_float32_t * pSrc,
                                           ne10_float32_t * pDst,
                                           ne10_uint32_t blockSize);
    extern void ne10_iir_lattice_float_c (const ne10_iir_lattice_instance_f32_t * S,
                                          ne10_float32_t * pSrc,
                                          ne10_float32_t * pDst,
                                          ne10_uint32_t blockSize);
#ifdef ENABLE_NE10_IIR_LATTICE_FLOAT_NEON
    /**
     * Specific implementation of @ref ne10_iir_lattice_float using NEON SIMD capabilities.
     */
    extern void ne10_iir_lattice_float_neon (const ne10_iir_lattice_instance_f32_t * S,
            ne10_float32_t * pSrc,
            ne10_float32_t * pDst,
            ne10_uint32_t blockSize) asm ("ne10_iir_lattice_float_neon");
#endif // ENABLE_NE10_IIR_LATTICE_FLOAT_NEON
    /** @} */ // IIR_Lattice

#ifdef __cplusplus
}
#endif

#endif
