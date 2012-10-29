/*
 *  Copyright 2011-12 ARM Limited
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
 * NE10 Library : math/NE10_mlac.neon.c
 */

#include "NE10_types.h"
#include "macros.h"

#include <assert.h>
#include <arm_neon.h>


ne10_result_t ne10_mlac_float_neon (ne10_float32_t * dst, ne10_float32_t * acc, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count)
{
    NE10_MLAC_OPERATION_FLOAT_NEON
    (
        n_dst = vmlaq_f32 (n_acc, n_src, n_cst);
        ,
        n_tmp_src = vmla_f32 (n_tmp_acc, n_tmp_src, n_tmp_cst);
    );
}

ne10_result_t ne10_mlac_vec2f_neon (ne10_vec2f_t * dst, ne10_vec2f_t * acc, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count)
{
    NE10_MLAC_OPERATION_VEC2F_NEON
    (
        n_dst = vmlaq_f32 (n_acc, n_src , n_cst);
        ,
        n_tmp_src = vmla_f32 (n_tmp_acc, n_tmp_src, n_tmp_cst);
    );
}

ne10_result_t ne10_mlac_vec3f_neon (ne10_vec3f_t * dst, ne10_vec3f_t * acc, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count)
{
    NE10_MLAC_OPERATION_VEC3F_NEON
    (
        n_dst1 = vmlaq_f32 (n_acc1, n_src1 , n_cst1);
        n_dst2 = vmlaq_f32 (n_acc2, n_src2 , n_cst2);
        n_dst3 = vmlaq_f32 (n_acc3, n_src3 , n_cst3);
        ,
        n_tmp_src.val[0] = vmla_f32 (n_tmp_acc.val[0], n_tmp_src.val[0], n_tmp_cst.val[0]);   /* the X lane */
        n_tmp_src.val[1] = vmla_f32 (n_tmp_acc.val[1], n_tmp_src.val[1], n_tmp_cst.val[1]);   /* the Y lane */
        n_tmp_src.val[2] = vmla_f32 (n_tmp_acc.val[2], n_tmp_src.val[2], n_tmp_cst.val[2]);   /* the Z lane */
    );
}

ne10_result_t ne10_mlac_vec4f_neon (ne10_vec4f_t * dst, ne10_vec4f_t * acc, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count)
{
    NE10_MLAC_OPERATION_VEC4F_NEON
    (
        n_dst = vmlaq_f32 (n_acc, n_src , n_cst);
    );
}
