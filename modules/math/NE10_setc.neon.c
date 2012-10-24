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
 * NE10 Library : math/NE10_setc.neon.c
 */

#include "NE10_types.h"
#include "macros.h"

#include <assert.h>
#include <arm_neon.h>


ne10_result_t setc_float_neon (ne10_float32_t * dst, const ne10_float32_t cst, ne10_uint32_t count)
{
    NE10_SETC_OPERATION_FLOAT_NEON
    (
        ;// The cst need not be altered
        ,
        ;// n_tmp_cst need not be altered
    );
}

ne10_result_t setc_vec2f_neon (ne10_vec2f_t * dst, const ne10_vec2f_t * cst, ne10_uint32_t count)
{
    NE10_SETC_OPERATION_VEC2F_NEON
    (
        ;// The cst need not be altered
        ,
        ;// n_tmp_cst need not be altered
    );
}

ne10_result_t setc_vec3f_neon (ne10_vec3f_t * dst, const ne10_vec3f_t * cst, ne10_uint32_t count)
{
    NE10_SETC_OPERATION_VEC3F_NEON
    (
        ;// cst1, cst2, and cst3 need not be altered
        ,
        ;// n_tmp_cst.val[0], .val[1], and .val[2] need not be altered
    );
}

ne10_result_t setc_vec4f_neon (ne10_vec4f_t * dst, const ne10_vec4f_t * cst, ne10_uint32_t count)
{
    NE10_SETC_OPERATION_VEC4F_NEON
    (
        ;// n_cst need not be altered
    );
}
