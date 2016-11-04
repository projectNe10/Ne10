/*
 *  Copyright 2011-15 ARM Limited and Contributors.
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

#define NUMTAPS   4
#define NUMBLOCKS 2
#define BLOCKSIZE 8
#define BUFFSIZE  (NUMBLOCKS * BLOCKSIZE)

/**
 * @ingroup groupSamples
 */
/**
 * @addtogroup groupSamples
 * @{
 */
/**
 * @brief An example of using Ne10's vanilla FIR function.
 */
int fir_sample_main(void)
{
    ne10_float32_t src[BUFFSIZE]; // A source array of input data
    ne10_float32_t dst[BUFFSIZE]; // A destination array for the transformed data
    ne10_float32_t st[NUMTAPS + BLOCKSIZE - 1]; // A "state" buffer for use within the FIR
    ne10_float32_t coeffs[NUMTAPS]; // An array of FIR coefficients (in reverse order)
    ne10_fir_instance_f32_t cfg;    // An FIR "instance structure"

    // Initialise Ne10, using hardware auto-detection to set library function pointers.
    if (ne10_init() != NE10_OK)
    {
        fprintf(stderr, "Failed to initialise Ne10.\n");
        return 1;
    }

    // Prepare the FIR instance structure, storing `NUMTAPS`, `coeffs`, and `st` within
    // it, and clearing the state buffer. (For constant parameters, this process can
    // instead be performed manually.)
    if (ne10_fir_init_float (&cfg, NUMTAPS, coeffs, st, BLOCKSIZE) != NE10_OK)
    {
        fprintf(stderr, "Failed to initialise FIR instance structure.\n");
        return 1;
    }

    // Generate test coefficient values
    for (int i = 0; i < NUMTAPS; i++)
    {
        coeffs[i] = (ne10_float32_t) rand() / RAND_MAX * 5.0f;
    }

    // Generate test input values
    for (int i = 0; i < BUFFSIZE; i++)
    {
        src[i] = (ne10_float32_t) rand() / RAND_MAX * 20.0f;
    }

    // Perform the FIR filtering of the input buffer in `NUMBLOCKS` blocks of `BLOCKSIZE`
    // elements using the parameters set up in the FIR instance structure `cfg`.
    for (int b = 0; b < NUMBLOCKS; b++)
    {
        ne10_fir_float (&cfg, src + (b * BLOCKSIZE), dst + (b * BLOCKSIZE), BLOCKSIZE);
    }

    // Display the results (dst[i] = b[0] * src[i] + b[1] * src[i - 1] + b[2] * src[i - 2]
    //                               + ... + b[NUMTAPS - 1] * src[i - (NUMTAPS - 1)])
    printf("Coefficients:\n");
    for (int i = NUMTAPS - 1; i >= 0; i--)
    {
        printf("\tb[%d] = %5.4f\n", NUMTAPS - (i + 1), coeffs[i]);
    }
    for (int i = 0; i < BUFFSIZE; i++)
    {
        printf( "IN[%2d]: %9.4f\t", i, src[i]);
        printf("OUT[%2d]: %9.4f\n", i, dst[i]);
    }

    return 0;
}
/**
 * @} end of groupSamples
 */
