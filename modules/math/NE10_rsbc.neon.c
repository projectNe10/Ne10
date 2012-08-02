/*
 *  Copyright 2011-12 ARM Limited
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/*
 * NE10 Library : math/NE10_rsbc.neon.c
 */

#include "NE10_types.h"
#include "macros.h"

#include <assert.h>
#include <arm_neon.h>


ne10_result_t rsbc_float_neon (ne10_float32_t * dst, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count)
{
    NE10_XC_OPERATION_FLOAT_NEON
    (
        n_dst = vsubq_f32 (n_cst, n_src);
        ,
        n_tmp_src = vsub_f32 (n_tmp_cst, n_tmp_src);
    );
}

ne10_result_t rsbc_vec2f_neon (ne10_vec2f_t * dst, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count)
{
    NE10_XC_OPERATION_VEC2F_NEON
    (
        n_dst = vsubq_f32 (n_cst, n_src);
        ,
        n_tmp_src = vsub_f32 (n_tmp_cst, n_tmp_src);
    );
}

ne10_result_t rsbc_vec3f_neon (ne10_vec3f_t * dst, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count)
{
    NE10_XC_OPERATION_VEC3F_NEON
    (
        n_dst1 = vsubq_f32 (n_cst1, n_src1);
        n_dst2 = vsubq_f32 (n_cst2, n_src2);
        n_dst3 = vsubq_f32 (n_cst3, n_src3);
        ,
        n_tmp_src.val[0] = vsub_f32 (n_tmp_cst.val[0], n_tmp_src.val[0]);
        n_tmp_src.val[1] = vsub_f32 (n_tmp_cst.val[1], n_tmp_src.val[1]);
        n_tmp_src.val[2] = vsub_f32 (n_tmp_cst.val[2], n_tmp_src.val[2]);
    );
}

ne10_result_t rsbc_vec4f_neon (ne10_vec4f_t * dst, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count)
{
    NE10_XC_OPERATION_VEC4F_NEON
    (
        n_dst = vsubq_f32 (n_cst, n_src);
    );
}
