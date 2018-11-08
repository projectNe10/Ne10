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
 * NE10 Library : inc/NE10_types.h
 */

/** NE10 defines a number of types for use in its function signatures.
 *  The types are defined within this header file.
 */

#ifndef NE10_TYPES_H
#define NE10_TYPES_H

#include <stdint.h>
#include <assert.h>

/**
 * @TODO Move the definition of NE10_UNROLL_LEVEL to cmake configuration files.
 * Macro NE10_UNROLL_LEVEL controls algorithm of FFT funtions.
 * When NE10_UNROLL_LEVEL == 0, complex FFT performs radix-4 x2 per loop.
 * When NE10_UNROLL_LEVEL == 1, complex FFT performs radix-4 x4 per loop.
 */
#if !defined(NE10_UNROLL_LEVEL)
#if defined(__arm__)
#define NE10_UNROLL_LEVEL 0
#elif defined(__aarch64__)
#define NE10_UNROLL_LEVEL 1
#else
#define NE10_UNROLL_LEVEL 0
#endif
#endif

/////////////////////////////////////////////////////////
// constant values that are used across the library
/////////////////////////////////////////////////////////
#define NE10_OK 0
#define NE10_ERR -1

/////////////////////////////////////////////////////////
// some external definitions to be exposed to the users
/////////////////////////////////////////////////////////

typedef int8_t   ne10_int8_t;
typedef uint8_t  ne10_uint8_t;
typedef int16_t  ne10_int16_t;
typedef uint16_t ne10_uint16_t;
typedef int32_t  ne10_int32_t;
typedef uint32_t ne10_uint32_t;
typedef int64_t  ne10_int64_t;
typedef uint64_t ne10_uint64_t;
typedef float    ne10_float32_t;
typedef double   ne10_float64_t;
typedef int      ne10_result_t;     // resulting [error-]code

/**
 * @brief A 2-tuple of ne10_float32_t values.
 */
typedef struct
{
    ne10_float32_t x;
    ne10_float32_t y;
} ne10_vec2f_t;

/**
 * @brief A 3-tuple of ne10_float32_t values.
 */
typedef struct
{
    ne10_float32_t x;
    ne10_float32_t y;
    ne10_float32_t z;
} ne10_vec3f_t;

/**
 * @brief A 4-tuple of ne10_float32_t values.
 */
typedef struct
{
    ne10_float32_t x;
    ne10_float32_t y;
    ne10_float32_t z;
    ne10_float32_t w;
} ne10_vec4f_t;

/////////////////////////////////////////////////////////
// definitions for matrix
/////////////////////////////////////////////////////////

typedef struct
{
    ne10_float32_t r1;
    ne10_float32_t r2;
} __attribute__ ( (packed)) ne10_mat_row2f;

typedef struct
{
    ne10_mat_row2f c1;
    ne10_mat_row2f c2;

} __attribute__ ( (packed)) ne10_mat2x2f_t;   // a 2x2 matrix

static inline void createColumnMajorMatrix2x2 (ne10_mat2x2f_t * outMat, ne10_float32_t m11, ne10_float32_t m21, ne10_float32_t m12, ne10_float32_t m22)
{
    assert (NULL != outMat);

    outMat->c1.r1 = m11;
    outMat->c1.r2 = m21;
    outMat->c2.r1 = m12;
    outMat->c2.r2 = m22;
}


typedef struct
{
    ne10_float32_t r1;
    ne10_float32_t r2;
    ne10_float32_t r3;
} __attribute__ ( (packed)) ne10_mat_row3f;

typedef struct
{
    ne10_mat_row3f c1;
    ne10_mat_row3f c2;
    ne10_mat_row3f c3;

} __attribute__ ( (packed)) ne10_mat3x3f_t;   // a 3x3 matrix

static inline void createColumnMajorMatrix3x3 (ne10_mat3x3f_t * outMat, ne10_float32_t m11, ne10_float32_t m21, ne10_float32_t m31,
        ne10_float32_t m12, ne10_float32_t m22, ne10_float32_t m32,
        ne10_float32_t m13, ne10_float32_t m23, ne10_float32_t m33)
{
    assert (NULL != outMat);

    outMat->c1.r1 = m11;
    outMat->c1.r2 = m21;
    outMat->c1.r3 = m31;

    outMat->c2.r1 = m12;
    outMat->c2.r2 = m22;
    outMat->c2.r3 = m32;

    outMat->c3.r1 = m13;
    outMat->c3.r2 = m23;
    outMat->c3.r3 = m33;
}


typedef struct
{
    ne10_float32_t r1;
    ne10_float32_t r2;
    ne10_float32_t r3;
    ne10_float32_t r4;
} __attribute__ ( (packed)) ne10_mat_row4f;

typedef struct
{
    ne10_mat_row4f c1;
    ne10_mat_row4f c2;
    ne10_mat_row4f c3;
    ne10_mat_row4f c4;

} __attribute__ ( (packed)) ne10_mat4x4f_t;   // a 4x4 matrix

static inline void createColumnMajorMatrix4x4 (ne10_mat4x4f_t * outMat, ne10_float32_t m11, ne10_float32_t m21, ne10_float32_t m31, ne10_float32_t m41,
        ne10_float32_t m12, ne10_float32_t m22, ne10_float32_t m32, ne10_float32_t m42,
        ne10_float32_t m13, ne10_float32_t m23, ne10_float32_t m33, ne10_float32_t m43,
        ne10_float32_t m14, ne10_float32_t m24, ne10_float32_t m34, ne10_float32_t m44)
{
    assert (NULL != outMat);

    outMat->c1.r1 = m11;
    outMat->c1.r2 = m21;
    outMat->c1.r3 = m31;
    outMat->c1.r4 = m41;

    outMat->c2.r1 = m12;
    outMat->c2.r2 = m22;
    outMat->c2.r3 = m32;
    outMat->c2.r4 = m42;

    outMat->c3.r1 = m13;
    outMat->c3.r2 = m23;
    outMat->c3.r3 = m33;
    outMat->c3.r4 = m43;

    outMat->c4.r1 = m14;
    outMat->c4.r2 = m24;
    outMat->c4.r3 = m34;
    outMat->c4.r4 = m44;
}

/////////////////////////////////////////////////////////
// definitions for fft
/////////////////////////////////////////////////////////

/**
 * @brief Structure for the floating point FFT function.
 */
#define NE10_MAXFACTORS             32
typedef struct
{
    ne10_float32_t r;
    ne10_float32_t i;
} ne10_fft_cpx_float32_t;

/**
 * @brief Structure for the floating point FFT state
 */
typedef struct
{
    ne10_int32_t nfft;
    ne10_int32_t *factors;
    ne10_fft_cpx_float32_t *twiddles;
    ne10_fft_cpx_float32_t *buffer;
    ne10_fft_cpx_float32_t *last_twiddles;
    /**
     *  @brief Flag to control scaling behaviour in forward floating point complex FFT.
     *  @note If is_forward_scaled is set 0, Ne10 will not scale output of forward floating
     *  point complex FFT. Otherwise, Ne10 will scale output of forward floating
     *  point complex FFT.
     *  @warning Only non-power-of-two FFTs are affected by this flag.
     */
    ne10_int32_t is_forward_scaled;
    /**
     *  @brief Flag to control scaling behaviour in backward floating point complex FFT.
     *  @note If is_backward_scaled is set 0, Ne10 will not scale output of backward floating
     *  point complex FFT. Otherwise, Ne10 will scale output of backward floating
     *  point complex FFT.
     *  @warning Only non-power-of-two FFTs are affected by this flag.
     */
    ne10_int32_t is_backward_scaled;
} ne10_fft_state_float32_t;

/**
 * @brief Configuration structure for floating point FFT.
 */
typedef ne10_fft_state_float32_t* ne10_fft_cfg_float32_t;

typedef struct
{
    ne10_fft_cpx_float32_t *buffer;
#if (NE10_UNROLL_LEVEL == 0)
    ne10_int32_t ncfft;
    ne10_int32_t *factors;
    ne10_fft_cpx_float32_t *twiddles;
    ne10_fft_cpx_float32_t *super_twiddles;
#elif (NE10_UNROLL_LEVEL > 0)
    ne10_int32_t nfft;
    ne10_fft_cpx_float32_t *r_twiddles;
    ne10_int32_t *r_factors;
    ne10_fft_cpx_float32_t *r_twiddles_backward;
    ne10_fft_cpx_float32_t *r_twiddles_neon;
    ne10_fft_cpx_float32_t *r_twiddles_neon_backward;
    ne10_int32_t *r_factors_neon;
    ne10_fft_cpx_float32_t *r_super_twiddles_neon;
#endif
} ne10_fft_r2c_state_float32_t;

typedef ne10_fft_r2c_state_float32_t* ne10_fft_r2c_cfg_float32_t;

/**
 * @brief Structure for the 16-bit fixed point FFT function.
 */
typedef struct
{
    ne10_int16_t r;
    ne10_int16_t i;
} ne10_fft_cpx_int16_t;

typedef struct
{
    ne10_int32_t nfft;
    ne10_int32_t *factors;
    ne10_fft_cpx_int16_t *twiddles;
    ne10_fft_cpx_int16_t *buffer;
} ne10_fft_state_int16_t;

typedef ne10_fft_state_int16_t* ne10_fft_cfg_int16_t;

typedef struct
{
    ne10_int32_t nfft;
    ne10_int32_t ncfft;
    ne10_int32_t *factors;
    ne10_fft_cpx_int16_t *twiddles;
    ne10_fft_cpx_int16_t *super_twiddles;
    ne10_fft_cpx_int16_t *buffer;
} ne10_fft_r2c_state_int16_t;

typedef ne10_fft_r2c_state_int16_t* ne10_fft_r2c_cfg_int16_t;

/**
 * @brief Structure for the 32-bit fixed point FFT function.
 */
typedef struct
{
    ne10_int32_t r;
    ne10_int32_t i;
} ne10_fft_cpx_int32_t;

typedef struct
{
    ne10_int32_t nfft;
    ne10_int32_t *factors;
    ne10_fft_cpx_int32_t *twiddles;
    ne10_fft_cpx_int32_t *buffer;
    ne10_fft_cpx_int32_t *last_twiddles;
} ne10_fft_state_int32_t;

typedef ne10_fft_state_int32_t* ne10_fft_cfg_int32_t;

typedef struct
{
    ne10_int32_t nfft;
    ne10_int32_t ncfft;
    ne10_int32_t *factors;
    ne10_fft_cpx_int32_t *twiddles;
    ne10_fft_cpx_int32_t *super_twiddles;
    ne10_fft_cpx_int32_t *buffer;
} ne10_fft_r2c_state_int32_t;

typedef ne10_fft_r2c_state_int32_t* ne10_fft_r2c_cfg_int32_t;

/////////////////////////////////////////////////////////
// definitions for fir
/////////////////////////////////////////////////////////

/**
 * @brief Instance structure for the floating-point FIR filter.
 */
typedef struct
{
    ne10_uint16_t numTaps;    /**< Length of the filter. */
    ne10_float32_t *pState;    /**< Points to the state variable array. The array is of length numTaps+maxBlockSize-1. */
    ne10_float32_t *pCoeffs;   /**< Points to the coefficient array. The array is of length numTaps. */
} ne10_fir_instance_f32_t;

/**
 * @brief Instance structure for the floating point FIR Lattice filter.
 */
typedef struct
{
    ne10_uint16_t numStages;    /**< numStages of the of lattice filter. */
    ne10_float32_t *pState;      /**< Points to the state variable array. The array is of length numStages. */
    ne10_float32_t *pCoeffs;     /**< Points to the coefficient array. The array is of length numStages. */
} ne10_fir_lattice_instance_f32_t;

/**
 * @brief Instance structure for the floating-point FIR Decimation.
 */
typedef struct
{
    ne10_uint8_t  M;            /**< Decimation Factor. */
    ne10_uint16_t numTaps;      /**< Length of the filter. */
    ne10_float32_t    *pCoeffs;      /**< Points to the coefficient array. The array is of length numTaps.*/
    ne10_float32_t    *pState;       /**< Points to the state variable array. The array is of length numTaps+maxBlockSize-1. */
} ne10_fir_decimate_instance_f32_t;

/**
 * @brief Instance structure for the floating-point FIR Interpolation.
 */
typedef struct
{
    ne10_uint8_t L;             /**< Interpolation Factor. */
    ne10_uint16_t phaseLength;  /**< Length of each polyphase filter component. */
    ne10_float32_t *pCoeffs;         /**< Points to the coefficient array. The array is of length numTaps.*/
    ne10_float32_t *pState;          /**< Points to the state variable array. The array is of length numTaps+maxBlockSize-1. */
} ne10_fir_interpolate_instance_f32_t;

/**
 * @brief Instance structure for the floating-point FIR Sparse filter.
 */
typedef struct
{
    ne10_uint16_t numTaps;      /**< Length of the filter. */
    ne10_uint16_t stateIndex;   /**< Index pointer for the state buffer .*/
    ne10_float32_t *pState;          /**< Points to the state variable array. The array is of length numTaps+maxBlockSize-1. */
    ne10_float32_t *pCoeffs;         /**< Points to the coefficient array. The array is of length numTaps.*/
    ne10_uint16_t  maxDelay;    /**< the largest number of delay line values .*/
    ne10_int32_t  *pTapDelay;    /**< Pointer to the array containing positions of the non-zero tap values. */
} ne10_fir_sparse_instance_f32_t;

/**
   * @brief Instance structure for the floating point IIR Lattice filter.
   */
typedef struct
{
    ne10_uint16_t numStages;    /**< numStages of the of lattice filter. */
    ne10_float32_t *pState;      /**< Points to the state variable array. The array is of length numStages + blockSize -1. */
    ne10_float32_t *pkCoeffs;    /**< Points to the reflection coefficient array. The array is of length numStages. */
    ne10_float32_t *pvCoeffs;    /**< Points to the ladder coefficient array. The array is of length numStages+1. */
} ne10_iir_lattice_instance_f32_t;

/////////////////////////////////////////////////////////
// definitions for imgproc module
/////////////////////////////////////////////////////////

/**
 * @brief Structure for point in image.
 */
typedef struct
{
    ne10_uint32_t x;
    ne10_uint32_t y;
} ne10_point_t;

typedef struct
{
    ne10_uint32_t x;
    ne10_uint32_t y;
} ne10_size_t;

typedef enum
{
    UBUNTU_COMMAND_LINE,
    ANDROID_DEMO,
    IOS_DEMO
} ne10_print_target_t;

#endif
