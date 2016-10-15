/*
 *  Copyright 2011-15 ARM Limited and Contributors.
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

/*
 * NE10 Library : inc/NE10_math.h
 */


#include "NE10_types.h"

#ifndef NE10_MATH_H
#define NE10_MATH_H

#ifdef __cplusplus
extern "C" {
#endif

///////////////////////////
// function prototypes:
///////////////////////////


// ## Vector-Constant Arithmetic ##

    /**
     * @ingroup groupMaths
     */

    /**
     * @defgroup ADD_VEC Vector Add
     *
     * \par
     * These functions implement the vector add operation for float data type.
     */

    /**
     * @addtogroup ADD_VEC
     * @{
     */

    /**
     * Adds a constant scalar value to all the elements of an input array and stores the results in an output array.
     * This function point could be pointed to one of ne10_addc_float_c, ne10_addc_float_neon and ne10_addc_float_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  cst   The constant scalar added to the input values
     * @param[in]  count The number of items in the input array
     */
    extern ne10_result_t (*ne10_addc_float) (ne10_float32_t * dst, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count);
    extern ne10_result_t ne10_addc_float_c (ne10_float32_t * dst, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count);
    extern ne10_result_t ne10_addc_float_neon (ne10_float32_t * dst, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count) asm ("ne10_addc_float_neon");
    extern ne10_result_t ne10_addc_float_asm (ne10_float32_t * dst, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count);
    /**
     * Adds a constant 2D vector to all of the vectors in an input array and stores the results in an output array.
     * This function point could be pointed to one of ne10_addc_vec2f_c, ne10_addc_vec2f_neon and ne10_addc_vec2f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  cst   Pointer to the 2D vector added to the input values
     * @param[in]  count The number of items in the input array
     */
    extern ne10_result_t (*ne10_addc_vec2f) (ne10_vec2f_t * dst, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count);
    extern ne10_result_t ne10_addc_vec2f_c (ne10_vec2f_t * dst, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count);
    extern ne10_result_t ne10_addc_vec2f_neon (ne10_vec2f_t * dst, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count) asm ("ne10_addc_vec2f_neon");
    extern ne10_result_t ne10_addc_vec2f_asm (ne10_vec2f_t * dst, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count);
    /**
     * Adds a constant 3D vector to all of the vectors in an input array and stores the results in an output array.
     * This function point could be pointed to one of ne10_addc_vec3f_c, ne10_addc_vec3f_neon and ne10_addc_vec3f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  cst   Pointer to the 3D vector added to the input values
     * @param[in]  count The number of items in the input array
     */
    extern ne10_result_t (*ne10_addc_vec3f) (ne10_vec3f_t * dst, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count);
    extern ne10_result_t ne10_addc_vec3f_c (ne10_vec3f_t * dst, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count);
    extern ne10_result_t ne10_addc_vec3f_neon (ne10_vec3f_t * dst, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count) asm ("ne10_addc_vec3f_neon");
    extern ne10_result_t ne10_addc_vec3f_asm (ne10_vec3f_t * dst, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count);
    /**
     * Adds a constant 4D vector to all of the vectors in an input array and stores the results in an output array.
     * This function point could be pointed to one of ne10_addc_vec4f_c, ne10_addc_vec4f_neon and ne10_addc_vec4f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  cst   Pointer to the 4D vector added to the input values
     * @param[in]  count The number of items in the input array
     */
    extern ne10_result_t (*ne10_addc_vec4f) (ne10_vec4f_t * dst, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count);
    extern ne10_result_t ne10_addc_vec4f_c (ne10_vec4f_t * dst, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count);
    extern ne10_result_t ne10_addc_vec4f_neon (ne10_vec4f_t * dst, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count) asm ("ne10_addc_vec4f_neon");
    extern ne10_result_t ne10_addc_vec4f_asm (ne10_vec4f_t * dst, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count);


    /**
     * Adds the elements of src1 to the elements of src2 and stores the results in the dst.
     * This function point could be pointed to one of ne10_add_float_c, ne10_add_float_neon and ne10_add_float_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src1  The first array to use as the input array
     * @param[in]  src2  The second array to use as the input array
     * @param[in]  count The number of items in the two input arrays
     */
    extern ne10_result_t (*ne10_add_float) (ne10_float32_t * dst, ne10_float32_t * src1, ne10_float32_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_add_float_c (ne10_float32_t * dst, ne10_float32_t * src1, ne10_float32_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_add_float_neon (ne10_float32_t * dst, ne10_float32_t * src1, ne10_float32_t * src2, ne10_uint32_t count) asm ("ne10_add_float_neon");
    extern ne10_result_t ne10_add_float_asm (ne10_float32_t * dst, ne10_float32_t * src1, ne10_float32_t * src2, ne10_uint32_t count);
    /**
     * Vector addition of two 2D vectors.
     * This function point could be pointed to one of ne10_add_vec2f_c, ne10_add_vec2f_neon and ne10_add_vec2f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src1   Pointer to the first source array
     * @param[in]  src2   Pointer to the second source array
     * @param[in]  count The number of items in the input arrays
     */
    extern ne10_result_t (*ne10_add_vec2f) (ne10_vec2f_t * dst, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_add_vec2f_c (ne10_vec2f_t * dst, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_add_vec2f_neon (ne10_vec2f_t * dst, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count) asm ("ne10_add_vec2f_neon");
    extern ne10_result_t ne10_add_vec2f_asm (ne10_vec2f_t * dst, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count);
    /**
     * Vector addition of two 3D vectors.
     * This function point could be pointed to one of ne10_add_vec3f_c, ne10_add_vec3f_neon and ne10_add_vec3f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src1   Pointer to the first source array
     * @param[in]  src2   Pointer to the second source array
     * @param[in]  count The number of items in the input arrays
     */
    extern ne10_result_t (*ne10_add_vec3f) (ne10_vec3f_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_add_vec3f_c (ne10_vec3f_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_add_vec3f_neon (ne10_vec3f_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count) asm ("ne10_add_vec3f_neon");
    extern ne10_result_t ne10_add_vec3f_asm (ne10_vec3f_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);
    /**
     * Vector addition of two 4D vectors.
     * This function point could be pointed to one of ne10_add_vec4f_c, ne10_add_vec4f_neon and ne10_add_vec4f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src1   Pointer to the first source array
     * @param[in]  src2   Pointer to the second source array
     * @param[in]  count The number of items in the input arrays
     */
    extern ne10_result_t (*ne10_add_vec4f) (ne10_vec4f_t * dst, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_add_vec4f_c (ne10_vec4f_t * dst, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_add_vec4f_neon (ne10_vec4f_t * dst, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count) asm ("ne10_add_vec4f_neon");
    extern ne10_result_t ne10_add_vec4f_asm (ne10_vec4f_t * dst, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count);
    /** @} */ //end of Vector Add group

    /**
     * @ingroup groupMaths
     */

    /**
     * @defgroup ADD_MAT Matrix Add
     *
     * \par
     * These functions implement the matrix add operation for float data type.
     */

    /**
     * @addtogroup ADD_MAT
     * @{
     */

    /**
     * Vector addition of two 4x4 matrixs.
     * This function point could be pointed to one of ne10_addmat_4x4f_c, ne10_addmat_4x4f_neon and ne10_addmat_4x4f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src1   Pointer to the first source array
     * @param[in]  src2   Pointer to the second source array
     * @param[in]  count The number of items in the input arrays
     */
    extern ne10_result_t (*ne10_addmat_4x4f) (ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src1, ne10_mat4x4f_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_addmat_4x4f_c (ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src1, ne10_mat4x4f_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_addmat_4x4f_neon (ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src1, ne10_mat4x4f_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_addmat_4x4f_asm (ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src1, ne10_mat4x4f_t * src2, ne10_uint32_t count);
    /**
     * Vector addition of two 3x3 matrixs.
     * This function point could be pointed to one of ne10_addmat_3x3f_c, ne10_addmat_3x3f_neon and ne10_addmat_3x3f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src1   Pointer to the first source array
     * @param[in]  src2   Pointer to the second source array
     * @param[in]  count The number of items in the input arrays
     */
    extern ne10_result_t (*ne10_addmat_3x3f) (ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src1, ne10_mat3x3f_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_addmat_3x3f_c (ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src1, ne10_mat3x3f_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_addmat_3x3f_neon (ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src1, ne10_mat3x3f_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_addmat_3x3f_asm (ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src1, ne10_mat3x3f_t * src2, ne10_uint32_t count);
    /**
     * Vector addition of two 2x2 matrixs.
     * This function point could be pointed to one of ne10_addmat_2x2f_c, ne10_addmat_2x2f_neon and ne10_addmat_2x2f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src1   Pointer to the first source array
     * @param[in]  src2   Pointer to the second source array
     * @param[in]  count The number of items in the input arrays
     */
    extern ne10_result_t (*ne10_addmat_2x2f) (ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src1, ne10_mat2x2f_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_addmat_2x2f_c (ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src1, ne10_mat2x2f_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_addmat_2x2f_neon (ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src1, ne10_mat2x2f_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_addmat_2x2f_asm (ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src1, ne10_mat2x2f_t * src2, ne10_uint32_t count);
    /** @} */ //end of Matrix Add group


    /**
     * @ingroup groupMaths
     */

    /**
     * @defgroup SUB_VEC Vector Sub
     *
     * \par
     * These functions implement the vector sub operation for float data type.
     */

    /**
     * @addtogroup SUB_VEC
     * @{
     */

    /**
     * Subtracts a constant scalar from all the elements of an input array and stores the results in an output array.
     * This function point could be pointed to one of ne10_subc_float_c, ne10_subc_float_neon and ne10_subc_float_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  cst   The constant scalar subtracted from the input values
     * @param[in]  count The number of items in the input array
     */
    extern ne10_result_t (*ne10_subc_float) (ne10_float32_t * dst, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count);
    extern ne10_result_t ne10_subc_float_c (ne10_float32_t * dst, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count); // subtract cst from the element(s)
    extern ne10_result_t ne10_subc_float_neon (ne10_float32_t * dst, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count); // subtract cst from the element(s)
    extern ne10_result_t ne10_subc_float_asm (ne10_float32_t * dst, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count); // subtract cst from the element(s)
    /**
     * Subtracts a constant 2D vector from all of the vectors in an input array and stores the results in an output array.
     * This function point could be pointed to one of ne10_subc_vec2f_c, ne10_subc_vec2f_neon and ne10_subc_vec2f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  cst   Pointer to the 2D vector subtracted from the input values
     * @param[in]  count The number of items in the input array
     */
    extern ne10_result_t (*ne10_subc_vec2f) (ne10_vec2f_t * dst, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count);
    extern ne10_result_t ne10_subc_vec2f_c (ne10_vec2f_t * dst, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count); // subtract cst from the element(s)
    extern ne10_result_t ne10_subc_vec2f_neon (ne10_vec2f_t * dst, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count); // subtract cst from the element(s)
    extern ne10_result_t ne10_subc_vec2f_asm (ne10_vec2f_t * dst, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count); // subtract cst from the element(s)
    /**
     * Subtracts a constant 3D vector from all of the vectors in an input array and stores the results in an output array.
     * This function point could be pointed to one of ne10_subc_vec3f_c, ne10_subc_vec3f_neon and ne10_subc_vec3f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  cst   Pointer to the 3D vector subtracted from the input values
     * @param[in]  count The number of items in the input array
     */
    extern ne10_result_t (*ne10_subc_vec3f) (ne10_vec3f_t * dst, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count);
    extern ne10_result_t ne10_subc_vec3f_c (ne10_vec3f_t * dst, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count); // subtract cst from the element(s)
    extern ne10_result_t ne10_subc_vec3f_neon (ne10_vec3f_t * dst, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count); // subtract cst from the element(s)
    extern ne10_result_t ne10_subc_vec3f_asm (ne10_vec3f_t * dst, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count); // subtract cst from the element(s)
    /**
     * Subtracts a constant 4D vector from all of the vectors in an input array and stores the results in an output array.
     * This function point could be pointed to one of ne10_subc_vec4f_c, ne10_subc_vec4f_neon and ne10_subc_vec4f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  cst   Pointer to the 4D vector subtracted from the input values
     * @param[in]  count The number of items in the input array
     */
    extern ne10_result_t (*ne10_subc_vec4f) (ne10_vec4f_t * dst, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count);
    extern ne10_result_t ne10_subc_vec4f_c (ne10_vec4f_t * dst, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count); // subtract cst from the element(s)
    extern ne10_result_t ne10_subc_vec4f_neon (ne10_vec4f_t * dst, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count); // subtract cst from the element(s)
    extern ne10_result_t ne10_subc_vec4f_asm (ne10_vec4f_t * dst, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count); // subtract cst from the element(s)

    /**
     * Subtracts the elements of src2 from the elements of src1 and stores the results in the dst.
     * This function point could be pointed to one of ne10_sub_float_c, ne10_sub_float_neon and ne10_sub_float_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src1  The first array to use as the input array
     * @param[in]  src2  The second array to use as the input array
     * @param[in]  count The number of items in the two input arrays
     */
    extern ne10_result_t (*ne10_sub_float) (ne10_float32_t * dst, ne10_float32_t * src1, ne10_float32_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_sub_float_c (ne10_float32_t * dst, ne10_float32_t * src1, ne10_float32_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_sub_float_neon (ne10_float32_t * dst, ne10_float32_t * src1, ne10_float32_t * src2, ne10_uint32_t count) asm ("ne10_sub_float_neon");
    extern ne10_result_t ne10_sub_float_asm (ne10_float32_t * dst, ne10_float32_t * src1, ne10_float32_t * src2, ne10_uint32_t count);
    /**
     * Vector subtraction of two 2D vectors.
     * This function point could be pointed to one of ne10_sub_vec2f_c, ne10_sub_vec2f_neon and ne10_sub_vec2f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src1   Pointer to the first source array
     * @param[in]  src2   Pointer to the second source array
     * @param[in]  count The number of items in the input arrays
     */
    extern ne10_result_t (*ne10_sub_vec2f) (ne10_vec2f_t * dst, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_sub_vec2f_c (ne10_vec2f_t * dst, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_sub_vec2f_neon (ne10_vec2f_t * dst, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count) asm ("ne10_sub_vec2f_neon");
    extern ne10_result_t ne10_sub_vec2f_asm (ne10_vec2f_t * dst, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count);
    /**
     * Vector subtraction of two 3D vectors.
     * This function point could be pointed to one of ne10_sub_vec3f_c, ne10_sub_vec3f_neon and ne10_sub_vec3f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src1   Pointer to the first source array
     * @param[in]  src2   Pointer to the second source array
     * @param[in]  count The number of items in the input arrays
     */
    extern ne10_result_t (*ne10_sub_vec3f) (ne10_vec3f_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_sub_vec3f_c (ne10_vec3f_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_sub_vec3f_neon (ne10_vec3f_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count) asm ("ne10_sub_vec3f_neon");
    extern ne10_result_t ne10_sub_vec3f_asm (ne10_vec3f_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);
    /**
     * Vector subtraction of two 4D vectors.
     * This function point could be pointed to one of ne10_sub_vec4f_c, ne10_sub_vec4f_neon and ne10_sub_vec4f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src1   Pointer to the first source array
     * @param[in]  src2   Pointer to the second source array
     * @param[in]  count The number of items in the input arrays
     */
    extern ne10_result_t (*ne10_sub_vec4f) (ne10_vec4f_t * dst, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_sub_vec4f_c (ne10_vec4f_t * dst, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_sub_vec4f_neon (ne10_vec4f_t * dst, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count) asm ("ne10_sub_vec4f_neon");
    extern ne10_result_t ne10_sub_vec4f_asm (ne10_vec4f_t * dst, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count);
    /** @} */ //end of Vector Sub group

    /**
     * @ingroup groupMaths
     */

    /**
     * @defgroup RSBC Vector Rsbc
     *
     * \par
     * These functions implement the vector rsbc operation for float data type.
     */

    /**
     * @addtogroup RSBC
     * @{
     */
    /**
     * Subtracts the elements of an input array from a constant scalar and stores the results in an output array.
     * This function point could be pointed to one of ne10_rsbc_float_c, ne10_rsbc_float_neon and ne10_rsbc_float_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  cst   The constant scalar to subtract the input values from
     * @param[in]  count The number of items in the input array
     */
    extern ne10_result_t (*ne10_rsbc_float) (ne10_float32_t * dst, ne10_float32_t *src, const ne10_float32_t cst, ne10_uint32_t count);
    extern ne10_result_t ne10_rsbc_float_c (ne10_float32_t * dst, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count); // subtract element(s) from a cst
    extern ne10_result_t ne10_rsbc_float_neon (ne10_float32_t * dst, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count); // subtract element(s) from a cst
    extern ne10_result_t ne10_rsbc_float_asm (ne10_float32_t * dst, ne10_float32_t *src, const ne10_float32_t cst, ne10_uint32_t count); // subtract element(s) from a cst
    /**
     * Subtracts the vectors in an input array from a constant 2D vector and stores the results in an output array.
     * This function point could be pointed to one of ne10_rsbc_vec2f_c, ne10_rsbc_vec2f_neon and ne10_rsbc_vec2f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  cst   Pointer to the 2D vector to subtract the input values from
     * @param[in]  count The number of items in the input array
     */
    extern ne10_result_t (*ne10_rsbc_vec2f) (ne10_vec2f_t * dst, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count);
    extern ne10_result_t ne10_rsbc_vec2f_c (ne10_vec2f_t * dst, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count); // subtract element(s) from a cst
    extern ne10_result_t ne10_rsbc_vec2f_neon (ne10_vec2f_t * dst, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count); // subtract element(s) from a cst
    extern ne10_result_t ne10_rsbc_vec2f_asm (ne10_vec2f_t * dst, ne10_vec2f_t *src, const ne10_vec2f_t * cst, ne10_uint32_t count); // subtract element(s) from a cst
    /**
     * Subtracts the vectors in an input array from a constant 3D vector and stores the results in an output array.
     * This function point could be pointed to one of ne10_rsbc_vec3f_c, ne10_rsbc_vec3f_neon and ne10_rsbc_vec3f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  cst   Pointer to the 3D vector to subtract the input values from
     * @param[in]  count The number of items in the input array
     */
    extern ne10_result_t (*ne10_rsbc_vec3f) (ne10_vec3f_t * dst, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count);
    extern ne10_result_t ne10_rsbc_vec3f_c (ne10_vec3f_t * dst, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count); // subtract element(s) from a cst
    extern ne10_result_t ne10_rsbc_vec3f_neon (ne10_vec3f_t * dst, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count); // subtract element(s) from a cst
    extern ne10_result_t ne10_rsbc_vec3f_asm (ne10_vec3f_t * dst, ne10_vec3f_t *src, const ne10_vec3f_t * cst, ne10_uint32_t count); // subtract element(s) from a cst
    /**
     * Subtracts the vectors in an input array from a constant 4D vector and stores the results in an output array.
     * This function point could be pointed to one of ne10_rsbc_vec4f_c, ne10_rsbc_vec4f_neon and ne10_rsbc_vec4f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  cst   Pointer to the 4D vector to subtract the input values from
     * @param[in]  count The number of items in the input array
     */
    extern ne10_result_t (*ne10_rsbc_vec4f) (ne10_vec4f_t * dst, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count);
    extern ne10_result_t ne10_rsbc_vec4f_c (ne10_vec4f_t * dst, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count); // subtract element(s) from a cst
    extern ne10_result_t ne10_rsbc_vec4f_neon (ne10_vec4f_t * dst, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count); // subtract element(s) from a cst
    extern ne10_result_t ne10_rsbc_vec4f_asm (ne10_vec4f_t * dst, ne10_vec4f_t *src, const ne10_vec4f_t * cst, ne10_uint32_t count); // subtract element(s) from a cst
    /** @} */ //end of Vector RSBC group

    /**
     * @ingroup groupMaths
     */

    /**
     * @defgroup SUB_MAT Matrix Sub
     *
     * \par
     * These functions implement the matrix sub operation for float data type.
     */

    /**
     * @addtogroup SUB_MAT
     * @{
     */
    /**
     * Matrix subtraction of two 4x4 matrixs.
     * This function point could be pointed to one of ne10_submat_4x4f_c, ne10_submat_4x4f_neon and ne10_submat_4x4f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src1   Pointer to the first source array
     * @param[in]  src2   Pointer to the second source array
     * @param[in]  count The number of items in the input arrays
     */
    extern ne10_result_t (*ne10_submat_4x4f) (ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src1, ne10_mat4x4f_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_submat_4x4f_c (ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src1, ne10_mat4x4f_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_submat_4x4f_neon (ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src1, ne10_mat4x4f_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_submat_4x4f_asm (ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src1, ne10_mat4x4f_t * src2, ne10_uint32_t count);

    /**
     * Matrix subtraction of two 3x3 matrixs.
     * This function point could be pointed to one of ne10_submat_3x3f_c, ne10_submat_3x3f_neon and ne10_submat_3x3f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src1   Pointer to the first source array
     * @param[in]  src2   Pointer to the second source array
     * @param[in]  count The number of items in the input arrays
     */
    extern ne10_result_t (*ne10_submat_3x3f) (ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src1, ne10_mat3x3f_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_submat_3x3f_c (ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src1, ne10_mat3x3f_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_submat_3x3f_neon (ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src1, ne10_mat3x3f_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_submat_3x3f_asm (ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src1, ne10_mat3x3f_t * src2, ne10_uint32_t count);

    /**
     * Matrix subtraction of two 2x2 matrixs.
     * This function point could be pointed to one of ne10_submat_2x2f_c, ne10_submat_2x2f_neon and ne10_submat_2x2f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src1   Pointer to the first source array
     * @param[in]  src2   Pointer to the second source array
     * @param[in]  count The number of items in the input arrays
     */
    extern ne10_result_t (*ne10_submat_2x2f) (ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src1, ne10_mat2x2f_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_submat_2x2f_c (ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src1, ne10_mat2x2f_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_submat_2x2f_neon (ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src1, ne10_mat2x2f_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_submat_2x2f_asm (ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src1, ne10_mat2x2f_t * src2, ne10_uint32_t count);
    /** @} */ //end of Matrix Sub group

    /**
     * @ingroup groupMaths
     */

    /**
     * @defgroup MUL_VEC Vector Multiply
     *
     * \par
     * These functions implement the vector multiply operation for float data type.
     */

    /**
     * @addtogroup MUL_VEC
     * @{
     */

    /**
     * Multiplies the elements of an input array by a constant scalar and stores the results in an output array.
     * This function point could be pointed to one of ne10_mulc_float_c, ne10_mulc_float_neon and ne10_mulc_float_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  cst   The constant scalar to multiply the input values with
     * @param[in]  count The number of items in the input array
     */
    extern ne10_result_t (*ne10_mulc_float) (ne10_float32_t * dst, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count);
    extern ne10_result_t ne10_mulc_float_c (ne10_float32_t * dst, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count);
    extern ne10_result_t ne10_mulc_float_neon (ne10_float32_t * dst, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count);
    extern ne10_result_t ne10_mulc_float_asm (ne10_float32_t * dst, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count);
    /**
     * Multiplies the components of 2D vectors in an input array by the components of a constant 2D vector and stores the results in an output array.
     * This function point could be pointed to one of ne10_mulc_vec2f_c, ne10_mulc_vec2f_neon and ne10_mulc_vec2f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  cst   Pointer to the 2D vector to multiply the input values with
     * @param[in]  count The number of items in the input array
     */
    extern ne10_result_t (*ne10_mulc_vec2f) (ne10_vec2f_t * dst, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count);
    extern ne10_result_t ne10_mulc_vec2f_c (ne10_vec2f_t * dst, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count);
    extern ne10_result_t ne10_mulc_vec2f_neon (ne10_vec2f_t * dst, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count);
    extern ne10_result_t ne10_mulc_vec2f_asm (ne10_vec2f_t * dst, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count);
    /**
     * Multiplies the components of 3D vectors in an input array by the components of a constant 3D vector and stores the results in an output array.
     * This function point could be pointed to one of ne10_mulc_vec3f_c, ne10_mulc_vec3f_neon and ne10_mulc_vec3f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  cst   Pointer to the 3D vector to multiply the input values with
     * @param[in]  count The number of items in the input array
     */
    extern ne10_result_t (*ne10_mulc_vec3f) (ne10_vec3f_t * dst, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count);
    extern ne10_result_t ne10_mulc_vec3f_c (ne10_vec3f_t * dst, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count);
    extern ne10_result_t ne10_mulc_vec3f_neon (ne10_vec3f_t * dst, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count);
    extern ne10_result_t ne10_mulc_vec3f_asm (ne10_vec3f_t * dst, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count);
    /**
     * Multiplies the components of 4D vectors in an input array by the components of a constant 4D vector and stores the results in an output array.
     * This function point could be pointed to one of ne10_mulc_vec4f_c, ne10_mulc_vec4f_neon and ne10_mulc_vec4f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  cst   Pointer to the 4D vector to multiply the input values with
     * @param[in]  count The number of items in the input array
     */
    extern ne10_result_t (*ne10_mulc_vec4f) (ne10_vec4f_t * dst, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count);
    extern ne10_result_t ne10_mulc_vec4f_c (ne10_vec4f_t * dst, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count);
    extern ne10_result_t ne10_mulc_vec4f_neon (ne10_vec4f_t * dst, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count);
    extern ne10_result_t ne10_mulc_vec4f_asm (ne10_vec4f_t * dst, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count);

    /**
     * Multiplies the elements of src1 by the elements of src2 and stores the results in the dst.
     * This function point could be pointed to one of ne10_mul_float_c, ne10_mul_float_neon and ne10_mul_float_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src1  The first array to use as the input array
     * @param[in]  src2  The second array to use as the input array
     * @param[in]  count The number of items in the two input arrays
     */
    extern ne10_result_t (*ne10_mul_float) (ne10_float32_t * dst, ne10_float32_t * src1, ne10_float32_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_mul_float_c (ne10_float32_t * dst, ne10_float32_t * src1, ne10_float32_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_mul_float_neon (ne10_float32_t * dst, ne10_float32_t * src1, ne10_float32_t * src2, ne10_uint32_t count) asm ("ne10_mul_float_neon");
    extern ne10_result_t ne10_mul_float_asm (ne10_float32_t * dst, ne10_float32_t * src1, ne10_float32_t * src2, ne10_uint32_t count);
    /**
     * Multiplies the components of a 2D vector with the corresponding components of another.
     * This function point could be pointed to one of ne10_vmul_vec2f_c, ne10_vmul_vec2f_neon and ne10_vmul_vec2f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src1   Pointer to the first source array
     * @param[in]  src2   Pointer to the second source array
     * @param[in]  count The number of items in the input arrays
     */
    extern ne10_result_t (*ne10_vmul_vec2f) (ne10_vec2f_t * dst, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_vmul_vec2f_c (ne10_vec2f_t * dst, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_vmul_vec2f_neon (ne10_vec2f_t * dst, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count) asm ("ne10_vmul_vec2f_neon");
    extern ne10_result_t ne10_vmul_vec2f_asm (ne10_vec2f_t * dst, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count);
    /**
     * Multiplies the components of a 3D vector with the corresponding components of another.
     * This function point could be pointed to one of ne10_vmul_vec3f_c, ne10_vmul_vec3f_neon and ne10_vmul_vec3f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src1   Pointer to the first source array
     * @param[in]  src2   Pointer to the second source array
     * @param[in]  count The number of items in the input arrays
     */
    extern ne10_result_t (*ne10_vmul_vec3f) (ne10_vec3f_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_vmul_vec3f_c (ne10_vec3f_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_vmul_vec3f_neon (ne10_vec3f_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count) asm ("ne10_vmul_vec3f_neon");
    extern ne10_result_t ne10_vmul_vec3f_asm (ne10_vec3f_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);
    /**
     * Multiplies the components of a 4D vector with the corresponding components of another.
     * This function point could be pointed to one of ne10_vmul_vec4f_c, ne10_vmul_vec4f_neon and ne10_vmul_vec4f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src1   Pointer to the first source array
     * @param[in]  src2   Pointer to the second source array
     * @param[in]  count The number of items in the input arrays
     */
    extern ne10_result_t (*ne10_vmul_vec4f) (ne10_vec4f_t * dst, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_vmul_vec4f_c (ne10_vec4f_t * dst, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_vmul_vec4f_neon (ne10_vec4f_t * dst, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count) asm ("ne10_vmul_vec4f_neon");
    extern ne10_result_t ne10_vmul_vec4f_asm (ne10_vec4f_t * dst, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count);
    /** @} */ //end of Vector Multiply group

    /**
     * @ingroup groupMaths
     */

    /**
     * @defgroup MLA_VEC Vector Multiply-Accumulator
     *
     * \par
     * These functions implement the vector multiply-accumulator operation for float data type.
     */

    /**
     * @addtogroup MLA_VEC
     * @{
     */

    /**
     * Multiplies each entry in the source array (src) by cst, then adds the result to
     * the corresponding item of the accumulation array (acc), and stores the result in the destination array.
     * This function point could be pointed to one of ne10_mlac_float_c, ne10_mlac_float_neon and ne10_mlac_float_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  acc   The corresponding elemetn is added to the result of the multiplication
     * @param[in]  src   Pointer to the source array
     * @param[in]  cst   The constant scalar to multiply the input elements with
     * @param[in]  count The number of items in the input array
     */
    extern ne10_result_t (*ne10_mlac_float) (ne10_float32_t * dst, ne10_float32_t * acc, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count);
    extern ne10_result_t ne10_mlac_float_c (ne10_float32_t * dst, ne10_float32_t * acc, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count);
    extern ne10_result_t ne10_mlac_float_neon (ne10_float32_t * dst, ne10_float32_t * acc, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count);
    extern ne10_result_t ne10_mlac_float_asm (ne10_float32_t * dst, ne10_float32_t * acc, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count);
    /**
     * Multiplies each entry in the source array (src) by the 2D vector cst, then adds the result to
     * the corresponding item of the accumulation array (acc), and stores the result in the destination array.
     * This function point could be pointed to one of ne10_mlac_vec2f_c, ne10_mlac_vec2f_neon and ne10_mlac_vec2f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  acc   The corresponding elemetn is added to the result of the multiplication
     * @param[in]  src   Pointer to the source array
     * @param[in]  cst   Pointer to the 2D vector to multiply the input vectors with
     * @param[in]  count The number of items in the input array
     */
    extern ne10_result_t (*ne10_mlac_vec2f) (ne10_vec2f_t * dst, ne10_vec2f_t * acc, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count);
    extern ne10_result_t ne10_mlac_vec2f_c (ne10_vec2f_t * dst, ne10_vec2f_t * acc, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count);
    extern ne10_result_t ne10_mlac_vec2f_neon (ne10_vec2f_t * dst, ne10_vec2f_t * acc, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count);
    extern ne10_result_t ne10_mlac_vec2f_asm (ne10_vec2f_t * dst, ne10_vec2f_t * acc, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count);
    /**
     * Multiplies each entry in the source array (src) by the 3D vector cst, then adds the result to
     * the corresponding item of the accumulation array (acc), and stores the result in the destination array.
     * This function point could be pointed to one of ne10_mlac_vec3f_c, ne10_mlac_vec3f_neon and ne10_mlac_vec3f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  acc   The corresponding elemetn is added to the result of the multiplication
     * @param[in]  src   Pointer to the source array
     * @param[in]  cst   Pointer to the 3D vector to multiply the input vectors with
     * @param[in]  count The number of items in the input array
     */
    extern ne10_result_t (*ne10_mlac_vec3f) (ne10_vec3f_t * dst, ne10_vec3f_t * acc, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count);
    extern ne10_result_t ne10_mlac_vec3f_c (ne10_vec3f_t * dst, ne10_vec3f_t * acc, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count);
    extern ne10_result_t ne10_mlac_vec3f_neon (ne10_vec3f_t * dst, ne10_vec3f_t * acc, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count);
    extern ne10_result_t ne10_mlac_vec3f_asm (ne10_vec3f_t * dst, ne10_vec3f_t * acc, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count);
    /**
     * Multiplies each entry in the source array (src) by the 4D vector cst, then adds the result to
     * the corresponding item of the accumulation array (acc), and stores the result in the destination array.
     * This function point could be pointed to one of ne10_mlac_vec4f_c, ne10_mlac_vec4f_neon and ne10_mlac_vec4f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  acc   The corresponding elemetn is added to the result of the multiplication
     * @param[in]  src   Pointer to the source array
     * @param[in]  cst   Pointer to the 4D vector to multiply the input vectors with
     * @param[in]  count The number of items in the input array
     */
    extern ne10_result_t (*ne10_mlac_vec4f) (ne10_vec4f_t * dst, ne10_vec4f_t * acc, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count);
    extern ne10_result_t ne10_mlac_vec4f_c (ne10_vec4f_t * dst, ne10_vec4f_t * acc, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count);
    extern ne10_result_t ne10_mlac_vec4f_neon (ne10_vec4f_t * dst, ne10_vec4f_t * acc, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count);
    extern ne10_result_t ne10_mlac_vec4f_asm (ne10_vec4f_t * dst, ne10_vec4f_t * acc, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count);

    /**
     * Performs a multiply and accumulate operation using the corresponding elements in acc, src1, and src2.
     * This function point could be pointed to one of ne10_mla_float_c, ne10_mla_float_neon and ne10_mla_float_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  acc   These elemtns are added to the result of the multiplication operation
     * @param[in]  src1  The first array to use as the input array
     * @param[in]  src2  The second array to use as the input array
     * @param[in]  count The number of items in the two input arrays
     */
    extern ne10_result_t (*ne10_mla_float) (ne10_float32_t * dst, ne10_float32_t * acc, ne10_float32_t * src1, ne10_float32_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_mla_float_c (ne10_float32_t * dst, ne10_float32_t * acc, ne10_float32_t * src1, ne10_float32_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_mla_float_neon (ne10_float32_t * dst, ne10_float32_t * acc, ne10_float32_t * src1, ne10_float32_t * src2, ne10_uint32_t count) asm ("ne10_mla_float_neon");
    extern ne10_result_t ne10_mla_float_asm (ne10_float32_t * dst, ne10_float32_t * acc, ne10_float32_t * src1, ne10_float32_t * src2, ne10_uint32_t count);
    /**
     * Performs a multiply and accumulate operation on the components of a 2D vector with the corresponding components of another.
     * This function point could be pointed to one of ne10_vmla_vec2f_c, ne10_vmla_vec2f_neon and ne10_vmla_vec2f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src1   Pointer to the first source array
     * @param[in]  src2   Pointer to the second source array
     * @param[in]  count The number of items in the input arrays
     */
    extern ne10_result_t (*ne10_vmla_vec2f) (ne10_vec2f_t * dst, ne10_vec2f_t * acc, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_vmla_vec2f_c (ne10_vec2f_t * dst, ne10_vec2f_t * acc, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_vmla_vec2f_neon (ne10_vec2f_t * dst, ne10_vec2f_t * acc, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count) asm ("ne10_vmla_vec2f_neon");
    extern ne10_result_t ne10_vmla_vec2f_asm (ne10_vec2f_t * dst, ne10_vec2f_t * acc, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count);
    /**
     * Performs a multiply and accumulate operation on the components of a 3D vector with the corresponding components of another.
     * This function point could be pointed to one of ne10_vmla_vec3f_c, ne10_vmla_vec3f_neon and ne10_vmla_vec3f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src1   Pointer to the first source array
     * @param[in]  src2   Pointer to the second source array
     * @param[in]  count The number of items in the input arrays
     */
    extern ne10_result_t (*ne10_vmla_vec3f) (ne10_vec3f_t * dst, ne10_vec3f_t * acc, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_vmla_vec3f_c (ne10_vec3f_t * dst, ne10_vec3f_t * acc, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_vmla_vec3f_neon (ne10_vec3f_t * dst, ne10_vec3f_t * acc, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count) asm ("ne10_vmla_vec3f_neon");
    extern ne10_result_t ne10_vmla_vec3f_asm (ne10_vec3f_t * dst, ne10_vec3f_t * acc, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);
    /**
     * Performs a multiply and accumulate operation on the components of a 4D vector with the corresponding components of another.
     * This function point could be pointed to one of ne10_vmla_vec4f_c, ne10_vmla_vec4f_neon and ne10_vmla_vec4f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src1   Pointer to the first source array
     * @param[in]  src2   Pointer to the second source array
     * @param[in]  count The number of items in the input arrays
     */
    extern ne10_result_t (*ne10_vmla_vec4f) (ne10_vec4f_t * dst, ne10_vec4f_t * acc, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_vmla_vec4f_c (ne10_vec4f_t * dst, ne10_vec4f_t * acc, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_vmla_vec4f_neon (ne10_vec4f_t * dst, ne10_vec4f_t * acc, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count) asm ("ne10_vmla_vec4f_neon");
    extern ne10_result_t ne10_vmla_vec4f_asm (ne10_vec4f_t * dst, ne10_vec4f_t * acc, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count);
    /** @} */ //end of Vector Multiply-Accumulator group

    /**
     * @ingroup groupMaths
     */

    /**
     * @defgroup MUL_MAT Matrix Multiply
     *
     * \par
     * These functions implement the matrix multiply operation for float data type.
     */

    /**
     * @addtogroup MUL_MAT
     * @{
     */

    /**
     * Matrix multiplication of two 4x4 matrixs.
     * This function point could be pointed to one of ne10_mulmat_4x4f_c, ne10_mulmat_4x4f_neon and ne10_mulmat_4x4f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src1   Pointer to the first source array
     * @param[in]  src2   Pointer to the second source array
     * @param[in]  count The number of items in the input arrays
     */
    extern ne10_result_t (*ne10_mulmat_4x4f) (ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src1, ne10_mat4x4f_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_mulmat_4x4f_c (ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src1, ne10_mat4x4f_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_mulmat_4x4f_neon (ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src1, ne10_mat4x4f_t * src2, ne10_uint32_t count) asm ("ne10_mulmat_4x4f_neon");
    extern ne10_result_t ne10_mulmat_4x4f_asm (ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src1, ne10_mat4x4f_t * src2, ne10_uint32_t count);

    /**
     * Matrix multiplication of two 3x3 matrixs.
     * This function point could be pointed to one of ne10_mulmat_3x3f_c, ne10_mulmat_3x3f_neon and ne10_mulmat_3x3f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src1   Pointer to the first source array
     * @param[in]  src2   Pointer to the second source array
     * @param[in]  count The number of items in the input arrays
     */
    extern ne10_result_t (*ne10_mulmat_3x3f) (ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src1, ne10_mat3x3f_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_mulmat_3x3f_c (ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src1, ne10_mat3x3f_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_mulmat_3x3f_neon (ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src1, ne10_mat3x3f_t * src2, ne10_uint32_t count) asm ("ne10_mulmat_3x3f_neon");
    extern ne10_result_t ne10_mulmat_3x3f_asm (ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src1, ne10_mat3x3f_t * src2, ne10_uint32_t count);

    /**
     * Matrix multiplication of two 2x2 matrixs.
     * This function point could be pointed to one of ne10_mulmat_2x2f_c, ne10_mulmat_2x2f_neon and ne10_mulmat_2x2f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src1   Pointer to the first source array
     * @param[in]  src2   Pointer to the second source array
     * @param[in]  count The number of items in the input arrays
     */
    extern ne10_result_t (*ne10_mulmat_2x2f) (ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src1, ne10_mat2x2f_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_mulmat_2x2f_c (ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src1, ne10_mat2x2f_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_mulmat_2x2f_neon (ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src1, ne10_mat2x2f_t * src2, ne10_uint32_t count) asm ("ne10_mulmat_2x2f_neon");
    extern ne10_result_t ne10_mulmat_2x2f_asm (ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src1, ne10_mat2x2f_t * src2, ne10_uint32_t count);
    /** @} */ //end of Matrix Multiply group

    /**
     * @ingroup groupMaths
     */

    /**
     * @defgroup MUL_MAT_VEC Matrix Vector Multiply
     *
     * \par
     * These functions implement the matrix vector multiply operation for float data type.
     */

    /**
     * @addtogroup MUL_MAT_VEC
     * @{
     */
    /**
     * Matrix multiplication of 4x4 matrix and 4D vector.
     * This function point could be pointed to one of ne10_mulcmatvec_cm4x4f_v4f_c, ne10_mulcmatvec_cm4x4f_v4f_neon and ne10_mulcmatvec_cm4x4f_v4f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  cst   Pointer to the matrix to multiply the input values with
     * @param[in]  src   Pointer to the source array
     * @param[in]  count The number of items in the input arrays
     */
    extern ne10_result_t (*ne10_mulcmatvec_cm4x4f_v4f) (ne10_vec4f_t * dst, const ne10_mat4x4f_t * cst, ne10_vec4f_t * src, ne10_uint32_t count);
    extern ne10_result_t ne10_mulcmatvec_cm4x4f_v4f_c (ne10_vec4f_t * dst, const ne10_mat4x4f_t * cst, ne10_vec4f_t * src, ne10_uint32_t count);
    extern ne10_result_t ne10_mulcmatvec_cm4x4f_v4f_neon (ne10_vec4f_t * dst, const ne10_mat4x4f_t * cst, ne10_vec4f_t * src, ne10_uint32_t count) asm ("ne10_mulcmatvec_cm4x4f_v4f_neon");
    extern ne10_result_t ne10_mulcmatvec_cm4x4f_v4f_asm (ne10_vec4f_t * dst, const ne10_mat4x4f_t * cst, ne10_vec4f_t * src, ne10_uint32_t count);
    /**
     * Matrix multiplication of 3x3 matrix and 3D vector.
     * This function point could be pointed to one of ne10_mulcmatvec_cm3x3f_v3f_c, ne10_mulcmatvec_cm3x3f_v3f_neon and ne10_mulcmatvec_cm3x3f_v3f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  cst   Pointer to the matrix to multiply the input values with
     * @param[in]  src   Pointer to the source array
     * @param[in]  count The number of items in the input arrays
     */
    extern ne10_result_t (*ne10_mulcmatvec_cm3x3f_v3f) (ne10_vec3f_t * dst, const ne10_mat3x3f_t * cst, ne10_vec3f_t * src, ne10_uint32_t count);
    extern ne10_result_t ne10_mulcmatvec_cm3x3f_v3f_c (ne10_vec3f_t * dst, const ne10_mat3x3f_t * cst, ne10_vec3f_t * src, ne10_uint32_t count);
    extern ne10_result_t ne10_mulcmatvec_cm3x3f_v3f_neon (ne10_vec3f_t * dst, const ne10_mat3x3f_t * cst, ne10_vec3f_t * src, ne10_uint32_t count) asm ("ne10_mulcmatvec_cm3x3f_v3f_neon");
    extern ne10_result_t ne10_mulcmatvec_cm3x3f_v3f_asm (ne10_vec3f_t * dst, const ne10_mat3x3f_t * cst, ne10_vec3f_t * src, ne10_uint32_t count);
    /**
     * Matrix multiplication of 2x2 matrix and 2D vector.
     * This function point could be pointed to one of ne10_mulcmatvec_cm2x2f_v2f_c, ne10_mulcmatvec_cm2x2f_v2f_neon and ne10_mulcmatvec_cm2x2f_v2f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  cst   Pointer to the matrix to multiply the input values with
     * @param[in]  src   Pointer to the source array
     * @param[in]  count The number of items in the input arrays
     */
    extern ne10_result_t (*ne10_mulcmatvec_cm2x2f_v2f) (ne10_vec2f_t * dst, const ne10_mat2x2f_t * cst, ne10_vec2f_t * src, ne10_uint32_t count);
    extern ne10_result_t ne10_mulcmatvec_cm2x2f_v2f_c (ne10_vec2f_t * dst, const ne10_mat2x2f_t * cst, ne10_vec2f_t * src, ne10_uint32_t count);
    extern ne10_result_t ne10_mulcmatvec_cm2x2f_v2f_neon (ne10_vec2f_t * dst, const ne10_mat2x2f_t * cst, ne10_vec2f_t * src, ne10_uint32_t count) asm ("ne10_mulcmatvec_cm2x2f_v2f_neon");
    extern ne10_result_t ne10_mulcmatvec_cm2x2f_v2f_asm (ne10_vec2f_t * dst, const ne10_mat2x2f_t * cst, ne10_vec2f_t * src, ne10_uint32_t count);

    /** @} */ //end of Matrix Vector Multiply group

    /**
     * @ingroup groupMaths
     */

    /**
     * @defgroup DIV_VEC Vector Div
     *
     * \par
     * These functions implement the vector division operation for float data type.
     */

    /**
     * @addtogroup DIV_VEC
     * @{
     */

    /**
     * Divides the elements of an input array by a constant scalar and stores the results in an output array.
     * This function point could be pointed to one of ne10_divc_float_c, ne10_divc_float_neon and ne10_divc_float_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  cst   The constant scalar to divide the input values by
     * @param[in]  count The number of items in the input array
     */
    extern ne10_result_t (*ne10_divc_float) (ne10_float32_t * dst, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count);
    extern ne10_result_t ne10_divc_float_c (ne10_float32_t * dst, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count);
    extern ne10_result_t ne10_divc_float_neon (ne10_float32_t * dst, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count);
    extern ne10_result_t ne10_divc_float_asm (ne10_float32_t * dst, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count);
    /**
     * Divides the components of 2D vectors in an input array with the components of a constant 2D vector and stores the results in an output array.
     * This function point could be pointed to one of ne10_divc_vec2f_c, ne10_divc_vec2f_neon and ne10_divc_vec2f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  cst   Pointer to the 2D vector to divide the input values by
     * @param[in]  count The number of items in the input array
     */
    extern ne10_result_t (*ne10_divc_vec2f) (ne10_vec2f_t * dst, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count);
    extern ne10_result_t ne10_divc_vec2f_c (ne10_vec2f_t * dst, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count);
    extern ne10_result_t ne10_divc_vec2f_neon (ne10_vec2f_t * dst, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count);
    extern ne10_result_t ne10_divc_vec2f_asm (ne10_vec2f_t * dst, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count);
    /**
     * Divides the components of 3D vectors in an input array with the components of a constant 3D vector and stores the results in an output array.
     * This function point could be pointed to one of ne10_divc_vec3f_c, ne10_divc_vec3f_neon and ne10_divc_vec3f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  cst   Pointer to the 3D vector to divide the input values by
     * @param[in]  count The number of items in the input array
     */
    extern ne10_result_t (*ne10_divc_vec3f) (ne10_vec3f_t * dst, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count);
    extern ne10_result_t ne10_divc_vec3f_c (ne10_vec3f_t * dst, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count);
    extern ne10_result_t ne10_divc_vec3f_neon (ne10_vec3f_t * dst, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count);
    extern ne10_result_t ne10_divc_vec3f_asm (ne10_vec3f_t * dst, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count);
    /**
     * Divides the components of 4D vectors in an input array with the components of a constant 4D vector and stores the results in an output array.
     * This function point could be pointed to one of ne10_divc_vec4f_c, ne10_divc_vec4f_neon and ne10_divc_vec4f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  cst   Pointer to the 4D vector to divide the input values by
     * @param[in]  count The number of items in the input array
     */
    extern ne10_result_t (*ne10_divc_vec4f) (ne10_vec4f_t * dst, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count);
    extern ne10_result_t ne10_divc_vec4f_c (ne10_vec4f_t * dst, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count);
    extern ne10_result_t ne10_divc_vec4f_neon (ne10_vec4f_t * dst, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count);
    extern ne10_result_t ne10_divc_vec4f_asm (ne10_vec4f_t * dst, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count);
    /**
     *  Divides the elements of src1 by the elements of src2 and stores the results in the dst.
     * This function point could be pointed to one of ne10_div_float_c, ne10_div_float_neon and ne10_div_float_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src1  The first array to use as the input array
     * @param[in]  src2  The second array to use as the input array
     * @param[in]  count The number of items in the two input arrays
     */
    extern ne10_result_t (*ne10_div_float) (ne10_float32_t * dst, ne10_float32_t * src1, ne10_float32_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_div_float_c (ne10_float32_t * dst, ne10_float32_t * src1, ne10_float32_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_div_float_neon (ne10_float32_t * dst, ne10_float32_t * src1, ne10_float32_t * src2, ne10_uint32_t count) asm ("ne10_div_float_neon");
    extern ne10_result_t ne10_div_float_asm (ne10_float32_t * dst, ne10_float32_t * src1, ne10_float32_t * src2, ne10_uint32_t count);
    /**
     * Divides the components of a 2D vector with the corresponding components of another.
     * This function point could be pointed to one of ne10_vdiv_vec2f_c, ne10_vdiv_vec2f_neon and ne10_vdiv_vec2f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src1   Pointer to the nominators' source array
     * @param[in]  src2   Pointer to the denominators' source array
     * @param[in]  count The number of items in the input arrays
     */
    extern ne10_result_t (*ne10_vdiv_vec2f) (ne10_vec2f_t * dst, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_vdiv_vec2f_c (ne10_vec2f_t * dst, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_vdiv_vec2f_neon (ne10_vec2f_t * dst, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count) asm ("ne10_vdiv_vec2f_neon");
    extern ne10_result_t ne10_vdiv_vec2f_asm (ne10_vec2f_t * dst, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count);
    /**
     * Divides the components of a 3D vector with the corresponding components of another.
     * This function point could be pointed to one of ne10_vdiv_vec3f_c, ne10_vdiv_vec3f_neon and ne10_vdiv_vec3f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src1   Pointer to the nominators' source array
     * @param[in]  src2   Pointer to the denominators' source array
     * @param[in]  count The number of items in the input arrays
     */
    extern ne10_result_t (*ne10_vdiv_vec3f) (ne10_vec3f_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_vdiv_vec3f_c (ne10_vec3f_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_vdiv_vec3f_neon (ne10_vec3f_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count) asm ("ne10_vdiv_vec3f_neon");
    extern ne10_result_t ne10_vdiv_vec3f_asm (ne10_vec3f_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);
    /**
     * Divides the components of a 4D vector with the corresponding components of another.
     * This function point could be pointed to one of ne10_vdiv_vec4f_c, ne10_vdiv_vec4f_neon and ne10_vdiv_vec4f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src1   Pointer to the nominators' source array
     * @param[in]  src2   Pointer to the denominators' source array
     * @param[in]  count The number of items in the input arrays
     */
    extern ne10_result_t (*ne10_vdiv_vec4f) (ne10_vec4f_t * dst, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_vdiv_vec4f_c (ne10_vec4f_t * dst, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_vdiv_vec4f_neon (ne10_vec4f_t * dst, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count) asm ("ne10_vdiv_vec4f_neon");
    extern ne10_result_t ne10_vdiv_vec4f_asm (ne10_vec4f_t * dst, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count);
    /** @} */ //end of Vector Div group

    /**
     * @ingroup groupMaths
     */

    /**
     * @defgroup DIV_MAT Matrix Div
     *
     * \par
     * These functions implement the matrix division operation for float data type.
     */

    /**
     * @addtogroup DIV_MAT
     * @{
     */

    /**
     * Divides the components of a 4x4 matrix with the corresponding components of another.
     * This function point could be pointed to one of ne10_divmat_4x4f_c, ne10_divmat_4x4f_neon and ne10_divmat_4x4f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src1   Pointer to the nominators' source array
     * @param[in]  src2   Pointer to the denominators' source array
     * @param[in]  count The number of items in the input arrays
     */
    extern ne10_result_t (*ne10_divmat_4x4f) (ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src1, ne10_mat4x4f_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_divmat_4x4f_c (ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src1, ne10_mat4x4f_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_divmat_4x4f_neon (ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src1, ne10_mat4x4f_t * src2, ne10_uint32_t count) asm ("ne10_divmat_4x4f_neon");
    extern ne10_result_t ne10_divmat_4x4f_asm (ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src1, ne10_mat4x4f_t * src2, ne10_uint32_t count);
    /**
     * Divides the components of a 3x3 matrix with the corresponding components of another.
     * This function point could be pointed to one of ne10_divmat_3x3f_c, ne10_divmat_3x3f_neon and ne10_divmat_3x3f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src1   Pointer to the nominators' source array
     * @param[in]  src2   Pointer to the denominators' source array
     * @param[in]  count The number of items in the input arrays
     */
    extern ne10_result_t (*ne10_divmat_3x3f) (ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src1, ne10_mat3x3f_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_divmat_3x3f_c (ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src1, ne10_mat3x3f_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_divmat_3x3f_neon (ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src1, ne10_mat3x3f_t * src2, ne10_uint32_t count) asm ("ne10_divmat_3x3f_neon");
    extern ne10_result_t ne10_divmat_3x3f_asm (ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src1, ne10_mat3x3f_t * src2, ne10_uint32_t count);
    /**
     * Divides the components of a 2x2 matrix with the corresponding components of another.
     * This function point could be pointed to one of ne10_divmat_2x2f_c, ne10_divmat_2x2f_neon and ne10_divmat_2x2f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src1   Pointer to the nominators' source array
     * @param[in]  src2   Pointer to the denominators' source array
     * @param[in]  count The number of items in the input arrays
     */
    extern ne10_result_t (*ne10_divmat_2x2f) (ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src1, ne10_mat2x2f_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_divmat_2x2f_c (ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src1, ne10_mat2x2f_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_divmat_2x2f_neon (ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src1, ne10_mat2x2f_t * src2, ne10_uint32_t count) asm ("ne10_divmat_2x2f_neon");
    extern ne10_result_t ne10_divmat_2x2f_asm (ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src1, ne10_mat2x2f_t * src2, ne10_uint32_t count);
    /** @} */ //end of Matrix Div group

    /**
     * @ingroup groupMaths
     */

    /**
     * @defgroup SETC_VEC Vector Setc
     *
     * \par
     * These functions implement vector setc operation for float data type.
     */

    /**
     * @addtogroup SETC_VEC
     * @{
     */

    /**
     * Sets the elements of an input array to a constant scalar and stores the results in an output array.
     * This function point could be pointed to one of ne10_setc_float_c, ne10_setc_float_neon and ne10_setc_float_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  cst   The constant scalar to set the input values to
     * @param[in]  count The number of items in the input array
     */
    extern ne10_result_t (*ne10_setc_float) (ne10_float32_t * dst, const ne10_float32_t cst, ne10_uint32_t count);
    extern ne10_result_t ne10_setc_float_c (ne10_float32_t * dst, const ne10_float32_t cst, ne10_uint32_t count);
    extern ne10_result_t ne10_setc_float_neon (ne10_float32_t * dst, const ne10_float32_t cst, ne10_uint32_t count);
    extern ne10_result_t ne10_setc_float_asm (ne10_float32_t * dst, const ne10_float32_t cst, ne10_uint32_t count);
    /**
     * Sets the components of 2D vectors in an input array to the components of a constant 2D vector and stores the results in an output array.
     * This function point could be pointed to one of ne10_setc_vec2f_c, ne10_setc_vec2f_neon and ne10_setc_vec2f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  cst   Pointer to the 2D vector to set the input values to
     * @param[in]  count The number of items in the input array
     */
    extern ne10_result_t (*ne10_setc_vec2f) (ne10_vec2f_t * dst, const ne10_vec2f_t * cst, ne10_uint32_t count);
    extern ne10_result_t ne10_setc_vec2f_c (ne10_vec2f_t * dst, const ne10_vec2f_t * cst, ne10_uint32_t count);
    extern ne10_result_t ne10_setc_vec2f_neon (ne10_vec2f_t * dst, const ne10_vec2f_t * cst, ne10_uint32_t count);
    extern ne10_result_t ne10_setc_vec2f_asm (ne10_vec2f_t * dst, const ne10_vec2f_t * cst, ne10_uint32_t count);
    /**
     * Sets the components of 3D vectors in an input array to the components of a constant 3D vector and stores the results in an output array.
     * This function point could be pointed to one of ne10_setc_vec3f_c, ne10_setc_vec3f_neon and ne10_setc_vec3f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  cst   Pointer to the 3D vector to set the input values to
     * @param[in]  count The number of items in the input array
     */
    extern ne10_result_t (*ne10_setc_vec3f) (ne10_vec3f_t * dst, const ne10_vec3f_t * cst, ne10_uint32_t count);
    extern ne10_result_t ne10_setc_vec3f_c (ne10_vec3f_t * dst, const ne10_vec3f_t * cst, ne10_uint32_t count);
    extern ne10_result_t ne10_setc_vec3f_neon (ne10_vec3f_t * dst, const ne10_vec3f_t * cst, ne10_uint32_t count);
    extern ne10_result_t ne10_setc_vec3f_asm (ne10_vec3f_t * dst, const ne10_vec3f_t * cst, ne10_uint32_t count);
    /**
     * Sets the components of 4D vectors in an input array to the components of a constant 3D vector and stores the results in an output array.
     * This function point could be pointed to one of ne10_setc_vec4f_c, ne10_setc_vec4f_neon and ne10_setc_vec4f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  cst   Pointer to the 4D vector to set the input values to
     * @param[in]  count The number of items in the input array
     */
    extern ne10_result_t (*ne10_setc_vec4f) (ne10_vec4f_t * dst, const ne10_vec4f_t * cst, ne10_uint32_t count);
    extern ne10_result_t ne10_setc_vec4f_c (ne10_vec4f_t * dst, const ne10_vec4f_t * cst, ne10_uint32_t count);
    extern ne10_result_t ne10_setc_vec4f_neon (ne10_vec4f_t * dst, const ne10_vec4f_t * cst, ne10_uint32_t count);
    extern ne10_result_t ne10_setc_vec4f_asm (ne10_vec4f_t * dst, const ne10_vec4f_t * cst, ne10_uint32_t count);
    /** @} */ //end of Vector Setc group

    /**
     * @ingroup groupMaths
     */

    /**
     * @defgroup LEN_VEC Vector Len
     *
     * \par
     * These functions implement vector len operation for float data type.
     */

    /**
     * @addtogroup LEN_VEC
     * @{
     */
    /**
     * Returns length of 2D vectors in corresponding elements of the output array.
     * This function point could be pointed to one of ne10_len_vec2f_c, ne10_len_vec2f_neon and ne10_len_vec2f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  count The number of items in the input array
     */
    extern ne10_result_t (*ne10_len_vec2f) (ne10_float32_t * dst, ne10_vec2f_t * src, ne10_uint32_t count);
    extern ne10_result_t ne10_len_vec2f_c (ne10_float32_t * dst, ne10_vec2f_t * src, ne10_uint32_t count);
    extern ne10_result_t ne10_len_vec2f_neon (ne10_float32_t * dst, ne10_vec2f_t * src, ne10_uint32_t count) asm ("ne10_len_vec2f_neon");
    extern ne10_result_t ne10_len_vec2f_asm (ne10_float32_t * dst, ne10_vec2f_t * src, ne10_uint32_t count);
    /**
     * Returns length of 3D vectors in corresponding elements of the output array.
     * This function point could be pointed to one of ne10_len_vec3f_c, ne10_len_vec3f_neon and ne10_len_vec3f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  count The number of items in the input array
     */
    extern ne10_result_t (*ne10_len_vec3f) (ne10_float32_t * dst, ne10_vec3f_t * src, ne10_uint32_t count);
    extern ne10_result_t ne10_len_vec3f_c (ne10_float32_t * dst, ne10_vec3f_t * src, ne10_uint32_t count);
    extern ne10_result_t ne10_len_vec3f_neon (ne10_float32_t * dst, ne10_vec3f_t * src, ne10_uint32_t count) asm ("ne10_len_vec3f_neon");
    extern ne10_result_t ne10_len_vec3f_asm (ne10_float32_t * dst, ne10_vec3f_t * src, ne10_uint32_t count);
    /**
     * Returns length of 4D vectors in corresponding elements of the output array.
     * This function point could be pointed to one of ne10_len_vec4f_c, ne10_len_vec4f_neon and ne10_len_vec4f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  count The number of items in the input array
     */
    extern ne10_result_t (*ne10_len_vec4f) (ne10_float32_t * dst, ne10_vec4f_t * src, ne10_uint32_t count);
    extern ne10_result_t ne10_len_vec4f_c (ne10_float32_t * dst, ne10_vec4f_t * src, ne10_uint32_t count);
    extern ne10_result_t ne10_len_vec4f_neon (ne10_float32_t * dst, ne10_vec4f_t * src, ne10_uint32_t count) asm ("ne10_len_vec4f_neon");
    extern ne10_result_t ne10_len_vec4f_asm (ne10_float32_t * dst, ne10_vec4f_t * src, ne10_uint32_t count);
    /** @} */ //end of Vector Len group


    /**
     * @ingroup groupMaths
     */

    /**
     * @defgroup NORM_VEC Vector Normalize
     *
     * \par
     * These functions implement vector normalize operation for float data type.
     */

    /**
     * @addtogroup NORM_VEC
     * @{
     */
    /**
     * Normalizes 2D vectors of the input array and stores them in the corresponding elements of the output array.
     * This function point could be pointed to one of ne10_normalize_vec2f_c, ne10_normalize_vec2f_neon and ne10_normalize_vec2f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  count The number of items in the input array
     */
    extern ne10_result_t (*ne10_normalize_vec2f) (ne10_vec2f_t * dst, ne10_vec2f_t * src, ne10_uint32_t count);
    extern ne10_result_t ne10_normalize_vec2f_c (ne10_vec2f_t * dst, ne10_vec2f_t * src, ne10_uint32_t count);
    extern ne10_result_t ne10_normalize_vec2f_neon (ne10_vec2f_t * dst, ne10_vec2f_t * src, ne10_uint32_t count) asm ("ne10_normalize_vec2f_neon");
    extern ne10_result_t ne10_normalize_vec2f_asm (ne10_vec2f_t * dst, ne10_vec2f_t * src, ne10_uint32_t count);
    /**
     * Normalizes 3D vectors of the input array and stores them in the corresponding elements of the output array.
     * This function point could be pointed to one of ne10_normalize_vec3f_c, ne10_normalize_vec3f_neon and ne10_normalize_vec3f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  count The number of items in the input array
     */
    extern ne10_result_t (*ne10_normalize_vec3f) (ne10_vec3f_t * dst, ne10_vec3f_t * src, ne10_uint32_t count);
    extern ne10_result_t ne10_normalize_vec3f_c (ne10_vec3f_t * dst, ne10_vec3f_t * src, ne10_uint32_t count);
    extern ne10_result_t ne10_normalize_vec3f_neon (ne10_vec3f_t * dst, ne10_vec3f_t * src, ne10_uint32_t count) asm ("ne10_normalize_vec3f_neon");
    extern ne10_result_t ne10_normalize_vec3f_asm (ne10_vec3f_t * dst, ne10_vec3f_t * src, ne10_uint32_t count);
    /**
     * Normalizes 4D vectors of the input array and stores them in the corresponding elements of the output array.
     * This function point could be pointed to one of ne10_normalize_vec4f_c, ne10_normalize_vec4f_neon and ne10_normalize_vec4f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  count The number of items in the input array
     */
    extern ne10_result_t (*ne10_normalize_vec4f) (ne10_vec4f_t * dst, ne10_vec4f_t * src, ne10_uint32_t count);
    extern ne10_result_t ne10_normalize_vec4f_c (ne10_vec4f_t * dst, ne10_vec4f_t * src, ne10_uint32_t count);
    extern ne10_result_t ne10_normalize_vec4f_neon (ne10_vec4f_t * dst, ne10_vec4f_t * src, ne10_uint32_t count) asm ("ne10_normalize_vec4f_neon");
    extern ne10_result_t ne10_normalize_vec4f_asm (ne10_vec4f_t * dst, ne10_vec4f_t * src, ne10_uint32_t count);
    /** @} */ //end of Vector Normalize group


    /**
     * @ingroup groupMaths
     */

    /**
     * @defgroup ABS_VEC Vector Abs
     *
     * \par
     * These functions implement vector abs operation for float data type.
     */

    /**
     * @addtogroup ABS_VEC
     * @{
     */

    /**
     * Calculates the absolute value of each element in the source array and stores the result in the corresponding entry of the destination array.
     * This function point could be pointed to one of ne10_abs_float_c, ne10_abs_float_neon and ne10_abs_float_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  count The number of items in the input array
     */
    extern ne10_result_t (*ne10_abs_float) (ne10_float32_t * dst, ne10_float32_t * src, ne10_uint32_t count);
    extern ne10_result_t ne10_abs_float_c (ne10_float32_t * dst, ne10_float32_t * src, ne10_uint32_t count);
    extern ne10_result_t ne10_abs_float_neon (ne10_float32_t * dst, ne10_float32_t * src, ne10_uint32_t count) asm ("ne10_abs_float_neon");
    extern ne10_result_t ne10_abs_float_asm (ne10_float32_t * dst, ne10_float32_t * src, ne10_uint32_t count);
    /**
     * Generates a 2D vector from the absolute values of each of the components of an input vector.
     * This function point could be pointed to one of ne10_abs_vec2f_c, ne10_abs_vec2f_neon and ne10_abs_vec2f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  count The number of items in the input array
     */
    extern ne10_result_t (*ne10_abs_vec2f) (ne10_vec2f_t * dst, ne10_vec2f_t * src, ne10_uint32_t count);
    extern ne10_result_t ne10_abs_vec2f_c (ne10_vec2f_t * dst, ne10_vec2f_t * src, ne10_uint32_t count);
    extern ne10_result_t ne10_abs_vec2f_neon (ne10_vec2f_t * dst, ne10_vec2f_t * src, ne10_uint32_t count) asm ("ne10_abs_vec2f_neon");
    extern ne10_result_t ne10_abs_vec2f_asm (ne10_vec2f_t * dst, ne10_vec2f_t * src, ne10_uint32_t count);
    /**
     * Generates a 3D vector from the absolute values of each of the components of an input vector.
     * This function point could be pointed to one of ne10_abs_vec3f_c, ne10_abs_vec3f_neon and ne10_abs_vec3f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  count The number of items in the input array
     */
    extern ne10_result_t (*ne10_abs_vec3f) (ne10_vec3f_t * dst, ne10_vec3f_t * src, ne10_uint32_t count);
    extern ne10_result_t ne10_abs_vec3f_c (ne10_vec3f_t * dst, ne10_vec3f_t * src, ne10_uint32_t count);
    extern ne10_result_t ne10_abs_vec3f_neon (ne10_vec3f_t * dst, ne10_vec3f_t * src, ne10_uint32_t count) asm ("ne10_abs_vec3f_neon");
    extern ne10_result_t ne10_abs_vec3f_asm (ne10_vec3f_t * dst, ne10_vec3f_t * src, ne10_uint32_t count);
    /**
     * Generates a 4D vector from the absolute values of each of the components of an input vector.
     * This function point could be pointed to one of ne10_abs_vec4f_c, ne10_abs_vec4f_neon and ne10_abs_vec4f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  count The number of items in the input array
     */
    extern ne10_result_t (*ne10_abs_vec4f) (ne10_vec4f_t * dst, ne10_vec4f_t * src, ne10_uint32_t count);
    extern ne10_result_t ne10_abs_vec4f_c (ne10_vec4f_t * dst, ne10_vec4f_t * src, ne10_uint32_t count);
    extern ne10_result_t ne10_abs_vec4f_neon (ne10_vec4f_t * dst, ne10_vec4f_t * src, ne10_uint32_t count) asm ("ne10_abs_vec4f_neon");
    extern ne10_result_t ne10_abs_vec4f_asm (ne10_vec4f_t * dst, ne10_vec4f_t * src, ne10_uint32_t count);
    /** @} */ //end of Vector Abs group

    /**
     * @ingroup groupMaths
     */

    /**
     * @defgroup DOT_VEC Vector Dot
     *
     * \par
     * These functions implement vector dot operation for float data type.
     */

    /**
     * @addtogroup DOT_VEC
     * @{
     */
    /**
     * Dot product of two 2D vectors.
     * This function point could be pointed to one of ne10_dot_vec2f_c, ne10_dot_vec2f_neon and ne10_dot_vec2f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src1   Pointer to the first source array
     * @param[in]  src2   Pointer to the second source array
     * @param[in]  count The number of items in the input arrays
     */
    extern ne10_result_t (*ne10_dot_vec2f) (ne10_float32_t * dst, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_dot_vec2f_c (ne10_float32_t * dst, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_dot_vec2f_neon (ne10_float32_t * dst, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count) asm ("ne10_dot_vec2f_neon");
    extern ne10_result_t ne10_dot_vec2f_asm (ne10_float32_t * dst, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count);
    /**
     * Dot product of two 3D vectors.
     * This function point could be pointed to one of ne10_dot_vec3f_c, ne10_dot_vec3f_neon and ne10_dot_vec3f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src1   Pointer to the first source array
     * @param[in]  src2   Pointer to the second source array
     * @param[in]  count The number of items in the input arrays
     */
    extern ne10_result_t (*ne10_dot_vec3f) (ne10_float32_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_dot_vec3f_c (ne10_float32_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_dot_vec3f_neon (ne10_float32_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count) asm ("ne10_dot_vec3f_neon");
    extern ne10_result_t ne10_dot_vec3f_asm (ne10_float32_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);
    /**
     * Dot product of two 4D vectors.
     * This function point could be pointed to one of ne10_dot_vec4f_c, ne10_dot_vec4f_neon and ne10_dot_vec4f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src1   Pointer to the first source array
     * @param[in]  src2   Pointer to the second source array
     * @param[in]  count The number of items in the input arrays
     */
    extern ne10_result_t (*ne10_dot_vec4f) (ne10_float32_t * dst, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_dot_vec4f_c (ne10_float32_t * dst, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_dot_vec4f_neon (ne10_float32_t * dst, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count) asm ("ne10_dot_vec4f_neon");
    extern ne10_result_t ne10_dot_vec4f_asm (ne10_float32_t * dst, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count);
    /** @} */ //end of Vector Dot group


    /**
     * @ingroup groupMaths
     */

    /**
     * @defgroup CROSS_VEC Vector Cross
     *
     * \par
     * These functions implement vector cross operation for float data type.
     */

    /**
     * @addtogroup CROSS_VEC
     * @{
     */

    /**
     * Performs a cross product operation on the two input vectors.
     * This function point could be pointed to one of ne10_cross_vec3f_c, ne10_cross_vec3f_neon and ne10_cross_vec3f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src1   Pointer to the first source array
     * @param[in]  src2   Pointer to the second source array
     * @param[in]  count The number of items in the input arrays
     */
    extern ne10_result_t (*ne10_cross_vec3f) (ne10_vec3f_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_cross_vec3f_c (ne10_vec3f_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);
    extern ne10_result_t ne10_cross_vec3f_neon (ne10_vec3f_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count) asm ("ne10_cross_vec3f_neon");
    extern ne10_result_t ne10_cross_vec3f_asm (ne10_vec3f_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);
    /** @} */ //end of Vector Cross group

    /**
     * @ingroup groupMaths
     */

    /**
     * @defgroup DET_MAT Matrix Determinant
     *
     * \par
     * These functions implement matrix determinant operation for float data type.
     */

    /**
     * @addtogroup DET_MAT
     * @{
     */

    /**
     * Calculate the determinant of a 4x4 matrix.
     * This function point could be pointed to one of ne10_detmat_4x4f_c, ne10_detmat_4x4f_neon and ne10_detmat_4x4f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  count The number of items in the input array
     */
    extern ne10_result_t (*ne10_detmat_4x4f) (ne10_float32_t * dst, ne10_mat4x4f_t * src, ne10_uint32_t count);
    extern ne10_result_t ne10_detmat_4x4f_c (ne10_float32_t * dst, ne10_mat4x4f_t * src, ne10_uint32_t count);
    extern ne10_result_t ne10_detmat_4x4f_neon (ne10_float32_t * dst, ne10_mat4x4f_t * src, ne10_uint32_t count) asm ("ne10_detmat_4x4f_neon");
    extern ne10_result_t ne10_detmat_4x4f_asm (ne10_float32_t * dst, ne10_mat4x4f_t * src, ne10_uint32_t count);
    /**
     * Calculate the determinant of a 3x3 matrix.
     * This function point could be pointed to one of ne10_detmat_3x3f_c, ne10_detmat_3x3f_neon and ne10_detmat_3x3f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  count The number of items in the input array
     */
    extern ne10_result_t (*ne10_detmat_3x3f) (ne10_float32_t * dst, ne10_mat3x3f_t * src, ne10_uint32_t count);
    extern ne10_result_t ne10_detmat_3x3f_c (ne10_float32_t * dst, ne10_mat3x3f_t * src, ne10_uint32_t count);
    extern ne10_result_t ne10_detmat_3x3f_neon (ne10_float32_t * dst, ne10_mat3x3f_t * src, ne10_uint32_t count) asm ("ne10_detmat_3x3f_neon");
    extern ne10_result_t ne10_detmat_3x3f_asm (ne10_float32_t * dst, ne10_mat3x3f_t * src, ne10_uint32_t count);
    /**
     * Calculate the determinant of a 2x2 matrix.
     * This function point could be pointed to one of ne10_detmat_2x2f_c, ne10_detmat_2x2f_neon and ne10_detmat_2x2f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  count The number of items in the input array
     */
    extern ne10_result_t (*ne10_detmat_2x2f) (ne10_float32_t * dst, ne10_mat2x2f_t * src, ne10_uint32_t count);
    extern ne10_result_t ne10_detmat_2x2f_c (ne10_float32_t * dst, ne10_mat2x2f_t * src, ne10_uint32_t count);
    extern ne10_result_t ne10_detmat_2x2f_neon (ne10_float32_t * dst, ne10_mat2x2f_t * src, ne10_uint32_t count) asm ("ne10_detmat_2x2f_neon");
    extern ne10_result_t ne10_detmat_2x2f_asm (ne10_float32_t * dst, ne10_mat2x2f_t * src, ne10_uint32_t count);
    /** @} */ //end of Matrix Determinant group

    /**
     * @ingroup groupMaths
     */

    /**
     * @defgroup INV_MAT Matrix Invertible
     *
     * \par
     * These functions implement matrix invertible operation for float data type.
     */

    /**
     * @addtogroup INV_MAT
     * @{
     */
    /**
     * Calculate the invertible matrix of a 4x4 matrix.
     * This function point could be pointed to one of ne10_invmat_4x4f_c, ne10_invmat_4x4f_neon and ne10_invmat_4x4f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  count The number of items in the input array
     */
    extern ne10_result_t (*ne10_invmat_4x4f) (ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src, ne10_uint32_t count);
    extern ne10_result_t ne10_invmat_4x4f_c (ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src, ne10_uint32_t count);
    extern ne10_result_t ne10_invmat_4x4f_neon (ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src, ne10_uint32_t count) asm ("ne10_invmat_4x4f_neon");
    extern ne10_result_t ne10_invmat_4x4f_asm (ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src, ne10_uint32_t count);
    /**
     * Calculate the invertible matrix of a 3x3 matrix.
     * This function point could be pointed to one of ne10_invmat_3x3f_c, ne10_invmat_3x3f_neon and ne10_invmat_3x3f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  count The number of items in the input array
     */
    extern ne10_result_t (*ne10_invmat_3x3f) (ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src, ne10_uint32_t count);
    extern ne10_result_t ne10_invmat_3x3f_c (ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src, ne10_uint32_t count);
    extern ne10_result_t ne10_invmat_3x3f_neon (ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src, ne10_uint32_t count) asm ("ne10_invmat_3x3f_neon");
    extern ne10_result_t ne10_invmat_3x3f_asm (ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src, ne10_uint32_t count);
    /**
     * Calculate the invertible matrix of a 2x2 matrix.
     * This function point could be pointed to one of ne10_invmat_2x2f_c, ne10_invmat_2x2f_neon and ne10_invmat_2x2f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  count The number of items in the input array
     */
    extern ne10_result_t (*ne10_invmat_2x2f) (ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src, ne10_uint32_t count);
    extern ne10_result_t ne10_invmat_2x2f_c (ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src, ne10_uint32_t count);
    extern ne10_result_t ne10_invmat_2x2f_neon (ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src, ne10_uint32_t count) asm ("ne10_invmat_2x2f_neon");
    extern ne10_result_t ne10_invmat_2x2f_asm (ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src, ne10_uint32_t count);
    /** @} */ //end of Matrix Invertible group

    /**
     * @ingroup groupMaths
     */

    /**
     * @defgroup TRANS_MAT Matrix Transpose
     *
     * \par
     * These functions implement matrix transpose operation for float data type.
     */

    /**
     * @addtogroup TRANS_MAT
     * @{
     */
    /**
     * Calculate the transpose matrix of a 4x4 matrix.
     * This function point could be pointed to one of ne10_transmat_4x4f_c, ne10_transmat_4x4f_neon and ne10_transmat_4x4f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  count The number of items in the input array
     */
    extern ne10_result_t (*ne10_transmat_4x4f) (ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src, ne10_uint32_t count);
    extern ne10_result_t ne10_transmat_4x4f_c (ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src, ne10_uint32_t count);
    extern ne10_result_t ne10_transmat_4x4f_neon (ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src, ne10_uint32_t count) asm ("ne10_transmat_4x4f_neon");
    extern ne10_result_t ne10_transmat_4x4f_asm (ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src, ne10_uint32_t count);
    /**
     * Calculate the transpose matrix of a 4x4 matrix.
     * This function point could be pointed to one of ne10_transmat_4x4f_c, ne10_transmat_4x4f_neon and ne10_transmat_4x4f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  count The number of items in the input array
     */
    extern ne10_result_t (*ne10_transmat_3x3f) (ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src, ne10_uint32_t count);
    extern ne10_result_t ne10_transmat_3x3f_c (ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src, ne10_uint32_t count);
    extern ne10_result_t ne10_transmat_3x3f_neon (ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src, ne10_uint32_t count) asm ("ne10_transmat_3x3f_neon");
    extern ne10_result_t ne10_transmat_3x3f_asm (ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src, ne10_uint32_t count);
    /**
     * Calculate the transpose matrix of a 3x3 matrix.
     * This function point could be pointed to one of ne10_transmat_3x3f_c, ne10_transmat_3x3f_neon and ne10_transmat_3x3f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  count The number of items in the input array
     */
    extern ne10_result_t (*ne10_transmat_2x2f) (ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src, ne10_uint32_t count);
    extern ne10_result_t ne10_transmat_2x2f_c (ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src, ne10_uint32_t count);
    extern ne10_result_t ne10_transmat_2x2f_neon (ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src, ne10_uint32_t count) asm ("ne10_transmat_2x2f_neon");
    extern ne10_result_t ne10_trans_mat2x2f_asm (ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src, ne10_uint32_t count);
    /** @} */ //end of Matrix Transpose group

    /**
     * @ingroup groupMaths
     */

    /**
     * @defgroup IDENTITY_MAT Matrix Identity
     *
     * \par
     * These functions implement matrix identity operation for float data type.
     */

    /**
     * @addtogroup IDENTITY_MAT
     * @{
     */
    /**
     * Set the identity matrix of a 2x2 matrix.
     * This function point could be pointed to one of ne10_identitymat_2x2f_c, ne10_identitymat_2x2f_neon and ne10_identitymat_2x2f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  count The number of items in the input array
     */
    extern ne10_result_t (*ne10_identitymat_4x4f) (ne10_mat4x4f_t * dst, ne10_uint32_t count);
    extern ne10_result_t ne10_identitymat_4x4f_c (ne10_mat4x4f_t * dst, ne10_uint32_t count);
    extern ne10_result_t ne10_identitymat_4x4f_neon (ne10_mat4x4f_t * dst, ne10_uint32_t count) asm ("ne10_identitymat_4x4f_neon");
    extern ne10_result_t ne10_identitymat_4x4f_asm (ne10_mat4x4f_t * dst, ne10_uint32_t count);
    /**
     * Set the identity matrix of a 3x3 matrix.
     * This function point could be pointed to one of ne10_identitymat_3x3f_c, ne10_identitymat_3x3f_neon and ne10_identitymat_3x3f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  count The number of items in the input array
     */
    extern ne10_result_t (*ne10_identitymat_3x3f) (ne10_mat3x3f_t * dst, ne10_uint32_t count);
    extern ne10_result_t ne10_identitymat_3x3f_c (ne10_mat3x3f_t * dst, ne10_uint32_t count);
    extern ne10_result_t ne10_identitymat_3x3f_neon (ne10_mat3x3f_t * dst, ne10_uint32_t count) asm ("ne10_identitymat_3x3f_neon");
    extern ne10_result_t ne10_identitymat_3x3f_asm (ne10_mat3x3f_t * dst, ne10_uint32_t count);
    /**
     * Set the identity matrix of a 2x2 matrix.
     * This function point could be pointed to one of ne10_identitymat_2x2f_c, ne10_identitymat_2x2f_neon and ne10_identitymat_2x2f_asm.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  count The number of items in the input array
     */
    extern ne10_result_t (*ne10_identitymat_2x2f) (ne10_mat2x2f_t * dst, ne10_uint32_t count);
    extern ne10_result_t ne10_identitymat_2x2f_c (ne10_mat2x2f_t * dst, ne10_uint32_t count);
    extern ne10_result_t ne10_identitymat_2x2f_neon (ne10_mat2x2f_t * dst, ne10_uint32_t count) asm ("ne10_identitymat_2x2f_neon");
    extern ne10_result_t ne10_identity_mat2x2f_asm (ne10_mat2x2f_t * dst, ne10_uint32_t count);
    /** @} */ //end of Matrix Identity group

#ifdef __cplusplus
}
#endif

#endif
