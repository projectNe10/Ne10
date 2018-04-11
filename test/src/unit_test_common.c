/*
 *  Copyright 2012-16 ARM Limited and Contributors.
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
 * NE10 Library : test/src/unit_test_common.c
 */

#include<time.h>
#include<unistd.h>
#include "unit_test_common.h"

void FILL_FLOAT_ARRAY (ne10_float32_t *arr, ne10_uint32_t count)
{
    ne10_uint32_t i = 0;

    sleep (1);

    NE10_float_rng_init (time (NULL));

    for (i = 0; i < count; i++)
    {
        arr[i] = NE10_float_rng_next();
    }
}

void FILL_FLOAT_ARRAY_LIMIT (ne10_float32_t *arr, ne10_uint32_t count)
{
    ne10_uint32_t i = 0;

    sleep (1);

    NE10_float_rng_limit_init (time (NULL));

    for (i = 0; i < count; i++)
    {
        arr[ i ] = NE10_float_rng_limit_next();
    }
}

void FILL_FLOAT_ARRAY_LIMIT_GT1 (ne10_float32_t *arr, ne10_uint32_t count)
{
    ne10_uint32_t i = 0;

    sleep (1);

    NE10_float_rng_limit_gt1_init (time (NULL));

    for (i = 0; i < count; i++)
    {
        arr[ i ] = NE10_float_rng_limit_gt1_next();
    }
}

// this function checks whether the difference between two ne10_float32_t values is within the acceptable error range
ne10_int32_t EQUALS_FLOAT (ne10_float32_t fa, ne10_float32_t fb , ne10_uint32_t err)
{   ne10_float32_t tolerance = (ne10_float32_t)err / 200000;
    if (fabs(fa-fb) <= tolerance)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

ne10_float32_t ARRAY_GUARD_SIG[ARRAY_GUARD_LEN] = { 10.0f, 20.0f, 30.0f, 40.0f };
// this function adds a ARRAY_GUARD_LEN signature to the begining and the end of an array, minimum acceptable size for the array is 2*ARRAY_GUARD_LEN.
ne10_int32_t GUARD_ARRAY (ne10_float32_t* array, ne10_uint32_t array_length)
{
    ne10_float32_t* the_array = array - ARRAY_GUARD_LEN;
    memcpy (the_array, ARRAY_GUARD_SIG, sizeof (ARRAY_GUARD_SIG));
    the_array = array + array_length;
    memcpy (the_array, ARRAY_GUARD_SIG, sizeof (ARRAY_GUARD_SIG));
    return 1;
}

// this function returns TRUE if the signature matches the ARRAY_GUARD_SIGguard and returns FALSE otherwise
ne10_int32_t CHECK_ARRAY_GUARD (ne10_float32_t* array, ne10_uint32_t array_length)
{
    ne10_float32_t* the_array = array - ARRAY_GUARD_LEN;
    ne10_int32_t i;
    for (i = 0; i < ARRAY_GUARD_LEN; i++)
    {
        if (! EQUALS_FLOAT (the_array[i], ARRAY_GUARD_SIG[i], ERROR_MARGIN_SMALL))
        {
            fprintf (stderr, " ERROR: prefix array guard signature is wrong. \n");
            return 0; // Match not found, return FALSE
        }
    }

    the_array = array + array_length;
    for (i = 0; i < ARRAY_GUARD_LEN; i++)
    {
        if (! EQUALS_FLOAT (the_array[i], ARRAY_GUARD_SIG[i], ERROR_MARGIN_SMALL))
        {
            fprintf (stderr, " ERROR: suffix array guard signature is wrong. \n");
            return 0; // Match not found, return FALSE
        }
    }

    return 1;
}


ne10_uint8_t ARRAY_GUARD_SIG_UINT8[ARRAY_GUARD_LEN] = { 0x12, 0x34, 0x56, 0x78 };
// this function adds a ARRAY_GUARD_LEN signature to the begining and the end of an array, minimum acceptable size for the array is 2*ARRAY_GUARD_LEN.
ne10_int32_t GUARD_ARRAY_UINT8 (ne10_uint8_t* array, ne10_uint32_t array_length)
{
    ne10_uint8_t* the_array = array - ARRAY_GUARD_LEN;
    memcpy (the_array, ARRAY_GUARD_SIG_UINT8, sizeof (ARRAY_GUARD_SIG_UINT8));
    the_array = array + array_length;
    memcpy (the_array, ARRAY_GUARD_SIG_UINT8, sizeof (ARRAY_GUARD_SIG_UINT8));
    return 1;
}

// this function returns TRUE if the signature matches the ARRAY_GUARD_SIGguard and returns FALSE otherwise
ne10_int32_t CHECK_ARRAY_GUARD_UINT8 (ne10_uint8_t* array, ne10_uint32_t array_length)
{
    ne10_uint8_t* the_array = array - ARRAY_GUARD_LEN;
    ne10_int32_t i;
    for (i = 0; i < ARRAY_GUARD_LEN; i++)
    {
        if (the_array[i] != ARRAY_GUARD_SIG_UINT8[i])
        {
            fprintf (stderr, " ERROR: prefix array guard signature is wrong. \n");
            return 0; // Match not found, return FALSE
        }
    }

    the_array = array + array_length;
    for (i = 0; i < ARRAY_GUARD_LEN; i++)
    {
        if (the_array[i] != ARRAY_GUARD_SIG_UINT8[i])
        {
            fprintf (stderr, " ERROR: suffix array guard signature is wrong. \n");
            return 0; // Match not found, return FALSE
        }
    }

    return 1;
}

/**
 * @brief  Caluclation of SNR.
 * @param  ne10_float32_t*  Pointer to the reference buffer
 * @param  ne10_float32_t*  Pointer to the test buffer
 * @param  ne10_uint32_t    total number of samples
 * @return ne10_float32_t   SNR
 *
 * The function Caluclates signal to noise ratio for the reference output
 * and test output
 */

ne10_float32_t CAL_SNR_FLOAT32 (ne10_float32_t *pRef, ne10_float32_t *pTest, ne10_uint32_t buffSize)
{
    ne10_float32_t EnergySignal = 0.0, EnergyError = 0.0;
    ne10_uint32_t i;
    ne10_float32_t SNR;

    for (i = 0; i < buffSize; i++)
    {
        EnergySignal += pRef[i] * pRef[i];
        EnergyError += (pRef[i] - pTest[i]) * (pRef[i] - pTest[i]);
    }
    SNR = 10 * log10 (EnergySignal / EnergyError);
    return (SNR);

}

/**
 * @brief  Caluclation of PSNR.
 * @param  ne10_uint8_t*  Pointer to the reference buffer
 * @param  ne10_uint8_t*  Pointer to the test buffer
 * @param  ne10_uint32_t    total number of samples
 * @return ne10_float32_t   PSNR
 *
 * The function Caluclates peak signal to noise ratio for the reference output
 * and test output
 */

ne10_float32_t CAL_PSNR_UINT8 (ne10_uint8_t *pRef, ne10_uint8_t *pTest, ne10_uint32_t buffSize)
{
    ne10_float64_t mse = 0.0, max = 255.0;
    ne10_uint32_t i;
    ne10_float32_t PSNR;

    for (i = 0; i < buffSize; i++)
    {
        mse += (pRef[i] - pTest[i]) * (pRef[i] - pTest[i]);
    }
    mse /= buffSize;
    PSNR = 10 * log10 (max*max / mse);
    return (PSNR);

}
char ne10_log_buffer[5000];
char *ne10_log_buffer_ptr;

void ne10_log(const char *func_name,
              const char *format_str,
              ne10_int32_t n,
              ne10_int32_t time_c,
              ne10_int32_t time_neon,
              ne10_float32_t time_savings,
              ne10_float32_t time_speedup)
{
    int byte_count = 0;
    byte_count = sprintf(ne10_log_buffer_ptr,
                         "{ \"name\" : \"%s %d\", \"time_c\" : %d, "
                         "\"time_neon\" : %d },",
                         func_name, n, time_c, time_neon);
    ne10_log_buffer_ptr += byte_count;

    /* print the result, which is needed by command line performance test. */
    fprintf (stdout,
             "%25d%20d%20d%19.2f%%%18.2f:1\n",
             n,
             time_c,
             time_neon,
             time_savings,
             time_speedup);
}

/* this function should handle all performance result output. ne10_log
 * should be replaced by this function.
 * info parameter contains the misc information, which is in below format:
 * key1:value1\nkey2:value2\nkey3:value3
 */
void ne10_performance_print(ne10_print_target_t target,
                            long int neon_ticks,
                            long int c_ticks,
                            char *info)
{
    double f = (double)c_ticks / neon_ticks;
    switch (target) {
    case UBUNTU_COMMAND_LINE:
        printf("\n%s\nneon(ms): %.2f c(ms): %.2f(ms) speedup: %.2f",
               info,
               neon_ticks / 1000.0,
               c_ticks / 1000.0,
               f);
        break;
    case ANDROID_DEMO:
        break;
    case IOS_DEMO:
        break;
    }
}

void diff(const ne10_uint8_t *mat1,
          const ne10_uint8_t *mat2,
          ne10_int32_t  *dst,
          ne10_uint32_t dst_stride,
          ne10_uint32_t width,
          ne10_uint32_t height,
          ne10_uint32_t src_stride,
          ne10_uint32_t channel)
{
    assert(mat1 != 0 && mat2 != 0 && dst != 0);

    int i, j, k;

    for(k = 0; k < channel; k++)
    {
        for(j = 0; j < height; j++)
        {
            const ne10_uint8_t *row1 = mat1 + j * src_stride;
            const ne10_uint8_t *row2 = mat2 + j * src_stride;
            ne10_int32_t *row_dst = dst + j * dst_stride / sizeof(ne10_int32_t);
            for(i = 0; i < width; i++)
            {
                *(row_dst + i * channel + k) =
                    (ne10_int32_t) (*(row1 + i * channel + k)) -
                    *(row2 + i * channel + k);
            }
        }
    }
}

/* check how many point is not in [-2, 2] */
int diff_count(const ne10_int32_t *mat,
               ne10_int32_t width,
               ne10_int32_t height,
               ne10_int32_t stride,
               ne10_int32_t channel)
{
    assert(mat != 0);

    int i, j, k;
    int count = 0;
    for(j = 0; j < height; j++)
    {
        const ne10_int32_t *row = mat + j * stride / sizeof(ne10_int32_t);
        for(i = 0; i < width; i++)
        {
            for(k = 0; k < channel; k++)
            {
                const ne10_int32_t val = *(row + i * channel + k);
                if (val != -1 && val != 0 && val != -2 && val != 1 && val != 2)
                {
                    ++count;
                }
            }
        }
    }

    return count;
}

void progress_bar(float progress)
{
    assert(progress <= 1);

    static float progress_prev = 0;
    if (progress != progress_prev)
    {
        int bar_width = 70;

        printf("[");
        int pos = bar_width * progress;
        int i;
        for (i = 0; i < bar_width; ++i) {
            if (i < pos)
                printf("=");
            else if (i == pos)
                printf(">");
            else
                printf(" ");
        }
        printf("] %d%%\r", (int)(progress * 100.0));
        fflush(stdout);
        progress_prev = progress;
    }
}
