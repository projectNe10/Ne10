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
 * NE10 Library : imgproc/NE10_resize.c
 */

#include "NE10.h"

/**
 * @ingroup groupIMGPROCs
 */
/**
 * @defgroup IMG_RESIZE Image Resize
 *
 * \par
 * Image resize is a generic functionality in image processing. In Ne10 library, we use the bilinear interpolation algorithm (http://en.wikipedia.org/wiki/Bilinear_interpolation) to implement image resize. For C implementation, we take the cvResize function from OpenCV (http://opencv.org/) for reference.
 * \par
 * This set of functions implements image resize with bilinear interpolation algorithm
 * for 8-bit data types.  The functions operate on out-of-place buffer which use different buffer for input and output.
 *
 */
#define INTER_RESIZE_COEF_BITS  11
#define INTER_RESIZE_COEF_SCALE (1 << 11)
#define NE10_MAX_ESIZE          16

static inline ne10_uint32_t ne10_align_size (ne10_int32_t sz, ne10_int32_t n)
{
    return (sz + n - 1) & -n;
}

static inline ne10_int32_t ne10_floor (ne10_float32_t a)
{
    return ( ( (a) >= 0) ? ( (ne10_int32_t) a) : ( (ne10_int32_t) a - 1));
}

static inline ne10_int32_t ne10_clip (ne10_int32_t x, ne10_int32_t a, ne10_int32_t b)
{
    return (x >= a ? (x < b ? x : b - 1) : a);
}

static inline ne10_uint8_t ne10_cast_op (ne10_int32_t val)
{
    ne10_int32_t bits = INTER_RESIZE_COEF_BITS * 2;
    ne10_int32_t SHIFT = bits;
    ne10_int32_t DELTA = 1 << (bits - 1) ;
    ne10_int32_t temp = NE10_MIN (255, NE10_MAX (0, (val + DELTA) >> SHIFT));
    return (ne10_uint8_t) (temp);
};

static void ne10_img_hresize_linear_c (const ne10_uint8_t** src,
                                       ne10_int32_t** dst,
                                       ne10_int32_t count,
                                       const ne10_int32_t* xofs,
                                       const ne10_int16_t* alpha,
                                       ne10_int32_t swidth,
                                       ne10_int32_t dwidth,
                                       ne10_int32_t cn,
                                       ne10_int32_t xmin,
                                       ne10_int32_t xmax)
{
    ne10_int32_t dx, k;

    ne10_int32_t dx0 = 0;

    //for (k = 0; k <= count - 2; k++)
    if (count == 2)
    {
        k = 0;
        const ne10_uint8_t *S0 = src[k], *S1 = src[k + 1];
        ne10_int32_t *D0 = dst[k], *D1 = dst[k + 1];
        for (dx = dx0; dx < xmax; dx++)
        {
            ne10_int32_t sx = xofs[dx];
            ne10_int32_t a0 = alpha[dx * 2], a1 = alpha[dx * 2 + 1];
            ne10_int32_t t0 = S0[sx] * a0 + S0[sx + cn] * a1;
            ne10_int32_t t1 = S1[sx] * a0 + S1[sx + cn] * a1;
            D0[dx] = t0;
            D1[dx] = t1;
        }

        for (; dx < dwidth; dx++)
        {
            ne10_int32_t sx = xofs[dx];
            D0[dx] = (ne10_int32_t) S0[sx] * INTER_RESIZE_COEF_SCALE;
            D1[dx] = (ne10_int32_t) S1[sx] * INTER_RESIZE_COEF_SCALE;
        }
    }

    //for (; k < count; k++)
    if (count == 1)
    {
        k = 0;
        const ne10_uint8_t *S = src[k];
        ne10_int32_t *D = dst[k];
        for (dx = 0; dx < xmax; dx++)
        {
            ne10_int32_t sx = xofs[dx];
            D[dx] = S[sx] * alpha[dx * 2] + S[sx + cn] * alpha[dx * 2 + 1];
        }

        for (; dx < dwidth; dx++)
            D[dx] = (ne10_int32_t) S[xofs[dx]] * INTER_RESIZE_COEF_SCALE;
    }
}


static void ne10_img_vresize_linear_c (const ne10_int32_t** src, ne10_uint8_t* dst, const ne10_int16_t* beta, ne10_int32_t width)
{
    ne10_int32_t b0 = beta[0], b1 = beta[1];
    const ne10_int32_t *S0 = src[0], *S1 = src[1];

    ne10_int32_t x = 0;
    for (; x <= width - 4; x += 4)
    {
        ne10_int32_t t0, t1;
        t0 = S0[x] * b0 + S1[x] * b1;
        t1 = S0[x + 1] * b0 + S1[x + 1] * b1;
        dst[x] = ne10_cast_op (t0);
        dst[x + 1] = ne10_cast_op (t1);
        t0 = S0[x + 2] * b0 + S1[x + 2] * b1;
        t1 = S0[x + 3] * b0 + S1[x + 3] * b1;
        dst[x + 2] = ne10_cast_op (t0);
        dst[x + 3] = ne10_cast_op (t1);
    }

    for (; x < width; x++)
        dst[x] = ne10_cast_op (S0[x] * b0 + S1[x] * b1);
}

static void ne10_img_resize_generic_linear_c (ne10_uint8_t* src,
        ne10_uint8_t* dst,
        const ne10_int32_t* xofs,
        const ne10_int16_t* _alpha,
        const ne10_int32_t* yofs,
        const ne10_int16_t* _beta,
        ne10_int32_t xmin,
        ne10_int32_t xmax,
        ne10_int32_t ksize,
        ne10_int32_t srcw,
        ne10_int32_t srch,
        ne10_int32_t srcstep,
        ne10_int32_t dstw,
        ne10_int32_t dsth,
        ne10_int32_t channels)
{

    const ne10_int16_t* alpha = _alpha;
    const ne10_int16_t* beta = _beta;
    ne10_int32_t cn = channels;
    srcw *= cn;
    dstw *= cn;

    ne10_int32_t bufstep = (ne10_int32_t) ne10_align_size (dstw, 16);
    ne10_int32_t dststep = (ne10_int32_t) ne10_align_size (dstw, 4);


    ne10_int32_t *buffer_ = (ne10_int32_t*) NE10_MALLOC (bufstep * ksize * sizeof (ne10_int32_t));

    const ne10_uint8_t* srows[NE10_MAX_ESIZE];
    ne10_int32_t* rows[NE10_MAX_ESIZE];
    ne10_int32_t prev_sy[NE10_MAX_ESIZE];
    ne10_int32_t k, dy;
    xmin *= cn;
    xmax *= cn;

    for (k = 0; k < ksize; k++)
    {
        prev_sy[k] = -1;
        rows[k] = (ne10_int32_t*) buffer_ + bufstep * k;
    }

    // image resize is a separable operation. In case of not too strong
    for (dy = 0; dy < dsth; dy++, beta += ksize)
    {
        ne10_int32_t sy0 = yofs[dy], k, k0 = ksize, k1 = 0, ksize2 = ksize / 2;

        for (k = 0; k < ksize; k++)
        {
            ne10_int32_t sy = ne10_clip (sy0 - ksize2 + 1 + k, 0, srch);
            for (k1 = NE10_MAX (k1, k); k1 < ksize; k1++)
            {
                if (sy == prev_sy[k1])  // if the sy-th row has been computed already, reuse it.
                {
                    if (k1 > k)
                        memcpy (rows[k], rows[k1], bufstep * sizeof (rows[0][0]));
                    break;
                }
            }
            if (k1 == ksize)
                k0 = NE10_MIN (k0, k); // remember the first row that needs to be computed
            srows[k] = (const ne10_uint8_t*) (src + srcstep * sy);
            prev_sy[k] = sy;
        }

        if (k0 < ksize)
            ne10_img_hresize_linear_c (srows + k0, rows + k0, ksize - k0, xofs, alpha,
                                       srcw, dstw, cn, xmin, xmax);

        ne10_img_vresize_linear_c ( (const ne10_int32_t**) rows, (ne10_uint8_t*) (dst + dststep * dy), beta, dstw);
    }

    NE10_FREE (buffer_);
}

static void ne10_img_resize_cal_offset_linear (ne10_int32_t* xofs,
        ne10_int16_t* ialpha,
        ne10_int32_t* yofs,
        ne10_int16_t* ibeta,
        ne10_int32_t *xmin,
        ne10_int32_t *xmax,
        ne10_int32_t ksize,
        ne10_int32_t ksize2,
        ne10_int32_t srcw,
        ne10_int32_t srch,
        ne10_int32_t dstw,
        ne10_int32_t dsth,
        ne10_int32_t channels)
{
    ne10_float32_t inv_scale_x = (ne10_float32_t) dstw / srcw;
    ne10_float32_t inv_scale_y = (ne10_float32_t) dsth / srch;

    ne10_int32_t cn = channels;
    ne10_float32_t scale_x = 1. / inv_scale_x;
    ne10_float32_t scale_y = 1. / inv_scale_y;
    ne10_int32_t k, sx, sy, dx, dy;


    ne10_float32_t fx, fy;

    ne10_float32_t cbuf[NE10_MAX_ESIZE];

    for (dx = 0; dx < dstw; dx++)
    {
        fx = (ne10_float32_t) ( (dx + 0.5) * scale_x - 0.5);
        sx = ne10_floor (fx);
        fx -= sx;

        if (sx < ksize2 - 1)
        {
            *xmin = dx + 1;
            if (sx < 0)
                fx = 0, sx = 0;
        }

        if (sx + ksize2 >= srcw)
        {
            *xmax = NE10_MIN (*xmax, dx);
            if (sx >= srcw - 1)
                fx = 0, sx = srcw - 1;
        }

        for (k = 0, sx *= cn; k < cn; k++)
            xofs[dx * cn + k] = sx + k;

        cbuf[0] = 1.f - fx;
        cbuf[1] = fx;

        for (k = 0; k < ksize; k++)
            ialpha[dx * cn * ksize + k] = (ne10_int16_t) (cbuf[k] * INTER_RESIZE_COEF_SCALE);
        for (; k < cn * ksize; k++)
            ialpha[dx * cn * ksize + k] = ialpha[dx * cn * ksize + k - ksize];
    }

    for (dy = 0; dy < dsth; dy++)
    {
        fy = (ne10_float32_t) ( (dy + 0.5) * scale_y - 0.5);
        sy = ne10_floor (fy);
        fy -= sy;

        yofs[dy] = sy;

        cbuf[0] = 1.f - fy;
        cbuf[1] = fy;

        for (k = 0; k < ksize; k++)
            ibeta[dy * ksize + k] = (ne10_int16_t) (cbuf[k] * INTER_RESIZE_COEF_SCALE);

    }

}

extern void ne10_img_hresize_4channels_linear_neon (const ne10_uint8_t** src,
        ne10_int32_t** dst,
        ne10_int32_t count,
        const ne10_int32_t* xofs,
        const ne10_int16_t* alpha,
        ne10_int32_t swidth,
        ne10_int32_t dwidth,
        ne10_int32_t cn,
        ne10_int32_t xmin,
        ne10_int32_t xmax);
extern void ne10_img_vresize_linear_neon (const ne10_int32_t** src, ne10_uint8_t* dst, const ne10_int16_t* beta, ne10_int32_t width);

static void ne10_img_resize_generic_linear_neon (ne10_uint8_t* src,
        ne10_uint8_t* dst,
        const ne10_int32_t* xofs,
        const ne10_int16_t* _alpha,
        const ne10_int32_t* yofs,
        const ne10_int16_t* _beta,
        ne10_int32_t xmin,
        ne10_int32_t xmax,
        ne10_int32_t ksize,
        ne10_int32_t srcw,
        ne10_int32_t srch,
        ne10_int32_t srcstep,
        ne10_int32_t dstw,
        ne10_int32_t dsth,
        ne10_int32_t channels)
{

    const ne10_int16_t* alpha = _alpha;
    const ne10_int16_t* beta = _beta;
    ne10_int32_t cn = channels;
    srcw *= cn;
    dstw *= cn;

    ne10_int32_t bufstep = (ne10_int32_t) ne10_align_size (dstw, 16);
    ne10_int32_t dststep = (ne10_int32_t) ne10_align_size (dstw, 4);


    ne10_int32_t *buffer_ = (ne10_int32_t*) NE10_MALLOC (bufstep * ksize * sizeof (ne10_int32_t));

    const ne10_uint8_t* srows[NE10_MAX_ESIZE];
    ne10_int32_t* rows[NE10_MAX_ESIZE];
    ne10_int32_t prev_sy[NE10_MAX_ESIZE];
    ne10_int32_t k, dy;
    xmin *= cn;
    xmax *= cn;

    for (k = 0; k < ksize; k++)
    {
        prev_sy[k] = -1;
        rows[k] = (ne10_int32_t*) buffer_ + bufstep * k;
    }

    // image resize is a separable operation. In case of not too strong
    for (dy = 0; dy < dsth; dy++, beta += ksize)
    {
        ne10_int32_t sy0 = yofs[dy], k, k0 = ksize, k1 = 0, ksize2 = ksize / 2;

        for (k = 0; k < ksize; k++)
        {
            ne10_int32_t sy = ne10_clip (sy0 - ksize2 + 1 + k, 0, srch);
            for (k1 = NE10_MAX (k1, k); k1 < ksize; k1++)
            {
                if (sy == prev_sy[k1])  // if the sy-th row has been computed already, reuse it.
                {
                    if (k1 > k)
                        memcpy (rows[k], rows[k1], bufstep * sizeof (rows[0][0]));
                    break;
                }
            }
            if (k1 == ksize)
                k0 = NE10_MIN (k0, k); // remember the first row that needs to be computed
            srows[k] = (const ne10_uint8_t*) (src + srcstep * sy);
            prev_sy[k] = sy;
        }

        if (k0 < ksize)
        {
            if (cn == 4)
                ne10_img_hresize_4channels_linear_neon (srows + k0, rows + k0, ksize - k0, xofs, alpha,
                                                        srcw, dstw, cn, xmin, xmax);
            else
                ne10_img_hresize_linear_c (srows + k0, rows + k0, ksize - k0, xofs, alpha,
                                           srcw, dstw, cn, xmin, xmax);
        }
        ne10_img_vresize_linear_neon ( (const ne10_int32_t**) rows, (ne10_uint8_t*) (dst + dststep * dy), beta, dstw);
    }

    NE10_FREE (buffer_);
}

/**
 * @ingroup IMG_RESIZE
 * Specific implementation of @ref ne10_img_resize_bilinear_rgba using plain C.
 */
void ne10_img_resize_bilinear_rgba_c (ne10_uint8_t* dst,
                                      ne10_uint32_t dst_width,
                                      ne10_uint32_t dst_height,
                                      ne10_uint8_t* src,
                                      ne10_uint32_t src_width,
                                      ne10_uint32_t src_height,
                                      ne10_uint32_t src_stride)
{
    ne10_int32_t dstw = dst_width;
    ne10_int32_t dsth = dst_height;
    ne10_int32_t srcw = src_width;
    ne10_int32_t srch = src_height;

    ne10_int32_t cn = 4;


    ne10_int32_t xmin = 0;
    ne10_int32_t xmax = dstw;
    ne10_int32_t width = dstw * cn;

    ne10_int32_t ksize = 0, ksize2;
    ksize = 2;
    ksize2 = ksize / 2;

    ne10_uint8_t *buffer_ = (ne10_uint8_t*) NE10_MALLOC ( (width + dsth) * (sizeof (ne10_int32_t) + sizeof (ne10_float32_t) * ksize));

    ne10_int32_t* xofs = (ne10_int32_t*) buffer_;
    ne10_int32_t* yofs = xofs + width;
    ne10_int16_t* ialpha = (ne10_int16_t*) (yofs + dsth);
    ne10_int16_t* ibeta = ialpha + width * ksize;

    ne10_img_resize_cal_offset_linear (xofs, ialpha, yofs, ibeta, &xmin, &xmax, ksize, ksize2, srcw, srch, dstw, dsth, cn);

    ne10_img_resize_generic_linear_c (src, dst, xofs, ialpha, yofs, ibeta, xmin, xmax, ksize, srcw, srch, src_stride, dstw, dsth, cn);
    NE10_FREE (buffer_);
}

/**
 * @ingroup IMG_RESIZE
 * Specific implementation of @ref ne10_img_resize_bilinear_rgba using NEON SIMD capabilities.
 */
void ne10_img_resize_bilinear_rgba_neon (ne10_uint8_t* dst,
        ne10_uint32_t dst_width,
        ne10_uint32_t dst_height,
        ne10_uint8_t* src,
        ne10_uint32_t src_width,
        ne10_uint32_t src_height,
        ne10_uint32_t src_stride)
{
    ne10_int32_t dstw = dst_width;
    ne10_int32_t dsth = dst_height;
    ne10_int32_t srcw = src_width;
    ne10_int32_t srch = src_height;

    ne10_int32_t cn = 4;


    ne10_int32_t xmin = 0;
    ne10_int32_t xmax = dstw;
    ne10_int32_t width = dstw * cn;

    ne10_int32_t ksize = 0, ksize2;
    ksize = 2;
    ksize2 = ksize / 2;

    ne10_uint8_t *buffer_ = (ne10_uint8_t*) NE10_MALLOC ( (width + dsth) * (sizeof (ne10_int32_t) + sizeof (ne10_float32_t) * ksize));

    ne10_int32_t* xofs = (ne10_int32_t*) buffer_;
    ne10_int32_t* yofs = xofs + width;
    ne10_int16_t* ialpha = (ne10_int16_t*) (yofs + dsth);
    ne10_int16_t* ibeta = ialpha + width * ksize;

    ne10_img_resize_cal_offset_linear (xofs, ialpha, yofs, ibeta, &xmin, &xmax, ksize, ksize2, srcw, srch, dstw, dsth, cn);

    ne10_img_resize_generic_linear_neon (src, dst, xofs, ialpha, yofs, ibeta, xmin, xmax, ksize, srcw, srch, src_stride, dstw, dsth, cn);
    NE10_FREE (buffer_);
}

/**
 * @} end of IMG_RESIZE group
 */
