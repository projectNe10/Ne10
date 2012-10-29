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
 * NE10 Library : math/NE10_mulcmatvec.neon.s
 */

#include "NE10_types.h"
#include "macros.h"

#include <assert.h>

ne10_result_t mulcmatvec_cm2x2f_v2f_c (ne10_vec2f_t * dst, const ne10_mat2x2f_t * cst, ne10_vec2f_t * src, ne10_uint32_t count)
{
#define A1 cst->c1.r1
#define B1 cst->c1.r2
#define C1 cst->c2.r1
#define D1 cst->c2.r2

    NE10_CMATVEC_OPERATION_X_C
    (
        dst[ itr ].x = A1 * src[ itr ].x + C1 * src[ itr ].y;
        dst[ itr ].y = B1 * src[ itr ].x + D1 * src[ itr ].y;
    );

#undef A1
#undef B1
#undef C1
#undef D1
}

ne10_result_t mulcmatvec_cm3x3f_v3f_c (ne10_vec3f_t * dst, const ne10_mat3x3f_t * cst, ne10_vec3f_t * src, ne10_uint32_t count)
{
#define A1 cst->c1.r1
#define B1 cst->c1.r2
#define C1 cst->c1.r3
#define D1 cst->c2.r1
#define E1 cst->c2.r2
#define F1 cst->c2.r3
#define G1 cst->c3.r1
#define H1 cst->c3.r2
#define I1 cst->c3.r3

    NE10_CMATVEC_OPERATION_X_C
    (
        dst[ itr ].x = A1 * src[ itr ].x + D1 * src[ itr ].y + G1 * src[ itr ].z;
        dst[ itr ].y = B1 * src[ itr ].x + E1 * src[ itr ].y + H1 * src[ itr ].z;
        dst[ itr ].z = C1 * src[ itr ].x + F1 * src[ itr ].y + I1 * src[ itr ].z;
    );

#undef A1
#undef B1
#undef C1
#undef D1
#undef E1
#undef F1
#undef G1
#undef H1
#undef I1
}

extern ne10_result_t mulcmatvec_cm4x4f_v4f_c (ne10_vec4f_t * dst, const ne10_mat4x4f_t * cst, ne10_vec4f_t * src, ne10_uint32_t count)
{
#define A1 cst->c1.r1
#define B1 cst->c1.r2
#define C1 cst->c1.r3
#define D1 cst->c1.r4
#define E1 cst->c2.r1
#define F1 cst->c2.r2
#define G1 cst->c2.r3
#define H1 cst->c2.r4
#define I1 cst->c3.r1
#define J1 cst->c3.r2
#define K1 cst->c3.r3
#define L1 cst->c3.r4
#define M1 cst->c4.r1
#define N1 cst->c4.r2
#define O1 cst->c4.r3
#define P1 cst->c4.r4

    NE10_CMATVEC_OPERATION_X_C
    (
        dst[ itr ].x = A1 * src[ itr ].x + E1 * src[ itr ].y + I1 * src[ itr ].z + M1 * src[ itr ].w;
        dst[ itr ].y = B1 * src[ itr ].x + F1 * src[ itr ].y + J1 * src[ itr ].z + N1 * src[ itr ].w;
        dst[ itr ].z = C1 * src[ itr ].x + G1 * src[ itr ].y + K1 * src[ itr ].z + O1 * src[ itr ].w;
        dst[ itr ].w = D1 * src[ itr ].x + H1 * src[ itr ].y + L1 * src[ itr ].z + P1 * src[ itr ].w;
    );

#undef A1
#undef B1
#undef C1
#undef D1
#undef E1
#undef F1
#undef G1
#undef H1
#undef I1
#undef J1
#undef K1
#undef L1
#undef M1
#undef N1
#undef O1
#undef P1
}
