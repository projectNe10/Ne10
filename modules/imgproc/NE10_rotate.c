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
 * NE10 Library : imgproc/NE10_rotate.c
 */

#include "NE10.h"
#include <math.h>


/**
 * @ingroup groupIMGPROCs
 * @defgroup IMG_ROTATE Image Rotation
 *
 * \par
 * Image rotate is a generic functionality in image processing. For C implementation, we take the cvGetQuadrangleSubPix function from OpenCV (http://opencv.org/) for reference.
 * \par
 * This set of functions implements image rotate with bilinear interpolation algorithm
 * for 8-bit data types.
 *
 */


void ne10_img_rotate_get_quad_rangle_subpix_rgba_c (ne10_uint8_t *dst,
        ne10_uint8_t *src,
        ne10_int32_t srcw,
        ne10_int32_t srch,
        ne10_int32_t dstw,
        ne10_int32_t dsth,
        ne10_float32_t *matrix)
{
    ne10_uint8_t* src_data = src;
    ne10_uint8_t* dst_data = dst;

    ne10_int32_t x, y;
    //ne10_float32_t dx = (dstw - 1) * 0.5;
    //ne10_float32_t dy = (dsth - 1) * 0.5;
    ne10_float32_t A11 = matrix[0], A12 = matrix[1], A13 = matrix[2];
    ne10_float32_t A21 = matrix[3], A22 = matrix[4], A23 = matrix[5];

    ne10_int32_t src_step = srcw * 4;
    ne10_int32_t dst_step = dstw * 4;
    for (y = 0; y < dsth; y++, dst_data += dst_step)
    {
        ne10_float32_t xs = A12 * y + A13;
        ne10_float32_t ys = A22 * y + A23;
        ne10_float32_t xe = A11 * (dstw - 1) + A12 * y + A13;
        ne10_float32_t ye = A21 * (dstw - 1) + A22 * y + A23;

        if ( (unsigned) ( (ne10_int32_t) (xs) - 1) < (unsigned) (srcw - 4) &&
                (unsigned) ( (ne10_int32_t) (ys) - 1) < (unsigned) (srch - 4) &&
                (unsigned) ( (ne10_int32_t) (xe) - 1) < (unsigned) (srcw - 4) &&
                (unsigned) ( (ne10_int32_t) (ye) - 1) < (unsigned) (srch - 4))
        {
            for (x = 0; x < dstw; x++)
            {
                ne10_int32_t ixs = (ne10_int32_t) (xs);
                ne10_int32_t iys = (ne10_int32_t) (ys);
                const ne10_uint8_t *ptr = src_data + src_step * iys + ixs * 4;
                //ne10_float32_t a = (xs - ixs), b = (ys - iys), a1 = (1.f - a);
                ne10_int16_t a = NE10_F2I16_OP (xs - ixs);
                ne10_int16_t b = NE10_F2I16_OP (ys - iys);
                ne10_int16_t a1 = NE10_F2I16_OP (1.f - (xs - ixs));

                ne10_uint8_t p0, p1;
                xs += A11;
                ys += A21;

                p0 = NE10_F2I16_SROUND (ptr[0] * a1 + ptr[4] * a);
                p1 = NE10_F2I16_SROUND (ptr[src_step] * a1 + ptr[src_step + 4] * a);
                dst_data[x * 4] = NE10_F2I16_SROUND (p0 * NE10_F2I16_MAX + b * (p1 - p0));

                p0 = NE10_F2I16_SROUND (ptr[1] * a1 + ptr[1] * a);
                p1 = NE10_F2I16_SROUND (ptr[src_step + 1] * a1 + ptr[src_step + 4 + 1] * a);
                dst_data[x * 4 + 1] = NE10_F2I16_SROUND (p0 * NE10_F2I16_MAX + b * (p1 - p0));

                p0 = NE10_F2I16_SROUND (ptr[2] * a1 + ptr[4 + 2] * a);
                p1 = NE10_F2I16_SROUND (ptr[src_step + 2] * a1 + ptr[src_step + 4 + 2] * a);
                dst_data[x * 4 + 2] = NE10_F2I16_SROUND (p0 * NE10_F2I16_MAX + b * (p1 - p0));

                p0 = NE10_F2I16_SROUND (ptr[3] * a1 + ptr[4 + 3] * a);
                p1 = NE10_F2I16_SROUND (ptr[src_step + 3] * a1 + ptr[src_step + 4 + 3] * a);
                dst_data[x * 4 + 3] = NE10_F2I16_SROUND (p0 * NE10_F2I16_MAX + b * (p1 - p0));
            }
        }
        else
        {
            for (x = 0; x < dstw; x++)
            {
                ne10_int32_t ixs = (ne10_int32_t) (xs), iys = (ne10_int32_t) (ys);
                //ne10_float32_t a = xs - ixs, b = ys - iys;
                //ne10_float32_t a1 = 1.f - a;
                ne10_int16_t  a =  NE10_F2I16_OP (xs - ixs);
                ne10_int16_t  b =  NE10_F2I16_OP (ys - iys);
                ne10_int16_t  a1 =  NE10_F2I16_OP (1.f - (xs - ixs));
                const ne10_uint8_t *ptr0, *ptr1;
                xs += A11;
                ys += A21;

                if ( (unsigned) iys < (unsigned) (srch - 1))
                {
                    ptr0 = src_data + src_step * iys;
                    ptr1 = ptr0 + src_step;
                }
                else
                {
                    continue;
                }

                if ( (unsigned) ixs < (unsigned) (srcw - 1))
                {

                    ne10_uint8_t p0, p1;

                    ptr0 += ixs * 4;
                    ptr1 += ixs * 4;

                    p0 = NE10_F2I16_SROUND (ptr0[0] * a1 + ptr0[4] * a);
                    p1 = NE10_F2I16_SROUND (ptr1[0] * a1 + ptr1[4] * a);
                    dst_data[x * 4] = NE10_F2I16_SROUND (p0 * NE10_F2I16_MAX + b * (p1 - p0));

                    p0 = NE10_F2I16_SROUND (ptr0[1] * a1 + ptr0[4 + 1] * a);
                    p1 = NE10_F2I16_SROUND (ptr1[1] * a1 + ptr1[4 + 1] * a);
                    dst_data[x * 4 + 1] = NE10_F2I16_SROUND (p0 * NE10_F2I16_MAX + b * (p1 - p0));

                    p0 = NE10_F2I16_SROUND (ptr0[2] * a1 + ptr0[4 + 2] * a);
                    p1 = NE10_F2I16_SROUND (ptr1[2] * a1 + ptr1[4 + 2] * a);
                    dst_data[x * 4 + 2] = NE10_F2I16_SROUND (p0 * NE10_F2I16_MAX + b * (p1 - p0));

                    p0 = NE10_F2I16_SROUND (ptr0[3] * a1 + ptr0[4 + 3] * a);
                    p1 = NE10_F2I16_SROUND (ptr1[3] * a1 + ptr1[4 + 3] * a);
                    dst_data[x * 4 + 3] = NE10_F2I16_SROUND (p0 * NE10_F2I16_MAX + b * (p1 - p0));
                }
            }
        }
    }
}


/**
 * @ingroup IMG_ROTATE
 * Specific implementation of @ref ne10_img_rotate_rgba using plain C.
 */
void ne10_img_rotate_rgba_c (ne10_uint8_t* dst,
                             ne10_uint32_t* dst_width,
                             ne10_uint32_t* dst_height,
                             ne10_uint8_t* src,
                             ne10_uint32_t src_width,
                             ne10_uint32_t src_height,
                             ne10_int32_t angle)
{
    ne10_float32_t radian = (angle * NE10_PI / 180.0);
    ne10_float32_t a = sin (radian), b = cos (radian);
    ne10_int32_t srcw = src_width;
    ne10_int32_t srch = src_height;
    ne10_int32_t dstw = (srch * fabs (a)) + (srcw * fabs (b)) + 1;
    ne10_int32_t dsth = (srch * fabs (b)) + (srcw * fabs (a)) + 1;
    ne10_float32_t m[6];
    ne10_float32_t dx = (dstw - 1) * 0.5;
    ne10_float32_t dy = (dsth - 1) * 0.5;

    m[0] = b;
    m[1] = a;
    m[3] = -m[1];
    m[4] = m[0];
    m[2] = srcw * 0.5f - m[0] * dx - m[1] * dy;
    m[5] = srch * 0.5f - m[3] * dx - m[4] * dy;

    *dst_width = dstw;
    *dst_height = dsth;
    ne10_img_rotate_get_quad_rangle_subpix_rgba_c (dst, src, srcw, srch, dstw, dsth, m);
}

#ifdef ENABLE_NE10_IMG_ROTATE_RGBA_NEON
extern void ne10_img_rotate_get_quad_rangle_subpix_rgba_neon (ne10_uint8_t *dst,
        ne10_uint8_t *src,
        ne10_int32_t srcw,
        ne10_int32_t srch,
        ne10_int32_t dstw,
        ne10_int32_t dsth,
        ne10_float32_t *matrix)
    asm("ne10_img_rotate_get_quad_rangle_subpix_rgba_neon");

/**
 * @ingroup IMG_ROTATE
 * Specific implementation of @ref ne10_img_rotate_rgba using NEON SIMD capabilities.
 */
void ne10_img_rotate_rgba_neon (ne10_uint8_t* dst,
                                ne10_uint32_t* dst_width,
                                ne10_uint32_t* dst_height,
                                ne10_uint8_t* src,
                                ne10_uint32_t src_width,
                                ne10_uint32_t src_height,
                                ne10_int32_t angle)
{
    ne10_float32_t radian = (angle * NE10_PI / 180.0);
    ne10_float32_t a = sin (radian), b = cos (radian);
    ne10_int32_t srcw = src_width;
    ne10_int32_t srch = src_height;
    ne10_int32_t dstw = (srch * fabs (a)) + (srcw * fabs (b)) + 1;
    ne10_int32_t dsth = (srch * fabs (b)) + (srcw * fabs (a)) + 1;
    ne10_float32_t m[6];
    ne10_float32_t dx = (dstw - 1) * 0.5;
    ne10_float32_t dy = (dsth - 1) * 0.5;

    m[0] = b;
    m[1] = a;
    m[3] = -m[1];
    m[4] = m[0];
    m[2] = srcw * 0.5f - m[0] * dx - m[1] * dy;
    m[5] = srch * 0.5f - m[3] * dx - m[4] * dy;

    *dst_width = dstw;
    *dst_height = dsth;
    ne10_img_rotate_get_quad_rangle_subpix_rgba_neon (dst, src, srcw, srch, dstw, dsth, m);
}
#endif // ENABLE_NE10_IMG_ROTATE_RGBA_NEON
