/*
 *  Copyright 2011-13 ARM Limited
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
#include <stdio.h>
#include <stdlib.h>

#include "NE10.h"

/**
 * @ingroup groupSamples
 */
/**
 * @addtogroup groupSamples
 * @{
 */

/**
 * @brief This test code shows you how to call Ne10 functions with auto detecting NEON hardware
 * @param[in]  none
 * @param[in]  none
 * @return none.
 */
void test_add1 (void)
{
    int i;
    ne10_float32_t thesrc[5];
    ne10_float32_t thecst;
    ne10_float32_t thedst[5];

    for (i = 0; i < 5; i++)
    {
        thesrc[i] = (ne10_float32_t) rand() / RAND_MAX * 5.0f;
    }
    thecst = (ne10_float32_t) rand() / RAND_MAX * 5.0f;

    ne10_addc_float (thedst , thesrc, thecst, 5);
}

/**
 * @brief This test code shows you how to call Ne10 functions directly
 * @param[in]  none
 * @param[in]  none
 * @return none.
 */
void test_add2 (void)
{
    int i;
    ne10_float32_t thesrc[5];
    ne10_float32_t thecst;
    ne10_float32_t thedst1[5];
    ne10_float32_t thedst2[5];
    for (i = 0; i < 5; i++)
    {
        thesrc[i] = (ne10_float32_t) rand() / RAND_MAX * 5.0f;
    }
    thecst = (ne10_float32_t) rand() / RAND_MAX * 5.0f;

    ne10_addc_float_c (thedst1 , thesrc, thecst, 5);
    ne10_addc_float_neon (thedst2 , thesrc, thecst, 5);
}

/**
 * @brief main function
 * @param[in]  none
 * @param[in]  none
 * @return none.
 */
int main()
{
    ne10_result_t status;
    printf ("Going to initialze NE10...\n");

    status = ne10_init();
    if (status != NE10_OK)
        printf ("NE10 init failed.\n");

    printf ("NE10 has been initialized.\n");
    test_add1();
    test_add2();
}
/**
 * @} end of groupSamples
 */
