/*
 *  Copyright 2012-13 ARM Limited
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

/*
 * NE10 Library : test/include/unit_test_common.h
 */

#ifndef __UNIT_TEST_COMMON__
#define __UNIT_TEST_COMMON__

// Make sure the following values are defined before including this header file:
// 1- length of the data arrays
//     #define ARRLEN
// 2- number of the operations in a given unit
//     #define OP_COUNT
// 3- number of the different implementations of each of the functions (C, ASM, NEON, ...)
//     #define IMPL_COUNT

#include <stdio.h>
#include <assert.h>
#include <math.h>

#include <stdlib.h>
#include <string.h>

#include "NE10.h"
#include "NE10_random.h"

//detect that it is regression test or smoke test
#if defined REGRESSION_TEST
#  define TEST_ITERATION 2048
#else
#  ifdef SMOKE_TEST
#    define TEST_ITERATION 11
#  endif
#endif

//detect that it is performance test
#if defined PERFORMANCE_TEST
#  define PERF_TEST_ITERATION 1024
#endif

// length of the test data arrays
// A number that is not divisible by 2 3 and 4 so that all the
//  execution paths are tested; The larger the number the more
//  number of random values are stored in the array and passed
//  into the array as the input stream.
// 2^11 + 3 = 2051, it is not divisible by 2, 3, or 4
//#define TEST_ARRLEN          2051
//#define TEST_ARRLEN_MATRICES 1051

#define ARRAY_GUARD_LEN      4
#define MAX_FUNC_COUNT       8 //C and NEON version with 4 different data type, 

// The sign bit mask
#define SIGNBIT_MASK  0x7FFFFFFF

// What's the acceptable error between the integer representations of two ne10_float32_t values
#define ERROR_MARGIN_SMALL 0x0A
#define ERROR_MARGIN_LARGE 0xFF
#define SNR_THRESHOLD 100.0f
#define SNR_THRESHOLD_INT16 35.0f
#define PSNR_THRESHOLD 30.0f

// What's the acceptable number of warnings in a test
#define ACCEPTABLE_WARNS 12
#define ACCEPTABLE_WARNS_MATRICES 48

#define NE10_SRC_ALLOC(src, guarded_src, length) { \
        (guarded_src) = (ne10_float32_t*) calloc (2*ARRAY_GUARD_LEN + (length), sizeof(ne10_float32_t)); \
        if ((guarded_src) == NULL) \
            printf ("error: calloc src failed\n"); \
        (src) = (guarded_src) + ARRAY_GUARD_LEN; \
        FILL_FLOAT_ARRAY((src), (length)); \
    }

#define NE10_SRC_ALLOC_LIMIT(src, guarded_src, length) { \
        (guarded_src) = (ne10_float32_t*) calloc (2*ARRAY_GUARD_LEN + (length), sizeof(ne10_float32_t)); \
        if ((guarded_src) == NULL) \
            printf ("error: calloc src failed\n"); \
        (src) = (guarded_src) + ARRAY_GUARD_LEN; \
        FILL_FLOAT_ARRAY_LIMIT((src), (length)); \
    }

#define NE10_DST_ALLOC(dst, guarded_dst, length) { \
        (guarded_dst) = (ne10_float32_t*) calloc (2*ARRAY_GUARD_LEN + (length), sizeof(ne10_float32_t)); \
        if ((guarded_dst) == NULL) \
            printf ("error: calloc dst failed\n"); \
        (dst) = (guarded_dst) + ARRAY_GUARD_LEN; \
    }

#define GET_TIME(time, code) { \
        (time) = GetTickCount(); \
        code \
        (time) = GetTickCount() - (time);\
    }
typedef ne10_result_t (*ne10_func_5args_t) (void * dst, void * acc, void * src1, void * src2, ne10_uint32_t count);
typedef ne10_result_t (*ne10_func_4args_t) (void * dst, void * src1, void * src2, ne10_uint32_t count);
typedef ne10_result_t (*ne10_func_3args_t) (void * dst, void * src, ne10_uint32_t count);
typedef ne10_result_t (*ne10_func_2args_t) (void * dst, ne10_uint32_t count);
typedef ne10_result_t (*ne10_func_5args_cst_t) (void * dst, void * acc, void * src, ne10_float32_t cst, ne10_uint32_t count);
typedef ne10_result_t (*ne10_func_4args_cst_t) (void * dst, void * src, const ne10_float32_t cst, ne10_uint32_t count);
typedef ne10_result_t (*ne10_func_3args_cst_t) (void * dst, const ne10_float32_t cst, ne10_uint32_t count);


extern void FILL_FLOAT_ARRAY( ne10_float32_t *arr, ne10_uint32_t count );
extern void FILL_FLOAT_ARRAY_LIMIT( ne10_float32_t *arr, ne10_uint32_t count );
extern void FILL_FLOAT_ARRAY_LIMIT_GT1( ne10_float32_t *arr, ne10_uint32_t count );

// this function checks whether the difference between two ne10_float32_t values is within the acceptable error range
extern int EQUALS_FLOAT( ne10_float32_t fa, ne10_float32_t fb , ne10_uint32_t err );
extern int GUARD_ARRAY( ne10_float32_t* array, ne10_uint32_t array_length );
extern int CHECK_ARRAY_GUARD( ne10_float32_t* array, ne10_uint32_t array_length );
extern ne10_int32_t GUARD_ARRAY_UINT8 (ne10_uint8_t* array, ne10_uint32_t array_length);
extern ne10_int32_t CHECK_ARRAY_GUARD_UINT8 (ne10_uint8_t* array, ne10_uint32_t array_length);
extern ne10_float32_t CAL_SNR_FLOAT32(ne10_float32_t *pRef, ne10_float32_t *pTest, ne10_uint32_t buffSize);
extern ne10_float32_t CAL_PSNR_UINT8 (ne10_uint8_t *pRef, ne10_uint8_t *pTest, ne10_uint32_t buffSize);

extern char ne10_log_buffer[];
extern char *ne10_log_buffer_ptr;
extern void ne10_log(const char *func_name,
                     const char *format_str,
                     ne10_int32_t n,
                     ne10_int32_t time_c,
                     ne10_int32_t time_neon,
                     ne10_float32_t time_savings,
                     ne10_float32_t time_speedup);

#endif // __UNIT_TEST_COMMON

