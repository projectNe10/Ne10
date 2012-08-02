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
 * NE10 Library : math/NE10_addc.neon.c
 */

#include <assert.h>
#include <arm_neon.h>

#include "NE10.h"
#include "macros.h"


ne10_result_t addc_float_neon (ne10_float32_t * dst, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count)
{
    NE10_XC_OPERATION_FLOAT_NEON
    (
        n_dst = vaddq_f32 (n_src , n_cst);
        ,
        n_tmp_src = vadd_f32 (n_tmp_src, n_tmp_cst);
    );
}

ne10_result_t addc_vec2f_neon (ne10_vec2f_t * dst, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count)
{
    NE10_XC_OPERATION_VEC2F_NEON
    (
        n_dst = vaddq_f32 (n_src , n_cst);
        ,
        n_tmp_src = vadd_f32 (n_tmp_src, n_tmp_cst);
    );
}

ne10_result_t addc_vec3f_neon (ne10_vec3f_t * dst, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count)
{
    NE10_XC_OPERATION_VEC3F_NEON
    (
        n_dst1 = vaddq_f32 (n_src1 , n_cst1);
        n_dst2 = vaddq_f32 (n_src2 , n_cst2);
        n_dst3 = vaddq_f32 (n_src3 , n_cst3);
        ,
        n_tmp_src.val[0] = vadd_f32 (n_tmp_src.val[0], n_tmp_cst.val[0]);   /* the X lane */
        n_tmp_src.val[1] = vadd_f32 (n_tmp_src.val[1], n_tmp_cst.val[1]);   /* the Y lane */
        n_tmp_src.val[2] = vadd_f32 (n_tmp_src.val[2], n_tmp_cst.val[2]);   /* the Z lane */
    );
}

ne10_result_t addc_vec4f_neon (ne10_vec4f_t * dst, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count)
{
    NE10_XC_OPERATION_VEC4F_NEON
    (
        n_dst = vaddq_f32 (n_src , n_cst);
    );
}
