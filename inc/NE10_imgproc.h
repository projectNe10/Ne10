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
 * NE10 Library : inc/NE10_imgproc.h
 */


#include <NE10_types.h>

#ifndef NE10_IMGPROC_H
#define NE10_IMGPROC_H

#ifdef __cplusplus
extern "C" {
#endif

    ne10_result_t ne10_init_imgproc (ne10_int32_t is_NEON_available);

///////////////////////////
// function prototypes:
///////////////////////////

    /* image resize functions*/
    /* function pointers*/
    extern void (*ne10_img_resize_bilinear_rgba) (ne10_uint8_t* dst,
            ne10_uint32_t dst_width,
            ne10_uint32_t dst_height,
            ne10_uint8_t* src,
            ne10_uint32_t src_width,
            ne10_uint32_t src_height,
            ne10_uint32_t src_stride);
    /* C version*/
    extern void ne10_img_resize_bilinear_rgba_c (ne10_uint8_t* dst,
            ne10_uint32_t dst_width,
            ne10_uint32_t dst_height,
            ne10_uint8_t* src,
            ne10_uint32_t src_width,
            ne10_uint32_t src_height,
            ne10_uint32_t src_stride);
    /* NEON version*/
    extern void ne10_img_resize_bilinear_rgba_neon (ne10_uint8_t* dst,
            ne10_uint32_t dst_width,
            ne10_uint32_t dst_height,
            ne10_uint8_t* src,
            ne10_uint32_t src_width,
            ne10_uint32_t src_height,
            ne10_uint32_t src_stride)
    asm ("ne10_img_resize_bilinear_rgba_neon");

    /* image rotate functions*/
    /* function pointers*/
    extern void (*ne10_img_rotate_rgba) (ne10_uint8_t* dst,
                                         ne10_uint32_t* dst_width,
                                         ne10_uint32_t* dst_height,
                                         ne10_uint8_t* src,
                                         ne10_uint32_t src_width,
                                         ne10_uint32_t src_height,
                                         ne10_int32_t angle);
    /* C version*/
    extern void ne10_img_rotate_rgba_c (ne10_uint8_t* dst,
                                        ne10_uint32_t* dst_width,
                                        ne10_uint32_t* dst_height,
                                        ne10_uint8_t* src,
                                        ne10_uint32_t src_width,
                                        ne10_uint32_t src_height,
                                        ne10_int32_t angle);
    /* NEON version*/
    extern void ne10_img_rotate_rgba_neon (ne10_uint8_t* dst,
                                           ne10_uint32_t* dst_width,
                                           ne10_uint32_t* dst_height,
                                           ne10_uint8_t* src,
                                           ne10_uint32_t src_width,
                                           ne10_uint32_t src_height,
                                           ne10_int32_t angle)
    asm ("ne10_img_rotate_rgba_neon");

    /* image boxfilter functions */
    /* function pointers */
    extern void (*ne10_img_boxfilter_rgba8888) (const ne10_uint8_t *src,
                                                ne10_uint8_t *dst,
                                                ne10_size_t src_size,
                                                ne10_int32_t src_stride,
                                                ne10_int32_t dst_stride,
                                                ne10_size_t kernel_size);
    /* C version*/
    extern void ne10_img_boxfilter_rgba8888_c (const ne10_uint8_t *src,
                                               ne10_uint8_t *dst,
                                               ne10_size_t src_size,
                                               ne10_int32_t src_stride,
                                               ne10_int32_t dst_stride,
                                               ne10_size_t kernel_size);
    /* NEON version*/
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
