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

#define ARR_LEN 5

/**
 * @example NE10_sample_intro.c
 * A simple example of initialising Ne10 and calling the C and NEON variants of
 * a function.
 */

/*
 * A simple example of using `ne10_addc_float`, an Ne10 function pointer that
 * gets dynamically initialised to the most appropriate function for the hardware.
 */
void test_add_dynamic (void)
{
    ne10_float32_t src[ARR_LEN]; // A source array of scalar floats
    ne10_float32_t cst;          // A constant scalar to add to the elements in `src`
    ne10_float32_t dst[ARR_LEN]; // A destination array for the results of the addition

    // Generate test input values for `src` and `cst` using `rand()`
    for (int i = 0; i < ARR_LEN; i++)
    {
        src[i] = (ne10_float32_t) rand() / RAND_MAX * 5.0f;
    }
    cst = (ne10_float32_t) rand() / RAND_MAX * 5.0f;

    // Perform the operation! This will use the NEON-optimised version of the function
    // if NEON hardware has been detected, or will otherwise fall back to the C version.
    ne10_addc_float (dst, src, cst, ARR_LEN);

    // Display the results
    printf("test_intro[test_add_dynamic]:\n");
    for (int i = 0; i < ARR_LEN; i++)
    {
        printf("\tne10_addc_float:\t%f + %f = %f\n", src[i], cst, dst[i]);
    }
}

/*
 * A simple example of calling the C and NEON specific versions of Ne10 functions
 * directly -- in this case, `ne10_addc_float_c` and `ne10_addc_float_neon`.
 */
void test_add_static (void)
{
    ne10_float32_t src[ARR_LEN];
    ne10_float32_t cst;
    ne10_float32_t dst_c[ARR_LEN];
    ne10_float32_t dst_neon[ARR_LEN];

    for (int i = 0; i < ARR_LEN; i++)
    {
        src[i] = (ne10_float32_t) rand() / RAND_MAX * 5.0f;
    }
    cst = (ne10_float32_t) rand() / RAND_MAX * 5.0f;

    ne10_addc_float_c (dst_c, src, cst, ARR_LEN);
    ne10_addc_float_neon (dst_neon, src, cst, ARR_LEN);

    printf("test_intro[test_add_static]:\n");
    for (int i = 0; i < ARR_LEN; i++)
    {
        printf("\tne10_addc_float_c:\t%f + %f = %f\n", src[i], cst, dst_c[i]);
        printf("\tne10_addc_float_neon:\t%f + %f = %f\n", src[i], cst, dst_neon[i]);
    }
}

/*
 * The entry point of this sample program (like `main` in regular C).
 */
int intro_sample_main(void)
{
    // Initialise Ne10, using hardware auto-detection to set function pointers such as
    // `ne10_addc_float` to point to the Ne10 code best optimised for this machine. (There
    // is no need to do this if the C or NEON versions of all Ne10 functions are called
    // directly rather than through function pointers.)
    if (ne10_init() != NE10_OK)
    {
        fprintf(stderr, "Failed to initialise Ne10.\n");
        return 1;
    }

    test_add_dynamic();
    test_add_static();

    return 0;
}
