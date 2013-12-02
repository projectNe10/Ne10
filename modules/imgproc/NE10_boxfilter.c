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
/* license of OpenCV */
/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                           License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
// Copyright (C) 2009, Willow Garage Inc., all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

/*
 * NE10 Library : imgproc/NE10_boxfilter.c
 */

#include "NE10.h"
#include <stdlib.h>

/* RGBA CHANNEL number is 4 */
#define RGBA_CH 4

/*
 * compute destination image's row border.
 */
void ne10_img_boxfilter_row_border (const ne10_uint8_t* src,
                                    ne10_uint8_t* dst,
                                    ne10_size_t src_sz,
                                    ne10_int32_t src_stride,
                                    ne10_int32_t dst_stride,
                                    ne10_size_t kernel,
                                    ne10_point_t anchor,
                                    ne10_int32_t *border_l_p,
                                    ne10_int32_t *border_r_p)
{
    assert (kernel.x <= src_sz.x);
    assert (src != dst);

    *border_l_p = anchor.x;
    *border_r_p = kernel.x - (anchor.x + 1);

    ne10_int32_t x, y, k;

    const ne10_uint8_t *src_row;
    ne10_uint8_t *dst_row;

    for (y = 0; y < src_sz.y; y++)
    {
        src_row = src + y * src_stride;
        dst_row = dst + y * dst_stride;
        ne10_float32_t sum[RGBA_CH];

        /* compute left border */
        ne10_int32_t offset = kernel.x - *border_l_p - 1;
        for (k = 0; k < RGBA_CH; k++)
        {
            sum[k] = 0;
            for (x = 0; x < offset; x++)
            {
                sum[k] += * (src_row + x * RGBA_CH + k);
            }
        }

        for (k = 0; k < RGBA_CH; k++)
        {
            for (x = 0; x < *border_l_p; x++)
            {
                sum[k] += * (src_row + (offset + x) *
                             RGBA_CH + k);
                * (dst_row + x * RGBA_CH + k) = sum[k] /
                                                kernel.x;
            }
        }

        /* compute right border */
        for (k = 0; k < RGBA_CH; k++)
        {
            sum[k] = 0;
            for (x = 0; x < kernel.x; x++)
            {
                sum[k] += * (src_row + (src_sz.x - kernel.x + x) *
                             RGBA_CH + k);
            }
        }

        for (k = 0; k < RGBA_CH; k++)
        {
            for (x = 0; x < *border_r_p; x++)
            {
                sum[k] -= * (src_row + (src_sz.x - kernel.x + x) *
                             RGBA_CH + k);
                * (dst_row + (src_sz.x - *border_r_p + x) *
                   RGBA_CH + k) = sum[k] / kernel.x;
            }
        }
    }
}

void ne10_img_boxfilter_row_c (const ne10_uint8_t *src,
                               ne10_uint8_t *dst,
                               ne10_size_t src_sz,
                               ne10_int32_t src_stride,
                               ne10_int32_t dst_stride,
                               ne10_size_t kernel,
                               ne10_point_t anchor,
                               ne10_int32_t border_l,
                               ne10_int32_t border_r)
{
    assert (src != dst);
    assert ( (kernel.x > 0) && (kernel.x < (1 << 16)));
    assert (kernel.x <= src_sz.x);

    ne10_int32_t x, y, k;

    for (y = 0; y < src_sz.y; y++)
    {
        const ne10_uint8_t *src_row = src + y * src_stride;
        ne10_uint8_t *dst_row = dst + y * dst_stride;
        ne10_float32_t sum[RGBA_CH];

        for (k = 0; k < RGBA_CH; k++)
        {
            sum[k] = 0;

            for (x = 0; x < kernel.x; x++)
            {
                sum[k] += * (src_row + x * RGBA_CH + k);
            }

            * (dst_row + border_l * RGBA_CH + k) = sum[k] /
                                                   kernel.x;
        }

        ne10_uint32_t prev = (anchor.x + 1) * RGBA_CH;
        ne10_uint32_t next = (kernel.x - anchor.x - 1) * RGBA_CH;
        const ne10_uint8_t *src_pixel = src_row + (1 + border_l) * RGBA_CH;
        const ne10_uint8_t *src_pixel_end = src_row + (src_sz.x - border_r) *
                                            RGBA_CH;
        ne10_uint8_t *dst_pixel = dst_row + (1 + border_l) * RGBA_CH;

        for (k = 0; src_pixel < src_pixel_end; src_pixel++, dst_pixel++)
        {
            sum[k] += src_pixel[next] - * (src_pixel - prev);
            *dst_pixel = sum[k] / kernel.x;
            k++;
            k &= 3;
        }
    }
}

void ne10_img_boxfilter_col_border (const ne10_uint8_t *src,
                                    ne10_uint8_t *dst,
                                    ne10_size_t src_sz,
                                    ne10_int32_t src_stride,
                                    ne10_int32_t dst_stride,
                                    ne10_size_t kernel,
                                    ne10_point_t anchor,
                                    ne10_int32_t *border_t_p,
                                    ne10_int32_t *border_b_p)
{
    assert (kernel.y <= src_sz.y);
    assert (src != dst);

    *border_t_p = anchor.y;
    *border_b_p = kernel.y - (anchor.y + 1);

    ne10_int32_t x, y, k;
    const ne10_uint8_t *src_col;
    ne10_uint8_t *dst_col;

    for (x = 0; x < src_sz.x; x++)
    {
        src_col = src + x * RGBA_CH;
        dst_col = dst + x * RGBA_CH;
        ne10_float32_t sum[RGBA_CH];

        /* compute top border */
        ne10_int32_t offset = kernel.y - *border_t_p - 1;
        for (k = 0; k < RGBA_CH; k++)
        {
            sum[k] = 0;

            for (y = 0; y < offset; y++)
            {
                sum[k] += * (src_col + y * src_stride + k);
            }
        }

        for (k = 0; k < RGBA_CH; k++)
        {
            for (y = 0; y < *border_t_p; y++)
            {
                sum[k] += * (src_col + (offset + y) *
                             src_stride + k);
                * (dst_col + y * dst_stride + k) = sum[k] /
                                                   kernel.y;
            }
        }

        /* compute the bottom border */
        for (k = 0; k < RGBA_CH; k++)
        {
            sum[k] = 0;
            for (y = 0; y < kernel.y; y++)
            {
                sum[k] += * (src_col + (src_sz.y - kernel.y + y) *
                             src_stride + k);
            }
        }

        for (k = 0; k < RGBA_CH; k++)
        {
            for (y = 0; y < *border_b_p; y++)
            {
                sum[k] -= * (src_col + (src_sz.y - kernel.y + y) *
                             src_stride + k);
                * (dst_col + (src_sz.y - *border_b_p + y) * dst_stride + k) =
                    sum[k] / kernel.y;
            }
        }
    }
}

void ne10_img_boxfilter_col_c (const ne10_uint8_t *src,
                               ne10_uint8_t *dst,
                               ne10_size_t src_sz,
                               ne10_int32_t src_stride,
                               ne10_int32_t dst_stride,
                               ne10_size_t kernel,
                               ne10_point_t anchor,
                               ne10_int32_t border_t,
                               ne10_int32_t border_b)
{
    assert (src != dst);
    assert ( (kernel.y > 0) && (kernel.y < (1 << 16)));
    assert (kernel.y <= src_sz.y);

    ne10_int32_t x, y, k;

    for (x = 0; x < src_sz.x; x++)
    {
        const ne10_uint8_t *src_col = src + x * RGBA_CH;
        ne10_uint8_t *dst_col = dst + x * RGBA_CH;
        ne10_float32_t sum[RGBA_CH];

        for (k = 0; k < RGBA_CH; k++)
        {
            sum[k] = 0;

            for (y = 0; y < kernel.y; y++)
            {
                sum[k] += * (src_col + y * src_stride + k);
            }

            * (dst_col + border_t * dst_stride + k) = sum[k] / kernel.y;
        }

        ne10_uint32_t prev = (anchor.y + 1) * src_stride;
        ne10_uint32_t next = (kernel.y - anchor.y - 1) * src_stride;
        const ne10_uint8_t *src_pixel = src_col + (1 + border_t) * src_stride;
        const ne10_uint8_t *src_end = src_col + (src_sz.y - border_b) *
                                      src_stride;
        ne10_uint8_t *dst_pixel = dst_col + (1 + border_t) * dst_stride;

        while (src_pixel < src_end)
        {
            for (k = 0; k < RGBA_CH; k++)
            {
                sum[k] += src_pixel[next + k] - * (src_pixel - prev + k);
                * (dst_pixel + k) = sum[k] / kernel.y;
            }
            dst_pixel += dst_stride;
            src_pixel += src_stride;
        }
    }
}

/**
 * @addtogroup IMG_BOXFILTER
 * @{
 */

/**
 * @brief box filter
 * @param[out]  *dst                  point to the destination image
 * @param[in]   *src                  point to the source image
 * @param[in]   src_sz              source image size struction
 * @param[in]   src_stride            stride of source image
 * @param[in]   dst_stride            stride of destination image
 * @param[in]   kernel                kernel size of box filter, both of kernel edge
 * supports from 1 to 65535, that is 1x1 1x2 1x3 ... 1x65535 2x1 2x2 2x3 ... 2x65535
 * ... are all supported.
 * @return none.
 * The function implements box filter, destination image's size is smae as
 * source image
 */
void ne10_img_boxfilter_rgba8888_c (const ne10_uint8_t *src,
                                    ne10_uint8_t *dst,
                                    ne10_size_t src_sz,
                                    ne10_int32_t src_stride,
                                    ne10_int32_t dst_stride,
                                    ne10_size_t kernel)
{
    ne10_int32_t border_l, border_r, border_t, border_b;
    ne10_point_t anchor;

    assert (src != 0 && dst != 0);
    assert (src_sz.x > 0 && src_sz.y > 0);
    assert (src_stride > 0 && dst_stride > 0);
    assert (kernel.x > 0 && kernel.x <= src_sz.x
            && kernel.y > 0 && kernel.y <= src_sz.y);

    anchor.x = kernel.x / 2;
    anchor.y = kernel.y / 2;

    ne10_uint8_t *dst_buf = (ne10_uint8_t *) malloc (sizeof (ne10_uint8_t) *
                            src_sz.x *
                            src_sz.y *
                            RGBA_CH);

    if (!dst_buf)
    {
        fprintf (stderr,
                 "ERROR: buffer allocation fails!\nallocation size: %d\n",
                 sizeof (ne10_uint8_t) *
                 src_sz.x *
                 src_sz.y *
                 RGBA_CH);
        return;
    }

    ne10_int32_t dst_buf_stride = src_sz.x * RGBA_CH;

    /* compute the row border of dst image */
    ne10_img_boxfilter_row_border (src,
                                   dst_buf,
                                   src_sz,
                                   src_stride,
                                   dst_buf_stride,
                                   kernel,
                                   anchor,
                                   &border_l,
                                   &border_r);
    /* boxfilter is separable filter, and then can be apply row filter and
     * column filter sequentially. here apply boxfilter's row part to image
     */
    ne10_img_boxfilter_row_c (src,
                              dst_buf,
                              src_sz,
                              src_stride,
                              dst_buf_stride,
                              kernel,
                              anchor,
                              border_l,
                              border_r);

    /* compute the column border of dst image,
     * which is based on previous row filter result.
     */
    ne10_img_boxfilter_col_border (dst_buf,
                                   dst,
                                   src_sz,
                                   dst_buf_stride,
                                   dst_stride,
                                   kernel,
                                   anchor,
                                   &border_t,
                                   &border_b);

    /* apply boxfilter column filter to image */
    ne10_img_boxfilter_col_c (dst_buf,
                              dst,
                              src_sz,
                              dst_buf_stride,
                              dst_stride,
                              kernel,
                              anchor,
                              border_t,
                              border_b);

    free (dst_buf);
}

/**
 * @} end of IMG_BOXFILTER group
 */
