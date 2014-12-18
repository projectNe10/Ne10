/*
 *  Copyright 2013-14 ARM Limited and Contributors.
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

#ifndef TEST_FUNCS_H
#define TEST_FUNCS_H

#ifdef __cplusplus
extern "C" {
#endif

    void test_abs();
    void test_addc();
    void test_add();
    void test_divc();
    void test_div();
    void test_dot();
    void test_len();
    void test_mlac();
    void test_mla();
    void test_mulc();
    void test_mul();
    void test_normalize();
    void test_rsbc();
    void test_setc();
    void test_subc();
    void test_sub();
    void test_addmat();
    void test_detmat();
    void test_identitymat();
    void test_invmat();
    void test_mulmat();
    void test_mulcmatvec();
    void test_submat();
    void test_transmat();

    void test_fir();
    void test_fir_decimate();
    void test_fir_interpolate();
    void test_fir_lattice();
    void test_fir_sparse();
    void test_iir_lattice();

    char* NE10RunTest(void);
#ifdef __cplusplus
}
#endif

#endif
