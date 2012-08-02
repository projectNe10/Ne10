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

#include "NE10_types.h"

ne10_result_t addmat_2x2f_neon (ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src1, ne10_mat2x2f_t * src2, ne10_uint32_t count)
{
    return add_vec2f_neon ( (ne10_vec2f_t*) dst, (ne10_vec2f_t*) src1, (ne10_vec2f_t*) src2, count * 2);
}

ne10_result_t addmat_3x3f_neon (ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src1, ne10_mat3x3f_t * src2, ne10_uint32_t count)
{
    return add_vec3f_neon ( (ne10_vec3f_t*) dst, (ne10_vec3f_t*) src1, (ne10_vec3f_t*) src2, count * 3);
}

ne10_result_t addmat_4x4f_neon (ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src1, ne10_mat4x4f_t * src2, ne10_uint32_t count)
{
    return add_vec4f_neon ( (ne10_vec4f_t*) dst, (ne10_vec4f_t*) src1, (ne10_vec4f_t*) src2, count * 4);
}

