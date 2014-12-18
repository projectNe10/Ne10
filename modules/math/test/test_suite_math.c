/*
 *  Copyright 2012-14 ARM Limited and Contributors.
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
 * NE10 Library : test_suite_math.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "NE10_math.h"
#include "seatest.h"

//function table
ne10_func_2args_t ftbl_2args[MAX_FUNC_COUNT];
ne10_func_3args_t ftbl_3args[MAX_FUNC_COUNT];
ne10_func_4args_t ftbl_4args[MAX_FUNC_COUNT];
ne10_func_5args_t ftbl_5args[MAX_FUNC_COUNT];
ne10_func_3args_cst_t ftbl_3args_cst[MAX_FUNC_COUNT];
ne10_func_4args_cst_t ftbl_4args_cst[MAX_FUNC_COUNT];
ne10_func_5args_cst_t ftbl_5args_cst[MAX_FUNC_COUNT];

//input and output
static ne10_float32_t * guarded_acc = NULL;
static ne10_float32_t * guarded_src1 = NULL;
static ne10_float32_t * guarded_src2 = NULL;
static ne10_float32_t * guarded_cst = NULL;
static ne10_float32_t * theacc = NULL;
static ne10_float32_t * thesrc1 = NULL;
static ne10_float32_t * thesrc2 = NULL;
static ne10_float32_t * thecst = NULL;

static ne10_float32_t * guarded_dst_c = NULL;
static ne10_float32_t * guarded_dst_neon = NULL;
static ne10_float32_t * thedst_c = NULL;
static ne10_float32_t * thedst_neon = NULL;

#ifdef PERFORMANCE_TEST
static ne10_float32_t * perftest_guarded_acc = NULL;
static ne10_float32_t * perftest_guarded_src1 = NULL;
static ne10_float32_t * perftest_guarded_src2 = NULL;
static ne10_float32_t * perftest_guarded_cst = NULL;
static ne10_float32_t * perftest_theacc = NULL;
static ne10_float32_t * perftest_thesrc1 = NULL;
static ne10_float32_t * perftest_thesrc2 = NULL;
static ne10_float32_t * perftest_thecst = NULL;

static ne10_float32_t * perftest_thedst_c = NULL;
static ne10_float32_t * perftest_guarded_dst_c = NULL;
static ne10_float32_t * perftest_guarded_dst_neon = NULL;
static ne10_float32_t * perftest_thedst_neon = NULL;
static ne10_uint32_t perftest_length = 0;

static ne10_int64_t time_c = 0;
static ne10_int64_t time_neon = 0;
static ne10_float32_t time_speedup = 0.0f;
static ne10_float32_t time_savings = 0.0f;
#endif

void test_abs_case0()
{
#define MAX_VEC_COMPONENTS 4
    ne10_int32_t loop;
    ne10_int32_t i;
    ne10_int32_t func_loop;
    ne10_int32_t vec_size;
    ne10_int32_t pos;

    /* init function table */
    memset (ftbl_3args, 0, sizeof (ftbl_3args));
    ftbl_3args[ 0] = (ne10_func_3args_t) ne10_abs_float_c;
    ftbl_3args[ 1] = (ne10_func_3args_t) ne10_abs_float_neon;
    ftbl_3args[ 2] = (ne10_func_3args_t) ne10_abs_vec2f_c;
    ftbl_3args[ 3] = (ne10_func_3args_t) ne10_abs_vec2f_neon;
    ftbl_3args[ 4] = (ne10_func_3args_t) ne10_abs_vec3f_c;
    ftbl_3args[ 5] = (ne10_func_3args_t) ne10_abs_vec3f_neon;
    ftbl_3args[ 6] = (ne10_func_3args_t) ne10_abs_vec4f_c;
    ftbl_3args[ 7] = (ne10_func_3args_t) ne10_abs_vec4f_neon;

    fprintf (stdout, "----------%30s start\n", __FUNCTION__);

#if defined (SMOKE_TEST)||(REGRESSION_TEST)
    const ne10_uint32_t fixed_length = TEST_ITERATION * MAX_VEC_COMPONENTS;

    /* init src memory */
    NE10_SRC_ALLOC (thesrc1, guarded_src1, fixed_length); // 16 extra bytes at the begining and 16 extra bytes at the end

    /* init dst memory */
    NE10_DST_ALLOC (thedst_c, guarded_dst_c, fixed_length);
    NE10_DST_ALLOC (thedst_neon, guarded_dst_neon, fixed_length);

    for (func_loop = 0; func_loop < MAX_VEC_COMPONENTS; func_loop++)
    {
        for (loop = 0; loop < TEST_ITERATION; loop++)
        {
            vec_size = func_loop + 1;

            GUARD_ARRAY (thedst_c, loop * vec_size);
            GUARD_ARRAY (thedst_neon, loop * vec_size);

            ftbl_3args[2 * func_loop] (thedst_c, thesrc1, loop);
            ftbl_3args[2 * func_loop + 1] (thedst_neon, thesrc1, loop);

            CHECK_ARRAY_GUARD (thedst_c, loop * vec_size);
            CHECK_ARRAY_GUARD (thedst_neon, loop * vec_size);

            for (pos = 0; pos < loop; pos++)
            {
#ifdef DEBUG_TRACE
                fprintf (stdout, "func: %d loop count: %d position: %d \n", func_loop, loop, pos);
                for (i = 0; i < vec_size; i++)
                {
                    fprintf (stdout, "thesrc->%d: %e [0x%04X] \n", i, thesrc1[pos * vec_size + i], * (ne10_uint32_t*) &thesrc1[pos * vec_size + i]);
                }
#endif
                assert_float_vec_equal (&thedst_c[pos * vec_size], &thedst_neon[pos * vec_size], ERROR_MARGIN_SMALL, vec_size);
            }
        }
    }
    free (guarded_src1);
    free (guarded_dst_c);
    free (guarded_dst_neon);
#endif

#ifdef PERFORMANCE_TEST
    fprintf (stdout, "%25s%20s%20s%20s%20s\n", "N-component Vector", "C Time in ms", "NEON Time in ms", "Time Savings", "Performance Ratio");
    perftest_length = PERF_TEST_ITERATION * MAX_VEC_COMPONENTS;
    /* init src memory */
    NE10_SRC_ALLOC (perftest_thesrc1, perftest_guarded_src1, perftest_length); // 16 extra bytes at the begining and 16 extra bytes at the end

    /* init dst memory */
    NE10_DST_ALLOC (perftest_thedst_c, perftest_guarded_dst_c, perftest_length);
    NE10_DST_ALLOC (perftest_thedst_neon, perftest_guarded_dst_neon, perftest_length);

    for (func_loop = 0; func_loop < MAX_VEC_COMPONENTS; func_loop++)
    {
        GET_TIME (time_c,
                  for (loop = 0; loop < PERF_TEST_ITERATION; loop++) ftbl_3args[2 * func_loop] (perftest_thedst_c, perftest_thesrc1, loop);
                 );
        GET_TIME (time_neon,
                  for (loop = 0; loop < PERF_TEST_ITERATION; loop++) ftbl_3args[2 * func_loop + 1] (perftest_thedst_neon, perftest_thesrc1, loop);
                 );
        time_speedup = (ne10_float32_t) time_c / time_neon;
        time_savings = ( ( (ne10_float32_t) (time_c - time_neon)) / time_c) * 100;
        ne10_log (__FUNCTION__, "%25d%20lld%20lld%19.2f%%%18.2f:1\n", func_loop + 1, time_c, time_neon, time_savings, time_speedup);
    }

    free (perftest_guarded_src1);
    free (perftest_guarded_dst_c);
    free (perftest_guarded_dst_neon);
#endif

    fprintf (stdout, "----------%30s end\n", __FUNCTION__);
#undef MAX_VEC_COMPONENTS
}

void test_addc_case0()
{
#define MAX_VEC_COMPONENTS 4
    ne10_int32_t loop;
    ne10_int32_t i;
    ne10_int32_t func_loop;
    ne10_int32_t vec_size;
    ne10_int32_t pos;

    fprintf (stdout, "----------%30s start\n", __FUNCTION__);

    /* init function table */
    memset (ftbl_4args, 0, sizeof (ftbl_4args));
    memset (ftbl_4args_cst, 0, sizeof (ftbl_4args_cst));
    ftbl_4args_cst[ 0] = (ne10_func_4args_cst_t) ne10_addc_float_c;
    ftbl_4args_cst[ 1] = (ne10_func_4args_cst_t) ne10_addc_float_neon;
    ftbl_4args[ 2] = (ne10_func_4args_t) ne10_addc_vec2f_c;
    ftbl_4args[ 3] = (ne10_func_4args_t) ne10_addc_vec2f_neon;
    ftbl_4args[ 4] = (ne10_func_4args_t) ne10_addc_vec3f_c;
    ftbl_4args[ 5] = (ne10_func_4args_t) ne10_addc_vec3f_neon;
    ftbl_4args[ 6] = (ne10_func_4args_t) ne10_addc_vec4f_c;
    ftbl_4args[ 7] = (ne10_func_4args_t) ne10_addc_vec4f_neon;

#if defined (SMOKE_TEST)||(REGRESSION_TEST)
    const ne10_uint32_t fixed_length = TEST_ITERATION * MAX_VEC_COMPONENTS;

    /* init src memory */
    NE10_SRC_ALLOC (thesrc1, guarded_src1, fixed_length); // 16 extra bytes at the begining and 16 extra bytes at the end
    NE10_SRC_ALLOC (thecst, guarded_cst, MAX_VEC_COMPONENTS); // 16 extra bytes at the begining and 16 extra bytes at the end

    /* init dst memory */
    NE10_DST_ALLOC (thedst_c, guarded_dst_c, fixed_length);
    NE10_DST_ALLOC (thedst_neon, guarded_dst_neon, fixed_length);

    for (func_loop = 0; func_loop < MAX_VEC_COMPONENTS; func_loop++)
    {
        for (loop = 0; loop < TEST_ITERATION; loop++)
        {
            vec_size = func_loop + 1;

            GUARD_ARRAY (thedst_c, loop * vec_size);
            GUARD_ARRAY (thedst_neon, loop * vec_size);

            if (func_loop == 0)
            {
                ftbl_4args_cst[2 * func_loop] (thedst_c, thesrc1, thecst[0], loop);
                ftbl_4args_cst[2 * func_loop + 1] (thedst_neon, thesrc1, thecst[0], loop);
            }
            else
            {
                ftbl_4args[2 * func_loop] (thedst_c, thesrc1, thecst, loop);
                ftbl_4args[2 * func_loop + 1] (thedst_neon, thesrc1, thecst, loop);
            }


            CHECK_ARRAY_GUARD (thedst_c, loop * vec_size);
            CHECK_ARRAY_GUARD (thedst_neon, loop * vec_size);

            for (pos = 0; pos < loop; pos++)
            {
#ifdef DEBUG_TRACE
                fprintf (stdout, "func: %d loop count: %d position: %d \n", func_loop, loop, pos);
                for (i = 0; i < vec_size; i++)
                {
                    fprintf (stdout, "thesrc->%d: %e [0x%04X] \n", i, thesrc1[pos * vec_size + i], * (ne10_uint32_t*) &thesrc1[pos * vec_size + i]);
                    fprintf (stdout, "thecst->%d: %e [0x%04X] \n", i, thecst[i], * (ne10_uint32_t*) &thecst[i]);
                }
#endif
                assert_float_vec_equal (&thedst_c[pos * vec_size], &thedst_neon[pos * vec_size], ERROR_MARGIN_SMALL, vec_size);
            }
        }
    }
    free (guarded_src1);
    free (guarded_cst);
    free (guarded_dst_c);
    free (guarded_dst_neon);
#endif

#ifdef PERFORMANCE_TEST
    fprintf (stdout, "%25s%20s%20s%20s%20s\n", "N-component Vector", "C Time in ms", "NEON Time in ms", "Time Savings", "Performance Ratio");
    perftest_length = PERF_TEST_ITERATION * MAX_VEC_COMPONENTS;
    /* init src memory */
    NE10_SRC_ALLOC (perftest_thesrc1, perftest_guarded_src1, perftest_length); // 16 extra bytes at the begining and 16 extra bytes at the end
    NE10_SRC_ALLOC (perftest_thecst, perftest_guarded_cst, MAX_VEC_COMPONENTS); // 16 extra bytes at the begining and 16 extra bytes at the end

    /* init dst memory */
    NE10_DST_ALLOC (perftest_thedst_c, perftest_guarded_dst_c, perftest_length);
    NE10_DST_ALLOC (perftest_thedst_neon, perftest_guarded_dst_neon, perftest_length);

    for (func_loop = 0; func_loop < 1; func_loop++)
    {
        GET_TIME (time_c,
                  for (loop = 0; loop < PERF_TEST_ITERATION; loop++) ftbl_4args_cst[2 * func_loop] (perftest_thedst_c, perftest_thesrc1, perftest_thecst[0], loop);
                 );
        GET_TIME (time_neon,
                  for (loop = 0; loop < PERF_TEST_ITERATION; loop++) ftbl_4args_cst[2 * func_loop + 1] (perftest_thedst_neon, perftest_thesrc1, perftest_thecst[0], loop);
                 );
        time_speedup = (ne10_float32_t) time_c / time_neon;
        time_savings = ( ( (ne10_float32_t) (time_c - time_neon)) / time_c) * 100;
        ne10_log (__FUNCTION__, "%25d%20lld%20lld%19.2f%%%18.2f:1\n", func_loop + 1, time_c, time_neon, time_savings, time_speedup);
    }
    for (; func_loop < MAX_VEC_COMPONENTS; func_loop++)
    {
        GET_TIME (time_c,
                  for (loop = 0; loop < PERF_TEST_ITERATION; loop++) ftbl_4args[2 * func_loop] (perftest_thedst_c, perftest_thesrc1, perftest_thecst, loop);
                 );
        GET_TIME (time_neon,
                  for (loop = 0; loop < PERF_TEST_ITERATION; loop++) ftbl_4args[2 * func_loop + 1] (perftest_thedst_neon, perftest_thesrc1, perftest_thecst, loop);
                 );
        time_speedup = (ne10_float32_t) time_c / time_neon;
        time_savings = ( ( (ne10_float32_t) (time_c - time_neon)) / time_c) * 100;
        ne10_log (__FUNCTION__, "%25d%20lld%20lld%19.2f%%%18.2f:1\n", func_loop + 1, time_c, time_neon, time_savings, time_speedup);
    }

    free (perftest_guarded_src1);
    free (perftest_guarded_cst);
    free (perftest_guarded_dst_c);
    free (perftest_guarded_dst_neon);
#endif

    fprintf (stdout, "----------%30s end\n", __FUNCTION__);
#undef MAX_VEC_COMPONENTS
}

void test_add_case0()
{
#define MAX_VEC_COMPONENTS 4
    ne10_int32_t loop;
    ne10_int32_t i;
    ne10_int32_t func_loop;
    ne10_int32_t vec_size;
    ne10_int32_t pos;

    fprintf (stdout, "----------%30s start\n", __FUNCTION__);

    /* init function table */
    memset (ftbl_4args, 0, sizeof (ftbl_4args));
    ftbl_4args[ 0] = (ne10_func_4args_t) ne10_add_float_c;
    ftbl_4args[ 1] = (ne10_func_4args_t) ne10_add_float_neon;
    ftbl_4args[ 2] = (ne10_func_4args_t) ne10_add_vec2f_c;
    ftbl_4args[ 3] = (ne10_func_4args_t) ne10_add_vec2f_neon;
    ftbl_4args[ 4] = (ne10_func_4args_t) ne10_add_vec3f_c;
    ftbl_4args[ 5] = (ne10_func_4args_t) ne10_add_vec3f_neon;
    ftbl_4args[ 6] = (ne10_func_4args_t) ne10_add_vec4f_c;
    ftbl_4args[ 7] = (ne10_func_4args_t) ne10_add_vec4f_neon;

#if defined (SMOKE_TEST)||(REGRESSION_TEST)
    const ne10_uint32_t fixed_length = TEST_ITERATION * MAX_VEC_COMPONENTS;

    /* init src memory */
    NE10_SRC_ALLOC (thesrc1, guarded_src1, fixed_length); // 16 extra bytes at the begining and 16 extra bytes at the end
    NE10_SRC_ALLOC (thesrc2, guarded_src2, fixed_length); // 16 extra bytes at the begining and 16 extra bytes at the end

    /* init dst memory */
    NE10_DST_ALLOC (thedst_c, guarded_dst_c, fixed_length);
    NE10_DST_ALLOC (thedst_neon, guarded_dst_neon, fixed_length);

    for (func_loop = 0; func_loop < MAX_VEC_COMPONENTS; func_loop++)
    {
        for (loop = 0; loop < TEST_ITERATION; loop++)
        {
            vec_size = func_loop + 1;

            GUARD_ARRAY (thedst_c, loop * vec_size);
            GUARD_ARRAY (thedst_neon, loop * vec_size);

            ftbl_4args[2 * func_loop] (thedst_c, thesrc1, thesrc2, loop);
            ftbl_4args[2 * func_loop + 1] (thedst_neon, thesrc1, thesrc2, loop);

            CHECK_ARRAY_GUARD (thedst_c, loop * vec_size);
            CHECK_ARRAY_GUARD (thedst_neon, loop * vec_size);

            for (pos = 0; pos < loop; pos++)
            {
#ifdef DEBUG_TRACE
                fprintf (stdout, "func: %d loop count: %d position: %d \n", func_loop, loop, pos);
                for (i = 0; i < vec_size; i++)
                {
                    fprintf (stdout, "thesrc1->%d: %e [0x%04X] \n", i, thesrc1[pos * vec_size + i], * (ne10_uint32_t*) &thesrc1[pos * vec_size + i]);
                    fprintf (stdout, "thesrc2->%d: %e [0x%04X] \n", i, thesrc2[pos * vec_size + i], * (ne10_uint32_t*) &thesrc2[pos * vec_size + i]);
                }
#endif
                assert_float_vec_equal (&thedst_c[pos * vec_size], &thedst_neon[pos * vec_size], ERROR_MARGIN_SMALL, vec_size);
            }
        }
    }
    free (guarded_src1);
    free (guarded_src2);
    free (guarded_dst_c);
    free (guarded_dst_neon);
#endif

#ifdef PERFORMANCE_TEST
    fprintf (stdout, "%25s%20s%20s%20s%20s\n", "N-component Vector", "C Time in ms", "NEON Time in ms", "Time Savings", "Performance Ratio");
    perftest_length = PERF_TEST_ITERATION * MAX_VEC_COMPONENTS;
    /* init src memory */
    NE10_SRC_ALLOC (perftest_thesrc1, perftest_guarded_src1, perftest_length); // 16 extra bytes at the begining and 16 extra bytes at the end
    NE10_SRC_ALLOC (perftest_thesrc2, perftest_guarded_src2, perftest_length); // 16 extra bytes at the begining and 16 extra bytes at the end

    /* init dst memory */
    NE10_DST_ALLOC (perftest_thedst_c, perftest_guarded_dst_c, perftest_length);
    NE10_DST_ALLOC (perftest_thedst_neon, perftest_guarded_dst_neon, perftest_length);

    for (func_loop = 0; func_loop < MAX_VEC_COMPONENTS; func_loop++)
    {
        GET_TIME (time_c,
                  for (loop = 0; loop < PERF_TEST_ITERATION; loop++) ftbl_4args[2 * func_loop] (perftest_thedst_c, perftest_thesrc1, perftest_thesrc2, loop);
                 );
        GET_TIME (time_neon,
                  for (loop = 0; loop < PERF_TEST_ITERATION; loop++) ftbl_4args[2 * func_loop + 1] (perftest_thedst_neon, perftest_thesrc1, perftest_thesrc2, loop);
                 );
        time_speedup = (ne10_float32_t) time_c / time_neon;
        time_savings = ( ( (ne10_float32_t) (time_c - time_neon)) / time_c) * 100;
        ne10_log (__FUNCTION__, "%25d%20lld%20lld%19.2f%%%18.2f:1\n", func_loop + 1, time_c, time_neon, time_savings, time_speedup);
    }

    free (perftest_guarded_src1);
    free (perftest_guarded_src2);
    free (perftest_guarded_dst_c);
    free (perftest_guarded_dst_neon);
#endif

    fprintf (stdout, "----------%30s end\n", __FUNCTION__);
#undef MAX_VEC_COMPONENTS
}

void test_cross_case0()
{
#define MAX_VEC_COMPONENTS 3
    ne10_int32_t loop;
    ne10_int32_t i;
    ne10_int32_t func_loop;
    ne10_int32_t vec_size;
    ne10_int32_t pos;

    fprintf (stdout, "----------%30s start\n", __FUNCTION__);

    /* init function table */
    memset (ftbl_4args, 0, sizeof (ftbl_4args));
    ftbl_4args[ 4] = (ne10_func_4args_t) ne10_cross_vec3f_c;
    ftbl_4args[ 5] = (ne10_func_4args_t) ne10_cross_vec3f_neon;

#if defined (SMOKE_TEST)||(REGRESSION_TEST)
    const ne10_uint32_t fixed_length = TEST_ITERATION * MAX_VEC_COMPONENTS;

    /* init src memory */
    NE10_SRC_ALLOC_LIMIT (thesrc1, guarded_src1, fixed_length); // 16 extra bytes at the begining and 16 extra bytes at the end
    NE10_SRC_ALLOC_LIMIT (thesrc2, guarded_src2, fixed_length); // 16 extra bytes at the begining and 16 extra bytes at the end

    /* init dst memory */
    NE10_DST_ALLOC (thedst_c, guarded_dst_c, fixed_length);
    NE10_DST_ALLOC (thedst_neon, guarded_dst_neon, fixed_length);

    for (func_loop = 2; func_loop < MAX_VEC_COMPONENTS; func_loop++)
    {
        for (loop = 0; loop < TEST_ITERATION; loop++)
        {
            vec_size = func_loop + 1;

            GUARD_ARRAY (thedst_c, loop * vec_size);
            GUARD_ARRAY (thedst_neon, loop * vec_size);

            ftbl_4args[2 * func_loop] (thedst_c, thesrc1, thesrc2, loop);
            ftbl_4args[2 * func_loop + 1] (thedst_neon, thesrc1, thesrc2, loop);

            CHECK_ARRAY_GUARD (thedst_c, loop * vec_size);
            CHECK_ARRAY_GUARD (thedst_neon, loop * vec_size);

            for (pos = 0; pos < loop; pos++)
            {
#ifdef DEBUG_TRACE
                fprintf (stdout, "func: %d loop count: %d position: %d \n", func_loop, loop, pos);
                for (i = 0; i < vec_size; i++)
                {
                    fprintf (stdout, "thesrc1->%d: %e [0x%04X] \n", i, thesrc1[pos * vec_size + i], * (ne10_uint32_t*) &thesrc1[pos * vec_size + i]);
                    fprintf (stdout, "thesrc2->%d: %e [0x%04X] \n", i, thesrc2[pos * vec_size + i], * (ne10_uint32_t*) &thesrc2[pos * vec_size + i]);
                }
#endif
                assert_float_vec_equal (&thedst_c[pos * vec_size], &thedst_neon[pos * vec_size], ERROR_MARGIN_SMALL, vec_size);
            }
        }
    }
    free (guarded_src1);
    free (guarded_src2);
    free (guarded_dst_c);
    free (guarded_dst_neon);
#endif

#ifdef PERFORMANCE_TEST
    fprintf (stdout, "%25s%20s%20s%20s%20s\n", "N-component Vector", "C Time in ms", "NEON Time in ms", "Time Savings", "Performance Ratio");
    perftest_length = PERF_TEST_ITERATION * MAX_VEC_COMPONENTS;
    /* init src memory */
    NE10_SRC_ALLOC_LIMIT (perftest_thesrc1, perftest_guarded_src1, perftest_length); // 16 extra bytes at the begining and 16 extra bytes at the end
    NE10_SRC_ALLOC_LIMIT (perftest_thesrc2, perftest_guarded_src2, perftest_length); // 16 extra bytes at the begining and 16 extra bytes at the end

    /* init dst memory */
    NE10_DST_ALLOC (perftest_thedst_c, perftest_guarded_dst_c, perftest_length);
    NE10_DST_ALLOC (perftest_thedst_neon, perftest_guarded_dst_neon, perftest_length);

    for (func_loop = 2; func_loop < MAX_VEC_COMPONENTS; func_loop++)
    {
        GET_TIME (time_c,
                  for (loop = 0; loop < PERF_TEST_ITERATION; loop++) ftbl_4args[2 * func_loop] (perftest_thedst_c, perftest_thesrc1, perftest_thesrc2, loop);
                 );
        GET_TIME (time_neon,
                  for (loop = 0; loop < PERF_TEST_ITERATION; loop++) ftbl_4args[2 * func_loop + 1] (perftest_thedst_neon, perftest_thesrc1, perftest_thesrc2, loop);
                 );
        time_speedup = (ne10_float32_t) time_c / time_neon;
        time_savings = ( ( (ne10_float32_t) (time_c - time_neon)) / time_c) * 100;
        ne10_log (__FUNCTION__, "%25d%20lld%20lld%19.2f%%%18.2f:1\n", func_loop + 1, time_c, time_neon, time_savings, time_speedup);
    }

    free (perftest_guarded_src1);
    free (perftest_guarded_src2);
    free (perftest_guarded_dst_c);
    free (perftest_guarded_dst_neon);
#endif

    fprintf (stdout, "----------%30s end\n", __FUNCTION__);
#undef MAX_VEC_COMPONENTS
}

void test_divc_case0()
{
#define MAX_VEC_COMPONENTS 4
    ne10_int32_t loop;
    ne10_int32_t i;
    ne10_int32_t func_loop;
    ne10_int32_t vec_size;
    ne10_int32_t pos;

    fprintf (stdout, "----------%30s start\n", __FUNCTION__);

    /* init function table */
    memset (ftbl_4args, 0, sizeof (ftbl_4args));
    memset (ftbl_4args_cst, 0, sizeof (ftbl_4args_cst));
    ftbl_4args_cst[ 0] = (ne10_func_4args_cst_t) ne10_divc_float_c;
    ftbl_4args_cst[ 1] = (ne10_func_4args_cst_t) ne10_divc_float_neon;
    ftbl_4args[ 2] = (ne10_func_4args_t) ne10_divc_vec2f_c;
    ftbl_4args[ 3] = (ne10_func_4args_t) ne10_divc_vec2f_neon;
    ftbl_4args[ 4] = (ne10_func_4args_t) ne10_divc_vec3f_c;
    ftbl_4args[ 5] = (ne10_func_4args_t) ne10_divc_vec3f_neon;
    ftbl_4args[ 6] = (ne10_func_4args_t) ne10_divc_vec4f_c;
    ftbl_4args[ 7] = (ne10_func_4args_t) ne10_divc_vec4f_neon;

#if defined (SMOKE_TEST)||(REGRESSION_TEST)
    const ne10_uint32_t fixed_length = TEST_ITERATION * MAX_VEC_COMPONENTS;

    /* init src memory */
    NE10_SRC_ALLOC (thesrc1, guarded_src1, fixed_length); // 16 extra bytes at the begining and 16 extra bytes at the end
    NE10_SRC_ALLOC (thecst, guarded_cst, MAX_VEC_COMPONENTS); // 16 extra bytes at the begining and 16 extra bytes at the end

    /* init dst memory */
    NE10_DST_ALLOC (thedst_c, guarded_dst_c, fixed_length);
    NE10_DST_ALLOC (thedst_neon, guarded_dst_neon, fixed_length);

    for (func_loop = 0; func_loop < MAX_VEC_COMPONENTS; func_loop++)
    {
        for (loop = 0; loop < TEST_ITERATION; loop++)
        {
            vec_size = func_loop + 1;

            GUARD_ARRAY (thedst_c, loop * vec_size);
            GUARD_ARRAY (thedst_neon, loop * vec_size);

            if (func_loop == 0)
            {
                ftbl_4args_cst[2 * func_loop] (thedst_c, thesrc1, thecst[0], loop);
                ftbl_4args_cst[2 * func_loop + 1] (thedst_neon, thesrc1, thecst[0], loop);
            }
            else
            {
                ftbl_4args[2 * func_loop] (thedst_c, thesrc1, thecst, loop);
                ftbl_4args[2 * func_loop + 1] (thedst_neon, thesrc1, thecst, loop);
            }

            CHECK_ARRAY_GUARD (thedst_c, loop * vec_size);
            CHECK_ARRAY_GUARD (thedst_neon, loop * vec_size);

            for (pos = 0; pos < loop; pos++)
            {
#ifdef DEBUG_TRACE
                fprintf (stdout, "func: %d loop count: %d position: %d \n", func_loop, loop, pos);
                for (i = 0; i < vec_size; i++)
                {
                    fprintf (stdout, "thesrc->%d: %e [0x%04X] \n", i, thesrc1[pos * vec_size + i], * (ne10_uint32_t*) &thesrc1[pos * vec_size + i]);
                    fprintf (stdout, "thecst->%d: %e [0x%04X] \n", i, thecst[i], * (ne10_uint32_t*) &thecst[i]);
                }
#endif
                assert_float_vec_equal (&thedst_c[pos * vec_size], &thedst_neon[pos * vec_size], ERROR_MARGIN_SMALL, vec_size);
            }
        }
    }
    free (guarded_src1);
    free (guarded_cst);
    free (guarded_dst_c);
    free (guarded_dst_neon);
#endif

#ifdef PERFORMANCE_TEST
    fprintf (stdout, "%25s%20s%20s%20s%20s\n", "N-component Vector", "C Time in ms", "NEON Time in ms", "Time Savings", "Performance Ratio");
    perftest_length = PERF_TEST_ITERATION * MAX_VEC_COMPONENTS;
    /* init src memory */
    NE10_SRC_ALLOC (perftest_thesrc1, perftest_guarded_src1, perftest_length); // 16 extra bytes at the begining and 16 extra bytes at the end
    NE10_SRC_ALLOC (perftest_thecst, perftest_guarded_cst, MAX_VEC_COMPONENTS); // 16 extra bytes at the begining and 16 extra bytes at the end

    /* init dst memory */
    NE10_DST_ALLOC (perftest_thedst_c, perftest_guarded_dst_c, perftest_length);
    NE10_DST_ALLOC (perftest_thedst_neon, perftest_guarded_dst_neon, perftest_length);

    for (func_loop = 0; func_loop < 1; func_loop++)
    {
        GET_TIME (time_c,
                  for (loop = 0; loop < PERF_TEST_ITERATION; loop++) ftbl_4args_cst[2 * func_loop] (perftest_thedst_c, perftest_thesrc1, perftest_thecst[0], loop);
                 );
        GET_TIME (time_neon,
                  for (loop = 0; loop < PERF_TEST_ITERATION; loop++) ftbl_4args_cst[2 * func_loop + 1] (perftest_thedst_neon, perftest_thesrc1, perftest_thecst[0], loop);
                 );
        time_speedup = (ne10_float32_t) time_c / time_neon;
        time_savings = ( ( (ne10_float32_t) (time_c - time_neon)) / time_c) * 100;
        ne10_log (__FUNCTION__, "%25d%20lld%20lld%19.2f%%%18.2f:1\n", func_loop + 1, time_c, time_neon, time_savings, time_speedup);
    }
    for (; func_loop < MAX_VEC_COMPONENTS; func_loop++)
    {
        GET_TIME (time_c,
                  for (loop = 0; loop < PERF_TEST_ITERATION; loop++) ftbl_4args[2 * func_loop] (perftest_thedst_c, perftest_thesrc1, perftest_thecst, loop);
                 );
        GET_TIME (time_neon,
                  for (loop = 0; loop < PERF_TEST_ITERATION; loop++) ftbl_4args[2 * func_loop + 1] (perftest_thedst_neon, perftest_thesrc1, perftest_thecst, loop);
                 );
        time_speedup = (ne10_float32_t) time_c / time_neon;
        time_savings = ( ( (ne10_float32_t) (time_c - time_neon)) / time_c) * 100;
        ne10_log (__FUNCTION__, "%25d%20lld%20lld%19.2f%%%18.2f:1\n", func_loop + 1, time_c, time_neon, time_savings, time_speedup);
    }

    free (perftest_guarded_src1);
    free (perftest_guarded_cst);
    free (perftest_guarded_dst_c);
    free (perftest_guarded_dst_neon);
#endif

    fprintf (stdout, "----------%30s end\n", __FUNCTION__);
#undef MAX_VEC_COMPONENTS
}

void test_div_case0()
{
#define MAX_VEC_COMPONENTS 4
    ne10_int32_t loop;
    ne10_int32_t i;
    ne10_int32_t func_loop;
    ne10_int32_t vec_size;
    ne10_int32_t pos;

    fprintf (stdout, "----------%30s start\n", __FUNCTION__);

    /* init function table */
    memset (ftbl_4args, 0, sizeof (ftbl_4args));
    ftbl_4args[ 0] = (ne10_func_4args_t) ne10_div_float_c;
    ftbl_4args[ 1] = (ne10_func_4args_t) ne10_div_float_neon;
    ftbl_4args[ 2] = (ne10_func_4args_t) ne10_vdiv_vec2f_c;
    ftbl_4args[ 3] = (ne10_func_4args_t) ne10_vdiv_vec2f_neon;
    ftbl_4args[ 4] = (ne10_func_4args_t) ne10_vdiv_vec3f_c;
    ftbl_4args[ 5] = (ne10_func_4args_t) ne10_vdiv_vec3f_neon;
    ftbl_4args[ 6] = (ne10_func_4args_t) ne10_vdiv_vec4f_c;
    ftbl_4args[ 7] = (ne10_func_4args_t) ne10_vdiv_vec4f_neon;

#if defined (SMOKE_TEST)||(REGRESSION_TEST)
    const ne10_uint32_t fixed_length = TEST_ITERATION * MAX_VEC_COMPONENTS;

    /* init src memory */
    NE10_SRC_ALLOC_LIMIT (thesrc1, guarded_src1, fixed_length); // 16 extra bytes at the begining and 16 extra bytes at the end
    NE10_SRC_ALLOC_LIMIT (thesrc2, guarded_src2, fixed_length); // 16 extra bytes at the begining and 16 extra bytes at the end

    /* init dst memory */
    NE10_DST_ALLOC (thedst_c, guarded_dst_c, fixed_length);
    NE10_DST_ALLOC (thedst_neon, guarded_dst_neon, fixed_length);

    for (func_loop = 0; func_loop < MAX_VEC_COMPONENTS; func_loop++)
    {
        for (loop = 0; loop < TEST_ITERATION; loop++)
        {
            vec_size = func_loop + 1;

            GUARD_ARRAY (thedst_c, loop * vec_size);
            GUARD_ARRAY (thedst_neon, loop * vec_size);

            ftbl_4args[2 * func_loop] (thedst_c, thesrc1, thesrc2, loop);
            ftbl_4args[2 * func_loop + 1] (thedst_neon, thesrc1, thesrc2, loop);

            CHECK_ARRAY_GUARD (thedst_c, loop * vec_size);
            CHECK_ARRAY_GUARD (thedst_neon, loop * vec_size);

            for (pos = 0; pos < loop; pos++)
            {
#ifdef DEBUG_TRACE
                fprintf (stdout, "func: %d loop count: %d position: %d \n", func_loop, loop, pos);
                for (i = 0; i < vec_size; i++)
                {
                    fprintf (stdout, "thesrc1->%d: %e [0x%04X] \n", i, thesrc1[pos * vec_size + i], * (ne10_uint32_t*) &thesrc1[pos * vec_size + i]);
                    fprintf (stdout, "thesrc2->%d: %e [0x%04X] \n", i, thesrc2[pos * vec_size + i], * (ne10_uint32_t*) &thesrc2[pos * vec_size + i]);
                }
#endif
                assert_float_vec_equal (&thedst_c[pos * vec_size], &thedst_neon[pos * vec_size], ERROR_MARGIN_LARGE, vec_size);
            }
        }
    }
    free (guarded_src1);
    free (guarded_src2);
    free (guarded_dst_c);
    free (guarded_dst_neon);
#endif

#ifdef PERFORMANCE_TEST
    fprintf (stdout, "%25s%20s%20s%20s%20s\n", "N-component Vector", "C Time in ms", "NEON Time in ms", "Time Savings", "Performance Ratio");
    perftest_length = PERF_TEST_ITERATION * MAX_VEC_COMPONENTS;
    /* init src memory */
    NE10_SRC_ALLOC (perftest_thesrc1, perftest_guarded_src1, perftest_length); // 16 extra bytes at the begining and 16 extra bytes at the end
    NE10_SRC_ALLOC (perftest_thesrc2, perftest_guarded_src2, perftest_length); // 16 extra bytes at the begining and 16 extra bytes at the end

    /* init dst memory */
    NE10_DST_ALLOC (perftest_thedst_c, perftest_guarded_dst_c, perftest_length);
    NE10_DST_ALLOC (perftest_thedst_neon, perftest_guarded_dst_neon, perftest_length);

    for (func_loop = 0; func_loop < MAX_VEC_COMPONENTS; func_loop++)
    {
        GET_TIME (time_c,
                  for (loop = 0; loop < PERF_TEST_ITERATION; loop++) ftbl_4args[2 * func_loop] (perftest_thedst_c, perftest_thesrc1, perftest_thesrc2, loop);
                 );
        GET_TIME (time_neon,
                  for (loop = 0; loop < PERF_TEST_ITERATION; loop++) ftbl_4args[2 * func_loop + 1] (perftest_thedst_neon, perftest_thesrc1, perftest_thesrc2, loop);
                 );
        time_speedup = (ne10_float32_t) time_c / time_neon;
        time_savings = ( ( (ne10_float32_t) (time_c - time_neon)) / time_c) * 100;
        ne10_log (__FUNCTION__, "%25d%20lld%20lld%19.2f%%%18.2f:1\n", func_loop + 1, time_c, time_neon, time_savings, time_speedup);
    }

    free (perftest_guarded_src1);
    free (perftest_guarded_src2);
    free (perftest_guarded_dst_c);
    free (perftest_guarded_dst_neon);
#endif

    fprintf (stdout, "----------%30s end\n", __FUNCTION__);
#undef MAX_VEC_COMPONENTS
}

void test_dot_case0()
{
#define MAX_VEC_COMPONENTS 4
    ne10_int32_t loop;
    ne10_int32_t i;
    ne10_int32_t func_loop;
    ne10_int32_t vec_size;
    ne10_int32_t pos;

    fprintf (stdout, "----------%30s start\n", __FUNCTION__);

    /* init function table */
    memset (ftbl_4args, 0, sizeof (ftbl_4args));
    ftbl_4args[ 2] = (ne10_func_4args_t) ne10_dot_vec2f_c;
    ftbl_4args[ 3] = (ne10_func_4args_t) ne10_dot_vec2f_neon;
    ftbl_4args[ 4] = (ne10_func_4args_t) ne10_dot_vec3f_c;
    ftbl_4args[ 5] = (ne10_func_4args_t) ne10_dot_vec3f_neon;
    ftbl_4args[ 6] = (ne10_func_4args_t) ne10_dot_vec4f_c;
    ftbl_4args[ 7] = (ne10_func_4args_t) ne10_dot_vec4f_neon;

#if defined (SMOKE_TEST)||(REGRESSION_TEST)
    const ne10_uint32_t fixed_length = TEST_ITERATION * MAX_VEC_COMPONENTS;

    /* init src memory */
    NE10_SRC_ALLOC_LIMIT (thesrc1, guarded_src1, fixed_length); // 16 extra bytes at the begining and 16 extra bytes at the end
    NE10_SRC_ALLOC_LIMIT (thesrc2, guarded_src2, fixed_length); // 16 extra bytes at the begining and 16 extra bytes at the end

    /* init dst memory */
    NE10_DST_ALLOC (thedst_c, guarded_dst_c, fixed_length);
    NE10_DST_ALLOC (thedst_neon, guarded_dst_neon, fixed_length);

    for (func_loop = 1; func_loop < MAX_VEC_COMPONENTS; func_loop++)
    {
        for (loop = 0; loop < TEST_ITERATION; loop++)
        {
            vec_size = func_loop + 1;

            GUARD_ARRAY (thedst_c, loop);
            GUARD_ARRAY (thedst_neon, loop);

            ftbl_4args[2 * func_loop] (thedst_c, thesrc1, thesrc2, loop);
            ftbl_4args[2 * func_loop + 1] (thedst_neon, thesrc1, thesrc2, loop);

            CHECK_ARRAY_GUARD (thedst_c, loop);
            CHECK_ARRAY_GUARD (thedst_neon, loop);

            for (pos = 0; pos < loop; pos++)
            {
#ifdef DEBUG_TRACE
                fprintf (stdout, "func: %d loop count: %d position: %d \n", func_loop, loop, pos);
                for (i = 0; i < vec_size; i++)
                {
                    fprintf (stdout, "thesrc1->%d: %e [0x%04X] \n", i, thesrc1[pos * vec_size + i], * (ne10_uint32_t*) &thesrc1[pos * vec_size + i]);
                    fprintf (stdout, "thesrc2->%d: %e [0x%04X] \n", i, thesrc2[pos * vec_size + i], * (ne10_uint32_t*) &thesrc2[pos * vec_size + i]);
                }
#endif
                assert_float_vec_equal (&thedst_c[pos], &thedst_neon[pos], ERROR_MARGIN_SMALL, 1);
            }
        }
    }
    free (guarded_src1);
    free (guarded_src2);
    free (guarded_dst_c);
    free (guarded_dst_neon);
#endif

#ifdef PERFORMANCE_TEST
    fprintf (stdout, "%25s%20s%20s%20s%20s\n", "N-component Vector", "C Time in ms", "NEON Time in ms", "Time Savings", "Performance Ratio");
    perftest_length = PERF_TEST_ITERATION * MAX_VEC_COMPONENTS;
    /* init src memory */
    NE10_SRC_ALLOC_LIMIT (perftest_thesrc1, perftest_guarded_src1, perftest_length); // 16 extra bytes at the begining and 16 extra bytes at the end
    NE10_SRC_ALLOC_LIMIT (perftest_thesrc2, perftest_guarded_src2, perftest_length); // 16 extra bytes at the begining and 16 extra bytes at the end

    /* init dst memory */
    NE10_DST_ALLOC (perftest_thedst_c, perftest_guarded_dst_c, perftest_length);
    NE10_DST_ALLOC (perftest_thedst_neon, perftest_guarded_dst_neon, perftest_length);

    for (func_loop = 1; func_loop < MAX_VEC_COMPONENTS; func_loop++)
    {
        GET_TIME (time_c,
                  for (loop = 0; loop < PERF_TEST_ITERATION; loop++) ftbl_4args[2 * func_loop] (perftest_thedst_c, perftest_thesrc1, perftest_thesrc2, loop);
                 );
        GET_TIME (time_neon,
                  for (loop = 0; loop < PERF_TEST_ITERATION; loop++) ftbl_4args[2 * func_loop + 1] (perftest_thedst_neon, perftest_thesrc1, perftest_thesrc2, loop);
                 );
        time_speedup = (ne10_float32_t) time_c / time_neon;
        time_savings = ( ( (ne10_float32_t) (time_c - time_neon)) / time_c) * 100;
        ne10_log (__FUNCTION__, "%25d%20lld%20lld%19.2f%%%18.2f:1\n", func_loop + 1, time_c, time_neon, time_savings, time_speedup);
    }

    free (perftest_guarded_src1);
    free (perftest_guarded_src2);
    free (perftest_guarded_dst_c);
    free (perftest_guarded_dst_neon);
#endif

    fprintf (stdout, "----------%30s end\n", __FUNCTION__);
#undef MAX_VEC_COMPONENTS
}

void test_len_case0()
{
#define MAX_VEC_COMPONENTS 4
    ne10_int32_t loop;
    ne10_int32_t i;
    ne10_int32_t func_loop;
    ne10_int32_t vec_size;
    ne10_int32_t pos;

    /* init function table */
    memset (ftbl_3args, 0, sizeof (ftbl_3args));
    ftbl_3args[ 2] = (ne10_func_3args_t) ne10_len_vec2f_c;
    ftbl_3args[ 3] = (ne10_func_3args_t) ne10_len_vec2f_neon;
    ftbl_3args[ 4] = (ne10_func_3args_t) ne10_len_vec3f_c;
    ftbl_3args[ 5] = (ne10_func_3args_t) ne10_len_vec3f_neon;
    ftbl_3args[ 6] = (ne10_func_3args_t) ne10_len_vec4f_c;
    ftbl_3args[ 7] = (ne10_func_3args_t) ne10_len_vec4f_neon;

    fprintf (stdout, "----------%30s start\n", __FUNCTION__);

#if defined (SMOKE_TEST)||(REGRESSION_TEST)
    const ne10_uint32_t fixed_length = TEST_ITERATION * MAX_VEC_COMPONENTS;

    /* init src memory */
    NE10_SRC_ALLOC_LIMIT (thesrc1, guarded_src1, fixed_length); // 16 extra bytes at the begining and 16 extra bytes at the end

    /* init dst memory */
    NE10_DST_ALLOC (thedst_c, guarded_dst_c, fixed_length);
    NE10_DST_ALLOC (thedst_neon, guarded_dst_neon, fixed_length);

    for (func_loop = 1; func_loop < MAX_VEC_COMPONENTS; func_loop++)
    {
        for (loop = 0; loop < TEST_ITERATION; loop++)
        {
            vec_size = func_loop + 1;

            GUARD_ARRAY (thedst_c, loop);
            GUARD_ARRAY (thedst_neon, loop);

            ftbl_3args[2 * func_loop] (thedst_c, thesrc1, loop);
            ftbl_3args[2 * func_loop + 1] (thedst_neon, thesrc1, loop);

            CHECK_ARRAY_GUARD (thedst_c, loop);
            CHECK_ARRAY_GUARD (thedst_neon, loop);

            for (pos = 0; pos < loop; pos++)
            {
#ifdef DEBUG_TRACE
                fprintf (stdout, "func: %d loop count: %d position: %d \n", func_loop, loop, pos);
                for (i = 0; i < vec_size; i++)
                {
                    fprintf (stdout, "thesrc->%d: %e [0x%04X] \n", i, thesrc1[pos * vec_size + i], * (ne10_uint32_t*) &thesrc1[pos * vec_size + i]);
                }
#endif
                assert_float_vec_equal (&thedst_c[pos * vec_size], &thedst_neon[pos * vec_size], ERROR_MARGIN_LARGE, 1);
            }
        }
    }
    free (guarded_src1);
    free (guarded_dst_c);
    free (guarded_dst_neon);
#endif

#ifdef PERFORMANCE_TEST
    fprintf (stdout, "%25s%20s%20s%20s%20s\n", "N-component Vector", "C Time in ms", "NEON Time in ms", "Time Savings", "Performance Ratio");
    perftest_length = PERF_TEST_ITERATION * MAX_VEC_COMPONENTS;
    /* init src memory */
    NE10_SRC_ALLOC_LIMIT (perftest_thesrc1, perftest_guarded_src1, perftest_length); // 16 extra bytes at the begining and 16 extra bytes at the end

    /* init dst memory */
    NE10_DST_ALLOC (perftest_thedst_c, perftest_guarded_dst_c, perftest_length);
    NE10_DST_ALLOC (perftest_thedst_neon, perftest_guarded_dst_neon, perftest_length);

    for (func_loop = 1; func_loop < MAX_VEC_COMPONENTS; func_loop++)
    {
        GET_TIME (time_c,
                  for (loop = 0; loop < PERF_TEST_ITERATION; loop++) ftbl_3args[2 * func_loop] (perftest_thedst_c, perftest_thesrc1, loop);
                 );
        GET_TIME (time_neon,
                  for (loop = 0; loop < PERF_TEST_ITERATION; loop++) ftbl_3args[2 * func_loop + 1] (perftest_thedst_neon, perftest_thesrc1, loop);
                 );
        time_speedup = (ne10_float32_t) time_c / time_neon;
        time_savings = ( ( (ne10_float32_t) (time_c - time_neon)) / time_c) * 100;
        ne10_log (__FUNCTION__, "%25d%20lld%20lld%19.2f%%%18.2f:1\n", func_loop + 1, time_c, time_neon, time_savings, time_speedup);
    }

    free (perftest_guarded_src1);
    free (perftest_guarded_dst_c);
    free (perftest_guarded_dst_neon);
#endif

    fprintf (stdout, "----------%30s end\n", __FUNCTION__);
#undef MAX_VEC_COMPONENTS
}

void test_mlac_case0()
{
#define MAX_VEC_COMPONENTS 4
    ne10_int32_t loop;
    ne10_int32_t i;
    ne10_int32_t func_loop;
    ne10_int32_t vec_size;
    ne10_int32_t pos;

    fprintf (stdout, "----------%30s start\n", __FUNCTION__);

    /* init function table */
    memset (ftbl_5args, 0, sizeof (ftbl_5args));
    memset (ftbl_5args_cst, 0, sizeof (ftbl_5args_cst));
    ftbl_5args_cst[ 0] = (ne10_func_5args_cst_t) ne10_mlac_float_c;
    ftbl_5args_cst[ 1] = (ne10_func_5args_cst_t) ne10_mlac_float_neon;
    ftbl_5args[ 2] = (ne10_func_5args_t) ne10_mlac_vec2f_c;
    ftbl_5args[ 3] = (ne10_func_5args_t) ne10_mlac_vec2f_neon;
    ftbl_5args[ 4] = (ne10_func_5args_t) ne10_mlac_vec3f_c;
    ftbl_5args[ 5] = (ne10_func_5args_t) ne10_mlac_vec3f_neon;
    ftbl_5args[ 6] = (ne10_func_5args_t) ne10_mlac_vec4f_c;
    ftbl_5args[ 7] = (ne10_func_5args_t) ne10_mlac_vec4f_neon;

#if defined (SMOKE_TEST)||(REGRESSION_TEST)
    const ne10_uint32_t fixed_length = TEST_ITERATION * MAX_VEC_COMPONENTS;

    /* init src memory */
    NE10_SRC_ALLOC_LIMIT (theacc, guarded_acc, fixed_length); // 16 extra bytes at the begining and 16 extra bytes at the end
    NE10_SRC_ALLOC_LIMIT (thesrc1, guarded_src1, fixed_length); // 16 extra bytes at the begining and 16 extra bytes at the end
    NE10_SRC_ALLOC_LIMIT (thecst, guarded_cst, MAX_VEC_COMPONENTS); // 16 extra bytes at the begining and 16 extra bytes at the end

    /* init dst memory */
    NE10_DST_ALLOC (thedst_c, guarded_dst_c, fixed_length);
    NE10_DST_ALLOC (thedst_neon, guarded_dst_neon, fixed_length);

    for (func_loop = 0; func_loop < MAX_VEC_COMPONENTS; func_loop++)
    {
        for (loop = 0; loop < TEST_ITERATION; loop++)
        {
            vec_size = func_loop + 1;

            GUARD_ARRAY (thedst_c, loop * vec_size);
            GUARD_ARRAY (thedst_neon, loop * vec_size);

            if (func_loop == 0)
            {
                ftbl_5args_cst[2 * func_loop] (thedst_c, theacc, thesrc1, thecst[0], loop);
                ftbl_5args_cst[2 * func_loop + 1] (thedst_neon, theacc, thesrc1, thecst[0], loop);
            }
            else
            {
                ftbl_5args[2 * func_loop] (thedst_c, theacc, thesrc1, thecst, loop);
                ftbl_5args[2 * func_loop + 1] (thedst_neon, theacc, thesrc1, thecst, loop);
            }

            CHECK_ARRAY_GUARD (thedst_c, loop * vec_size);
            CHECK_ARRAY_GUARD (thedst_neon, loop * vec_size);

            for (pos = 0; pos < loop; pos++)
            {
#ifdef DEBUG_TRACE
                fprintf (stdout, "func: %d loop count: %d position: %d \n", func_loop, loop, pos);
                for (i = 0; i < vec_size; i++)
                {
                    fprintf (stdout, "theacc->%d: %f [0x%04X] \n", i, theacc[pos * vec_size + i], * (ne10_uint32_t*) &theacc[pos * vec_size + i]);
                    fprintf (stdout, "thesrc->%d: %f [0x%04X] \n", i, thesrc1[pos * vec_size + i], * (ne10_uint32_t*) &thesrc1[pos * vec_size + i]);
                    fprintf (stdout, "thecst->%d: %f [0x%04X] \n", i, thecst[i], * (ne10_uint32_t*) &thecst[i]);
                }
#endif
                assert_float_vec_equal (&thedst_c[pos * vec_size], &thedst_neon[pos * vec_size], ERROR_MARGIN_SMALL, vec_size);
            }
        }
    }
    free (guarded_acc);
    free (guarded_src1);
    free (guarded_cst);
    free (guarded_dst_c);
    free (guarded_dst_neon);
#endif

#ifdef PERFORMANCE_TEST
    fprintf (stdout, "%25s%20s%20s%20s%20s\n", "N-component Vector", "C Time in ms", "NEON Time in ms", "Time Savings", "Performance Ratio");
    perftest_length = PERF_TEST_ITERATION * MAX_VEC_COMPONENTS;
    /* init src memory */
    NE10_SRC_ALLOC_LIMIT (perftest_theacc, perftest_guarded_acc, perftest_length); // 16 extra bytes at the begining and 16 extra bytes at the end
    NE10_SRC_ALLOC_LIMIT (perftest_thesrc1, perftest_guarded_src1, perftest_length); // 16 extra bytes at the begining and 16 extra bytes at the end
    NE10_SRC_ALLOC_LIMIT (perftest_thecst, perftest_guarded_cst, MAX_VEC_COMPONENTS); // 16 extra bytes at the begining and 16 extra bytes at the end

    /* init dst memory */
    NE10_DST_ALLOC (perftest_thedst_c, perftest_guarded_dst_c, perftest_length);
    NE10_DST_ALLOC (perftest_thedst_neon, perftest_guarded_dst_neon, perftest_length);

    for (func_loop = 0; func_loop < 1; func_loop++)
    {
        GET_TIME (time_c,
                  for (loop = 0; loop < PERF_TEST_ITERATION; loop++) ftbl_5args_cst[2 * func_loop] (perftest_thedst_c, perftest_theacc, perftest_thesrc1, perftest_thecst[0], loop);
                 );
        GET_TIME (time_neon,
                  for (loop = 0; loop < PERF_TEST_ITERATION; loop++) ftbl_5args_cst[2 * func_loop + 1] (perftest_thedst_neon, perftest_theacc, perftest_thesrc1, perftest_thecst[0], loop);
                 );
        time_speedup = (ne10_float32_t) time_c / time_neon;
        time_savings = ( ( (ne10_float32_t) (time_c - time_neon)) / time_c) * 100;
        ne10_log (__FUNCTION__, "%25d%20lld%20lld%19.2f%%%18.2f:1\n", func_loop + 1, time_c, time_neon, time_savings, time_speedup);
    }
    for (; func_loop < MAX_VEC_COMPONENTS; func_loop++)
    {
        GET_TIME (time_c,
                  for (loop = 0; loop < PERF_TEST_ITERATION; loop++) ftbl_5args[2 * func_loop] (perftest_thedst_c, perftest_theacc, perftest_thesrc1, perftest_thecst, loop);
                 );
        GET_TIME (time_neon,
                  for (loop = 0; loop < PERF_TEST_ITERATION; loop++) ftbl_5args[2 * func_loop + 1] (perftest_thedst_neon, perftest_theacc, perftest_thesrc1, perftest_thecst, loop);
                 );
        time_speedup = (ne10_float32_t) time_c / time_neon;
        time_savings = ( ( (ne10_float32_t) (time_c - time_neon)) / time_c) * 100;
        ne10_log (__FUNCTION__, "%25d%20lld%20lld%19.2f%%%18.2f:1\n", func_loop + 1, time_c, time_neon, time_savings, time_speedup);
    }

    free (perftest_guarded_acc);
    free (perftest_guarded_src1);
    free (perftest_guarded_cst);
    free (perftest_guarded_dst_c);
    free (perftest_guarded_dst_neon);
#endif

    fprintf (stdout, "----------%30s end\n", __FUNCTION__);
#undef MAX_VEC_COMPONENTS
}

void test_mla_case0()
{
#define MAX_VEC_COMPONENTS 4
    ne10_int32_t loop;
    ne10_int32_t i;
    ne10_int32_t func_loop;
    ne10_int32_t vec_size;
    ne10_int32_t pos;

    fprintf (stdout, "----------%30s start\n", __FUNCTION__);

    /* init function table */
    memset (ftbl_5args, 0, sizeof (ftbl_5args));
    ftbl_5args[ 0] = (ne10_func_5args_t) ne10_mla_float_c;
    ftbl_5args[ 1] = (ne10_func_5args_t) ne10_mla_float_neon;
    ftbl_5args[ 2] = (ne10_func_5args_t) ne10_vmla_vec2f_c;
    ftbl_5args[ 3] = (ne10_func_5args_t) ne10_vmla_vec2f_neon;
    ftbl_5args[ 4] = (ne10_func_5args_t) ne10_vmla_vec3f_c;
    ftbl_5args[ 5] = (ne10_func_5args_t) ne10_vmla_vec3f_neon;
    ftbl_5args[ 6] = (ne10_func_5args_t) ne10_vmla_vec4f_c;
    ftbl_5args[ 7] = (ne10_func_5args_t) ne10_vmla_vec4f_neon;

#if defined (SMOKE_TEST)||(REGRESSION_TEST)
    const ne10_uint32_t fixed_length = TEST_ITERATION * MAX_VEC_COMPONENTS;

    /* init src memory */
    NE10_SRC_ALLOC_LIMIT (theacc, guarded_acc, fixed_length); // 16 extra bytes at the begining and 16 extra bytes at the end
    NE10_SRC_ALLOC_LIMIT (thesrc1, guarded_src1, fixed_length); // 16 extra bytes at the begining and 16 extra bytes at the end
    NE10_SRC_ALLOC_LIMIT (thesrc2, guarded_src2, fixed_length); // 16 extra bytes at the begining and 16 extra bytes at the end

    /* init dst memory */
    NE10_DST_ALLOC (thedst_c, guarded_dst_c, fixed_length);
    NE10_DST_ALLOC (thedst_neon, guarded_dst_neon, fixed_length);

    for (func_loop = 0; func_loop < MAX_VEC_COMPONENTS; func_loop++)
    {
        for (loop = 0; loop < TEST_ITERATION; loop++)
        {
            vec_size = func_loop + 1;

            GUARD_ARRAY (thedst_c, loop * vec_size);
            GUARD_ARRAY (thedst_neon, loop * vec_size);

            ftbl_5args[2 * func_loop] (thedst_c, theacc, thesrc1, thesrc2, loop);
            ftbl_5args[2 * func_loop + 1] (thedst_neon, theacc, thesrc1, thesrc2, loop);

            CHECK_ARRAY_GUARD (thedst_c, loop * vec_size);
            CHECK_ARRAY_GUARD (thedst_neon, loop * vec_size);

            for (pos = 0; pos < loop; pos++)
            {
#ifdef DEBUG_TRACE
                fprintf (stdout, "func: %d loop count: %d position: %d \n", func_loop, loop, pos);
                for (i = 0; i < vec_size; i++)
                {
                    fprintf (stdout, "theacc->%d: %e [0x%04X] \n", i, theacc[pos * vec_size + i], * (ne10_uint32_t*) &theacc[pos * vec_size + i]);
                    fprintf (stdout, "thesrc1->%d: %e [0x%04X] \n", i, thesrc1[pos * vec_size + i], * (ne10_uint32_t*) &thesrc1[pos * vec_size + i]);
                    fprintf (stdout, "thesrc2->%d: %e [0x%04X] \n", i, thesrc2[pos * vec_size + i], * (ne10_uint32_t*) &thesrc2[pos * vec_size + i]);
                }
#endif
                assert_float_vec_equal (&thedst_c[pos * vec_size], &thedst_neon[pos * vec_size], ERROR_MARGIN_SMALL, vec_size);
            }
        }
    }
    free (guarded_acc);
    free (guarded_src1);
    free (guarded_src2);
    free (guarded_dst_c);
    free (guarded_dst_neon);
#endif

#ifdef PERFORMANCE_TEST
    fprintf (stdout, "%25s%20s%20s%20s%20s\n", "N-component Vector", "C Time in ms", "NEON Time in ms", "Time Savings", "Performance Ratio");
    perftest_length = PERF_TEST_ITERATION * MAX_VEC_COMPONENTS;
    /* init src memory */
    NE10_SRC_ALLOC_LIMIT (perftest_theacc, perftest_guarded_acc, perftest_length); // 16 extra bytes at the begining and 16 extra bytes at the end
    NE10_SRC_ALLOC_LIMIT (perftest_thesrc1, perftest_guarded_src1, perftest_length); // 16 extra bytes at the begining and 16 extra bytes at the end
    NE10_SRC_ALLOC_LIMIT (perftest_thesrc2, perftest_guarded_src2, perftest_length); // 16 extra bytes at the begining and 16 extra bytes at the end

    /* init dst memory */
    NE10_DST_ALLOC (perftest_thedst_c, perftest_guarded_dst_c, perftest_length);
    NE10_DST_ALLOC (perftest_thedst_neon, perftest_guarded_dst_neon, perftest_length);

    for (func_loop = 0; func_loop < MAX_VEC_COMPONENTS; func_loop++)
    {
        GET_TIME (time_c,
                  for (loop = 0; loop < PERF_TEST_ITERATION; loop++) ftbl_5args[2 * func_loop] (perftest_thedst_c, perftest_theacc, perftest_thesrc1, perftest_thesrc2, loop);
                 );
        GET_TIME (time_neon,
                  for (loop = 0; loop < PERF_TEST_ITERATION; loop++) ftbl_5args[2 * func_loop + 1] (perftest_thedst_neon, perftest_theacc, perftest_thesrc1, perftest_thesrc2, loop);
                 );
        time_speedup = (ne10_float32_t) time_c / time_neon;
        time_savings = ( ( (ne10_float32_t) (time_c - time_neon)) / time_c) * 100;
        ne10_log (__FUNCTION__, "%25d%20lld%20lld%19.2f%%%18.2f:1\n", func_loop + 1, time_c, time_neon, time_savings, time_speedup);
    }

    free (perftest_guarded_acc);
    free (perftest_guarded_src1);
    free (perftest_guarded_src2);
    free (perftest_guarded_dst_c);
    free (perftest_guarded_dst_neon);
#endif

    fprintf (stdout, "----------%30s end\n", __FUNCTION__);
#undef MAX_VEC_COMPONENTS
}

void test_mulc_case0()
{
#define MAX_VEC_COMPONENTS 4
    ne10_int32_t loop;
    ne10_int32_t i;
    ne10_int32_t func_loop;
    ne10_int32_t vec_size;
    ne10_int32_t pos;

    fprintf (stdout, "----------%30s start\n", __FUNCTION__);

    /* init function table */
    memset (ftbl_4args, 0, sizeof (ftbl_4args));
    memset (ftbl_4args_cst, 0, sizeof (ftbl_4args_cst));
    ftbl_4args_cst[ 0] = (ne10_func_4args_cst_t) ne10_mulc_float_c;
    ftbl_4args_cst[ 1] = (ne10_func_4args_cst_t) ne10_mulc_float_neon;
    ftbl_4args[ 2] = (ne10_func_4args_t) ne10_mulc_vec2f_c;
    ftbl_4args[ 3] = (ne10_func_4args_t) ne10_mulc_vec2f_neon;
    ftbl_4args[ 4] = (ne10_func_4args_t) ne10_mulc_vec3f_c;
    ftbl_4args[ 5] = (ne10_func_4args_t) ne10_mulc_vec3f_neon;
    ftbl_4args[ 6] = (ne10_func_4args_t) ne10_mulc_vec4f_c;
    ftbl_4args[ 7] = (ne10_func_4args_t) ne10_mulc_vec4f_neon;

#if defined (SMOKE_TEST)||(REGRESSION_TEST)
    const ne10_uint32_t fixed_length = TEST_ITERATION * MAX_VEC_COMPONENTS;

    /* init src memory */
    NE10_SRC_ALLOC_LIMIT (thesrc1, guarded_src1, fixed_length); // 16 extra bytes at the begining and 16 extra bytes at the end
    NE10_SRC_ALLOC_LIMIT (thecst, guarded_cst, MAX_VEC_COMPONENTS); // 16 extra bytes at the begining and 16 extra bytes at the end

    /* init dst memory */
    NE10_DST_ALLOC (thedst_c, guarded_dst_c, fixed_length);
    NE10_DST_ALLOC (thedst_neon, guarded_dst_neon, fixed_length);

    for (func_loop = 0; func_loop < MAX_VEC_COMPONENTS; func_loop++)
    {
        for (loop = 0; loop < TEST_ITERATION; loop++)
        {
            vec_size = func_loop + 1;

            GUARD_ARRAY (thedst_c, loop * vec_size);
            GUARD_ARRAY (thedst_neon, loop * vec_size);

            if (func_loop == 0)
            {
                ftbl_4args_cst[2 * func_loop] (thedst_c, thesrc1, thecst[0], loop);
                ftbl_4args_cst[2 * func_loop + 1] (thedst_neon, thesrc1, thecst[0], loop);
            }
            else
            {
                ftbl_4args[2 * func_loop] (thedst_c, thesrc1, thecst, loop);
                ftbl_4args[2 * func_loop + 1] (thedst_neon, thesrc1, thecst, loop);
            }

            CHECK_ARRAY_GUARD (thedst_c, loop * vec_size);
            CHECK_ARRAY_GUARD (thedst_neon, loop * vec_size);

            for (pos = 0; pos < loop; pos++)
            {
#ifdef DEBUG_TRACE
                fprintf (stdout, "func: %d loop count: %d position: %d \n", func_loop, loop, pos);
                for (i = 0; i < vec_size; i++)
                {
                    fprintf (stdout, "thesrc->%d: %e [0x%04X] \n", i, thesrc1[pos * vec_size + i], * (ne10_uint32_t*) &thesrc1[pos * vec_size + i]);
                    fprintf (stdout, "thecst->%d: %e [0x%04X] \n", i, thecst[i], * (ne10_uint32_t*) &thecst[i]);
                }
#endif
                assert_float_vec_equal (&thedst_c[pos * vec_size], &thedst_neon[pos * vec_size], ERROR_MARGIN_SMALL, vec_size);
            }
        }
    }
    free (guarded_src1);
    free (guarded_cst);
    free (guarded_dst_c);
    free (guarded_dst_neon);
#endif

#ifdef PERFORMANCE_TEST
    fprintf (stdout, "%25s%20s%20s%20s%20s\n", "N-component Vector", "C Time in ms", "NEON Time in ms", "Time Savings", "Performance Ratio");
    perftest_length = PERF_TEST_ITERATION * MAX_VEC_COMPONENTS;
    /* init src memory */
    NE10_SRC_ALLOC_LIMIT (perftest_thesrc1, perftest_guarded_src1, perftest_length); // 16 extra bytes at the begining and 16 extra bytes at the end
    NE10_SRC_ALLOC_LIMIT (perftest_thecst, perftest_guarded_cst, MAX_VEC_COMPONENTS); // 16 extra bytes at the begining and 16 extra bytes at the end

    /* init dst memory */
    NE10_DST_ALLOC (perftest_thedst_c, perftest_guarded_dst_c, perftest_length);
    NE10_DST_ALLOC (perftest_thedst_neon, perftest_guarded_dst_neon, perftest_length);

    for (func_loop = 0; func_loop < 1; func_loop++)
    {
        GET_TIME (time_c,
                  for (loop = 0; loop < PERF_TEST_ITERATION; loop++) ftbl_4args_cst[2 * func_loop] (perftest_thedst_c, perftest_thesrc1, perftest_thecst[0], loop);
                 );
        GET_TIME (time_neon,
                  for (loop = 0; loop < PERF_TEST_ITERATION; loop++) ftbl_4args_cst[2 * func_loop + 1] (perftest_thedst_neon, perftest_thesrc1, perftest_thecst[0], loop);
                 );
        time_speedup = (ne10_float32_t) time_c / time_neon;
        time_savings = ( ( (ne10_float32_t) (time_c - time_neon)) / time_c) * 100;
        ne10_log (__FUNCTION__, "%25d%20lld%20lld%19.2f%%%18.2f:1\n", func_loop + 1, time_c, time_neon, time_savings, time_speedup);
    }
    for (; func_loop < MAX_VEC_COMPONENTS; func_loop++)
    {
        GET_TIME (time_c,
                  for (loop = 0; loop < PERF_TEST_ITERATION; loop++) ftbl_4args[2 * func_loop] (perftest_thedst_c, perftest_thesrc1, perftest_thecst, loop);
                 );
        GET_TIME (time_neon,
                  for (loop = 0; loop < PERF_TEST_ITERATION; loop++) ftbl_4args[2 * func_loop + 1] (perftest_thedst_neon, perftest_thesrc1, perftest_thecst, loop);
                 );
        time_speedup = (ne10_float32_t) time_c / time_neon;
        time_savings = ( ( (ne10_float32_t) (time_c - time_neon)) / time_c) * 100;
        ne10_log (__FUNCTION__, "%25d%20lld%20lld%19.2f%%%18.2f:1\n", func_loop + 1, time_c, time_neon, time_savings, time_speedup);
    }

    free (perftest_guarded_src1);
    free (perftest_guarded_cst);
    free (perftest_guarded_dst_c);
    free (perftest_guarded_dst_neon);
#endif

    fprintf (stdout, "----------%30s end\n", __FUNCTION__);
#undef MAX_VEC_COMPONENTS
}

void test_mul_case0()
{
#define MAX_VEC_COMPONENTS 4
    ne10_int32_t loop;
    ne10_int32_t i;
    ne10_int32_t func_loop;
    ne10_int32_t vec_size;
    ne10_int32_t pos;

    fprintf (stdout, "----------%30s start\n", __FUNCTION__);

    /* init function table */
    memset (ftbl_4args, 0, sizeof (ftbl_4args));
    ftbl_4args[ 0] = (ne10_func_4args_t) ne10_mul_float_c;
    ftbl_4args[ 1] = (ne10_func_4args_t) ne10_mul_float_neon;
    ftbl_4args[ 2] = (ne10_func_4args_t) ne10_vmul_vec2f_c;
    ftbl_4args[ 3] = (ne10_func_4args_t) ne10_vmul_vec2f_neon;
    ftbl_4args[ 4] = (ne10_func_4args_t) ne10_vmul_vec3f_c;
    ftbl_4args[ 5] = (ne10_func_4args_t) ne10_vmul_vec3f_neon;
    ftbl_4args[ 6] = (ne10_func_4args_t) ne10_vmul_vec4f_c;
    ftbl_4args[ 7] = (ne10_func_4args_t) ne10_vmul_vec4f_neon;

#if defined (SMOKE_TEST)||(REGRESSION_TEST)
    const ne10_uint32_t fixed_length = TEST_ITERATION * MAX_VEC_COMPONENTS;

    /* init src memory */
    NE10_SRC_ALLOC_LIMIT (thesrc1, guarded_src1, fixed_length); // 16 extra bytes at the begining and 16 extra bytes at the end
    NE10_SRC_ALLOC_LIMIT (thesrc2, guarded_src2, fixed_length); // 16 extra bytes at the begining and 16 extra bytes at the end

    /* init dst memory */
    NE10_DST_ALLOC (thedst_c, guarded_dst_c, fixed_length);
    NE10_DST_ALLOC (thedst_neon, guarded_dst_neon, fixed_length);

    for (func_loop = 0; func_loop < MAX_VEC_COMPONENTS; func_loop++)
    {
        for (loop = 0; loop < TEST_ITERATION; loop++)
        {
            vec_size = func_loop + 1;

            GUARD_ARRAY (thedst_c, loop * vec_size);
            GUARD_ARRAY (thedst_neon, loop * vec_size);

            ftbl_4args[2 * func_loop] (thedst_c, thesrc1, thesrc2, loop);
            ftbl_4args[2 * func_loop + 1] (thedst_neon, thesrc1, thesrc2, loop);

            CHECK_ARRAY_GUARD (thedst_c, loop * vec_size);
            CHECK_ARRAY_GUARD (thedst_neon, loop * vec_size);

            for (pos = 0; pos < loop; pos++)
            {
#ifdef DEBUG_TRACE
                fprintf (stdout, "func: %d loop count: %d position: %d \n", func_loop, loop, pos);
                for (i = 0; i < vec_size; i++)
                {
                    fprintf (stdout, "thesrc1->%d: %e [0x%04X] \n", i, thesrc1[pos * vec_size + i], * (ne10_uint32_t*) &thesrc1[pos * vec_size + i]);
                    fprintf (stdout, "thesrc2->%d: %e [0x%04X] \n", i, thesrc2[pos * vec_size + i], * (ne10_uint32_t*) &thesrc2[pos * vec_size + i]);
                }
#endif
                assert_float_vec_equal (&thedst_c[pos * vec_size], &thedst_neon[pos * vec_size], ERROR_MARGIN_SMALL, vec_size);
            }
        }
    }
    free (guarded_src1);
    free (guarded_src2);
    free (guarded_dst_c);
    free (guarded_dst_neon);
#endif

#ifdef PERFORMANCE_TEST
    fprintf (stdout, "%25s%20s%20s%20s%20s\n", "N-component Vector", "C Time in ms", "NEON Time in ms", "Time Savings", "Performance Ratio");
    perftest_length = PERF_TEST_ITERATION * MAX_VEC_COMPONENTS;
    /* init src memory */
    NE10_SRC_ALLOC_LIMIT (perftest_thesrc1, perftest_guarded_src1, perftest_length); // 16 extra bytes at the begining and 16 extra bytes at the end
    NE10_SRC_ALLOC_LIMIT (perftest_thesrc2, perftest_guarded_src2, perftest_length); // 16 extra bytes at the begining and 16 extra bytes at the end

    /* init dst memory */
    NE10_DST_ALLOC (perftest_thedst_c, perftest_guarded_dst_c, perftest_length);
    NE10_DST_ALLOC (perftest_thedst_neon, perftest_guarded_dst_neon, perftest_length);

    for (func_loop = 0; func_loop < MAX_VEC_COMPONENTS; func_loop++)
    {
        GET_TIME (time_c,
                  for (loop = 0; loop < PERF_TEST_ITERATION; loop++) ftbl_4args[2 * func_loop] (perftest_thedst_c, perftest_thesrc1, perftest_thesrc2, loop);
                 );
        GET_TIME (time_neon,
                  for (loop = 0; loop < PERF_TEST_ITERATION; loop++) ftbl_4args[2 * func_loop + 1] (perftest_thedst_neon, perftest_thesrc1, perftest_thesrc2, loop);
                 );
        time_speedup = (ne10_float32_t) time_c / time_neon;
        time_savings = ( ( (ne10_float32_t) (time_c - time_neon)) / time_c) * 100;
        ne10_log (__FUNCTION__, "%25d%20lld%20lld%19.2f%%%18.2f:1\n", func_loop + 1, time_c, time_neon, time_savings, time_speedup);
    }

    free (perftest_guarded_src1);
    free (perftest_guarded_src2);
    free (perftest_guarded_dst_c);
    free (perftest_guarded_dst_neon);
#endif

    fprintf (stdout, "----------%30s end\n", __FUNCTION__);
#undef MAX_VEC_COMPONENTS
}

void test_normalize_case0()
{
#define MAX_VEC_COMPONENTS 4
    ne10_int32_t loop;
    ne10_int32_t i;
    ne10_int32_t func_loop;
    ne10_int32_t vec_size;
    ne10_int32_t pos;

    /* init function table */
    memset (ftbl_3args, 0, sizeof (ftbl_3args));
    ftbl_3args[ 2] = (ne10_func_3args_t) ne10_normalize_vec2f_c;
    ftbl_3args[ 3] = (ne10_func_3args_t) ne10_normalize_vec2f_neon;
    ftbl_3args[ 4] = (ne10_func_3args_t) ne10_normalize_vec3f_c;
    ftbl_3args[ 5] = (ne10_func_3args_t) ne10_normalize_vec3f_neon;
    ftbl_3args[ 6] = (ne10_func_3args_t) ne10_normalize_vec4f_c;
    ftbl_3args[ 7] = (ne10_func_3args_t) ne10_normalize_vec4f_neon;

    fprintf (stdout, "----------%30s start\n", __FUNCTION__);

#if defined (SMOKE_TEST)||(REGRESSION_TEST)
    const ne10_uint32_t fixed_length = TEST_ITERATION * MAX_VEC_COMPONENTS;

    /* init src memory */
    NE10_SRC_ALLOC_LIMIT (thesrc1, guarded_src1, fixed_length); // 16 extra bytes at the begining and 16 extra bytes at the end

    /* init dst memory */
    NE10_DST_ALLOC (thedst_c, guarded_dst_c, fixed_length);
    NE10_DST_ALLOC (thedst_neon, guarded_dst_neon, fixed_length);

    for (func_loop = 1; func_loop < MAX_VEC_COMPONENTS; func_loop++)
    {
        for (loop = 0; loop < TEST_ITERATION; loop++)
        {
            vec_size = func_loop + 1;

            GUARD_ARRAY (thedst_c, loop * vec_size);
            GUARD_ARRAY (thedst_neon, loop * vec_size);

            ftbl_3args[2 * func_loop] (thedst_c, thesrc1, loop);
            ftbl_3args[2 * func_loop + 1] (thedst_neon, thesrc1, loop);

            CHECK_ARRAY_GUARD (thedst_c, loop * vec_size);
            CHECK_ARRAY_GUARD (thedst_neon, loop * vec_size);

            for (pos = 0; pos < loop; pos++)
            {
#ifdef DEBUG_TRACE
                fprintf (stdout, "func: %d loop count: %d position: %d \n", func_loop, loop, pos);
                for (i = 0; i < vec_size; i++)
                {
                    fprintf (stdout, "thesrc->%d: %e [0x%04X] \n", i, thesrc1[pos * vec_size + i], * (ne10_uint32_t*) &thesrc1[pos * vec_size + i]);
                }
#endif
                assert_float_vec_equal (&thedst_c[pos * vec_size], &thedst_neon[pos * vec_size], ERROR_MARGIN_LARGE, vec_size);
            }
        }
    }
    free (guarded_src1);
    free (guarded_dst_c);
    free (guarded_dst_neon);
#endif

#ifdef PERFORMANCE_TEST
    fprintf (stdout, "%25s%20s%20s%20s%20s\n", "N-component Vector", "C Time in ms", "NEON Time in ms", "Time Savings", "Performance Ratio");
    perftest_length = PERF_TEST_ITERATION * MAX_VEC_COMPONENTS;
    /* init src memory */
    NE10_SRC_ALLOC_LIMIT (perftest_thesrc1, perftest_guarded_src1, perftest_length); // 16 extra bytes at the begining and 16 extra bytes at the end

    /* init dst memory */
    NE10_DST_ALLOC (perftest_thedst_c, perftest_guarded_dst_c, perftest_length);
    NE10_DST_ALLOC (perftest_thedst_neon, perftest_guarded_dst_neon, perftest_length);

    for (func_loop = 1; func_loop < MAX_VEC_COMPONENTS; func_loop++)
    {
        GET_TIME (time_c,
                  for (loop = 0; loop < PERF_TEST_ITERATION; loop++) ftbl_3args[2 * func_loop] (perftest_thedst_c, perftest_thesrc1, loop);
                 );
        GET_TIME (time_neon,
                  for (loop = 0; loop < PERF_TEST_ITERATION; loop++) ftbl_3args[2 * func_loop + 1] (perftest_thedst_neon, perftest_thesrc1, loop);
                 );
        time_speedup = (ne10_float32_t) time_c / time_neon;
        time_savings = ( ( (ne10_float32_t) (time_c - time_neon)) / time_c) * 100;
        ne10_log (__FUNCTION__, "%25d%20lld%20lld%19.2f%%%18.2f:1\n", func_loop + 1, time_c, time_neon, time_savings, time_speedup);
    }

    free (perftest_guarded_src1);
    free (perftest_guarded_dst_c);
    free (perftest_guarded_dst_neon);
#endif

    fprintf (stdout, "----------%30s end\n", __FUNCTION__);
#undef MAX_VEC_COMPONENTS
}

void test_rsbc_case0()
{
#define MAX_VEC_COMPONENTS 4
    ne10_int32_t loop;
    ne10_int32_t i;
    ne10_int32_t func_loop;
    ne10_int32_t vec_size;
    ne10_int32_t pos;

    fprintf (stdout, "----------%30s start\n", __FUNCTION__);

    /* init function table */
    memset (ftbl_4args, 0, sizeof (ftbl_4args));
    memset (ftbl_4args_cst, 0, sizeof (ftbl_4args_cst));
    ftbl_4args_cst[ 0] = (ne10_func_4args_cst_t) ne10_rsbc_float_c;
    ftbl_4args_cst[ 1] = (ne10_func_4args_cst_t) ne10_rsbc_float_neon;
    ftbl_4args[ 2] = (ne10_func_4args_t) ne10_rsbc_vec2f_c;
    ftbl_4args[ 3] = (ne10_func_4args_t) ne10_rsbc_vec2f_neon;
    ftbl_4args[ 4] = (ne10_func_4args_t) ne10_rsbc_vec3f_c;
    ftbl_4args[ 5] = (ne10_func_4args_t) ne10_rsbc_vec3f_neon;
    ftbl_4args[ 6] = (ne10_func_4args_t) ne10_rsbc_vec4f_c;
    ftbl_4args[ 7] = (ne10_func_4args_t) ne10_rsbc_vec4f_neon;

#if defined (SMOKE_TEST)||(REGRESSION_TEST)
    const ne10_uint32_t fixed_length = TEST_ITERATION * MAX_VEC_COMPONENTS;

    /* init src memory */
    NE10_SRC_ALLOC (thesrc1, guarded_src1, fixed_length); // 16 extra bytes at the begining and 16 extra bytes at the end
    NE10_SRC_ALLOC (thecst, guarded_cst, MAX_VEC_COMPONENTS); // 16 extra bytes at the begining and 16 extra bytes at the end

    /* init dst memory */
    NE10_DST_ALLOC (thedst_c, guarded_dst_c, fixed_length);
    NE10_DST_ALLOC (thedst_neon, guarded_dst_neon, fixed_length);

    for (func_loop = 0; func_loop < MAX_VEC_COMPONENTS; func_loop++)
    {
        for (loop = 0; loop < TEST_ITERATION; loop++)
        {
            vec_size = func_loop + 1;

            GUARD_ARRAY (thedst_c, loop * vec_size);
            GUARD_ARRAY (thedst_neon, loop * vec_size);

            if (func_loop == 0)
            {
                ftbl_4args_cst[2 * func_loop] (thedst_c, thesrc1, thecst[0], loop);
                ftbl_4args_cst[2 * func_loop + 1] (thedst_neon, thesrc1, thecst[0], loop);
            }
            else
            {
                ftbl_4args[2 * func_loop] (thedst_c, thesrc1, thecst, loop);
                ftbl_4args[2 * func_loop + 1] (thedst_neon, thesrc1, thecst, loop);
            }

            CHECK_ARRAY_GUARD (thedst_c, loop * vec_size);
            CHECK_ARRAY_GUARD (thedst_neon, loop * vec_size);

            for (pos = 0; pos < loop; pos++)
            {
#ifdef DEBUG_TRACE
                fprintf (stdout, "func: %d loop count: %d position: %d \n", func_loop, loop, pos);
                for (i = 0; i < vec_size; i++)
                {
                    fprintf (stdout, "thesrc->%d: %e [0x%04X] \n", i, thesrc1[pos * vec_size + i], * (ne10_uint32_t*) &thesrc1[pos * vec_size + i]);
                    fprintf (stdout, "thecst->%d: %e [0x%04X] \n", i, thecst[i], * (ne10_uint32_t*) &thecst[i]);
                }
#endif
                assert_float_vec_equal (&thedst_c[pos * vec_size], &thedst_neon[pos * vec_size], ERROR_MARGIN_SMALL, vec_size);
            }
        }
    }
    free (guarded_src1);
    free (guarded_cst);
    free (guarded_dst_c);
    free (guarded_dst_neon);
#endif

#ifdef PERFORMANCE_TEST
    fprintf (stdout, "%25s%20s%20s%20s%20s\n", "N-component Vector", "C Time in ms", "NEON Time in ms", "Time Savings", "Performance Ratio");
    perftest_length = PERF_TEST_ITERATION * MAX_VEC_COMPONENTS;
    /* init src memory */
    NE10_SRC_ALLOC (perftest_thesrc1, perftest_guarded_src1, perftest_length); // 16 extra bytes at the begining and 16 extra bytes at the end
    NE10_SRC_ALLOC (perftest_thecst, perftest_guarded_cst, MAX_VEC_COMPONENTS); // 16 extra bytes at the begining and 16 extra bytes at the end

    /* init dst memory */
    NE10_DST_ALLOC (perftest_thedst_c, perftest_guarded_dst_c, perftest_length);
    NE10_DST_ALLOC (perftest_thedst_neon, perftest_guarded_dst_neon, perftest_length);

    for (func_loop = 0; func_loop < 1; func_loop++)
    {
        GET_TIME (time_c,
                  for (loop = 0; loop < PERF_TEST_ITERATION; loop++) ftbl_4args_cst[2 * func_loop] (perftest_thedst_c, perftest_thesrc1, perftest_thecst[0], loop);
                 );
        GET_TIME (time_neon,
                  for (loop = 0; loop < PERF_TEST_ITERATION; loop++) ftbl_4args_cst[2 * func_loop + 1] (perftest_thedst_neon, perftest_thesrc1, perftest_thecst[0], loop);
                 );
        time_speedup = (ne10_float32_t) time_c / time_neon;
        time_savings = ( ( (ne10_float32_t) (time_c - time_neon)) / time_c) * 100;
        ne10_log (__FUNCTION__, "%25d%20lld%20lld%19.2f%%%18.2f:1\n", func_loop + 1, time_c, time_neon, time_savings, time_speedup);
    }
    for (; func_loop < MAX_VEC_COMPONENTS; func_loop++)
    {
        GET_TIME (time_c,
                  for (loop = 0; loop < PERF_TEST_ITERATION; loop++) ftbl_4args[2 * func_loop] (perftest_thedst_c, perftest_thesrc1, perftest_thecst, loop);
                 );
        GET_TIME (time_neon,
                  for (loop = 0; loop < PERF_TEST_ITERATION; loop++) ftbl_4args[2 * func_loop + 1] (perftest_thedst_neon, perftest_thesrc1, perftest_thecst, loop);
                 );
        time_speedup = (ne10_float32_t) time_c / time_neon;
        time_savings = ( ( (ne10_float32_t) (time_c - time_neon)) / time_c) * 100;
        ne10_log (__FUNCTION__, "%25d%20lld%20lld%19.2f%%%18.2f:1\n", func_loop + 1, time_c, time_neon, time_savings, time_speedup);
    }

    free (perftest_guarded_src1);
    free (perftest_guarded_cst);
    free (perftest_guarded_dst_c);
    free (perftest_guarded_dst_neon);
#endif

    fprintf (stdout, "----------%30s end\n", __FUNCTION__);
#undef MAX_VEC_COMPONENTS
}

void test_setc_case0()
{
#define MAX_VEC_COMPONENTS 4
    ne10_int32_t loop;
    ne10_int32_t i;
    ne10_int32_t func_loop;
    ne10_int32_t vec_size;
    ne10_int32_t pos;

    fprintf (stdout, "----------%30s start\n", __FUNCTION__);

    /* init function table */
    memset (ftbl_3args, 0, sizeof (ftbl_3args));
    memset (ftbl_3args_cst, 0, sizeof (ftbl_3args_cst));
    ftbl_3args_cst[ 0] = (ne10_func_3args_cst_t) ne10_setc_float_c;
    ftbl_3args_cst[ 1] = (ne10_func_3args_cst_t) ne10_setc_float_neon;
    ftbl_3args[ 2] = (ne10_func_3args_t) ne10_setc_vec2f_c;
    ftbl_3args[ 3] = (ne10_func_3args_t) ne10_setc_vec2f_neon;
    ftbl_3args[ 4] = (ne10_func_3args_t) ne10_setc_vec3f_c;
    ftbl_3args[ 5] = (ne10_func_3args_t) ne10_setc_vec3f_neon;
    ftbl_3args[ 6] = (ne10_func_3args_t) ne10_setc_vec4f_c;
    ftbl_3args[ 7] = (ne10_func_3args_t) ne10_setc_vec4f_neon;

#if defined (SMOKE_TEST)||(REGRESSION_TEST)
    const ne10_uint32_t fixed_length = TEST_ITERATION * MAX_VEC_COMPONENTS;

    /* init src memory */
    NE10_SRC_ALLOC (thecst, guarded_cst, MAX_VEC_COMPONENTS); // 16 extra bytes at the begining and 16 extra bytes at the end

    /* init dst memory */
    NE10_DST_ALLOC (thedst_c, guarded_dst_c, fixed_length);
    NE10_DST_ALLOC (thedst_neon, guarded_dst_neon, fixed_length);

    for (func_loop = 0; func_loop < MAX_VEC_COMPONENTS; func_loop++)
    {
        for (loop = 0; loop < TEST_ITERATION; loop++)
        {
            vec_size = func_loop + 1;

            GUARD_ARRAY (thedst_c, loop * vec_size);
            GUARD_ARRAY (thedst_neon, loop * vec_size);

            if (func_loop == 0)
            {
                ftbl_3args_cst[2 * func_loop] (thedst_c, thecst[0], loop);
                ftbl_3args_cst[2 * func_loop + 1] (thedst_neon, thecst[0], loop);
            }
            else
            {
                ftbl_3args[2 * func_loop] (thedst_c, thecst, loop);
                ftbl_3args[2 * func_loop + 1] (thedst_neon, thecst, loop);
            }

            CHECK_ARRAY_GUARD (thedst_c, loop * vec_size);
            CHECK_ARRAY_GUARD (thedst_neon, loop * vec_size);

            for (pos = 0; pos < loop; pos++)
            {
#ifdef DEBUG_TRACE
                fprintf (stdout, "func: %d loop count: %d position: %d \n", func_loop, loop, pos);
                for (i = 0; i < vec_size; i++)
                {
                    fprintf (stdout, "thesrc->%d: %e [0x%04X] \n", i, thesrc1[pos * vec_size + i], * (ne10_uint32_t*) &thesrc1[pos * vec_size + i]);
                    fprintf (stdout, "thecst->%d: %e [0x%04X] \n", i, thecst[i], * (ne10_uint32_t*) &thecst[i]);
                }
#endif
                assert_float_vec_equal (&thedst_c[pos * vec_size], &thedst_neon[pos * vec_size], ERROR_MARGIN_SMALL, vec_size);
            }
        }
    }
    free (guarded_cst);
    free (guarded_dst_c);
    free (guarded_dst_neon);
#endif

#ifdef PERFORMANCE_TEST
    fprintf (stdout, "%25s%20s%20s%20s%20s\n", "N-component Vector", "C Time in ms", "NEON Time in ms", "Time Savings", "Performance Ratio");
    perftest_length = PERF_TEST_ITERATION * MAX_VEC_COMPONENTS;
    /* init src memory */
    NE10_SRC_ALLOC (perftest_thecst, perftest_guarded_cst, MAX_VEC_COMPONENTS); // 16 extra bytes at the begining and 16 extra bytes at the end

    /* init dst memory */
    NE10_DST_ALLOC (perftest_thedst_c, perftest_guarded_dst_c, perftest_length);
    NE10_DST_ALLOC (perftest_thedst_neon, perftest_guarded_dst_neon, perftest_length);

    for (func_loop = 0; func_loop < 1; func_loop++)
    {
        GET_TIME (time_c,
                  for (loop = 0; loop < PERF_TEST_ITERATION; loop++) ftbl_3args_cst[2 * func_loop] (perftest_thedst_c, perftest_thecst[0], loop);
                 );
        GET_TIME (time_neon,
                  for (loop = 0; loop < PERF_TEST_ITERATION; loop++) ftbl_3args_cst[2 * func_loop + 1] (perftest_thedst_neon, perftest_thecst[0], loop);
                 );
        time_speedup = (ne10_float32_t) time_c / time_neon;
        time_savings = ( ( (ne10_float32_t) (time_c - time_neon)) / time_c) * 100;
        ne10_log (__FUNCTION__, "%25d%20lld%20lld%19.2f%%%18.2f:1\n", func_loop + 1, time_c, time_neon, time_savings, time_speedup);
    }
    for (; func_loop < MAX_VEC_COMPONENTS; func_loop++)
    {
        GET_TIME (time_c,
                  for (loop = 0; loop < PERF_TEST_ITERATION; loop++) ftbl_3args[2 * func_loop] (perftest_thedst_c, perftest_thecst, loop);
                 );
        GET_TIME (time_neon,
                  for (loop = 0; loop < PERF_TEST_ITERATION; loop++) ftbl_3args[2 * func_loop + 1] (perftest_thedst_neon, perftest_thecst, loop);
                 );
        time_speedup = (ne10_float32_t) time_c / time_neon;
        time_savings = ( ( (ne10_float32_t) (time_c - time_neon)) / time_c) * 100;
        ne10_log (__FUNCTION__, "%25d%20lld%20lld%19.2f%%%18.2f:1\n", func_loop + 1, time_c, time_neon, time_savings, time_speedup);
    }

    free (perftest_guarded_cst);
    free (perftest_guarded_dst_c);
    free (perftest_guarded_dst_neon);
#endif

    fprintf (stdout, "----------%30s end\n", __FUNCTION__);
#undef MAX_VEC_COMPONENTS
}

void test_subc_case0()
{
#define MAX_VEC_COMPONENTS 4
    ne10_int32_t loop;
    ne10_int32_t i;
    ne10_int32_t func_loop;
    ne10_int32_t vec_size;
    ne10_int32_t pos;

    fprintf (stdout, "----------%30s start\n", __FUNCTION__);

    /* init function table */
    memset (ftbl_4args, 0, sizeof (ftbl_4args));
    memset (ftbl_4args_cst, 0, sizeof (ftbl_4args_cst));
    ftbl_4args_cst[ 0] = (ne10_func_4args_cst_t) ne10_subc_float_c;
    ftbl_4args_cst[ 1] = (ne10_func_4args_cst_t) ne10_subc_float_neon;
    ftbl_4args[ 2] = (ne10_func_4args_t) ne10_subc_vec2f_c;
    ftbl_4args[ 3] = (ne10_func_4args_t) ne10_subc_vec2f_neon;
    ftbl_4args[ 4] = (ne10_func_4args_t) ne10_subc_vec3f_c;
    ftbl_4args[ 5] = (ne10_func_4args_t) ne10_subc_vec3f_neon;
    ftbl_4args[ 6] = (ne10_func_4args_t) ne10_subc_vec4f_c;
    ftbl_4args[ 7] = (ne10_func_4args_t) ne10_subc_vec4f_neon;

#if defined (SMOKE_TEST)||(REGRESSION_TEST)
    const ne10_uint32_t fixed_length = TEST_ITERATION * MAX_VEC_COMPONENTS;

    /* init src memory */
    NE10_SRC_ALLOC (thesrc1, guarded_src1, fixed_length); // 16 extra bytes at the begining and 16 extra bytes at the end
    NE10_SRC_ALLOC (thecst, guarded_cst, MAX_VEC_COMPONENTS); // 16 extra bytes at the begining and 16 extra bytes at the end

    /* init dst memory */
    NE10_DST_ALLOC (thedst_c, guarded_dst_c, fixed_length);
    NE10_DST_ALLOC (thedst_neon, guarded_dst_neon, fixed_length);

    for (func_loop = 0; func_loop < MAX_VEC_COMPONENTS; func_loop++)
    {
        for (loop = 0; loop < TEST_ITERATION; loop++)
        {
            vec_size = func_loop + 1;

            GUARD_ARRAY (thedst_c, loop * vec_size);
            GUARD_ARRAY (thedst_neon, loop * vec_size);

            if (func_loop == 0)
            {
                ftbl_4args_cst[2 * func_loop] (thedst_c, thesrc1, thecst[0], loop);
                ftbl_4args_cst[2 * func_loop + 1] (thedst_neon, thesrc1, thecst[0], loop);
            }
            else
            {
                ftbl_4args[2 * func_loop] (thedst_c, thesrc1, thecst, loop);
                ftbl_4args[2 * func_loop + 1] (thedst_neon, thesrc1, thecst, loop);
            }

            CHECK_ARRAY_GUARD (thedst_c, loop * vec_size);
            CHECK_ARRAY_GUARD (thedst_neon, loop * vec_size);

            for (pos = 0; pos < loop; pos++)
            {
#ifdef DEBUG_TRACE
                fprintf (stdout, "func: %d loop count: %d position: %d \n", func_loop, loop, pos);
                for (i = 0; i < vec_size; i++)
                {
                    fprintf (stdout, "thesrc->%d: %e [0x%04X] \n", i, thesrc1[pos * vec_size + i], * (ne10_uint32_t*) &thesrc1[pos * vec_size + i]);
                    fprintf (stdout, "thecst->%d: %e [0x%04X] \n", i, thecst[i], * (ne10_uint32_t*) &thecst[i]);
                }
#endif
                assert_float_vec_equal (&thedst_c[pos * vec_size], &thedst_neon[pos * vec_size], ERROR_MARGIN_SMALL, vec_size);
            }
        }
    }
    free (guarded_src1);
    free (guarded_cst);
    free (guarded_dst_c);
    free (guarded_dst_neon);
#endif

#ifdef PERFORMANCE_TEST
    fprintf (stdout, "%25s%20s%20s%20s%20s\n", "N-component Vector", "C Time in ms", "NEON Time in ms", "Time Savings", "Performance Ratio");
    perftest_length = PERF_TEST_ITERATION * MAX_VEC_COMPONENTS;
    /* init src memory */
    NE10_SRC_ALLOC (perftest_thesrc1, perftest_guarded_src1, perftest_length); // 16 extra bytes at the begining and 16 extra bytes at the end
    NE10_SRC_ALLOC (perftest_thecst, perftest_guarded_cst, MAX_VEC_COMPONENTS); // 16 extra bytes at the begining and 16 extra bytes at the end

    /* init dst memory */
    NE10_DST_ALLOC (perftest_thedst_c, perftest_guarded_dst_c, perftest_length);
    NE10_DST_ALLOC (perftest_thedst_neon, perftest_guarded_dst_neon, perftest_length);

    for (func_loop = 0; func_loop < 1; func_loop++)
    {
        GET_TIME (time_c,
                  for (loop = 0; loop < PERF_TEST_ITERATION; loop++) ftbl_4args_cst[2 * func_loop] (perftest_thedst_c, perftest_thesrc1, perftest_thecst[0], loop);
                 );
        GET_TIME (time_neon,
                  for (loop = 0; loop < PERF_TEST_ITERATION; loop++) ftbl_4args_cst[2 * func_loop + 1] (perftest_thedst_neon, perftest_thesrc1, perftest_thecst[0], loop);
                 );
        time_speedup = (ne10_float32_t) time_c / time_neon;
        time_savings = ( ( (ne10_float32_t) (time_c - time_neon)) / time_c) * 100;
        ne10_log (__FUNCTION__, "%25d%20lld%20lld%19.2f%%%18.2f:1\n", func_loop + 1, time_c, time_neon, time_savings, time_speedup);
    }
    for (; func_loop < MAX_VEC_COMPONENTS; func_loop++)
    {
        GET_TIME (time_c,
                  for (loop = 0; loop < PERF_TEST_ITERATION; loop++) ftbl_4args[2 * func_loop] (perftest_thedst_c, perftest_thesrc1, perftest_thecst, loop);
                 );
        GET_TIME (time_neon,
                  for (loop = 0; loop < PERF_TEST_ITERATION; loop++) ftbl_4args[2 * func_loop + 1] (perftest_thedst_neon, perftest_thesrc1, perftest_thecst, loop);
                 );
        time_speedup = (ne10_float32_t) time_c / time_neon;
        time_savings = ( ( (ne10_float32_t) (time_c - time_neon)) / time_c) * 100;
        ne10_log (__FUNCTION__, "%25d%20lld%20lld%19.2f%%%18.2f:1\n", func_loop + 1, time_c, time_neon, time_savings, time_speedup);
    }

    free (perftest_guarded_src1);
    free (perftest_guarded_cst);
    free (perftest_guarded_dst_c);
    free (perftest_guarded_dst_neon);
#endif

    fprintf (stdout, "----------%30s end\n", __FUNCTION__);
#undef MAX_VEC_COMPONENTS
}

void test_sub_case0()
{
#define MAX_VEC_COMPONENTS 4
    ne10_int32_t loop;
    ne10_int32_t i;
    ne10_int32_t func_loop;
    ne10_int32_t vec_size;
    ne10_int32_t pos;

    fprintf (stdout, "----------%30s start\n", __FUNCTION__);

    /* init function table */
    memset (ftbl_4args, 0, sizeof (ftbl_4args));
    ftbl_4args[ 0] = (ne10_func_4args_t) ne10_sub_float_c;
    ftbl_4args[ 1] = (ne10_func_4args_t) ne10_sub_float_neon;
    ftbl_4args[ 2] = (ne10_func_4args_t) ne10_sub_vec2f_c;
    ftbl_4args[ 3] = (ne10_func_4args_t) ne10_sub_vec2f_neon;
    ftbl_4args[ 4] = (ne10_func_4args_t) ne10_sub_vec3f_c;
    ftbl_4args[ 5] = (ne10_func_4args_t) ne10_sub_vec3f_neon;
    ftbl_4args[ 6] = (ne10_func_4args_t) ne10_sub_vec4f_c;
    ftbl_4args[ 7] = (ne10_func_4args_t) ne10_sub_vec4f_neon;

#if defined (SMOKE_TEST)||(REGRESSION_TEST)
    const ne10_uint32_t fixed_length = TEST_ITERATION * MAX_VEC_COMPONENTS;

    /* init src memory */
    NE10_SRC_ALLOC (thesrc1, guarded_src1, fixed_length); // 16 extra bytes at the begining and 16 extra bytes at the end
    NE10_SRC_ALLOC (thesrc2, guarded_src2, fixed_length); // 16 extra bytes at the begining and 16 extra bytes at the end

    /* init dst memory */
    NE10_DST_ALLOC (thedst_c, guarded_dst_c, fixed_length);
    NE10_DST_ALLOC (thedst_neon, guarded_dst_neon, fixed_length);

    for (func_loop = 0; func_loop < MAX_VEC_COMPONENTS; func_loop++)
    {
        for (loop = 0; loop < TEST_ITERATION; loop++)
        {
            vec_size = func_loop + 1;

            GUARD_ARRAY (thedst_c, loop * vec_size);
            GUARD_ARRAY (thedst_neon, loop * vec_size);

            ftbl_4args[2 * func_loop] (thedst_c, thesrc1, thesrc2, loop);
            ftbl_4args[2 * func_loop + 1] (thedst_neon, thesrc1, thesrc2, loop);

            CHECK_ARRAY_GUARD (thedst_c, loop * vec_size);
            CHECK_ARRAY_GUARD (thedst_neon, loop * vec_size);

            for (pos = 0; pos < loop; pos++)
            {
#ifdef DEBUG_TRACE
                fprintf (stdout, "func: %d loop count: %d position: %d \n", func_loop, loop, pos);
                for (i = 0; i < vec_size; i++)
                {
                    fprintf (stdout, "thesrc1->%d: %e [0x%04X] \n", i, thesrc1[pos * vec_size + i], * (ne10_uint32_t*) &thesrc1[pos * vec_size + i]);
                    fprintf (stdout, "thesrc2->%d: %e [0x%04X] \n", i, thesrc2[pos * vec_size + i], * (ne10_uint32_t*) &thesrc2[pos * vec_size + i]);
                }
#endif
                assert_float_vec_equal (&thedst_c[pos * vec_size], &thedst_neon[pos * vec_size], ERROR_MARGIN_SMALL, vec_size);
            }
        }
    }
    free (guarded_src1);
    free (guarded_src2);
    free (guarded_dst_c);
    free (guarded_dst_neon);
#endif

#ifdef PERFORMANCE_TEST
    fprintf (stdout, "%25s%20s%20s%20s%20s\n", "N-component Vector", "C Time in ms", "NEON Time in ms", "Time Savings", "Performance Ratio");
    perftest_length = PERF_TEST_ITERATION * MAX_VEC_COMPONENTS;
    /* init src memory */
    NE10_SRC_ALLOC (perftest_thesrc1, perftest_guarded_src1, perftest_length); // 16 extra bytes at the begining and 16 extra bytes at the end
    NE10_SRC_ALLOC (perftest_thesrc2, perftest_guarded_src2, perftest_length); // 16 extra bytes at the begining and 16 extra bytes at the end

    /* init dst memory */
    NE10_DST_ALLOC (perftest_thedst_c, perftest_guarded_dst_c, perftest_length);
    NE10_DST_ALLOC (perftest_thedst_neon, perftest_guarded_dst_neon, perftest_length);

    for (func_loop = 0; func_loop < MAX_VEC_COMPONENTS; func_loop++)
    {
        GET_TIME (time_c,
                  for (loop = 0; loop < PERF_TEST_ITERATION; loop++) ftbl_4args[2 * func_loop] (perftest_thedst_c, perftest_thesrc1, perftest_thesrc2, loop);
                 );
        GET_TIME (time_neon,
                  for (loop = 0; loop < PERF_TEST_ITERATION; loop++) ftbl_4args[2 * func_loop + 1] (perftest_thedst_neon, perftest_thesrc1, perftest_thesrc2, loop);
                 );
        time_speedup = (ne10_float32_t) time_c / time_neon;
        time_savings = ( ( (ne10_float32_t) (time_c - time_neon)) / time_c) * 100;
        ne10_log (__FUNCTION__, "%25d%20lld%20lld%19.2f%%%18.2f:1\n", func_loop + 1, time_c, time_neon, time_savings, time_speedup);
    }

    free (perftest_guarded_src1);
    free (perftest_guarded_src2);
    free (perftest_guarded_dst_c);
    free (perftest_guarded_dst_neon);
#endif

    fprintf (stdout, "----------%30s end\n", __FUNCTION__);
#undef MAX_VEC_COMPONENTS
}

void test_addmat_case0()
{
#define MAX_VEC_COMPONENTS 4
    ne10_int32_t loop;
    ne10_int32_t i;
    ne10_int32_t func_loop;
    ne10_int32_t vec_size;
    ne10_int32_t pos;

    fprintf (stdout, "----------%30s start\n", __FUNCTION__);

    /* init function table */
    memset (ftbl_4args, 0, sizeof (ftbl_4args));
    ftbl_4args[ 2] = (ne10_func_4args_t) ne10_addmat_2x2f_c;
    ftbl_4args[ 3] = (ne10_func_4args_t) ne10_addmat_2x2f_neon;
    ftbl_4args[ 4] = (ne10_func_4args_t) ne10_addmat_3x3f_c;
    ftbl_4args[ 5] = (ne10_func_4args_t) ne10_addmat_3x3f_neon;
    ftbl_4args[ 6] = (ne10_func_4args_t) ne10_addmat_4x4f_c;
    ftbl_4args[ 7] = (ne10_func_4args_t) ne10_addmat_4x4f_neon;

#if defined (SMOKE_TEST)||(REGRESSION_TEST)
    const ne10_uint32_t fixed_length = TEST_ITERATION * MAX_VEC_COMPONENTS * MAX_VEC_COMPONENTS;

    /* init src memory */
    NE10_SRC_ALLOC (thesrc1, guarded_src1, fixed_length); // 16 extra bytes at the begining and 16 extra bytes at the end
    NE10_SRC_ALLOC (thesrc2, guarded_src2, fixed_length); // 16 extra bytes at the begining and 16 extra bytes at the end

    /* init dst memory */
    NE10_DST_ALLOC (thedst_c, guarded_dst_c, fixed_length);
    NE10_DST_ALLOC (thedst_neon, guarded_dst_neon, fixed_length);

    for (func_loop = 1; func_loop < MAX_VEC_COMPONENTS; func_loop++)
    {
        for (loop = 0; loop < TEST_ITERATION; loop++)
        {
            vec_size = (func_loop + 1) * (func_loop + 1);

            GUARD_ARRAY (thedst_c, loop * vec_size);
            GUARD_ARRAY (thedst_neon, loop * vec_size);

            ftbl_4args[2 * func_loop] (thedst_c, thesrc1, thesrc2, loop);
            ftbl_4args[2 * func_loop + 1] (thedst_neon, thesrc1, thesrc2, loop);

            CHECK_ARRAY_GUARD (thedst_c, loop * vec_size);
            CHECK_ARRAY_GUARD (thedst_neon, loop * vec_size);

            for (pos = 0; pos < loop; pos++)
            {
#ifdef DEBUG_TRACE
                fprintf (stdout, "func: %d loop count: %d position: %d \n", func_loop, loop, pos);
                for (i = 0; i < vec_size; i++)
                {
                    fprintf (stdout, "thesrc1->%d: %e [0x%04X] \n", i, thesrc1[pos * vec_size + i], * (ne10_uint32_t*) &thesrc1[pos * vec_size + i]);
                    fprintf (stdout, "thesrc2->%d: %e [0x%04X] \n", i, thesrc2[pos * vec_size + i], * (ne10_uint32_t*) &thesrc2[pos * vec_size + i]);
                }
#endif
                assert_float_vec_equal (&thedst_c[pos * vec_size], &thedst_neon[pos * vec_size], ERROR_MARGIN_SMALL, vec_size);
            }
        }
    }
    free (guarded_src1);
    free (guarded_src2);
    free (guarded_dst_c);
    free (guarded_dst_neon);
#endif

#ifdef PERFORMANCE_TEST
    fprintf (stdout, "%25s%20s%20s%20s%20s\n", "N-component Vector", "C Time in ms", "NEON Time in ms", "Time Savings", "Performance Ratio");
    perftest_length = PERF_TEST_ITERATION * MAX_VEC_COMPONENTS * MAX_VEC_COMPONENTS;
    /* init src memory */
    NE10_SRC_ALLOC (perftest_thesrc1, perftest_guarded_src1, perftest_length); // 16 extra bytes at the begining and 16 extra bytes at the end
    NE10_SRC_ALLOC (perftest_thesrc2, perftest_guarded_src2, perftest_length); // 16 extra bytes at the begining and 16 extra bytes at the end

    /* init dst memory */
    NE10_DST_ALLOC (perftest_thedst_c, perftest_guarded_dst_c, perftest_length);
    NE10_DST_ALLOC (perftest_thedst_neon, perftest_guarded_dst_neon, perftest_length);

    for (func_loop = 1; func_loop < MAX_VEC_COMPONENTS; func_loop++)
    {
        GET_TIME (time_c,
                  for (loop = 0; loop < PERF_TEST_ITERATION; loop++) ftbl_4args[2 * func_loop] (perftest_thedst_c, perftest_thesrc1, perftest_thesrc2, loop);
                 );
        GET_TIME (time_neon,
                  for (loop = 0; loop < PERF_TEST_ITERATION; loop++) ftbl_4args[2 * func_loop + 1] (perftest_thedst_neon, perftest_thesrc1, perftest_thesrc2, loop);
                 );
        time_speedup = (ne10_float32_t) time_c / time_neon;
        time_savings = ( ( (ne10_float32_t) (time_c - time_neon)) / time_c) * 100;
        ne10_log (__FUNCTION__, "%25d%20lld%20lld%19.2f%%%18.2f:1\n", func_loop + 1, time_c, time_neon, time_savings, time_speedup);
    }

    free (perftest_guarded_src1);
    free (perftest_guarded_src2);
    free (perftest_guarded_dst_c);
    free (perftest_guarded_dst_neon);
#endif

    fprintf (stdout, "----------%30s end\n", __FUNCTION__);
#undef MAX_VEC_COMPONENTS
}

void test_detmat_case0()
{
#define MAX_VEC_COMPONENTS 4
    ne10_int32_t loop;
    ne10_int32_t i;
    ne10_int32_t func_loop;
    ne10_int32_t vec_size;
    ne10_int32_t pos;

    fprintf (stdout, "----------%30s start\n", __FUNCTION__);

    /* init function table */
    memset (ftbl_3args, 0, sizeof (ftbl_3args));
    ftbl_3args[ 2] = (ne10_func_3args_t) ne10_detmat_2x2f_c;
    ftbl_3args[ 3] = (ne10_func_3args_t) ne10_detmat_2x2f_neon;
    ftbl_3args[ 4] = (ne10_func_3args_t) ne10_detmat_3x3f_c;
    ftbl_3args[ 5] = (ne10_func_3args_t) ne10_detmat_3x3f_neon;
    ftbl_3args[ 6] = (ne10_func_3args_t) ne10_detmat_4x4f_c;
    ftbl_3args[ 7] = (ne10_func_3args_t) ne10_detmat_4x4f_neon;

#if defined (SMOKE_TEST)||(REGRESSION_TEST)
    const ne10_uint32_t fixed_length = TEST_ITERATION * MAX_VEC_COMPONENTS * MAX_VEC_COMPONENTS;

    /* init src memory */
    NE10_SRC_ALLOC_LIMIT (thesrc1, guarded_src1, fixed_length); // 16 extra bytes at the begining and 16 extra bytes at the end

    /* init dst memory */
    NE10_DST_ALLOC (thedst_c, guarded_dst_c, fixed_length);
    NE10_DST_ALLOC (thedst_neon, guarded_dst_neon, fixed_length);

    for (func_loop = 1; func_loop < MAX_VEC_COMPONENTS; func_loop++)
    {
        for (loop = 0; loop < TEST_ITERATION; loop++)
        {
            vec_size = (func_loop + 1) * (func_loop + 1);

            GUARD_ARRAY (thedst_c, loop);
            GUARD_ARRAY (thedst_neon, loop);

            ftbl_3args[2 * func_loop] (thedst_c, thesrc1, loop);
            ftbl_3args[2 * func_loop + 1] (thedst_neon, thesrc1, loop);

            CHECK_ARRAY_GUARD (thedst_c, loop);
            CHECK_ARRAY_GUARD (thedst_neon, loop);

            for (pos = 0; pos < loop; pos++)
            {
#ifdef DEBUG_TRACE
                fprintf (stdout, "func: %d loop count: %d position: %d \n", func_loop, loop, pos);
                for (i = 0; i < vec_size; i++)
                {
                    fprintf (stdout, "thesrc1->%d: %e [0x%04X] \n", i, thesrc1[pos * vec_size + i], * (ne10_uint32_t*) &thesrc1[pos * vec_size + i]);
                }
#endif
                assert_float_vec_equal (&thedst_c[pos * vec_size], &thedst_neon[pos * vec_size], ERROR_MARGIN_SMALL, 1);
            }
        }
    }
    free (guarded_src1);
    free (guarded_dst_c);
    free (guarded_dst_neon);
#endif

#ifdef PERFORMANCE_TEST
    fprintf (stdout, "%25s%20s%20s%20s%20s\n", "N-component Vector", "C Time in ms", "NEON Time in ms", "Time Savings", "Performance Ratio");
    perftest_length = PERF_TEST_ITERATION * MAX_VEC_COMPONENTS * MAX_VEC_COMPONENTS;
    /* init src memory */
    NE10_SRC_ALLOC (perftest_thesrc1, perftest_guarded_src1, perftest_length); // 16 extra bytes at the begining and 16 extra bytes at the end

    /* init dst memory */
    NE10_DST_ALLOC (perftest_thedst_c, perftest_guarded_dst_c, perftest_length);
    NE10_DST_ALLOC (perftest_thedst_neon, perftest_guarded_dst_neon, perftest_length);

    for (func_loop = 1; func_loop < MAX_VEC_COMPONENTS; func_loop++)
    {
        GET_TIME (time_c,
                  for (loop = 0; loop < PERF_TEST_ITERATION; loop++) ftbl_3args[2 * func_loop] (perftest_thedst_c, perftest_thesrc1, loop);
                 );
        GET_TIME (time_neon,
                  for (loop = 0; loop < PERF_TEST_ITERATION; loop++) ftbl_3args[2 * func_loop + 1] (perftest_thedst_neon, perftest_thesrc1, loop);
                 );
        time_speedup = (ne10_float32_t) time_c / time_neon;
        time_savings = ( ( (ne10_float32_t) (time_c - time_neon)) / time_c) * 100;
        ne10_log (__FUNCTION__, "%25d%20lld%20lld%19.2f%%%18.2f:1\n", func_loop + 1, time_c, time_neon, time_savings, time_speedup);
    }

    free (perftest_guarded_src1);
    free (perftest_guarded_dst_c);
    free (perftest_guarded_dst_neon);
#endif

    fprintf (stdout, "----------%30s end\n", __FUNCTION__);
#undef MAX_VEC_COMPONENTS
}

void test_identitymat_case0()
{
#define MAX_VEC_COMPONENTS 4
    ne10_int32_t loop;
    ne10_int32_t i;
    ne10_int32_t func_loop;
    ne10_int32_t vec_size;
    ne10_int32_t pos;

    fprintf (stdout, "----------%30s start\n", __FUNCTION__);

    /* init function table */
    memset (ftbl_2args, 0, sizeof (ftbl_2args));
    ftbl_2args[ 2] = (ne10_func_2args_t) ne10_identitymat_2x2f_c;
    ftbl_2args[ 3] = (ne10_func_2args_t) ne10_identitymat_2x2f_neon;
    ftbl_2args[ 4] = (ne10_func_2args_t) ne10_identitymat_3x3f_c;
    ftbl_2args[ 5] = (ne10_func_2args_t) ne10_identitymat_3x3f_neon;
    ftbl_2args[ 6] = (ne10_func_2args_t) ne10_identitymat_4x4f_c;
    ftbl_2args[ 7] = (ne10_func_2args_t) ne10_identitymat_4x4f_neon;

#if defined (SMOKE_TEST)||(REGRESSION_TEST)
    const ne10_uint32_t fixed_length = TEST_ITERATION * MAX_VEC_COMPONENTS * MAX_VEC_COMPONENTS;

    /* init dst memory */
    NE10_DST_ALLOC (thedst_c, guarded_dst_c, fixed_length);
    NE10_DST_ALLOC (thedst_neon, guarded_dst_neon, fixed_length);

    for (func_loop = 1; func_loop < MAX_VEC_COMPONENTS; func_loop++)
    {
        for (loop = 0; loop < TEST_ITERATION; loop++)
        {
            vec_size = (func_loop + 1) * (func_loop + 1);

            GUARD_ARRAY (thedst_c, loop * vec_size);
            GUARD_ARRAY (thedst_neon, loop * vec_size);

            ftbl_2args[2 * func_loop] (thedst_c, loop);
            ftbl_2args[2 * func_loop + 1] (thedst_neon, loop);

            CHECK_ARRAY_GUARD (thedst_c, loop * vec_size);
            CHECK_ARRAY_GUARD (thedst_neon, loop * vec_size);

            for (pos = 0; pos < loop; pos++)
            {
#ifdef DEBUG_TRACE
                fprintf (stdout, "func: %d loop count: %d position: %d \n", func_loop, loop, pos);
#endif
                assert_float_vec_equal (&thedst_c[pos * vec_size], &thedst_neon[pos * vec_size], ERROR_MARGIN_SMALL, vec_size);
            }
        }
    }
    free (guarded_dst_c);
    free (guarded_dst_neon);
#endif

#ifdef PERFORMANCE_TEST
    fprintf (stdout, "%25s%20s%20s%20s%20s\n", "N-component Vector", "C Time in ms", "NEON Time in ms", "Time Savings", "Performance Ratio");
    perftest_length = PERF_TEST_ITERATION * MAX_VEC_COMPONENTS * MAX_VEC_COMPONENTS;
    /* init dst memory */
    NE10_DST_ALLOC (perftest_thedst_c, perftest_guarded_dst_c, perftest_length);
    NE10_DST_ALLOC (perftest_thedst_neon, perftest_guarded_dst_neon, perftest_length);

    for (func_loop = 1; func_loop < MAX_VEC_COMPONENTS; func_loop++)
    {
        GET_TIME (time_c,
                  for (loop = 0; loop < PERF_TEST_ITERATION; loop++) ftbl_2args[2 * func_loop] (perftest_thedst_c, loop);
                 );
        GET_TIME (time_neon,
                  for (loop = 0; loop < PERF_TEST_ITERATION; loop++) ftbl_2args[2 * func_loop + 1] (perftest_thedst_neon, loop);
                 );
        time_speedup = (ne10_float32_t) time_c / time_neon;
        time_savings = ( ( (ne10_float32_t) (time_c - time_neon)) / time_c) * 100;
        ne10_log (__FUNCTION__, "%25d%20lld%20lld%19.2f%%%18.2f:1\n", func_loop + 1, time_c, time_neon, time_savings, time_speedup);
    }

    free (perftest_guarded_dst_c);
    free (perftest_guarded_dst_neon);
#endif

    fprintf (stdout, "----------%30s end\n", __FUNCTION__);
#undef MAX_VEC_COMPONENTS
}

void test_invmat_case0()
{
#define MAX_VEC_COMPONENTS 4
    ne10_int32_t loop;
    ne10_int32_t i;
    ne10_int32_t func_loop;
    ne10_int32_t vec_size;
    ne10_int32_t pos;

    fprintf (stdout, "----------%30s start\n", __FUNCTION__);

    /* init function table */
    memset (ftbl_3args, 0, sizeof (ftbl_3args));
    ftbl_3args[ 2] = (ne10_func_3args_t) ne10_invmat_2x2f_c;
    ftbl_3args[ 3] = (ne10_func_3args_t) ne10_invmat_2x2f_neon;
    ftbl_3args[ 4] = (ne10_func_3args_t) ne10_invmat_3x3f_c;
    ftbl_3args[ 5] = (ne10_func_3args_t) ne10_invmat_3x3f_neon;
    ftbl_3args[ 6] = (ne10_func_3args_t) ne10_invmat_4x4f_c;
    ftbl_3args[ 7] = (ne10_func_3args_t) ne10_invmat_4x4f_neon;

#if defined (SMOKE_TEST)||(REGRESSION_TEST)
    const ne10_uint32_t fixed_length = TEST_ITERATION * MAX_VEC_COMPONENTS * MAX_VEC_COMPONENTS;

    /* init src memory */
    NE10_SRC_ALLOC_LIMIT (thesrc1, guarded_src1, fixed_length); // 16 extra bytes at the begining and 16 extra bytes at the end

    /* init dst memory */
    NE10_DST_ALLOC (thedst_c, guarded_dst_c, fixed_length);
    NE10_DST_ALLOC (thedst_neon, guarded_dst_neon, fixed_length);

    for (func_loop = 1; func_loop < MAX_VEC_COMPONENTS; func_loop++)
    {
        for (loop = 0; loop < TEST_ITERATION; loop++)
        {
            vec_size = (func_loop + 1) * (func_loop + 1);

            GUARD_ARRAY (thedst_c, loop * vec_size);
            GUARD_ARRAY (thedst_neon, loop * vec_size);

            ftbl_3args[2 * func_loop] (thedst_c, thesrc1, loop);
            ftbl_3args[2 * func_loop + 1] (thedst_neon, thesrc1, loop);

            CHECK_ARRAY_GUARD (thedst_c, loop * vec_size);
            CHECK_ARRAY_GUARD (thedst_neon, loop * vec_size);

            for (pos = 0; pos < loop; pos++)
            {
#ifdef DEBUG_TRACE
                fprintf (stdout, "func: %d loop count: %d position: %d \n", func_loop, loop, pos);
                for (i = 0; i < vec_size; i++)
                {
                    fprintf (stdout, "thesrc1->%d: %e [0x%04X] \n", i, thesrc1[pos * vec_size + i], * (ne10_uint32_t*) &thesrc1[pos * vec_size + i]);
                }
#endif
                assert_float_vec_equal (&thedst_c[pos * vec_size], &thedst_neon[pos * vec_size], ERROR_MARGIN_LARGE, vec_size);
            }
        }
    }
    free (guarded_src1);
    free (guarded_dst_c);
    free (guarded_dst_neon);
#endif

#ifdef PERFORMANCE_TEST
    fprintf (stdout, "%25s%20s%20s%20s%20s\n", "N-component Vector", "C Time in ms", "NEON Time in ms", "Time Savings", "Performance Ratio");
    perftest_length = PERF_TEST_ITERATION * MAX_VEC_COMPONENTS * MAX_VEC_COMPONENTS;
    /* init src memory */
    NE10_SRC_ALLOC (perftest_thesrc1, perftest_guarded_src1, perftest_length); // 16 extra bytes at the begining and 16 extra bytes at the end

    /* init dst memory */
    NE10_DST_ALLOC (perftest_thedst_c, perftest_guarded_dst_c, perftest_length);
    NE10_DST_ALLOC (perftest_thedst_neon, perftest_guarded_dst_neon, perftest_length);

    for (func_loop = 1; func_loop < MAX_VEC_COMPONENTS; func_loop++)
    {
        GET_TIME (time_c,
                  for (loop = 0; loop < PERF_TEST_ITERATION; loop++) ftbl_3args[2 * func_loop] (perftest_thedst_c, perftest_thesrc1, loop);
                 );
        GET_TIME (time_neon,
                  for (loop = 0; loop < PERF_TEST_ITERATION; loop++) ftbl_3args[2 * func_loop + 1] (perftest_thedst_neon, perftest_thesrc1, loop);
                 );
        time_speedup = (ne10_float32_t) time_c / time_neon;
        time_savings = ( ( (ne10_float32_t) (time_c - time_neon)) / time_c) * 100;
        ne10_log (__FUNCTION__, "%25d%20lld%20lld%19.2f%%%18.2f:1\n", func_loop + 1, time_c, time_neon, time_savings, time_speedup);
    }

    free (perftest_guarded_src1);
    free (perftest_guarded_dst_c);
    free (perftest_guarded_dst_neon);
#endif

    fprintf (stdout, "----------%30s end\n", __FUNCTION__);
#undef MAX_VEC_COMPONENTS
}

void test_mulmat_case0()
{
#define MAX_VEC_COMPONENTS 4
    ne10_int32_t loop;
    ne10_int32_t i;
    ne10_int32_t func_loop;
    ne10_int32_t vec_size;
    ne10_int32_t pos;

    fprintf (stdout, "----------%30s start\n", __FUNCTION__);

    /* init function table */
    memset (ftbl_4args, 0, sizeof (ftbl_4args));
    ftbl_4args[ 2] = (ne10_func_4args_t) ne10_mulmat_2x2f_c;
    ftbl_4args[ 3] = (ne10_func_4args_t) ne10_mulmat_2x2f_neon;
    ftbl_4args[ 4] = (ne10_func_4args_t) ne10_mulmat_3x3f_c;
    ftbl_4args[ 5] = (ne10_func_4args_t) ne10_mulmat_3x3f_neon;
    ftbl_4args[ 6] = (ne10_func_4args_t) ne10_mulmat_4x4f_c;
    ftbl_4args[ 7] = (ne10_func_4args_t) ne10_mulmat_4x4f_neon;

#if defined (SMOKE_TEST)||(REGRESSION_TEST)
    const ne10_uint32_t fixed_length = TEST_ITERATION * MAX_VEC_COMPONENTS * MAX_VEC_COMPONENTS;

    /* init src memory */
    NE10_SRC_ALLOC_LIMIT (thesrc1, guarded_src1, fixed_length); // 16 extra bytes at the begining and 16 extra bytes at the end
    NE10_SRC_ALLOC_LIMIT (thesrc2, guarded_src2, fixed_length); // 16 extra bytes at the begining and 16 extra bytes at the end

    /* init dst memory */
    NE10_DST_ALLOC (thedst_c, guarded_dst_c, fixed_length);
    NE10_DST_ALLOC (thedst_neon, guarded_dst_neon, fixed_length);

    for (func_loop = 1; func_loop < MAX_VEC_COMPONENTS; func_loop++)
    {
        for (loop = 0; loop < TEST_ITERATION; loop++)
        {
            vec_size = (func_loop + 1) * (func_loop + 1);

            GUARD_ARRAY (thedst_c, loop * vec_size);
            GUARD_ARRAY (thedst_neon, loop * vec_size);

            ftbl_4args[2 * func_loop] (thedst_c, thesrc1, thesrc2, loop);
            ftbl_4args[2 * func_loop + 1] (thedst_neon, thesrc1, thesrc2, loop);

            CHECK_ARRAY_GUARD (thedst_c, loop * vec_size);
            CHECK_ARRAY_GUARD (thedst_neon, loop * vec_size);

            for (pos = 0; pos < loop; pos++)
            {
#ifdef DEBUG_TRACE
                fprintf (stdout, "func: %d loop count: %d position: %d \n", func_loop, loop, pos);
                for (i = 0; i < vec_size; i++)
                {
                    fprintf (stdout, "thesrc1->%d: %e [0x%04X] \n", i, thesrc1[pos * vec_size + i], * (ne10_uint32_t*) &thesrc1[pos * vec_size + i]);
                    fprintf (stdout, "thesrc2->%d: %e [0x%04X] \n", i, thesrc2[pos * vec_size + i], * (ne10_uint32_t*) &thesrc2[pos * vec_size + i]);
                }
#endif
                assert_float_vec_equal (&thedst_c[pos * vec_size], &thedst_neon[pos * vec_size], ERROR_MARGIN_SMALL, vec_size);
            }
        }
    }
    free (guarded_src1);
    free (guarded_src2);
    free (guarded_dst_c);
    free (guarded_dst_neon);
#endif

#ifdef PERFORMANCE_TEST
    fprintf (stdout, "%25s%20s%20s%20s%20s\n", "N-component Vector", "C Time in ms", "NEON Time in ms", "Time Savings", "Performance Ratio");
    perftest_length = PERF_TEST_ITERATION * MAX_VEC_COMPONENTS * MAX_VEC_COMPONENTS;
    /* init src memory */
    NE10_SRC_ALLOC (perftest_thesrc1, perftest_guarded_src1, perftest_length); // 16 extra bytes at the begining and 16 extra bytes at the end
    NE10_SRC_ALLOC (perftest_thesrc2, perftest_guarded_src2, perftest_length); // 16 extra bytes at the begining and 16 extra bytes at the end

    /* init dst memory */
    NE10_DST_ALLOC (perftest_thedst_c, perftest_guarded_dst_c, perftest_length);
    NE10_DST_ALLOC (perftest_thedst_neon, perftest_guarded_dst_neon, perftest_length);

    for (func_loop = 1; func_loop < MAX_VEC_COMPONENTS; func_loop++)
    {
        GET_TIME (time_c,
                  for (loop = 0; loop < PERF_TEST_ITERATION; loop++) ftbl_4args[2 * func_loop] (perftest_thedst_c, perftest_thesrc1, perftest_thesrc2, loop);
                 );
        GET_TIME (time_neon,
                  for (loop = 0; loop < PERF_TEST_ITERATION; loop++) ftbl_4args[2 * func_loop + 1] (perftest_thedst_neon, perftest_thesrc1, perftest_thesrc2, loop);
                 );
        time_speedup = (ne10_float32_t) time_c / time_neon;
        time_savings = ( ( (ne10_float32_t) (time_c - time_neon)) / time_c) * 100;
        ne10_log (__FUNCTION__, "%25d%20lld%20lld%19.2f%%%18.2f:1\n", func_loop + 1, time_c, time_neon, time_savings, time_speedup);
    }

    free (perftest_guarded_src1);
    free (perftest_guarded_src2);
    free (perftest_guarded_dst_c);
    free (perftest_guarded_dst_neon);
#endif

    fprintf (stdout, "----------%30s end\n", __FUNCTION__);
#undef MAX_VEC_COMPONENTS
}

void test_submat_case0()
{
#define MAX_VEC_COMPONENTS 4
    ne10_int32_t loop;
    ne10_int32_t i;
    ne10_int32_t func_loop;
    ne10_int32_t vec_size;
    ne10_int32_t pos;

    fprintf (stdout, "----------%30s start\n", __FUNCTION__);

    /* init function table */
    memset (ftbl_4args, 0, sizeof (ftbl_4args));
    ftbl_4args[ 2] = (ne10_func_4args_t) ne10_submat_2x2f_c;
    ftbl_4args[ 3] = (ne10_func_4args_t) ne10_submat_2x2f_neon;
    ftbl_4args[ 4] = (ne10_func_4args_t) ne10_submat_3x3f_c;
    ftbl_4args[ 5] = (ne10_func_4args_t) ne10_submat_3x3f_neon;
    ftbl_4args[ 6] = (ne10_func_4args_t) ne10_submat_4x4f_c;
    ftbl_4args[ 7] = (ne10_func_4args_t) ne10_submat_4x4f_neon;

#if defined (SMOKE_TEST)||(REGRESSION_TEST)
    const ne10_uint32_t fixed_length = TEST_ITERATION * MAX_VEC_COMPONENTS * MAX_VEC_COMPONENTS;

    /* init src memory */
    NE10_SRC_ALLOC (thesrc1, guarded_src1, fixed_length); // 16 extra bytes at the begining and 16 extra bytes at the end
    NE10_SRC_ALLOC (thesrc2, guarded_src2, fixed_length); // 16 extra bytes at the begining and 16 extra bytes at the end

    /* init dst memory */
    NE10_DST_ALLOC (thedst_c, guarded_dst_c, fixed_length);
    NE10_DST_ALLOC (thedst_neon, guarded_dst_neon, fixed_length);

    for (func_loop = 1; func_loop < MAX_VEC_COMPONENTS; func_loop++)
    {
        for (loop = 0; loop < TEST_ITERATION; loop++)
        {
            vec_size = (func_loop + 1) * (func_loop + 1);

            GUARD_ARRAY (thedst_c, loop * vec_size);
            GUARD_ARRAY (thedst_neon, loop * vec_size);

            ftbl_4args[2 * func_loop] (thedst_c, thesrc1, thesrc2, loop);
            ftbl_4args[2 * func_loop + 1] (thedst_neon, thesrc1, thesrc2, loop);

            CHECK_ARRAY_GUARD (thedst_c, loop * vec_size);
            CHECK_ARRAY_GUARD (thedst_neon, loop * vec_size);

            for (pos = 0; pos < loop; pos++)
            {
#ifdef DEBUG_TRACE
                fprintf (stdout, "func: %d loop count: %d position: %d \n", func_loop, loop, pos);
                for (i = 0; i < vec_size; i++)
                {
                    fprintf (stdout, "thesrc1->%d: %e [0x%04X] \n", i, thesrc1[pos * vec_size + i], * (ne10_uint32_t*) &thesrc1[pos * vec_size + i]);
                    fprintf (stdout, "thesrc2->%d: %e [0x%04X] \n", i, thesrc2[pos * vec_size + i], * (ne10_uint32_t*) &thesrc2[pos * vec_size + i]);
                }
#endif
                assert_float_vec_equal (&thedst_c[pos * vec_size], &thedst_neon[pos * vec_size], ERROR_MARGIN_SMALL, vec_size);
            }
        }
    }
    free (guarded_src1);
    free (guarded_src2);
    free (guarded_dst_c);
    free (guarded_dst_neon);
#endif

#ifdef PERFORMANCE_TEST
    fprintf (stdout, "%25s%20s%20s%20s%20s\n", "N-component Vector", "C Time in ms", "NEON Time in ms", "Time Savings", "Performance Ratio");
    perftest_length = PERF_TEST_ITERATION * MAX_VEC_COMPONENTS * MAX_VEC_COMPONENTS;
    /* init src memory */
    NE10_SRC_ALLOC (perftest_thesrc1, perftest_guarded_src1, perftest_length); // 16 extra bytes at the begining and 16 extra bytes at the end
    NE10_SRC_ALLOC (perftest_thesrc2, perftest_guarded_src2, perftest_length); // 16 extra bytes at the begining and 16 extra bytes at the end

    /* init dst memory */
    NE10_DST_ALLOC (perftest_thedst_c, perftest_guarded_dst_c, perftest_length);
    NE10_DST_ALLOC (perftest_thedst_neon, perftest_guarded_dst_neon, perftest_length);

    for (func_loop = 1; func_loop < MAX_VEC_COMPONENTS; func_loop++)
    {
        GET_TIME (time_c,
                  for (loop = 0; loop < PERF_TEST_ITERATION; loop++) ftbl_4args[2 * func_loop] (perftest_thedst_c, perftest_thesrc1, perftest_thesrc2, loop);
                 );
        GET_TIME (time_neon,
                  for (loop = 0; loop < PERF_TEST_ITERATION; loop++) ftbl_4args[2 * func_loop + 1] (perftest_thedst_neon, perftest_thesrc1, perftest_thesrc2, loop);
                 );
        time_speedup = (ne10_float32_t) time_c / time_neon;
        time_savings = ( ( (ne10_float32_t) (time_c - time_neon)) / time_c) * 100;
        ne10_log (__FUNCTION__, "%25d%20lld%20lld%19.2f%%%18.2f:1\n", func_loop + 1, time_c, time_neon, time_savings, time_speedup);
    }

    free (perftest_guarded_src1);
    free (perftest_guarded_src2);
    free (perftest_guarded_dst_c);
    free (perftest_guarded_dst_neon);
#endif

    fprintf (stdout, "----------%30s end\n", __FUNCTION__);
#undef MAX_VEC_COMPONENTS
}

void test_transmat_case0()
{
#define MAX_VEC_COMPONENTS 4
    ne10_int32_t loop;
    ne10_int32_t i;
    ne10_int32_t func_loop;
    ne10_int32_t vec_size;
    ne10_int32_t pos;

    fprintf (stdout, "----------%30s start\n", __FUNCTION__);

    /* init function table */
    memset (ftbl_3args, 0, sizeof (ftbl_3args));
    ftbl_3args[ 2] = (ne10_func_3args_t) ne10_transmat_2x2f_c;
    ftbl_3args[ 3] = (ne10_func_3args_t) ne10_transmat_2x2f_neon;
    ftbl_3args[ 4] = (ne10_func_3args_t) ne10_transmat_3x3f_c;
    ftbl_3args[ 5] = (ne10_func_3args_t) ne10_transmat_3x3f_neon;
    ftbl_3args[ 6] = (ne10_func_3args_t) ne10_transmat_4x4f_c;
    ftbl_3args[ 7] = (ne10_func_3args_t) ne10_transmat_4x4f_neon;

#if defined (SMOKE_TEST)||(REGRESSION_TEST)
    const ne10_uint32_t fixed_length = TEST_ITERATION * MAX_VEC_COMPONENTS * MAX_VEC_COMPONENTS;

    /* init src memory */
    NE10_SRC_ALLOC (thesrc1, guarded_src1, fixed_length); // 16 extra bytes at the begining and 16 extra bytes at the end

    /* init dst memory */
    NE10_DST_ALLOC (thedst_c, guarded_dst_c, fixed_length);
    NE10_DST_ALLOC (thedst_neon, guarded_dst_neon, fixed_length);

    for (func_loop = 1; func_loop < MAX_VEC_COMPONENTS; func_loop++)
    {
        for (loop = 0; loop < TEST_ITERATION; loop++)
        {
            vec_size = (func_loop + 1) * (func_loop + 1);

            GUARD_ARRAY (thedst_c, loop * vec_size);
            GUARD_ARRAY (thedst_neon, loop * vec_size);

            ftbl_3args[2 * func_loop] (thedst_c, thesrc1, loop);
            ftbl_3args[2 * func_loop + 1] (thedst_neon, thesrc1, loop);

            CHECK_ARRAY_GUARD (thedst_c, loop * vec_size);
            CHECK_ARRAY_GUARD (thedst_neon, loop * vec_size);

            for (pos = 0; pos < loop; pos++)
            {
#ifdef DEBUG_TRACE
                fprintf (stdout, "func: %d loop count: %d position: %d \n", func_loop, loop, pos);
                for (i = 0; i < vec_size; i++)
                {
                    fprintf (stdout, "thesrc1->%d: %e [0x%04X] \n", i, thesrc1[pos * vec_size + i], * (ne10_uint32_t*) &thesrc1[pos * vec_size + i]);
                }
#endif
                assert_float_vec_equal (&thedst_c[pos * vec_size], &thedst_neon[pos * vec_size], ERROR_MARGIN_SMALL, vec_size);
            }
        }
    }
    free (guarded_src1);
    free (guarded_dst_c);
    free (guarded_dst_neon);
#endif

#ifdef PERFORMANCE_TEST
    fprintf (stdout, "%25s%20s%20s%20s%20s\n", "N-component Vector", "C Time in ms", "NEON Time in ms", "Time Savings", "Performance Ratio");
    perftest_length = PERF_TEST_ITERATION * MAX_VEC_COMPONENTS * MAX_VEC_COMPONENTS;
    /* init src memory */
    NE10_SRC_ALLOC (perftest_thesrc1, perftest_guarded_src1, perftest_length); // 16 extra bytes at the begining and 16 extra bytes at the end

    /* init dst memory */
    NE10_DST_ALLOC (perftest_thedst_c, perftest_guarded_dst_c, perftest_length);
    NE10_DST_ALLOC (perftest_thedst_neon, perftest_guarded_dst_neon, perftest_length);

    for (func_loop = 1; func_loop < MAX_VEC_COMPONENTS; func_loop++)
    {
        GET_TIME (time_c,
                  for (loop = 0; loop < PERF_TEST_ITERATION; loop++) ftbl_3args[2 * func_loop] (perftest_thedst_c, perftest_thesrc1, loop);
                 );
        GET_TIME (time_neon,
                  for (loop = 0; loop < PERF_TEST_ITERATION; loop++) ftbl_3args[2 * func_loop + 1] (perftest_thedst_neon, perftest_thesrc1, loop);
                 );
        time_speedup = (ne10_float32_t) time_c / time_neon;
        time_savings = ( ( (ne10_float32_t) (time_c - time_neon)) / time_c) * 100;
        ne10_log (__FUNCTION__, "%25d%20lld%20lld%19.2f%%%18.2f:1\n", func_loop + 1, time_c, time_neon, time_savings, time_speedup);
    }

    free (perftest_guarded_src1);
    free (perftest_guarded_dst_c);
    free (perftest_guarded_dst_neon);
#endif

    fprintf (stdout, "----------%30s end\n", __FUNCTION__);
#undef MAX_VEC_COMPONENTS
}

void test_mulcmatvec_case0()
{
#define MAX_VEC_COMPONENTS 4
    ne10_int32_t loop;
    ne10_int32_t i;
    ne10_int32_t func_loop;
    ne10_int32_t vec_size;
    ne10_int32_t pos;

    fprintf (stdout, "----------%30s start\n", __FUNCTION__);

    /* init function table */
    memset (ftbl_4args, 0, sizeof (ftbl_4args));
    ftbl_4args[ 2] = (ne10_func_4args_t) ne10_mulcmatvec_cm2x2f_v2f_c;
    ftbl_4args[ 3] = (ne10_func_4args_t) ne10_mulcmatvec_cm2x2f_v2f_neon;
    ftbl_4args[ 4] = (ne10_func_4args_t) ne10_mulcmatvec_cm3x3f_v3f_c;
    ftbl_4args[ 5] = (ne10_func_4args_t) ne10_mulcmatvec_cm3x3f_v3f_neon;
    ftbl_4args[ 6] = (ne10_func_4args_t) ne10_mulcmatvec_cm4x4f_v4f_c;
    ftbl_4args[ 7] = (ne10_func_4args_t) ne10_mulcmatvec_cm4x4f_v4f_neon;

#if defined (SMOKE_TEST)||(REGRESSION_TEST)
    const ne10_uint32_t fixed_length = TEST_ITERATION * MAX_VEC_COMPONENTS;

    /* init src memory */
    NE10_SRC_ALLOC_LIMIT (thesrc1, guarded_src1, fixed_length); // 16 extra bytes at the begining and 16 extra bytes at the end
    NE10_SRC_ALLOC_LIMIT (thecst, guarded_cst, MAX_VEC_COMPONENTS * MAX_VEC_COMPONENTS); // 16 extra bytes at the begining and 16 extra bytes at the end

    /* init dst memory */
    NE10_DST_ALLOC (thedst_c, guarded_dst_c, fixed_length);
    NE10_DST_ALLOC (thedst_neon, guarded_dst_neon, fixed_length);

    for (func_loop = 1; func_loop < MAX_VEC_COMPONENTS; func_loop++)
    {
        for (loop = 0; loop < TEST_ITERATION; loop++)
        {
            vec_size = func_loop + 1;

            GUARD_ARRAY (thedst_c, loop * vec_size);
            GUARD_ARRAY (thedst_neon, loop * vec_size);

            ftbl_4args[2 * func_loop] (thedst_c, thecst, thesrc1, loop);
            ftbl_4args[2 * func_loop + 1] (thedst_neon, thecst, thesrc1, loop);

            CHECK_ARRAY_GUARD (thedst_c, loop * vec_size);
            CHECK_ARRAY_GUARD (thedst_neon, loop * vec_size);

            for (pos = 0; pos < loop; pos++)
            {
#ifdef DEBUG_TRACE
                fprintf (stdout, "func: %d loop count: %d position: %d \n", func_loop, loop, pos);
                for (i = 0; i < vec_size * vec_size; i++)
                {
                    fprintf (stdout, "thecst->%d: %e [0x%04X] \n", i, thecst[i], * (ne10_uint32_t*) &thecst[i]);
                }
                for (i = 0; i < vec_size; i++)
                {
                    fprintf (stdout, "thesrc1->%d: %e [0x%04X] \n", i, thesrc1[pos * vec_size + i], * (ne10_uint32_t*) &thesrc1[pos * vec_size + i]);
                }
#endif
                assert_float_vec_equal (&thedst_c[pos * vec_size], &thedst_neon[pos * vec_size], ERROR_MARGIN_SMALL, vec_size);
            }
        }
    }
    free (guarded_src1);
    free (guarded_cst);
    free (guarded_dst_c);
    free (guarded_dst_neon);
#endif

#ifdef PERFORMANCE_TEST
    fprintf (stdout, "%25s%20s%20s%20s%20s\n", "N-component Vector", "C Time in ms", "NEON Time in ms", "Time Savings", "Performance Ratio");
    perftest_length = PERF_TEST_ITERATION * MAX_VEC_COMPONENTS;
    /* init src memory */
    NE10_SRC_ALLOC (perftest_thesrc1, perftest_guarded_src1, perftest_length); // 16 extra bytes at the begining and 16 extra bytes at the end
    NE10_SRC_ALLOC (perftest_thecst, perftest_guarded_cst, MAX_VEC_COMPONENTS * MAX_VEC_COMPONENTS); // 16 extra bytes at the begining and 16 extra bytes at the end

    /* init dst memory */
    NE10_DST_ALLOC (perftest_thedst_c, perftest_guarded_dst_c, perftest_length);
    NE10_DST_ALLOC (perftest_thedst_neon, perftest_guarded_dst_neon, perftest_length);

    for (func_loop = 1; func_loop < MAX_VEC_COMPONENTS; func_loop++)
    {
        GET_TIME (time_c,
                  for (loop = 0; loop < PERF_TEST_ITERATION; loop++) ftbl_4args[2 * func_loop] (perftest_thedst_c, perftest_thecst, perftest_thesrc1, loop);
                 );
        GET_TIME (time_neon,
                  for (loop = 0; loop < PERF_TEST_ITERATION; loop++) ftbl_4args[2 * func_loop + 1] (perftest_thedst_neon, perftest_thecst, perftest_thesrc1, loop);
                 );
        time_speedup = (ne10_float32_t) time_c / time_neon;
        time_savings = ( ( (ne10_float32_t) (time_c - time_neon)) / time_c) * 100;
        ne10_log (__FUNCTION__, "%25d%20lld%20lld%19.2f%%%18.2f:1\n", func_loop + 1, time_c, time_neon, time_savings, time_speedup);
    }

    free (perftest_guarded_src1);
    free (perftest_guarded_cst);
    free (perftest_guarded_dst_c);
    free (perftest_guarded_dst_neon);
#endif

    fprintf (stdout, "----------%30s end\n", __FUNCTION__);
#undef MAX_VEC_COMPONENTS
}

void test_abs()
{
    test_abs_case0();
}

void test_addc()
{
    test_addc_case0();
}

void test_add()
{
    test_add_case0();
}

void test_cross()
{
    test_cross_case0();
}

void test_divc()
{
    test_divc_case0();
}

void test_div()
{
    test_div_case0();
}

void test_dot()
{
    test_dot_case0();
}

void test_len()
{
    test_len_case0();
}

void test_mlac()
{
    test_mlac_case0();
}

void test_mla()
{
    test_mla_case0();
}

void test_mulc()
{
    test_mulc_case0();
}

void test_mul()
{
    test_mul_case0();
}
void test_normalize()
{
    test_normalize_case0();
}

void test_rsbc()
{
    test_rsbc_case0();
}

void test_setc()
{
    test_setc_case0();
}

void test_subc()
{
    test_subc_case0();
}

void test_sub()
{
    test_sub_case0();
}

void test_addmat()
{
    test_addmat_case0();
}

void test_detmat()
{
    test_detmat_case0();
}

void test_identitymat()
{
    test_identitymat_case0();
}

void test_invmat()
{
    test_invmat_case0();
}

void test_mulmat()
{
    test_mulmat_case0();
}

void test_mulcmatvec()
{
    test_mulcmatvec_case0();
}

void test_submat()
{
    test_submat_case0();
}

void test_transmat()
{
    test_transmat_case0();
}

static void my_test_setup (void)
{
    //printf("------%-30s start\r\n", __FUNCTION__);
    ne10_log_buffer_ptr = ne10_log_buffer;
}

void my_test_teardown (void)
{
    //printf("--------end\r\n");
}

void test_fixture_math (void)
{
    test_fixture_start();               // starts a fixture

    fixture_setup (my_test_setup);
    fixture_teardown (my_test_teardown);

    run_test (test_abs);       // run tests
    run_test (test_addc);
    run_test (test_add);
    run_test (test_cross);
    run_test (test_divc);
    run_test (test_div);
    run_test (test_dot);
    run_test (test_len);
    run_test (test_mlac);
    run_test (test_mla);
    run_test (test_mulc);
    run_test (test_mul);
    run_test (test_normalize);
    run_test (test_rsbc);
    run_test (test_setc);
    run_test (test_subc);
    run_test (test_sub);
    run_test (test_addmat);
    run_test (test_detmat);
    run_test (test_identitymat);
    run_test (test_invmat);
    run_test (test_mulmat);
    run_test (test_mulcmatvec);
    run_test (test_submat);
    run_test (test_transmat);

    test_fixture_end();                 // ends a fixture
}
