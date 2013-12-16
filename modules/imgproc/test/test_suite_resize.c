/*
 *  Copyright 2013 ARM Limited
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
 * NE10 Library : test_suite_resize.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "NE10_imgproc.h"
#include "seatest.h"
#include "unit_test_common.h"

/* ----------------------------------------------------------------------
** Global defines
** ------------------------------------------------------------------- */
#define MEM_SIZE 128

#define TEST_COUNT 5000


/* ----------------------------------------------------------------------
** Defines each of the tests performed
** ------------------------------------------------------------------- */


//input and output
static ne10_uint8_t * in_c = NULL;
static ne10_uint8_t * in_neon = NULL;

static ne10_uint8_t * out_c = NULL;
static ne10_uint8_t * out_neon = NULL;

static ne10_float32_t snr = 0.0f;

void test_resize_conformance_case()
{
    ne10_int32_t srcw;
    ne10_int32_t srch;
    ne10_int32_t dstw;
    ne10_int32_t dsth;
    ne10_int32_t i;
    ne10_int32_t w, h;
    ne10_int32_t channels = 4;
    ne10_int32_t pic_size = MEM_SIZE * MEM_SIZE * channels * sizeof (ne10_uint8_t);
    ne10_float32_t PSNR = 0.0f;

    /* init input memory */
    in_c = NE10_MALLOC (pic_size);
    in_neon = NE10_MALLOC (pic_size);

    /* init dst memory */
    out_c = NE10_MALLOC (pic_size);
    out_neon = NE10_MALLOC (pic_size);

    for (i = 0; i < pic_size; i++)
    {
        in_c[i] = in_neon[i] = (rand() & 0xff);
    }
#if defined(SMOKE_TEST)
    for (h = 96; h < MEM_SIZE; h++)
    {
        for (w = 96; w < MEM_SIZE; w++)
        {
            srcw = h;
            srch = h;
            dstw = w;
            dsth = w;

            printf ("srcw X srch = %d X %d \n", srcw, srch);
            printf ("dstw X dsth = %d X %d \n", dstw, dsth);

            ne10_img_resize_bilinear_rgba_c (out_c, dstw, dsth, in_c, srcw, srch, srcw);
            ne10_img_resize_bilinear_rgba_neon (out_neon, dstw, dsth, in_neon, srcw, srch, srcw);

            PSNR = CAL_PSNR_UINT8 (out_c, out_neon, dstw * dsth * channels);
            assert_false ( (PSNR < PSNR_THRESHOLD));
        }
    }
#endif

#if defined(REGRESSION_TEST)
    for (h = 1; h < MEM_SIZE; h++)
    {
        for (w = 1; w < MEM_SIZE; w++)
        {
            srcw = h;
            srch = h;
            dstw = w;
            dsth = w;

            printf ("srcw X srch = %d X %d \n", srcw, srch);
            printf ("dstw X dsth = %d X %d \n", dstw, dsth);

            ne10_img_resize_bilinear_rgba_c (out_c, dstw, dsth, in_c, srcw, srch, srcw);
            ne10_img_resize_bilinear_rgba_neon (out_neon, dstw, dsth, in_neon, srcw, srch, srcw);

            PSNR = CAL_PSNR_UINT8 (out_c, out_neon, dstw * dsth * channels);
            assert_false ( (PSNR < PSNR_THRESHOLD));
        }
    }
#endif

    NE10_FREE (in_c);
    NE10_FREE (in_neon);
    NE10_FREE (out_c);
    NE10_FREE (out_neon);
}

void test_resize_performance_case()
{
    ne10_int32_t srcw;
    ne10_int32_t srch;
    ne10_int32_t dstw;
    ne10_int32_t dsth;
    ne10_int32_t i;
    ne10_int32_t w, h;
    ne10_int32_t channels = 4;
    ne10_int32_t pic_size = MEM_SIZE * MEM_SIZE * channels * sizeof (ne10_uint8_t);
    ne10_int64_t time_c = 0;
    ne10_int64_t time_neon = 0;

    /* init input memory */
    in_c = NE10_MALLOC (pic_size);
    in_neon = NE10_MALLOC (pic_size);

    /* init dst memory */
    out_c = NE10_MALLOC (pic_size);
    out_neon = NE10_MALLOC (pic_size);

    for (i = 0; i < pic_size; i++)
    {
        in_c[i] = in_neon[i] = (rand() & 0xff);
    }

    for (h = 16; h < MEM_SIZE; h += 4)
    {
        for (w = 16; w < MEM_SIZE; w += 4)
        {
            srcw = h;
            srch = h;
            dstw = w;
            dsth = w;

            printf ("srcw X srch = %d X %d \n", srcw, srch);
            printf ("dstw X dsth = %d X %d \n", dstw, dsth);

            GET_TIME
            (
                time_c,
            {
                for (i = 0; i < TEST_COUNT; i++)
                    ne10_img_resize_bilinear_rgba_c (out_c, dstw, dsth, in_c, srcw, srch, srcw);
            }
            );

            GET_TIME
            (
                time_neon,
            {
                for (i = 0; i < TEST_COUNT; i++)
                    ne10_img_resize_bilinear_rgba_neon (out_neon, dstw, dsth, in_neon, srcw, srch, srcw);
            }
            );
            //printf ("time c %lld \n", time_c);
            //printf ("time neon %lld \n", time_neon);
            ne10_log (__FUNCTION__, "IMAGERESIZE%20d%20lld%20lld%19.2f%%%18.2f:1\n", (h * MEM_SIZE + w), time_c, time_neon, 0, 0);

        }
    }
    NE10_FREE (in_c);
    NE10_FREE (in_neon);
    NE10_FREE (out_c);
    NE10_FREE (out_neon);
}

void test_resize()
{
#if defined (SMOKE_TEST)||(REGRESSION_TEST)
    test_resize_conformance_case();
#endif

#if defined PERFORMANCE_TEST
    test_resize_performance_case();
#endif
}

static void my_test_setup (void)
{
    ne10_log_buffer_ptr = ne10_log_buffer;
}

void test_fixture_resize (void)
{
    test_fixture_start();               // starts a fixture

    fixture_setup (my_test_setup);

    run_test (test_resize);       // run tests

    test_fixture_end();                 // ends a fixture
}



