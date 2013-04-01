/*
 *  Copyright 2013 ARM Limited
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

#include <stdio.h>

#include "NE10_imgproc.h"

ne10_result_t ne10_init_imgproc (ne10_int32_t is_NEON_available)
{
    if (NE10_OK == is_NEON_available)
    {
        ne10_vresize = ne10_vresize_neon;
        ne10_hresize_4channels = ne10_hresize_4channels_neon;
        ne10_img_rotate_get_quad_rangle_subpix = ne10_img_rotate_get_quad_rangle_subpix_neon;
    }
    else
    {
        ;
    }
    return NE10_OK;
}

// These are actual definitions of our function pointers that are declared in inc/NE10_imgproc.h
void (*ne10_vresize) (const ne10_int32_t** src,
                      ne10_uint8_t* dst,
                      const ne10_int16_t* beta,
                      ne10_int32_t width);
void (*ne10_hresize_4channels) (const ne10_uint8_t** src,
                                ne10_int32_t** dst,
                                ne10_int32_t count,
                                const ne10_int32_t* xofs,
                                const ne10_int16_t* alpha,
                                ne10_int32_t swidth,
                                ne10_int32_t dwidth,
                                ne10_int32_t cn,
                                ne10_int32_t xmin,
                                ne10_int32_t xmax);

void (*ne10_img_rotate_get_quad_rangle_subpix) (ne10_uint8_t* dst,
        const ne10_uint8_t* src,
        ne10_int32_t swidth,
        ne10_int32_t sheight,
        ne10_int32_t dwidth,
        ne10_int32_t dheight,
        ne10_float32_t* matrix);
