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

#define MATRICES 3

static void initialise_matrix_column(ne10_mat_row3f *col);
static void initialise_matrix(ne10_mat3x3f_t *mat);

/**
 * @ingroup groupSamples
 */
/**
 * @addtogroup groupSamples
 * @{
 */
/**
 * @brief An example of using the matrix multiply functions.
 */
int matrix_multiply_sample_main(void)
{
    ne10_mat3x3f_t src[MATRICES]; // A source array of `MATRICES` input matrices
    ne10_mat3x3f_t mul[MATRICES]; // An array of matrices to multiply those in `src` by
    ne10_mat3x3f_t dst[MATRICES]; // A destination array for the multiplication results

    // Initialise Ne10, using hardware auto-detection to set library function pointers.
    if (ne10_init() != NE10_OK)
    {
        fprintf(stderr, "Failed to initialise Ne10.\n");
        return 1;
    }

    // Generate test input values
    for (int i = 0; i < MATRICES; i++)
    {
        initialise_matrix(&src[i]);
        initialise_matrix(&mul[i]);
    }

    // Perform the multiplication of the matrices in `src` by those in `mul`
    ne10_mulmat_3x3f (dst, src, mul, MATRICES);

    // Display the results (src[i] * mul[i] == dst[i])
    for (int i = 0; i < MATRICES; i++)
    {
        printf("[ %5.2f %5.2f %5.2f     [ %5.2f %5.2f %5.2f     [ %5.2f %5.2f %5.2f\n",
            src[i].c1.r1, src[i].c2.r1, src[i].c3.r1,
            mul[i].c1.r1, mul[i].c2.r1, mul[i].c3.r1,
            dst[i].c1.r1, dst[i].c2.r1, dst[i].c3.r1);
        printf("  %5.2f %5.2f %5.2f   *   %5.2f %5.2f %5.2f   =   %5.2f %5.2f %5.2f\n",
            src[i].c1.r2, src[i].c2.r2, src[i].c3.r2,
            mul[i].c1.r2, mul[i].c2.r2, mul[i].c3.r2,
            dst[i].c1.r2, dst[i].c2.r2, dst[i].c3.r2);
        printf("  %5.2f %5.2f %5.2f ]     %5.2f %5.2f %5.2f ]     %5.2f %5.2f %5.2f ]\n",
            src[i].c1.r3, src[i].c2.r3, src[i].c3.r3,
            mul[i].c1.r3, mul[i].c2.r3, mul[i].c3.r3,
            dst[i].c1.r3, dst[i].c2.r3, dst[i].c3.r3);
        printf("\n");
    }

    return 0;
}
/**
 * @} end of groupSamples
 */

void initialise_matrix(ne10_mat3x3f_t *mat)
{
    initialise_matrix_column(&mat->c1);
    initialise_matrix_column(&mat->c2);
    initialise_matrix_column(&mat->c3);
}

void initialise_matrix_column(ne10_mat_row3f *col)
{
    col->r1 = (ne10_float32_t) rand() / RAND_MAX * 5.0f;
    col->r2 = (ne10_float32_t) rand() / RAND_MAX * 5.0f;
    col->r3 = (ne10_float32_t) rand() / RAND_MAX * 5.0f;
}
