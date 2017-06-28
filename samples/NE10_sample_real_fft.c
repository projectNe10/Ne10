/*
 *  Copyright 2011-16 ARM Limited and Contributors.
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
#include <stdio.h>
#include <stdlib.h>

#include "NE10.h"

#define SAMPLES 16

/**
 * @example NE10_sample_real_fft.c
 * An example of using the real-to-complex FFT functions.
 */
int real_fft_sample_main(void)
{
    ne10_float32_t src[SAMPLES] = {};                   // A source array of input data
    ne10_fft_cpx_float32_t dst[(SAMPLES / 2) + 1] = {}; // A destination array for the transformed data
    ne10_fft_r2c_cfg_float32_t cfg;                     // An FFT "configuration structure"

    // Initialise Ne10, using hardware auto-detection to set library function pointers
    if (ne10_init() != NE10_OK)
    {
        fprintf(stderr, "Failed to initialise Ne10.\n");
        return 1;
    }

    // Prepare the real-to-complex single precision floating point FFT configuration
    // structure for inputs of length `SAMPLES`. (You need only generate this once for a
    // particular input size.)
    cfg = ne10_fft_alloc_r2c_float32(SAMPLES);

    // Generate test input values
    for (int i = 0; i < SAMPLES; i++)
    {
        src[i] = (ne10_float32_t)rand() / RAND_MAX * 50.0f;
    }

    // Perform the FFT
    ne10_fft_r2c_1d_float32(dst, src, cfg);

    // Display the results
    for (int i = 0; i < SAMPLES; i++)
    {
        printf( "IN[%2d]: %10.4f\t", i, src[i]);
        if (i <= SAMPLES / 2)
            printf("OUT[%2d]: %10.4f + %10.4fi", i, dst[i].r, dst[i].i);
        printf("\n");
    }

    // Free the allocated configuration structure
    ne10_fft_destroy_r2c_float32(cfg);

    return 0;
}
