/*
 *  Copyright 2012 ARM Limited
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
 * NE10 Library : math/NE10_invmat.c
 */

#include "NE10_types.h"
#include "macros.h"
#include "NE10_detmat.c.h"
#include <math.h>

#include <assert.h>

// This macro is used to determine floating point values that are small enough to be consiedered nearly zero
#define IS_FLOAT_NEAR_ZERO(x) ( ((fabs(x))<(1e-12)) ? 1 : 0 )

ne10_result_t invmat_2x2f_c (ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src, ne10_uint32_t count)
{
    ne10_float32_t det = 0.0f;

    NE10_DETMAT_OPERATION_X_C
    (
        det = DET2x2 (&src[ itr ]);

        if (1 == IS_FLOAT_NEAR_ZERO (det))
        {
            det = 1.0f;
        }

        det = 1.0f / det;
        dst[ itr ].c1.r1 =         det * src[ itr ].c2.r2;
        dst[ itr ].c1.r2 =    -1 * det * src[ itr ].c1.r2;
        dst[ itr ].c2.r1 =    -1 * det * src[ itr ].c2.r1;
        dst[ itr ].c2.r2 =         det * src[ itr ].c1.r1;
    );
}

ne10_result_t invmat_3x3f_c (ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src, ne10_uint32_t count)
{
#define aa   (src[ itr ].c1.r1)
#define bb   (src[ itr ].c1.r2)
#define cc   (src[ itr ].c1.r3)
#define dd   (src[ itr ].c2.r1)
#define ee   (src[ itr ].c2.r2)
#define ff   (src[ itr ].c2.r3)
#define gg   (src[ itr ].c3.r1)
#define hh   (src[ itr ].c3.r2)
#define ii   (src[ itr ].c3.r3)

    ne10_float32_t det = 0.0f;
    ne10_mat2x2f_t A, B, C, D, E, F, G, H, I;

    NE10_DETMAT_OPERATION_X_C
    (
        det = DET3x3 (&src[ itr ]);

        if (1 == IS_FLOAT_NEAR_ZERO (det))
        {
            det = 1.0f;
        }
        det = 1.0f / det;

        // Calculate the coefficients
        createColumnMajorMatrix2x2 (&A, ee, ff, hh, ii);
        createColumnMajorMatrix2x2 (&B, dd, ff, gg, ii);
        createColumnMajorMatrix2x2 (&C, dd, ee, gg, hh);
        createColumnMajorMatrix2x2 (&D, bb, cc, hh, ii);
        createColumnMajorMatrix2x2 (&E, aa, cc, gg, ii);
        createColumnMajorMatrix2x2 (&F, aa, bb, gg, hh);
        createColumnMajorMatrix2x2 (&G, bb, cc, ee, ff);
        createColumnMajorMatrix2x2 (&H, aa, cc, dd, ff);
        createColumnMajorMatrix2x2 (&I, aa, bb, dd, ee);

        dst[ itr ].c1.r1 =         det * DET2x2 (&A);
        dst[ itr ].c1.r2 = -1.0f * det * DET2x2 (&D);
        dst[ itr ].c1.r3 =         det * DET2x2 (&G);

        dst[ itr ].c2.r1 = -1.0f * det * DET2x2 (&B);
        dst[ itr ].c2.r2 =         det * DET2x2 (&E);
        dst[ itr ].c2.r3 = -1.0f * det * DET2x2 (&H);

        dst[ itr ].c3.r1 =         det * DET2x2 (&C);
        dst[ itr ].c3.r2 = -1.0f * det * DET2x2 (&F);
        dst[ itr ].c3.r3 =         det * DET2x2 (&I);
    );

#undef aa
#undef bb
#undef cc
#undef dd
#undef ee
#undef ff
#undef gg
#undef hh
#undef ii
}

ne10_result_t invmat_4x4f_c (ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src, ne10_uint32_t count)
{
#define aa   (src[ itr ].c1.r1)
#define bb   (src[ itr ].c1.r2)
#define cc   (src[ itr ].c1.r3)
#define dd   (src[ itr ].c1.r4)

#define ee   (src[ itr ].c2.r1)
#define ff   (src[ itr ].c2.r2)
#define gg   (src[ itr ].c2.r3)
#define hh   (src[ itr ].c2.r4)

#define ii   (src[ itr ].c3.r1)
#define jj   (src[ itr ].c3.r2)
#define kk   (src[ itr ].c3.r3)
#define ll   (src[ itr ].c3.r4)

#define mm   (src[ itr ].c4.r1)
#define nn   (src[ itr ].c4.r2)
#define oo   (src[ itr ].c4.r3)
#define pp   (src[ itr ].c4.r4)

    ne10_float32_t det = 0.0f;
    ne10_mat3x3f_t A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P;

    NE10_DETMAT_OPERATION_X_C
    (
        det = DET4x4 (&src[ itr ]);

        if (1 == IS_FLOAT_NEAR_ZERO (det))
        {
            det = 1.0f;
        }
        det = 1.0f / det;

        // Calculate the coefficients
        createColumnMajorMatrix3x3 (&A, ff, gg, hh, jj, kk, ll, nn, oo, pp);
        createColumnMajorMatrix3x3 (&B, ee, gg, hh, ii, kk, ll, mm, oo, pp);
        createColumnMajorMatrix3x3 (&C, ee, ff, hh, ii, jj, ll, mm, nn, pp);
        createColumnMajorMatrix3x3 (&D, ee, ff, gg, ii, jj, kk, mm, nn, oo);
        createColumnMajorMatrix3x3 (&E, bb, cc, dd, jj, kk, ll, nn, oo, pp);
        createColumnMajorMatrix3x3 (&F, aa, cc, dd, ii, kk, ll, mm, oo, pp);
        createColumnMajorMatrix3x3 (&G, aa, bb, dd, ii, jj, ll, mm, nn, pp);
        createColumnMajorMatrix3x3 (&H, aa, bb, cc, ii, jj, kk, mm, nn, oo);
        createColumnMajorMatrix3x3 (&I, bb, cc, dd, ff, gg, hh, nn, oo, pp);
        createColumnMajorMatrix3x3 (&J, aa, cc, dd, ee, gg, hh, mm, oo, pp);
        createColumnMajorMatrix3x3 (&K, aa, bb, dd, ee, ff, hh, mm, nn, pp);
        createColumnMajorMatrix3x3 (&L, aa, bb, cc, ee, ff, gg, mm, nn, oo);
        createColumnMajorMatrix3x3 (&M, bb, cc, dd, ff, gg, hh, jj, kk, ll);
        createColumnMajorMatrix3x3 (&N, aa, cc, dd, ee, gg, hh, ii, kk, ll);
        createColumnMajorMatrix3x3 (&O, aa, bb, dd, ee, ff, hh, ii, jj, ll);
        createColumnMajorMatrix3x3 (&P, aa, bb, cc, ee, ff, gg, ii, jj, kk);


        dst[ itr ].c1.r1 =         det * DET3x3 (&A);
        dst[ itr ].c1.r2 = -1.0f * det * DET3x3 (&E);
        dst[ itr ].c1.r3 =         det * DET3x3 (&I);
        dst[ itr ].c1.r4 = -1.0f * det * DET3x3 (&M);

        dst[ itr ].c2.r1 = -1.0f * det * DET3x3 (&B);
        dst[ itr ].c2.r2 =         det * DET3x3 (&F);
        dst[ itr ].c2.r3 = -1.0f * det * DET3x3 (&J);
        dst[ itr ].c2.r4 =         det * DET3x3 (&N);

        dst[ itr ].c3.r1 =         det * DET3x3 (&C);
        dst[ itr ].c3.r2 = -1.0f * det * DET3x3 (&G);
        dst[ itr ].c3.r3 =         det * DET3x3 (&K);
        dst[ itr ].c3.r4 = -1.0f * det * DET3x3 (&O);

        dst[ itr ].c4.r1 = -1.0f * det * DET3x3 (&D);
        dst[ itr ].c4.r2 =         det * DET3x3 (&H);
        dst[ itr ].c4.r3 = -1.0f * det * DET3x3 (&L);
        dst[ itr ].c4.r4 =         det * DET3x3 (&P);
    );

#undef aa
#undef bb
#undef cc
#undef dd
#undef ee
#undef ff
#undef gg
#undef hh
#undef ii
#undef jj
#undef kk
#undef ll
#undef mm
#undef nn
#undef oo
#undef pp
}
