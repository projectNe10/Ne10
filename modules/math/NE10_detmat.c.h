/*
 *  Copyright 2011-14 ARM Limited
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
 * NE10 Library : math/NE10_detmat.c.h
 */

#ifndef __NE10_DETMAT_C_H__
#define __NE10_DETMAT_C_H__

#include "NE10_types.h"
#include "macros.h"

#include <assert.h>

static inline ne10_float32_t DET2x2( ne10_mat2x2f_t * mat )
{
    // 2x2 matrix layout
    //  c1r1 c2r1
    //  c1r2 c2r2

    return (  (mat->c1.r1 * mat->c2.r2)
             -(mat->c2.r1 * mat->c1.r2) );
}

static inline ne10_float32_t DET3x3( ne10_mat3x3f_t * mat )
{
    // 3x3 matrix layout
    //  c1r1 c2r1 c3r1
    //  c1r2 c2r2 c3r2
    //  c1r3 c2r3 c3r3

    ne10_mat2x2f_t subm11 = { {mat->c2.r2, mat->c2.r3}, {mat->c3.r2, mat->c3.r3} };
    ne10_mat2x2f_t subm21 = { {mat->c1.r2, mat->c1.r3}, {mat->c3.r2, mat->c3.r3} };
    ne10_mat2x2f_t subm31 = { {mat->c1.r2, mat->c1.r3}, {mat->c2.r2, mat->c2.r3} };
    return    (mat->c1.r1*DET2x2( &subm11 ))
            - (mat->c2.r1*DET2x2( &subm21 ))
            + (mat->c3.r1*DET2x2( &subm31 ));
}

static inline ne10_float32_t DET4x4( ne10_mat4x4f_t * mat )
{
    // 4x4 matrix layout
    //  c1r1 c2r1 c3r1 c4r1
    //  c1r2 c2r2 c3r2 c4r2
    //  c1r3 c2r3 c3r3 c4r3
    //  c1r4 c2r4 c3r4 c4r4

    ne10_mat3x3f_t subm11 = { {mat->c2.r2, mat->c2.r3, mat->c2.r4},
                             {mat->c3.r2, mat->c3.r3, mat->c3.r4},
                             {mat->c4.r2, mat->c4.r3, mat->c4.r4} };

    ne10_mat3x3f_t subm21 = { {mat->c1.r2, mat->c1.r3, mat->c1.r4},
                             {mat->c3.r2, mat->c3.r3, mat->c3.r4},
                             {mat->c4.r2, mat->c4.r3, mat->c4.r4} };

    ne10_mat3x3f_t subm31 = { {mat->c1.r2, mat->c1.r3, mat->c1.r4},
                             {mat->c2.r2, mat->c2.r3, mat->c2.r4},
                             {mat->c4.r2, mat->c4.r3, mat->c4.r4} };

    ne10_mat3x3f_t subm41 = { {mat->c1.r2, mat->c1.r3, mat->c1.r4},
                             {mat->c2.r2, mat->c2.r3, mat->c2.r4},
                             {mat->c3.r2, mat->c3.r3, mat->c3.r4} };

    return    (mat->c1.r1*DET3x3( &subm11 ))
            - (mat->c2.r1*DET3x3( &subm21 ))
            + (mat->c3.r1*DET3x3( &subm31 ))
            - (mat->c4.r1*DET3x3( &subm41 ));
}




#endif
