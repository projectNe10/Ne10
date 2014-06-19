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

#include <stdio.h>
#include <stdlib.h>
#include "NE10.h"
#include "test_funcs.h"
#include "unit_test_common.h"

char* NE10RunTest(void)
{
    static int test_count = 0;

    void (*test_funcs[])(void) = {test_abs, test_addc, test_add, test_divc,
                                  test_div, test_dot, test_len, test_mlac,
                                  test_mla, test_mulc, test_mul, test_normalize,
                                  test_rsbc, test_setc, test_subc, test_sub,
                                  test_addmat, test_detmat, test_identitymat,
                                  test_invmat, test_mulmat, test_mulcmatvec,
                                  test_submat, test_transmat,
                                  test_fir, test_fir_decimate,
                                  test_fir_interpolate, test_fir_lattice,
                                  test_fir_sparse, test_iir_lattice};

    while (test_count < (sizeof(test_funcs) / sizeof(void (*)(void)))) {
        /* ne10_log_buffer is a global buffer which contain test's result
         * ne10_log_buffer's position need be setup first
         */
        ne10_log_buffer_ptr = ne10_log_buffer;
        *ne10_log_buffer_ptr++ = '[';

        (*test_funcs[test_count])();
        ++test_count;

        /* ne10_log_buffer_ptr is updated in test_funcs */
        --ne10_log_buffer_ptr;
        *ne10_log_buffer_ptr = ']';

        return ne10_log_buffer;
    }
    return NULL;
}
