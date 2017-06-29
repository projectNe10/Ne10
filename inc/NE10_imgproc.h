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
 * NE10 Library : inc/NE10_imgproc.h
 */


#include "NE10_types.h"

#ifndef NE10_IMGPROC_H
#define NE10_IMGPROC_H

#ifdef __cplusplus
extern "C" {
#endif
    ne10_result_t ne10_init_imgproc (ne10_int32_t is_NEON_available);

    /**
     * @ingroup IMG_RESIZE
     * @brief Image resize of 8-bit data.
     *
     * @param[out]  *dst                  point to the destination image
     * @param[in]   dst_width             width of destination image
     * @param[in]   dst_height            height of destination image
     * @param[in]   *src                  point to the source image
     * @param[in]   src_width             width of source image
     * @param[in]   src_height            height of source image
     * @param[in]   src_stride            stride of source buffer
     *
     * The function implements image resize.
     * Points to @ref ne10_img_resize_bilinear_rgba_c or @ref ne10_img_resize_bilinear_rgba_neon.
     */
    extern void (*ne10_img_resize_bilinear_rgba) (ne10_uint8_t* dst,
            ne10_uint32_t dst_width,
            ne10_uint32_t dst_height,
            ne10_uint8_t* src,
            ne10_uint32_t src_width,
            ne10_uint32_t src_height,
            ne10_uint32_t src_stride);
    extern void ne10_img_resize_bilinear_rgba_c (ne10_uint8_t* dst,
            ne10_uint32_t dst_width,
            ne10_uint32_t dst_height,
            ne10_uint8_t* src,
            ne10_uint32_t src_width,
            ne10_uint32_t src_height,
            ne10_uint32_t src_stride);
    extern void ne10_img_resize_bilinear_rgba_neon (ne10_uint8_t* dst,
            ne10_uint32_t dst_width,
            ne10_uint32_t dst_height,
            ne10_uint8_t* src,
            ne10_uint32_t src_width,
            ne10_uint32_t src_height,
            ne10_uint32_t src_stride)
    asm ("ne10_img_resize_bilinear_rgba_neon");

    /**
     * @ingroup IMG_ROTATE
     * @brief Image rotate of 8-bit data.
     *
     * @param[out]  *dst                  point to the destination image
     * @param[out]  *dst_width            width of destination image
     * @param[out]  *dst_height           height of destination image
     * @param[in]   *src                  point to the source image
     * @param[in]   src_width             width of source image
     * @param[in]   src_height            height of source image
     * @param[in]   angle                 angle of rotate
     *
     * The function extracts pixels from src at sub-pixel accuracy and stores them to dst.
     * Points to @ref ne10_img_rotate_rgba_c or @ref ne10_img_rotate_rgba_neon.
     */
    extern void (*ne10_img_rotate_rgba) (ne10_uint8_t* dst,
                                         ne10_uint32_t* dst_width,
                                         ne10_uint32_t* dst_height,
                                         ne10_uint8_t* src,
                                         ne10_uint32_t src_width,
                                         ne10_uint32_t src_height,
                                         ne10_int32_t angle);
    extern void ne10_img_rotate_rgba_c (ne10_uint8_t* dst,
                                        ne10_uint32_t* dst_width,
                                        ne10_uint32_t* dst_height,
                                        ne10_uint8_t* src,
                                        ne10_uint32_t src_width,
                                        ne10_uint32_t src_height,
                                        ne10_int32_t angle);
#ifdef ENABLE_NE10_IMG_ROTATE_RGBA_NEON
    extern void ne10_img_rotate_rgba_neon (ne10_uint8_t* dst,
                                           ne10_uint32_t* dst_width,
                                           ne10_uint32_t* dst_height,
                                           ne10_uint8_t* src,
                                           ne10_uint32_t src_width,
                                           ne10_uint32_t src_height,
                                           ne10_int32_t angle)
    asm ("ne10_img_rotate_rgba_neon");
#endif // ENABLE_NE10_IMG_ROTATE_RGBA_NEON

    /**
     * @ingroup IMG_BOXFILTER
     * @brief Box blur of RGBA8888 image data
     *
     * @param[out]  *dst       pointer to the output buffer
     * @param[in]   *src       pointer to the input buffer
     * @param[in]   src_sz     size of the source image
     * @param[in]   src_stride source stride
     * @param[in]   dst_stride destination stride
     * @param[in]   kernel     blurring kernel size (with sides between 1 and 65535 pixels wide)
     *
     * Applies a box filter (also known as a [box blur](https://en.wikipedia.org/wiki/Box_blur))
     * to an input image in the RGBA8888 format, producing a blurred output
     * image of the same size and format. The blurring kernel is anchored at its
     * center. When values must be obtained from beyond the source image
     * boundaries, zero-valued pixels are assumed.
     *
     * Points to @ref ne10_img_boxfilter_rgba8888_c or
     * @ref ne10_img_boxfilter_rgba8888_neon. This is an out-of-place algorithm.
     */
    extern void (*ne10_img_boxfilter_rgba8888) (const ne10_uint8_t *src,
            ne10_uint8_t *dst,
            ne10_size_t src_size,
            ne10_int32_t src_stride,
            ne10_int32_t dst_stride,
            ne10_size_t kernel_size);
    extern void ne10_img_boxfilter_rgba8888_c (const ne10_uint8_t *src,
            ne10_uint8_t *dst,
            ne10_size_t src_size,
            ne10_int32_t src_stride,
            ne10_int32_t dst_stride,
            ne10_size_t kernel_size);
    extern void ne10_img_boxfilter_rgba8888_neon (const ne10_uint8_t *src,
            ne10_uint8_t *dst,
            ne10_size_t src_size,
            ne10_int32_t src_stride,
            ne10_int32_t dst_stride,
            ne10_size_t kernel_size);

#ifdef __cplusplus
}
#endif

#endif
