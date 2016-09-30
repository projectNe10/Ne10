/*
 *  Copyright 2014-15 ARM Limited and Contributors.
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
 * NE10 Library : dsp/NE10_fft.c
 */

#include "NE10_types.h"
#include "NE10_macros.h"
#include "NE10_fft.h"

/*
 * This function outputs a factor buffer ('facbuf') that decomposes an FFT of input size
 * n into a number of radix-r butterfly calculations (for r in some set of radix values).
 *
 * Factor buffer layout:
 *     index 0: stage count
 *     index 1: stride for the first stage
 *     index 2 to (2*stage_count + 1): pairs of factors (number of sections, section size)
 *     index (2*stage_count + 2): an flag specifying which algorithm to use
 *
 * e.g. 1024 samples might result in the following five stage radix-4 factors buffer:
 *          [5, 256, 4, 256, 4, 64, 4, 16, 4, 4, 4, 1]
 *          i.e. 1024 = 4x256, each of which is 4x64, each of which is 4x16, each of which
 *               is 4x4, each of which is 4x1. There are 5 stages, and the stride for the
 *               first stage is 256 (1024 / 4, for radix-4).
 *
 * Only the leading 42 int32 is used to store factors.
 * The left can be used as algorithm flags, or status flags.
 * Even the leading bits of stage number can be reused.
 */
ne10_int32_t ne10_factor (ne10_int32_t n,
        ne10_int32_t * facbuf,
        ne10_int32_t ne10_factor_flags)
{
    // This is a workaround. We need to "return" some flags.
    // Otherwise, we need to modify signature of ne10_factor.
    assert (NE10_MAXFACTORS >= 32);

    if ((facbuf == NULL)
        || (n <= 0))
    {
        return NE10_ERR;
    }

    ne10_int32_t p;
    ne10_int32_t i = 1;
    ne10_int32_t stage_num = 0;
    ne10_int32_t stride_max = n;

    // Default algorithm flag is NE10_FFT_ALG_24
    ne10_int32_t alg_flag = NE10_FFT_ALG_24;

    // Factor out powers of 4, 2, 5, 3, and other.
    do
    {
        // If NE10_FACTOR_FLAGS has enable NE10_FACTOR_EIGHT.
        // Try to combine one radix-4 and one radix-2 stages
        // into one radix-8 stage.
        if ((ne10_factor_flags & NE10_FACTOR_EIGHT)
                && ((n==8) || (n==40) || (n==24)))
        {
            switch (n)
            {
            case 8:
                p = 8;
                break;
            case 24:
                p = 3;
                alg_flag = NE10_FFT_ALG_ANY;
                break;
            default: // n == 40
                p = 5;
                alg_flag = NE10_FFT_ALG_ANY;
                break;
            }
        }
        else if ((n % 4) == 0)
        {
            p = 4;
        }
        else if ((n % 2) == 0)
        {
            p = 2;
        }
        else if ((n % 5) == 0)
        {
            p = 5;
            alg_flag = NE10_FFT_ALG_ANY;
        }
        else if ((n % 3) == 0)
        {
            p = 3;
            alg_flag = NE10_FFT_ALG_ANY;
        }
        else // stop factoring
        {
            p = n;
            alg_flag = NE10_FFT_ALG_ANY;
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

    if (stage_num > 21)
    {
        // Since nfft is ne10_int32_t, stage_num can never be greater than 21,
        // because 3^21 > 2^32
        return NE10_ERR;
    }

    facbuf[2 * i] = alg_flag;
    return NE10_OK;
}

// Twiddles matrix [radix-1][mstride]
// First column (k == 0) is ignored because phase == 1, and
// twiddle = (1.0, 0.0).
void ne10_fft_generate_twiddles_line_float32 (ne10_fft_cpx_float32_t * twiddles,
        const ne10_int32_t mstride,
        const ne10_int32_t fstride,
        const ne10_int32_t radix,
        const ne10_int32_t nfft)
{
    ne10_int32_t j, k;
    ne10_float32_t phase;
    const ne10_float64_t pi = NE10_PI;

    for (j = 0; j < mstride; j++)
    {
        for (k = 1; k < radix; k++) // phase = 1 when k = 0
        {
            phase = -2 * pi * fstride * k * j / nfft;
            twiddles[mstride * (k - 1) + j].r = (ne10_float32_t) cos (phase);
            twiddles[mstride * (k - 1) + j].i = (ne10_float32_t) sin (phase);
        } // radix
    } // mstride
}

// Transposed twiddles matrix [mstride][radix-1]
// First row (k == 0) is ignored because phase == 1, and
// twiddle = (1.0, 0.0).
// Transposed twiddle tables are used in RFFT to avoid memory access by a large
// stride.
void ne10_fft_generate_twiddles_line_transposed_float32 (
    ne10_fft_cpx_float32_t* twiddles,
    const ne10_int32_t mstride,
    const ne10_int32_t fstride,
    const ne10_int32_t radix,
    const ne10_int32_t nfft)
{
    ne10_int32_t j, k;
    ne10_float32_t phase;
    const ne10_float64_t pi = NE10_PI;

    for (j = 0; j < mstride; j++)
    {
        for (k = 1; k < radix; k++) // phase = 1 when k = 0
        {
            phase = -2 * pi * fstride * k * j / nfft;
            twiddles[(radix - 1) * j + k - 1].r = (ne10_float32_t) cos (phase);
            twiddles[(radix - 1) * j + k - 1].i = (ne10_float32_t) sin (phase);
        } // radix
    } // mstride
}

// Twiddles matrix [mstride][radix-1]
// First column (k == 0)is ignored because phase == 1, and
// twiddle = (1.0, 0.0).
static void ne10_fft_generate_twiddles_line_int32 (ne10_fft_cpx_int32_t * twiddles,
        const ne10_int32_t mstride,
        const ne10_int32_t fstride,
        const ne10_int32_t radix,
        const ne10_int32_t nfft)
{
    ne10_int32_t j, k;
    ne10_float32_t phase;
    const ne10_float64_t pi = NE10_PI;

    for (j = 0; j < mstride; j++)
    {
        for (k = 1; k < radix; k++) // phase = 1 when k = 0
        {
            phase = -2 * pi * fstride * k * j / nfft;

            ne10_fft_cpx_int32_t *tw = &twiddles[mstride * (k - 1) + j];

            tw->r = (ne10_int32_t) floor (0.5f + NE10_F2I32_MAX * cos(phase));
            tw->i = (ne10_int32_t) floor (0.5f + NE10_F2I32_MAX * sin(phase));
        } // radix
    } // mstride
}

ne10_fft_cpx_int32_t* ne10_fft_generate_twiddles_int32 (ne10_fft_cpx_int32_t * twiddles,
        const ne10_int32_t * factors,
        const ne10_int32_t nfft )
{
    ne10_int32_t stage_count = factors[0];
    ne10_int32_t fstride = factors[1];
    ne10_int32_t mstride;
    ne10_int32_t cur_radix; // current radix

    // for first stage
    cur_radix = factors[2 * stage_count];
    if (cur_radix % 2) // current radix is not 4 or 2
    {
        twiddles += 1;
        ne10_fft_generate_twiddles_line_int32 (twiddles, 1, fstride, cur_radix, nfft);
        twiddles += cur_radix - 1;
    }
    stage_count--;

    // for other stage
    for (; stage_count > 0; stage_count--)
    {
        cur_radix = factors[2 * stage_count];
        fstride /= cur_radix;
        mstride = factors[2 * stage_count + 1];
        ne10_fft_generate_twiddles_line_int32 (twiddles, mstride, fstride, cur_radix, nfft);
        twiddles += mstride * (cur_radix - 1);
    } // stage_count

    return twiddles;
}

typedef void (*line_generator_float32)(ne10_fft_cpx_float32_t*,
      const ne10_int32_t,
      const ne10_int32_t,
      const ne10_int32_t,
      const ne10_int32_t);

ne10_fft_cpx_float32_t* ne10_fft_generate_twiddles_impl_float32 (
      line_generator_float32 generator,
      ne10_fft_cpx_float32_t * twiddles,
      const ne10_int32_t * factors,
      const ne10_int32_t nfft)
{
    ne10_int32_t stage_count = factors[0];
    ne10_int32_t fstride = factors[1];
    ne10_int32_t mstride;
    ne10_int32_t cur_radix; // current radix

    // for first stage
    cur_radix = factors[2 * stage_count];
    if (cur_radix % 2) // current radix is not 4 or 2
    {
        twiddles[0].r = 1.0;
        twiddles[0].i = 0.0;
        twiddles += 1;
        generator (twiddles, 1, fstride, cur_radix, nfft);
        twiddles += cur_radix - 1;
    }
    stage_count --;

    // for other stage
    for (; stage_count > 0; stage_count --)
    {
        cur_radix = factors[2 * stage_count];
        fstride /= cur_radix;
        mstride = factors[2 * stage_count + 1];
        generator (twiddles, mstride, fstride, cur_radix, nfft);
        twiddles += mstride * (cur_radix - 1);
    } // stage_count

    return twiddles;
}

ne10_fft_cpx_float32_t* ne10_fft_generate_twiddles_float32 (ne10_fft_cpx_float32_t * twiddles,
        const ne10_int32_t * factors,
        const ne10_int32_t nfft )
{
    line_generator_float32 generator = ne10_fft_generate_twiddles_line_float32;
    twiddles = ne10_fft_generate_twiddles_impl_float32(generator,
        twiddles, factors, nfft);
    return twiddles;
}

ne10_fft_cpx_float32_t* ne10_fft_generate_twiddles_transposed_float32 (
      ne10_fft_cpx_float32_t * twiddles,
      const ne10_int32_t * factors,
      const ne10_int32_t nfft)
{
    line_generator_float32 generator =
        ne10_fft_generate_twiddles_line_transposed_float32;
    twiddles = ne10_fft_generate_twiddles_impl_float32(generator,
        twiddles, factors, nfft);
    return twiddles;
}

/**
 * @addtogroup C2C_FFT_IFFT
 * @{
 */

/**
 * @brief User-callable function to create a configuration structure for the FP32 C2C NEON FFT/IFFT.
 * @param[in]   nfft             length of FFT
 * @retval      st               pointer to the FFT configuration memory, allocated with malloc.
 *
 * This function allocates and initialises an ne10_fft_cfg_float32_t configuration structure for the
 * NEON complex-to-complex single precision floating point FFT/IFFT. As part of this, it reserves a
 * buffer used internally by the FFT algorithm, factors the length of the FFT into simpler chunks,
 * and generates a "twiddle table" of coefficients used in the FFT "butterfly" calculations.
 */
ne10_fft_cfg_float32_t ne10_fft_alloc_c2c_float32_neon (ne10_int32_t nfft)
{
    // For input shorter than 15, fall back to c version.
    // We would not get much improvement from NEON for these cases.
    if (nfft < 15)
    {
        return ne10_fft_alloc_c2c_float32_c (nfft);
    }

    ne10_fft_cfg_float32_t st = NULL;
    ne10_uint32_t memneeded = sizeof (ne10_fft_state_float32_t)
                              + sizeof (ne10_int32_t) * (NE10_MAXFACTORS * 2) /* factors*/
                              + sizeof (ne10_fft_cpx_float32_t) * nfft        /* twiddle*/
                              + sizeof (ne10_fft_cpx_float32_t) * nfft        /* buffer*/
                              + NE10_FFT_BYTE_ALIGNMENT;     /* 64-bit alignment*/

    st = (ne10_fft_cfg_float32_t) NE10_MALLOC (memneeded);

    // Only backward FFT is scaled by default.
    st->is_forward_scaled = 0;
    st->is_backward_scaled = 1;

    // Bad allocation.
    if (st == NULL)
    {
        return st;
    }

    uintptr_t address = (uintptr_t) st + sizeof (ne10_fft_state_float32_t);
    NE10_BYTE_ALIGNMENT (address, NE10_FFT_BYTE_ALIGNMENT);
    st->factors = (ne10_int32_t*) address;
    st->twiddles = (ne10_fft_cpx_float32_t*) (st->factors + (NE10_MAXFACTORS * 2));
    st->buffer = st->twiddles + nfft;

    // st->last_twiddles is default NULL.
    // Calling fft_c or fft_neon is decided by this pointers.
    st->last_twiddles = NULL;

    st->nfft = nfft;
    if (nfft % NE10_FFT_PARA_LEVEL == 0)
    {
        // Size of FFT satisfies requirement of NEON optimization.
        st->nfft /= NE10_FFT_PARA_LEVEL;
        st->last_twiddles = st->twiddles + nfft / NE10_FFT_PARA_LEVEL;
    }

    ne10_int32_t result = ne10_factor (st->nfft, st->factors, NE10_FACTOR_DEFAULT);

    // Can not factor.
    if (result == NE10_ERR)
    {
        NE10_FREE (st);
        return st;
    }

    // Check if radix-8 can be enabled
    ne10_int32_t stage_count    = st->factors[0];
    ne10_int32_t algorithm_flag = st->factors[2 * (stage_count + 1)];

    // Enable radix-8.
    if (algorithm_flag == NE10_FFT_ALG_ANY)
    {
        result = ne10_factor (st->nfft, st->factors, NE10_FACTOR_EIGHT);
        if (result == NE10_ERR)
        {
            NE10_FREE (st);
            return st;
        }
        ne10_fft_generate_twiddles_float32 (st->twiddles, st->factors, st->nfft);
    }
    else
    {
        st->last_twiddles = NULL;
        st->nfft = nfft;
        result = ne10_factor (st->nfft, st->factors, NE10_FACTOR_DEFAULT);
        ne10_fft_generate_twiddles_float32 (st->twiddles, st->factors, st->nfft);
        return st;
    }

    // Generate super twiddles for the last stage.
    if (nfft % NE10_FFT_PARA_LEVEL == 0)
    {
        // Size of FFT satisfies requirement of NEON optimization.
        ne10_fft_generate_twiddles_line_float32 (st->last_twiddles,
                st->nfft,
                1,
                NE10_FFT_PARA_LEVEL,
                nfft);
    }
    return st;
}

/**
 * @brief User-callable function to create a configuration structure for the INT32 C2C NEON FFT/IFFT.
 * @param[in]   nfft             length of FFT
 * @retval      st               pointer to the FFT configuration memory, allocated with malloc.
 *
 * This function allocates and initialises an ne10_fft_cfg_int32_t configuration structure for the
 * NEON complex-to-complex 32-bit integer fixed point FFT/IFFT. As part of this, it reserves a
 * buffer used internally by the FFT algorithm, factors the length of the FFT into simpler chunks,
 * and generates a "twiddle table" of coefficients used in the FFT "butterfly" calculations.
 */
ne10_fft_cfg_int32_t ne10_fft_alloc_c2c_int32_neon (ne10_int32_t nfft)
{
    // For input shorter than 15, fall back to c version.
    // We would not get much improvement from NEON for these cases.
    if (nfft < 15)
    {
        return ne10_fft_alloc_c2c_int32_c (nfft);
    }

    ne10_fft_cfg_int32_t st = NULL;
    ne10_uint32_t memneeded = sizeof (ne10_fft_state_int32_t)
                              + sizeof (ne10_int32_t) * (NE10_MAXFACTORS * 2) /* factors*/
                              + sizeof (ne10_fft_cpx_int32_t) * nfft        /* twiddle*/
                              + sizeof (ne10_fft_cpx_int32_t) * nfft        /* buffer*/
                              + NE10_FFT_BYTE_ALIGNMENT;     /* 64-bit alignment*/

    st = (ne10_fft_cfg_int32_t) NE10_MALLOC (memneeded);

    // Bad allocation.
    if (st == NULL)
    {
        return st;
    }

    uintptr_t address = (uintptr_t) st + sizeof (ne10_fft_state_int32_t);
    NE10_BYTE_ALIGNMENT (address, NE10_FFT_BYTE_ALIGNMENT);
    st->factors = (ne10_int32_t*) address;
    st->twiddles = (ne10_fft_cpx_int32_t*) (st->factors + (NE10_MAXFACTORS * 2));
    st->buffer = st->twiddles + nfft;

    // st->last_twiddles is default NULL.
    // Calling fft_c or fft_neon is decided by this pointers.
    st->last_twiddles = NULL;

    st->nfft = nfft;
    if (nfft % NE10_FFT_PARA_LEVEL == 0)
    {
        // Size of FFT satisfies requirement of NEON optimization.
        st->nfft /= NE10_FFT_PARA_LEVEL;
        st->last_twiddles = st->twiddles + nfft / NE10_FFT_PARA_LEVEL;
    }

    ne10_int32_t result = ne10_factor (st->nfft, st->factors, NE10_FACTOR_DEFAULT);

    // Can not factor.
    if (result == NE10_ERR)
    {
        NE10_FREE (st);
        return st;
    }

    // Check if radix-8 can be enabled
    ne10_int32_t stage_count    = st->factors[0];
    ne10_int32_t algorithm_flag = st->factors[2 * (stage_count + 1)];

    // Enable radix-8.
    if (algorithm_flag == NE10_FFT_ALG_ANY)
    {
        result = ne10_factor (st->nfft, st->factors, NE10_FACTOR_EIGHT);
        if (result == NE10_ERR)
        {
            NE10_FREE (st);
            return st;
        }
        ne10_fft_generate_twiddles_int32 (st->twiddles, st->factors, st->nfft);
    }
    else
    {
        st->last_twiddles = NULL;
        st->nfft = nfft;
        result = ne10_factor (st->nfft, st->factors, NE10_FACTOR_DEFAULT);
        ne10_fft_generate_twiddles_int32 (st->twiddles, st->factors, st->nfft);
        return st;
    }

    // Generate super twiddles for the last stage.
    if (nfft % NE10_FFT_PARA_LEVEL == 0)
    {
        // Size of FFT satisfies requirement of NEON optimization.
        ne10_fft_generate_twiddles_line_int32 (st->last_twiddles,
                st->nfft,
                1,
                NE10_FFT_PARA_LEVEL,
                nfft);
    }
    return st;
}

/**
 * @defgroup C2C_FFT_IFFT_DESTROY Floating & Fixed Point Complex FFT "Destroy" Functions
 * @brief User-callable functions to destroy the configuration structure for the FFT (free memory, etc.)
 * @param[in]   cfg     pointer to the FFT configuration memory, allocated with malloc by Ne10.
 * @{
 */

void ne10_fft_destroy_c2c_float32 (ne10_fft_cfg_float32_t cfg)
{
    free(cfg);
}

void ne10_fft_destroy_c2c_int32 (ne10_fft_cfg_int32_t cfg)
{
    free (cfg);
}

void ne10_fft_destroy_c2c_int16 (ne10_fft_cfg_int16_t cfg)
{
    free (cfg);
}

/**
 * @}
 */ //end of C2C_FFT_IFFT_DESTROY group

/**
 * @}
 */ //end of C2C_FFT_IFFT group

/**
 * @addtogroup R2C_FFT_IFFT
 * @{
 */

/**
 * @defgroup R2C_FFT_IFFT_DESTROY Floating & Fixed Point Real-to-Complex FFT "Destroy" Functions
 * @brief User-callable functions to destroy the configuration structure for the FFT (free memory, etc.)
 * @param[in]   cfg     pointer to the FFT configuration memory, allocated with malloc by Ne10.
 * @{
 */

void ne10_fft_destroy_r2c_float32 (ne10_fft_r2c_cfg_float32_t cfg)
{
    free(cfg);
}

void ne10_fft_destroy_r2c_int32 (ne10_fft_r2c_cfg_int32_t cfg)
{
    free (cfg);
}

void ne10_fft_destroy_r2c_int16 (ne10_fft_r2c_cfg_int16_t cfg)
{
    free (cfg);
}

/**
 * @}
 */ //end of R2C_FFT_IFFT_DESTROY group

/**
 * @}
 */ //end of R2C_FFT_IFFT group
