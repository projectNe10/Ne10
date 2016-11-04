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
 * NE10 Library : imgproc/NE10_boxfilter.c
 */

#include "NE10.h"
#include <stdlib.h>
#include <math.h>
#include <arm_neon.h>
/**
 * @ingroup groupIMGPROCs
 */

extern void ne10_img_boxfilter_row_border (const ne10_uint8_t* src,
                                           ne10_uint8_t* dst,
                                           ne10_size_t src_sz,
                                           ne10_int32_t src_stride,
                                           ne10_int32_t dst_stride,
                                           ne10_size_t kernel,
                                           ne10_point_t anchor,
                                           ne10_int32_t *border_l_ptr,
                                           ne10_int32_t *border_r_ptr);

extern void ne10_img_boxfilter_col_border (const ne10_uint8_t *src,
                                           ne10_uint8_t *dst,
                                           ne10_size_t src_sz,
                                           ne10_int32_t src_stride,
                                           ne10_int32_t dst_stride,
                                           ne10_size_t kernel,
                                           ne10_point_t anchor,
                                           ne10_int32_t *border_t_ptr,
                                           ne10_int32_t *border_b_ptr);

extern void ne10_img_boxfilter_row_c (const ne10_uint8_t *src,
                                      ne10_uint8_t *dst,
                                      ne10_size_t src_sz,
                                      ne10_int32_t src_stride,
                                      ne10_int32_t dst_stride,
                                      ne10_size_t kernel,
                                      ne10_point_t anchor,
                                      ne10_int32_t border_l,
                                      ne10_int32_t border_r);

extern void ne10_img_boxfilter_col_c (const ne10_uint8_t *src,
                                      ne10_uint8_t *dst,
                                      ne10_size_t src_sz,
                                      ne10_int32_t src_stride,
                                      ne10_int32_t dst_stride,
                                      ne10_size_t kernel,
                                      ne10_point_t anchor,
                                      ne10_int32_t border_t,
                                      ne10_int32_t border_b);

/* RGBA CHANNEL number is 4 */
#define RGBA_CH 4
/* DIV_SHIFT is used in replacement of constant division */
#define DIV_SHIFT 15

void ne10_img_boxfilter_row_neon (const ne10_uint8_t *src,
                                  ne10_uint8_t *dst,
                                  ne10_size_t src_sz,
                                  ne10_int32_t src_stride,
                                  ne10_int32_t dst_stride,
                                  ne10_size_t kernel,
                                  ne10_point_t anchor,
                                  ne10_int32_t border_l,
                                  ne10_int32_t border_r)
{
    /* when in special cases, we'll call the c version of row filter */
    if (src_sz.y == 1 || kernel.x >= (1 << 7) || kernel.x == 1)
    {
        return ne10_img_boxfilter_row_c (src,
                                         dst,
                                         src_sz,
                                         src_stride,
                                         dst_stride,
                                         kernel,
                                         anchor,
                                         border_l,
                                         border_r);
    }

    assert (src != dst);
    assert (kernel.x > 0);
    assert ( (kernel.x <= src_sz.x) && (kernel.y <= src_sz.y));
    /* if kernel.x was 1, the mul variable would overflow when it
     * is casted to ne10_int16_t type.
     */
    assert ((src_sz.y > 1) &&
            (kernel.x < (1 << 7)) &&
            (kernel.x > 1));

    ne10_int32_t x, y, k;
    ne10_int16_t mul = (1 << DIV_SHIFT) / kernel.x;
    int16x8_t mul_vec = vdupq_n_s16 (mul);

    for (y = 0; y < src_sz.y; y += 2)
    {
        /* step back one row when image height is odd and before reaching last
         * line
         */
        if ((src_sz.y % 2 != 0) && (y == src_sz.y - 1))
            y--;

        const ne10_uint8_t *src_row1 = src + y * src_stride;
        const ne10_uint8_t *src_row2 = src + (y + 1) * src_stride;
        ne10_uint8_t *dst_row1 = dst + y * dst_stride;
        ne10_uint8_t *dst_row2 = dst + (y + 1) * dst_stride;
        ne10_int16_t sum[RGBA_CH * 2];

        for (k = 0; k < RGBA_CH; k++)
        {
            sum[k] = 0;
            sum[k + 4] = 0;

            for (x = 0; x < kernel.x; x++)
            {
                sum[k] += * (src_row1 + x * RGBA_CH + k);
                sum[k + 4] += * (src_row2 + x * RGBA_CH + k);
            }

            *(dst_row1 + border_l * RGBA_CH + k) = sum[k] * mul >>
                                                   DIV_SHIFT;
            *(dst_row2 + border_l * RGBA_CH + k) = sum[k + 4] * mul >>
                                                   DIV_SHIFT;
        }

        ne10_uint32_t prev = (anchor.x + 1) * RGBA_CH;
        ne10_uint32_t next = (kernel.x - anchor.x - 1) * RGBA_CH;
        const ne10_uint8_t *src_pixel1 = src_row1 + (1 + border_l) * RGBA_CH;
        const ne10_uint8_t *src_pixel2 = src_row2 + (1 + border_l) * RGBA_CH;
        const ne10_uint8_t *src_pixel_end = src_row1 + (src_sz.x - border_r) *
                                             RGBA_CH;
        ne10_uint8_t *dst_pixel1 = dst_row1 + (1 + border_l) * RGBA_CH;
        ne10_uint8_t *dst_pixel2 = dst_row2 + (1 + border_l) * RGBA_CH;

        int16x8_t sum_vec = vld1q_s16 (sum);
        int16x8_t sum_tmp;
        uint16x8_t sum_vec_u;
        uint8x8_t src_pixel_next_vec, src_pixel_prev_vec;
        uint32x2_t src_pixel_next_tmp_vec, src_pixel_prev_tmp_vec;
        uint32x2_t src_pixel_next_tmp_vec_pre, src_pixel_prev_tmp_vec_pre;
        uint32x2_t dst_pixel_vec;
        uint8x8_t dst_pixel_tmp_vec;


        /* preload */
        src_pixel_next_tmp_vec = vld1_lane_u32 (
                                     (const ne10_uint32_t*) (src_pixel1 + next),
                                     src_pixel_next_tmp_vec,
                                     0);
        src_pixel_prev_tmp_vec = vld1_lane_u32 (
                                     (const ne10_uint32_t*) (src_pixel1 - prev),
                                     src_pixel_prev_tmp_vec,
                                     0);
        src_pixel_next_tmp_vec = vld1_lane_u32 (
                                     (const ne10_uint32_t*) (src_pixel2 + next),
                                     src_pixel_next_tmp_vec,
                                     1);
        src_pixel_prev_tmp_vec = vld1_lane_u32 (
                                     (const ne10_uint32_t*) (src_pixel2 - prev),
                                     src_pixel_prev_tmp_vec,
                                     1);

        /* load two rows to do filtering */
        while (src_pixel1 < src_pixel_end)
        {
            /* preload */
            src_pixel_next_tmp_vec_pre = vld1_lane_u32 (
                                             (const ne10_uint32_t*) (src_pixel1 + 4 + next),
                                             src_pixel_next_tmp_vec_pre,
                                             0);
            src_pixel_prev_tmp_vec_pre = vld1_lane_u32 (
                                             (const ne10_uint32_t*) (src_pixel1 + 4 - prev),
                                             src_pixel_prev_tmp_vec_pre,
                                             0);
            src_pixel_next_tmp_vec_pre = vld1_lane_u32 (
                                             (const ne10_uint32_t*) (src_pixel2 + 4 + next),
                                             src_pixel_next_tmp_vec_pre,
                                             1);
            src_pixel_prev_tmp_vec_pre = vld1_lane_u32 (
                                             (const ne10_uint32_t*) (src_pixel2 + 4 - prev),
                                             src_pixel_prev_tmp_vec_pre,
                                             1);

            src_pixel_prev_vec = vreinterpret_u8_u32 (src_pixel_prev_tmp_vec);
            src_pixel_next_vec = vreinterpret_u8_u32 (src_pixel_next_tmp_vec);

            sum_vec_u = vreinterpretq_u16_s16 (sum_vec);
            sum_vec_u = vaddw_u8 (sum_vec_u, src_pixel_next_vec);
            sum_vec_u = vsubw_u8 (sum_vec_u, src_pixel_prev_vec);
            sum_vec = vreinterpretq_s16_u16 (sum_vec_u);
            /* vqdmulhq_n_s16 would shift the result 16 bit */
            sum_tmp = vqdmulhq_s16 (sum_vec, mul_vec);
            dst_pixel_tmp_vec = vqmovun_s16 (sum_tmp);
            dst_pixel_vec = vreinterpret_u32_u8 (dst_pixel_tmp_vec);
            vst1_lane_u32 ((ne10_uint32_t *) dst_pixel1, dst_pixel_vec, 0);
            vst1_lane_u32 ((ne10_uint32_t *) dst_pixel2, dst_pixel_vec, 1);

            src_pixel_prev_tmp_vec = src_pixel_prev_tmp_vec_pre;
            src_pixel_next_tmp_vec = src_pixel_next_tmp_vec_pre;

            src_pixel1 += 4;
            src_pixel2 += 4;
            dst_pixel1 += 4;
            dst_pixel2 += 4;
        }
    }
}

void ne10_img_boxfilter_col_neon (const ne10_uint8_t *src,
                                  ne10_uint8_t *dst,
                                  ne10_size_t src_sz,
                                  ne10_int32_t src_stride,
                                  ne10_int32_t dst_stride,
                                  ne10_size_t kernel,
                                  ne10_point_t anchor,
                                  ne10_int32_t border_t,
                                  ne10_int32_t border_b)
{
    /* when in special cases, we'll call c version to do the work */
    if (kernel.y == 1 || kernel.y >= (1 << 7) || src_sz.x == 1)
    {
        return ne10_img_boxfilter_col_c (src,
                                         dst,
                                         src_sz,
                                         src_stride,
                                         dst_stride,
                                         kernel,
                                         anchor,
                                         border_t,
                                         border_b);
    }

    assert (src != dst);
    assert (kernel.y > 0);
    assert ( (kernel.x <= src_sz.x) && (kernel.y <= src_sz.y));
    /* if kernel.y was 1, the mul variable would overflow when it
     * is casted to ne10_int16_t type.
     */
    assert ( (src_sz.x > 1) &&
             (kernel.y < (1 << 7)) &&
             (kernel.y > 1));

    ne10_int32_t x, y, k;
    ne10_uint16_t *sum_row = (ne10_uint16_t *) malloc (src_sz.x *
                             RGBA_CH *
                             sizeof (ne10_uint16_t));
    ne10_uint16_t mul = (1 << DIV_SHIFT) / kernel.y;

    if (!sum_row)
    {
        fprintf (stderr,
                 "ERROR: buffer allocation fails!\nallocation size: %d\n",
                 sizeof (ne10_uint32_t) *
                 src_sz.x *
                 RGBA_CH);
        return;
    }

    for (x = 0; x < src_sz.x * RGBA_CH; x++)
    {
        sum_row[x] = 0;
    }

    for (x = 0; x < src_sz.x; x++)
    {
        const ne10_uint8_t *src_col = src + x * RGBA_CH;
        ne10_uint8_t *dst_col = dst + x * RGBA_CH;
        ne10_uint8_t *dst_pixel = dst_col + border_t * dst_stride;
        ne10_uint16_t *sum = sum_row + x * RGBA_CH;

        for (y = 0; y < kernel.y; y++)
        {
            const ne10_uint8_t *src_pixel = src_col + y * src_stride;

            for (k = 0; k < RGBA_CH; k++)
            {
                sum[k] += src_pixel[k];
            }
        }

        for (k = 0; k < RGBA_CH; k++)
        {
            dst_pixel[k] = sum_row[x * RGBA_CH + k] * mul >>
                           DIV_SHIFT;
        }
    }

    const ne10_uint8_t *src_row = src + (1 + border_t) * src_stride;
    const ne10_uint8_t *src_row_end = src + (src_sz.y - border_b) *
                                      src_stride;
    ne10_uint8_t *dst_row = dst + (1 + border_t) * dst_stride;
    ne10_uint32_t prev = (anchor.y + 1) * src_stride;
    ne10_uint32_t next = (kernel.y - anchor.y - 1) * src_stride;

    uint16x8_t sum_vec, sum_vec_pre;
    int16x8_t sum_vec_s;
    uint8x8_t src_pixel_prev_vec, src_pixel_next_vec;
    uint8x8_t src_pixel_prev_vec_pre, src_pixel_next_vec_pre;
    uint8x8_t dst_pixel_vec;

    ne10_uint16_t sum_val_bakcup[RGBA_CH];
    ne10_uint32_t src_sz_x_adjust = src_sz.x;
    int16x8_t mul_vec = vdupq_n_s16 (mul);

    if (src_sz.x % 2 != 0)
    {
        for (k = 0; k < RGBA_CH; k++)
            sum_val_bakcup[k] = sum_row[ (src_sz.x - 2) * RGBA_CH + k];
        src_sz_x_adjust = src_sz.x - 1;
    }

    /* boxfilter column filter is done once in a row, which
     * is more friendly to cache than once in a column.
     */
    while (src_row < src_row_end)
    {
        /* preload */
        const ne10_uint8_t *src_pixel = src_row;
        ne10_uint8_t *dst_pixel = dst_row;
        src_pixel_prev_vec = vld1_u8 (src_pixel - prev);
        src_pixel_next_vec = vld1_u8 (src_pixel + next);
        ne10_uint16_t *sum, *sum_pre;
        sum_vec = vld1q_u16 (sum_row);

        for (x = 0; x < src_sz_x_adjust; x += 2)
        {
            sum = sum_row + x * RGBA_CH;
            sum_pre = sum + 2 * RGBA_CH;
            sum_vec_pre = vld1q_u16 (sum_pre);
            /* preload */
            src_pixel = src_row + (x + 2) * RGBA_CH;
            src_pixel_prev_vec_pre = vld1_u8 (src_pixel - prev);
            src_pixel_next_vec_pre = vld1_u8 (src_pixel + next);

            sum_vec = vaddw_u8 (sum_vec, src_pixel_next_vec);
            sum_vec = vsubw_u8 (sum_vec, src_pixel_prev_vec);
            sum_vec_s = vreinterpretq_s16_u16 (sum_vec);
            /* vqdmulhq_n_s16 would shift the result 16 bit */
            sum_vec_s = vqdmulhq_s16 (sum_vec_s, mul_vec);
            dst_pixel_vec = vqmovun_s16 (sum_vec_s);
            dst_pixel = dst_row + x * RGBA_CH;
            vst1_u8 (dst_pixel, dst_pixel_vec);
            vst1q_u16 (sum, sum_vec);

            src_pixel_next_vec = src_pixel_next_vec_pre;
            src_pixel_prev_vec = src_pixel_prev_vec_pre;
            sum_vec = sum_vec_pre;
        }
        src_row += src_stride;
        dst_row += dst_stride;
    }

    if (src_sz.x % 2 != 0)
    {
        for (k = 0; k < RGBA_CH; k++)
            sum_row[ (src_sz.x - 2) * RGBA_CH + k] = sum_val_bakcup[k];

        src_row = src + (1 + border_t) * src_stride;
        dst_row = dst + (1 + border_t) * dst_stride;
        /* step back one column */
        x = src_sz.x - 2;
        sum_vec = vld1q_u16 (sum_row + x * RGBA_CH);

        while (src_row < src_row_end)
        {
            const ne10_uint8_t *src_pixel = src_row + x * RGBA_CH;
            ne10_uint8_t *dst_pixel = dst_row + x * RGBA_CH;
            src_pixel_prev_vec = vld1_u8 (src_pixel - prev);
            src_pixel_next_vec = vld1_u8 (src_pixel + next);
            sum_vec = vaddw_u8 (sum_vec, src_pixel_next_vec);
            sum_vec = vsubw_u8 (sum_vec, src_pixel_prev_vec);
            sum_vec_s = vreinterpretq_s16_u16 (sum_vec);
            /* vqdmulhq_n_s16 would shift the result 16 bit
             */
            sum_vec_s = vqdmulhq_s16 (sum_vec_s, mul_vec);
            dst_pixel_vec = vqmovun_s16 (sum_vec_s);
            vst1_u8 (dst_pixel, dst_pixel_vec);

            src_row += src_stride;
            dst_row += dst_stride;
        }
    }

    free (sum_row);
}

/**
 * @addtogroup IMG_BOXFILTER
 * @{
 */

/**
 * @brief NEON optimized box filter.
 * @param[out]  *dst                  point to the destination image
 * @param[in]   *src                  point to the source image
 * @param[in]   src_sz              source image size struction
 * @param[in]   src_stride            stride of source image
 * @param[in]   dst_stride            stride of destination image
 * @param[in]   kernel                kernel size of box filter, both of kernel
 * edge supports from 2 to 127 are all supported. and for 1 and larger than 127
 * case, it is supported by calling the c version of box filter
 *
 * The function implements box filter, destination image's size is smae as
 * source image
 */
void ne10_img_boxfilter_rgba8888_neon (const ne10_uint8_t *src,
                                       ne10_uint8_t *dst,
                                       ne10_size_t src_sz,
                                       ne10_int32_t src_stride,
                                       ne10_int32_t dst_stride,
                                       ne10_size_t kernel)
{
    assert (src != 0 && dst != 0);
    assert (src_sz.x > 0 && src_sz.y > 0);
    assert (src_stride > 0 && dst_stride > 0);
    assert (kernel.x > 0 && kernel.x <= src_sz.x
            && kernel.y > 0 && kernel.y <= src_sz.y);

    ne10_int32_t border_l, border_r, border_t, border_b;
    ne10_point_t anchor;

    anchor.x = kernel.x / 2;
    anchor.y = kernel.y / 2;

    /* the extra 2 elements here is reserved for pre-load when do row or column
     * filter */
    ne10_uint32_t mem_bytes = (sizeof (ne10_uint8_t) * src_sz.x * src_sz.y + 2)
        * RGBA_CH;

    ne10_uint8_t *dst_buf = (ne10_uint8_t *) malloc (mem_bytes);

    if (!dst_buf)
    {
        fprintf (stderr,
                 "ERROR: buffer allocation fails!\nallocation size: %d\n",
                 mem_bytes);
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
    ne10_img_boxfilter_row_neon (src,
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
    ne10_img_boxfilter_col_neon (dst_buf,
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
