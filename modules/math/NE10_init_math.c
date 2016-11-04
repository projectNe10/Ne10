/*
 *  Copyright 2011-16 ARM Limited and Contributors.
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

#include <stdio.h>

#include "NE10_math.h"

ne10_result_t ne10_init_math (int is_NEON_available)
{
    if (NE10_OK == is_NEON_available)
    {
        ne10_addc_float = ne10_addc_float_neon;
        ne10_addc_vec2f = ne10_addc_vec2f_neon;
        ne10_addc_vec3f = ne10_addc_vec3f_neon;
        ne10_addc_vec4f = ne10_addc_vec4f_neon;
        ne10_subc_float = ne10_subc_float_neon;
        ne10_subc_vec2f = ne10_subc_vec2f_neon;
        ne10_subc_vec3f = ne10_subc_vec3f_neon;
        ne10_subc_vec4f = ne10_subc_vec4f_neon;
        ne10_rsbc_float = ne10_rsbc_float_neon;
        ne10_rsbc_vec2f = ne10_rsbc_vec2f_neon;
        ne10_rsbc_vec3f = ne10_rsbc_vec3f_neon;
        ne10_rsbc_vec4f = ne10_rsbc_vec4f_neon;
        ne10_mulc_float = ne10_mulc_float_neon;
        ne10_mulc_vec2f = ne10_mulc_vec2f_neon;
        ne10_mulc_vec3f = ne10_mulc_vec3f_neon;
        ne10_mulc_vec4f = ne10_mulc_vec4f_neon;
        ne10_divc_float = ne10_divc_float_neon;
        ne10_divc_vec2f = ne10_divc_vec2f_neon;
        ne10_divc_vec3f = ne10_divc_vec3f_neon;
        ne10_divc_vec4f = ne10_divc_vec4f_neon;
        ne10_setc_float = ne10_setc_float_neon;
        ne10_setc_vec2f = ne10_setc_vec2f_neon;
        ne10_setc_vec3f = ne10_setc_vec3f_neon;
        ne10_setc_vec4f = ne10_setc_vec4f_neon;
        ne10_mlac_float = ne10_mlac_float_neon;
        ne10_mlac_vec2f = ne10_mlac_vec2f_neon;
        ne10_mlac_vec3f = ne10_mlac_vec3f_neon;
        ne10_mlac_vec4f = ne10_mlac_vec4f_neon;
        ne10_add_float = ne10_add_float_neon;
        ne10_sub_float = ne10_sub_float_neon;
        ne10_mul_float = ne10_mul_float_neon;
        ne10_div_float = ne10_div_float_neon;
        ne10_mla_float = ne10_mla_float_neon;
        ne10_abs_float = ne10_abs_float_neon;
        ne10_len_vec2f = ne10_len_vec2f_neon;
        ne10_len_vec3f = ne10_len_vec3f_neon;
        ne10_len_vec4f = ne10_len_vec4f_neon;
        ne10_normalize_vec2f = ne10_normalize_vec2f_neon;
        ne10_normalize_vec3f = ne10_normalize_vec3f_neon;
        ne10_normalize_vec4f = ne10_normalize_vec4f_neon;

        ne10_abs_vec2f = ne10_abs_vec2f_neon;
        ne10_abs_vec3f = ne10_abs_vec3f_neon;
        ne10_abs_vec4f = ne10_abs_vec4f_neon;
        ne10_vmul_vec2f = ne10_vmul_vec2f_neon;
        ne10_vmul_vec3f = ne10_vmul_vec3f_neon;
        ne10_vmul_vec4f = ne10_vmul_vec4f_neon;
        ne10_vdiv_vec2f = ne10_vdiv_vec2f_neon;
        ne10_vdiv_vec3f = ne10_vdiv_vec3f_neon;
        ne10_vdiv_vec4f = ne10_vdiv_vec4f_neon;
        ne10_vmla_vec2f = ne10_vmla_vec2f_neon;
        ne10_vmla_vec3f = ne10_vmla_vec3f_neon;
        ne10_vmla_vec4f = ne10_vmla_vec4f_neon;
        ne10_add_vec2f = ne10_add_vec2f_neon;
        ne10_add_vec3f = ne10_add_vec3f_neon;
        ne10_add_vec4f = ne10_add_vec4f_neon;
        ne10_sub_vec2f = ne10_sub_vec2f_neon;
        ne10_sub_vec3f = ne10_sub_vec3f_neon;
        ne10_sub_vec4f = ne10_sub_vec4f_neon;
        ne10_dot_vec2f = ne10_dot_vec2f_neon;
        ne10_dot_vec3f = ne10_dot_vec3f_neon;
        ne10_dot_vec4f = ne10_dot_vec4f_neon;
        ne10_cross_vec3f = ne10_cross_vec3f_neon;

        ne10_addmat_2x2f = ne10_addmat_2x2f_neon;
        ne10_addmat_3x3f = ne10_addmat_3x3f_neon;
        ne10_addmat_4x4f = ne10_addmat_4x4f_neon;
        ne10_submat_2x2f = ne10_submat_2x2f_neon;
        ne10_submat_3x3f = ne10_submat_3x3f_neon;
        ne10_submat_4x4f = ne10_submat_4x4f_neon;
        ne10_mulmat_2x2f = ne10_mulmat_2x2f_neon;
        ne10_mulmat_3x3f = ne10_mulmat_3x3f_neon;
        ne10_mulmat_4x4f = ne10_mulmat_4x4f_neon;
        ne10_mulcmatvec_cm2x2f_v2f = ne10_mulcmatvec_cm2x2f_v2f_neon;
        ne10_mulcmatvec_cm3x3f_v3f = ne10_mulcmatvec_cm3x3f_v3f_neon;
        ne10_mulcmatvec_cm4x4f_v4f = ne10_mulcmatvec_cm4x4f_v4f_neon;
        ne10_detmat_2x2f = ne10_detmat_2x2f_neon;
        ne10_detmat_3x3f = ne10_detmat_3x3f_neon;
        ne10_detmat_4x4f = ne10_detmat_4x4f_neon;
        ne10_invmat_2x2f = ne10_invmat_2x2f_neon;
        ne10_invmat_3x3f = ne10_invmat_3x3f_neon;
        ne10_invmat_4x4f = ne10_invmat_4x4f_neon;
        ne10_transmat_4x4f = ne10_transmat_4x4f_neon;
        ne10_identitymat_4x4f = ne10_identitymat_4x4f_neon;
        ne10_transmat_3x3f = ne10_transmat_3x3f_neon;
        ne10_identitymat_3x3f = ne10_identitymat_3x3f_neon;
        ne10_transmat_2x2f = ne10_transmat_2x2f_neon;
        ne10_identitymat_2x2f = ne10_identitymat_2x2f_neon;
    }
    else
    {
        ne10_addc_float = ne10_addc_float_c;
        ne10_addc_vec2f = ne10_addc_vec2f_c;
        ne10_addc_vec3f = ne10_addc_vec3f_c;
        ne10_addc_vec4f = ne10_addc_vec4f_c;
        ne10_subc_float = ne10_subc_float_c;
        ne10_subc_vec2f = ne10_subc_vec2f_c;
        ne10_subc_vec3f = ne10_subc_vec3f_c;
        ne10_subc_vec4f = ne10_subc_vec4f_c;
        ne10_rsbc_float = ne10_rsbc_float_c;
        ne10_rsbc_vec2f = ne10_rsbc_vec2f_c;
        ne10_rsbc_vec3f = ne10_rsbc_vec3f_c;
        ne10_rsbc_vec4f = ne10_rsbc_vec4f_c;
        ne10_mulc_float = ne10_mulc_float_c;
        ne10_mulc_vec2f = ne10_mulc_vec2f_c;
        ne10_mulc_vec3f = ne10_mulc_vec3f_c;
        ne10_mulc_vec4f = ne10_mulc_vec4f_c;
        ne10_divc_float = ne10_divc_float_c;
        ne10_divc_vec2f = ne10_divc_vec2f_c;
        ne10_divc_vec3f = ne10_divc_vec3f_c;
        ne10_divc_vec4f = ne10_divc_vec4f_c;
        ne10_setc_float = ne10_setc_float_c;
        ne10_setc_vec2f = ne10_setc_vec2f_c;
        ne10_setc_vec3f = ne10_setc_vec3f_c;
        ne10_setc_vec4f = ne10_setc_vec4f_c;
        ne10_mlac_float = ne10_mlac_float_c;
        ne10_mlac_vec2f = ne10_mlac_vec2f_c;
        ne10_mlac_vec3f = ne10_mlac_vec3f_c;
        ne10_mlac_vec4f = ne10_mlac_vec4f_c;
        ne10_add_float = ne10_add_float_c;
        ne10_sub_float = ne10_sub_float_c;
        ne10_mul_float = ne10_mul_float_c;
        ne10_div_float = ne10_div_float_c;
        ne10_mla_float = ne10_mla_float_c;
        ne10_abs_float = ne10_abs_float_c;
        ne10_len_vec2f = ne10_len_vec2f_c;
        ne10_len_vec3f = ne10_len_vec3f_c;
        ne10_len_vec4f = ne10_len_vec4f_c;
        ne10_normalize_vec2f = ne10_normalize_vec2f_c;
        ne10_normalize_vec3f = ne10_normalize_vec3f_c;
        ne10_normalize_vec4f = ne10_normalize_vec4f_c;

        ne10_abs_vec2f = ne10_abs_vec2f_c;
        ne10_abs_vec3f = ne10_abs_vec3f_c;
        ne10_abs_vec4f = ne10_abs_vec4f_c;
        ne10_vmul_vec2f = ne10_vmul_vec2f_c;
        ne10_vmul_vec3f = ne10_vmul_vec3f_c;
        ne10_vmul_vec4f = ne10_vmul_vec4f_c;
        ne10_vdiv_vec2f = ne10_vdiv_vec2f_c;
        ne10_vdiv_vec3f = ne10_vdiv_vec3f_c;
        ne10_vdiv_vec4f = ne10_vdiv_vec4f_c;
        ne10_vmla_vec2f = ne10_vmla_vec2f_c;
        ne10_vmla_vec3f = ne10_vmla_vec3f_c;
        ne10_vmla_vec4f = ne10_vmla_vec4f_c;
        ne10_add_vec2f = ne10_add_vec2f_c;
        ne10_add_vec3f = ne10_add_vec3f_c;
        ne10_add_vec4f = ne10_add_vec4f_c;
        ne10_sub_vec2f = ne10_sub_vec2f_c;
        ne10_sub_vec3f = ne10_sub_vec3f_c;
        ne10_sub_vec4f = ne10_sub_vec4f_c;
        ne10_dot_vec2f = ne10_dot_vec2f_c;
        ne10_dot_vec3f = ne10_dot_vec3f_c;
        ne10_dot_vec4f = ne10_dot_vec4f_c;
        ne10_cross_vec3f = ne10_cross_vec3f_c;

        ne10_addmat_2x2f = ne10_addmat_2x2f_c;
        ne10_addmat_3x3f = ne10_addmat_3x3f_c;
        ne10_addmat_4x4f = ne10_addmat_4x4f_c;
        ne10_submat_2x2f = ne10_submat_2x2f_c;
        ne10_submat_3x3f = ne10_submat_3x3f_c;
        ne10_submat_4x4f = ne10_submat_4x4f_c;
        ne10_mulmat_2x2f = ne10_mulmat_2x2f_c;
        ne10_mulmat_3x3f = ne10_mulmat_3x3f_c;
        ne10_mulmat_4x4f = ne10_mulmat_4x4f_c;
        ne10_mulcmatvec_cm2x2f_v2f = ne10_mulcmatvec_cm2x2f_v2f_c;
        ne10_mulcmatvec_cm3x3f_v3f = ne10_mulcmatvec_cm3x3f_v3f_c;
        ne10_mulcmatvec_cm4x4f_v4f = ne10_mulcmatvec_cm4x4f_v4f_c;
        ne10_detmat_2x2f = ne10_detmat_2x2f_c;
        ne10_detmat_3x3f = ne10_detmat_3x3f_c;
        ne10_detmat_4x4f = ne10_detmat_4x4f_c;
        ne10_invmat_2x2f = ne10_invmat_2x2f_c;
        ne10_invmat_3x3f = ne10_invmat_3x3f_c;
        ne10_invmat_4x4f = ne10_invmat_4x4f_c;
        ne10_transmat_4x4f = ne10_transmat_4x4f_c;
        ne10_identitymat_4x4f = ne10_identitymat_4x4f_c;
        ne10_transmat_3x3f = ne10_transmat_3x3f_c;
        ne10_identitymat_3x3f = ne10_identitymat_3x3f_c;
        ne10_transmat_2x2f = ne10_transmat_2x2f_c;
        ne10_identitymat_2x2f = ne10_identitymat_2x2f_c;
    }
    return NE10_OK;
}

// These are actual definitions of our function pointers that are declared in inc/NE10_types.h
ne10_result_t (*ne10_addc_float) (ne10_float32_t * dst, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count);
ne10_result_t (*ne10_addc_vec2f) (ne10_vec2f_t * dst, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count);
ne10_result_t (*ne10_addc_vec3f) (ne10_vec3f_t * dst, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count);
ne10_result_t (*ne10_addc_vec4f) (ne10_vec4f_t * dst, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count);
ne10_result_t (*ne10_subc_float) (ne10_float32_t * dst, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count);
ne10_result_t (*ne10_subc_vec2f) (ne10_vec2f_t * dst, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count);
ne10_result_t (*ne10_subc_vec3f) (ne10_vec3f_t * dst, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count);
ne10_result_t (*ne10_subc_vec4f) (ne10_vec4f_t * dst, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count);
ne10_result_t (*ne10_rsbc_float) (ne10_float32_t * dst, ne10_float32_t *src, const ne10_float32_t cst, ne10_uint32_t count);
ne10_result_t (*ne10_rsbc_vec2f) (ne10_vec2f_t * dst, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count);
ne10_result_t (*ne10_rsbc_vec3f) (ne10_vec3f_t * dst, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count);
ne10_result_t (*ne10_rsbc_vec4f) (ne10_vec4f_t * dst, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count);
ne10_result_t (*ne10_mulc_float) (ne10_float32_t * dst, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count);
ne10_result_t (*ne10_mulc_vec2f) (ne10_vec2f_t * dst, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count);
ne10_result_t (*ne10_mulc_vec3f) (ne10_vec3f_t * dst, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count);
ne10_result_t (*ne10_mulc_vec4f) (ne10_vec4f_t * dst, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count);
ne10_result_t (*ne10_divc_float) (ne10_float32_t * dst, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count);
ne10_result_t (*ne10_divc_vec2f) (ne10_vec2f_t * dst, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count);
ne10_result_t (*ne10_divc_vec3f) (ne10_vec3f_t * dst, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count);
ne10_result_t (*ne10_divc_vec4f) (ne10_vec4f_t * dst, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count);
ne10_result_t (*ne10_setc_float) (ne10_float32_t * dst, const ne10_float32_t cst, ne10_uint32_t count);
ne10_result_t (*ne10_setc_vec2f) (ne10_vec2f_t * dst, const ne10_vec2f_t * cst, ne10_uint32_t count);
ne10_result_t (*ne10_setc_vec3f) (ne10_vec3f_t * dst, const ne10_vec3f_t * cst, ne10_uint32_t count);
ne10_result_t (*ne10_setc_vec4f) (ne10_vec4f_t * dst, const ne10_vec4f_t * cst, ne10_uint32_t count);
ne10_result_t (*ne10_mlac_float) (ne10_float32_t * dst, ne10_float32_t * acc, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count);
ne10_result_t (*ne10_mlac_vec2f) (ne10_vec2f_t * dst, ne10_vec2f_t * acc, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count);
ne10_result_t (*ne10_mlac_vec3f) (ne10_vec3f_t * dst, ne10_vec3f_t * acc, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count);
ne10_result_t (*ne10_mlac_vec4f) (ne10_vec4f_t * dst, ne10_vec4f_t * acc, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count);
ne10_result_t (*ne10_add_float) (ne10_float32_t * dst, ne10_float32_t * src1, ne10_float32_t * src2, ne10_uint32_t count);
ne10_result_t (*ne10_sub_float) (ne10_float32_t * dst, ne10_float32_t * src1, ne10_float32_t * src2, ne10_uint32_t count);
ne10_result_t (*ne10_mul_float) (ne10_float32_t * dst, ne10_float32_t * src1, ne10_float32_t * src2, ne10_uint32_t count);
ne10_result_t (*ne10_div_float) (ne10_float32_t * dst, ne10_float32_t * src1, ne10_float32_t * src2, ne10_uint32_t count);
ne10_result_t (*ne10_mla_float) (ne10_float32_t * dst, ne10_float32_t * acc, ne10_float32_t * src1, ne10_float32_t * src2, ne10_uint32_t count);
ne10_result_t (*ne10_abs_float) (ne10_float32_t * dst, ne10_float32_t * src, ne10_uint32_t count);
ne10_result_t (*ne10_len_vec2f) (ne10_float32_t * dst, ne10_vec2f_t * src, ne10_uint32_t count);
ne10_result_t (*ne10_len_vec3f) (ne10_float32_t * dst, ne10_vec3f_t * src, ne10_uint32_t count);
ne10_result_t (*ne10_len_vec4f) (ne10_float32_t * dst, ne10_vec4f_t * src, ne10_uint32_t count);
ne10_result_t (*ne10_normalize_vec2f) (ne10_vec2f_t * dst, ne10_vec2f_t * src, ne10_uint32_t count);
ne10_result_t (*ne10_normalize_vec3f) (ne10_vec3f_t * dst, ne10_vec3f_t * src, ne10_uint32_t count);
ne10_result_t (*ne10_normalize_vec4f) (ne10_vec4f_t * dst, ne10_vec4f_t * src, ne10_uint32_t count);

ne10_result_t (*ne10_abs_vec2f) (ne10_vec2f_t * dst, ne10_vec2f_t * src, ne10_uint32_t count);
ne10_result_t (*ne10_abs_vec3f) (ne10_vec3f_t * dst, ne10_vec3f_t * src, ne10_uint32_t count);
ne10_result_t (*ne10_abs_vec4f) (ne10_vec4f_t * dst, ne10_vec4f_t * src, ne10_uint32_t count);
ne10_result_t (*ne10_vmul_vec2f) (ne10_vec2f_t * dst, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count);
ne10_result_t (*ne10_vmul_vec3f) (ne10_vec3f_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);
ne10_result_t (*ne10_vmul_vec4f) (ne10_vec4f_t * dst, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count);
ne10_result_t (*ne10_vdiv_vec2f) (ne10_vec2f_t * dst, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count);
ne10_result_t (*ne10_vdiv_vec3f) (ne10_vec3f_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);
ne10_result_t (*ne10_vdiv_vec4f) (ne10_vec4f_t * dst, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count);
ne10_result_t (*ne10_vmla_vec2f) (ne10_vec2f_t * dst, ne10_vec2f_t * acc, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count);
ne10_result_t (*ne10_vmla_vec3f) (ne10_vec3f_t * dst, ne10_vec3f_t * acc, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);
ne10_result_t (*ne10_vmla_vec4f) (ne10_vec4f_t * dst, ne10_vec4f_t * acc, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count);
ne10_result_t (*ne10_add_vec2f) (ne10_vec2f_t * dst, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count);
ne10_result_t (*ne10_add_vec3f) (ne10_vec3f_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);
ne10_result_t (*ne10_add_vec4f) (ne10_vec4f_t * dst, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count);
ne10_result_t (*ne10_sub_vec2f) (ne10_vec2f_t * dst, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count);
ne10_result_t (*ne10_sub_vec3f) (ne10_vec3f_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);
ne10_result_t (*ne10_sub_vec4f) (ne10_vec4f_t * dst, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count);
ne10_result_t (*ne10_dot_vec2f) (ne10_float32_t * dst, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count);
ne10_result_t (*ne10_dot_vec3f) (ne10_float32_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);
ne10_result_t (*ne10_dot_vec4f) (ne10_float32_t * dst, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count);
ne10_result_t (*ne10_cross_vec3f) (ne10_vec3f_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);

ne10_result_t (*ne10_addmat_2x2f) (ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src1, ne10_mat2x2f_t * src2, ne10_uint32_t count);
ne10_result_t (*ne10_addmat_3x3f) (ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src1, ne10_mat3x3f_t * src2, ne10_uint32_t count);
ne10_result_t (*ne10_addmat_4x4f) (ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src1, ne10_mat4x4f_t * src2, ne10_uint32_t count);
ne10_result_t (*ne10_submat_2x2f) (ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src1, ne10_mat2x2f_t * src2, ne10_uint32_t count);
ne10_result_t (*ne10_submat_3x3f) (ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src1, ne10_mat3x3f_t * src2, ne10_uint32_t count);
ne10_result_t (*ne10_submat_4x4f) (ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src1, ne10_mat4x4f_t * src2, ne10_uint32_t count);
ne10_result_t (*ne10_mulmat_2x2f) (ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src1, ne10_mat2x2f_t * src2, ne10_uint32_t count);
ne10_result_t (*ne10_mulmat_3x3f) (ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src1, ne10_mat3x3f_t * src2, ne10_uint32_t count);
ne10_result_t (*ne10_mulmat_4x4f) (ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src1, ne10_mat4x4f_t * src2, ne10_uint32_t count);
ne10_result_t (*ne10_mulcmatvec_cm4x4f_v4f) (ne10_vec4f_t * dst, const ne10_mat4x4f_t * cst, ne10_vec4f_t * src, ne10_uint32_t count);
ne10_result_t (*ne10_mulcmatvec_cm3x3f_v3f) (ne10_vec3f_t * dst, const ne10_mat3x3f_t * cst, ne10_vec3f_t * src, ne10_uint32_t count);
ne10_result_t (*ne10_mulcmatvec_cm2x2f_v2f) (ne10_vec2f_t * dst, const ne10_mat2x2f_t * cst, ne10_vec2f_t * src, ne10_uint32_t count);
ne10_result_t (*ne10_detmat_4x4f) (ne10_float32_t * dst, ne10_mat4x4f_t * src, ne10_uint32_t count);
ne10_result_t (*ne10_detmat_3x3f) (ne10_float32_t * dst, ne10_mat3x3f_t * src, ne10_uint32_t count);
ne10_result_t (*ne10_detmat_2x2f) (ne10_float32_t * dst, ne10_mat2x2f_t * src, ne10_uint32_t count);
ne10_result_t (*ne10_invmat_4x4f) (ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src, ne10_uint32_t count);
ne10_result_t (*ne10_invmat_3x3f) (ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src, ne10_uint32_t count);
ne10_result_t (*ne10_invmat_2x2f) (ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src, ne10_uint32_t count);
ne10_result_t (*ne10_transmat_4x4f) (ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src, ne10_uint32_t count);
ne10_result_t (*ne10_identitymat_4x4f) (ne10_mat4x4f_t * dst, ne10_uint32_t count);
ne10_result_t (*ne10_transmat_3x3f) (ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src, ne10_uint32_t count);
ne10_result_t (*ne10_identitymat_3x3f) (ne10_mat3x3f_t * dst, ne10_uint32_t count);
ne10_result_t (*ne10_transmat_2x2f) (ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src, ne10_uint32_t count);
ne10_result_t (*ne10_identitymat_2x2f) (ne10_mat2x2f_t * dst, ne10_uint32_t count);
