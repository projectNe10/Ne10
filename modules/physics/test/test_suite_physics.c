/*
 *  Copyright 2014 ARM Limited
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
 * NE10 Library : test/test_suite_physics.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "NE10_physics.h"
#include "seatest.h"
#include "unit_test_common.h"

/* ----------------------------------------------------------------------
** Global defines
** ------------------------------------------------------------------- */
#define TEST_LENGTH_SAMPLES 1024
#define TEST_COUNT 5000

static ne10_int64_t time_c = 0;
static ne10_int64_t time_neon = 0;
static ne10_float32_t time_speedup = 0.0f;
static ne10_float32_t time_savings = 0.0f;

static void float_array_assignment (ne10_float32_t *array, ne10_int32_t len)
{
    int i;
    for (i = 0; i < len; i++)
    {
        array[i] = (ne10_float32_t) (drand48() * 32768.0f - 16384.0f);
    }
}


void test_compute_aabb_vec2f_conformance()
{
    ne10_vec2f_t radius = {0.2f, 0.2f};
    ne10_vec2f_t *vertices_c, *vertices_neon;
    ne10_mat2x2f_t aabb_c, aabb_neon;
    ne10_mat2x2f_t xf;
    ne10_int32_t i;
    ne10_int32_t vertex_count;
    ne10_int32_t vec_size = sizeof (ne10_mat2x2f_t) / sizeof (ne10_float32_t);

    fprintf (stdout, "----------%30s start\n", __FUNCTION__);

    /* init input memory */
    vertices_c = (ne10_vec2f_t*) NE10_MALLOC (TEST_LENGTH_SAMPLES * sizeof (ne10_vec2f_t));
    vertices_neon = (ne10_vec2f_t*) NE10_MALLOC (TEST_LENGTH_SAMPLES * sizeof (ne10_vec2f_t));
    float_array_assignment ( (ne10_float32_t *) vertices_c, TEST_LENGTH_SAMPLES * sizeof (ne10_vec2f_t) / sizeof (ne10_float32_t));
    memcpy ( (ne10_float32_t *) vertices_neon, (ne10_float32_t *) vertices_c, TEST_LENGTH_SAMPLES * sizeof (ne10_vec2f_t));

    ne10_float32_t tmp = (ne10_float32_t) (drand48() * 64.0f - 32.0f);
    xf.c1.r1 = (ne10_float32_t) (drand48() * 16.0f - 8.0f);
    xf.c1.r2 = (ne10_float32_t) (drand48() * 16.0f - 8.0f);
    xf.c2.r1 = sin (tmp);
    xf.c2.r2 = cos (tmp);

#if defined (REGRESSION_TEST)
    for (vertex_count = 1; vertex_count < TEST_LENGTH_SAMPLES; vertex_count++)
    {
        //C version
        ne10_physics_compute_aabb_vec2f_c (&aabb_c, vertices_c, &xf, &radius, vertex_count);
        //neon version
        ne10_physics_compute_aabb_vec2f_neon (&aabb_neon, vertices_neon, &xf, &radius, vertex_count);
        printf ("----vertex_count %d\n", vertex_count);
        assert_float_vec_equal ( (ne10_float32_t*) &aabb_c, (ne10_float32_t*) &aabb_neon, ERROR_MARGIN_LARGE, vec_size);
    }
#endif

#if defined (SMOKE_TEST)
    for (vertex_count = 1; vertex_count < TEST_LENGTH_SAMPLES; vertex_count += 3)
    {
        //C version
        ne10_physics_compute_aabb_vec2f_c (&aabb_c, vertices_c, &xf, &radius, vertex_count);
        //neon version
        ne10_physics_compute_aabb_vec2f_neon (&aabb_neon, vertices_neon, &xf, &radius, vertex_count);
        printf ("----vertex_count %d\n", vertex_count);
        assert_float_vec_equal ( (ne10_float32_t*) &aabb_c, (ne10_float32_t*) &aabb_neon, ERROR_MARGIN_LARGE, vec_size);
    }
#endif
    free (vertices_c);
    free (vertices_neon);
}

void test_compute_aabb_vec2f_performance()
{
    ne10_vec2f_t radius = {0.2f, 0.2f};
    ne10_vec2f_t *vertices_c, *vertices_neon;
    ne10_mat2x2f_t aabb_c, aabb_neon;
    ne10_mat2x2f_t xf;
    ne10_int32_t i;
    ne10_int32_t vertex_count;
    ne10_int32_t vec_size = sizeof (ne10_mat2x2f_t) / sizeof (ne10_float32_t);

    fprintf (stdout, "----------%30s start\n", __FUNCTION__);
    fprintf (stdout, "%25s%20s%20s%20s%20s\n", "vertex count", "C Time in ms", "NEON Time in ms", "Time Savings", "Performance Ratio");

    /* init input memory */
    vertices_c = (ne10_vec2f_t*) NE10_MALLOC (TEST_LENGTH_SAMPLES * sizeof (ne10_vec2f_t));
    vertices_neon = (ne10_vec2f_t*) NE10_MALLOC (TEST_LENGTH_SAMPLES * sizeof (ne10_vec2f_t));
    float_array_assignment ( (ne10_float32_t *) vertices_c, TEST_LENGTH_SAMPLES * sizeof (ne10_vec2f_t) / sizeof (ne10_float32_t));
    memcpy ( (ne10_float32_t *) vertices_neon, (ne10_float32_t *) vertices_c, TEST_LENGTH_SAMPLES * sizeof (ne10_vec2f_t));

    ne10_float32_t tmp = (ne10_float32_t) (drand48() * 64.0f - 32.0f);
    xf.c1.r1 = (ne10_float32_t) (drand48() * 16.0f - 8.0f);
    xf.c1.r2 = (ne10_float32_t) (drand48() * 16.0f - 8.0f);
    xf.c2.r1 = sin (tmp);
    xf.c2.r2 = cos (tmp);

    for (vertex_count = 4; vertex_count < TEST_LENGTH_SAMPLES; vertex_count += 4)
    {
        //C version
        GET_TIME
        (time_c,
        {
            for (i = 0; i < TEST_COUNT; i++)
                ne10_physics_compute_aabb_vec2f_c (&aabb_c, vertices_c, &xf, &radius, vertex_count);
        }
        );
        //neon version
        GET_TIME
        (time_neon,
        {
            for (i = 0; i < TEST_COUNT; i++)
                ne10_physics_compute_aabb_vec2f_neon (&aabb_neon, vertices_neon, &xf, &radius, vertex_count);
        }
        );
        time_speedup = (ne10_float32_t) time_c / time_neon;
        time_savings = ( ( (ne10_float32_t) (time_c - time_neon)) / time_c) * 100;
        printf ("vertax count: %10d time C: %10lld time NEON: %10lld\n", vertex_count, time_c, time_neon);
        //ne10_log (__FUNCTION__, "Compute aabb%21d%20lld%20lld%19.2f%%%18.2f:1\n", vertex_count, time_c, time_neon, time_savings, time_speedup);
    }
    free (vertices_c);
    free (vertices_neon);
}

void test_relative_v_vec2f_conformance()
{
    ne10_vec2f_t *guarded_dv_c, *guarded_dv_neon;
    ne10_vec2f_t *dv_c, *dv_neon;
    ne10_vec3f_t *v_wa, *v_wb;
    ne10_vec2f_t *ra, *rb;
    ne10_int32_t i;
    ne10_int32_t count;
    ne10_int32_t vec_size = sizeof (ne10_vec2f_t) / sizeof (ne10_float32_t);

    fprintf (stdout, "----------%30s start\n", __FUNCTION__);

    /* init input memory */
    v_wa = (ne10_vec3f_t*) NE10_MALLOC (TEST_LENGTH_SAMPLES * sizeof (ne10_vec3f_t));
    v_wb = (ne10_vec3f_t*) NE10_MALLOC (TEST_LENGTH_SAMPLES * sizeof (ne10_vec3f_t));
    ra = (ne10_vec2f_t*) NE10_MALLOC (TEST_LENGTH_SAMPLES * sizeof (ne10_vec2f_t));
    rb = (ne10_vec2f_t*) NE10_MALLOC (TEST_LENGTH_SAMPLES * sizeof (ne10_vec2f_t));
    float_array_assignment ( (ne10_float32_t *) v_wa, TEST_LENGTH_SAMPLES * sizeof (ne10_vec3f_t) / sizeof (ne10_float32_t));
    float_array_assignment ( (ne10_float32_t *) v_wb, TEST_LENGTH_SAMPLES * sizeof (ne10_vec3f_t) / sizeof (ne10_float32_t));
    float_array_assignment ( (ne10_float32_t *) ra, TEST_LENGTH_SAMPLES * sizeof (ne10_vec2f_t) / sizeof (ne10_float32_t));
    float_array_assignment ( (ne10_float32_t *) rb, TEST_LENGTH_SAMPLES * sizeof (ne10_vec2f_t) / sizeof (ne10_float32_t));

    /* init dst memory */
    guarded_dv_c = (ne10_vec2f_t*) NE10_MALLOC (TEST_LENGTH_SAMPLES * sizeof (ne10_vec2f_t) + ARRAY_GUARD_LEN * 2 * sizeof (ne10_float32_t));
    guarded_dv_neon = (ne10_vec2f_t*) NE10_MALLOC (TEST_LENGTH_SAMPLES * sizeof (ne10_vec2f_t) + + ARRAY_GUARD_LEN * 2 * sizeof (ne10_float32_t));
    dv_c = (ne10_vec2f_t*) ( (ne10_float32_t*) guarded_dv_c + ARRAY_GUARD_LEN);
    dv_neon = (ne10_vec2f_t*) ( (ne10_float32_t*) guarded_dv_neon + ARRAY_GUARD_LEN);

#if defined (REGRESSION_TEST)
    for (count = 1; count < TEST_LENGTH_SAMPLES; count++)
    {
        GUARD_ARRAY ( (ne10_float32_t*) dv_c, count * vec_size);
        GUARD_ARRAY ( (ne10_float32_t*) dv_neon, count * vec_size);

        //C version
        ne10_physics_relative_v_vec2f_c (dv_c, v_wa, ra, v_wb, rb, count);
        //neon version
        ne10_physics_relative_v_vec2f_neon (dv_neon, v_wa, ra, v_wb, rb, count);

        CHECK_ARRAY_GUARD ( (ne10_float32_t*) dv_c, count * vec_size);
        CHECK_ARRAY_GUARD ( (ne10_float32_t*) dv_neon, count * vec_size);
        printf ("----count %d\n", count);
        for (i = 0; i < count; i++)
            assert_float_vec_equal ( (ne10_float32_t*) &dv_c[i], (ne10_float32_t*) &dv_neon[i], ERROR_MARGIN_LARGE, vec_size);
    }
#endif

#if defined (SMOKE_TEST)
    for (count = 1; count < TEST_LENGTH_SAMPLES; count += 5)
    {
        GUARD_ARRAY ( (ne10_float32_t*) dv_c, count * vec_size);
        GUARD_ARRAY ( (ne10_float32_t*) dv_neon, count * vec_size);

        //C version
        ne10_physics_relative_v_vec2f_c (dv_c, v_wa, ra, v_wb, rb, count);
        //neon version
        ne10_physics_relative_v_vec2f_neon (dv_neon, v_wa, ra, v_wb, rb, count);

        CHECK_ARRAY_GUARD ( (ne10_float32_t*) dv_c, count * vec_size);
        CHECK_ARRAY_GUARD ( (ne10_float32_t*) dv_neon, count * vec_size);
        printf ("----count %d\n", count);
        for (i = 0; i < count; i++)
            assert_float_vec_equal ( (ne10_float32_t*) &dv_c[i], (ne10_float32_t*) &dv_neon[i], ERROR_MARGIN_LARGE, vec_size);
    }
#endif
    free (v_wa);
    free (v_wb);
    free (ra);
    free (rb);
    free (guarded_dv_c);
    free (guarded_dv_neon);
}

void test_relative_v_vec2f_performance()
{
    ne10_vec2f_t *guarded_dv_c, *guarded_dv_neon;
    ne10_vec2f_t *dv_c, *dv_neon;
    ne10_vec3f_t *v_wa, *v_wb;
    ne10_vec2f_t *ra, *rb;
    ne10_int32_t i;
    ne10_int32_t count;
    ne10_int32_t vec_size = sizeof (ne10_vec2f_t) / sizeof (ne10_float32_t);

    fprintf (stdout, "----------%30s start\n", __FUNCTION__);
    fprintf (stdout, "%25s%20s%20s%20s%20s\n", "count", "C Time in ms", "NEON Time in ms", "Time Savings", "Performance Ratio");

    /* init input memory */
    v_wa = (ne10_vec3f_t*) NE10_MALLOC (TEST_LENGTH_SAMPLES * sizeof (ne10_vec3f_t));
    v_wb = (ne10_vec3f_t*) NE10_MALLOC (TEST_LENGTH_SAMPLES * sizeof (ne10_vec3f_t));
    ra = (ne10_vec2f_t*) NE10_MALLOC (TEST_LENGTH_SAMPLES * sizeof (ne10_vec2f_t));
    rb = (ne10_vec2f_t*) NE10_MALLOC (TEST_LENGTH_SAMPLES * sizeof (ne10_vec2f_t));
    float_array_assignment ( (ne10_float32_t *) v_wa, TEST_LENGTH_SAMPLES * sizeof (ne10_vec3f_t) / sizeof (ne10_float32_t));
    float_array_assignment ( (ne10_float32_t *) v_wb, TEST_LENGTH_SAMPLES * sizeof (ne10_vec3f_t) / sizeof (ne10_float32_t));
    float_array_assignment ( (ne10_float32_t *) ra, TEST_LENGTH_SAMPLES * sizeof (ne10_vec2f_t) / sizeof (ne10_float32_t));
    float_array_assignment ( (ne10_float32_t *) rb, TEST_LENGTH_SAMPLES * sizeof (ne10_vec2f_t) / sizeof (ne10_float32_t));

    /* init dst memory */
    guarded_dv_c = (ne10_vec2f_t*) NE10_MALLOC (TEST_LENGTH_SAMPLES * sizeof (ne10_vec2f_t) + ARRAY_GUARD_LEN * 2 * sizeof (ne10_float32_t));
    guarded_dv_neon = (ne10_vec2f_t*) NE10_MALLOC (TEST_LENGTH_SAMPLES * sizeof (ne10_vec2f_t) + + ARRAY_GUARD_LEN * 2 * sizeof (ne10_float32_t));
    dv_c = (ne10_vec2f_t*) ( (ne10_float32_t*) guarded_dv_c + ARRAY_GUARD_LEN);
    dv_neon = (ne10_vec2f_t*) ( (ne10_float32_t*) guarded_dv_neon + ARRAY_GUARD_LEN);

    for (count = 2; count < TEST_LENGTH_SAMPLES; count += 4)
    {
        //C version
        GET_TIME
        (time_c,
        {
            for (i = 0; i < TEST_COUNT; i++)
                ne10_physics_relative_v_vec2f_c (dv_c, v_wa, ra, v_wb, rb, count);
        }
        );
        //neon version
        GET_TIME
        (time_neon,
        {
            for (i = 0; i < TEST_COUNT; i++)
                ne10_physics_relative_v_vec2f_neon (dv_neon, v_wa, ra, v_wb, rb, count);
        }
        );
        time_speedup = (ne10_float32_t) time_c / time_neon;
        time_savings = ( ( (ne10_float32_t) (time_c - time_neon)) / time_c) * 100;
        printf ("count: %10d time C: %10lld time NEON: %10lld\n", count, time_c, time_neon);
        //ne10_log (__FUNCTION__, "Compute aabb%21d%20lld%20lld%19.2f%%%18.2f:1\n", count, time_c, time_neon, time_savings, time_speedup);
    }

    free (v_wa);
    free (v_wb);
    free (ra);
    free (rb);
    free (guarded_dv_c);
    free (guarded_dv_neon);
}

void test_apply_impulse_vec2f_conformance()
{
    ne10_vec3f_t *guarded_v_wa_c, *guarded_v_wa_neon, *guarded_v_wb_c, *guarded_v_wb_neon;
    ne10_vec3f_t *v_wa_c, *v_wa_neon, *v_wb_c, *v_wb_neon;
    ne10_vec2f_t *ra, *rb, *ima, *imb, *p;
    ne10_int32_t i;
    ne10_int32_t count;
    ne10_int32_t vec_size = sizeof (ne10_vec3f_t) / sizeof (ne10_float32_t);

    fprintf (stdout, "----------%30s start\n", __FUNCTION__);

    /* init input memory */
    ra = (ne10_vec2f_t*) NE10_MALLOC (TEST_LENGTH_SAMPLES * sizeof (ne10_vec2f_t));
    rb = (ne10_vec2f_t*) NE10_MALLOC (TEST_LENGTH_SAMPLES * sizeof (ne10_vec2f_t));
    ima = (ne10_vec2f_t*) NE10_MALLOC (TEST_LENGTH_SAMPLES * sizeof (ne10_vec2f_t));
    imb = (ne10_vec2f_t*) NE10_MALLOC (TEST_LENGTH_SAMPLES * sizeof (ne10_vec2f_t));
    p = (ne10_vec2f_t*) NE10_MALLOC (TEST_LENGTH_SAMPLES * sizeof (ne10_vec2f_t));
    float_array_assignment ( (ne10_float32_t *) ra, TEST_LENGTH_SAMPLES * sizeof (ne10_vec2f_t) / sizeof (ne10_float32_t));
    float_array_assignment ( (ne10_float32_t *) rb, TEST_LENGTH_SAMPLES * sizeof (ne10_vec2f_t) / sizeof (ne10_float32_t));
    float_array_assignment ( (ne10_float32_t *) ima, TEST_LENGTH_SAMPLES * sizeof (ne10_vec2f_t) / sizeof (ne10_float32_t));
    float_array_assignment ( (ne10_float32_t *) imb, TEST_LENGTH_SAMPLES * sizeof (ne10_vec2f_t) / sizeof (ne10_float32_t));
    float_array_assignment ( (ne10_float32_t *) p, TEST_LENGTH_SAMPLES * sizeof (ne10_vec2f_t) / sizeof (ne10_float32_t));

    /* init dst memory */
    guarded_v_wa_c = (ne10_vec3f_t*) NE10_MALLOC (TEST_LENGTH_SAMPLES * sizeof (ne10_vec3f_t) + ARRAY_GUARD_LEN * 2 * sizeof (ne10_float32_t));
    guarded_v_wa_neon = (ne10_vec3f_t*) NE10_MALLOC (TEST_LENGTH_SAMPLES * sizeof (ne10_vec3f_t) + + ARRAY_GUARD_LEN * 2 * sizeof (ne10_float32_t));
    guarded_v_wb_c = (ne10_vec3f_t*) NE10_MALLOC (TEST_LENGTH_SAMPLES * sizeof (ne10_vec3f_t) + ARRAY_GUARD_LEN * 2 * sizeof (ne10_float32_t));
    guarded_v_wb_neon = (ne10_vec3f_t*) NE10_MALLOC (TEST_LENGTH_SAMPLES * sizeof (ne10_vec3f_t) + + ARRAY_GUARD_LEN * 2 * sizeof (ne10_float32_t));
    v_wa_c = (ne10_vec3f_t*) ( (ne10_float32_t*) guarded_v_wa_c + ARRAY_GUARD_LEN);
    v_wa_neon = (ne10_vec3f_t*) ( (ne10_float32_t*) guarded_v_wa_neon + ARRAY_GUARD_LEN);
    v_wb_c = (ne10_vec3f_t*) ( (ne10_float32_t*) guarded_v_wb_c + ARRAY_GUARD_LEN);
    v_wb_neon = (ne10_vec3f_t*) ( (ne10_float32_t*) guarded_v_wb_neon + ARRAY_GUARD_LEN);
    float_array_assignment ( (ne10_float32_t *) v_wa_c, TEST_LENGTH_SAMPLES * sizeof (ne10_vec3f_t) / sizeof (ne10_float32_t));
    float_array_assignment ( (ne10_float32_t *) v_wb_c, TEST_LENGTH_SAMPLES * sizeof (ne10_vec3f_t) / sizeof (ne10_float32_t));
    memcpy (v_wa_neon, v_wa_c, TEST_LENGTH_SAMPLES * sizeof (ne10_vec3f_t));
    memcpy (v_wb_neon, v_wb_c, TEST_LENGTH_SAMPLES * sizeof (ne10_vec3f_t));

#if defined (REGRESSION_TEST)
    for (count = 1; count < TEST_LENGTH_SAMPLES; count++)
    {
        GUARD_ARRAY ( (ne10_float32_t*) v_wa_c, count * vec_size);
        GUARD_ARRAY ( (ne10_float32_t*) v_wa_neon, count * vec_size);
        GUARD_ARRAY ( (ne10_float32_t*) v_wb_c, count * vec_size);
        GUARD_ARRAY ( (ne10_float32_t*) v_wb_neon, count * vec_size);

        //C version
        ne10_physics_apply_impulse_vec2f_c (v_wa_c, v_wb_c, ra, rb, ima, imb, p, count);
        //neon version
        ne10_physics_apply_impulse_vec2f_neon (v_wa_neon, v_wb_neon, ra, rb, ima, imb, p, count);

        CHECK_ARRAY_GUARD ( (ne10_float32_t*) v_wa_c, count * vec_size);
        CHECK_ARRAY_GUARD ( (ne10_float32_t*) v_wa_neon, count * vec_size);
        CHECK_ARRAY_GUARD ( (ne10_float32_t*) v_wb_c, count * vec_size);
        CHECK_ARRAY_GUARD ( (ne10_float32_t*) v_wb_neon, count * vec_size);

        printf ("----count %d\n", count);
        for (i = 0; i < count; i++)
        {
            assert_float_vec_equal ( (ne10_float32_t*) &v_wa_c[i], (ne10_float32_t*) &v_wa_neon[i], ERROR_MARGIN_LARGE, vec_size);
            assert_float_vec_equal ( (ne10_float32_t*) &v_wb_c[i], (ne10_float32_t*) &v_wb_neon[i], ERROR_MARGIN_LARGE, vec_size);
        }
    }
#endif

#if defined (SMOKE_TEST)
    for (count = 1; count < TEST_LENGTH_SAMPLES; count += 5)
    {
        GUARD_ARRAY ( (ne10_float32_t*) v_wa_c, count * vec_size);
        GUARD_ARRAY ( (ne10_float32_t*) v_wa_neon, count * vec_size);
        GUARD_ARRAY ( (ne10_float32_t*) v_wb_c, count * vec_size);
        GUARD_ARRAY ( (ne10_float32_t*) v_wb_neon, count * vec_size);

        //C version
        ne10_physics_apply_impulse_vec2f_c (v_wa_c, v_wb_c, ra, rb, ima, imb, p, count);
        //neon version
        ne10_physics_apply_impulse_vec2f_neon (v_wa_neon, v_wb_neon, ra, rb, ima, imb, p, count);

        CHECK_ARRAY_GUARD ( (ne10_float32_t*) v_wa_c, count * vec_size);
        CHECK_ARRAY_GUARD ( (ne10_float32_t*) v_wa_neon, count * vec_size);
        CHECK_ARRAY_GUARD ( (ne10_float32_t*) v_wb_c, count * vec_size);
        CHECK_ARRAY_GUARD ( (ne10_float32_t*) v_wb_neon, count * vec_size);
        printf ("----count %d\n", count);
        for (i = 0; i < count; i++)
        {
            assert_float_vec_equal ( (ne10_float32_t*) &v_wa_c[i], (ne10_float32_t*) &v_wa_neon[i], ERROR_MARGIN_LARGE, vec_size);
            assert_float_vec_equal ( (ne10_float32_t*) &v_wb_c[i], (ne10_float32_t*) &v_wb_neon[i], ERROR_MARGIN_LARGE, vec_size);
        }
    }
#endif
    free (ra);
    free (rb);
    free (ima);
    free (imb);
    free (p);
    free (guarded_v_wa_c);
    free (guarded_v_wa_neon);
    free (guarded_v_wb_c);
    free (guarded_v_wb_neon);
}

void test_apply_impulse_vec2f_performance()
{
    ne10_vec3f_t *guarded_v_wa_c, *guarded_v_wa_neon, *guarded_v_wb_c, *guarded_v_wb_neon;
    ne10_vec3f_t *v_wa_c, *v_wa_neon, *v_wb_c, *v_wb_neon;
    ne10_vec2f_t *ra, *rb, *ima, *imb, *p;
    ne10_int32_t i;
    ne10_int32_t count;
    ne10_int32_t vec_size = sizeof (ne10_vec3f_t) / sizeof (ne10_float32_t);

    fprintf (stdout, "----------%30s start\n", __FUNCTION__);
    fprintf (stdout, "%25s%20s%20s%20s%20s\n", "count", "C Time in ms", "NEON Time in ms", "Time Savings", "Performance Ratio");

    /* init input memory */
    ra = (ne10_vec2f_t*) NE10_MALLOC (TEST_LENGTH_SAMPLES * sizeof (ne10_vec2f_t));
    rb = (ne10_vec2f_t*) NE10_MALLOC (TEST_LENGTH_SAMPLES * sizeof (ne10_vec2f_t));
    ima = (ne10_vec2f_t*) NE10_MALLOC (TEST_LENGTH_SAMPLES * sizeof (ne10_vec2f_t));
    imb = (ne10_vec2f_t*) NE10_MALLOC (TEST_LENGTH_SAMPLES * sizeof (ne10_vec2f_t));
    p = (ne10_vec2f_t*) NE10_MALLOC (TEST_LENGTH_SAMPLES * sizeof (ne10_vec2f_t));
    float_array_assignment ( (ne10_float32_t *) ra, TEST_LENGTH_SAMPLES * sizeof (ne10_vec2f_t) / sizeof (ne10_float32_t));
    float_array_assignment ( (ne10_float32_t *) rb, TEST_LENGTH_SAMPLES * sizeof (ne10_vec2f_t) / sizeof (ne10_float32_t));
    float_array_assignment ( (ne10_float32_t *) ima, TEST_LENGTH_SAMPLES * sizeof (ne10_vec2f_t) / sizeof (ne10_float32_t));
    float_array_assignment ( (ne10_float32_t *) imb, TEST_LENGTH_SAMPLES * sizeof (ne10_vec2f_t) / sizeof (ne10_float32_t));
    float_array_assignment ( (ne10_float32_t *) p, TEST_LENGTH_SAMPLES * sizeof (ne10_vec2f_t) / sizeof (ne10_float32_t));

    /* init dst memory */
    guarded_v_wa_c = (ne10_vec3f_t*) NE10_MALLOC (TEST_LENGTH_SAMPLES * sizeof (ne10_vec3f_t) + ARRAY_GUARD_LEN * 2 * sizeof (ne10_float32_t));
    guarded_v_wa_neon = (ne10_vec3f_t*) NE10_MALLOC (TEST_LENGTH_SAMPLES * sizeof (ne10_vec3f_t) + + ARRAY_GUARD_LEN * 2 * sizeof (ne10_float32_t));
    guarded_v_wb_c = (ne10_vec3f_t*) NE10_MALLOC (TEST_LENGTH_SAMPLES * sizeof (ne10_vec3f_t) + ARRAY_GUARD_LEN * 2 * sizeof (ne10_float32_t));
    guarded_v_wb_neon = (ne10_vec3f_t*) NE10_MALLOC (TEST_LENGTH_SAMPLES * sizeof (ne10_vec3f_t) + + ARRAY_GUARD_LEN * 2 * sizeof (ne10_float32_t));
    v_wa_c = (ne10_vec3f_t*) ( (ne10_float32_t*) guarded_v_wa_c + ARRAY_GUARD_LEN);
    v_wa_neon = (ne10_vec3f_t*) ( (ne10_float32_t*) guarded_v_wa_neon + ARRAY_GUARD_LEN);
    v_wb_c = (ne10_vec3f_t*) ( (ne10_float32_t*) guarded_v_wb_c + ARRAY_GUARD_LEN);
    v_wb_neon = (ne10_vec3f_t*) ( (ne10_float32_t*) guarded_v_wb_neon + ARRAY_GUARD_LEN);
    float_array_assignment ( (ne10_float32_t *) v_wa_c, TEST_LENGTH_SAMPLES * sizeof (ne10_vec3f_t) / sizeof (ne10_float32_t));
    float_array_assignment ( (ne10_float32_t *) v_wb_c, TEST_LENGTH_SAMPLES * sizeof (ne10_vec3f_t) / sizeof (ne10_float32_t));
    memcpy (v_wa_neon, v_wa_c, TEST_LENGTH_SAMPLES * sizeof (ne10_vec3f_t));
    memcpy (v_wb_neon, v_wb_c, TEST_LENGTH_SAMPLES * sizeof (ne10_vec3f_t));

    for (count = 2; count < TEST_LENGTH_SAMPLES; count += 4)
    {
        //C version
        GET_TIME
        (time_c,
        {
            for (i = 0; i < TEST_COUNT; i++)
                ne10_physics_apply_impulse_vec2f_c (v_wa_c, v_wb_c, ra, rb, ima, imb, p, count);
        }
        );
        //neon version
        GET_TIME
        (time_neon,
        {
            for (i = 0; i < TEST_COUNT; i++)
                ne10_physics_apply_impulse_vec2f_neon (v_wa_neon, v_wb_neon, ra, rb, ima, imb, p, count);
        }
        );
        time_speedup = (ne10_float32_t) time_c / time_neon;
        time_savings = ( ( (ne10_float32_t) (time_c - time_neon)) / time_c) * 100;
        printf ("count: %10d time C: %10lld time NEON: %10lld\n", count, time_c, time_neon);
        //ne10_log (__FUNCTION__, "Compute aabb%21d%20lld%20lld%19.2f%%%18.2f:1\n", count, time_c, time_neon, time_savings, time_speedup);

    }
    free (ra);
    free (rb);
    free (ima);
    free (imb);
    free (p);
    free (guarded_v_wa_c);
    free (guarded_v_wa_neon);
    free (guarded_v_wb_c);
    free (guarded_v_wb_neon);
}

void test_compute_aabb_vec2f()
{
#if defined (SMOKE_TEST)||(REGRESSION_TEST)
    test_compute_aabb_vec2f_conformance();
#endif

#if defined (PERFORMANCE_TEST)
    test_compute_aabb_vec2f_performance();
#endif
}

void test_relative_v_vec2f()
{
#if defined (SMOKE_TEST)||(REGRESSION_TEST)
    test_relative_v_vec2f_conformance();
#endif

#if defined (PERFORMANCE_TEST)
    test_relative_v_vec2f_performance();
#endif
}

void test_apply_impulse_vec2f()
{
#if defined (SMOKE_TEST)||(REGRESSION_TEST)
    test_apply_impulse_vec2f_conformance();
#endif

#if defined (PERFORMANCE_TEST)
    test_apply_impulse_vec2f_performance();
#endif
}

void my_test_setup (void)
{
    //printf("------%-30s start\r\n", __FUNCTION__);
}

void my_test_teardown (void)
{
    //printf("--------end\r\n");
}

void test_fixture_physics (void)
{
    test_fixture_start();               // starts a fixture

    fixture_setup (my_test_setup);
    fixture_teardown (my_test_teardown);

    run_test (test_compute_aabb_vec2f);       // run tests
    run_test (test_relative_v_vec2f);
    run_test (test_apply_impulse_vec2f);

    test_fixture_end();                 // ends a fixture
}
