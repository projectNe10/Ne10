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

/*
 * NE10 Library : inc/NE10_math.h
 */

#include "NE10_types.h"

#ifndef NE10_MATH_H
#define NE10_MATH_H

#ifdef __cplusplus
extern "C" {
#endif
    /**
     * @ingroup groupMaths
     * @defgroup ADD_VEC Vector Addition
     *
     * \par
     * These functions implement vector addition operations for single precision floating point values.
     * @{
     */

    /**
     * Adds a constant scalar value to all elements of an input array, storing the results
     * in an output array. Points to @ref ne10_addc_float_c, @ref ne10_addc_float_neon, or
     * @ref ne10_addc_float_asm. This operation can be performed in-place.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  cst   The constant to add
     * @param[in]  count The number of scalar values to be processed
     */
    extern ne10_result_t (*ne10_addc_float) (ne10_float32_t * dst, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_addc_float using plain C code. */
    extern ne10_result_t ne10_addc_float_c (ne10_float32_t * dst, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_addc_float using NEON intrinsics. */
    extern ne10_result_t ne10_addc_float_neon (ne10_float32_t * dst, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count) asm ("ne10_addc_float_neon");
    /** Specific implementation of @ref ne10_addc_float using NEON assembly. */
    extern ne10_result_t ne10_addc_float_asm (ne10_float32_t * dst, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count);

    /**
     * Adds a constant 2D vector to all the vectors in an input array, storing the
     * results in an output array. Points to @ref ne10_addc_vec2f_c, @ref ne10_addc_vec2f_neon, or
     * @ref ne10_addc_vec2f_asm. This operation can be performed in-place.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  cst   Pointer to the constant 2D vector to add
     * @param[in]  count The number of 2D vectors to be processed
     */
    extern ne10_result_t (*ne10_addc_vec2f) (ne10_vec2f_t * dst, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_addc_vec2f using plain C code. */
    extern ne10_result_t ne10_addc_vec2f_c (ne10_vec2f_t * dst, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_addc_vec2f using NEON intrinsics. */
    extern ne10_result_t ne10_addc_vec2f_neon (ne10_vec2f_t * dst, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count) asm ("ne10_addc_vec2f_neon");
    /** Specific implementation of @ref ne10_addc_vec2f using NEON assembly. */
    extern ne10_result_t ne10_addc_vec2f_asm (ne10_vec2f_t * dst, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count);

    /**
     * Adds a constant 3D vector to all the vectors in an input array, storing the
     * results in an output array. Points to @ref ne10_addc_vec3f_c, @ref ne10_addc_vec3f_neon, or
     * @ref ne10_addc_vec3f_asm. This operation can be performed in-place.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  cst   Pointer to the constant 3D vector to add
     * @param[in]  count The number of 3D vectors to be processed
     */
    extern ne10_result_t (*ne10_addc_vec3f) (ne10_vec3f_t * dst, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_addc_vec3f using plain C code. */
    extern ne10_result_t ne10_addc_vec3f_c (ne10_vec3f_t * dst, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_addc_vec3f using NEON intrinsics. */
    extern ne10_result_t ne10_addc_vec3f_neon (ne10_vec3f_t * dst, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count) asm ("ne10_addc_vec3f_neon");
    /** Specific implementation of @ref ne10_addc_vec3f using NEON assembly. */
    extern ne10_result_t ne10_addc_vec3f_asm (ne10_vec3f_t * dst, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count);

    /**
     * Adds a constant 4D vector to all the vectors in an input array, storing the
     * results in an output array. Points to @ref ne10_addc_vec4f_c, @ref ne10_addc_vec4f_neon, or
     * @ref ne10_addc_vec4f_asm. This operation can be performed in-place.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  cst   Pointer to the constant 4D vector to add
     * @param[in]  count The number of 4D vectors to be processed
     */
    extern ne10_result_t (*ne10_addc_vec4f) (ne10_vec4f_t * dst, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_addc_vec4f using plain C code. */
    extern ne10_result_t ne10_addc_vec4f_c (ne10_vec4f_t * dst, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_addc_vec4f using NEON intrinsics. */
    extern ne10_result_t ne10_addc_vec4f_neon (ne10_vec4f_t * dst, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count) asm ("ne10_addc_vec4f_neon");
    /** Specific implementation of @ref ne10_addc_vec4f using NEON assembly. */
    extern ne10_result_t ne10_addc_vec4f_asm (ne10_vec4f_t * dst, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count);

    /**
     * Adds the scalar elements of one input array with those of the same index in another,
     * storing the results in an output array. Points to @ref ne10_add_float_c, @ref ne10_add_float_neon,
     * or @ref ne10_add_float_asm. This operation can be performed in-place.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src1  Pointer to the first source array
     * @param[in]  src2  Pointer to the second source array
     * @param[in]  count The number of additions to be performed (i.e. the length of the source arrays)
     */
    extern ne10_result_t (*ne10_add_float) (ne10_float32_t * dst, ne10_float32_t * src1, ne10_float32_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_add_float using plain C code. */
    extern ne10_result_t ne10_add_float_c (ne10_float32_t * dst, ne10_float32_t * src1, ne10_float32_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_add_float using NEON intrinsics. */
    extern ne10_result_t ne10_add_float_neon (ne10_float32_t * dst, ne10_float32_t * src1, ne10_float32_t * src2, ne10_uint32_t count) asm ("ne10_add_float_neon");
    /** Specific implementation of @ref ne10_add_float using NEON assembly. */
    extern ne10_result_t ne10_add_float_asm (ne10_float32_t * dst, ne10_float32_t * src1, ne10_float32_t * src2, ne10_uint32_t count);

    /**
     * Adds the 2D vectors of one input array with those of the same index in another,
     * storing the results in an output array. Points to @ref ne10_add_vec2f_c, @ref ne10_add_vec2f_neon,
     * or @ref ne10_add_vec2f_asm. This operation can be performed in-place.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src1  Pointer to the first source array
     * @param[in]  src2  Pointer to the second source array
     * @param[in]  count The number of additions to be performed (i.e. the length of the source arrays)
     */
    extern ne10_result_t (*ne10_add_vec2f) (ne10_vec2f_t * dst, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_add_vec2f using plain C code. */
    extern ne10_result_t ne10_add_vec2f_c (ne10_vec2f_t * dst, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_add_vec2f using NEON intrinsics. */
    extern ne10_result_t ne10_add_vec2f_neon (ne10_vec2f_t * dst, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count) asm ("ne10_add_vec2f_neon");
    /** Specific implementation of @ref ne10_add_vec2f using NEON assembly. */
    extern ne10_result_t ne10_add_vec2f_asm (ne10_vec2f_t * dst, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count);

    /**
     * Adds the 3D vectors of one input array with those of the same index in another,
     * storing the results in an output array. Points to @ref ne10_add_vec3f_c, @ref ne10_add_vec3f_neon,
     * or @ref ne10_add_vec3f_asm. This operation can be performed in-place.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src1  Pointer to the first source array
     * @param[in]  src2  Pointer to the second source array
     * @param[in]  count The number of additions to be performed (i.e. the length of the source arrays)
     */
    extern ne10_result_t (*ne10_add_vec3f) (ne10_vec3f_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_add_vec3f using plain C code. */
    extern ne10_result_t ne10_add_vec3f_c (ne10_vec3f_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_add_vec3f using NEON intrinsics. */
    extern ne10_result_t ne10_add_vec3f_neon (ne10_vec3f_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count) asm ("ne10_add_vec3f_neon");
    /** Specific implementation of @ref ne10_add_vec3f using NEON assembly. */
    extern ne10_result_t ne10_add_vec3f_asm (ne10_vec3f_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);

    /**
     * Adds the 4D vectors of one input array with those of the same index in another,
     * storing the results in an output array. Points to @ref ne10_add_vec4f_c, @ref ne10_add_vec4f_neon,
     * or @ref ne10_add_vec4f_asm. This operation can be performed in-place.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src1  Pointer to the first source array
     * @param[in]  src2  Pointer to the second source array
     * @param[in]  count The number of additions to be performed (i.e. the length of the source arrays)
     */
    extern ne10_result_t (*ne10_add_vec4f) (ne10_vec4f_t * dst, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_add_vec4f using plain C code. */
    extern ne10_result_t ne10_add_vec4f_c (ne10_vec4f_t * dst, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_add_vec4f using NEON intrinsics. */
    extern ne10_result_t ne10_add_vec4f_neon (ne10_vec4f_t * dst, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count) asm ("ne10_add_vec4f_neon");
    /** Specific implementation of @ref ne10_add_vec4f using NEON assembly. */
    extern ne10_result_t ne10_add_vec4f_asm (ne10_vec4f_t * dst, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count);
    /** @} */

    /**
     * @ingroup groupMaths
     * @defgroup ADD_MAT Matrix Addition
     *
     * \par
     * These functions implement matrix addition operations for single precision floating point values.
     * @{
     */

    /**
     * Adds the 4x4 matrices of one input array with those of the same index in another,
     * storing the results in an output array. Points to @ref ne10_addmat_4x4f_c, @ref ne10_addmat_4x4f_neon,
     * or @ref ne10_addmat_4x4f_asm. This operation can be performed in-place.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src1  Pointer to the first source array
     * @param[in]  src2  Pointer to the second source array
     * @param[in]  count The number of additions to be performed (i.e. the length of the source arrays)
     */
    extern ne10_result_t (*ne10_addmat_4x4f) (ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src1, ne10_mat4x4f_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_addmat_4x4f using plain C code. */
    extern ne10_result_t ne10_addmat_4x4f_c (ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src1, ne10_mat4x4f_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_addmat_4x4f using NEON intrinsics. */
    extern ne10_result_t ne10_addmat_4x4f_neon (ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src1, ne10_mat4x4f_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_addmat_4x4f using NEON assembly. */
    extern ne10_result_t ne10_addmat_4x4f_asm (ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src1, ne10_mat4x4f_t * src2, ne10_uint32_t count);

    /**
     * Adds the 3x3 matrices of one input array with those of the same index in another,
     * storing the results in an output array. Points to @ref ne10_addmat_3x3f_c, @ref ne10_addmat_3x3f_neon,
     * or @ref ne10_addmat_3x3f_asm. This operation can be performed in-place.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src1  Pointer to the first source array
     * @param[in]  src2  Pointer to the second source array
     * @param[in]  count The number of additions to be performed (i.e. the length of the source arrays)
     */
    extern ne10_result_t (*ne10_addmat_3x3f) (ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src1, ne10_mat3x3f_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_addmat_3x3f using plain C code. */
    extern ne10_result_t ne10_addmat_3x3f_c (ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src1, ne10_mat3x3f_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_addmat_3x3f using NEON intrinsics. */
    extern ne10_result_t ne10_addmat_3x3f_neon (ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src1, ne10_mat3x3f_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_addmat_3x3f using NEON assembly. */
    extern ne10_result_t ne10_addmat_3x3f_asm (ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src1, ne10_mat3x3f_t * src2, ne10_uint32_t count);

    /**
     * Adds the 2x2 matrices of one input array with those of the same index in another,
     * storing the results in an output array. Points to @ref ne10_addmat_2x2f_c, @ref ne10_addmat_2x2f_neon,
     * or @ref ne10_addmat_2x2f_asm. This operation can be performed in-place.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src1  Pointer to the first source array
     * @param[in]  src2  Pointer to the second source array
     * @param[in]  count The number of additions to be performed (i.e. the length of the source arrays)
     */
    extern ne10_result_t (*ne10_addmat_2x2f) (ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src1, ne10_mat2x2f_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_addmat_2x2f using plain C code. */
    extern ne10_result_t ne10_addmat_2x2f_c (ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src1, ne10_mat2x2f_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_addmat_2x2f using NEON intrinsics. */
    extern ne10_result_t ne10_addmat_2x2f_neon (ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src1, ne10_mat2x2f_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_addmat_2x2f using NEON assembly. */
    extern ne10_result_t ne10_addmat_2x2f_asm (ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src1, ne10_mat2x2f_t * src2, ne10_uint32_t count);
    /** @} */


    /**
     * @ingroup groupMaths
     * @defgroup SUB_VEC Vector Subtraction (w[i] = u[i] - v)
     *
     * \par
     * These functions implement vector subtraction operations for single precision floating point values.
     * @{
     */

    /**
     * Subtracts a constant scalar value from all elements of an input array, storing the
     * results in an output array. Points to @ref ne10_subc_float_c, @ref ne10_subc_float_neon, or
     * @ref ne10_subc_float_asm. This operation can be performed in-place.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  cst   The constant to be subtracted
     * @param[in]  count The number of scalar values to be processed
     */
    extern ne10_result_t (*ne10_subc_float) (ne10_float32_t * dst, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_subc_float using plain C code. */
    extern ne10_result_t ne10_subc_float_c (ne10_float32_t * dst, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_subc_float using NEON intrinsics. */
    extern ne10_result_t ne10_subc_float_neon (ne10_float32_t * dst, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_subc_float using NEON assembly. */
    extern ne10_result_t ne10_subc_float_asm (ne10_float32_t * dst, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count);

    /**
     * Subtracts a constant 2D vector from all the vectors in an input array, storing the
     * results in an output array. Points to @ref ne10_subc_vec2f_c, @ref ne10_subc_vec2f_neon, or
     * @ref ne10_subc_vec2f_asm. This operation can be performed in-place.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  cst   Pointer to the constant 2D vector be subtract
     * @param[in]  count The number of 2D vectors to be processed
     */
    extern ne10_result_t (*ne10_subc_vec2f) (ne10_vec2f_t * dst, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_subc_vec2f using plain C code. */
    extern ne10_result_t ne10_subc_vec2f_c (ne10_vec2f_t * dst, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_subc_vec2f using NEON intrinsics. */
    extern ne10_result_t ne10_subc_vec2f_neon (ne10_vec2f_t * dst, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_subc_vec2f using NEON assembly. */
    extern ne10_result_t ne10_subc_vec2f_asm (ne10_vec2f_t * dst, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count);

    /**
     * Subtracts a constant 3D vector from all the vectors in an input array, storing the
     * results in an output array. Points to @ref ne10_subc_vec3f_c, @ref ne10_subc_vec3f_neon, or
     * @ref ne10_subc_vec3f_asm. This operation can be performed in-place.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  cst   Pointer to the constant 3D vector to subtract
     * @param[in]  count The number of 3D vectors to be processed
     */
    extern ne10_result_t (*ne10_subc_vec3f) (ne10_vec3f_t * dst, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_subc_vec3f using plain C code. */
    extern ne10_result_t ne10_subc_vec3f_c (ne10_vec3f_t * dst, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_subc_vec3f using NEON intrinsics. */
    extern ne10_result_t ne10_subc_vec3f_neon (ne10_vec3f_t * dst, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_subc_vec3f using NEON assembly. */
    extern ne10_result_t ne10_subc_vec3f_asm (ne10_vec3f_t * dst, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count);

    /**
     * Subtracts a constant 4D vector from all the vectors in an input array, storing the
     * results in an output array. Points to @ref ne10_subc_vec4f_c, @ref ne10_subc_vec4f_neon, or
     * @ref ne10_subc_vec4f_asm. This operation can be performed in-place.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  cst   Pointer to the constant 4D vector to subtract
     * @param[in]  count The number of 4D vectors to be processed
     */
    extern ne10_result_t (*ne10_subc_vec4f) (ne10_vec4f_t * dst, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_subc_vec4f using plain C code. */
    extern ne10_result_t ne10_subc_vec4f_c (ne10_vec4f_t * dst, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_subc_vec4f using NEON intrinsics. */
    extern ne10_result_t ne10_subc_vec4f_neon (ne10_vec4f_t * dst, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_subc_vec4f using NEON assembly. */
    extern ne10_result_t ne10_subc_vec4f_asm (ne10_vec4f_t * dst, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count);

    /**
     * Subtracts the scalar elements of one input array from those of the same index in another,
     * storing the results in an output array. Points to @ref ne10_sub_float_c, @ref ne10_sub_float_neon,
     * or @ref ne10_sub_float_asm. This operation can be performed in-place.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src1  Pointer to the source array of elements to be subtracted from
     * @param[in]  src2  Pointer to the source array of elements to subtract
     * @param[in]  count The number of subtractions to be performed (i.e. the length of the source arrays)
     */
    extern ne10_result_t (*ne10_sub_float) (ne10_float32_t * dst, ne10_float32_t * src1, ne10_float32_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_sub_float using plain C code. */
    extern ne10_result_t ne10_sub_float_c (ne10_float32_t * dst, ne10_float32_t * src1, ne10_float32_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_sub_float using NEON intrinsics. */
    extern ne10_result_t ne10_sub_float_neon (ne10_float32_t * dst, ne10_float32_t * src1, ne10_float32_t * src2, ne10_uint32_t count) asm ("ne10_sub_float_neon");
    /** Specific implementation of @ref ne10_sub_float using NEON assembly. */
    extern ne10_result_t ne10_sub_float_asm (ne10_float32_t * dst, ne10_float32_t * src1, ne10_float32_t * src2, ne10_uint32_t count);

    /**
     * Subtracts the 2D vectors of one input array from those of the same index in another,
     * storing the results in an output array. Points to @ref ne10_sub_vec2f_c, @ref ne10_sub_vec2f_neon,
     * or @ref ne10_sub_vec2f_asm. This operation can be performed in-place.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src1  Pointer to the source array of elements to be subtracted from
     * @param[in]  src2  Pointer to the source array of elements to subtract
     * @param[in]  count The number of subtractions to be performed (i.e. the length of the source arrays)
     */
    extern ne10_result_t (*ne10_sub_vec2f) (ne10_vec2f_t * dst, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_sub_vec2f using plain C code. */
    extern ne10_result_t ne10_sub_vec2f_c (ne10_vec2f_t * dst, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_sub_vec2f using NEON intrinsics. */
    extern ne10_result_t ne10_sub_vec2f_neon (ne10_vec2f_t * dst, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count) asm ("ne10_sub_vec2f_neon");
    /** Specific implementation of @ref ne10_sub_vec2f using NEON assembly. */
    extern ne10_result_t ne10_sub_vec2f_asm (ne10_vec2f_t * dst, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count);

    /**
     * Subtracts the 3D vectors of one input array from those of the same index in another,
     * storing the results in an output array. Points to @ref ne10_sub_vec3f_c, @ref ne10_sub_vec3f_neon,
     * or @ref ne10_sub_vec3f_asm. This operation can be performed in-place.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src1  Pointer to the source array of elements to be subtracted from
     * @param[in]  src2  Pointer to the source array of elements to subtract
     * @param[in]  count The number of subtractions to be performed (i.e. the length of the source arrays)
     */
    extern ne10_result_t (*ne10_sub_vec3f) (ne10_vec3f_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_sub_vec3f using plain C code. */
    extern ne10_result_t ne10_sub_vec3f_c (ne10_vec3f_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_sub_vec3f using NEON intrinsics. */
    extern ne10_result_t ne10_sub_vec3f_neon (ne10_vec3f_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count) asm ("ne10_sub_vec3f_neon");
    /** Specific implementation of @ref ne10_sub_vec3f using NEON assembly. */
    extern ne10_result_t ne10_sub_vec3f_asm (ne10_vec3f_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);

    /**
     * Subtracts the 4D vectors of one input array from those of the same index in another,
     * storing the results in an output array. Points to @ref ne10_sub_vec4f_c, @ref ne10_sub_vec4f_neon,
     * or @ref ne10_sub_vec4f_asm. This operation can be performed in-place.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src1  Pointer to the source array of elements to be subtracted from
     * @param[in]  src2  Pointer to the source array of elements to subtract
     * @param[in]  count The number of subtractions to be performed (i.e. the length of the source arrays)
     */
    extern ne10_result_t (*ne10_sub_vec4f) (ne10_vec4f_t * dst, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_sub_vec4f using plain C code. */
    extern ne10_result_t ne10_sub_vec4f_c (ne10_vec4f_t * dst, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_sub_vec4f using NEON intrinsics. */
    extern ne10_result_t ne10_sub_vec4f_neon (ne10_vec4f_t * dst, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count) asm ("ne10_sub_vec4f_neon");
    /** Specific implementation of @ref ne10_sub_vec4f using NEON assembly. */
    extern ne10_result_t ne10_sub_vec4f_asm (ne10_vec4f_t * dst, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count);
    /** @} */

    /**
     * @ingroup groupMaths
     * @defgroup RSBC Vector Subtraction From (w[i] = v - u[i])
     *
     * \par
     * These functions implement vector "subtract from" operations for single precision floating point values.
     * @{
     */

    /**
     * Subtracts all scalar elements of an input array from a constant value, storing the
     * results in an output array. Points to @ref ne10_rsbc_float_c, @ref ne10_rsbc_float_neon, or
     * @ref ne10_rsbc_float_asm. This operation can be performed in-place.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array of elements to subtract
     * @param[in]  cst   The constant to be subtracted from
     * @param[in]  count The number of scalar values to be processed
     */
    extern ne10_result_t (*ne10_rsbc_float) (ne10_float32_t * dst, ne10_float32_t *src, const ne10_float32_t cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_rsbc_float using plain C code. */
    extern ne10_result_t ne10_rsbc_float_c (ne10_float32_t * dst, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_rsbc_float using NEON intrinsics. */
    extern ne10_result_t ne10_rsbc_float_neon (ne10_float32_t * dst, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_rsbc_float using NEON assembly. */
    extern ne10_result_t ne10_rsbc_float_asm (ne10_float32_t * dst, ne10_float32_t *src, const ne10_float32_t cst, ne10_uint32_t count);

    /**
     * Subtracts all the 2D vectors in an input array from a constant vector, storing the
     * results in an output array. Points to @ref ne10_rsbc_vec2f_c, @ref ne10_rsbc_vec2f_neon, or
     * @ref ne10_rsbc_vec2f_asm. This operation can be performed in-place.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array of elements to subtract
     * @param[in]  cst   Pointer to the constant 2D vector to be subtracted from
     * @param[in]  count The number of 2D vectors to be processed
     */
    extern ne10_result_t (*ne10_rsbc_vec2f) (ne10_vec2f_t * dst, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_rsbc_vec2f using plain C code. */
    extern ne10_result_t ne10_rsbc_vec2f_c (ne10_vec2f_t * dst, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_rsbc_vec2f using NEON intrinsics. */
    extern ne10_result_t ne10_rsbc_vec2f_neon (ne10_vec2f_t * dst, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_rsbc_vec2f using NEON assembly. */
    extern ne10_result_t ne10_rsbc_vec2f_asm (ne10_vec2f_t * dst, ne10_vec2f_t *src, const ne10_vec2f_t * cst, ne10_uint32_t count);

    /**
     * Subtracts all the 3D vectors in an input array from a constant vector, storing the
     * results in an output array. Points to @ref ne10_rsbc_vec3f_c, @ref ne10_rsbc_vec3f_neon, or
     * @ref ne10_rsbc_vec3f_asm. This operation can be performed in-place.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array of elements to subtract
     * @param[in]  cst   Pointer to the constant 3D vector to be subtracted from
     * @param[in]  count The number of 3D vectors to be processed
     */
    extern ne10_result_t (*ne10_rsbc_vec3f) (ne10_vec3f_t * dst, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_rsbc_vec3f using plain C code. */
    extern ne10_result_t ne10_rsbc_vec3f_c (ne10_vec3f_t * dst, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_rsbc_vec3f using NEON intrinsics. */
    extern ne10_result_t ne10_rsbc_vec3f_neon (ne10_vec3f_t * dst, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_rsbc_vec3f using NEON assembly. */
    extern ne10_result_t ne10_rsbc_vec3f_asm (ne10_vec3f_t * dst, ne10_vec3f_t *src, const ne10_vec3f_t * cst, ne10_uint32_t count);

    /**
     * Subtracts all the 4D vectors in an input array from a constant vector, storing the
     * results in an output array. Points to @ref ne10_rsbc_vec4f_c, @ref ne10_rsbc_vec4f_neon, or
     * @ref ne10_rsbc_vec4f_asm. This operation can be performed in-place.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array of elements to subtract
     * @param[in]  cst   Pointer to the constant 4D vector to be subtracted from
     * @param[in]  count The number of 4D vectors to be processed
     */
    extern ne10_result_t (*ne10_rsbc_vec4f) (ne10_vec4f_t * dst, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_rsbc_vec4f using plain C code. */
    extern ne10_result_t ne10_rsbc_vec4f_c (ne10_vec4f_t * dst, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_rsbc_vec4f using NEON intrinsics. */
    extern ne10_result_t ne10_rsbc_vec4f_neon (ne10_vec4f_t * dst, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_rsbc_vec4f using NEON assembly. */
    extern ne10_result_t ne10_rsbc_vec4f_asm (ne10_vec4f_t * dst, ne10_vec4f_t *src, const ne10_vec4f_t * cst, ne10_uint32_t count);
    /** @} */

    /**
     * @ingroup groupMaths
     * @defgroup SUB_MAT Matrix Subtraction
     *
     * \par
     * These functions implement matrix subtraction operations for single precision floating point values.
     * @{
     */

    /**
     * Subtracts the 4x4 matrices of one input array from those of the same index in another,
     * storing the results in an output array. Points to @ref ne10_submat_4x4f_c, @ref ne10_submat_4x4f_neon,
     * or @ref ne10_submat_4x4f_asm. This operation can be performed in-place.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src1  Pointer to the source array of elements to be subtracted from
     * @param[in]  src2  Pointer to the source array of elements to subtract
     * @param[in]  count The number of subtractions to be performed (i.e. the length of the source arrays)
     */
    extern ne10_result_t (*ne10_submat_4x4f) (ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src1, ne10_mat4x4f_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_submat_4x4f using plain C code. */
    extern ne10_result_t ne10_submat_4x4f_c (ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src1, ne10_mat4x4f_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_submat_4x4f using NEON intrinsics. */
    extern ne10_result_t ne10_submat_4x4f_neon (ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src1, ne10_mat4x4f_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_submat_4x4f using NEON assembly. */
    extern ne10_result_t ne10_submat_4x4f_asm (ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src1, ne10_mat4x4f_t * src2, ne10_uint32_t count);

    /**
     * Subtracts the 3x3 matrices of one input array from those of the same index in another,
     * storing the results in an output array. Points to @ref ne10_submat_3x3f_c, @ref ne10_submat_3x3f_neon,
     * or @ref ne10_submat_3x3f_asm. This operation can be performed in-place.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src1  Pointer to the source array of elements to be subtracted from
     * @param[in]  src2  Pointer to the source array of elements to subtract
     * @param[in]  count The number of subtractions to be performed (i.e. the length of the source arrays)
     */
    extern ne10_result_t (*ne10_submat_3x3f) (ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src1, ne10_mat3x3f_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_submat_3x3f using plain C code. */
    extern ne10_result_t ne10_submat_3x3f_c (ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src1, ne10_mat3x3f_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_submat_3x3f using NEON intrinsics. */
    extern ne10_result_t ne10_submat_3x3f_neon (ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src1, ne10_mat3x3f_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_submat_3x3f using NEON assembly. */
    extern ne10_result_t ne10_submat_3x3f_asm (ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src1, ne10_mat3x3f_t * src2, ne10_uint32_t count);

    /**
     * Subtracts the 2x2 matrices of one input array from those of the same index in another,
     * storing the results in an output array. Points to @ref ne10_submat_2x2f_c, @ref ne10_submat_2x2f_neon,
     * or @ref ne10_submat_2x2f_asm. This operation can be performed in-place.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src1  Pointer to the source array of elements to be subtracted from
     * @param[in]  src2  Pointer to the source array of elements to subtract
     * @param[in]  count The number of subtractions to be performed (i.e. the length of the source arrays)
     */
    extern ne10_result_t (*ne10_submat_2x2f) (ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src1, ne10_mat2x2f_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_submat_2x2f using plain C code. */
    extern ne10_result_t ne10_submat_2x2f_c (ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src1, ne10_mat2x2f_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_submat_2x2f using NEON intrinsics. */
    extern ne10_result_t ne10_submat_2x2f_neon (ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src1, ne10_mat2x2f_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_submat_2x2f using NEON assembly. */
    extern ne10_result_t ne10_submat_2x2f_asm (ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src1, ne10_mat2x2f_t * src2, ne10_uint32_t count);
    /** @} */

    /**
     * @ingroup groupMaths
     * @defgroup MUL_VEC Vector Multiplication
     *
     * \par
     * These functions implement vector element-wise multiplication operations for single precision floating point values.
     * @{
     */

    /**
     * Multiplies all scalar elements of an input array by a constant value, storing the
     * results in an output array. Points to @ref ne10_mulc_float_c, @ref ne10_mulc_float_neon, or
     * @ref ne10_mulc_float_asm. This operation can be performed in-place.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  cst   The constant to multiply by
     * @param[in]  count The number of scalar values to be processed
     */
    extern ne10_result_t (*ne10_mulc_float) (ne10_float32_t * dst, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_mulc_float using plain C code. */
    extern ne10_result_t ne10_mulc_float_c (ne10_float32_t * dst, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_mulc_float using NEON intrinsics. */
    extern ne10_result_t ne10_mulc_float_neon (ne10_float32_t * dst, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_mulc_float using NEON assembly. */
    extern ne10_result_t ne10_mulc_float_asm (ne10_float32_t * dst, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count);

    /**
     * Element-wise multiplies all the 2D vectors in an input array by a constant vector,
     * storing the results in an output array. Points to @ref ne10_mulc_vec2f_c, @ref ne10_mulc_vec2f_neon,
     * or @ref ne10_mulc_vec2f_asm. This operation can be performed in-place.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  cst   Pointer to the constant 2D vector to multiply by
     * @param[in]  count The number of 2D vectors to be processed
     */
    extern ne10_result_t (*ne10_mulc_vec2f) (ne10_vec2f_t * dst, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_mulc_vec2f using plain C code. */
    extern ne10_result_t ne10_mulc_vec2f_c (ne10_vec2f_t * dst, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_mulc_vec2f using NEON intrinsics. */
    extern ne10_result_t ne10_mulc_vec2f_neon (ne10_vec2f_t * dst, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_mulc_vec2f using NEON assembly. */
    extern ne10_result_t ne10_mulc_vec2f_asm (ne10_vec2f_t * dst, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count);

    /**
     * Element-wise multiplies all the 3D vectors in an input array by a constant vector,
     * storing the results in an output array. Points to @ref ne10_mulc_vec3f_c, @ref ne10_mulc_vec3f_neon,
     * or @ref ne10_mulc_vec3f_asm. This operation can be performed in-place.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  cst   Pointer to the constant 3D vector to multiply by
     * @param[in]  count The number of 3D vectors to be processed
     */
    extern ne10_result_t (*ne10_mulc_vec3f) (ne10_vec3f_t * dst, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_mulc_vec3f using plain C code. */
    extern ne10_result_t ne10_mulc_vec3f_c (ne10_vec3f_t * dst, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_mulc_vec3f using NEON intrinsics. */
    extern ne10_result_t ne10_mulc_vec3f_neon (ne10_vec3f_t * dst, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_mulc_vec3f using NEON assembly. */
    extern ne10_result_t ne10_mulc_vec3f_asm (ne10_vec3f_t * dst, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count);

    /**
     * Element-wise multiplies all the 4D vectors in an input array by a constant vector,
     * storing the results in an output array. Points to @ref ne10_mulc_vec4f_c, @ref ne10_mulc_vec4f_neon,
     * or @ref ne10_mulc_vec4f_asm. This operation can be performed in-place.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  cst   Pointer to the constant 4D vector to multiply by
     * @param[in]  count The number of 4D vectors to be processed
     */
    extern ne10_result_t (*ne10_mulc_vec4f) (ne10_vec4f_t * dst, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_mulc_vec4f using plain C code. */
    extern ne10_result_t ne10_mulc_vec4f_c (ne10_vec4f_t * dst, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_mulc_vec4f using NEON intrinsics. */
    extern ne10_result_t ne10_mulc_vec4f_neon (ne10_vec4f_t * dst, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_mulc_vec4f using NEON assembly. */
    extern ne10_result_t ne10_mulc_vec4f_asm (ne10_vec4f_t * dst, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count);

    /**
     * Multiplies the scalar elements of one input array by those of the same index in another,
     * storing the results in an output array. Points to @ref ne10_mul_float_c, @ref ne10_mul_float_neon,
     * or @ref ne10_mul_float_asm. This operation can be performed in-place.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src1  Pointer to the first source array
     * @param[in]  src2  Pointer to the second source array
     * @param[in]  count The number of multiplications to be performed (i.e. the length of the source arrays)
     */
    extern ne10_result_t (*ne10_mul_float) (ne10_float32_t * dst, ne10_float32_t * src1, ne10_float32_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_mul_float using plain C code. */
    extern ne10_result_t ne10_mul_float_c (ne10_float32_t * dst, ne10_float32_t * src1, ne10_float32_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_mul_float using NEON intrinsics. */
    extern ne10_result_t ne10_mul_float_neon (ne10_float32_t * dst, ne10_float32_t * src1, ne10_float32_t * src2, ne10_uint32_t count) asm ("ne10_mul_float_neon");
    /** Specific implementation of @ref ne10_mul_float using NEON assembly. */
    extern ne10_result_t ne10_mul_float_asm (ne10_float32_t * dst, ne10_float32_t * src1, ne10_float32_t * src2, ne10_uint32_t count);

    /**
     * Element-wise multiplies the 2D vectors of one input array by those of the same index
     * in another, storing the results in an output array. Points to @ref ne10_vmul_vec2f_c,
     * @ref ne10_vmul_vec2f_neon, or @ref ne10_vmul_vec2f_asm. This operation can be performed in-place.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src1  Pointer to the first source array
     * @param[in]  src2  Pointer to the second source array
     * @param[in]  count The number of multiplications to be performed (i.e. the length of the source arrays)
     */
    extern ne10_result_t (*ne10_vmul_vec2f) (ne10_vec2f_t * dst, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_vmul_vec2f using plain C code. */
    extern ne10_result_t ne10_vmul_vec2f_c (ne10_vec2f_t * dst, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_vmul_vec2f using NEON intrinsics. */
    extern ne10_result_t ne10_vmul_vec2f_neon (ne10_vec2f_t * dst, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count) asm ("ne10_vmul_vec2f_neon");
    /** Specific implementation of @ref ne10_vmul_vec2f using NEON assembly. */
    extern ne10_result_t ne10_vmul_vec2f_asm (ne10_vec2f_t * dst, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count);

    /**
     * Element-wise multiplies the 3D vectors of one input array by those of the same index
     * in another, storing the results in an output array. Points to @ref ne10_vmul_vec3f_c,
     * @ref ne10_vmul_vec3f_neon, or @ref ne10_vmul_vec3f_asm. This operation can be performed in-place.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src1  Pointer to the first source array
     * @param[in]  src2  Pointer to the second source array
     * @param[in]  count The number of multiplications to be performed (i.e. the length of the source arrays)
     */
    extern ne10_result_t (*ne10_vmul_vec3f) (ne10_vec3f_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_vmul_vec3f using plain C code. */
    extern ne10_result_t ne10_vmul_vec3f_c (ne10_vec3f_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_vmul_vec3f using NEON intrinsics. */
    extern ne10_result_t ne10_vmul_vec3f_neon (ne10_vec3f_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count) asm ("ne10_vmul_vec3f_neon");
    /** Specific implementation of @ref ne10_vmul_vec3f using NEON assembly. */
    extern ne10_result_t ne10_vmul_vec3f_asm (ne10_vec3f_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);

    /**
     * Element-wise multiplies the 4D vectors of one input array by those of the same index
     * in another, storing the results in an output array. Points to @ref ne10_vmul_vec4f_c,
     * @ref ne10_vmul_vec4f_neon, or @ref ne10_vmul_vec4f_asm. This operation can be performed in-place.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src1  Pointer to the first source array
     * @param[in]  src2  Pointer to the second source array
     * @param[in]  count The number of multiplications to be performed (i.e. the length of the source arrays)
     */
    extern ne10_result_t (*ne10_vmul_vec4f) (ne10_vec4f_t * dst, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_vmul_vec4f using plain C code. */
    extern ne10_result_t ne10_vmul_vec4f_c (ne10_vec4f_t * dst, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_vmul_vec4f using NEON intrinsics. */
    extern ne10_result_t ne10_vmul_vec4f_neon (ne10_vec4f_t * dst, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count) asm ("ne10_vmul_vec4f_neon");
    /** Specific implementation of @ref ne10_vmul_vec4f using NEON assembly. */
    extern ne10_result_t ne10_vmul_vec4f_asm (ne10_vec4f_t * dst, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count);
    /** @} */

    /**
     * @ingroup groupMaths
     * @defgroup MLA_VEC Vector Multiply-Accumulate
     *
     * \par
     * These functions implement vector multiply-accumulate operations for single precision floating point values.
     * @{
     */

    /**
     * Multiplies all scalar elements of an input array by constant value, adding this product
     * to a value of the same index in another input array, and storing the results in an
     * output array. Points to @ref ne10_mlac_float_c, @ref ne10_mlac_float_neon, or @ref ne10_mlac_float_asm.
     *
     * This operation can be performed in-place.
     * @param[out] dst   Pointer to the destination array
     * @param[in]  acc   Pointer to the array of elements to be added post-multiplication
     * @param[in]  src   Pointer to the source array
     * @param[in]  cst   The constant to multiply by
     * @param[in]  count The number of multiply-accumulates to be performed (i.e. the length of the source array)
     */
    extern ne10_result_t (*ne10_mlac_float) (ne10_float32_t * dst, ne10_float32_t * acc, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_mlac_float using plain C code. */
    extern ne10_result_t ne10_mlac_float_c (ne10_float32_t * dst, ne10_float32_t * acc, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_mlac_float using NEON intrinsics. */
    extern ne10_result_t ne10_mlac_float_neon (ne10_float32_t * dst, ne10_float32_t * acc, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_mlac_float using NEON assembly. */
    extern ne10_result_t ne10_mlac_float_asm (ne10_float32_t * dst, ne10_float32_t * acc, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count);

    /**
     * Element-wise multiplies all the 2D vectors of an input array by a constant vector,
     * adding this vector to another of the same index in another input array, and storing
     * the results in an output array. Points to @ref ne10_mlac_vec2f_c, @ref ne10_mlac_vec2f_neon,
     * or @ref ne10_mlac_vec2f_asm. This operation can be performed in-place.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  acc   Pointer to the array of elements to be added post-multiplication
     * @param[in]  src   Pointer to the source array
     * @param[in]  cst   The constant to multiply by
     * @param[in]  count The number of multiply-accumulates to be performed (i.e. the length of the source array)
     */
    extern ne10_result_t (*ne10_mlac_vec2f) (ne10_vec2f_t * dst, ne10_vec2f_t * acc, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_mlac_vec2f using plain C code. */
    extern ne10_result_t ne10_mlac_vec2f_c (ne10_vec2f_t * dst, ne10_vec2f_t * acc, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_mlac_vec2f using NEON intrinsics. */
    extern ne10_result_t ne10_mlac_vec2f_neon (ne10_vec2f_t * dst, ne10_vec2f_t * acc, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_mlac_vec2f using NEON assembly. */
    extern ne10_result_t ne10_mlac_vec2f_asm (ne10_vec2f_t * dst, ne10_vec2f_t * acc, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count);

    /**
     * Element-wise multiplies all the 3D vectors of an input array by a constant vector,
     * adding this vector to another of the same index in another input array, and storing
     * the results in an output array. Points to @ref ne10_mlac_vec3f_c, @ref ne10_mlac_vec3f_neon,
     * or @ref ne10_mlac_vec3f_asm. This operation can be performed in-place.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  acc   Pointer to the array of elements to be added post-multiplication
     * @param[in]  src   Pointer to the source array
     * @param[in]  cst   The constant to multiply by
     * @param[in]  count The number of multiply-accumulates to be performed (i.e. the length of the source array)
     */
    extern ne10_result_t (*ne10_mlac_vec3f) (ne10_vec3f_t * dst, ne10_vec3f_t * acc, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_mlac_vec3f using plain C code. */
    extern ne10_result_t ne10_mlac_vec3f_c (ne10_vec3f_t * dst, ne10_vec3f_t * acc, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_mlac_vec3f using NEON intrinsics. */
    extern ne10_result_t ne10_mlac_vec3f_neon (ne10_vec3f_t * dst, ne10_vec3f_t * acc, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_mlac_vec3f using NEON assembly. */
    extern ne10_result_t ne10_mlac_vec3f_asm (ne10_vec3f_t * dst, ne10_vec3f_t * acc, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count);

    /**
     * Element-wise multiplies all the 4D vectors of an input array by a constant vector,
     * adding this vector to another of the same index in another input array, and storing
     * the results in an output array. Points to @ref ne10_mlac_vec4f_c, @ref ne10_mlac_vec4f_neon,
     * or @ref ne10_mlac_vec4f_asm. This operation can be performed in-place.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  acc   Pointer to the array of elements to be added post-multiplication
     * @param[in]  src   Pointer to the source array
     * @param[in]  cst   The constant to multiply by
     * @param[in]  count The number of multiply-accumulates to be performed (i.e. the length of the source array)
     */
    extern ne10_result_t (*ne10_mlac_vec4f) (ne10_vec4f_t * dst, ne10_vec4f_t * acc, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_mlac_vec4f using plain C code. */
    extern ne10_result_t ne10_mlac_vec4f_c (ne10_vec4f_t * dst, ne10_vec4f_t * acc, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_mlac_vec4f using NEON intrinsics. */
    extern ne10_result_t ne10_mlac_vec4f_neon (ne10_vec4f_t * dst, ne10_vec4f_t * acc, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_mlac_vec4f using NEON assembly. */
    extern ne10_result_t ne10_mlac_vec4f_asm (ne10_vec4f_t * dst, ne10_vec4f_t * acc, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count);

    /**
     * Multiplies all scalar elements of an input array by those of the same index in another,
     * adding this product to a value of the same index in yet another input array, and
     * storing the results in an output array. Points to @ref ne10_mlac_float_c, @ref ne10_mlac_float_neon,
     * or @ref ne10_mlac_float_asm. This operation can be performed in-place.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  acc   Pointer to the array of elements to be added post-multiplication
     * @param[in]  src1  Pointer to the source array
     * @param[in]  src2  Pointer to the array of elements to multiply by
     * @param[in]  count The number of multiply-accumulates to be performed (i.e. the length of the source arrays)
     */
    extern ne10_result_t (*ne10_mla_float) (ne10_float32_t * dst, ne10_float32_t * acc, ne10_float32_t * src1, ne10_float32_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_mla_float using plain C code. */
    extern ne10_result_t ne10_mla_float_c (ne10_float32_t * dst, ne10_float32_t * acc, ne10_float32_t * src1, ne10_float32_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_mla_float using NEON intrinsics. */
    extern ne10_result_t ne10_mla_float_neon (ne10_float32_t * dst, ne10_float32_t * acc, ne10_float32_t * src1, ne10_float32_t * src2, ne10_uint32_t count) asm ("ne10_mla_float_neon");
    /** Specific implementation of @ref ne10_mla_float using NEON assembly. */
    extern ne10_result_t ne10_mla_float_asm (ne10_float32_t * dst, ne10_float32_t * acc, ne10_float32_t * src1, ne10_float32_t * src2, ne10_uint32_t count);

    /**
     * Element-wise multiplies all the 2D vector elements of an input array by those of the
     * same index in another, adding this vector to another of the same index in yet another
     * input array, and storing the results in an output array. Points to @ref ne10_vmla_vec2f_c,
     * @ref ne10_vmla_vec2f_neon, or @ref ne10_vmla_vec2f_asm. This operation can be performed in-place.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  acc   Pointer to the array of elements to be added post-multiplication
     * @param[in]  src1  Pointer to the source array
     * @param[in]  src2  Pointer to the array of elements to multiply by
     * @param[in]  count The number of multiply-accumulates to be performed (i.e. the length of the source arrays)
     */
    extern ne10_result_t (*ne10_vmla_vec2f) (ne10_vec2f_t * dst, ne10_vec2f_t * acc, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_vmla_vec2f using plain C code. */
    extern ne10_result_t ne10_vmla_vec2f_c (ne10_vec2f_t * dst, ne10_vec2f_t * acc, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_vmla_vec2f using NEON intrinsics. */
    extern ne10_result_t ne10_vmla_vec2f_neon (ne10_vec2f_t * dst, ne10_vec2f_t * acc, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count) asm ("ne10_vmla_vec2f_neon");
    /** Specific implementation of @ref ne10_vmla_vec2f using NEON assembly. */
    extern ne10_result_t ne10_vmla_vec2f_asm (ne10_vec2f_t * dst, ne10_vec2f_t * acc, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count);

    /**
     * Element-wise multiplies all the 3D vector elements of an input array by those of the
     * same index in another, adding this vector to another of the same index in yet another
     * input array, and storing the results in an output array. Points to @ref ne10_vmla_vec3f_c,
     * @ref ne10_vmla_vec3f_neon, or @ref ne10_vmla_vec3f_asm. This operation can be performed in-place.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  acc   Pointer to the array of elements to be added post-multiplication
     * @param[in]  src1  Pointer to the source array
     * @param[in]  src2  Pointer to the array of elements to multiply by
     * @param[in]  count The number of multiply-accumulates to be performed (i.e. the length of the source arrays)
     */
    extern ne10_result_t (*ne10_vmla_vec3f) (ne10_vec3f_t * dst, ne10_vec3f_t * acc, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_vmla_vec3f using plain C code. */
    extern ne10_result_t ne10_vmla_vec3f_c (ne10_vec3f_t * dst, ne10_vec3f_t * acc, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_vmla_vec3f using NEON intrinsics. */
    extern ne10_result_t ne10_vmla_vec3f_neon (ne10_vec3f_t * dst, ne10_vec3f_t * acc, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count) asm ("ne10_vmla_vec3f_neon");
    /** Specific implementation of @ref ne10_vmla_vec3f using NEON assembly. */
    extern ne10_result_t ne10_vmla_vec3f_asm (ne10_vec3f_t * dst, ne10_vec3f_t * acc, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);

    /**
     * Element-wise multiplies all the 4D vector elements of an input array by those of the
     * same index in another, adding this vector to another of the same index in yet another
     * input array, and storing the results in an output array. Points to @ref ne10_vmla_vec4f_c,
     * @ref ne10_vmla_vec4f_neon, or @ref ne10_vmla_vec4f_asm. This operation can be performed in-place.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  acc   Pointer to the array of elements to be added post-multiplication
     * @param[in]  src1  Pointer to the source array
     * @param[in]  src2  Pointer to the array of elements to multiply by
     * @param[in]  count The number of multiply-accumulates to be performed (i.e. the length of the source arrays)
     */
    extern ne10_result_t (*ne10_vmla_vec4f) (ne10_vec4f_t * dst, ne10_vec4f_t * acc, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_vmla_vec4f using plain C code. */
    extern ne10_result_t ne10_vmla_vec4f_c (ne10_vec4f_t * dst, ne10_vec4f_t * acc, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_vmla_vec4f using NEON intrinsics. */
    extern ne10_result_t ne10_vmla_vec4f_neon (ne10_vec4f_t * dst, ne10_vec4f_t * acc, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count) asm ("ne10_vmla_vec4f_neon");
    /** Specific implementation of @ref ne10_vmla_vec4f using NEON assembly. */
    extern ne10_result_t ne10_vmla_vec4f_asm (ne10_vec4f_t * dst, ne10_vec4f_t * acc, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count);
    /** @} */

    /**
     * @ingroup groupMaths
     * @defgroup MUL_MAT Matrix Multiplication
     *
     * \par
     * These functions implement matrix multiplication operations for single precision floating point values.
     * @{
     */

    /**
     * Multiplies the 4x4 matrices of one input array by those of the same index in another,
     * storing the results in an output array. Points to @ref ne10_mulmat_4x4f_c, @ref ne10_mulmat_4x4f_neon,
     * or @ref ne10_mulmat_4x4f_asm. This operation cannot be performed in-place.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src1  Pointer to the first source array
     * @param[in]  src2  Pointer to the second source array
     * @param[in]  count The number of multiplications to be performed (i.e. the length of the source arrays)
     */
    extern ne10_result_t (*ne10_mulmat_4x4f) (ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src1, ne10_mat4x4f_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_mulmat_4x4f using plain C code. */
    extern ne10_result_t ne10_mulmat_4x4f_c (ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src1, ne10_mat4x4f_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_mulmat_4x4f using NEON intrinsics. */
    extern ne10_result_t ne10_mulmat_4x4f_neon (ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src1, ne10_mat4x4f_t * src2, ne10_uint32_t count) asm ("ne10_mulmat_4x4f_neon");
    /** Specific implementation of @ref ne10_mulmat_4x4f using NEON assembly. */
    extern ne10_result_t ne10_mulmat_4x4f_asm (ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src1, ne10_mat4x4f_t * src2, ne10_uint32_t count);

    /**
     * Multiplies the 3x3 matrices of one input array by those of the same index in another,
     * storing the results in an output array. Points to @ref ne10_mulmat_3x3f_c, @ref ne10_mulmat_3x3f_neon,
     * or @ref ne10_mulmat_3x3f_asm. This operation cannot be performed in-place.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src1  Pointer to the first source array
     * @param[in]  src2  Pointer to the second source array
     * @param[in]  count The number of multiplications to be performed (i.e. the length of the source arrays)
     */
    extern ne10_result_t (*ne10_mulmat_3x3f) (ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src1, ne10_mat3x3f_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_mulmat_3x3f using plain C code. */
    extern ne10_result_t ne10_mulmat_3x3f_c (ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src1, ne10_mat3x3f_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_mulmat_3x3f using NEON intrinsics. */
    extern ne10_result_t ne10_mulmat_3x3f_neon (ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src1, ne10_mat3x3f_t * src2, ne10_uint32_t count) asm ("ne10_mulmat_3x3f_neon");
    /** Specific implementation of @ref ne10_mulmat_3x3f using NEON assembly. */
    extern ne10_result_t ne10_mulmat_3x3f_asm (ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src1, ne10_mat3x3f_t * src2, ne10_uint32_t count);

    /**
     * Multiplies the 2x2 matrices of one input array by those of the same index in another,
     * storing the results in an output array. Points to @ref ne10_mulmat_2x2f_c, @ref ne10_mulmat_2x2f_neon,
     * or @ref ne10_mulmat_2x2f_asm. This operation cannot be performed in-place.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src1  Pointer to the first source array
     * @param[in]  src2  Pointer to the second source array
     * @param[in]  count The number of multiplications to be performed (i.e. the length of the source arrays)
     */
    extern ne10_result_t (*ne10_mulmat_2x2f) (ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src1, ne10_mat2x2f_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_mulmat_2x2f using plain C code. */
    extern ne10_result_t ne10_mulmat_2x2f_c (ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src1, ne10_mat2x2f_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_mulmat_2x2f using NEON intrinsics. */
    extern ne10_result_t ne10_mulmat_2x2f_neon (ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src1, ne10_mat2x2f_t * src2, ne10_uint32_t count) asm ("ne10_mulmat_2x2f_neon");
    /** Specific implementation of @ref ne10_mulmat_2x2f using NEON assembly. */
    extern ne10_result_t ne10_mulmat_2x2f_asm (ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src1, ne10_mat2x2f_t * src2, ne10_uint32_t count);
    /** @} */

    /**
     * @ingroup groupMaths
     * @defgroup MUL_MAT_VEC Matrix-Vector Multiplication
     *
     * \par
     * These functions implement matrix-vector multiplication operations for single precision floating point values.
     * @{
     */

    /**
     * Multiplies the 4x4 matrices of one input array by the 4D vectors of the same index in
     * another, storing the results in an output array. Points to @ref ne10_mulcmatvec_cm4x4f_v4f_c,
     * @ref ne10_mulcmatvec_cm4x4f_v4f_neon, or @ref ne10_mulcmatvec_cm4x4f_v4f_asm. This operation
     * cannot be performed in-place.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  cst   Pointer to the source array of matrices
     * @param[in]  src   Pointer to the source array of vectors
     * @param[in]  count The number of multiplications to be performed (i.e. the length of the source arrays)
     */
    extern ne10_result_t (*ne10_mulcmatvec_cm4x4f_v4f) (ne10_vec4f_t * dst, const ne10_mat4x4f_t * cst, ne10_vec4f_t * src, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_mulcmatvec_cm4x4f_v4f using plain C code. */
    extern ne10_result_t ne10_mulcmatvec_cm4x4f_v4f_c (ne10_vec4f_t * dst, const ne10_mat4x4f_t * cst, ne10_vec4f_t * src, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_mulcmatvec_cm4x4f_v4f using NEON intrinsics. */
    extern ne10_result_t ne10_mulcmatvec_cm4x4f_v4f_neon (ne10_vec4f_t * dst, const ne10_mat4x4f_t * cst, ne10_vec4f_t * src, ne10_uint32_t count) asm ("ne10_mulcmatvec_cm4x4f_v4f_neon");
    /** Specific implementation of @ref ne10_mulcmatvec_cm4x4f_v4f using NEON assembly. */
    extern ne10_result_t ne10_mulcmatvec_cm4x4f_v4f_asm (ne10_vec4f_t * dst, const ne10_mat4x4f_t * cst, ne10_vec4f_t * src, ne10_uint32_t count);

    /**
     * Multiplies the 3x3 matrices of one input array by the 3D vectors of the same index in
     * another, storing the results in an output array. Points to @ref ne10_mulcmatvec_cm3x3f_v3f_c,
     * @ref ne10_mulcmatvec_cm3x3f_v3f_neon, or @ref ne10_mulcmatvec_cm3x3f_v3f_asm. This operation
     * cannot be performed in-place.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  cst   Pointer to the source array of matrices
     * @param[in]  src   Pointer to the source array of vectors
     * @param[in]  count The number of multiplications to be performed (i.e. the length of the source arrays)
     */
    extern ne10_result_t (*ne10_mulcmatvec_cm3x3f_v3f) (ne10_vec3f_t * dst, const ne10_mat3x3f_t * cst, ne10_vec3f_t * src, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_mulcmatvec_cm3x3f_v3f using plain C code. */
    extern ne10_result_t ne10_mulcmatvec_cm3x3f_v3f_c (ne10_vec3f_t * dst, const ne10_mat3x3f_t * cst, ne10_vec3f_t * src, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_mulcmatvec_cm3x3f_v3f using NEON intrinsics. */
    extern ne10_result_t ne10_mulcmatvec_cm3x3f_v3f_neon (ne10_vec3f_t * dst, const ne10_mat3x3f_t * cst, ne10_vec3f_t * src, ne10_uint32_t count) asm ("ne10_mulcmatvec_cm3x3f_v3f_neon");
    /** Specific implementation of @ref ne10_mulcmatvec_cm3x3f_v3f using NEON assembly. */
    extern ne10_result_t ne10_mulcmatvec_cm3x3f_v3f_asm (ne10_vec3f_t * dst, const ne10_mat3x3f_t * cst, ne10_vec3f_t * src, ne10_uint32_t count);

    /**
     * Multiplies the 2x2 matrices of one input array by the 2D vectors of the same index in
     * another, storing the results in an output array. Points to @ref ne10_mulcmatvec_cm2x2f_v2f_c,
     * @ref ne10_mulcmatvec_cm2x2f_v2f_neon, or @ref ne10_mulcmatvec_cm2x2f_v2f_asm. This operation
     * cannot be performed in-place.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  cst   Pointer to the source array of matrices
     * @param[in]  src   Pointer to the source array of vectors
     * @param[in]  count The number of multiplications to be performed (i.e. the length of the source arrays)
     */
    extern ne10_result_t (*ne10_mulcmatvec_cm2x2f_v2f) (ne10_vec2f_t * dst, const ne10_mat2x2f_t * cst, ne10_vec2f_t * src, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_mulcmatvec_cm2x2f_v2f using plain C code. */
    extern ne10_result_t ne10_mulcmatvec_cm2x2f_v2f_c (ne10_vec2f_t * dst, const ne10_mat2x2f_t * cst, ne10_vec2f_t * src, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_mulcmatvec_cm2x2f_v2f using NEON intrinsics. */
    extern ne10_result_t ne10_mulcmatvec_cm2x2f_v2f_neon (ne10_vec2f_t * dst, const ne10_mat2x2f_t * cst, ne10_vec2f_t * src, ne10_uint32_t count) asm ("ne10_mulcmatvec_cm2x2f_v2f_neon");
    /** Specific implementation of @ref ne10_mulcmatvec_cm2x2f_v2f using NEON assembly. */
    extern ne10_result_t ne10_mulcmatvec_cm2x2f_v2f_asm (ne10_vec2f_t * dst, const ne10_mat2x2f_t * cst, ne10_vec2f_t * src, ne10_uint32_t count);
    /** @} */

    /**
     * @ingroup groupMaths
     * @defgroup DIV_VEC Vector Division
     *
     * \par
     * These functions implement vector element-wise division operations for single precision floating point values.
     * @{
     */

    /**
     * Divides all scalar elements of an input array by a constant value, storing the
     * results in an output array. Points to @ref ne10_divc_float_c, @ref ne10_divc_float_neon,
     * or @ref ne10_divc_float_asm. This operation can be performed in-place.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  cst   The constant to divide by
     * @param[in]  count The number of scalar values to be processed
     */
    extern ne10_result_t (*ne10_divc_float) (ne10_float32_t * dst, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_divc_float using plain C code. */
    extern ne10_result_t ne10_divc_float_c (ne10_float32_t * dst, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_divc_float using NEON intrinsics. */
    extern ne10_result_t ne10_divc_float_neon (ne10_float32_t * dst, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_divc_float using NEON assembly. */
    extern ne10_result_t ne10_divc_float_asm (ne10_float32_t * dst, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count);

    /**
     * Element-wise divides all the 2D vectors in an input array by a constant vector,
     * storing the results in an output array. Points to @ref ne10_divc_vec2f_c, @ref ne10_divc_vec2f_neon,
     * or @ref ne10_divc_vec2f_asm. This operation can be performed in-place.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  cst   Pointer to the constant 2D vector to divide by
     * @param[in]  count The number of 2D vectors to be processed
     */
    extern ne10_result_t (*ne10_divc_vec2f) (ne10_vec2f_t * dst, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_divc_vec2f using plain C code. */
    extern ne10_result_t ne10_divc_vec2f_c (ne10_vec2f_t * dst, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_divc_vec2f using NEON intrinsics. */
    extern ne10_result_t ne10_divc_vec2f_neon (ne10_vec2f_t * dst, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_divc_vec2f using NEON assembly. */
    extern ne10_result_t ne10_divc_vec2f_asm (ne10_vec2f_t * dst, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count);

    /**
     * Element-wise divides all the 3D vectors in an input array by a constant vector,
     * storing the results in an output array. Points to @ref ne10_divc_vec3f_c, @ref ne10_divc_vec3f_neon,
     * or @ref ne10_divc_vec3f_asm. This operation can be performed in-place.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  cst   Pointer to the constant 3D vector to divide by
     * @param[in]  count The number of 3D vectors to be processed
     */
    extern ne10_result_t (*ne10_divc_vec3f) (ne10_vec3f_t * dst, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_divc_vec3f using plain C code. */
    extern ne10_result_t ne10_divc_vec3f_c (ne10_vec3f_t * dst, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_divc_vec3f using NEON intrinsics. */
    extern ne10_result_t ne10_divc_vec3f_neon (ne10_vec3f_t * dst, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_divc_vec3f using NEON assembly. */
    extern ne10_result_t ne10_divc_vec3f_asm (ne10_vec3f_t * dst, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count);

    /**
     * Element-wise divides all the 4D vectors in an input array by a constant vector,
     * storing the results in an output array. Points to @ref ne10_divc_vec4f_c, @ref ne10_divc_vec4f_neon,
     * or @ref ne10_divc_vec4f_asm. This operation can be performed in-place.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  cst   Pointer to the constant 4D vector to divide by
     * @param[in]  count The number of 4D vectors to be processed
     */
    extern ne10_result_t (*ne10_divc_vec4f) (ne10_vec4f_t * dst, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_divc_vec4f using plain C code. */
    extern ne10_result_t ne10_divc_vec4f_c (ne10_vec4f_t * dst, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_divc_vec4f using NEON intrinsics. */
    extern ne10_result_t ne10_divc_vec4f_neon (ne10_vec4f_t * dst, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_divc_vec4f using NEON assembly. */
    extern ne10_result_t ne10_divc_vec4f_asm (ne10_vec4f_t * dst, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count);

    /**
     * Divides the scalar elements of one input array by those of the same index in another,
     * storing the results in an output array. Points to @ref ne10_div_float_c, @ref ne10_div_float_neon,
     * or @ref ne10_div_float_asm. This operation can be performed in-place.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src1  Pointer to source array of elements to be divided by
     * @param[in]  src2  Pointer to the source array of elements to divide by
     * @param[in]  count The number of divisions to be performed (i.e. the length of the source arrays)
     */
    extern ne10_result_t (*ne10_div_float) (ne10_float32_t * dst, ne10_float32_t * src1, ne10_float32_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_div_float using plain C code. */
    extern ne10_result_t ne10_div_float_c (ne10_float32_t * dst, ne10_float32_t * src1, ne10_float32_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_div_float using NEON intrinsics. */
    extern ne10_result_t ne10_div_float_neon (ne10_float32_t * dst, ne10_float32_t * src1, ne10_float32_t * src2, ne10_uint32_t count) asm ("ne10_div_float_neon");
    /** Specific implementation of @ref ne10_div_float using NEON assembly. */
    extern ne10_result_t ne10_div_float_asm (ne10_float32_t * dst, ne10_float32_t * src1, ne10_float32_t * src2, ne10_uint32_t count);

    /**
     * Element-wise divides the 2D vectors of one input array by those of the same index in
     * another, storing the results in an output array. Points to @ref ne10_vdiv_vec2f_c,
     * @ref ne10_vdiv_vec2f_neon, or @ref ne10_vdiv_vec2f_asm. This operation can be performed in-place.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src1  Pointer to source array of elements to be divided by
     * @param[in]  src2  Pointer to the source array of elements to divide by
     * @param[in]  count The number of divisions to be performed (i.e. the length of the source arrays)
     */
    extern ne10_result_t (*ne10_vdiv_vec2f) (ne10_vec2f_t * dst, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_vdiv_vec2f using plain C code. */
    extern ne10_result_t ne10_vdiv_vec2f_c (ne10_vec2f_t * dst, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_vdiv_vec2f using NEON intrinsics. */
    extern ne10_result_t ne10_vdiv_vec2f_neon (ne10_vec2f_t * dst, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count) asm ("ne10_vdiv_vec2f_neon");
    /** Specific implementation of @ref ne10_vdiv_vec2f using NEON assembly. */
    extern ne10_result_t ne10_vdiv_vec2f_asm (ne10_vec2f_t * dst, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count);

    /**
     * Element-wise divides the 3D vectors of one input array by those of the same index in
     * another, storing the results in an output array. Points to @ref ne10_vdiv_vec3f_c,
     * @ref ne10_vdiv_vec3f_neon, or @ref ne10_vdiv_vec3f_asm. This operation can be performed in-place.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src1  Pointer to source array of elements to be divided by
     * @param[in]  src2  Pointer to the source array of elements to divide by
     * @param[in]  count The number of divisions to be performed (i.e. the length of the source arrays)
     */
    extern ne10_result_t (*ne10_vdiv_vec3f) (ne10_vec3f_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_vdiv_vec3f using plain C code. */
    extern ne10_result_t ne10_vdiv_vec3f_c (ne10_vec3f_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_vdiv_vec3f using NEON intrinsics. */
    extern ne10_result_t ne10_vdiv_vec3f_neon (ne10_vec3f_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count) asm ("ne10_vdiv_vec3f_neon");
    /** Specific implementation of @ref ne10_vdiv_vec3f using NEON assembly. */
    extern ne10_result_t ne10_vdiv_vec3f_asm (ne10_vec3f_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);

    /**
     * Element-wise divides the 4D vectors of one input array by those of the same index in
     * another, storing the results in an output array. Points to @ref ne10_vdiv_vec4f_c,
     * @ref ne10_vdiv_vec4f_neon, or @ref ne10_vdiv_vec4f_asm. This operation can be performed in-place.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src1  Pointer to source array of elements to be divided by
     * @param[in]  src2  Pointer to the source array of elements to divide by
     * @param[in]  count The number of divisions to be performed (i.e. the length of the source arrays)
     */
    extern ne10_result_t (*ne10_vdiv_vec4f) (ne10_vec4f_t * dst, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_vdiv_vec4f using plain C code. */
    extern ne10_result_t ne10_vdiv_vec4f_c (ne10_vec4f_t * dst, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_vdiv_vec4f using NEON intrinsics. */
    extern ne10_result_t ne10_vdiv_vec4f_neon (ne10_vec4f_t * dst, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count) asm ("ne10_vdiv_vec4f_neon");
    /** Specific implementation of @ref ne10_vdiv_vec4f using NEON assembly. */
    extern ne10_result_t ne10_vdiv_vec4f_asm (ne10_vec4f_t * dst, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count);
    /** @} */

    /**
     * @ingroup groupMaths
     * @cond DOXYGEN_EXCLUDE
     * @defgroup DIV_MAT Matrix Division
     *
     * \par
     * These functions implement matrix division operations for single precision floating point values.
     * @{
     */

    /**
     * Divides the 4x4 matrices of one input array by those of the same index in another,
     * storing the results in an output array. Points to @ref ne10_divmat_4x4f_c,
     * @ref ne10_divmat_4x4f_neon, or @ref ne10_divmat_4x4f_asm.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src1  Pointer to source array of elements to be divided by
     * @param[in]  src2  Pointer to the source array of elements to divide by
     * @param[in]  count The number of divisions to be performed (i.e. the length of the source arrays)
     */
    extern ne10_result_t (*ne10_divmat_4x4f) (ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src1, ne10_mat4x4f_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_divmat_4x4f using plain C code. */
    extern ne10_result_t ne10_divmat_4x4f_c (ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src1, ne10_mat4x4f_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_divmat_4x4f using NEON intrinsics. */
    extern ne10_result_t ne10_divmat_4x4f_neon (ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src1, ne10_mat4x4f_t * src2, ne10_uint32_t count) asm ("ne10_divmat_4x4f_neon");
    /** Specific implementation of @ref ne10_divmat_4x4f using NEON assembly. */
    extern ne10_result_t ne10_divmat_4x4f_asm (ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src1, ne10_mat4x4f_t * src2, ne10_uint32_t count);

    /**
     * Divides the 3x3 matrices of one input array by those of the same index in another,
     * storing the results in an output array. Points to @ref ne10_divmat_3x3f_c,
     * @ref ne10_divmat_3x3f_neon, or @ref ne10_divmat_3x3f_asm.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src1  Pointer to source array of elements to be divided by
     * @param[in]  src2  Pointer to the source array of elements to divide by
     * @param[in]  count The number of divisions to be performed (i.e. the length of the source arrays)
     */
    extern ne10_result_t (*ne10_divmat_3x3f) (ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src1, ne10_mat3x3f_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_divmat_3x3f using plain C code. */
    extern ne10_result_t ne10_divmat_3x3f_c (ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src1, ne10_mat3x3f_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_divmat_3x3f using NEON intrinsics. */
    extern ne10_result_t ne10_divmat_3x3f_neon (ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src1, ne10_mat3x3f_t * src2, ne10_uint32_t count) asm ("ne10_divmat_3x3f_neon");
    /** Specific implementation of @ref ne10_divmat_3x3f using NEON assembly. */
    extern ne10_result_t ne10_divmat_3x3f_asm (ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src1, ne10_mat3x3f_t * src2, ne10_uint32_t count);

    /**
     * Divides the 2x2 matrices of one input array by those of the same index in another,
     * storing the results in an output array. Points to @ref ne10_divmat_2x2f_c,
     * @ref ne10_divmat_2x2f_neon, or @ref ne10_divmat_2x2f_asm.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src1  Pointer to source array of elements to be divided by
     * @param[in]  src2  Pointer to the source array of elements to divide by
     * @param[in]  count The number of divisions to be performed (i.e. the length of the source arrays)
     */
    extern ne10_result_t (*ne10_divmat_2x2f) (ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src1, ne10_mat2x2f_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_divmat_2x2f using plain C code. */
    extern ne10_result_t ne10_divmat_2x2f_c (ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src1, ne10_mat2x2f_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_divmat_2x2f using NEON intrinsics. */
    extern ne10_result_t ne10_divmat_2x2f_neon (ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src1, ne10_mat2x2f_t * src2, ne10_uint32_t count) asm ("ne10_divmat_2x2f_neon");
    /** Specific implementation of @ref ne10_divmat_2x2f using NEON assembly. */
    extern ne10_result_t ne10_divmat_2x2f_asm (ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src1, ne10_mat2x2f_t * src2, ne10_uint32_t count);
    /** @}
     * @endcond
     */

    /**
     * @ingroup groupMaths
     * @defgroup SETC_VEC Vector Assignment
     *
     * \par
     * These functions implement vector set operations for single precision floating point values.
     * @{
     */

    /**
     * Sets the scalar elements of an array to a constant value. Points to @ref ne10_setc_float_c,
     * @ref ne10_setc_float_neon, or @ref ne10_setc_float_asm.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  cst   The constant to set the elements of the destination to
     * @param[in]  count The number of elements to set
     */
    extern ne10_result_t (*ne10_setc_float) (ne10_float32_t * dst, const ne10_float32_t cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_setc_float using plain C code. */
    extern ne10_result_t ne10_setc_float_c (ne10_float32_t * dst, const ne10_float32_t cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_setc_float using NEON intrinsics. */
    extern ne10_result_t ne10_setc_float_neon (ne10_float32_t * dst, const ne10_float32_t cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_setc_float using NEON assembly. */
    extern ne10_result_t ne10_setc_float_asm (ne10_float32_t * dst, const ne10_float32_t cst, ne10_uint32_t count);

    /**
     * Sets the 2D vector elements of an array to a constant vector. Points to @ref ne10_setc_vec2f_c,
     * @ref ne10_setc_vec2f_neon, or @ref ne10_setc_vec2f_asm.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  cst   The constant to set the elements of the destination to
     * @param[in]  count The number of elements to set
     */
    extern ne10_result_t (*ne10_setc_vec2f) (ne10_vec2f_t * dst, const ne10_vec2f_t * cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_setc_vec2f using plain C code. */
    extern ne10_result_t ne10_setc_vec2f_c (ne10_vec2f_t * dst, const ne10_vec2f_t * cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_setc_vec2f using NEON intrinsics. */
    extern ne10_result_t ne10_setc_vec2f_neon (ne10_vec2f_t * dst, const ne10_vec2f_t * cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_setc_vec2f using NEON assembly. */
    extern ne10_result_t ne10_setc_vec2f_asm (ne10_vec2f_t * dst, const ne10_vec2f_t * cst, ne10_uint32_t count);

    /**
     * Sets the 3D vector elements of an array to a constant vector. Points to @ref ne10_setc_vec3f_c,
     * @ref ne10_setc_vec3f_neon, or @ref ne10_setc_vec3f_asm.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  cst   The constant to set the elements of the destination to
     * @param[in]  count The number of elements to set
     */
    extern ne10_result_t (*ne10_setc_vec3f) (ne10_vec3f_t * dst, const ne10_vec3f_t * cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_setc_vec3f using plain C code. */
    extern ne10_result_t ne10_setc_vec3f_c (ne10_vec3f_t * dst, const ne10_vec3f_t * cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_setc_vec3f using NEON intrinsics. */
    extern ne10_result_t ne10_setc_vec3f_neon (ne10_vec3f_t * dst, const ne10_vec3f_t * cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_setc_vec3f using NEON assembly. */
    extern ne10_result_t ne10_setc_vec3f_asm (ne10_vec3f_t * dst, const ne10_vec3f_t * cst, ne10_uint32_t count);

    /**
     * Sets the 4D vector elements of an array to a constant vector. Points to @ref ne10_setc_vec4f_c,
     * @ref ne10_setc_vec4f_neon, or @ref ne10_setc_vec4f_asm.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  cst   The constant to set the elements of the destination to
     * @param[in]  count The number of elements to set
     */
    extern ne10_result_t (*ne10_setc_vec4f) (ne10_vec4f_t * dst, const ne10_vec4f_t * cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_setc_vec4f using plain C code. */
    extern ne10_result_t ne10_setc_vec4f_c (ne10_vec4f_t * dst, const ne10_vec4f_t * cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_setc_vec4f using NEON intrinsics. */
    extern ne10_result_t ne10_setc_vec4f_neon (ne10_vec4f_t * dst, const ne10_vec4f_t * cst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_setc_vec4f using NEON assembly. */
    extern ne10_result_t ne10_setc_vec4f_asm (ne10_vec4f_t * dst, const ne10_vec4f_t * cst, ne10_uint32_t count);
    /** @} */

    /**
     * @ingroup groupMaths
     * @defgroup LEN_VEC Vector Modulus (Length)
     *
     * \par
     * These functions implement vector magnitude operations for single precision floating point values.
     * @{
     */

    /**
     * Calculates the magnitude of 2D vectors within an input array, storing the results
     * in an output array. Points to @ref ne10_len_vec2f_c, @ref ne10_len_vec2f_neon, or
     * @ref ne10_len_vec2f_asm.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  count The number of vectors to be processed
     */
    extern ne10_result_t (*ne10_len_vec2f) (ne10_float32_t * dst, ne10_vec2f_t * src, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_len_vec2f using plain C code. */
    extern ne10_result_t ne10_len_vec2f_c (ne10_float32_t * dst, ne10_vec2f_t * src, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_len_vec2f using NEON intrinsics. */
    extern ne10_result_t ne10_len_vec2f_neon (ne10_float32_t * dst, ne10_vec2f_t * src, ne10_uint32_t count) asm ("ne10_len_vec2f_neon");
    /** Specific implementation of @ref ne10_len_vec2f using NEON assembly. */
    extern ne10_result_t ne10_len_vec2f_asm (ne10_float32_t * dst, ne10_vec2f_t * src, ne10_uint32_t count);

    /**
     * Calculates the magnitude of 3D vectors within an input array, storing the results
     * in an output array. Points to @ref ne10_len_vec3f_c, @ref ne10_len_vec3f_neon, or
     * @ref ne10_len_vec3f_asm.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  count The number of vectors to be processed
     */
    extern ne10_result_t (*ne10_len_vec3f) (ne10_float32_t * dst, ne10_vec3f_t * src, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_len_vec3f using plain C code. */
    extern ne10_result_t ne10_len_vec3f_c (ne10_float32_t * dst, ne10_vec3f_t * src, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_len_vec3f using NEON intrinsics. */
    extern ne10_result_t ne10_len_vec3f_neon (ne10_float32_t * dst, ne10_vec3f_t * src, ne10_uint32_t count) asm ("ne10_len_vec3f_neon");
    /** Specific implementation of @ref ne10_len_vec3f using NEON assembly. */
    extern ne10_result_t ne10_len_vec3f_asm (ne10_float32_t * dst, ne10_vec3f_t * src, ne10_uint32_t count);

    /**
     * Calculates the magnitude of 4D vectors within an input array, storing the results
     * in an output array. Points to @ref ne10_len_vec4f_c, @ref ne10_len_vec4f_neon, or
     * @ref ne10_len_vec4f_asm.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  count The number of vectors to be processed
     */
    extern ne10_result_t (*ne10_len_vec4f) (ne10_float32_t * dst, ne10_vec4f_t * src, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_len_vec4f using plain C code. */
    extern ne10_result_t ne10_len_vec4f_c (ne10_float32_t * dst, ne10_vec4f_t * src, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_len_vec4f using NEON intrinsics. */
    extern ne10_result_t ne10_len_vec4f_neon (ne10_float32_t * dst, ne10_vec4f_t * src, ne10_uint32_t count) asm ("ne10_len_vec4f_neon");
    /** Specific implementation of @ref ne10_len_vec4f using NEON assembly. */
    extern ne10_result_t ne10_len_vec4f_asm (ne10_float32_t * dst, ne10_vec4f_t * src, ne10_uint32_t count);
    /** @} */


    /**
     * @ingroup groupMaths
     * @defgroup NORM_VEC Vector Normalization
     *
     * \par
     * These functions implement vector normalization operations for single precision floating point values.
     * @{
     */

    /**
     * Calculates the normalization of 2D vectors within an input array, storing the results
     * in an output array. Points to @ref ne10_normalize_vec2f_c, @ref ne10_normalize_vec2f_neon,
     * or @ref ne10_normalize_vec2f_asm. This operation can be performed in-place.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  count The number of vectors to be processed
     */
    extern ne10_result_t (*ne10_normalize_vec2f) (ne10_vec2f_t * dst, ne10_vec2f_t * src, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_normalize_vec2f using plain C code. */
    extern ne10_result_t ne10_normalize_vec2f_c (ne10_vec2f_t * dst, ne10_vec2f_t * src, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_normalize_vec2f using NEON intrinsics. */
    extern ne10_result_t ne10_normalize_vec2f_neon (ne10_vec2f_t * dst, ne10_vec2f_t * src, ne10_uint32_t count) asm ("ne10_normalize_vec2f_neon");
    /** Specific implementation of @ref ne10_normalize_vec2f using NEON assembly. */
    extern ne10_result_t ne10_normalize_vec2f_asm (ne10_vec2f_t * dst, ne10_vec2f_t * src, ne10_uint32_t count);

    /**
     * Calculates the normalization of 3D vectors within an input array, storing the results
     * in an output array. Points to @ref ne10_normalize_vec3f_c, @ref ne10_normalize_vec3f_neon,
     * or @ref ne10_normalize_vec3f_asm. This operation can be performed in-place.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  count The number of vectors to be processed
     */
    extern ne10_result_t (*ne10_normalize_vec3f) (ne10_vec3f_t * dst, ne10_vec3f_t * src, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_normalize_vec3f using plain C code. */
    extern ne10_result_t ne10_normalize_vec3f_c (ne10_vec3f_t * dst, ne10_vec3f_t * src, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_normalize_vec3f using NEON intrinsics. */
    extern ne10_result_t ne10_normalize_vec3f_neon (ne10_vec3f_t * dst, ne10_vec3f_t * src, ne10_uint32_t count) asm ("ne10_normalize_vec3f_neon");
    /** Specific implementation of @ref ne10_normalize_vec3f using NEON assembly. */
    extern ne10_result_t ne10_normalize_vec3f_asm (ne10_vec3f_t * dst, ne10_vec3f_t * src, ne10_uint32_t count);

    /**
     * Calculates the normalization of 4D vectors within an input array, storing the results
     * in an output array. Points to @ref ne10_normalize_vec4f_c, @ref ne10_normalize_vec4f_neon,
     * or @ref ne10_normalize_vec4f_asm. This operation can be performed in-place.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  count The number of vectors to be processed
     */
    extern ne10_result_t (*ne10_normalize_vec4f) (ne10_vec4f_t * dst, ne10_vec4f_t * src, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_normalize_vec4f using plain C code. */
    extern ne10_result_t ne10_normalize_vec4f_c (ne10_vec4f_t * dst, ne10_vec4f_t * src, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_normalize_vec4f using NEON intrinsics. */
    extern ne10_result_t ne10_normalize_vec4f_neon (ne10_vec4f_t * dst, ne10_vec4f_t * src, ne10_uint32_t count) asm ("ne10_normalize_vec4f_neon");
    /** Specific implementation of @ref ne10_normalize_vec4f using NEON assembly. */
    extern ne10_result_t ne10_normalize_vec4f_asm (ne10_vec4f_t * dst, ne10_vec4f_t * src, ne10_uint32_t count);
    /** @} */


    /**
     * @ingroup groupMaths
     * @defgroup ABS_VEC Vector Element-wise Absolute Value
     *
     * \par
     * These functions implement vector absolute value operations for single precision floating point values.
     * @{
     */

    /**
     * Calculates the absolute value of scalars within an input array, storing the results
     * in an output array. Points to @ref ne10_abs_float_c, @ref ne10_abs_float_neon, or
     * @ref ne10_abs_float_asm. This operation can be performed in-place.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  count The number of vectors to be processed
     */
    extern ne10_result_t (*ne10_abs_float) (ne10_float32_t * dst, ne10_float32_t * src, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_abs_float using plain C code. */
    extern ne10_result_t ne10_abs_float_c (ne10_float32_t * dst, ne10_float32_t * src, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_abs_float using NEON intrinsics. */
    extern ne10_result_t ne10_abs_float_neon (ne10_float32_t * dst, ne10_float32_t * src, ne10_uint32_t count) asm ("ne10_abs_float_neon");
    /** Specific implementation of @ref ne10_abs_float using NEON assembly. */
    extern ne10_result_t ne10_abs_float_asm (ne10_float32_t * dst, ne10_float32_t * src, ne10_uint32_t count);

    /**
     * Calculates the element-wise absolute value of 2D vectors within an input array, storing
     * the results in an output array. Points to @ref ne10_abs_vec2f_c, @ref ne10_abs_vec2f_neon, or
     * @ref ne10_abs_vec2f_asm. This operation can be performed in-place.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  count The number of vectors to be processed
     */
    extern ne10_result_t (*ne10_abs_vec2f) (ne10_vec2f_t * dst, ne10_vec2f_t * src, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_abs_vec2f using plain C code. */
    extern ne10_result_t ne10_abs_vec2f_c (ne10_vec2f_t * dst, ne10_vec2f_t * src, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_abs_vec2f using NEON intrinsics. */
    extern ne10_result_t ne10_abs_vec2f_neon (ne10_vec2f_t * dst, ne10_vec2f_t * src, ne10_uint32_t count) asm ("ne10_abs_vec2f_neon");
    /** Specific implementation of @ref ne10_abs_vec2f using NEON assembly. */
    extern ne10_result_t ne10_abs_vec2f_asm (ne10_vec2f_t * dst, ne10_vec2f_t * src, ne10_uint32_t count);

    /**
     * Calculates the element-wise absolute value of 3D vectors within an input array, storing
     * the results in an output array. Points to @ref ne10_abs_vec3f_c, @ref ne10_abs_vec3f_neon, or
     * @ref ne10_abs_vec3f_asm. This operation can be performed in-place.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  count The number of vectors to be processed
     */
    extern ne10_result_t (*ne10_abs_vec3f) (ne10_vec3f_t * dst, ne10_vec3f_t * src, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_abs_vec3f using plain C code. */
    extern ne10_result_t ne10_abs_vec3f_c (ne10_vec3f_t * dst, ne10_vec3f_t * src, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_abs_vec3f using NEON intrinsics. */
    extern ne10_result_t ne10_abs_vec3f_neon (ne10_vec3f_t * dst, ne10_vec3f_t * src, ne10_uint32_t count) asm ("ne10_abs_vec3f_neon");
    /** Specific implementation of @ref ne10_abs_vec3f using NEON assembly. */
    extern ne10_result_t ne10_abs_vec3f_asm (ne10_vec3f_t * dst, ne10_vec3f_t * src, ne10_uint32_t count);

    /**
     * Calculates the element-wise absolute value of 4D vectors within an input array, storing
     * the results in an output array. Points to @ref ne10_abs_vec4f_c, @ref ne10_abs_vec4f_neon, or
     * @ref ne10_abs_vec4f_asm. This operation can be performed in-place.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  count The number of vectors to be processed
     */
    extern ne10_result_t (*ne10_abs_vec4f) (ne10_vec4f_t * dst, ne10_vec4f_t * src, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_abs_vec4f using plain C code. */
    extern ne10_result_t ne10_abs_vec4f_c (ne10_vec4f_t * dst, ne10_vec4f_t * src, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_abs_vec4f using NEON intrinsics. */
    extern ne10_result_t ne10_abs_vec4f_neon (ne10_vec4f_t * dst, ne10_vec4f_t * src, ne10_uint32_t count) asm ("ne10_abs_vec4f_neon");
    /** Specific implementation of @ref ne10_abs_vec4f using NEON assembly. */
    extern ne10_result_t ne10_abs_vec4f_asm (ne10_vec4f_t * dst, ne10_vec4f_t * src, ne10_uint32_t count);
    /** @} */

    /**
     * @ingroup groupMaths
     * @defgroup DOT_VEC Vector Dot Product
     *
     * \par
     * These functions implement vector dot product operations for single precision floating point values.
     * @{
     */

    /**
     * Calculates the dot product of 2D vectors from one input array with those of the same
     * index in another, storing the results in an output array. Points to @ref ne10_dot_vec2f_c,
     * @ref ne10_dot_vec2f_neon, or @ref ne10_dot_vec2f_asm.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src1  Pointer to the first source array
     * @param[in]  src2  Pointer to the second source array
     * @param[in]  count The number of dot products to be performed (i.e. the length of the source arrays)
     */
    extern ne10_result_t (*ne10_dot_vec2f) (ne10_float32_t * dst, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_dot_vec2f using plain C code. */
    extern ne10_result_t ne10_dot_vec2f_c (ne10_float32_t * dst, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_dot_vec2f using NEON intrinsics. */
    extern ne10_result_t ne10_dot_vec2f_neon (ne10_float32_t * dst, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count) asm ("ne10_dot_vec2f_neon");
    /** Specific implementation of @ref ne10_dot_vec2f using NEON assembly. */
    extern ne10_result_t ne10_dot_vec2f_asm (ne10_float32_t * dst, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count);

    /**
     * Calculate the dot product of 3D vectors from one input array with those of the same
     * index in another, storing the results in an output array. Points to @ref ne10_dot_vec3f_c,
     * @ref ne10_dot_vec3f_neon, or @ref ne10_dot_vec3f_asm.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src1  Pointer to the first source array
     * @param[in]  src2  Pointer to the second source array
     * @param[in]  count The number of dot products to be performed (i.e. the length of the source arrays)
     */
    extern ne10_result_t (*ne10_dot_vec3f) (ne10_float32_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_dot_vec3f using plain C code. */
    extern ne10_result_t ne10_dot_vec3f_c (ne10_float32_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_dot_vec3f using NEON intrinsics. */
    extern ne10_result_t ne10_dot_vec3f_neon (ne10_float32_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count) asm ("ne10_dot_vec3f_neon");
    /** Specific implementation of @ref ne10_dot_vec3f using NEON assembly. */
    extern ne10_result_t ne10_dot_vec3f_asm (ne10_float32_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);

    /**
     * Calculates the dot product of 4D vectors from one input array with those of the same
     * index in another, storing the results in an output array. Points to @ref ne10_dot_vec4f_c,
     * @ref ne10_dot_vec4f_neon, or @ref ne10_dot_vec4f_asm.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src1  Pointer to the first source array
     * @param[in]  src2  Pointer to the second source array
     * @param[in]  count The number of dot products to be performed (i.e. the length of the source arrays)
     */
    extern ne10_result_t (*ne10_dot_vec4f) (ne10_float32_t * dst, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_dot_vec4f using plain C code. */
    extern ne10_result_t ne10_dot_vec4f_c (ne10_float32_t * dst, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_dot_vec4f using NEON intrinsics. */
    extern ne10_result_t ne10_dot_vec4f_neon (ne10_float32_t * dst, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count) asm ("ne10_dot_vec4f_neon");
    /** Specific implementation of @ref ne10_dot_vec4f using NEON assembly. */
    extern ne10_result_t ne10_dot_vec4f_asm (ne10_float32_t * dst, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count);
    /** @} */


    /**
     * @ingroup groupMaths
     * @defgroup CROSS_VEC Vector Cross Product
     *
     * \par
     * These functions implement vector cross product operations for single precision floating point values.
     * @{
     */

    /**
     * Calculate the cross product of 3D vectors from one input array with those of the same
     * index in another, storing the results in an output array. Points to @ref ne10_cross_vec3f_c,
     * @ref ne10_cross_vec3f_neon, or @ref ne10_cross_vec3f_asm. This operation cannot be performed in-place.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src1  Pointer to the first source array
     * @param[in]  src2  Pointer to the second source array
     * @param[in]  count The number of dot products to be performed (i.e. the length of the source arrays)
     */
    extern ne10_result_t (*ne10_cross_vec3f) (ne10_vec3f_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_cross_vec3f using plain C code. */
    extern ne10_result_t ne10_cross_vec3f_c (ne10_vec3f_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_cross_vec3f using NEON intrinsics. */
    extern ne10_result_t ne10_cross_vec3f_neon (ne10_vec3f_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count) asm ("ne10_cross_vec3f_neon");
    /** Specific implementation of @ref ne10_cross_vec3f using NEON assembly. */
    extern ne10_result_t ne10_cross_vec3f_asm (ne10_vec3f_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);
    /** @} */

    /**
     * @ingroup groupMaths
     * @defgroup DET_MAT Matrix Determinant Calculation
     *
     * \par
     * These functions implement matrix determinant operations for single precision floating point values.
     * @{
     */

    /**
     * Calculates the determinant of 4x4 matrices within an input array, storing the results
     * in an output array. Points to @ref ne10_detmat_4x4f_c, @ref ne10_detmat_4x4f_neon, or
     * @ref ne10_detmat_4x4f_asm.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  count The number of matrices to be processed
     */
    extern ne10_result_t (*ne10_detmat_4x4f) (ne10_float32_t * dst, ne10_mat4x4f_t * src, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_detmat_4x4f using plain C code. */
    extern ne10_result_t ne10_detmat_4x4f_c (ne10_float32_t * dst, ne10_mat4x4f_t * src, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_detmat_4x4f using NEON intrinsics. */
    extern ne10_result_t ne10_detmat_4x4f_neon (ne10_float32_t * dst, ne10_mat4x4f_t * src, ne10_uint32_t count) asm ("ne10_detmat_4x4f_neon");
    /** Specific implementation of @ref ne10_detmat_4x4f using NEON assembly. */
    extern ne10_result_t ne10_detmat_4x4f_asm (ne10_float32_t * dst, ne10_mat4x4f_t * src, ne10_uint32_t count);

    /**
     * Calculates the determinant of 3x3 matrices within an input array, storing the results
     * in an output array. Points to @ref ne10_detmat_3x3f_c, @ref ne10_detmat_3x3f_neon, or
     * @ref ne10_detmat_3x3f_asm.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  count The number of matrices to be processed
     */
    extern ne10_result_t (*ne10_detmat_3x3f) (ne10_float32_t * dst, ne10_mat3x3f_t * src, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_detmat_3x3f using plain C code. */
    extern ne10_result_t ne10_detmat_3x3f_c (ne10_float32_t * dst, ne10_mat3x3f_t * src, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_detmat_3x3f using NEON intrinsics. */
    extern ne10_result_t ne10_detmat_3x3f_neon (ne10_float32_t * dst, ne10_mat3x3f_t * src, ne10_uint32_t count) asm ("ne10_detmat_3x3f_neon");
    /** Specific implementation of @ref ne10_detmat_3x3f using NEON assembly. */
    extern ne10_result_t ne10_detmat_3x3f_asm (ne10_float32_t * dst, ne10_mat3x3f_t * src, ne10_uint32_t count);

    /**
     * Calculates the determinant of 2x2 matrices within an input array, storing the results
     * in an output array. Points to @ref ne10_detmat_2x2f_c, @ref ne10_detmat_2x2f_neon, or
     * @ref ne10_detmat_2x2f_asm.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  count The number of matrices to be processed
     */
    extern ne10_result_t (*ne10_detmat_2x2f) (ne10_float32_t * dst, ne10_mat2x2f_t * src, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_detmat_2x2f using plain C code. */
    extern ne10_result_t ne10_detmat_2x2f_c (ne10_float32_t * dst, ne10_mat2x2f_t * src, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_detmat_2x2f using NEON intrinsics. */
    extern ne10_result_t ne10_detmat_2x2f_neon (ne10_float32_t * dst, ne10_mat2x2f_t * src, ne10_uint32_t count) asm ("ne10_detmat_2x2f_neon");
    /** Specific implementation of @ref ne10_detmat_2x2f using NEON assembly. */
    extern ne10_result_t ne10_detmat_2x2f_asm (ne10_float32_t * dst, ne10_mat2x2f_t * src, ne10_uint32_t count);
    /** @} */

    /**
     * @ingroup groupMaths
     * @defgroup INV_MAT Matrix Inversion
     *
     * \par
     * These functions implement matrix inversion operations for single precision floating point values.
     * @{
     */

    /**
     * Calculates the inverse of 4x4 matrices within an input array, storing the results
     * in an output array. Points to @ref ne10_invmat_4x4f_c, @ref ne10_invmat_4x4f_neon, or
     * @ref ne10_invmat_4x4f_asm.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  count The number of matrices to be processed
     */
    extern ne10_result_t (*ne10_invmat_4x4f) (ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_invmat_4x4f using plain C code. */
    extern ne10_result_t ne10_invmat_4x4f_c (ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_invmat_4x4f using NEON intrinsics. */
    extern ne10_result_t ne10_invmat_4x4f_neon (ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src, ne10_uint32_t count) asm ("ne10_invmat_4x4f_neon");
    /** Specific implementation of @ref ne10_invmat_4x4f using NEON assembly. */
    extern ne10_result_t ne10_invmat_4x4f_asm (ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src, ne10_uint32_t count);

    /**
     * Calculates the inverse of 3x3 matrices within an input array, storing the results
     * in an output array. Points to @ref ne10_invmat_3x3f_c, @ref ne10_invmat_3x3f_neon, or
     * @ref ne10_invmat_3x3f_asm.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  count The number of matrices to be processed
     */
    extern ne10_result_t (*ne10_invmat_3x3f) (ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_invmat_3x3f using plain C code. */
    extern ne10_result_t ne10_invmat_3x3f_c (ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_invmat_3x3f using NEON intrinsics. */
    extern ne10_result_t ne10_invmat_3x3f_neon (ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src, ne10_uint32_t count) asm ("ne10_invmat_3x3f_neon");
    /** Specific implementation of @ref ne10_invmat_3x3f using NEON assembly. */
    extern ne10_result_t ne10_invmat_3x3f_asm (ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src, ne10_uint32_t count);

    /**
     * Calculates the inverse of 2x2 matrices within an input array, storing the results
     * in an output array. Points to @ref ne10_invmat_2x2f_c, @ref ne10_invmat_2x2f_neon, or
     * @ref ne10_invmat_2x2f_asm.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  count The number of matrices to be processed
     */
    extern ne10_result_t (*ne10_invmat_2x2f) (ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_invmat_2x2f using plain C code. */
    extern ne10_result_t ne10_invmat_2x2f_c (ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_invmat_2x2f using NEON intrinsics. */
    extern ne10_result_t ne10_invmat_2x2f_neon (ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src, ne10_uint32_t count) asm ("ne10_invmat_2x2f_neon");
    /** Specific implementation of @ref ne10_invmat_2x2f using NEON assembly. */
    extern ne10_result_t ne10_invmat_2x2f_asm (ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src, ne10_uint32_t count);
    /** @} */

    /**
     * @ingroup groupMaths
     * @defgroup TRANS_MAT Matrix Transposition
     *
     * \par
     * These functions implement matrix transpose operations for single precision floating point values.
     * @{
     */

    /**
     * Calculates the transpose of 4x4 matrices within an input array, storing the results
     * in an output array. Points to @ref ne10_transmat_4x4f_c, @ref ne10_transmat_4x4f_neon, or
     * @ref ne10_transmat_4x4f_asm. This operation cannot be performed in-place.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  count The number of matrices to be processed
     */
    extern ne10_result_t (*ne10_transmat_4x4f) (ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_transmat_4x4f using plain C code. */
    extern ne10_result_t ne10_transmat_4x4f_c (ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_transmat_4x4f using NEON intrinsics. */
    extern ne10_result_t ne10_transmat_4x4f_neon (ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src, ne10_uint32_t count) asm ("ne10_transmat_4x4f_neon");
    /** Specific implementation of @ref ne10_transmat_4x4f using NEON assembly. */
    extern ne10_result_t ne10_transmat_4x4f_asm (ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src, ne10_uint32_t count);

    /**
     * Calculates the transpose of 3x3 matrices within an input array, storing the results
     * in an output array. Points to @ref ne10_transmat_3x3f_c, @ref ne10_transmat_3x3f_neon, or
     * @ref ne10_transmat_3x3f_asm. This operation cannot be performed in-place.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  count The number of matrices to be processed
     */
    extern ne10_result_t (*ne10_transmat_3x3f) (ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_transmat_3x3f using plain C code. */
    extern ne10_result_t ne10_transmat_3x3f_c (ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_transmat_3x3f using NEON intrinsics. */
    extern ne10_result_t ne10_transmat_3x3f_neon (ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src, ne10_uint32_t count) asm ("ne10_transmat_3x3f_neon");
    /** Specific implementation of @ref ne10_transmat_3x3f using NEON assembly. */
    extern ne10_result_t ne10_transmat_3x3f_asm (ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src, ne10_uint32_t count);

    /**
     * Calculates the transpose of 2x2 matrices within an input array, storing the results
     * in an output array. Points to @ref ne10_transmat_2x2f_c, @ref ne10_transmat_2x2f_neon, or
     * @ref ne10_transmat_2x2f_asm. This operation cannot be performed in-place.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  src   Pointer to the source array
     * @param[in]  count The number of matrices to be processed
     */
    extern ne10_result_t (*ne10_transmat_2x2f) (ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_transmat_2x2f using plain C code. */
    extern ne10_result_t ne10_transmat_2x2f_c (ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_transmat_2x2f using NEON intrinsics. */
    extern ne10_result_t ne10_transmat_2x2f_neon (ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src, ne10_uint32_t count) asm ("ne10_transmat_2x2f_neon");
    /** Specific implementation of @ref ne10_transmat_2x2f using NEON assembly. */
    extern ne10_result_t ne10_trans_mat2x2f_asm (ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src, ne10_uint32_t count);
    /** @} */

    /**
     * @ingroup groupMaths
     * @defgroup IDENTITY_MAT Matrix Identity Assignment
     *
     * \par
     * These functions implement matrix identity operations for single precision floating point values.
     * @{
     */

    /**
     * Sets the elements of an array of 4x4 matrices to the identity matrix. Points to
     * @ref ne10_identitymat_4x4f_c, @ref ne10_identitymat_4x4f_neon, or @ref ne10_identitymat_4x4f_asm.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  count The number of elements to set
     */
    extern ne10_result_t (*ne10_identitymat_4x4f) (ne10_mat4x4f_t * dst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_identitymat_4x4f using plain C code. */
    extern ne10_result_t ne10_identitymat_4x4f_c (ne10_mat4x4f_t * dst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_identitymat_4x4f using NEON intrinsics. */
    extern ne10_result_t ne10_identitymat_4x4f_neon (ne10_mat4x4f_t * dst, ne10_uint32_t count) asm ("ne10_identitymat_4x4f_neon");
    /** Specific implementation of @ref ne10_identitymat_4x4f using NEON assembly. */
    extern ne10_result_t ne10_identitymat_4x4f_asm (ne10_mat4x4f_t * dst, ne10_uint32_t count);

    /**
     * Sets the elements of an array of 3x3 matrices to the identity matrix. Points to
     * @ref ne10_identitymat_3x3f_c, @ref ne10_identitymat_3x3f_neon, or @ref ne10_identitymat_3x3f_asm.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  count The number of elements to set
     */
    extern ne10_result_t (*ne10_identitymat_3x3f) (ne10_mat3x3f_t * dst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_identitymat_3x3f using plain C code. */
    extern ne10_result_t ne10_identitymat_3x3f_c (ne10_mat3x3f_t * dst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_identitymat_3x3f using NEON intrinsics. */
    extern ne10_result_t ne10_identitymat_3x3f_neon (ne10_mat3x3f_t * dst, ne10_uint32_t count) asm ("ne10_identitymat_3x3f_neon");
    /** Specific implementation of @ref ne10_identitymat_3x3f using NEON assembly. */
    extern ne10_result_t ne10_identitymat_3x3f_asm (ne10_mat3x3f_t * dst, ne10_uint32_t count);

    /**
     * Sets the elements of an array of 2x2 matrices to the identity matrix. Points to
     * @ref ne10_identitymat_2x2f_c, @ref ne10_identitymat_2x2f_neon, or @ref ne10_identitymat_2x2f_asm.
     *
     * @param[out] dst   Pointer to the destination array
     * @param[in]  count The number of elements to set
     */
    extern ne10_result_t (*ne10_identitymat_2x2f) (ne10_mat2x2f_t * dst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_identitymat_2x2f using plain C code. */
    extern ne10_result_t ne10_identitymat_2x2f_c (ne10_mat2x2f_t * dst, ne10_uint32_t count);
    /** Specific implementation of @ref ne10_identitymat_2x2f using NEON intrinsics. */
    extern ne10_result_t ne10_identitymat_2x2f_neon (ne10_mat2x2f_t * dst, ne10_uint32_t count) asm ("ne10_identitymat_2x2f_neon");
    /** Specific implementation of @ref ne10_identitymat_2x2f using NEON assembly. */
    extern ne10_result_t ne10_identity_mat2x2f_asm (ne10_mat2x2f_t * dst, ne10_uint32_t count);
    /** @} */

#ifdef __cplusplus
}
#endif

#endif
