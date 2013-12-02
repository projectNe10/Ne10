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

/*
 * NE10 Library : test_suite_boxfilter.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "NE10_imgproc.h"
#include "seatest.h"
#include "unit_test_common.h"

#define BASIC_KERNEL_SIZE 5
#define KERNEL_COUNT BASIC_KERNEL_SIZE * BASIC_KERNEL_SIZE

ne10_float32_t cal_psnr_uint8_rgba (const ne10_uint8_t *pRef,
                                    const ne10_uint8_t *pTest,
                                    const ne10_uint32_t buffSize)
{
    ne10_float64_t mse = 0.0, max = 255.0;
    ne10_uint32_t i;
    ne10_float32_t psnr_value;

    for (i = 0; i < buffSize; i++)
    {
        mse += (pRef[i] - pTest[i]) * (pRef[i] - pTest[i]);
    }
    mse = mse / buffSize / 4;
    psnr_value = 10 * log10 (max * max / mse);
    return psnr_value;
}

int rand_range (int min, int max)
{
    int diff = max - min;
    return (int) ( ( (double) (diff + 1) / RAND_MAX) * rand() + min);
}

int valid_kernels (ne10_size_t *kernels, int size)
{
    int i;
    for (i = 0; i < size; i++)
    {
        if (kernels[i].x < 1 || kernels[i].y < 1)
            return NE10_ERR;
    }
    return NE10_OK;
}

/*
 * this function check whether there is big difference between image1
 * and image2. Here we employ 2 kinds of check: diff() and
 * cal_psnr_uint8_rgba_color()
 */
int valid_result (const ne10_uint8_t *image1,
                  const ne10_uint8_t *image2,
                  ne10_size_t src_sz,
                  ne10_int32_t src_stride,
                  ne10_int32_t channel)
{
    assert ((image1 != 0) && (image2 != 0));
    assert ((src_sz.x != 0) && (src_sz.y != 0)
             && (src_stride != 0) && (channel != 0));

    ne10_int32_t *diff_mat = (ne10_int32_t *) malloc (sizeof (ne10_int32_t)
                             * channel
                             * src_sz.x
                             * src_sz.y);
    ne10_int32_t diff_mat_stride = sizeof (ne10_int32_t) * channel * src_sz.x;

    if (diff_mat == 0)
    {
        printf ("**ERROR**: allocating %d bytes memory for kernels fails!",
                sizeof (ne10_int32_t)
                * src_sz.x
                * src_sz.y
                * channel);
        return NE10_ERR;
    }

    diff (image1,
          image2,
          diff_mat,
          diff_mat_stride,
          src_sz.x,
          src_sz.y,
          src_stride,
          4);

    ne10_int32_t diff_nu = diff_count ( (const ne10_int32_t *) diff_mat,
                                        src_sz.x,
                                        src_sz.y,
                                        diff_mat_stride,
                                        4);
    free (diff_mat);

    ne10_float32_t psnr_value = cal_psnr_uint8_rgba (image1,
                                image2,
                                src_sz.x
                                * src_sz.y
                                * channel);
    if (diff_nu != 0 && psnr_value < PSNR_THRESHOLD)
    {
        printf ("\ndifferent point is:%d\t PSNR value is:%f\n",
                diff_nu, psnr_value);
        return NE10_ERR;
    }
    else
    {
        return NE10_OK;
    }
}

void boxfilter_get_kernels (size_t max_kernel_length,
                            ne10_size_t **kernels_ptr,
                            int *size)
{

    if (max_kernel_length > BASIC_KERNEL_SIZE)
    {
        *size = KERNEL_COUNT + 3;
    }
    else if (max_kernel_length < BASIC_KERNEL_SIZE)
    {
        *size = max_kernel_length * max_kernel_length;
    }
    else
    {
        *size = KERNEL_COUNT;
    }

    *kernels_ptr = (ne10_size_t *) malloc (sizeof (ne10_size_t) * (*size));
    if (*kernels_ptr == 0)
    {
        printf ("**ERROR**: allocating %d bytes memory for kernels fails!\n",
                sizeof (ne10_size_t) * (*size));
    }

    int x, y, first_part_size;

    if (max_kernel_length < BASIC_KERNEL_SIZE)
    {
        first_part_size = max_kernel_length;
    }
    else
    {
        first_part_size = BASIC_KERNEL_SIZE;
    }

    for (x = 0; x < first_part_size; x++)
    {
        for (y = 0; y < first_part_size; y++)
        {
            (*kernels_ptr) [x * first_part_size + y].x = x + 1;
            (*kernels_ptr) [x * first_part_size + y].y = y + 1;
        }
    }
    /* add:
     * max_kernel_length x 1
     * 1 x max_kernel_length
     * max_kernel_length x max_kernel_length
     * to kernels.
     */
    if (max_kernel_length > BASIC_KERNEL_SIZE)
    {
        (*kernels_ptr) [*size - 3].x = max_kernel_length;
        (*kernels_ptr) [*size - 3].y = 1;
        (*kernels_ptr) [*size - 2].x = 1;
        (*kernels_ptr) [*size - 2].y = max_kernel_length;
        (*kernels_ptr) [*size - 1].x = max_kernel_length;
        (*kernels_ptr) [*size - 1].y = max_kernel_length;
    }

    assert (valid_kernels (*kernels_ptr, *size) == NE10_OK);
}

void create_rgba8888_image (ne10_uint8_t **img, ne10_size_t src_sz)
{
    assert ( (src_sz.x != 0) || (src_sz.y != 0));

    int size = sizeof (ne10_uint8_t) * src_sz.x * src_sz.y * 4;

    *img = (ne10_uint8_t *) NE10_MALLOC (sizeof (ne10_uint8_t) *
                                         src_sz.x *
                                         src_sz.y * 4);
    int i;
    for (i = 0; i < size; i++)
    {
        * (*img + i) = rand_range (0, 255);
    }

    assert (*img != NULL);
}

int boxfilter_conformance_test (ne10_size_t src_sz)
{
    assert ( (src_sz.x != 0) || (src_sz.y != 0));

    printf ("\ntest boxfilter on image with size:%d x %d:\n",
            src_sz.x, src_sz.y);

    int max_kernel_length = src_sz.x < src_sz.y ?
                            src_sz.x : src_sz.y;
    max_kernel_length = max_kernel_length < ( (1 << 7) - 1) ?
                        max_kernel_length : ( (1 << 7) - 1);

    ne10_size_t *kernels;
    int kernels_size;
    boxfilter_get_kernels (max_kernel_length, &kernels, &kernels_size);

    ne10_uint8_t *src, *neon_dst, *c_dst;
    create_rgba8888_image (&src, src_sz);
    create_rgba8888_image (&neon_dst, src_sz);
    create_rgba8888_image (&c_dst, src_sz);
    ne10_int32_t stride = src_sz.x * 4 * sizeof (ne10_uint8_t);

    int i;
    for (i = 0; i < kernels_size; i++)
    {
        printf ("test kernel size(%d x %d):",
                kernels[i].x, kernels[i].y);
        //use ne10 neon version
        ne10_img_boxfilter_rgba8888_neon (src,
                                          neon_dst,
                                          src_sz,
                                          stride,
                                          stride,
                                          kernels[i]);
        //use ne10 c version
        ne10_img_boxfilter_rgba8888_c (src,
                                       c_dst,
                                       src_sz,
                                       stride,
                                       stride,
                                       kernels[i]);
        assert_true (valid_result (c_dst,
                                   neon_dst,
                                   src_sz,
                                   stride,
                                   4) == NE10_OK);
        printf (" OK.\n");
    }

    free (kernels);
    free (src);
    free (c_dst);
    free (neon_dst);
    return NE10_OK;
}

void boxfilter_performance_test (ne10_size_t img_size,
                                 ne10_size_t kernel_size,
                                 long int *neon_ticks,
                                 long int *c_ticks)
{
    int run_loop = 10;
    int i;
    ne10_uint8_t *src, *neon_dst, *c_dst;
    create_rgba8888_image (&src, img_size);
    create_rgba8888_image (&neon_dst, img_size);
    create_rgba8888_image (&c_dst, img_size);
    ne10_int32_t stride = img_size.x * 4 * sizeof (ne10_uint8_t);

    long int ticks;
    /* boxfilter c version, run multiple times to get average time */
    for (i = 0; i < run_loop; i++)
    {
        GET_TIME (ticks,
                  ne10_img_boxfilter_rgba8888_c (src,
                          c_dst,
                          img_size,
                          stride,
                          stride,
                          kernel_size););
        ticks += ticks;
    }
    *c_ticks = ticks / run_loop;

    /* boxfilter c version, run multiple times to get average time */
    for (i = 0; i < run_loop; i++)
    {
        GET_TIME (ticks,
                  ne10_img_boxfilter_rgba8888_neon (src,
                          c_dst,
                          img_size,
                          stride,
                          stride,
                          kernel_size););
        ticks += ticks;
    }
    *neon_ticks = ticks / run_loop;
}

void test_boxfilter_performance_case()
{
    ne10_size_t img_sizes[] = {{240, 320}, {480, 320}, {960, 1280},
        {1200, 1600}, {2000, 2000}
    };
    ne10_size_t kernel_sizes[] = {{3, 3}, {5, 5}, {7, 7}, {9, 9}};

    int i, j, n_img, n_kernel;
    n_img = sizeof (img_sizes) / sizeof (img_sizes[0]);
    n_kernel = sizeof (kernel_sizes) / sizeof (kernel_sizes[0]);
    long int neon_ticks, c_ticks;

    char info[100];
    for (i = 0; i < n_img; i++)
    {
        for (j = 0; j < n_kernel; j++)
        {
            boxfilter_performance_test (img_sizes[i],
                                        kernel_sizes[j],
                                        &neon_ticks,
                                        &c_ticks);
            sprintf (info,
                     "name:box filter\n"
                     "image size:%dx%d\n"
                     "kernel size:%dx%d",
                     img_sizes[i].x, img_sizes[i].y,
                     kernel_sizes[j].x, kernel_sizes[j].y);

            ne10_performance_print (UBUNTU_COMMAND_LINE,
                                    neon_ticks,
                                    c_ticks,
                                    info);
        }
    }
}

void test_boxfilter_smoke_case()
{
    ne10_size_t img_sizes[] = {{1, 1}, {2, 2}, {8, 3}, {10, 19},
        {240, 320}
    };
    int n = sizeof (img_sizes) / sizeof (img_sizes[0]);
    int i;
    for (i = 0; i < n; i++)
    {
        boxfilter_conformance_test (img_sizes[i]);
        //progress_bar((float)(i + 1) / n);
    }
}

void test_boxfilter_regression_case()
{
    ne10_size_t img_sizes[] = {{1, 1}, {2, 2}, {8, 3}, {10, 19},
        {239, 319}, {240, 320}, {480, 640},
        {969, 1280}, {1200, 1600}
    };
    int n = sizeof (img_sizes) / sizeof (img_sizes[0]);
    int i;
    for (i = 0; i < n; i++)
    {
        boxfilter_conformance_test (img_sizes[i]);
    }
}

void test_boxfilter()
{
#if defined (SMOKE_TEST)
    test_boxfilter_smoke_case();
#endif

#if defined (REGRESSION_TEST)
    test_boxfilter_regression_case();
#endif

#if defined PERFORMANCE_TEST
    test_boxfilter_performance_case();
#endif
}

static void my_test_setup (void)
{
    ne10_log_buffer_ptr = ne10_log_buffer;
}

void test_fixture_boxfilter (void)
{
    test_fixture_start();

    fixture_setup (my_test_setup);

    run_test (test_boxfilter);

    test_fixture_end();
}
