/*
 *  Copyright 2012-15 ARM Limited and Contributors.
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

#include <string.h>

#include "NE10_types.h"

/**
 * @details
 *
 * @param[in,out] *S points to an instance of the floating-point FIR filter structure.
 * @param[in]     numTaps  Number of filter coefficients in the filter.
 * @param[in]     *pCoeffs points to the filter coefficients buffer.
 * @param[in]     *pState points to the state buffer.
 * @param[in]     blockSize number of samples that are processed per call.
 *
 * <b>Description:</b>
 * \par
 * <code>pCoeffs</code> points to the array of filter coefficients stored in time reversed order:
 * <pre>
 *    {b[numTaps-1], b[numTaps-2], b[N-2], ..., b[1], b[0]}
 * </pre>
 * \par
 * <code>pState</code> points to the array of state variables.
 * <code>pState</code> is of length <code>numTaps+blockSize-1</code> samples, where <code>blockSize</code> is the number of input samples processed by each call to <code>arm_fir_f32()</code>.
 */

ne10_result_t ne10_fir_init_float (ne10_fir_instance_f32_t * S,
                                   ne10_uint16_t numTaps,
                                   ne10_float32_t * pCoeffs,
                                   ne10_float32_t * pState,
                                   ne10_uint32_t blockSize)
{
    /* Assign filter taps */
    S->numTaps = numTaps;

    /* Assign coefficient pointer */
    S->pCoeffs = pCoeffs;

    /* Clear state buffer and the size of state buffer is (blockSize + numTaps - 1) */
    memset (pState, 0, (numTaps + (blockSize - 1u)) * sizeof (ne10_float32_t));

    /* Assign state pointer */
    S->pState = pState;
    return NE10_OK;
}

/**
 * @brief Initialization function for the floating-point FIR decimator.
 * @param[in,out] *S points to an instance of the floating-point FIR decimator structure.
 * @param[in] numTaps  number of coefficients in the filter.
 * @param[in] M  decimation factor.
 * @param[in] *pCoeffs points to the filter coefficients.
 * @param[in] *pState points to the state buffer.
 * @param[in] blockSize number of input samples to process per call.
 * @return    The function returns NE10_OK if initialization was successful or NE10_ERR if
 * <code>blockSize</code> is not a multiple of <code>M</code>.
 *
 * <b>Description:</b>
 * \par
 * <code>pCoeffs</code> points to the array of filter coefficients stored in time reversed order:
 * <pre>
 *    {b[numTaps-1], b[numTaps-2], b[N-2], ..., b[1], b[0]}
 * </pre>
 * \par
 * <code>pState</code> points to the array of state variables.
 * <code>pState</code> is of length <code>numTaps+blockSize-1</code> words where <code>blockSize</code> is the number of input samples passed to <code>arm_fir_decimate_f32()</code>.
 * <code>M</code> is the decimation factor.
 */

ne10_result_t ne10_fir_decimate_init_float (
    ne10_fir_decimate_instance_f32_t * S,
    ne10_uint16_t numTaps,
    ne10_uint8_t M,
    ne10_float32_t * pCoeffs,
    ne10_float32_t * pState,
    ne10_uint32_t blockSize)
{
    ne10_result_t status;

    /* The size of the input block must be a multiple of the decimation factor */
    if ( (blockSize % M) != 0u)
    {
        /* Set status as NE10_ERR */
        status = NE10_ERR;
    }
    else
    {
        /* Assign filter taps */
        S->numTaps = numTaps;

        /* Assign coefficient pointer */
        S->pCoeffs = pCoeffs;

        /* Clear state buffer and size is always (blockSize + numTaps - 1) */
        memset (pState, 0, (numTaps + (blockSize - 1u)) * sizeof (ne10_float32_t));

        /* Assign state pointer */
        S->pState = pState;

        /* Assign Decimation Factor */
        S->M = M;

        status = NE10_OK;
    }

    return (status);

}

/**
 * @brief Initialization function for the floating-point FIR interpolator.
 * @param[in,out] *S        points to an instance of the floating-point FIR interpolator structure.
 * @param[in]     L         upsample factor.
 * @param[in]     numTaps   number of filter coefficients in the filter.
 * @param[in]     *pCoeffs  points to the filter coefficient buffer.
 * @param[in]     *pState   points to the state buffer.
 * @param[in]     blockSize number of input samples to process per call.
 * @return        The function returns NE10_OK if initialization was successful or NE10_ERR if
 * the filter length <code>numTaps</code> is not a multiple of the interpolation factor <code>L</code>.
 *
 * <b>Description:</b>
 * \par
 * <code>pCoeffs</code> points to the array of filter coefficients stored in time reversed order:
 * <pre>
 *    {b[numTaps-1], b[numTaps-2], b[numTaps-2], ..., b[1], b[0]}
 * </pre>
 * The length of the filter <code>numTaps</code> must be a multiple of the interpolation factor <code>L</code>.
 * \par
 * <code>pState</code> points to the array of state variables.
 * <code>pState</code> is of length <code>(numTaps/L)+blockSize-1</code> words
 * where <code>blockSize</code> is the number of input samples processed by each call to <code>arm_fir_interpolate_f32()</code>.
 */

ne10_result_t ne10_fir_interpolate_init_float (
    ne10_fir_interpolate_instance_f32_t * S,
    ne10_uint8_t L,
    ne10_uint16_t numTaps,
    ne10_float32_t * pCoeffs,
    ne10_float32_t * pState,
    ne10_uint32_t blockSize)
{
    ne10_result_t status;

    /* The filter length must be a multiple of the interpolation factor */
    if ( (numTaps % L) != 0u)
    {
        /* Set status as NE10_ERR */
        status = NE10_ERR;
    }
    else
    {

        /* Assign coefficient pointer */
        S->pCoeffs = pCoeffs;

        /* Assign Interpolation factor */
        S->L = L;

        /* Assign polyPhaseLength */
        S->phaseLength = numTaps / L;

        /* Clear state buffer and size of state array is always phaseLength + blockSize - 1 */
        memset (pState, 0,
                (blockSize +
                 ( (ne10_uint32_t) S->phaseLength - 1u)) * sizeof (ne10_float32_t));

        /* Assign state pointer */
        S->pState = pState;

        status = NE10_OK;
    }

    return (status);

}

/**
 * @brief Initialization function for the floating-point FIR lattice filter.
 * @param[in] *S points to an instance of the floating-point FIR lattice structure.
 * @param[in] numStages  number of filter stages.
 * @param[in] *pCoeffs points to the coefficient buffer.  The array is of length numStages.
 * @param[in] *pState points to the state buffer.  The array is of length numStages.
 */

ne10_result_t ne10_fir_lattice_init_float (
    ne10_fir_lattice_instance_f32_t * S,
    ne10_uint16_t numStages,
    ne10_float32_t * pCoeffs,
    ne10_float32_t * pState)
{
    /* Assign filter taps */
    S->numStages = numStages;

    /* Assign coefficient pointer */
    S->pCoeffs = pCoeffs;

    /* Clear state buffer and size is always numStages */
    memset (pState, 0, (numStages) * sizeof (ne10_float32_t));

    /* Assign state pointer */
    S->pState = pState;

    return NE10_OK;
}

/**
 * @brief Initialization function for the floating-point sparse FIR filter.
 * @param[in,out] *S         points to an instance of the floating-point sparse FIR structure.
 * @param[in]     numTaps    number of nonzero coefficients in the filter.
 * @param[in]     *pCoeffs   points to the array of filter coefficients.
 * @param[in]     *pState    points to the state buffer.
 * @param[in]     *pTapDelay points to the array of offset times.
 * @param[in]     maxDelay   maximum offset time supported.
 * @param[in]     blockSize  number of samples that will be processed per block.
 *
 * <b>Description:</b>
 * \par
 * <code>pCoeffs</code> holds the filter coefficients and has length <code>numTaps</code>.
 * <code>pState</code> holds the filter's state variables and must be of length
 * <code>maxDelay + blockSize</code>, where <code>maxDelay</code>
 * is the maximum number of delay line values.
 * <code>blockSize</code> is the
 * number of samples processed by the <code>arm_fir_sparse_f32()</code> function.
 */

ne10_result_t ne10_fir_sparse_init_float (
    ne10_fir_sparse_instance_f32_t * S,
    ne10_uint16_t numTaps,
    ne10_float32_t * pCoeffs,
    ne10_float32_t * pState,
    ne10_int32_t * pTapDelay,
    ne10_uint16_t maxDelay,
    ne10_uint32_t blockSize)
{
    /* Assign filter taps */
    S->numTaps = numTaps;

    /* Assign coefficient pointer */
    S->pCoeffs = pCoeffs;

    /* Assign TapDelay pointer */
    S->pTapDelay = pTapDelay;

    /* Assign MaxDelay */
    S->maxDelay = maxDelay;

    /* reset the stateIndex to 0 */
    S->stateIndex = 0u;

    /* Clear state buffer and size is always maxDelay + blockSize */
    memset (pState, 0, (maxDelay + blockSize) * sizeof (ne10_float32_t));

    /* Assign state pointer */
    S->pState = pState;

    return NE10_OK;
}
