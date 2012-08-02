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
 * NE10 Library : math/NE10_addmat.c
 */

#include "NE10_types.h"
#include "macros.h"
#include <math.h>

#include <assert.h>

inline void swap (ne10_float32_t *a, ne10_float32_t *b)
{
    ne10_float32_t tmp = *a;
    *a = *b;
    *b = tmp;
}

ne10_result_t transmat_2x2f_c (ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src, ne10_uint32_t count)
{
    NE10_DETMAT_OPERATION_X_C
    (
        dst[ itr ].c1.r1 =  src[ itr ].c1.r1;
        dst[ itr ].c1.r2 =  src[ itr ].c2.r1;
        dst[ itr ].c2.r1 =  src[ itr ].c1.r2;
        dst[ itr ].c2.r2 =  src[ itr ].c2.r2;
    );
}

ne10_result_t transmat_3x3f_c (ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src, ne10_uint32_t count)
{
    NE10_DETMAT_OPERATION_X_C
    (
        dst[ itr ].c1.r1 =  src[ itr ].c1.r1;
        dst[ itr ].c1.r2 =  src[ itr ].c2.r1;
        dst[ itr ].c1.r3 =  src[ itr ].c3.r1;

        dst[ itr ].c2.r1 =  src[ itr ].c1.r2;
        dst[ itr ].c2.r2 =  src[ itr ].c2.r2;
        dst[ itr ].c2.r3 =  src[ itr ].c3.r2;

        dst[ itr ].c3.r1 =  src[ itr ].c1.r3;
        dst[ itr ].c3.r2 =  src[ itr ].c2.r3;
        dst[ itr ].c3.r3 =  src[ itr ].c3.r3;
    );
}

ne10_result_t transmat_4x4f_c (ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src, ne10_uint32_t count)
{
    NE10_DETMAT_OPERATION_X_C
    (
        dst[ itr ].c1.r1 =  src[ itr ].c1.r1;
        dst[ itr ].c1.r2 =  src[ itr ].c2.r1;
        dst[ itr ].c1.r3 =  src[ itr ].c3.r1;
        dst[ itr ].c1.r4 =  src[ itr ].c4.r1;

        dst[ itr ].c2.r1 =  src[ itr ].c1.r2;
        dst[ itr ].c2.r2 =  src[ itr ].c2.r2;
        dst[ itr ].c2.r3 =  src[ itr ].c3.r2;
        dst[ itr ].c2.r4 =  src[ itr ].c4.r2;

        dst[ itr ].c3.r1 =  src[ itr ].c1.r3;
        dst[ itr ].c3.r2 =  src[ itr ].c2.r3;
        dst[ itr ].c3.r3 =  src[ itr ].c3.r3;
        dst[ itr ].c3.r4 =  src[ itr ].c4.r3;

        dst[ itr ].c4.r1 =  src[ itr ].c1.r4;
        dst[ itr ].c4.r2 =  src[ itr ].c2.r4;
        dst[ itr ].c4.r3 =  src[ itr ].c3.r4;
        dst[ itr ].c4.r4 =  src[ itr ].c4.r4;
    );
}
