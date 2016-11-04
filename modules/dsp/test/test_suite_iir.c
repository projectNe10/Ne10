/*
 *  Copyright 2012-15 ARM Limited and Contributors.
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
 * NE10 Library : test_suite_iir.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "NE10_dsp.h"
#include "seatest.h"


/* ----------------------------------------------------------------------
** Global defines
** ------------------------------------------------------------------- */

/* Max FFT Length 1024 and double buffer for real and imag */
#define TEST_LENGTH_SAMPLES 320
#define MAX_BLOCKSIZE 320
#define MAX_NUMTAPS 100

#define TEST_COUNT 5000

/* ----------------------------------------------------------------------
** Coefficients of 1-tap filter for F32
** ------------------------------------------------------------------- */

static ne10_float32_t testkCoeffs1[1] =
{
    -0.3249
};
static ne10_float32_t testvCoeffs1[2] =
{
    0.447214,    0.337540
};

/* ----------------------------------------------------------------------
** Coefficients of 9-tap filter for F32, Q31, Q15
** ------------------------------------------------------------------- */

static ne10_float32_t testkCoeffs9[9] =
{
    -0.003320,    0.035949,    -0.164096,    0.406018,    -0.633594,    0.764885,    -0.817318,    0.893064,
    -0.748373
};
static ne10_float32_t testvCoeffs9[10] =
{
    -0.013805,    -0.001180,    0.075167,    0.156646,    0.156373,    0.093161,    0.036815,    0.009947,
    0.001679,    0.000133
};

/* ----------------------------------------------------------------------
** Coefficients of 8-tap filter for F32, Q31, Q15
** ------------------------------------------------------------------- */

static ne10_float32_t testkCoeffs8[8] =
{
    0.006226,    -0.059956,    0.238433,    -0.507424,    0.708901,    -0.798284,    0.881225,    -0.754774

};
static ne10_float32_t testvCoeffs8[9] =
{
    -0.018552,    0.019153,    0.124951,    0.186823,    0.143778,    0.067568,    0.020944,    0.004009,
    0.000358
};

/* ----------------------------------------------------------------------
** Coefficients of 10-tap filter for F32
** ------------------------------------------------------------------- */

static ne10_float32_t testkCoeffs10[10] = { 0.001770,    -0.021279,    0.109785,    -0.312208,    0.551053,    -0.711844,    0.797513,    -0.828316,
        0.902786,    -0.741338
};

static ne10_float32_t testvCoeffs10[11] =
{
    -0.008154,    -0.009240,    0.037339,    0.117832,    0.151836,    0.113971,    0.055862,    0.019182,
    0.004598,    0.000694,    0.000050
};

/* ----------------------------------------------------------------------
** Coefficients of 10-tap filter for F32
** ------------------------------------------------------------------- */

static ne10_float32_t testkCoeffs33[33] =
{
    0.001770,    -0.021279,    0.109785,    -0.312208,    0.551053,    -0.711844,    0.797513,    -0.828316, 0.902786,    -0.741338,
    0.001770,    -0.021279,    0.109785,    -0.312208,    0.551053,    -0.711844,    0.797513,    -0.828316, 0.902786,    -0.741338,
    0.001770,    -0.021279,    0.109785,    -0.312208,    0.551053,    -0.711844,    0.797513,    -0.828316, 0.902786,    -0.741338,
    0.001770,    -0.021279,    0.109785
};

static ne10_float32_t testvCoeffs33[34] =
{
    -0.008154,    -0.009240,    0.037339,    0.117832,    0.151836,    0.113971,    0.055862,    0.019182, 0.004598,    0.000050,
    -0.008154,    -0.009240,    0.037339,    0.117832,    0.151836,    0.113971,    0.055862,    0.019182, 0.004598,    0.000694,
    -0.008154,    -0.009240,    0.037339,    0.117832,    0.151836,    0.113971,    0.055862,    0.019182, 0.004598,    0.000694,
    -0.008154,    -0.009240,    0.037339,    0.117832
};

/* ----------------------------------------------------------------------
** Coefficients of 2-tap filter    for F32
** ------------------------------------------------------------------- */

static ne10_float32_t testkCoeffs2[2] = { 0.2722, -0.5878 };

static ne10_float32_t testvCoeffs2[3] =
{
    0.3072,     0.3603,     0.1311
};

/* ----------------------------------------------------------------------
** Test input data for F32
** Generated by the MATLAB rand() function
** ------------------------------------------------------------------- */

static ne10_float32_t testInput_f32[TEST_LENGTH_SAMPLES] =
{
    -0.432565,    -1.665584,    0.125332,    0.287676,    -1.146471,    1.190915,    1.189164,    -0.037633,
    0.327292,    0.174639,    -0.186709,    0.725791,    -0.588317,    2.183186,    -0.136396,    0.113931,
    1.066768,    0.059281,    -0.095648,    -0.832349,    0.294411,    -1.336182,    0.714325,    1.623562,
    -0.691776,    0.857997,    1.254001,    -1.593730,    -1.440964,    0.571148,    -0.399886,    0.689997,
    0.815622,    0.711908,    1.290250,    0.668601,    1.190838,    -1.202457,    -0.019790,    -0.156717,
    -1.604086,    0.257304,    -1.056473,    1.415141,    -0.805090,    0.528743,    0.219321,    -0.921902,
    -2.170674,    -0.059188,    -1.010634,    0.614463,    0.507741,    1.692430,    0.591283,    -0.643595,
    0.380337,    -1.009116,    -0.019511,    -0.048221,    0.000043,    -0.317859,    1.095004,    -1.873990,
    0.428183,    0.895638,    0.730957,    0.577857,    0.040314,    0.677089,    0.568900,    -0.255645,
    -0.377469,    -0.295887,    -1.475135,    -0.234004,    0.118445,    0.314809,    1.443508,    -0.350975,
    0.623234,    0.799049,    0.940890,    -0.992092,    0.212035,    0.237882,    -1.007763,    -0.742045,
    1.082295,    -0.131500,    0.389880,    0.087987,    -0.635465,    -0.559573,    0.443653,    -0.949904,
    0.781182,    0.568961,    -0.821714,    -0.265607,    -1.187777,    -2.202321,    0.986337,    -0.518635,
    0.327368,    0.234057,    0.021466,    -1.003944,    -0.947146,    -0.374429,    -1.185886,    -1.055903,
    1.472480,    0.055744,    -1.217317,    -0.041227,    -1.128344,    -1.349278,    -0.261102,    0.953465,
    0.128644,    0.656468,    -1.167819,    -0.460605,    -0.262440,    -1.213152,    -1.319437,    0.931218,
    0.011245,    -0.645146,    0.805729,    0.231626,    -0.989760,    1.339586,    0.289502,    1.478917,
    1.138028,    -0.684139,    -1.291936,    -0.072926,    -0.330599,    -0.843628,    0.497770,    1.488490,
    -0.546476,    -0.846758,    -0.246337,    0.663024,    -0.854197,    -1.201315,    -0.119869,    -0.065294,
    0.485296,    -0.595491,    -0.149668,    -0.434752,    -0.079330,    1.535152,    -0.606483,    -1.347363,
    0.469383,    -0.903567,    0.035880,    -0.627531,    0.535398,    0.552884,    -0.203690,    -2.054325,
    0.132561,    1.592941,    1.018412,    -1.580402,    -0.078662,    -0.681657,    -1.024553,    -1.234353,
    0.288807,    -0.429303,    0.055801,    -0.367874,    -0.464973,    0.370961,    0.728283,    2.112160,
    -1.357298,    -1.022610,    1.037834,    -0.389800,    -1.381266,    0.315543,    1.553243,    0.707894,
    1.957385,    0.504542,    1.864529,    -0.339812,    -1.139779,    -0.211123,    1.190245,    -1.116209,
    0.635274,    -0.601412,    0.551185,    -1.099840,    0.085991,    -2.004563,    -0.493088,    0.462048,
    -0.321005,    1.236556,    -0.631280,    -2.325211,    -1.231637,    1.055648,    -0.113224,    0.379224,
    0.944200,    -2.120427,    -0.644679,    -0.704302,    -1.018137,    -0.182082,    1.521013,    -0.038439,
    1.227448,    -0.696205,    0.007524,    -0.782893,    0.586939,    -0.251207,    0.480136,    0.668155,
    -0.078321,    0.889173,    2.309287,    0.524639,    -0.011787,    0.913141,    0.055941,    -1.107070,
    0.485498,    -0.005005,    -0.276218,    1.276452,    1.863401,    -0.522559,    0.103424,    -0.807649,
    0.680439,    -2.364590,    0.990115,    0.218899,    0.261662,    1.213444,    -0.274667,    -0.133134,
    -1.270500,    -1.663606,    -0.703554,    0.280880,    -0.541209,    -1.333531,    1.072686,    -0.712085,
    -0.011286,    -0.000817,    -0.249436,    0.396575,    -0.264013,    -1.664011,    -1.028975,    0.243095,
    -1.256590,    -0.347183,    -0.941372,    -1.174560,    -1.021142,    -0.401667,    0.173666,    -0.116118,
    1.064119,    -0.245386,    -1.517539,    0.009734,    0.071373,    0.316536,    0.499826,    1.278084,
    -0.547816,    0.260808,    -0.013177,    -0.580264,    2.136308,    -0.257617,    -1.409528,    1.770101,
    0.325546,    -1.119040,    0.620350,    1.269782,    -0.896043,    0.135175,    -0.139040,    -1.163395,
    1.183720,    -0.015430,    0.536219,    -0.716429,    -0.655559,    0.314363,    0.106814,    1.848216,
    -0.275106,    2.212554,    1.508526,    -1.945079,    -1.680543,    -0.573534,    -0.185817,    0.008934

};


/* ----------------------------------------------------------------------
** Defines each of the tests performed
** ------------------------------------------------------------------- */
typedef struct
{
    ne10_uint32_t blockSize;
    ne10_uint32_t numTaps;
    ne10_uint32_t numFrames;
    ne10_float32_t *kCoeffsF32;
    ne10_float32_t *vCoeffsF32;
    ne10_float32_t *inputF32;
} test_config;

/* All Test configurations, 100% Code Coverage */
#if defined (SMOKE_TEST)||(REGRESSION_TEST)
static test_config CONFIG[] = {{32, 1, 10, &testkCoeffs1[0], &testvCoeffs1[0], &testInput_f32[0]},
    {32, 9, 10, &testkCoeffs9[0], &testvCoeffs9[0], &testInput_f32[0]},
    {2, 9, 160, &testkCoeffs9[0], &testvCoeffs9[0], &testInput_f32[0]},
    {32, 10, 10, &testkCoeffs10[0], &testvCoeffs10[0], &testInput_f32[0]},
    {5, 2, 64, &testkCoeffs2[0], &testvCoeffs2[0], &testInput_f32[0]},
    {0, 8, 10, &testkCoeffs8[0], &testvCoeffs8[0], &testInput_f32[0]},
    {0, 0, 10, &testkCoeffs8[0], &testvCoeffs8[0], &testInput_f32[0]},
    {32, 8, 10, &testkCoeffs8[0], &testvCoeffs8[0], &testInput_f32[0]},
    {32, 33, 10, &testkCoeffs33[0], &testvCoeffs33[0], &testInput_f32[0]}
};
#define NUM_TESTS (sizeof(CONFIG) / sizeof(CONFIG[0]) )
#endif
#ifdef PERFORMANCE_TEST
static test_config CONFIG_PERF[] =
{
    {2, 9, 160, &testkCoeffs9[0], &testvCoeffs9[0], &testInput_f32[0]},
    {32, 9, 10, &testkCoeffs9[0], &testvCoeffs9[0], &testInput_f32[0]},
    {32, 33, 10, &testkCoeffs33[0], &testvCoeffs33[0], &testInput_f32[0]}
};
#define NUM_PERF_TESTS (sizeof(CONFIG_PERF) / sizeof(CONFIG_PERF[0]) )
#endif

//input and output
static ne10_float32_t * guarded_in_c = NULL;
static ne10_float32_t * guarded_in_neon = NULL;
static ne10_float32_t * in_c = NULL;
static ne10_float32_t * in_neon = NULL;

static ne10_float32_t * guarded_out_c = NULL;
static ne10_float32_t * guarded_out_neon = NULL;
static ne10_float32_t * out_c = NULL;
static ne10_float32_t * out_neon = NULL;

static ne10_float32_t * guarded_iir_state_c = NULL;
static ne10_float32_t * guarded_iir_state_neon = NULL;
static ne10_float32_t * iir_state_c = NULL;
static ne10_float32_t * iir_state_neon = NULL;

#if defined (SMOKE_TEST)||(REGRESSION_TEST)
static ne10_float32_t snr = 0.0f;
#endif
#ifdef PERFORMANCE_TEST
static ne10_int64_t time_c = 0;
static ne10_int64_t time_neon = 0;
static ne10_float32_t time_speedup = 0.0f;
static ne10_float32_t time_savings = 0.0f;
#endif

void test_iir_lattice_case0()
{
    ne10_iir_lattice_instance_f32_t SC, SN;

    ne10_uint16_t loop = 0;
    ne10_uint16_t block = 0;
    ne10_uint16_t i = 0;

    test_config *config;

    fprintf (stdout, "----------%30s start\n", __FUNCTION__);

    /* init input memory */
    NE10_SRC_ALLOC (in_c, guarded_in_c, TEST_LENGTH_SAMPLES); // 16 extra bytes at the begining and 16 extra bytes at the end
    NE10_SRC_ALLOC (in_neon, guarded_in_neon, TEST_LENGTH_SAMPLES); // 16 extra bytes at the begining and 16 extra bytes at the end

    /* init dst memory */
    NE10_DST_ALLOC (out_c, guarded_out_c, TEST_LENGTH_SAMPLES);
    NE10_DST_ALLOC (out_neon, guarded_out_neon, TEST_LENGTH_SAMPLES);

    /* init state memory */
    NE10_DST_ALLOC (iir_state_c, guarded_iir_state_c, MAX_NUMTAPS + MAX_BLOCKSIZE);
    NE10_DST_ALLOC (iir_state_neon, guarded_iir_state_neon, MAX_NUMTAPS + MAX_BLOCKSIZE);

#ifdef ENABLE_NE10_IIR_LATTICE_FLOAT_NEON
#if defined (SMOKE_TEST)||(REGRESSION_TEST)
    ne10_uint16_t pos = 0;
    for (loop = 0; loop < NUM_TESTS; loop++)
    {
        config = &CONFIG[loop];

        /* Initialize the CFFT/CIFFT module */
        ne10_iir_lattice_init_float (&SC, config->numTaps, config->kCoeffsF32, config->vCoeffsF32, iir_state_c, config->blockSize);
        ne10_iir_lattice_init_float (&SN, config->numTaps, config->kCoeffsF32, config->vCoeffsF32, iir_state_neon, config->blockSize);

        /* copy input to input buffer */
        for (i = 0; i < TEST_LENGTH_SAMPLES; i++)
        {
            in_c[i] = testInput_f32[i];
            in_neon[i] = testInput_f32[i];
            out_c[i] = 0;
            out_neon[i] = 0;
        }

        GUARD_ARRAY (out_c, TEST_LENGTH_SAMPLES);
        GUARD_ARRAY (out_neon, TEST_LENGTH_SAMPLES);

        for (block = 0; block < config->numFrames; block++)
        {
            ne10_iir_lattice_float_c (&SC, in_c + (block * config->blockSize), out_c + (block * config->blockSize), config->blockSize);
        }
        for (block = 0; block < config->numFrames; block++)
        {
            ne10_iir_lattice_float_neon (&SN, in_neon + (block * config->blockSize), out_neon + (block * config->blockSize), config->blockSize);
        }

        assert_true (CHECK_ARRAY_GUARD (out_c, TEST_LENGTH_SAMPLES));
        assert_true (CHECK_ARRAY_GUARD (out_neon, TEST_LENGTH_SAMPLES));

        //conformance test 1: compare snr
        snr = CAL_SNR_FLOAT32 (out_c, out_neon, TEST_LENGTH_SAMPLES);
#if defined (DEBUG_TRACE)
        printf ("--------------------config %d\n", loop);
        printf ("snr %f\n", snr);
#endif
        assert_false ( (snr < SNR_THRESHOLD));

        //conformance test 2: compare output of C and neon
        for (pos = 0; pos < TEST_LENGTH_SAMPLES; pos++)
        {
#if defined (DEBUG_TRACE)
            printf ("pos %d \n", pos);
            printf ("c %f (0x%04X) neon %f (0x%04X)\n", out_c[pos], * (ne10_uint32_t*) &out_c[pos], out_neon[pos], * (ne10_uint32_t*) &out_neon[pos]);
#endif
            assert_float_vec_equal (&out_c[pos], &out_neon[pos], ERROR_MARGIN_LARGE, 1);
        }

    }
#endif
#endif // ENABLE_NE10_IIR_LATTICE_FLOAT_NEON

#ifdef PERFORMANCE_TEST
    ne10_uint16_t k;
    fprintf (stdout, "%25s%20s%20s%20s%20s\n", "IIR Length&Taps", "C Time (micro-s)", "NEON Time (micro-s)", "Time Savings", "Performance Ratio");
    for (loop = 0; loop < NUM_PERF_TESTS; loop++)
    {
        config = &CONFIG_PERF[loop];

        /* Initialize the CFFT/CIFFT module */
        ne10_iir_lattice_init_float (&SC, config->numTaps, config->kCoeffsF32, config->vCoeffsF32, iir_state_c, config->blockSize);
        ne10_iir_lattice_init_float (&SN, config->numTaps, config->kCoeffsF32, config->vCoeffsF32, iir_state_neon, config->blockSize);

        /* copy input to input buffer */
        for (i = 0; i < TEST_LENGTH_SAMPLES; i++)
        {
            in_c[i] = testInput_f32[i];
            in_neon[i] = testInput_f32[i];
        }

        GET_TIME
        (
            time_c,
        {
            for (k = 0; k < TEST_COUNT; k++)
            {
                for (block = 0; block < config->numFrames; block++)
                {
                    ne10_iir_lattice_float_c (&SC, in_c + (block * config->blockSize), out_c + (block * config->blockSize), config->blockSize);
                }
            }
        }
        );

#ifdef ENABLE_NE10_IIR_LATTICE_FLOAT_NEON
        GET_TIME
        (
            time_neon,
        {
            for (k = 0; k < TEST_COUNT; k++)
            {
                for (block = 0; block < config->numFrames; block++)
                {
                    ne10_iir_lattice_float_neon (&SN, in_neon + (block * config->blockSize), out_neon + (block * config->blockSize), config->blockSize);
                }
            }
        }
        );
#endif // ENABLE_NE10_IIR_LATTICE_FLOAT_NEON

        time_speedup = (ne10_float32_t) time_c / time_neon;
        time_savings = ( ( (ne10_float32_t) (time_c - time_neon)) / time_c) * 100;
        ne10_log (__FUNCTION__, "%20d,%4d%20lld%20lld%19.2f%%%18.2f:1\n", config->numTaps, time_c, time_neon, time_savings, time_speedup);

    }
#endif

    free (guarded_in_c);
    free (guarded_in_neon);
    free (guarded_out_c);
    free (guarded_out_neon);
    free (guarded_iir_state_c);
    free (guarded_iir_state_neon);
    fprintf (stdout, "----------%30s end\n", __FUNCTION__);
}

void test_iir_lattice()
{
    test_iir_lattice_case0();
}

static void my_test_setup (void)
{
    ne10_log_buffer_ptr = ne10_log_buffer;
}

void test_fixture_iir_lattice (void)
{
    test_fixture_start();               // starts a fixture

    fixture_setup (my_test_setup);

    run_test (test_iir_lattice);       // run tests

    test_fixture_end();                 // ends a fixture
}
