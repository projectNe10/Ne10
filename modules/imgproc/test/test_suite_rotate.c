/*
 *  Copyright 2013-16 ARM Limited and Contributors.
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
 * NE10 Library : test_suite_rotate.c
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

#define SRC_HEIGHT        512
#define SRC_WIDTH         512
#define DST_HEIGHT        734 //sqrt(512*512 + 512*512) + 10
#define DST_WIDTH         734 //sqrt(512*512 + 512*512) + 10
#define TEST_COUNT        5000


/* ----------------------------------------------------------------------
** Defines each of the tests performed
** ------------------------------------------------------------------- */


//input and output
static ne10_uint8_t * in_c = NULL;
static ne10_uint8_t * in_neon = NULL;

static ne10_uint8_t * out_c = NULL;
static ne10_uint8_t * out_neon = NULL;

#ifdef ENABLE_NE10_IMG_ROTATE_RGBA_NEON
void test_rotate_conformance_case()
{
    ne10_int32_t i;
    ne10_int32_t channels = 4;
    ne10_int32_t in_size = SRC_HEIGHT * SRC_WIDTH * channels;
    ne10_int32_t out_size = DST_HEIGHT * DST_WIDTH * channels;
    ne10_float32_t PSNR = 0.0f;
    ne10_int32_t srcw = SRC_WIDTH;
    ne10_int32_t srch = SRC_HEIGHT;
    ne10_uint32_t dstw_c, dsth_c;
    ne10_uint32_t dstw_neon, dsth_neon;
    ne10_int32_t angle;

    /* init input memory */
    in_c = NE10_MALLOC (in_size * sizeof (ne10_uint8_t));
    in_neon = NE10_MALLOC (in_size * sizeof (ne10_uint8_t));

    /* init dst memory */
    out_c = NE10_MALLOC (out_size * sizeof (ne10_uint8_t));
    out_neon = NE10_MALLOC (out_size * sizeof (ne10_uint8_t));

    for (i = 0; i < in_size; i++)
    {
        in_c[i] = in_neon[i] = (rand() & 0xff);
    }

    for (angle = -360; angle <= 360; angle += 30)
    {
        printf ("rotate angle %d \n", angle);

        memset (out_c, 0, out_size);
        ne10_img_rotate_rgba_c (out_c, &dstw_c, &dsth_c, in_c, srcw, srch, angle);

        memset (out_neon, 0, out_size);
        ne10_img_rotate_rgba_neon (out_neon, &dstw_neon, &dsth_neon, in_neon, srcw, srch, angle);

        PSNR = CAL_PSNR_UINT8 (out_c, out_neon, dstw_c * dsth_c * 4);
        assert_false ( (PSNR < PSNR_THRESHOLD));
        //printf ("PSNR %f \n", PSNR);
    }
    NE10_FREE (in_c);
    NE10_FREE (in_neon);
    NE10_FREE (out_c);
    NE10_FREE (out_neon);
}
#endif // ENABLE_NE10_IMG_ROTATE_RGBA_NEON

void test_rotate_performance_case()
{
    ne10_int32_t i;
    ne10_int32_t channels = 4;
    ne10_int32_t in_size = SRC_HEIGHT * SRC_WIDTH * channels;
    ne10_int32_t out_size = DST_HEIGHT * DST_WIDTH * channels;
    ne10_int32_t srcw = SRC_WIDTH;
    ne10_int32_t srch = SRC_HEIGHT;
    ne10_uint32_t dstw_c, dsth_c;
#ifdef ENABLE_NE10_IMG_ROTATE_RGBA_NEON
    ne10_uint32_t dstw_neon, dsth_neon;
#endif // ENABLE_NE10_IMG_ROTATE_RGBA_NEON
    ne10_int32_t angle;
    ne10_int64_t time_c = 0;
    ne10_int64_t time_neon = 0;

    /* init input memory */
    in_c = NE10_MALLOC (in_size * sizeof (ne10_uint8_t));
    in_neon = NE10_MALLOC (in_size * sizeof (ne10_uint8_t));

    /* init dst memory */
    out_c = NE10_MALLOC (out_size * sizeof (ne10_uint8_t));
    out_neon = NE10_MALLOC (out_size * sizeof (ne10_uint8_t));

    for (i = 0; i < in_size; i++)
    {
        in_c[i] = in_neon[i] = (rand() & 0xff);
    }

    //for (angle = -360; angle <= 360; angle += 5)
    for (angle = 45; angle <= 45; angle += 5)
    {
        printf ("rotate angle %d \n", angle);

        memset (out_c, 0, out_size);
        GET_TIME
        (
            time_c,
        {
            for (i = 0; i < TEST_COUNT; i++)
                ne10_img_rotate_rgba_c (out_c, &dstw_c, &dsth_c, in_c, srcw, srch, angle);
        }
        );

#ifdef ENABLE_NE10_IMG_ROTATE_RGBA_NEON
        memset (out_neon, 0, out_size);
        GET_TIME
        (
            time_neon,
        {
            for (i = 0; i < TEST_COUNT; i++)
                ne10_img_rotate_rgba_neon (out_neon, &dstw_neon, &dsth_neon, in_neon, srcw, srch, angle);
        }
        );
#endif // ENABLE_NE10_IMG_ROTATE_RGBA_NEON

        //printf ("time c %lld \n", time_c);
        //printf ("time neon %lld \n", time_neon);
        ne10_log (__FUNCTION__, "IMAGEROTATE%20d%20lld%20lld%19.2f%%%18.2f:1\n", angle, time_c, time_neon, 0, 0);
    }

    NE10_FREE (in_c);
    NE10_FREE (in_neon);
    NE10_FREE (out_c);
    NE10_FREE (out_neon);
}

void test_rotate()
{
#ifdef ENABLE_NE10_IMG_ROTATE_RGBA_NEON
#if defined (SMOKE_TEST)||(REGRESSION_TEST)
    test_rotate_conformance_case();
#endif
#endif // ENABLE_NE10_IMG_ROTATE_RGBA_NEON

#if defined PERFORMANCE_TEST
    test_rotate_performance_case();
#endif
}

static void my_test_setup (void)
{
    ne10_log_buffer_ptr = ne10_log_buffer;
}

void test_fixture_rotate (void)
{
    test_fixture_start();               // starts a fixture

    fixture_setup (my_test_setup);

    run_test (test_rotate);       // run tests

    test_fixture_end();                 // ends a fixture
}
