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
 * NE10 Library : inc/NE10_math.h
 */

/*! \file NE10_math.h
    \brief All NE10 math routines declarations.

    The routines that are provided by this library are all declared in this header file.
 */

#include <NE10_types.h>

#ifndef NE10_MATH_H
#define NE10_MATH_H

#ifdef __cplusplus
extern "C" {
#endif

///////////////////////////
// function prototypes:
///////////////////////////


// ## Vector-Constant Arithmetic ##

/*!
    Adds a constant scalar value to all the elements of an input array and stores the results in an output array.
    @param[out] dst   Pointer to the destination array
    @param[in]  src   Pointer to the source array
    @param[in]  cst   The constant scalar added to the input values
    @param[in]  count The number of items in the input array
 */
extern ne10_result_t (*addc_float)(ne10_float32_t * dst, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count);
/*!
    Adds a constant 2D vector to all of the vectors in an input array and stores the results in an output array.
    @param[out] dst   Pointer to the destination array
    @param[in]  src   Pointer to the source array
    @param[in]  cst   Pointer to the 2D vector added to the input values
    @param[in]  count The number of items in the input array
 */
extern ne10_result_t (*addc_vec2f)(ne10_vec2f_t * dst, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count);
/*!
    Adds a constant 3D vector to all of the vectors in an input array and stores the results in an output array.
    @param[out] dst   Pointer to the destination array
    @param[in]  src   Pointer to the source array
    @param[in]  cst   Pointer to the 3D vector added to the input values
    @param[in]  count The number of items in the input array
 */
extern ne10_result_t (*addc_vec3f)(ne10_vec3f_t * dst, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count);
/*!
    Adds a constant 4D vector to all of the vectors in an input array and stores the results in an output array.
    @param[out] dst   Pointer to the destination array
    @param[in]  src   Pointer to the source array
    @param[in]  cst   Pointer to the 4D vector added to the input values
    @param[in]  count The number of items in the input array
 */
extern ne10_result_t (*addc_vec4f)(ne10_vec4f_t * dst, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count);



/*!
    Subtracts a constant scalar from all the elements of an input array and stores the results in an output array.
    @param[out] dst   Pointer to the destination array
    @param[in]  src   Pointer to the source array
    @param[in]  cst   The constant scalar subtracted from the input values
    @param[in]  count The number of items in the input array
 */
extern ne10_result_t (*subc_float)(ne10_float32_t * dst, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count);
/*!
    Subtracts a constant 2D vector from all of the vectors in an input array and stores the results in an output array.
    @param[out] dst   Pointer to the destination array
    @param[in]  src   Pointer to the source array
    @param[in]  cst   Pointer to the 2D vector subtracted from the input values
    @param[in]  count The number of items in the input array
 */
extern ne10_result_t (*subc_vec2f)(ne10_vec2f_t * dst, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count);
/*!
    Subtracts a constant 3D vector from all of the vectors in an input array and stores the results in an output array.
    @param[out] dst   Pointer to the destination array
    @param[in]  src   Pointer to the source array
    @param[in]  cst   Pointer to the 3D vector subtracted from the input values
    @param[in]  count The number of items in the input array
 */
extern ne10_result_t (*subc_vec3f)(ne10_vec3f_t * dst, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count);
/*!
    Subtracts a constant 4D vector from all of the vectors in an input array and stores the results in an output array.
    @param[out] dst   Pointer to the destination array
    @param[in]  src   Pointer to the source array
    @param[in]  cst   Pointer to the 4D vector subtracted from the input values
    @param[in]  count The number of items in the input array
 */
extern ne10_result_t (*subc_vec4f)(ne10_vec4f_t * dst, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count);



/*!
    Subtracts the elements of an input array from a constant scalar and stores the results in an output array.
    @param[out] dst   Pointer to the destination array
    @param[in]  src   Pointer to the source array
    @param[in]  cst   The constant scalar to subtract the input values from
    @param[in]  count The number of items in the input array
 */
extern ne10_result_t (*rsbc_float)(ne10_float32_t * dst, ne10_float32_t *src, const ne10_float32_t cst, ne10_uint32_t count);
/*!
    Subtracts the vectors in an input array from a constant 2D vector and stores the results in an output array.
    @param[out] dst   Pointer to the destination array
    @param[in]  src   Pointer to the source array
    @param[in]  cst   Pointer to the 2D vector to subtract the input values from
    @param[in]  count The number of items in the input array
 */
extern ne10_result_t (*rsbc_vec2f)(ne10_vec2f_t * dst, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count);
/*!
    Subtracts the vectors in an input array from a constant 3D vector and stores the results in an output array.
    @param[out] dst   Pointer to the destination array
    @param[in]  src   Pointer to the source array
    @param[in]  cst   Pointer to the 3D vector to subtract the input values from
    @param[in]  count The number of items in the input array
 */
extern ne10_result_t (*rsbc_vec3f)(ne10_vec3f_t * dst, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count);
/*!
    Subtracts the vectors in an input array from a constant 4D vector and stores the results in an output array.
    @param[out] dst   Pointer to the destination array
    @param[in]  src   Pointer to the source array
    @param[in]  cst   Pointer to the 4D vector to subtract the input values from
    @param[in]  count The number of items in the input array
 */
extern ne10_result_t (*rsbc_vec4f)(ne10_vec4f_t * dst, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count);



/*!
    Multiplies the elements of an input array by a constant scalar and stores the results in an output array.
    @param[out] dst   Pointer to the destination array
    @param[in]  src   Pointer to the source array
    @param[in]  cst   The constant scalar to multiply the input values with
    @param[in]  count The number of items in the input array
 */
extern ne10_result_t (*mulc_float)(ne10_float32_t * dst, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count);
/*!
    Multiplies the components of 2D vectors in an input array by the components of a constant 2D vector and stores the results in an output array.
    @param[out] dst   Pointer to the destination array
    @param[in]  src   Pointer to the source array
    @param[in]  cst   Pointer to the 2D vector to multiply the input values with
    @param[in]  count The number of items in the input array
 */
extern ne10_result_t (*mulc_vec2f)(ne10_vec2f_t * dst, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count);
/*!
    Multiplies the components of 3D vectors in an input array by the components of a constant 3D vector and stores the results in an output array.
    @param[out] dst   Pointer to the destination array
    @param[in]  src   Pointer to the source array
    @param[in]  cst   Pointer to the 3D vector to multiply the input values with
    @param[in]  count The number of items in the input array
 */
extern ne10_result_t (*mulc_vec3f)(ne10_vec3f_t * dst, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count);
/*!
    Multiplies the components of 4D vectors in an input array by the components of a constant 4D vector and stores the results in an output array.
    @param[out] dst   Pointer to the destination array
    @param[in]  src   Pointer to the source array
    @param[in]  cst   Pointer to the 4D vector to multiply the input values with
    @param[in]  count The number of items in the input array
 */
extern ne10_result_t (*mulc_vec4f)(ne10_vec4f_t * dst, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count);



/*!
    Divides the elements of an input array by a constant scalar and stores the results in an output array.
    @param[out] dst   Pointer to the destination array
    @param[in]  src   Pointer to the source array
    @param[in]  cst   The constant scalar to divide the input values by
    @param[in]  count The number of items in the input array
 */
extern ne10_result_t (*divc_float)(ne10_float32_t * dst, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count);
/*!
    Divides the components of 2D vectors in an input array with the components of a constant 2D vector and stores the results in an output array.
    @param[out] dst   Pointer to the destination array
    @param[in]  src   Pointer to the source array
    @param[in]  cst   Pointer to the 2D vector to divide the input values by
    @param[in]  count The number of items in the input array
 */
extern ne10_result_t (*divc_vec2f)(ne10_vec2f_t * dst, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count);
/*!
    Divides the components of 3D vectors in an input array with the components of a constant 3D vector and stores the results in an output array.
    @param[out] dst   Pointer to the destination array
    @param[in]  src   Pointer to the source array
    @param[in]  cst   Pointer to the 3D vector to divide the input values by
    @param[in]  count The number of items in the input array
 */
extern ne10_result_t (*divc_vec3f)(ne10_vec3f_t * dst, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count);
/*!
    Divides the components of 4D vectors in an input array with the components of a constant 4D vector and stores the results in an output array.
    @param[out] dst   Pointer to the destination array
    @param[in]  src   Pointer to the source array
    @param[in]  cst   Pointer to the 4D vector to divide the input values by
    @param[in]  count The number of items in the input array
 */
extern ne10_result_t (*divc_vec4f)(ne10_vec4f_t * dst, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count);



/*!
    Sets the elements of an input array to a constant scalar and stores the results in an output array.
    @param[out] dst   Pointer to the destination array
    @param[in]  cst   The constant scalar to set the input values to
    @param[in]  count The number of items in the input array
 */
extern ne10_result_t (*setc_float)(ne10_float32_t * dst, const ne10_float32_t cst, ne10_uint32_t count);
/*!
    Sets the components of 2D vectors in an input array to the components of a constant 2D vector and stores the results in an output array.
    @param[out] dst   Pointer to the destination array
    @param[in]  cst   Pointer to the 2D vector to set the input values to
    @param[in]  count The number of items in the input array
 */
extern ne10_result_t (*setc_vec2f)(ne10_vec2f_t * dst, const ne10_vec2f_t * cst, ne10_uint32_t count);
/*!
    Sets the components of 3D vectors in an input array to the components of a constant 3D vector and stores the results in an output array.
    @param[out] dst   Pointer to the destination array
    @param[in]  cst   Pointer to the 3D vector to set the input values to
    @param[in]  count The number of items in the input array
 */
extern ne10_result_t (*setc_vec3f)(ne10_vec3f_t * dst, const ne10_vec3f_t * cst, ne10_uint32_t count);
/*!
    Sets the components of 3D vectors in an input array to the components of a constant 3D vector and stores the results in an output array.
    @param[out] dst   Pointer to the destination array
    @param[in]  cst   Pointer to the 4D vector to set the input values to
    @param[in]  count The number of items in the input array
 */
extern ne10_result_t (*setc_vec4f)(ne10_vec4f_t * dst, const ne10_vec4f_t * cst, ne10_uint32_t count);



/*!
    Multiplies each entry in the source array (src) by cst, then adds the result to
     the corresponding item of the accumulation array (acc), and stores the result in the destination array.
    @param[out] dst   Pointer to the destination array
    @param[in]  acc   The corresponding elemetn is added to the result of the multiplication
    @param[in]  src   Pointer to the source array
    @param[in]  cst   The constant scalar to multiply the input elements with
    @param[in]  count The number of items in the input array
 */
extern ne10_result_t (*mlac_float)(ne10_float32_t * dst, ne10_float32_t * acc, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count);
/*!
   Multiplies each entry in the source array (src) by the 2D vector cst, then adds the result to
     the corresponding item of the accumulation array (acc), and stores the result in the destination array.
    @param[out] dst   Pointer to the destination array
    @param[in]  acc   The corresponding elemetn is added to the result of the multiplication
    @param[in]  src   Pointer to the source array
    @param[in]  cst   Pointer to the 2D vector to multiply the input vectors with
    @param[in]  count The number of items in the input array
 */
extern ne10_result_t (*mlac_vec2f)(ne10_vec2f_t * dst, ne10_vec2f_t * acc, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count);
/*!
   Multiplies each entry in the source array (src) by the 3D vector cst, then adds the result to
     the corresponding item of the accumulation array (acc), and stores the result in the destination array.
    @param[out] dst   Pointer to the destination array
    @param[in]  acc   The corresponding elemetn is added to the result of the multiplication
    @param[in]  src   Pointer to the source array
    @param[in]  cst   Pointer to the 3D vector to multiply the input vectors with
    @param[in]  count The number of items in the input array
 */
extern ne10_result_t (*mlac_vec3f)(ne10_vec3f_t * dst, ne10_vec3f_t * acc, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count);
/*!
   Multiplies each entry in the source array (src) by the 4D vector cst, then adds the result to
     the corresponding item of the accumulation array (acc), and stores the result in the destination array.
    @param[out] dst   Pointer to the destination array
    @param[in]  acc   The corresponding elemetn is added to the result of the multiplication
    @param[in]  src   Pointer to the source array
    @param[in]  cst   Pointer to the 4D vector to multiply the input vectors with
    @param[in]  count The number of items in the input array
 */
extern ne10_result_t (*mlac_vec4f)(ne10_vec4f_t * dst, ne10_vec4f_t * acc, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count);



// ## Arithmetic functions over arrays of cst values ##

/*!
    Adds the elements of src1 to the elements of src2 and stores the results in the dst.
    @param[out] dst   Pointer to the destination array
    @param[in]  src1  The first array to use as the input array
    @param[in]  src2  The second array to use as the input array
    @param[in]  count The number of items in the two input arrays
 */
extern ne10_result_t (*add_float)(ne10_float32_t * dst, ne10_float32_t * src1, ne10_float32_t * src2, ne10_uint32_t count);
/*!
    Subtracts the elements of src2 from the elements of src2 and stores the results in the dst.
    @param[out] dst   Pointer to the destination array
    @param[in]  src1  The first array to use as the input array
    @param[in]  src2  The second array to use as the input array
    @param[in]  count The number of items in the two input arrays
 */
extern ne10_result_t (*sub_float)(ne10_float32_t * dst, ne10_float32_t * src1, ne10_float32_t * src2, ne10_uint32_t count);
/*!
    Multiplies the elements of src1 by the elements of src2 and stores the results in the dst.
    @param[out] dst   Pointer to the destination array
    @param[in]  src1  The first array to use as the input array
    @param[in]  src2  The second array to use as the input array
    @param[in]  count The number of items in the two input arrays
 */
extern ne10_result_t (*mul_float)(ne10_float32_t * dst, ne10_float32_t * src1, ne10_float32_t * src2, ne10_uint32_t count);
/*!
    Divides the elements of src1 by the elements of src2 and stores the results in the dst.
    @param[out] dst   Pointer to the destination array
    @param[in]  src1  The first array to use as the input array
    @param[in]  src2  The second array to use as the input array
    @param[in]  count The number of items in the two input arrays
 */
extern ne10_result_t (*div_float)(ne10_float32_t * dst, ne10_float32_t * src1, ne10_float32_t * src2, ne10_uint32_t count);
/*!
    Performs a multiply and accumulate operation using the corresponding elements in acc, src1, and src2.
    @param[out] dst   Pointer to the destination array
    @param[in]  acc   These elemtns are added to the result of the multiplication operation
    @param[in]  src1  The first array to use as the input array
    @param[in]  src2  The second array to use as the input array
    @param[in]  count The number of items in the two input arrays
 */
extern ne10_result_t (*mla_float)(ne10_float32_t * dst, ne10_float32_t * acc, ne10_float32_t * src1, ne10_float32_t * src2, ne10_uint32_t count);
/*!
    Calculates the absolute value of each element in the source array and stores the result in the corresponding entry of the destination array.
    @param[out] dst   Pointer to the destination array
    @param[in]  src   Pointer to the source array
    @param[in]  count The number of items in the input array
 */
extern ne10_result_t (*abs_float)(ne10_float32_t * dst, ne10_float32_t * src, ne10_uint32_t count);



// ## Operations on Vectors ##
/*!
    Returns length of 2D vectors in corresponding elements of the output array.
    @param[out] dst   Pointer to the destination array
    @param[in]  src   Pointer to the source array
    @param[in]  count The number of items in the input array
 */
extern ne10_result_t (*len_vec2f)(ne10_float32_t * dst, ne10_vec2f_t * src, ne10_uint32_t count);
/*!
    Returns length of 3D vectors in corresponding elements of the output array.
    @param[out] dst   Pointer to the destination array
    @param[in]  src   Pointer to the source array
    @param[in]  count The number of items in the input array
 */
extern ne10_result_t (*len_vec3f)(ne10_float32_t * dst, ne10_vec3f_t * src, ne10_uint32_t count);
/*!
    Returns length of 4D vectors in corresponding elements of the output array.
    @param[out] dst   Pointer to the destination array
    @param[in]  src   Pointer to the source array
    @param[in]  count The number of items in the input array
 */
extern ne10_result_t (*len_vec4f)(ne10_float32_t * dst, ne10_vec4f_t * src, ne10_uint32_t count);



/*!
    Normalizes 2D vectors of the input array and stores them in the corresponding elements of the output array.
    @param[out] dst   Pointer to the destination array
    @param[in]  src   Pointer to the source array
    @param[in]  count The number of items in the input array
 */
extern ne10_result_t (*normalize_vec2f)(ne10_vec2f_t * dst, ne10_vec2f_t * src, ne10_uint32_t count);
/*!
    Normalizes 3D vectors of the input array and stores them in the corresponding elements of the output array.
    @param[out] dst   Pointer to the destination array
    @param[in]  src   Pointer to the source array
    @param[in]  count The number of items in the input array
 */
extern ne10_result_t (*normalize_vec3f)(ne10_vec3f_t * dst, ne10_vec3f_t * src, ne10_uint32_t count);
/*!
    Normalizes 4D vectors of the input array and stores them in the corresponding elements of the output array.
    @param[out] dst   Pointer to the destination array
    @param[in]  src   Pointer to the source array
    @param[in]  count The number of items in the input array
 */
extern ne10_result_t (*normalize_vec4f)(ne10_vec4f_t * dst, ne10_vec4f_t * src, ne10_uint32_t count);




/*!
    Generates a 2D vector from the absolute values of each of the components of an input vector
    @param[out] dst   Pointer to the destination array
    @param[in]  src   Pointer to the source array
    @param[in]  count The number of items in the input array
 */
extern ne10_result_t (*abs_vec2f)(ne10_vec2f_t * dst, ne10_vec2f_t * src, ne10_uint32_t count);
/*!
    Generates a 3D vector from the absolute values of each of the components of an input vector
    @param[out] dst   Pointer to the destination array
    @param[in]  src   Pointer to the source array
    @param[in]  count The number of items in the input array
 */
extern ne10_result_t (*abs_vec3f)(ne10_vec3f_t * dst, ne10_vec3f_t * src, ne10_uint32_t count);
/*!
    Generates a 4D vector from the absolute values of each of the components of an input vector
    @param[out] dst   Pointer to the destination array
    @param[in]  src   Pointer to the source array
    @param[in]  count The number of items in the input array
 */
extern ne10_result_t (*abs_vec4f)(ne10_vec4f_t * dst, ne10_vec4f_t * src, ne10_uint32_t count);



// ## SIMD Component-wise Arithmetic on Two Vectors ##

/*!
    Multiplies the components of a 2D vector with the corresponding components of another
    @param[out] dst   Pointer to the destination array
    @param[in]  src1   Pointer to the first source array
    @param[in]  src2   Pointer to the second source array
    @param[in]  count The number of items in the input arrays
 */
extern ne10_result_t (*vmul_vec2f)(ne10_vec2f_t * dst, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count);
/*!
    Multiplies the components of a 3D vector with the corresponding components of another
    @param[out] dst   Pointer to the destination array
    @param[in]  src1   Pointer to the first source array
    @param[in]  src2   Pointer to the second source array
    @param[in]  count The number of items in the input arrays
 */
extern ne10_result_t (*vmul_vec3f)(ne10_vec3f_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);
/*!
    Multiplies the components of a 4D vector with the corresponding components of another
    @param[out] dst   Pointer to the destination array
    @param[in]  src1   Pointer to the first source array
    @param[in]  src2   Pointer to the second source array
    @param[in]  count The number of items in the input arrays
 */
extern ne10_result_t (*vmul_vec4f)(ne10_vec4f_t * dst, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count);



/*!
    Divides the components of a 2D vector with the corresponding components of another
    @param[out] dst   Pointer to the destination array
    @param[in]  src1   Pointer to the nominators' source array
    @param[in]  src2   Pointer to the denominators' source array
    @param[in]  count The number of items in the input arrays
 */
extern ne10_result_t (*vdiv_vec2f)(ne10_vec2f_t * dst, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count);
/*!
    Divides the components of a 3D vector with the corresponding components of another
    @param[out] dst   Pointer to the destination array
    @param[in]  src1   Pointer to the nominators' source array
    @param[in]  src2   Pointer to the denominators' source array
    @param[in]  count The number of items in the input arrays
 */
extern ne10_result_t (*vdiv_vec3f)(ne10_vec3f_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);
/*!
    Divides the components of a 4D vector with the corresponding components of another
    @param[out] dst   Pointer to the destination array
    @param[in]  src1   Pointer to the nominators' source array
    @param[in]  src2   Pointer to the denominators' source array
    @param[in]  count The number of items in the input arrays
 */
extern ne10_result_t (*vdiv_vec4f)(ne10_vec4f_t * dst, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count);



/*!
    Performs a multiply and accumulate operation on the components of a 2D vector with the corresponding components of another
    @param[out] dst   Pointer to the destination array
    @param[in]  src1   Pointer to the first source array
    @param[in]  src2   Pointer to the second source array
    @param[in]  count The number of items in the input arrays
 */
extern ne10_result_t (*vmla_vec2f)(ne10_vec2f_t * dst, ne10_vec2f_t * acc, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count);
/*!
    Performs a multiply and accumulate operation on the components of a 3D vector with the corresponding components of another
    @param[out] dst   Pointer to the destination array
    @param[in]  src1   Pointer to the first source array
    @param[in]  src2   Pointer to the second source array
    @param[in]  count The number of items in the input arrays
 */
extern ne10_result_t (*vmla_vec3f)(ne10_vec3f_t * dst, ne10_vec3f_t * acc, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);
/*!
    Performs a multiply and accumulate operation on the components of a 4D vector with the corresponding components of another
    @param[out] dst   Pointer to the destination array
    @param[in]  src1   Pointer to the first source array
    @param[in]  src2   Pointer to the second source array
    @param[in]  count The number of items in the input arrays
 */
extern ne10_result_t (*vmla_vec4f)(ne10_vec4f_t * dst, ne10_vec4f_t * acc, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count);



// ## Vector-Vector Algebra ##

/*!
    Vector addition of two 2D vectors
    @param[out] dst   Pointer to the destination array
    @param[in]  src1   Pointer to the first source array
    @param[in]  src2   Pointer to the second source array
    @param[in]  count The number of items in the input arrays
 */
extern ne10_result_t (*add_vec2f)(ne10_vec2f_t * dst, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count);
/*!
    Vector addition of two 3D vectors
    @param[out] dst   Pointer to the destination array
    @param[in]  src1   Pointer to the first source array
    @param[in]  src2   Pointer to the second source array
    @param[in]  count The number of items in the input arrays
 */
extern ne10_result_t (*add_vec3f)(ne10_vec3f_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);
/*!
    Vector addition of two 4D vectors
    @param[out] dst   Pointer to the destination array
    @param[in]  src1   Pointer to the first source array
    @param[in]  src2   Pointer to the second source array
    @param[in]  count The number of items in the input arrays
 */
extern ne10_result_t (*add_vec4f)(ne10_vec4f_t * dst, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count);



/*!
    Vector subtraction of two 2D vectors
    @param[out] dst   Pointer to the destination array
    @param[in]  src1   Pointer to the first source array
    @param[in]  src2   Pointer to the second source array
    @param[in]  count The number of items in the input arrays
 */
extern ne10_result_t (*sub_vec2f)(ne10_vec2f_t * dst, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count);
/*!
    Vector subtraction of two 3D vectors
    @param[out] dst   Pointer to the destination array
    @param[in]  src1   Pointer to the first source array
    @param[in]  src2   Pointer to the second source array
    @param[in]  count The number of items in the input arrays
 */
extern ne10_result_t (*sub_vec3f)(ne10_vec3f_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);
/*!
    Vector subtraction of two 4D vectors
    @param[out] dst   Pointer to the destination array
    @param[in]  src1   Pointer to the first source array
    @param[in]  src2   Pointer to the second source array
    @param[in]  count The number of items in the input arrays
 */
extern ne10_result_t (*sub_vec4f)(ne10_vec4f_t * dst, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count);



/*!
    Dot product of two 2D vectors
    @param[out] dst   Pointer to the destination array
    @param[in]  src1   Pointer to the first source array
    @param[in]  src2   Pointer to the second source array
    @param[in]  count The number of items in the input arrays
 */
extern ne10_result_t (*dot_vec2f)(ne10_float32_t * dst, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count);
/*!
    Dot product of two 3D vectors
    @param[out] dst   Pointer to the destination array
    @param[in]  src1   Pointer to the first source array
    @param[in]  src2   Pointer to the second source array
    @param[in]  count The number of items in the input arrays
 */
extern ne10_result_t (*dot_vec3f)(ne10_float32_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);
/*!
    Dot product of two 4D vectors
    @param[out] dst   Pointer to the destination array
    @param[in]  src1   Pointer to the first source array
    @param[in]  src2   Pointer to the second source array
    @param[in]  count The number of items in the input arrays
 */
extern ne10_result_t (*dot_vec4f)(ne10_float32_t * dst, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count);



/*!
    Performs a cross product operation on the two input vectors
    @param[out] dst   Pointer to the destination array
    @param[in]  src1   Pointer to the first source array
    @param[in]  src2   Pointer to the second source array
    @param[in]  count The number of items in the input arrays
 */
extern ne10_result_t (*cross_vec3f)(ne10_vec3f_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);




// ## Matrix-Constant Arithmetic ##

// ne10_mat4x4f_t
extern ne10_result_t (*addmat_4x4f)(ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src1, ne10_mat4x4f_t * src2, ne10_uint32_t count);
extern ne10_result_t (*submat_4x4f)(ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src1, ne10_mat4x4f_t * src2, ne10_uint32_t count);
extern ne10_result_t (*mulmat_4x4f)(ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src1, ne10_mat4x4f_t * src2, ne10_uint32_t count);
extern ne10_result_t (*divmat_4x4f)(ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src1, ne10_mat4x4f_t * src2, ne10_uint32_t count);
extern ne10_result_t (*setmat_4x4f)(ne10_mat4x4f_t * dst, const ne10_float32_t cst, ne10_uint32_t count);

extern ne10_result_t (*addmat_3x3f)(ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src1, ne10_mat3x3f_t * src2, ne10_uint32_t count);
extern ne10_result_t (*submat_3x3f)(ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src1, ne10_mat3x3f_t * src2, ne10_uint32_t count);
extern ne10_result_t (*mulmat_3x3f)(ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src1, ne10_mat3x3f_t * src2, ne10_uint32_t count);
extern ne10_result_t (*divmat_3x3f)(ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src1, ne10_mat3x3f_t * src2, ne10_uint32_t count);
extern ne10_result_t (*setmat_3x3f)(ne10_mat3x3f_t * dst, const ne10_float32_t cst, ne10_uint32_t count);

extern ne10_result_t (*addmat_2x2f)(ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src1, ne10_mat2x2f_t * src2, ne10_uint32_t count);
extern ne10_result_t (*submat_2x2f)(ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src1, ne10_mat2x2f_t * src2, ne10_uint32_t count);
extern ne10_result_t (*mulmat_2x2f)(ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src1, ne10_mat2x2f_t * src2, ne10_uint32_t count);
extern ne10_result_t (*divmat_2x2f)(ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src1, ne10_mat2x2f_t * src2, ne10_uint32_t count);
extern ne10_result_t (*setmat_2x2f)(ne10_mat2x2f_t * dst, const ne10_float32_t cst, ne10_uint32_t count);



// ## Operations on Matrices ##

extern ne10_result_t (*detmat_4x4f)(ne10_float32_t * dst, ne10_mat4x4f_t * src, ne10_uint32_t count);
extern ne10_result_t (*detmat_3x3f)(ne10_float32_t * dst, ne10_mat3x3f_t * src, ne10_uint32_t count);
extern ne10_result_t (*detmat_2x2f)(ne10_float32_t * dst, ne10_mat2x2f_t * src, ne10_uint32_t count);

extern ne10_result_t (*invmat_4x4f)(ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src, ne10_uint32_t count);
extern ne10_result_t (*invmat_3x3f)(ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src, ne10_uint32_t count);
extern ne10_result_t (*invmat_2x2f)(ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src, ne10_uint32_t count);

extern ne10_result_t (*transmat_4x4f)(ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src, ne10_uint32_t count);
extern ne10_result_t (*identitymat_4x4f)(ne10_mat4x4f_t * dst, ne10_uint32_t count);

extern ne10_result_t (*transmat_3x3f)(ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src, ne10_uint32_t count);
extern ne10_result_t (*identitymat_3x3f)(ne10_mat3x3f_t * dst, ne10_uint32_t count);

extern ne10_result_t (*transmat_2x2f)(ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src, ne10_uint32_t count);
extern ne10_result_t (*identitymat_2x2f)(ne10_mat2x2f_t * dst, ne10_uint32_t count);



// ## Matrix-Vector Algebra ##
extern ne10_result_t (*mulcmatvec_cm4x4f_v4f)(ne10_vec4f_t * dst, const ne10_mat4x4f_t * cst, ne10_vec4f_t * src, ne10_uint32_t count);
extern ne10_result_t (*mulcmatvec_cm3x3f_v3f)(ne10_vec3f_t * dst, const ne10_mat3x3f_t * cst, ne10_vec3f_t * src, ne10_uint32_t count);
extern ne10_result_t (*mulcmatvec_cm2x2f_v2f)(ne10_vec2f_t * dst, const ne10_mat2x2f_t * cst, ne10_vec2f_t * src, ne10_uint32_t count);


// ## Matrix-Matrix Algebra ##
extern ne10_result_t (*multrans_mat4x4f)(ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src1, ne10_mat4x4f_t * src2, ne10_uint32_t count);
extern ne10_result_t (*multrans_mat3x3f)(ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src1, ne10_mat3x3f_t * src2, ne10_uint32_t count);
extern ne10_result_t (*multrans_mat2x2f)(ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src1, ne10_mat2x2f_t * src2, ne10_uint32_t count);


///////////////////////////
// C function prototypes:
///////////////////////////


// ## Vector-Constant Arithmetic ##

extern ne10_result_t addc_float_c(ne10_float32_t * dst, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count);
extern ne10_result_t addc_vec2f_c(ne10_vec2f_t * dst, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count);
extern ne10_result_t addc_vec3f_c(ne10_vec3f_t * dst, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count);
extern ne10_result_t addc_vec4f_c(ne10_vec4f_t * dst, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count);



extern ne10_result_t subc_float_c(ne10_float32_t * dst, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count); // subtract cst from the element(s)
extern ne10_result_t subc_vec2f_c(ne10_vec2f_t * dst, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count); // subtract cst from the element(s)
extern ne10_result_t subc_vec3f_c(ne10_vec3f_t * dst, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count); // subtract cst from the element(s)
extern ne10_result_t subc_vec4f_c(ne10_vec4f_t * dst, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count); // subtract cst from the element(s)



extern ne10_result_t rsbc_float_c(ne10_float32_t * dst, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count); // subtract element(s) from a cst
extern ne10_result_t rsbc_vec2f_c(ne10_vec2f_t * dst, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count); // subtract element(s) from a cst
extern ne10_result_t rsbc_vec3f_c(ne10_vec3f_t * dst, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count); // subtract element(s) from a cst
extern ne10_result_t rsbc_vec4f_c(ne10_vec4f_t * dst, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count); // subtract element(s) from a cst



extern ne10_result_t mulc_float_c(ne10_float32_t * dst, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count);
extern ne10_result_t mulc_vec2f_c(ne10_vec2f_t * dst, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count);
extern ne10_result_t mulc_vec3f_c(ne10_vec3f_t * dst, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count);
extern ne10_result_t mulc_vec4f_c(ne10_vec4f_t * dst, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count);



extern ne10_result_t divc_float_c(ne10_float32_t * dst, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count);
extern ne10_result_t divc_vec2f_c(ne10_vec2f_t * dst, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count);
extern ne10_result_t divc_vec3f_c(ne10_vec3f_t * dst, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count);
extern ne10_result_t divc_vec4f_c(ne10_vec4f_t * dst, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count);



extern ne10_result_t setc_float_c(ne10_float32_t * dst, const ne10_float32_t cst, ne10_uint32_t count);
extern ne10_result_t setc_vec2f_c(ne10_vec2f_t * dst, const ne10_vec2f_t * cst, ne10_uint32_t count);
extern ne10_result_t setc_vec3f_c(ne10_vec3f_t * dst, const ne10_vec3f_t * cst, ne10_uint32_t count);
extern ne10_result_t setc_vec4f_c(ne10_vec4f_t * dst, const ne10_vec4f_t * cst, ne10_uint32_t count);



extern ne10_result_t mlac_float_c(ne10_float32_t * dst, ne10_float32_t * acc, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count);
extern ne10_result_t mlac_vec2f_c(ne10_vec2f_t * dst, ne10_vec2f_t * acc, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count);
extern ne10_result_t mlac_vec3f_c(ne10_vec3f_t * dst, ne10_vec3f_t * acc, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count);
extern ne10_result_t mlac_vec4f_c(ne10_vec4f_t * dst, ne10_vec4f_t * acc, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count);

// ## Arithmetic functions over arrays of cst values ##
extern ne10_result_t add_float_c(ne10_float32_t * dst, ne10_float32_t * src1, ne10_float32_t * src2, ne10_uint32_t count);
extern ne10_result_t sub_float_c(ne10_float32_t * dst, ne10_float32_t * src1, ne10_float32_t * src2, ne10_uint32_t count);
extern ne10_result_t mul_float_c(ne10_float32_t * dst, ne10_float32_t * src1, ne10_float32_t * src2, ne10_uint32_t count);
extern ne10_result_t div_float_c(ne10_float32_t * dst, ne10_float32_t * src1, ne10_float32_t * src2, ne10_uint32_t count);
extern ne10_result_t mla_float_c(ne10_float32_t * dst, ne10_float32_t * acc, ne10_float32_t * src1, ne10_float32_t * src2, ne10_uint32_t count);
extern ne10_result_t abs_float_c(ne10_float32_t * dst, ne10_float32_t * src, ne10_uint32_t count);

// ## Operations on Vectors ##
extern ne10_result_t len_vec2f_c(ne10_float32_t * dst, ne10_vec2f_t * src, ne10_uint32_t count);
extern ne10_result_t len_vec3f_c(ne10_float32_t * dst, ne10_vec3f_t * src, ne10_uint32_t count);
extern ne10_result_t len_vec4f_c(ne10_float32_t * dst, ne10_vec4f_t * src, ne10_uint32_t count);



extern ne10_result_t normalize_vec2f_c(ne10_vec2f_t * dst, ne10_vec2f_t * src, ne10_uint32_t count);
extern ne10_result_t normalize_vec3f_c(ne10_vec3f_t * dst, ne10_vec3f_t * src, ne10_uint32_t count);
extern ne10_result_t normalize_vec4f_c(ne10_vec4f_t * dst, ne10_vec4f_t * src, ne10_uint32_t count);



extern ne10_result_t abs_vec2f_c(ne10_vec2f_t * dst, ne10_vec2f_t * src, ne10_uint32_t count);
extern ne10_result_t abs_vec3f_c(ne10_vec3f_t * dst, ne10_vec3f_t * src, ne10_uint32_t count);
extern ne10_result_t abs_vec4f_c(ne10_vec4f_t * dst, ne10_vec4f_t * src, ne10_uint32_t count);



// ## SIMD Component-wise Arithmetic on Two Vectors ##
extern ne10_result_t vmul_vec2f_c(ne10_vec2f_t * dst, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count);
extern ne10_result_t vmul_vec3f_c(ne10_vec3f_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);
extern ne10_result_t vmul_vec4f_c(ne10_vec4f_t * dst, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count);



extern ne10_result_t vdiv_vec2f_c(ne10_vec2f_t * dst, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count);
extern ne10_result_t vdiv_vec3f_c(ne10_vec3f_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);
extern ne10_result_t vdiv_vec4f_c(ne10_vec4f_t * dst, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count);



extern ne10_result_t vmla_vec2f_c(ne10_vec2f_t * dst, ne10_vec2f_t * acc, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count);
extern ne10_result_t vmla_vec3f_c(ne10_vec3f_t * dst, ne10_vec3f_t * acc, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);
extern ne10_result_t vmla_vec4f_c(ne10_vec4f_t * dst, ne10_vec4f_t * acc, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count);



// ## Vector-Vector Algebra ##
extern ne10_result_t add_vec2f_c(ne10_vec2f_t * dst, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count);
extern ne10_result_t add_vec3f_c(ne10_vec3f_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);
extern ne10_result_t add_vec4f_c(ne10_vec4f_t * dst, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count);



extern ne10_result_t sub_vec2f_c(ne10_vec2f_t * dst, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count);
extern ne10_result_t sub_vec3f_c(ne10_vec3f_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);
extern ne10_result_t sub_vec4f_c(ne10_vec4f_t * dst, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count);



extern ne10_result_t dot_vec2f_c(ne10_float32_t * dst, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count);
extern ne10_result_t dot_vec3f_c(ne10_float32_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);
extern ne10_result_t dot_vec4f_c(ne10_float32_t * dst, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count);



extern ne10_result_t cross_vec3f_c(ne10_vec3f_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);



// ## Matrix-Constant Arithmetic ##

// ne10_mat4x4f_t
extern ne10_result_t addmat_4x4f_c(ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src1, ne10_mat4x4f_t * src2, ne10_uint32_t count);
extern ne10_result_t submat_4x4f_c(ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src1, ne10_mat4x4f_t * src2, ne10_uint32_t count);
extern ne10_result_t mulmat_4x4f_c(ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src1, ne10_mat4x4f_t * src2, ne10_uint32_t count);
extern ne10_result_t divmat_4x4f_c(ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src1, ne10_mat4x4f_t * src2, ne10_uint32_t count);
extern ne10_result_t setmat_4x4f_c(ne10_mat4x4f_t * dst, const ne10_float32_t cst, ne10_uint32_t count);

extern ne10_result_t addmat_3x3f_c(ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src1, ne10_mat3x3f_t * src2, ne10_uint32_t count);
extern ne10_result_t submat_3x3f_c(ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src1, ne10_mat3x3f_t * src2, ne10_uint32_t count);
extern ne10_result_t mulmat_3x3f_c(ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src1, ne10_mat3x3f_t * src2, ne10_uint32_t count);
extern ne10_result_t divmat_3x3f_c(ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src1, ne10_mat3x3f_t * src2, ne10_uint32_t count);
extern ne10_result_t setmat_3x3f_c(ne10_mat3x3f_t * dst, const ne10_float32_t cst, ne10_uint32_t count);

extern ne10_result_t addmat_2x2f_c(ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src1, ne10_mat2x2f_t * src2, ne10_uint32_t count);
extern ne10_result_t submat_2x2f_c(ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src1, ne10_mat2x2f_t * src2, ne10_uint32_t count);
extern ne10_result_t mulmat_2x2f_c(ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src1, ne10_mat2x2f_t * src2, ne10_uint32_t count);
extern ne10_result_t divmat_2x2f_c(ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src1, ne10_mat2x2f_t * src2, ne10_uint32_t count);
extern ne10_result_t setmat_2x2f_c(ne10_mat2x2f_t * dst, const ne10_float32_t cst, ne10_uint32_t count);



// ## Operations on Matrices ##

extern ne10_result_t detmat_4x4f_c(ne10_float32_t * dst, ne10_mat4x4f_t * src, ne10_uint32_t count);
extern ne10_result_t detmat_3x3f_c(ne10_float32_t * dst, ne10_mat3x3f_t * src, ne10_uint32_t count);
extern ne10_result_t detmat_2x2f_c(ne10_float32_t * dst, ne10_mat2x2f_t * src, ne10_uint32_t count);

extern ne10_result_t invmat_4x4f_c(ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src, ne10_uint32_t count);
extern ne10_result_t invmat_3x3f_c(ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src, ne10_uint32_t count);
extern ne10_result_t invmat_2x2f_c(ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src, ne10_uint32_t count);

extern ne10_result_t transmat_4x4f_c(ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src, ne10_uint32_t count);
extern ne10_result_t identitymat_4x4f_c(ne10_mat4x4f_t * dst, ne10_uint32_t count);

extern ne10_result_t transmat_3x3f_c(ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src, ne10_uint32_t count);
extern ne10_result_t identitymat_3x3f_c(ne10_mat3x3f_t * dst, ne10_uint32_t count);

extern ne10_result_t transmat_2x2f_c(ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src, ne10_uint32_t count);
extern ne10_result_t identitymat_2x2f_c(ne10_mat2x2f_t * dst, ne10_uint32_t count);



// ## Matrix-Vector Algebra ##
extern ne10_result_t mulcmatvec_cm4x4f_v4f_c(ne10_vec4f_t * dst, const ne10_mat4x4f_t * cst, ne10_vec4f_t * src, ne10_uint32_t count);
extern ne10_result_t mulcmatvec_cm3x3f_v3f_c(ne10_vec3f_t * dst, const ne10_mat3x3f_t * cst, ne10_vec3f_t * src, ne10_uint32_t count);
extern ne10_result_t mulcmatvec_cm2x2f_v2f_c(ne10_vec2f_t * dst, const ne10_mat2x2f_t * cst, ne10_vec2f_t * src, ne10_uint32_t count);


// ## Matrix-Matrix Algebra ##
extern ne10_result_t multrans_mat4x4f_c(ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src1, ne10_mat4x4f_t * src2, ne10_uint32_t count);
extern ne10_result_t multrans_mat3x3f_c(ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src1, ne10_mat3x3f_t * src2, ne10_uint32_t count);
extern ne10_result_t multrans_mat2x2f_c(ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src1, ne10_mat2x2f_t * src2, ne10_uint32_t count);


/////////////////////////////
// NEON function prototypes:
/////////////////////////////


// ## Vector-Constant Arithmetic ##

extern ne10_result_t addc_float_neon(ne10_float32_t * dst, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count);
extern ne10_result_t addc_vec2f_neon(ne10_vec2f_t * dst, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count);
extern ne10_result_t addc_vec3f_neon(ne10_vec3f_t * dst, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count);
extern ne10_result_t addc_vec4f_neon(ne10_vec4f_t * dst, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count);



extern ne10_result_t subc_float_neon(ne10_float32_t * dst, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count); // subtract cst from the element(s)
extern ne10_result_t subc_vec2f_neon(ne10_vec2f_t * dst, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count); // subtract cst from the element(s)
extern ne10_result_t subc_vec3f_neon(ne10_vec3f_t * dst, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count); // subtract cst from the element(s)
extern ne10_result_t subc_vec4f_neon(ne10_vec4f_t * dst, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count); // subtract cst from the element(s)



extern ne10_result_t rsbc_float_neon(ne10_float32_t * dst, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count); // subtract element(s) from a cst
extern ne10_result_t rsbc_vec2f_neon(ne10_vec2f_t * dst, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count); // subtract element(s) from a cst
extern ne10_result_t rsbc_vec3f_neon(ne10_vec3f_t * dst, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count); // subtract element(s) from a cst
extern ne10_result_t rsbc_vec4f_neon(ne10_vec4f_t * dst, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count); // subtract element(s) from a cst



extern ne10_result_t mulc_float_neon(ne10_float32_t * dst, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count);
extern ne10_result_t mulc_vec2f_neon(ne10_vec2f_t * dst, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count);
extern ne10_result_t mulc_vec3f_neon(ne10_vec3f_t * dst, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count);
extern ne10_result_t mulc_vec4f_neon(ne10_vec4f_t * dst, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count);



extern ne10_result_t divc_float_neon(ne10_float32_t * dst, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count);
extern ne10_result_t divc_vec2f_neon(ne10_vec2f_t * dst, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count);
extern ne10_result_t divc_vec3f_neon(ne10_vec3f_t * dst, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count);
extern ne10_result_t divc_vec4f_neon(ne10_vec4f_t * dst, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count);



extern ne10_result_t setc_float_neon(ne10_float32_t * dst, const ne10_float32_t cst, ne10_uint32_t count);
extern ne10_result_t setc_vec2f_neon(ne10_vec2f_t * dst, const ne10_vec2f_t * cst, ne10_uint32_t count);
extern ne10_result_t setc_vec3f_neon(ne10_vec3f_t * dst, const ne10_vec3f_t * cst, ne10_uint32_t count);
extern ne10_result_t setc_vec4f_neon(ne10_vec4f_t * dst, const ne10_vec4f_t * cst, ne10_uint32_t count);



extern ne10_result_t mlac_float_neon(ne10_float32_t * dst, ne10_float32_t * acc, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count);
extern ne10_result_t mlac_vec2f_neon(ne10_vec2f_t * dst, ne10_vec2f_t * acc, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count);
extern ne10_result_t mlac_vec3f_neon(ne10_vec3f_t * dst, ne10_vec3f_t * acc, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count);
extern ne10_result_t mlac_vec4f_neon(ne10_vec4f_t * dst, ne10_vec4f_t * acc, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count);



// ## Arithmetic functions over arrays of cst values ##
extern ne10_result_t add_float_neon(ne10_float32_t * dst, ne10_float32_t * src1, ne10_float32_t * src2, ne10_uint32_t count);
extern ne10_result_t sub_float_neon(ne10_float32_t * dst, ne10_float32_t * src1, ne10_float32_t * src2, ne10_uint32_t count);
extern ne10_result_t mul_float_neon(ne10_float32_t * dst, ne10_float32_t * src1, ne10_float32_t * src2, ne10_uint32_t count);
extern ne10_result_t div_float_neon(ne10_float32_t * dst, ne10_float32_t * src1, ne10_float32_t * src2, ne10_uint32_t count);
extern ne10_result_t mla_float_neon(ne10_float32_t * dst, ne10_float32_t * acc, ne10_float32_t * src1, ne10_float32_t * src2, ne10_uint32_t count);
extern ne10_result_t abs_float_neon(ne10_float32_t * dst, ne10_float32_t * src, ne10_uint32_t count);

// ## Operations on Vectors ##
extern ne10_result_t len_vec2f_neon(ne10_float32_t * dst, ne10_vec2f_t * src, ne10_uint32_t count);
extern ne10_result_t len_vec3f_neon(ne10_float32_t * dst, ne10_vec3f_t * src, ne10_uint32_t count);
extern ne10_result_t len_vec4f_neon(ne10_float32_t * dst, ne10_vec4f_t * src, ne10_uint32_t count);



extern ne10_result_t normalize_vec2f_neon(ne10_vec2f_t * dst, ne10_vec2f_t * src, ne10_uint32_t count);
extern ne10_result_t normalize_vec3f_neon(ne10_vec3f_t * dst, ne10_vec3f_t * src, ne10_uint32_t count);
extern ne10_result_t normalize_vec4f_neon(ne10_vec4f_t * dst, ne10_vec4f_t * src, ne10_uint32_t count);



extern ne10_result_t abs_vec2f_neon(ne10_vec2f_t * dst, ne10_vec2f_t * src, ne10_uint32_t count);
extern ne10_result_t abs_vec3f_neon(ne10_vec3f_t * dst, ne10_vec3f_t * src, ne10_uint32_t count);
extern ne10_result_t abs_vec4f_neon(ne10_vec4f_t * dst, ne10_vec4f_t * src, ne10_uint32_t count);



// ## SIMD Component-wise Arithmetic on Two Vectors ##
extern ne10_result_t vmul_vec2f_neon(ne10_vec2f_t * dst, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count);
extern ne10_result_t vmul_vec3f_neon(ne10_vec3f_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);
extern ne10_result_t vmul_vec4f_neon(ne10_vec4f_t * dst, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count);



extern ne10_result_t vdiv_vec2f_neon(ne10_vec2f_t * dst, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count);
extern ne10_result_t vdiv_vec3f_neon(ne10_vec3f_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);
extern ne10_result_t vdiv_vec4f_neon(ne10_vec4f_t * dst, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count);



extern ne10_result_t vmla_vec2f_neon(ne10_vec2f_t * dst, ne10_vec2f_t * acc, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count);
extern ne10_result_t vmla_vec3f_neon(ne10_vec3f_t * dst, ne10_vec3f_t * acc, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);
extern ne10_result_t vmla_vec4f_neon(ne10_vec4f_t * dst, ne10_vec4f_t * acc, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count);



// ## Vector-Vector Algebra ##
extern ne10_result_t add_vec2f_neon(ne10_vec2f_t * dst, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count);
extern ne10_result_t add_vec3f_neon(ne10_vec3f_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);
extern ne10_result_t add_vec4f_neon(ne10_vec4f_t * dst, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count);



extern ne10_result_t sub_vec2f_neon(ne10_vec2f_t * dst, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count);
extern ne10_result_t sub_vec3f_neon(ne10_vec3f_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);
extern ne10_result_t sub_vec4f_neon(ne10_vec4f_t * dst, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count);



extern ne10_result_t dot_vec2f_neon(ne10_float32_t * dst, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count);
extern ne10_result_t dot_vec3f_neon(ne10_float32_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);
extern ne10_result_t dot_vec4f_neon(ne10_float32_t * dst, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count);



extern ne10_result_t cross_vec3f_neon(ne10_vec3f_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);



// ## Matrix-Constant Arithmetic ##

// ne10_mat4x4f_t
extern ne10_result_t addmat_4x4f_neon(ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src1, ne10_mat4x4f_t * src2, ne10_uint32_t count);
extern ne10_result_t submat_4x4f_neon(ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src1, ne10_mat4x4f_t * src2, ne10_uint32_t count);
extern ne10_result_t mulmat_4x4f_neon(ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src1, ne10_mat4x4f_t * src2, ne10_uint32_t count);
extern ne10_result_t divmat_4x4f_neon(ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src1, ne10_mat4x4f_t * src2, ne10_uint32_t count);
extern ne10_result_t setmat_4x4f_neon(ne10_mat4x4f_t * dst, const ne10_float32_t cst, ne10_uint32_t count);

extern ne10_result_t addmat_3x3f_neon(ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src1, ne10_mat3x3f_t * src2, ne10_uint32_t count);
extern ne10_result_t submat_3x3f_neon(ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src1, ne10_mat3x3f_t * src2, ne10_uint32_t count);
extern ne10_result_t mulmat_3x3f_neon(ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src1, ne10_mat3x3f_t * src2, ne10_uint32_t count);
extern ne10_result_t divmat_3x3f_neon(ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src1, ne10_mat3x3f_t * src2, ne10_uint32_t count);
extern ne10_result_t setmat_3x3f_neon(ne10_mat3x3f_t * dst, const ne10_float32_t cst, ne10_uint32_t count);

extern ne10_result_t addmat_2x2f_neon(ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src1, ne10_mat2x2f_t * src2, ne10_uint32_t count);
extern ne10_result_t submat_2x2f_neon(ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src1, ne10_mat2x2f_t * src2, ne10_uint32_t count);
extern ne10_result_t mulmat_2x2f_neon(ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src1, ne10_mat2x2f_t * src2, ne10_uint32_t count);
extern ne10_result_t divmat_2x2f_neon(ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src1, ne10_mat2x2f_t * src2, ne10_uint32_t count);
extern ne10_result_t setmat_2x2f_neon(ne10_mat2x2f_t * dst, const ne10_float32_t cst, ne10_uint32_t count);



// ## Operations on Matrices ##


extern ne10_result_t detmat_4x4f_neon(ne10_float32_t * dst, ne10_mat4x4f_t * src, ne10_uint32_t count);
extern ne10_result_t detmat_3x3f_neon(ne10_float32_t * dst, ne10_mat3x3f_t * src, ne10_uint32_t count);
extern ne10_result_t detmat_2x2f_neon(ne10_float32_t * dst, ne10_mat2x2f_t * src, ne10_uint32_t count);

extern ne10_result_t invmat_4x4f_neon(ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src, ne10_uint32_t count);
extern ne10_result_t invmat_3x3f_neon(ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src, ne10_uint32_t count);
extern ne10_result_t invmat_2x2f_neon(ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src, ne10_uint32_t count);

extern ne10_result_t transmat_4x4f_neon(ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src, ne10_uint32_t count);
extern ne10_result_t identitymat_4x4f_neon(ne10_mat4x4f_t * dst, ne10_uint32_t count);

extern ne10_result_t transmat_3x3f_neon(ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src, ne10_uint32_t count);
extern ne10_result_t identitymat_3x3f_neon(ne10_mat3x3f_t * dst, ne10_uint32_t count);

extern ne10_result_t transmat_2x2f_neon(ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src, ne10_uint32_t count);
extern ne10_result_t identitymat_2x2f_neon(ne10_mat2x2f_t * dst, ne10_uint32_t count);



// ## Matrix-Vector Algebra ##
extern ne10_result_t mulcmatvec_cm4x4f_v4f_neon(ne10_vec4f_t * dst, const ne10_mat4x4f_t * cst, ne10_vec4f_t * src, ne10_uint32_t count);
extern ne10_result_t mulcmatvec_cm3x3f_v3f_neon(ne10_vec3f_t * dst, const ne10_mat3x3f_t * cst, ne10_vec3f_t * src, ne10_uint32_t count);
extern ne10_result_t mulcmatvec_cm2x2f_v2f_neon(ne10_vec2f_t * dst, const ne10_mat2x2f_t * cst, ne10_vec2f_t * src, ne10_uint32_t count);




// ## Matrix-Matrix Algebra ##
extern ne10_result_t multrans_mat4x4f_neon(ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src1, ne10_mat4x4f_t * src2, ne10_uint32_t count);
extern ne10_result_t multrans_mat3x3f_neon(ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src1, ne10_mat3x3f_t * src2, ne10_uint32_t count);
extern ne10_result_t multrans_mat2x2f_neon(ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src1, ne10_mat2x2f_t * src2, ne10_uint32_t count);


////////////////////////////
// VFP function prototypes:
////////////////////////////

// ## Vector-Constant Arithmetic ##

extern ne10_result_t addc_float_asm(ne10_float32_t * dst, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count);
extern ne10_result_t addc_vec2f_asm(ne10_vec2f_t * dst, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count);
extern ne10_result_t addc_vec3f_asm(ne10_vec3f_t * dst, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count);
extern ne10_result_t addc_vec4f_asm(ne10_vec4f_t * dst, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count);



extern ne10_result_t subc_float_asm(ne10_float32_t * dst, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count); // subtract cst from the element(s)
extern ne10_result_t subc_vec2f_asm(ne10_vec2f_t * dst, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count); // subtract cst from the element(s)
extern ne10_result_t subc_vec3f_asm(ne10_vec3f_t * dst, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count); // subtract cst from the element(s)
extern ne10_result_t subc_vec4f_asm(ne10_vec4f_t * dst, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count); // subtract cst from the element(s)



extern ne10_result_t rsbc_float_asm(ne10_float32_t * dst, ne10_float32_t *src, const ne10_float32_t cst, ne10_uint32_t count); // subtract element(s) from a cst
extern ne10_result_t rsbc_vec2f_asm(ne10_vec2f_t * dst, ne10_vec2f_t *src, const ne10_vec2f_t * cst, ne10_uint32_t count); // subtract element(s) from a cst
extern ne10_result_t rsbc_vec3f_asm(ne10_vec3f_t * dst, ne10_vec3f_t *src, const ne10_vec3f_t * cst, ne10_uint32_t count); // subtract element(s) from a cst
extern ne10_result_t rsbc_vec4f_asm(ne10_vec4f_t * dst, ne10_vec4f_t *src, const ne10_vec4f_t * cst, ne10_uint32_t count); // subtract element(s) from a cst



extern ne10_result_t mulc_float_asm(ne10_float32_t * dst, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count);
extern ne10_result_t mulc_vec2f_asm(ne10_vec2f_t * dst, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count);
extern ne10_result_t mulc_vec3f_asm(ne10_vec3f_t * dst, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count);
extern ne10_result_t mulc_vec4f_asm(ne10_vec4f_t * dst, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count);



extern ne10_result_t divc_float_asm(ne10_float32_t * dst, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count);
extern ne10_result_t divc_vec2f_asm(ne10_vec2f_t * dst, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count);
extern ne10_result_t divc_vec3f_asm(ne10_vec3f_t * dst, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count);
extern ne10_result_t divc_vec4f_asm(ne10_vec4f_t * dst, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count);



extern ne10_result_t setc_float_asm(ne10_float32_t * dst, const ne10_float32_t cst, ne10_uint32_t count);
extern ne10_result_t setc_vec2f_asm(ne10_vec2f_t * dst, const ne10_vec2f_t * cst, ne10_uint32_t count);
extern ne10_result_t setc_vec3f_asm(ne10_vec3f_t * dst, const ne10_vec3f_t * cst, ne10_uint32_t count);
extern ne10_result_t setc_vec4f_asm(ne10_vec4f_t * dst, const ne10_vec4f_t * cst, ne10_uint32_t count);



extern ne10_result_t mlac_float_asm(ne10_float32_t * dst, ne10_float32_t * acc, ne10_float32_t * src, const ne10_float32_t cst, ne10_uint32_t count);
extern ne10_result_t mlac_vec2f_asm(ne10_vec2f_t * dst, ne10_vec2f_t * acc, ne10_vec2f_t * src, const ne10_vec2f_t * cst, ne10_uint32_t count);
extern ne10_result_t mlac_vec3f_asm(ne10_vec3f_t * dst, ne10_vec3f_t * acc, ne10_vec3f_t * src, const ne10_vec3f_t * cst, ne10_uint32_t count);
extern ne10_result_t mlac_vec4f_asm(ne10_vec4f_t * dst, ne10_vec4f_t * acc, ne10_vec4f_t * src, const ne10_vec4f_t * cst, ne10_uint32_t count);



// ## Arithmetic functions over arrays of cst values ##
extern ne10_result_t add_float_asm(ne10_float32_t * dst, ne10_float32_t * src1, ne10_float32_t * src2, ne10_uint32_t count);
extern ne10_result_t sub_float_asm(ne10_float32_t * dst, ne10_float32_t * src1, ne10_float32_t * src2, ne10_uint32_t count);
extern ne10_result_t mul_float_asm(ne10_float32_t * dst, ne10_float32_t * src1, ne10_float32_t * src2, ne10_uint32_t count);
extern ne10_result_t div_float_asm(ne10_float32_t * dst, ne10_float32_t * src1, ne10_float32_t * src2, ne10_uint32_t count);
extern ne10_result_t mla_float_asm(ne10_float32_t * dst, ne10_float32_t * acc, ne10_float32_t * src1, ne10_float32_t * src2, ne10_uint32_t count);
extern ne10_result_t abs_float_asm(ne10_float32_t * dst, ne10_float32_t * src, ne10_uint32_t count);

// ## Operations on Vectors ##
extern ne10_result_t len_vec2f_asm(ne10_float32_t * dst, ne10_vec2f_t * src, ne10_uint32_t count);
extern ne10_result_t len_vec3f_asm(ne10_float32_t * dst, ne10_vec3f_t * src, ne10_uint32_t count);
extern ne10_result_t len_vec4f_asm(ne10_float32_t * dst, ne10_vec4f_t * src, ne10_uint32_t count);



extern ne10_result_t normalize_vec2f_asm(ne10_vec2f_t * dst, ne10_vec2f_t * src, ne10_uint32_t count);
extern ne10_result_t normalize_vec3f_asm(ne10_vec3f_t * dst, ne10_vec3f_t * src, ne10_uint32_t count);
extern ne10_result_t normalize_vec4f_asm(ne10_vec4f_t * dst, ne10_vec4f_t * src, ne10_uint32_t count);



extern ne10_result_t abs_vec2f_asm(ne10_vec2f_t * dst, ne10_vec2f_t * src, ne10_uint32_t count);
extern ne10_result_t abs_vec3f_asm(ne10_vec3f_t * dst, ne10_vec3f_t * src, ne10_uint32_t count);
extern ne10_result_t abs_vec4f_asm(ne10_vec4f_t * dst, ne10_vec4f_t * src, ne10_uint32_t count);



// ## SIMD Component-wise Arithmetic on Two Vectors ##
extern ne10_result_t vmul_vec2f_asm(ne10_vec2f_t * dst, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count);
extern ne10_result_t vmul_vec3f_asm(ne10_vec3f_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);
extern ne10_result_t vmul_vec4f_asm(ne10_vec4f_t * dst, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count);



extern ne10_result_t vdiv_vec2f_asm(ne10_vec2f_t * dst, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count);
extern ne10_result_t vdiv_vec3f_asm(ne10_vec3f_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);
extern ne10_result_t vdiv_vec4f_asm(ne10_vec4f_t * dst, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count);



extern ne10_result_t vmla_vec2f_asm(ne10_vec2f_t * dst, ne10_vec2f_t * acc, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count);
extern ne10_result_t vmla_vec3f_asm(ne10_vec3f_t * dst, ne10_vec3f_t * acc, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);
extern ne10_result_t vmla_vec4f_asm(ne10_vec4f_t * dst, ne10_vec4f_t * acc, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count);



// ## Vector-Vector Algebra ##
extern ne10_result_t add_vec2f_asm(ne10_vec2f_t * dst, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count);
extern ne10_result_t add_vec3f_asm(ne10_vec3f_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);
extern ne10_result_t add_vec4f_asm(ne10_vec4f_t * dst, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count);



extern ne10_result_t sub_vec2f_asm(ne10_vec2f_t * dst, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count);
extern ne10_result_t sub_vec3f_asm(ne10_vec3f_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);
extern ne10_result_t sub_vec4f_asm(ne10_vec4f_t * dst, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count);



extern ne10_result_t dot_vec2f_asm(ne10_float32_t * dst, ne10_vec2f_t * src1, ne10_vec2f_t * src2, ne10_uint32_t count);
extern ne10_result_t dot_vec3f_asm(ne10_float32_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);
extern ne10_result_t dot_vec4f_asm(ne10_float32_t * dst, ne10_vec4f_t * src1, ne10_vec4f_t * src2, ne10_uint32_t count);



extern ne10_result_t cross_vec3f_asm(ne10_vec3f_t * dst, ne10_vec3f_t * src1, ne10_vec3f_t * src2, ne10_uint32_t count);


// ## Matrix-Constant Arithmetic ##

// ne10_mat4x4f_t
extern ne10_result_t addmat_4x4f_asm(ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src1, ne10_mat4x4f_t * src2, ne10_uint32_t count);
extern ne10_result_t submat_4x4f_asm(ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src1, ne10_mat4x4f_t * src2, ne10_uint32_t count);
extern ne10_result_t mulmat_4x4f_asm(ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src1, ne10_mat4x4f_t * src2, ne10_uint32_t count);
extern ne10_result_t divmat_4x4f_asm(ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src1, ne10_mat4x4f_t * src2, ne10_uint32_t count);
extern ne10_result_t setmat_4x4f_asm(ne10_mat4x4f_t * dst, const ne10_float32_t cst, ne10_uint32_t count);

extern ne10_result_t addmat_3x3f_asm(ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src1, ne10_mat3x3f_t * src2, ne10_uint32_t count);
extern ne10_result_t submat_3x3f_asm(ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src1, ne10_mat3x3f_t * src2, ne10_uint32_t count);
extern ne10_result_t mulmat_3x3f_asm(ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src1, ne10_mat3x3f_t * src2, ne10_uint32_t count);
extern ne10_result_t divmat_3x3f_asm(ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src1, ne10_mat3x3f_t * src2, ne10_uint32_t count);
extern ne10_result_t setmat_3x3f_asm(ne10_mat3x3f_t * dst, const ne10_float32_t cst, ne10_uint32_t count);

extern ne10_result_t addmat_2x2f_asm(ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src1, ne10_mat2x2f_t * src2, ne10_uint32_t count);
extern ne10_result_t submat_2x2f_asm(ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src1, ne10_mat2x2f_t * src2, ne10_uint32_t count);
extern ne10_result_t mulmat_2x2f_asm(ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src1, ne10_mat2x2f_t * src2, ne10_uint32_t count);
extern ne10_result_t divmat_2x2f_asm(ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src1, ne10_mat2x2f_t * src2, ne10_uint32_t count);
extern ne10_result_t setmat_2x2f_asm(ne10_mat2x2f_t * dst, const ne10_float32_t cst, ne10_uint32_t count);



// ## Operations on Matrices ##

extern ne10_result_t detmat_4x4f_asm(ne10_float32_t * dst, ne10_mat4x4f_t * src, ne10_uint32_t count);
extern ne10_result_t detmat_3x3f_asm(ne10_float32_t * dst, ne10_mat3x3f_t * src, ne10_uint32_t count);
extern ne10_result_t detmat_2x2f_asm(ne10_float32_t * dst, ne10_mat2x2f_t * src, ne10_uint32_t count);

extern ne10_result_t invmat_4x4f_asm(ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src, ne10_uint32_t count);
extern ne10_result_t invmat_3x3f_asm(ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src, ne10_uint32_t count);
extern ne10_result_t invmat_2x2f_asm(ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src, ne10_uint32_t count);

extern ne10_result_t transmat_4x4f_asm(ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src, ne10_uint32_t count);
extern ne10_result_t identitymat_4x4f_asm(ne10_mat4x4f_t * dst, ne10_uint32_t count);

extern ne10_result_t transmat_3x3f_asm(ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src, ne10_uint32_t count);
extern ne10_result_t identitymat_3x3f_asm(ne10_mat3x3f_t * dst, ne10_uint32_t count);

extern ne10_result_t trans_mat2x2f_asm(ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src, ne10_uint32_t count);
extern ne10_result_t identity_mat2x2f_asm(ne10_mat2x2f_t * dst, ne10_uint32_t count);



// ## Matrix-Vector Algebra ##
extern ne10_result_t mulcmatvec_cm4x4f_v4f_asm(ne10_vec4f_t * dst, const ne10_mat4x4f_t * cst, ne10_vec4f_t * src, ne10_uint32_t count);
extern ne10_result_t mulcmatvec_cm3x3f_v3f_asm(ne10_vec3f_t * dst, const ne10_mat3x3f_t * cst, ne10_vec3f_t * src, ne10_uint32_t count);
extern ne10_result_t mulcmatvec_cm2x2f_v2f_asm(ne10_vec2f_t * dst, const ne10_mat2x2f_t * cst, ne10_vec2f_t * src, ne10_uint32_t count);




// ## Matrix-Matrix Algebra ##
extern ne10_result_t multrans_mat4x4f_asm(ne10_mat4x4f_t * dst, ne10_mat4x4f_t * src1, ne10_mat4x4f_t * src2, ne10_uint32_t count);
extern ne10_result_t multrans_mat3x3f_asm(ne10_mat3x3f_t * dst, ne10_mat3x3f_t * src1, ne10_mat3x3f_t * src2, ne10_uint32_t count);
extern ne10_result_t multrans_mat2x2f_asm(ne10_mat2x2f_t * dst, ne10_mat2x2f_t * src1, ne10_mat2x2f_t * src2, ne10_uint32_t count);

#ifdef __cplusplus
}
#endif

#endif
