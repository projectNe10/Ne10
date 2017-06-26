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
 * NE10 Library : test_suite_fft_int32.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "NE10_dsp.h"
#include "seatest.h"
#include "unit_test_common.h"


/* ----------------------------------------------------------------------
** Global defines
** ------------------------------------------------------------------- */

/* Max FFT Length and double buffer for real and imag */
#define TEST_LENGTH_SAMPLES (32768)
#define MIN_LENGTH_SAMPLES_CPX (2)
#define MIN_LENGTH_SAMPLES_REAL (MIN_LENGTH_SAMPLES_CPX)

#define SNR_THRESHOLD_INT32 25.0f

#define TEST_COUNT 250000

#define NE10_FFT_PARA_LEVEL 4

/* ----------------------------------------------------------------------
** Defines each of the tests performed
** ------------------------------------------------------------------- */

//input and output
static ne10_int32_t testInput_i32_unscaled[TEST_LENGTH_SAMPLES * 2];
static ne10_int32_t testInput_i32_scaled[TEST_LENGTH_SAMPLES * 2];
static ne10_int32_t * guarded_in_c = NULL;
static ne10_int32_t * guarded_in_neon = NULL;
static ne10_int32_t * in_c = NULL;
static ne10_int32_t * in_neon = NULL;

static ne10_int32_t * guarded_out_c = NULL;
static ne10_int32_t * guarded_out_neon = NULL;
static ne10_int32_t * out_c = NULL;
static ne10_int32_t * out_neon = NULL;

static ne10_float32_t snr = 0.0f;

static ne10_int64_t time_c = 0;
static ne10_int64_t time_neon = 0;
static ne10_float32_t time_speedup = 0.0f;
static ne10_float32_t time_savings = 0.0f;

void test_fft_c2c_1d_int32_conformance()
{

    ne10_int32_t i = 0;
    ne10_int32_t baseSize = 0, fftSize = 0;
    ne10_int32_t factor;
    ne10_fft_cfg_int32_t cfg_c;
    ne10_fft_cfg_int32_t cfg_neon;
    ne10_float32_t * out_c_tmp = NULL;
    ne10_float32_t * out_neon_tmp = NULL;

    fprintf (stdout, "----------%30s start\n", __FUNCTION__);

    /* init input memory */
    guarded_in_c = (ne10_int32_t*) NE10_MALLOC ( (TEST_LENGTH_SAMPLES * 2 + ARRAY_GUARD_LEN * 2) * sizeof (ne10_int32_t));
    guarded_in_neon = (ne10_int32_t*) NE10_MALLOC ( (TEST_LENGTH_SAMPLES * 2 + ARRAY_GUARD_LEN * 2) * sizeof (ne10_int32_t));
    in_c = guarded_in_c + ARRAY_GUARD_LEN;
    in_neon = guarded_in_neon + ARRAY_GUARD_LEN;

    /* init dst memory */
    guarded_out_c = (ne10_int32_t*) NE10_MALLOC ( (TEST_LENGTH_SAMPLES * 2 + ARRAY_GUARD_LEN * 2) * sizeof (ne10_int32_t));
    guarded_out_neon = (ne10_int32_t*) NE10_MALLOC ( (TEST_LENGTH_SAMPLES * 2 + ARRAY_GUARD_LEN * 2) * sizeof (ne10_int32_t));
    out_c = guarded_out_c + ARRAY_GUARD_LEN;
    out_neon = guarded_out_neon + ARRAY_GUARD_LEN;

    out_c_tmp = (ne10_float32_t*) NE10_MALLOC ( (TEST_LENGTH_SAMPLES * 2) * sizeof (ne10_float32_t));
    out_neon_tmp = (ne10_float32_t*) NE10_MALLOC ( (TEST_LENGTH_SAMPLES * 2) * sizeof (ne10_float32_t));

    for (i = 0; i < TEST_LENGTH_SAMPLES * 2; i++)
    {
        testInput_i32_unscaled[i] = (ne10_int32_t) (drand48() * 8192) - 4096;
        testInput_i32_scaled[i] = (ne10_int32_t) (drand48() * NE10_F2I32_MAX) - NE10_F2I32_MAX / 2;
    }

    for (baseSize = MIN_LENGTH_SAMPLES_CPX; baseSize <= TEST_LENGTH_SAMPLES; baseSize *= 2)
    {
        factor = 2;
        fftSize = baseSize;
        while (factor && fftSize <= TEST_LENGTH_SAMPLES)
        {
            if (fftSize == 2 || fftSize % NE10_FFT_PARA_LEVEL == 0)
            {
                fprintf (stdout, "FFT size %d\n", fftSize);
                /* FFT init */
                cfg_c = ne10_fft_alloc_c2c_int32_c (fftSize);
                if (cfg_c == NULL)
                {
                    fprintf (stdout, "======ERROR, FFT alloc fails\n");
                    return;
                }

                cfg_neon = ne10_fft_alloc_c2c_int32_neon (fftSize);
                if (cfg_neon == NULL)
                {
                    NE10_FREE (cfg_c);
                    fprintf (stdout, "======ERROR, FFT alloc fails\n");
                    return;
                }

                /* unscaled FFT test */
                memcpy (in_c, testInput_i32_unscaled, 2 * fftSize * sizeof (ne10_int32_t));
                memcpy (in_neon, testInput_i32_unscaled, 2 * fftSize * sizeof (ne10_int32_t));

                GUARD_ARRAY_UINT8 ( (ne10_uint8_t*) out_c, fftSize * 2 * sizeof (ne10_int32_t));
                GUARD_ARRAY_UINT8 ( (ne10_uint8_t*) out_neon, fftSize * 2 * sizeof (ne10_int32_t));
                ne10_fft_c2c_1d_int32_c ( (ne10_fft_cpx_int32_t*) out_c, (ne10_fft_cpx_int32_t*) in_c, cfg_c, 0, 0);
                ne10_fft_c2c_1d_int32_neon ( (ne10_fft_cpx_int32_t*) out_neon, (ne10_fft_cpx_int32_t*) in_neon, cfg_neon, 0, 0);
                assert_true (CHECK_ARRAY_GUARD_UINT8 ( (ne10_uint8_t*) out_c, fftSize * 2 * sizeof (ne10_int32_t)));
                assert_true (CHECK_ARRAY_GUARD_UINT8 ( (ne10_uint8_t*) out_neon, fftSize * 2 * sizeof (ne10_int32_t)));

                //conformance test
                for (i = 0; i < fftSize * 2; i++)
                {
                    out_c_tmp[i] = (ne10_float32_t) out_c[i];
                    out_neon_tmp[i] = (ne10_float32_t) out_neon[i];
                }
                snr = CAL_SNR_FLOAT32 (out_c_tmp, out_neon_tmp, fftSize * 2);
                assert_false ( (snr < SNR_THRESHOLD_INT32));

                /* IFFT test */
                memcpy (in_c, testInput_i32_unscaled, 2 * fftSize * sizeof (ne10_int32_t));
                memcpy (in_neon, testInput_i32_unscaled, 2 * fftSize * sizeof (ne10_int32_t));

                GUARD_ARRAY_UINT8 ( (ne10_uint8_t*) out_c, fftSize * 2 * sizeof (ne10_int32_t));
                GUARD_ARRAY_UINT8 ( (ne10_uint8_t*) out_neon, fftSize * 2 * sizeof (ne10_int32_t));
                ne10_fft_c2c_1d_int32_c ( (ne10_fft_cpx_int32_t*) out_c, (ne10_fft_cpx_int32_t*) in_c, cfg_c, 1, 0);
                ne10_fft_c2c_1d_int32_neon ( (ne10_fft_cpx_int32_t*) out_neon, (ne10_fft_cpx_int32_t*) in_neon, cfg_neon, 1, 0);
                assert_true (CHECK_ARRAY_GUARD_UINT8 ( (ne10_uint8_t*) out_c, fftSize * 2 * sizeof (ne10_int32_t)));
                assert_true (CHECK_ARRAY_GUARD_UINT8 ( (ne10_uint8_t*) out_neon, fftSize * 2 * sizeof (ne10_int32_t)));

                //conformance test
                for (i = 0; i < fftSize * 2; i++)
                {
                    out_c_tmp[i] = (ne10_float32_t) out_c[i];
                    out_neon_tmp[i] = (ne10_float32_t) out_neon[i];
                }
                snr = CAL_SNR_FLOAT32 (out_c_tmp, out_neon_tmp, fftSize * 2);
                assert_false ( (snr < SNR_THRESHOLD_INT32));

                /* scaled FFT test */
                memcpy (in_c, testInput_i32_scaled, 2 * fftSize * sizeof (ne10_int32_t));
                memcpy (in_neon, testInput_i32_scaled, 2 * fftSize * sizeof (ne10_int32_t));

                GUARD_ARRAY_UINT8 ( (ne10_uint8_t*) out_c, fftSize * 2 * sizeof (ne10_int32_t));
                GUARD_ARRAY_UINT8 ( (ne10_uint8_t*) out_neon, fftSize * 2 * sizeof (ne10_int32_t));
                ne10_fft_c2c_1d_int32_c ( (ne10_fft_cpx_int32_t*) out_c, (ne10_fft_cpx_int32_t*) in_c, cfg_c, 0, 1);
                ne10_fft_c2c_1d_int32_neon ( (ne10_fft_cpx_int32_t*) out_neon, (ne10_fft_cpx_int32_t*) in_neon, cfg_neon, 0, 1);
                assert_true (CHECK_ARRAY_GUARD_UINT8 ( (ne10_uint8_t*) out_c, fftSize * 2 * sizeof (ne10_int32_t)));
                assert_true (CHECK_ARRAY_GUARD_UINT8 ( (ne10_uint8_t*) out_neon, fftSize * 2 * sizeof (ne10_int32_t)));

                //conformance test
                for (i = 0; i < fftSize * 2; i++)
                {
                    out_c_tmp[i] = (ne10_float32_t) out_c[i];
                    out_neon_tmp[i] = (ne10_float32_t) out_neon[i];
                }
                snr = CAL_SNR_FLOAT32 (out_c_tmp, out_neon_tmp, fftSize * 2);
                assert_false ( (snr < SNR_THRESHOLD_INT32));

                /* IFFT test */
                memcpy (in_c, testInput_i32_scaled, 2 * fftSize * sizeof (ne10_int32_t));
                memcpy (in_neon, testInput_i32_scaled, 2 * fftSize * sizeof (ne10_int32_t));

                GUARD_ARRAY_UINT8 ( (ne10_uint8_t*) out_c, fftSize * 2 * sizeof (ne10_int32_t));
                GUARD_ARRAY_UINT8 ( (ne10_uint8_t*) out_neon, fftSize * 2 * sizeof (ne10_int32_t));
                ne10_fft_c2c_1d_int32_c ( (ne10_fft_cpx_int32_t*) out_c, (ne10_fft_cpx_int32_t*) in_c, cfg_c, 1, 1);
                ne10_fft_c2c_1d_int32_neon ( (ne10_fft_cpx_int32_t*) out_neon, (ne10_fft_cpx_int32_t*) in_neon, cfg_neon, 1, 1);
                assert_true (CHECK_ARRAY_GUARD_UINT8 ( (ne10_uint8_t*) out_c, fftSize * 2 * sizeof (ne10_int32_t)));
                assert_true (CHECK_ARRAY_GUARD_UINT8 ( (ne10_uint8_t*) out_neon, fftSize * 2 * sizeof (ne10_int32_t)));

                //conformance test
                for (i = 0; i < fftSize * 2; i++)
                {
                    out_c_tmp[i] = (ne10_float32_t) out_c[i];
                    out_neon_tmp[i] = (ne10_float32_t) out_neon[i];
                }
                snr = CAL_SNR_FLOAT32 (out_c_tmp, out_neon_tmp, fftSize * 2);
                assert_false ( (snr < SNR_THRESHOLD_INT32));

                NE10_FREE (cfg_c);
                NE10_FREE (cfg_neon);
            }

            switch (factor)
            {
            case 2:
                factor = 3;
                fftSize += (fftSize / 2);
                break;
            case 3:
                factor = 5;
                fftSize += (fftSize / 3) * 2;
                break;
            case 5:
                factor = 15;
                fftSize += (fftSize * 2);
                break;
            case 15:
                factor = 0;
                break;
            }
        }
    }

    NE10_FREE (guarded_in_c);
    NE10_FREE (guarded_in_neon);
    NE10_FREE (guarded_out_c);
    NE10_FREE (guarded_out_neon);
    NE10_FREE (out_c_tmp);
    NE10_FREE (out_neon_tmp);
}

void test_fft_c2c_1d_int32_performance()
{

    ne10_int32_t i = 0;
    ne10_int32_t fftSize = 0;
    ne10_fft_cfg_int32_t cfg_c;
    ne10_fft_cfg_int32_t cfg_neon;
    ne10_int32_t test_loop = 0;

    fprintf (stdout, "----------%30s start\n", __FUNCTION__);
    fprintf (stdout, "%25s%20s%20s%20s%20s\n", "FFT Length", "C Time (micro-s)", "NEON Time (micro-s)", "Time Savings", "Performance Ratio");

    /* init input memory */
    guarded_in_c = (ne10_int32_t*) NE10_MALLOC ( (TEST_LENGTH_SAMPLES * 2 + ARRAY_GUARD_LEN * 2) * sizeof (ne10_int32_t));
    guarded_in_neon = (ne10_int32_t*) NE10_MALLOC ( (TEST_LENGTH_SAMPLES * 2 + ARRAY_GUARD_LEN * 2) * sizeof (ne10_int32_t));
    in_c = guarded_in_c + ARRAY_GUARD_LEN;
    in_neon = guarded_in_neon + ARRAY_GUARD_LEN;

    /* init dst memory */
    guarded_out_c = (ne10_int32_t*) NE10_MALLOC ( (TEST_LENGTH_SAMPLES * 2 + ARRAY_GUARD_LEN * 2) * sizeof (ne10_int32_t));
    guarded_out_neon = (ne10_int32_t*) NE10_MALLOC ( (TEST_LENGTH_SAMPLES * 2 + ARRAY_GUARD_LEN * 2) * sizeof (ne10_int32_t));
    out_c = guarded_out_c + ARRAY_GUARD_LEN;
    out_neon = guarded_out_neon + ARRAY_GUARD_LEN;

    for (i = 0; i < TEST_LENGTH_SAMPLES * 2; i++)
    {
        testInput_i32_unscaled[i] = (ne10_int32_t) (drand48() * 8192) - 4096;
        testInput_i32_scaled[i] = (ne10_int32_t) (drand48() * NE10_F2I32_MAX) - NE10_F2I32_MAX / 2;
    }
    for (fftSize = MIN_LENGTH_SAMPLES_CPX; fftSize <= TEST_LENGTH_SAMPLES; fftSize *= 2)
    {
        fprintf (stdout, "FFT size %d\n", fftSize);

        /* FFT test */
        memcpy (in_c, testInput_i32_unscaled, 2 * fftSize * sizeof (ne10_int32_t));
        memcpy (in_neon, testInput_i32_unscaled, 2 * fftSize * sizeof (ne10_int32_t));
        cfg_c = ne10_fft_alloc_c2c_int32_c (fftSize);
        if (cfg_c == NULL)
        {
            fprintf (stdout, "======ERROR, FFT alloc fails\n");
            return;
        }

        cfg_neon = ne10_fft_alloc_c2c_int32_neon (fftSize);
        if (cfg_neon == NULL)
        {
            NE10_FREE (cfg_c);
            fprintf (stdout, "======ERROR, FFT alloc fails\n");
            return;
        }

        test_loop = TEST_COUNT / fftSize;

        GET_TIME
        (
            time_c,
        {
            for (i = 0; i < test_loop; i++)
                ne10_fft_c2c_1d_int32_c ( (ne10_fft_cpx_int32_t*) out_c, (ne10_fft_cpx_int32_t*) in_c, cfg_c, 0, 0);
        }
        );
        GET_TIME
        (
            time_neon,
        {
            for (i = 0; i < test_loop; i++)
                ne10_fft_c2c_1d_int32_neon ( (ne10_fft_cpx_int32_t*) out_c, (ne10_fft_cpx_int32_t*) in_c, cfg_neon, 0, 0);
        }
        );
        time_speedup = (ne10_float32_t) time_c / time_neon;
        time_savings = ( ( (ne10_float32_t) (time_c - time_neon)) / time_c) * 100;
        ne10_log (__FUNCTION__, " unscaled FFT%21d%20lld%20lld%19.2f%%%18.2f:1\n", fftSize, time_c, time_neon, time_savings, time_speedup);

        /* IFFT test */
        memcpy (in_c, out_c, 2 * fftSize * sizeof (ne10_int32_t));
        memcpy (in_neon, out_c, 2 * fftSize * sizeof (ne10_int32_t));

        GET_TIME
        (
            time_c,
        {
            for (i = 0; i < test_loop; i++)
                ne10_fft_c2c_1d_int32_c ( (ne10_fft_cpx_int32_t*) out_c, (ne10_fft_cpx_int32_t*) in_c, cfg_c, 1, 0);
        }
        );
        GET_TIME
        (
            time_neon,
        {
            for (i = 0; i < test_loop; i++)
                ne10_fft_c2c_1d_int32_neon ( (ne10_fft_cpx_int32_t*) out_neon, (ne10_fft_cpx_int32_t*) in_neon, cfg_neon, 1, 0);
        }
        );

        time_speedup = (ne10_float32_t) time_c / time_neon;
        time_savings = ( ( (ne10_float32_t) (time_c - time_neon)) / time_c) * 100;
        ne10_log (__FUNCTION__, "unscaled IFFT%21d%20lld%20lld%19.2f%%%18.2f:1\n", fftSize, time_c, time_neon, time_savings, time_speedup);

        /* FFT test */
        memcpy (in_c, testInput_i32_scaled, 2 * fftSize * sizeof (ne10_int32_t));
        memcpy (in_neon, testInput_i32_scaled, 2 * fftSize * sizeof (ne10_int32_t));

        GET_TIME
        (
            time_c,
        {
            for (i = 0; i < test_loop; i++)
                ne10_fft_c2c_1d_int32_c ( (ne10_fft_cpx_int32_t*) out_c, (ne10_fft_cpx_int32_t*) in_c, cfg_c, 0, 1);
        }
        );
        GET_TIME
        (
            time_neon,
        {
            for (i = 0; i < test_loop; i++)
                ne10_fft_c2c_1d_int32_neon ( (ne10_fft_cpx_int32_t*) out_c, (ne10_fft_cpx_int32_t*) in_c, cfg_neon, 0, 1);
        }
        );
        time_speedup = (ne10_float32_t) time_c / time_neon;
        time_savings = ( ( (ne10_float32_t) (time_c - time_neon)) / time_c) * 100;
        ne10_log (__FUNCTION__, "   scaled FFT%21d%20lld%20lld%19.2f%%%18.2f:1\n", fftSize, time_c, time_neon, time_savings, time_speedup);

        /* IFFT test */
        memcpy (in_c, out_c, 2 * fftSize * sizeof (ne10_int32_t));
        memcpy (in_neon, out_c, 2 * fftSize * sizeof (ne10_int32_t));

        GET_TIME
        (
            time_c,
        {
            for (i = 0; i < test_loop; i++)
                ne10_fft_c2c_1d_int32_c ( (ne10_fft_cpx_int32_t*) out_c, (ne10_fft_cpx_int32_t*) in_c, cfg_c, 1, 1);
        }
        );
        GET_TIME
        (
            time_neon,
        {
            for (i = 0; i < test_loop; i++)
                ne10_fft_c2c_1d_int32_neon ( (ne10_fft_cpx_int32_t*) out_neon, (ne10_fft_cpx_int32_t*) in_neon, cfg_neon, 1, 1);
        }
        );

        time_speedup = (ne10_float32_t) time_c / time_neon;
        time_savings = ( ( (ne10_float32_t) (time_c - time_neon)) / time_c) * 100;
        ne10_log (__FUNCTION__, "  scaled IFFT%21d%20lld%20lld%19.2f%%%18.2f:1\n", fftSize, time_c, time_neon, time_savings, time_speedup);

        NE10_FREE (cfg_c);
        NE10_FREE (cfg_neon);
    }

    NE10_FREE (guarded_in_c);
    NE10_FREE (guarded_in_neon);
    NE10_FREE (guarded_out_c);
    NE10_FREE (guarded_out_neon);
}

void test_fft_r2c_1d_int32_conformance()
{

    ne10_int32_t i = 0;
    ne10_int32_t fftSize = 0;
    ne10_fft_r2c_cfg_int32_t cfg;
    ne10_float32_t * out_c_tmp = NULL;
    ne10_float32_t * out_neon_tmp = NULL;

    fprintf (stdout, "----------%30s start\n", __FUNCTION__);

    /* init input memory */
    guarded_in_c = (ne10_int32_t*) NE10_MALLOC ( (TEST_LENGTH_SAMPLES * 2 + ARRAY_GUARD_LEN * 2) * sizeof (ne10_int32_t));
    guarded_in_neon = (ne10_int32_t*) NE10_MALLOC ( (TEST_LENGTH_SAMPLES * 2 + ARRAY_GUARD_LEN * 2) * sizeof (ne10_int32_t));
    in_c = guarded_in_c + ARRAY_GUARD_LEN;
    in_neon = guarded_in_neon + ARRAY_GUARD_LEN;

    /* init dst memory */
    guarded_out_c = (ne10_int32_t*) NE10_MALLOC ( (TEST_LENGTH_SAMPLES * 2 + ARRAY_GUARD_LEN * 2) * sizeof (ne10_int32_t));
    guarded_out_neon = (ne10_int32_t*) NE10_MALLOC ( (TEST_LENGTH_SAMPLES * 2 + ARRAY_GUARD_LEN * 2) * sizeof (ne10_int32_t));
    out_c = guarded_out_c + ARRAY_GUARD_LEN;
    out_neon = guarded_out_neon + ARRAY_GUARD_LEN;

    out_c_tmp = (ne10_float32_t*) NE10_MALLOC ( (TEST_LENGTH_SAMPLES * 2) * sizeof (ne10_float32_t));
    out_neon_tmp = (ne10_float32_t*) NE10_MALLOC ( (TEST_LENGTH_SAMPLES * 2) * sizeof (ne10_float32_t));

    for (i = 0; i < TEST_LENGTH_SAMPLES * 2; i++)
    {
        testInput_i32_unscaled[i] = (ne10_int32_t) (drand48() * 8192) - 4096;
        testInput_i32_scaled[i] = (ne10_int32_t) (drand48() * NE10_F2I32_MAX) - NE10_F2I32_MAX / 2;
    }
    for (fftSize = MIN_LENGTH_SAMPLES_REAL; fftSize <= TEST_LENGTH_SAMPLES; fftSize *= 2)
    {
        fprintf (stdout, "FFT size %d\n", fftSize);
        /* FFT init */
        cfg = ne10_fft_alloc_r2c_int32 (fftSize);
        if (cfg == NULL)
        {
            fprintf (stdout, "======ERROR, FFT alloc fails\n");
            return;
        }

        /* unscaled FFT test */
        memcpy (in_c, testInput_i32_unscaled, fftSize * sizeof (ne10_int32_t));
        memcpy (in_neon, testInput_i32_unscaled, fftSize * sizeof (ne10_int32_t));

        GUARD_ARRAY_UINT8 ( (ne10_uint8_t*) out_c, (fftSize / 2 + 1) * 2 * sizeof (ne10_int32_t));
        GUARD_ARRAY_UINT8 ( (ne10_uint8_t*) out_neon, (fftSize / 2 + 1) * 2 * sizeof (ne10_int32_t));

        ne10_fft_r2c_1d_int32_c ( (ne10_fft_cpx_int32_t*) out_c, in_c, cfg, 0);
        ne10_fft_r2c_1d_int32_neon ( (ne10_fft_cpx_int32_t*) out_neon, in_neon, cfg, 0);

        assert_true (CHECK_ARRAY_GUARD_UINT8 ( (ne10_uint8_t*) out_c, (fftSize / 2 + 1) * 2 * sizeof (ne10_int32_t)));
        assert_true (CHECK_ARRAY_GUARD_UINT8 ( (ne10_uint8_t*) out_neon, (fftSize / 2 + 1) * 2 * sizeof (ne10_int32_t)));

        //conformance test
        for (i = 0; i < (fftSize / 2 + 1) * 2; i++)
        {
            out_c_tmp[i] = (ne10_float32_t) out_c[i];
            out_neon_tmp[i] = (ne10_float32_t) out_neon[i];
        }
        snr = CAL_SNR_FLOAT32 (out_c_tmp, out_neon_tmp, (fftSize / 2 + 1) * 2);
        assert_false ( (snr < SNR_THRESHOLD_INT32));

        /* IFFT test */
        for (i = 1; i < (fftSize / 2); i++)
        {
            in_c[2 * i] = testInput_i32_unscaled[2 * i];
            in_c[2 * i + 1] = testInput_i32_unscaled[2 * i + 1];
            in_c[2 * (fftSize - i)] = in_c[2 * i];
            in_c[2 * (fftSize - i) + 1] = -in_c[2 * i + 1];
        }
        in_c[0] = testInput_i32_unscaled[0];
        in_c[1] = 0;
        in_c[fftSize] = testInput_i32_unscaled[1];
        in_c[fftSize + 1] = 0;
        memcpy (in_neon, in_c, fftSize * 2 * sizeof (ne10_int32_t));

        GUARD_ARRAY_UINT8 ( (ne10_uint8_t*) out_c, fftSize * sizeof (ne10_int32_t));
        GUARD_ARRAY_UINT8 ( (ne10_uint8_t*) out_neon, fftSize * sizeof (ne10_int32_t));

        ne10_fft_c2r_1d_int32_c (out_c, (ne10_fft_cpx_int32_t*) in_c, cfg, 0);
        ne10_fft_c2r_1d_int32_neon (out_neon, (ne10_fft_cpx_int32_t*) in_neon, cfg, 0);

        assert_true (CHECK_ARRAY_GUARD_UINT8 ( (ne10_uint8_t*) out_c, fftSize * sizeof (ne10_int32_t)));
        assert_true (CHECK_ARRAY_GUARD_UINT8 ( (ne10_uint8_t*) out_neon, fftSize * sizeof (ne10_int32_t)));

        //conformance test
        for (i = 0; i < fftSize; i++)
        {
            out_c_tmp[i] = (ne10_float32_t) out_c[i];
            out_neon_tmp[i] = (ne10_float32_t) out_neon[i];
        }
        snr = CAL_SNR_FLOAT32 (out_c_tmp, out_neon_tmp, fftSize);
        assert_false ( (snr < SNR_THRESHOLD_INT32));

        /* scaled FFT test */
        memcpy (in_c, testInput_i32_scaled, fftSize * sizeof (ne10_int32_t));
        memcpy (in_neon, testInput_i32_scaled, fftSize * sizeof (ne10_int32_t));

        GUARD_ARRAY_UINT8 ( (ne10_uint8_t*) out_c, (fftSize / 2 + 1) * 2 * sizeof (ne10_int32_t));
        GUARD_ARRAY_UINT8 ( (ne10_uint8_t*) out_neon, (fftSize / 2 + 1) * 2 * sizeof (ne10_int32_t));

        ne10_fft_r2c_1d_int32_c ( (ne10_fft_cpx_int32_t*) out_c, in_c, cfg, 1);
        ne10_fft_r2c_1d_int32_neon ( (ne10_fft_cpx_int32_t*) out_neon, in_neon, cfg, 1);

        assert_true (CHECK_ARRAY_GUARD_UINT8 ( (ne10_uint8_t*) out_c, (fftSize / 2 + 1) * 2 * sizeof (ne10_int32_t)));
        assert_true (CHECK_ARRAY_GUARD_UINT8 ( (ne10_uint8_t*) out_neon, (fftSize / 2 + 1) * 2 * sizeof (ne10_int32_t)));

        //conformance test
        for (i = 0; i < (fftSize / 2 + 1) * 2; i++)
        {
            out_c_tmp[i] = (ne10_float32_t) out_c[i];
            out_neon_tmp[i] = (ne10_float32_t) out_neon[i];
        }
        snr = CAL_SNR_FLOAT32 (out_c_tmp, out_neon_tmp, (fftSize / 2 + 1) * 2);
        assert_false ( (snr < SNR_THRESHOLD_INT32));

        /* IFFT test */
        for (i = 1; i < (fftSize / 2); i++)
        {
            in_c[2 * i] = testInput_i32_scaled[2 * i];
            in_c[2 * i + 1] = testInput_i32_scaled[2 * i + 1];
            in_c[2 * (fftSize - i)] = in_c[2 * i];
            in_c[2 * (fftSize - i) + 1] = -in_c[2 * i + 1];
        }
        in_c[0] = testInput_i32_scaled[0];
        in_c[1] = 0;
        in_c[fftSize] = testInput_i32_scaled[1];
        in_c[fftSize + 1] = 0;
        memcpy (in_neon, in_c, fftSize * 2 * sizeof (ne10_int32_t));

        GUARD_ARRAY_UINT8 ( (ne10_uint8_t*) out_c, fftSize * sizeof (ne10_int32_t));
        GUARD_ARRAY_UINT8 ( (ne10_uint8_t*) out_neon, fftSize * sizeof (ne10_int32_t));

        ne10_fft_c2r_1d_int32_c (out_c, (ne10_fft_cpx_int32_t*) in_c, cfg, 1);
        ne10_fft_c2r_1d_int32_neon (out_neon, (ne10_fft_cpx_int32_t*) in_neon, cfg, 1);

        assert_true (CHECK_ARRAY_GUARD_UINT8 ( (ne10_uint8_t*) out_c, fftSize * sizeof (ne10_int32_t)));
        assert_true (CHECK_ARRAY_GUARD_UINT8 ( (ne10_uint8_t*) out_neon, fftSize * sizeof (ne10_int32_t)));

        //conformance test
        for (i = 0; i < fftSize; i++)
        {
            out_c_tmp[i] = (ne10_float32_t) out_c[i];
            out_neon_tmp[i] = (ne10_float32_t) out_neon[i];
        }
        snr = CAL_SNR_FLOAT32 (out_c_tmp, out_neon_tmp, fftSize);
        assert_false ( (snr < SNR_THRESHOLD_INT32));


        NE10_FREE (cfg);
    }

    NE10_FREE (guarded_in_c);
    NE10_FREE (guarded_in_neon);
    NE10_FREE (guarded_out_c);
    NE10_FREE (guarded_out_neon);
    NE10_FREE (out_c_tmp);
    NE10_FREE (out_neon_tmp);
}

void test_fft_r2c_1d_int32_performance()
{

    ne10_int32_t i = 0;
    ne10_int32_t fftSize = 0;
    ne10_fft_r2c_cfg_int32_t cfg;
    ne10_int32_t test_loop = 0;

    fprintf (stdout, "----------%30s start\n", __FUNCTION__);
    fprintf (stdout, "%25s%20s%20s%20s%20s\n", "FFT Length", "C Time (micro-s)", "NEON Time (micro-s)", "Time Savings", "Performance Ratio");

    /* init input memory */
    guarded_in_c = (ne10_int32_t*) NE10_MALLOC ( (TEST_LENGTH_SAMPLES * 2 + ARRAY_GUARD_LEN * 2) * sizeof (ne10_int32_t));
    guarded_in_neon = (ne10_int32_t*) NE10_MALLOC ( (TEST_LENGTH_SAMPLES * 2 + ARRAY_GUARD_LEN * 2) * sizeof (ne10_int32_t));
    in_c = guarded_in_c + ARRAY_GUARD_LEN;
    in_neon = guarded_in_neon + ARRAY_GUARD_LEN;

    /* init dst memory */
    guarded_out_c = (ne10_int32_t*) NE10_MALLOC ( (TEST_LENGTH_SAMPLES * 2 + ARRAY_GUARD_LEN * 2) * sizeof (ne10_int32_t));
    guarded_out_neon = (ne10_int32_t*) NE10_MALLOC ( (TEST_LENGTH_SAMPLES * 2 + ARRAY_GUARD_LEN * 2) * sizeof (ne10_int32_t));
    out_c = guarded_out_c + ARRAY_GUARD_LEN;
    out_neon = guarded_out_neon + ARRAY_GUARD_LEN;

    for (i = 0; i < TEST_LENGTH_SAMPLES * 2; i++)
    {
        testInput_i32_unscaled[i] = (ne10_int32_t) (drand48() * 8192) - 4096;
        testInput_i32_scaled[i] = (ne10_int32_t) (drand48() * NE10_F2I32_MAX) - NE10_F2I32_MAX / 2;
    }
    for (fftSize = MIN_LENGTH_SAMPLES_REAL; fftSize <= TEST_LENGTH_SAMPLES; fftSize *= 2)
    {
        fprintf (stdout, "FFT size %d\n", fftSize);

        cfg = ne10_fft_alloc_r2c_int32 (fftSize);
        if (cfg == NULL)
        {
            fprintf (stdout, "======ERROR, FFT alloc fails\n");
            return;
        }
        test_loop = TEST_COUNT / fftSize;
        /* unscaled FFT test */
        memcpy (in_c, testInput_i32_unscaled, fftSize * sizeof (ne10_int32_t));
        memcpy (in_neon, testInput_i32_unscaled, fftSize * sizeof (ne10_int32_t));

        GET_TIME
        (
            time_c,
        {
            for (i = 0; i < test_loop; i++)
                ne10_fft_r2c_1d_int32_c ( (ne10_fft_cpx_int32_t*) out_c, in_c, cfg, 0);
        }
        );
        GET_TIME
        (
            time_neon,
        {
            for (i = 0; i < test_loop; i++)
                ne10_fft_r2c_1d_int32_neon ( (ne10_fft_cpx_int32_t*) out_neon, in_neon, cfg, 0);
        }
        );

        time_speedup = (ne10_float32_t) time_c / time_neon;
        time_savings = ( ( (ne10_float32_t) (time_c - time_neon)) / time_c) * 100;
        ne10_log (__FUNCTION__, "Int32 unscaled RFFT%21d%20lld%20lld%19.2f%%%18.2f:1\n", fftSize, time_c, time_neon, time_savings, time_speedup);

        /* IFFT test */
        for (i = 1; i < (fftSize / 2); i++)
        {
            in_c[2 * i] = testInput_i32_unscaled[2 * i];
            in_c[2 * i + 1] = testInput_i32_unscaled[2 * i + 1];
            in_c[2 * (fftSize - i)] = in_c[2 * i];
            in_c[2 * (fftSize - i) + 1] = -in_c[2 * i + 1];
        }
        in_c[0] = testInput_i32_unscaled[0];
        in_c[1] = 0;
        in_c[fftSize] = testInput_i32_unscaled[1];
        in_c[fftSize + 1] = 0;
        memcpy (in_neon, in_c, fftSize * 2 * sizeof (ne10_int32_t));

        GET_TIME
        (
            time_c,
        {
            for (i = 0; i < test_loop; i++)
                ne10_fft_c2r_1d_int32_c (out_c, (ne10_fft_cpx_int32_t*) in_c, cfg, 0);
        }
        );
        GET_TIME
        (
            time_neon,
        {
            for (i = 0; i < test_loop; i++)
                ne10_fft_c2r_1d_int32_neon (out_neon, (ne10_fft_cpx_int32_t*) in_neon, cfg, 0);
        }
        );

        time_speedup = (ne10_float32_t) time_c / time_neon;
        time_savings = ( ( (ne10_float32_t) (time_c - time_neon)) / time_c) * 100;
        ne10_log (__FUNCTION__, "Int32 unscaled RIFFT%21d%20lld%20lld%19.2f%%%18.2f:1\n", fftSize, time_c, time_neon, time_savings, time_speedup);

        /* scaled FFT test */
        memcpy (in_c, testInput_i32_scaled, fftSize * sizeof (ne10_int32_t));
        memcpy (in_neon, testInput_i32_scaled, fftSize * sizeof (ne10_int32_t));

        GET_TIME
        (
            time_c,
        {
            for (i = 0; i < test_loop; i++)
                ne10_fft_r2c_1d_int32_c ( (ne10_fft_cpx_int32_t*) out_c, in_c, cfg, 1);
        }
        );
        GET_TIME
        (
            time_neon,
        {
            for (i = 0; i < test_loop; i++)
                ne10_fft_r2c_1d_int32_neon ( (ne10_fft_cpx_int32_t*) out_neon, in_neon, cfg, 1);
        }
        );

        time_speedup = (ne10_float32_t) time_c / time_neon;
        time_savings = ( ( (ne10_float32_t) (time_c - time_neon)) / time_c) * 100;
        ne10_log (__FUNCTION__, "Int32 scaled RFFT%21d%20lld%20lld%19.2f%%%18.2f:1\n", fftSize, time_c, time_neon, time_savings, time_speedup);

        /* IFFT test */
        for (i = 1; i < (fftSize / 2); i++)
        {
            in_c[2 * i] = testInput_i32_scaled[2 * i];
            in_c[2 * i + 1] = testInput_i32_scaled[2 * i + 1];
            in_c[2 * (fftSize - i)] = in_c[2 * i];
            in_c[2 * (fftSize - i) + 1] = -in_c[2 * i + 1];
        }
        in_c[0] = testInput_i32_scaled[0];
        in_c[1] = 0;
        in_c[fftSize] = testInput_i32_scaled[1];
        in_c[fftSize + 1] = 0;
        memcpy (in_neon, in_c, fftSize * 2 * sizeof (ne10_int32_t));

        GET_TIME
        (
            time_c,
        {
            for (i = 0; i < test_loop; i++)
                ne10_fft_c2r_1d_int32_c (out_c, (ne10_fft_cpx_int32_t*) in_c, cfg, 1);
        }
        );
        GET_TIME
        (
            time_neon,
        {
            for (i = 0; i < test_loop; i++)
                ne10_fft_c2r_1d_int32_neon (out_neon, (ne10_fft_cpx_int32_t*) in_neon, cfg, 1);
        }
        );

        time_speedup = (ne10_float32_t) time_c / time_neon;
        time_savings = ( ( (ne10_float32_t) (time_c - time_neon)) / time_c) * 100;
        ne10_log (__FUNCTION__, "Int32 scaled RIFFT%21d%20lld%20lld%19.2f%%%18.2f:1\n", fftSize, time_c, time_neon, time_savings, time_speedup);

        NE10_FREE (cfg);
    }

    NE10_FREE (guarded_in_c);
    NE10_FREE (guarded_in_neon);
    NE10_FREE (guarded_out_c);
    NE10_FREE (guarded_out_neon);
}

void test_fft_c2c_1d_int32()
{
#if defined (SMOKE_TEST)||(REGRESSION_TEST)
    test_fft_c2c_1d_int32_conformance();
#endif

#if defined (PERFORMANCE_TEST)
    test_fft_c2c_1d_int32_performance();
#endif
}

void test_fft_r2c_1d_int32()
{
#if defined (SMOKE_TEST)||(REGRESSION_TEST)
    test_fft_r2c_1d_int32_conformance();
#endif

#if defined (PERFORMANCE_TEST)
    test_fft_r2c_1d_int32_performance();
#endif
}

static void my_test_setup (void)
{
    ne10_log_buffer_ptr = ne10_log_buffer;
}

void test_fixture_fft_c2c_1d_int32 (void)
{
    test_fixture_start();               // starts a fixture

    fixture_setup (my_test_setup);

    run_test (test_fft_c2c_1d_int32);       // run tests

    test_fixture_end();                 // ends a fixture
}

void test_fixture_fft_r2c_1d_int32 (void)
{
    test_fixture_start();               // starts a fixture

    fixture_setup (my_test_setup);

    run_test (test_fft_r2c_1d_int32);       // run tests

    test_fixture_end();                 // ends a fixture
}
