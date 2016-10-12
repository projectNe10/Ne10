/*
 *  Copyright 2011-15 ARM Limited and Contributors.
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
 * NE10 Library : math/NE10_divc.neon.c
 */

#include "NE10_types.h"
#include "macros.h"

#include <assert.h>
#include <arm_neon.h>

#include <stdio.h>
#include <stdlib.h>

ne10_result_t ne10_divc_float_neon (ne10_float32_t * dst, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count)
{
    NE10_XC_OPERATION_FLOAT_NEON
    (
        /* a single division operation */
        float32x4_t rec = vrecpeq_f32 (n_cst);
        rec = vmulq_f32 (vrecpsq_f32 (n_cst, rec), rec);
        rec = vmulq_f32 (vrecpsq_f32 (n_cst, rec), rec);
        n_dst = vmulq_f32 (n_src , rec);
        ,
        /* a single division operation */
        float32x2_t rec = vrecpe_f32 (n_tmp_cst);
        rec = vmul_f32 (vrecps_f32 (n_tmp_cst, rec), rec);
        rec = vmul_f32 (vrecps_f32 (n_tmp_cst, rec), rec);
        n_tmp_src = vmul_f32 (n_tmp_src, rec);
    );
}

ne10_result_t ne10_divc_vec2f_neon (ne10_vec2f_t * dst, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count)
{
    NE10_XC_OPERATION_VEC2F_NEON
    (
        /* a single division operation */
        float32x4_t rec = vrecpeq_f32 (n_cst);
        rec = vmulq_f32 (vrecpsq_f32 (n_cst, rec), rec);
        rec = vmulq_f32 (vrecpsq_f32 (n_cst, rec), rec);
        n_dst = vmulq_f32 (n_src , rec);
        ,
        /* a single division operation */
        float32x2_t rec = vrecpe_f32 (n_tmp_cst);
        rec = vmul_f32 (vrecps_f32 (n_tmp_cst, rec), rec);
        rec = vmul_f32 (vrecps_f32 (n_tmp_cst, rec), rec);
        n_tmp_src = vmul_f32 (n_tmp_src, rec);
    );
}

ne10_result_t ne10_divc_vec3f_neon (ne10_vec3f_t * dst, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count)
{
    NE10_XC_OPERATION_VEC3F_NEON
    (
        /* three division operations */
        float32x4_t rec = vrecpeq_f32 (n_cst1);
        rec = vmulq_f32 (vrecpsq_f32 (n_cst1, rec), rec);
        rec = vmulq_f32 (vrecpsq_f32 (n_cst1, rec), rec);
        n_dst1 = vmulq_f32 (n_src1 , rec);

        rec = vrecpeq_f32 (n_cst2);
        rec = vmulq_f32 (vrecpsq_f32 (n_cst2, rec), rec);
        rec = vmulq_f32 (vrecpsq_f32 (n_cst2, rec), rec);
        n_dst2 = vmulq_f32 (n_src2 , rec);

        rec = vrecpeq_f32 (n_cst3);
        rec = vmulq_f32 (vrecpsq_f32 (n_cst3, rec), rec);
        rec = vmulq_f32 (vrecpsq_f32 (n_cst3, rec), rec);
        n_dst3 = vmulq_f32 (n_src3 , rec);
        ,
        /* three division operations */
        float32x2_t rec = vrecpe_f32 (n_tmp_cst.val[0]);
        rec = vmul_f32 (vrecps_f32 (n_tmp_cst.val[0], rec), rec);
        rec = vmul_f32 (vrecps_f32 (n_tmp_cst.val[0], rec), rec);
        n_tmp_src.val[0] = vmul_f32 (n_tmp_src.val[0] , rec);

        rec = vrecpe_f32 (n_tmp_cst.val[1]);
        rec = vmul_f32 (vrecps_f32 (n_tmp_cst.val[1], rec), rec);
        rec = vmul_f32 (vrecps_f32 (n_tmp_cst.val[1], rec), rec);
        n_tmp_src.val[1] = vmul_f32 (n_tmp_src.val[1] , rec);

        rec = vrecpe_f32 (n_tmp_cst.val[2]);
        rec = vmul_f32 (vrecps_f32 (n_tmp_cst.val[2], rec), rec);
        rec = vmul_f32 (vrecps_f32 (n_tmp_cst.val[2], rec), rec);
        n_tmp_src.val[2] = vmul_f32 (n_tmp_src.val[2] , rec);
    );
}

ne10_result_t ne10_divc_vec4f_neon (ne10_vec4f_t * dst, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count)
{
    NE10_XC_OPERATION_VEC4F_NEON
    (
        /* a single division operation */
        float32x4_t rec = vrecpeq_f32 (n_cst);
        rec = vmulq_f32 (vrecpsq_f32 (n_cst, rec), rec);
        rec = vmulq_f32 (vrecpsq_f32 (n_cst, rec), rec);
        n_dst = vmulq_f32 (n_src , rec);
    );
}
