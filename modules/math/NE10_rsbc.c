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
 * NE10 Library : math/NE10_rsbc.c
 */

#include "NE10_types.h"
#include "macros.h"

#include <assert.h>

ne10_result_t rsbc_float_c (ne10_float32_t * dst, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count)
{
    NE10_XC_OPERATION_X_C
    (
        dst[ itr ] = cst - src[ itr ];
    );
}

ne10_result_t rsbc_vec2f_c (ne10_vec2f_t * dst, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count)
{
    NE10_XC_OPERATION_X_C
    (
        dst[ itr ].x = cst->x - src[ itr ].x;
        dst[ itr ].y = cst->y - src[ itr ].y;
    );
}

ne10_result_t rsbc_vec3f_c (ne10_vec3f_t * dst, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count)
{
    NE10_XC_OPERATION_X_C
    (
        dst[ itr ].x = cst->x - src[ itr ].x;
        dst[ itr ].y = cst->y - src[ itr ].y;
        dst[ itr ].z = cst->z - src[ itr ].z;
    );
}

ne10_result_t rsbc_vec4f_c (ne10_vec4f_t * dst, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count)
{
    NE10_XC_OPERATION_X_C
    (
        dst[ itr ].x = cst->x - src[ itr ].x;
        dst[ itr ].y = cst->y - src[ itr ].y;
        dst[ itr ].z = cst->z - src[ itr ].z;
        dst[ itr ].w = cst->w - src[ itr ].w;
    );
}
