/*
 *  Copyright 2012 ARM Limited
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/*
 * NE10 Library : dsp/NE10_fir.c
 */

#include "NE10_types.h"

/**
 * @ingroup groupFilters
 */

/**
 * @defgroup FIR Finite Impulse Response (FIR) Filters
 *
 * This set of functions implements Finite Impulse Response (FIR) filters
 * for floating-point data types.
 * The functions operate on blocks of input and output data and each call to the function processes
 * <code>blockSize</code> samples through the filter.  <code>pSrc</code> and
 * <code>pDst</code> points to input and output arrays containing <code>blockSize</code> values.
 *
 * \par Algorithm:
 * The FIR filter algorithm is based upon a sequence of multiply-accumulate (MAC) operations.
 * Each filter coefficient <code>b[n]</code> is multiplied by a state variable which equals a previous input sample <code>x[n]</code>.
 * <pre>
 *    y[n] = b[0] * x[n] + b[1] * x[n-1] + b[2] * x[n-2] + ...+ b[numTaps-1] * x[n-numTaps+1]
 * </pre>
 * \par
 * \image html FIR.gif "Finite Impulse Response filter"
 * \par
 * <code>pCoeffs</code> points to a coefficient array of size <code>numTaps</code>.
 * Coefficients are stored in time reversed order.
 * \par
 * <pre>
 *    {b[numTaps-1], b[numTaps-2], b[N-2], ..., b[1], b[0]}
 * </pre>
 * \par
 * <code>pState</code> points to a state array of size <code>numTaps + blockSize - 1</code>.
 * Samples in the state buffer are stored in the following order.
 * \par
 * <pre>
 *    {x[n-numTaps+1], x[n-numTaps], x[n-numTaps-1], x[n-numTaps-2]....x[0], x[1], ..., x[blockSize-1]}
 * </pre>
 * \par
 * Note that the length of the state buffer exceeds the length of the coefficient array by <code>blockSize-1</code>.
 * The increased state buffer length allows circular addressing, which is traditionally used in the FIR filters,
 * to be avoided and yields a significant speed improvement.
 * The state variables are updated after each block of data is processed; the coefficients are untouched.
 * \par Instance Structure
 * The coefficients and state variables for a filter are stored together in an instance data structure.
 * A separate instance structure must be defined for each filter.
 * Coefficient arrays may be shared among several instances while state variable arrays cannot be shared.
 * There are separate instance structure declarations for each of the 4 supported data types.
 *
 * \par Initialization Functions
 * There is also an associated initialization function for each data type.
 * The initialization function performs the following operations:
 * - Sets the values of the internal structure fields.
 * - Zeros out the values in the state buffer.
 *
 * \par
 * Use of the initialization function is optional.
 * However, if the initialization function is used, then the instance structure cannot be placed into a const data section.
 * To place an instance structure into a const data section, the instance structure must be manually initialized.
 * Set the values in the state buffer to zeros before static initialization.
 * The code below statically initializes each of the 4 different data type filter instance structures
 * <pre>
 *ne10_fir_instance_f32_t S = {numTaps, pState, pCoeffs};
 * </pre>
 *
 * where <code>numTaps</code> is the number of filter coefficients in the filter; <code>pState</code> is the address of the state buffer;
 * <code>pCoeffs</code> is the address of the coefficient buffer.
 *
 * \par Fixed-Point Behavior
 * Care must be taken when using the fixed-point versions of the FIR filter functions.
 * In particular, the overflow and saturation behavior of the accumulator used in each function must be considered.
 * Refer to the function specific documentation below for usage guidelines.
 */

/**
 * @addtogroup FIR
 * @{
 */

/**
 *
 * @param[in]  *S points to an instance of the floating-point FIR filter structure.
 * @param[in]  *pSrc points to the block of input data.
 * @param[out] *pDst points to the block of output data.
 * @param[in]  blockSize number of samples to process per call.
 * @return     none.
 *
 */

void ne10_fir_float_c (const ne10_fir_instance_f32_t * S,
    ne10_float32_t * pSrc,
    ne10_float32_t * pDst,
    ne10_uint32_t blockSize)
{

    ne10_float32_t *pState = S->pState;                 /* State pointer */
    ne10_float32_t *pCoeffs = S->pCoeffs;               /* Coefficient pointer */
    ne10_float32_t *pStateCurnt;                        /* Points to the current sample of the state */
    ne10_float32_t *px, *pb;                            /* Temporary pointers for state and coefficient buffers */
    ne10_uint32_t numTaps = S->numTaps;                 /* Number of filter coefficients in the filter */
    ne10_uint32_t i, tapCnt, blkCnt;                    /* Loop counters */

    /* Run the below code for Cortex-M4 and Cortex-M3 */

    ne10_float32_t acc0, acc1, acc2, acc3;              /* Accumulators */
    ne10_float32_t x0, x1, x2, x3, c0;                  /* Temporary variables to hold state and coefficient values */


    /* S->pState points to state array which contains previous frame (numTaps - 1) samples */
    /* pStateCurnt points to the location where the new input data should be written */
    pStateCurnt = & (S->pState[ (numTaps - 1u)]);

    /* Apply loop unrolling and compute 4 output values simultaneously.
     * The variables acc0 ... acc3 hold output values that are being computed:
     *
     *    acc0 =  b[numTaps-1] * x[n-numTaps-1] + b[numTaps-2] * x[n-numTaps-2] + b[numTaps-3] * x[n-numTaps-3] +...+ b[0] * x[0]
     *    acc1 =  b[numTaps-1] * x[n-numTaps] +   b[numTaps-2] * x[n-numTaps-1] + b[numTaps-3] * x[n-numTaps-2] +...+ b[0] * x[1]
     *    acc2 =  b[numTaps-1] * x[n-numTaps+1] + b[numTaps-2] * x[n-numTaps] +   b[numTaps-3] * x[n-numTaps-1] +...+ b[0] * x[2]
     *    acc3 =  b[numTaps-1] * x[n-numTaps+2] + b[numTaps-2] * x[n-numTaps+1] + b[numTaps-3] * x[n-numTaps]   +...+ b[0] * x[3]
     */
    blkCnt = blockSize >> 2;

    /* First part of the processing with loop unrolling.  Compute 4 outputs at a time.
     ** a second loop below computes the remaining 1 to 3 samples. */
    while (blkCnt > 0u)
    {
        /* Copy four new input samples into the state buffer */
        *pStateCurnt++ = *pSrc++;
        *pStateCurnt++ = *pSrc++;
        *pStateCurnt++ = *pSrc++;
        *pStateCurnt++ = *pSrc++;

        /* Set all accumulators to zero */
        acc0 = 0.0f;
        acc1 = 0.0f;
        acc2 = 0.0f;
        acc3 = 0.0f;

        /* Initialize state pointer */
        px = pState;

        /* Initialize coeff pointer */
        pb = (pCoeffs);

        /* Read the first three samples from the state buffer:  x[n-numTaps], x[n-numTaps-1], x[n-numTaps-2] */
        x0 = *px++;
        x1 = *px++;
        x2 = *px++;

        /* Loop unrolling.  Process 4 taps at a time. */
        tapCnt = numTaps >> 2u;

        /* Loop over the number of taps.  Unroll by a factor of 4.
         ** Repeat until we've computed numTaps-4 coefficients. */
        while (tapCnt > 0u)
        {
            /* Read the b[numTaps-1] coefficient */
            c0 = * (pb++);

            /* Read x[n-numTaps-3] sample */
            x3 = * (px++);

            /* acc0 +=  b[numTaps-1] * x[n-numTaps] */
            acc0 += x0 * c0;

            /* acc1 +=  b[numTaps-1] * x[n-numTaps-1] */
            acc1 += x1 * c0;

            /* acc2 +=  b[numTaps-1] * x[n-numTaps-2] */
            acc2 += x2 * c0;

            /* acc3 +=  b[numTaps-1] * x[n-numTaps-3] */
            acc3 += x3 * c0;

            /* Read the b[numTaps-2] coefficient */
            c0 = * (pb++);

            /* Read x[n-numTaps-4] sample */
            x0 = * (px++);

            /* Perform the multiply-accumulate */
            acc0 += x1 * c0;
            acc1 += x2 * c0;
            acc2 += x3 * c0;
            acc3 += x0 * c0;

            /* Read the b[numTaps-3] coefficient */
            c0 = * (pb++);

            /* Read x[n-numTaps-5] sample */
            x1 = * (px++);

            /* Perform the multiply-accumulates */
            acc0 += x2 * c0;
            acc1 += x3 * c0;
            acc2 += x0 * c0;
            acc3 += x1 * c0;

            /* Read the b[numTaps-4] coefficient */
            c0 = * (pb++);

            /* Read x[n-numTaps-6] sample */
            x2 = * (px++);

            /* Perform the multiply-accumulates */
            acc0 += x3 * c0;
            acc1 += x0 * c0;
            acc2 += x1 * c0;
            acc3 += x2 * c0;

            tapCnt--;
        }

        /* If the filter length is not a multiple of 4, compute the remaining filter taps */
        tapCnt = numTaps % 0x4u;

        while (tapCnt > 0u)
        {
            /* Read coefficients */
            c0 = * (pb++);

            /* Fetch 1 state variable */
            x3 = * (px++);

            /* Perform the multiply-accumulates */
            acc0 += x0 * c0;
            acc1 += x1 * c0;
            acc2 += x2 * c0;
            acc3 += x3 * c0;

            /* Reuse the present sample states for next sample */
            x0 = x1;
            x1 = x2;
            x2 = x3;

            /* Decrement the loop counter */
            tapCnt--;
        }

        /* Advance the state pointer by 4 to process the next group of 4 samples */
        pState = pState + 4;

        /* The results in the 4 accumulators, store in the destination buffer. */
        *pDst++ = acc0;
        *pDst++ = acc1;
        *pDst++ = acc2;
        *pDst++ = acc3;

        blkCnt--;
    }

    /* If the blockSize is not a multiple of 4, compute any remaining output samples here.
     ** No loop unrolling is used. */
    blkCnt = blockSize % 0x4u;

    while (blkCnt > 0u)
    {
        /* Copy one sample at a time into state buffer */
        *pStateCurnt++ = *pSrc++;

        /* Set the accumulator to zero */
        acc0 = 0.0f;

        /* Initialize state pointer */
        px = pState;

        /* Initialize Coefficient pointer */
        pb = (pCoeffs);

        i = numTaps;

        /* Perform the multiply-accumulates */
        do
        {
            acc0 += *px++ * *pb++;
            i--;

        }
        while (i > 0u);

        /* The result is store in the destination buffer. */
        *pDst++ = acc0;

        /* Advance state pointer by 1 for the next sample */
        pState = pState + 1;

        blkCnt--;
    }

    /* Processing is complete.
     ** Now copy the last numTaps - 1 samples to the satrt of the state buffer.
     ** This prepares the state buffer for the next function call. */

    /* Points to the start of the state buffer */
    pStateCurnt = S->pState;

    tapCnt = (numTaps - 1u) >> 2u;

    /* copy data */
    while (tapCnt > 0u)
    {
        *pStateCurnt++ = *pState++;
        *pStateCurnt++ = *pState++;
        *pStateCurnt++ = *pState++;
        *pStateCurnt++ = *pState++;

        /* Decrement the loop counter */
        tapCnt--;
    }

    /* Calculate remaining number of copies */
    tapCnt = (numTaps - 1u) % 0x4u;

    /* Copy the remaining q31_t data */
    while (tapCnt > 0u)
    {
        *pStateCurnt++ = *pState++;

        /* Decrement the loop counter */
        tapCnt--;
    }

}

/**
   * @brief Processing function for the floating-point FIR decimator.
   * @param[in] *S        points to an instance of the floating-point FIR decimator structure.
   * @param[in] *pSrc     points to the block of input data.
   * @param[out] *pDst    points to the block of output data.
   * @param[in] blockSize number of input samples to process per call.
   * @return none.
   */

void ne10_fir_decimate_float_c (const ne10_fir_decimate_instance_f32_t * S,
    ne10_float32_t * pSrc,
    ne10_float32_t * pDst,
    ne10_uint32_t blockSize)
{
    ne10_float32_t *pState = S->pState;                 /* State pointer */
    ne10_float32_t *pCoeffs = S->pCoeffs;               /* Coefficient pointer */
    ne10_float32_t *pStateCurnt;                        /* Points to the current sample of the state */
    ne10_float32_t *px, *pb;                            /* Temporary pointers for state and coefficient buffers */
    ne10_float32_t sum0;                                /* Accumulator */
    ne10_float32_t x0, c0;                              /* Temporary variables to hold state and coefficient values */
    ne10_uint32_t numTaps = S->numTaps;                 /* Number of filter coefficients in the filter */
    ne10_uint32_t i, tapCnt, blkCnt, outBlockSize = blockSize / S->M;  /* Loop counters */


    /* Run the below code for Cortex-M4 and Cortex-M3 */

    /* S->pState buffer contains previous frame (numTaps - 1) samples */
    /* pStateCurnt points to the location where the new input data should be written */
    pStateCurnt = S->pState + (numTaps - 1u);

    /* Total number of output samples to be computed */
    blkCnt = outBlockSize;

    while (blkCnt > 0u)
    {
        /* Copy decimation factor number of new input samples into the state buffer */
        i = S->M;

        do
        {
            *pStateCurnt++ = *pSrc++;

        }
        while (--i);

        /* Set accumulator to zero */
        sum0 = 0.0f;

        /* Initialize state pointer */
        px = pState;

        /* Initialize coeff pointer */
        pb = pCoeffs;

        /* Loop unrolling.  Process 4 taps at a time. */
        tapCnt = numTaps >> 2;

        /* Loop over the number of taps.  Unroll by a factor of 4.
         ** Repeat until we've computed numTaps-4 coefficients. */
        while (tapCnt > 0u)
        {
            /* Read the b[numTaps-1] coefficient */
            c0 = * (pb++);

            /* Read x[n-numTaps-1] sample */
            x0 = * (px++);

            /* Perform the multiply-accumulate */
            sum0 += x0 * c0;

            /* Read the b[numTaps-2] coefficient */
            c0 = * (pb++);

            /* Read x[n-numTaps-2] sample */
            x0 = * (px++);

            /* Perform the multiply-accumulate */
            sum0 += x0 * c0;

            /* Read the b[numTaps-3] coefficient */
            c0 = * (pb++);

            /* Read x[n-numTaps-3] sample */
            x0 = * (px++);

            /* Perform the multiply-accumulate */
            sum0 += x0 * c0;

            /* Read the b[numTaps-4] coefficient */
            c0 = * (pb++);

            /* Read x[n-numTaps-4] sample */
            x0 = * (px++);

            /* Perform the multiply-accumulate */
            sum0 += x0 * c0;

            /* Decrement the loop counter */
            tapCnt--;
        }

        /* If the filter length is not a multiple of 4, compute the remaining filter taps */
        tapCnt = numTaps % 0x4u;

        while (tapCnt > 0u)
        {
            /* Read coefficients */
            c0 = * (pb++);

            /* Fetch 1 state variable */
            x0 = * (px++);

            /* Perform the multiply-accumulate */
            sum0 += x0 * c0;

            /* Decrement the loop counter */
            tapCnt--;
        }

        /* Advance the state pointer by the decimation factor
         * to process the next group of decimation factor number samples */
        pState = pState + S->M;

        /* The result is in the accumulator, store in the destination buffer. */
        *pDst++ = sum0;

        /* Decrement the loop counter */
        blkCnt--;
    }

    /* Processing is complete.
     ** Now copy the last numTaps - 1 samples to the satrt of the state buffer.
     ** This prepares the state buffer for the next function call. */

    /* Points to the start of the state buffer */
    pStateCurnt = S->pState;

    i = (numTaps - 1u) >> 2;

    /* copy data */
    while (i > 0u)
    {
        *pStateCurnt++ = *pState++;
        *pStateCurnt++ = *pState++;
        *pStateCurnt++ = *pState++;
        *pStateCurnt++ = *pState++;

        /* Decrement the loop counter */
        i--;
    }

    i = (numTaps - 1u) % 0x04u;

    /* copy data */
    while (i > 0u)
    {
        *pStateCurnt++ = *pState++;

        /* Decrement the loop counter */
        i--;
    }

}

/**
 * @brief Processing function for the floating-point FIR interpolator.
 * @param[in] *S        points to an instance of the floating-point FIR interpolator structure.
 * @param[in] *pSrc     points to the block of input data.
 * @param[out] *pDst    points to the block of output data.
 * @param[in] blockSize number of input samples to process per call.
 * @return none.
 */

void ne10_fir_interpolate_float_c (const ne10_fir_interpolate_instance_f32_t * S,
    ne10_float32_t * pSrc,
    ne10_float32_t * pDst,
    ne10_uint32_t blockSize)
{
    ne10_float32_t *pState = S->pState;                 /* State pointer */
    ne10_float32_t *pCoeffs = S->pCoeffs;               /* Coefficient pointer */
    ne10_float32_t *pStateCurnt;                        /* Points to the current sample of the state */
    ne10_float32_t *ptr1, *ptr2;                        /* Temporary pointers for state and coefficient buffers */


    /* Run the below code for Cortex-M4 and Cortex-M3 */

    ne10_float32_t sum0;                                /* Accumulators */
    ne10_float32_t x0, c0;                              /* Temporary variables to hold state and coefficient values */
    ne10_uint32_t i, blkCnt, j;                         /* Loop counters */
    ne10_uint16_t phaseLen = S->phaseLength, tapCnt;    /* Length of each polyphase filter component */


    /* S->pState buffer contains previous frame (phaseLen - 1) samples */
    /* pStateCurnt points to the location where the new input data should be written */
    pStateCurnt = S->pState + (phaseLen - 1u);

    /* Total number of intput samples */
    blkCnt = blockSize;

    /* Loop over the blockSize. */
    while (blkCnt > 0u)
    {
        /* Copy new input sample into the state buffer */
        *pStateCurnt++ = *pSrc++;

        /* Address modifier index of coefficient buffer */
        j = 1u;

        /* Loop over the Interpolation factor. */
        i = S->L;
        while (i > 0u)
        {
            /* Set accumulator to zero */
            sum0 = 0.0f;

            /* Initialize state pointer */
            ptr1 = pState;

            /* Initialize coefficient pointer */
            ptr2 = pCoeffs + (S->L - j);

            /* Loop over the polyPhase length. Unroll by a factor of 4.
             ** Repeat until we've computed numTaps-(4*S->L) coefficients. */
            tapCnt = phaseLen >> 2u;
            while (tapCnt > 0u)
            {

                /* Read the coefficient */
                c0 = * (ptr2);

                /* Upsampling is done by stuffing L-1 zeros between each sample.
                 * So instead of multiplying zeros with coefficients,
                 * Increment the coefficient pointer by interpolation factor times. */
                ptr2 += S->L;

                /* Read the input sample */
                x0 = * (ptr1++);

                /* Perform the multiply-accumulate */
                sum0 += x0 * c0;

                /* Read the coefficient */
                c0 = * (ptr2);

                /* Increment the coefficient pointer by interpolation factor times. */
                ptr2 += S->L;

                /* Read the input sample */
                x0 = * (ptr1++);

                /* Perform the multiply-accumulate */
                sum0 += x0 * c0;

                /* Read the coefficient */
                c0 = * (ptr2);

                /* Increment the coefficient pointer by interpolation factor times. */
                ptr2 += S->L;

                /* Read the input sample */
                x0 = * (ptr1++);

                /* Perform the multiply-accumulate */
                sum0 += x0 * c0;

                /* Read the coefficient */
                c0 = * (ptr2);

                /* Increment the coefficient pointer by interpolation factor times. */
                ptr2 += S->L;

                /* Read the input sample */
                x0 = * (ptr1++);

                /* Perform the multiply-accumulate */
                sum0 += x0 * c0;

                /* Decrement the loop counter */
                tapCnt--;
            }

            /* If the polyPhase length is not a multiple of 4, compute the remaining filter taps */
            tapCnt = phaseLen % 0x4u;

            while (tapCnt > 0u)
            {
                /* Perform the multiply-accumulate */
                sum0 += * (ptr1++) * (*ptr2);

                /* Increment the coefficient pointer by interpolation factor times. */
                ptr2 += S->L;

                /* Decrement the loop counter */
                tapCnt--;
            }

            /* The result is in the accumulator, store in the destination buffer. */
            *pDst++ = sum0;

            /* Increment the address modifier index of coefficient buffer */
            j++;

            /* Decrement the loop counter */
            i--;
        }

        /* Advance the state pointer by 1
         * to process the next group of interpolation factor number samples */
        pState = pState + 1;

        /* Decrement the loop counter */
        blkCnt--;
    }

    /* Processing is complete.
     ** Now copy the last phaseLen - 1 samples to the satrt of the state buffer.
     ** This prepares the state buffer for the next function call. */

    /* Points to the start of the state buffer */
    pStateCurnt = S->pState;

    tapCnt = (phaseLen - 1u) >> 2u;

    /* copy data */
    while (tapCnt > 0u)
    {
        *pStateCurnt++ = *pState++;
        *pStateCurnt++ = *pState++;
        *pStateCurnt++ = *pState++;
        *pStateCurnt++ = *pState++;

        /* Decrement the loop counter */
        tapCnt--;
    }

    tapCnt = (phaseLen - 1u) % 0x04u;

    while (tapCnt > 0u)
    {
        *pStateCurnt++ = *pState++;

        /* Decrement the loop counter */
        tapCnt--;
    }

}

/**
   * @brief Processing function for the floating-point FIR lattice filter.
   * @param[in]  *S        points to an instance of the floating-point FIR lattice structure.
   * @param[in]  *pSrc     points to the block of input data.
   * @param[out] *pDst     points to the block of output data
   * @param[in]  blockSize number of samples to process.
   * @return none.
   */

void ne10_fir_lattice_float_c (const ne10_fir_lattice_instance_f32_t * S,
    ne10_float32_t * pSrc,
    ne10_float32_t * pDst,
    ne10_uint32_t blockSize)
{
    ne10_float32_t *pState;                             /* State pointer */
    ne10_float32_t *pCoeffs = S->pCoeffs;               /* Coefficient pointer */
    ne10_float32_t *px;                                 /* temporary state pointer */
    ne10_float32_t *pk;                                 /* temporary coefficient pointer */


    /* Run the below code for Cortex-M4 and Cortex-M3 */

    ne10_float32_t fcurr1, fnext1, gcurr1, gnext1;      /* temporary variables for first sample in loop unrolling */
    ne10_float32_t fcurr2, fnext2, gnext2;              /* temporary variables for second sample in loop unrolling */
    ne10_float32_t fcurr3, fnext3, gnext3;              /* temporary variables for third sample in loop unrolling */
    ne10_float32_t fcurr4, fnext4, gnext4;              /* temporary variables for fourth sample in loop unrolling */
    ne10_uint32_t numStages = S->numStages;             /* Number of stages in the filter */
    ne10_uint32_t blkCnt, stageCnt;                     /* temporary variables for counts */

    gcurr1 = 0.0f;
    pState = &S->pState[0];

    blkCnt = blockSize >> 2;

    /* First part of the processing with loop unrolling.  Compute 4 outputs at a time.
       a second loop below computes the remaining 1 to 3 samples. */
    while (blkCnt > 0u)
    {

        /* Read two samples from input buffer */
        /* f0(n) = x(n) */
        fcurr1 = *pSrc++;
        fcurr2 = *pSrc++;

        /* Initialize coeff pointer */
        pk = (pCoeffs);

        /* Initialize state pointer */
        px = pState;

        /* Read g0(n-1) from state */
        gcurr1 = *px;

        /* Process first sample for first tap */
        /* f1(n) = f0(n) +  K1 * g0(n-1) */
        fnext1 = fcurr1 + ( (*pk) * gcurr1);
        /* g1(n) = f0(n) * K1  +  g0(n-1) */
        gnext1 = (fcurr1 * (*pk)) + gcurr1;

        /* Process second sample for first tap */
        /* for sample 2 processing */
        fnext2 = fcurr2 + ( (*pk) * fcurr1);
        gnext2 = (fcurr2 * (*pk)) + fcurr1;

        /* Read next two samples from input buffer */
        /* f0(n+2) = x(n+2) */
        fcurr3 = *pSrc++;
        fcurr4 = *pSrc++;

        /* Copy only last input samples into the state buffer
           which will be used for next four samples processing */
        *px++ = fcurr4;

        /* Process third sample for first tap */
        fnext3 = fcurr3 + ( (*pk) * fcurr2);
        gnext3 = (fcurr3 * (*pk)) + fcurr2;

        /* Process fourth sample for first tap */
        fnext4 = fcurr4 + ( (*pk) * fcurr3);
        gnext4 = (fcurr4 * (*pk++)) + fcurr3;

        /* Update of f values for next coefficient set processing */
        fcurr1 = fnext1;
        fcurr2 = fnext2;
        fcurr3 = fnext3;
        fcurr4 = fnext4;

        /* Loop unrolling.  Process 4 taps at a time . */
        stageCnt = (numStages - 1u) >> 2u;

        /* Loop over the number of taps.  Unroll by a factor of 4.
         ** Repeat until we've computed numStages-3 coefficients. */

        /* Process 2nd, 3rd, 4th and 5th taps ... here */
        while (stageCnt > 0u)
        {
            /* Read g1(n-1), g3(n-1) .... from state */
            gcurr1 = *px;

            /* save g1(n) in state buffer */
            *px++ = gnext4;

            /* Process first sample for 2nd, 6th .. tap */
            /* Sample processing for K2, K6.... */
            /* f2(n) = f1(n) +  K2 * g1(n-1) */
            fnext1 = fcurr1 + ( (*pk) * gcurr1);
            /* Process second sample for 2nd, 6th .. tap */
            /* for sample 2 processing */
            fnext2 = fcurr2 + ( (*pk) * gnext1);
            /* Process third sample for 2nd, 6th .. tap */
            fnext3 = fcurr3 + ( (*pk) * gnext2);
            /* Process fourth sample for 2nd, 6th .. tap */
            fnext4 = fcurr4 + ( (*pk) * gnext3);

            /* g2(n) = f1(n) * K2  +  g1(n-1) */
            /* Calculation of state values for next stage */
            gnext4 = (fcurr4 * (*pk)) + gnext3;
            gnext3 = (fcurr3 * (*pk)) + gnext2;
            gnext2 = (fcurr2 * (*pk)) + gnext1;
            gnext1 = (fcurr1 * (*pk++)) + gcurr1;


            /* Read g2(n-1), g4(n-1) .... from state */
            gcurr1 = *px;

            /* save g2(n) in state buffer */
            *px++ = gnext4;

            /* Sample processing for K3, K7.... */
            /* Process first sample for 3rd, 7th .. tap */
            /* f3(n) = f2(n) +  K3 * g2(n-1) */
            fcurr1 = fnext1 + ( (*pk) * gcurr1);
            /* Process second sample for 3rd, 7th .. tap */
            fcurr2 = fnext2 + ( (*pk) * gnext1);
            /* Process third sample for 3rd, 7th .. tap */
            fcurr3 = fnext3 + ( (*pk) * gnext2);
            /* Process fourth sample for 3rd, 7th .. tap */
            fcurr4 = fnext4 + ( (*pk) * gnext3);

            /* Calculation of state values for next stage */
            /* g3(n) = f2(n) * K3  +  g2(n-1) */
            gnext4 = (fnext4 * (*pk)) + gnext3;
            gnext3 = (fnext3 * (*pk)) + gnext2;
            gnext2 = (fnext2 * (*pk)) + gnext1;
            gnext1 = (fnext1 * (*pk++)) + gcurr1;


            /* Read g1(n-1), g3(n-1) .... from state */
            gcurr1 = *px;

            /* save g3(n) in state buffer */
            *px++ = gnext4;

            /* Sample processing for K4, K8.... */
            /* Process first sample for 4th, 8th .. tap */
            /* f4(n) = f3(n) +  K4 * g3(n-1) */
            fnext1 = fcurr1 + ( (*pk) * gcurr1);
            /* Process second sample for 4th, 8th .. tap */
            /* for sample 2 processing */
            fnext2 = fcurr2 + ( (*pk) * gnext1);
            /* Process third sample for 4th, 8th .. tap */
            fnext3 = fcurr3 + ( (*pk) * gnext2);
            /* Process fourth sample for 4th, 8th .. tap */
            fnext4 = fcurr4 + ( (*pk) * gnext3);

            /* g4(n) = f3(n) * K4  +  g3(n-1) */
            /* Calculation of state values for next stage */
            gnext4 = (fcurr4 * (*pk)) + gnext3;
            gnext3 = (fcurr3 * (*pk)) + gnext2;
            gnext2 = (fcurr2 * (*pk)) + gnext1;
            gnext1 = (fcurr1 * (*pk++)) + gcurr1;

            /* Read g2(n-1), g4(n-1) .... from state */
            gcurr1 = *px;

            /* save g4(n) in state buffer */
            *px++ = gnext4;

            /* Sample processing for K5, K9.... */
            /* Process first sample for 5th, 9th .. tap */
            /* f5(n) = f4(n) +  K5 * g4(n-1) */
            fcurr1 = fnext1 + ( (*pk) * gcurr1);
            /* Process second sample for 5th, 9th .. tap */
            fcurr2 = fnext2 + ( (*pk) * gnext1);
            /* Process third sample for 5th, 9th .. tap */
            fcurr3 = fnext3 + ( (*pk) * gnext2);
            /* Process fourth sample for 5th, 9th .. tap */
            fcurr4 = fnext4 + ( (*pk) * gnext3);

            /* Calculation of state values for next stage */
            /* g5(n) = f4(n) * K5  +  g4(n-1) */
            gnext4 = (fnext4 * (*pk)) + gnext3;
            gnext3 = (fnext3 * (*pk)) + gnext2;
            gnext2 = (fnext2 * (*pk)) + gnext1;
            gnext1 = (fnext1 * (*pk++)) + gcurr1;

            stageCnt--;
        }

        /* If the (filter length -1) is not a multiple of 4, compute the remaining filter taps */
        stageCnt = (numStages - 1u) % 0x4u;

        while (stageCnt > 0u)
        {
            gcurr1 = *px;

            /* save g value in state buffer */
            *px++ = gnext4;

            /* Process four samples for last three taps here */
            fnext1 = fcurr1 + ( (*pk) * gcurr1);
            fnext2 = fcurr2 + ( (*pk) * gnext1);
            fnext3 = fcurr3 + ( (*pk) * gnext2);
            fnext4 = fcurr4 + ( (*pk) * gnext3);

            /* g1(n) = f0(n) * K1  +  g0(n-1) */
            gnext4 = (fcurr4 * (*pk)) + gnext3;
            gnext3 = (fcurr3 * (*pk)) + gnext2;
            gnext2 = (fcurr2 * (*pk)) + gnext1;
            gnext1 = (fcurr1 * (*pk++)) + gcurr1;

            /* Update of f values for next coefficient set processing */
            fcurr1 = fnext1;
            fcurr2 = fnext2;
            fcurr3 = fnext3;
            fcurr4 = fnext4;

            stageCnt--;

        }

        /* The results in the 4 accumulators, store in the destination buffer. */
        /* y(n) = fN(n) */
        *pDst++ = fcurr1;
        *pDst++ = fcurr2;
        *pDst++ = fcurr3;
        *pDst++ = fcurr4;

        blkCnt--;
    }

    /* If the blockSize is not a multiple of 4, compute any remaining output samples here.
     ** No loop unrolling is used. */
    blkCnt = blockSize % 0x4u;

    while (blkCnt > 0u)
    {
        /* f0(n) = x(n) */
        fcurr1 = *pSrc++;

        /* Initialize coeff pointer */
        pk = (pCoeffs);

        /* Initialize state pointer */
        px = pState;

        /* read g2(n) from state buffer */
        gcurr1 = *px;

        /* for sample 1 processing */
        /* f1(n) = f0(n) +  K1 * g0(n-1) */
        fnext1 = fcurr1 + ( (*pk) * gcurr1);
        /* g1(n) = f0(n) * K1  +  g0(n-1) */
        gnext1 = (fcurr1 * (*pk++)) + gcurr1;

        /* save g1(n) in state buffer */
        *px++ = fcurr1;

        /* f1(n) is saved in fcurr1
           for next stage processing */
        fcurr1 = fnext1;

        stageCnt = (numStages - 1u);

        /* stage loop */
        while (stageCnt > 0u)
        {
            /* read g2(n) from state buffer */
            gcurr1 = *px;

            /* save g1(n) in state buffer */
            *px++ = gnext1;

            /* Sample processing for K2, K3.... */
            /* f2(n) = f1(n) +  K2 * g1(n-1) */
            fnext1 = fcurr1 + ( (*pk) * gcurr1);
            /* g2(n) = f1(n) * K2  +  g1(n-1) */
            gnext1 = (fcurr1 * (*pk++)) + gcurr1;

            /* f1(n) is saved in fcurr1
               for next stage processing */
            fcurr1 = fnext1;

            stageCnt--;

        }

        /* y(n) = fN(n) */
        *pDst++ = fcurr1;

        blkCnt--;

    }

}
/**
   * @brief floating-point Circular write function.
   */

static void ne10_circular_write_float (ne10_int32_t * circBuffer,
    ne10_int32_t L,
    ne10_uint16_t * writeOffset,
    ne10_int32_t bufferInc,
    const ne10_int32_t * src,
    ne10_int32_t srcInc,
    ne10_uint32_t blockSize)
{
    ne10_uint32_t i = 0u;
    ne10_int32_t wOffset;

    /* Copy the value of Index pointer that points
     * to the current location where the input samples to be copied */
    wOffset = *writeOffset;

    /* Loop over the blockSize */
    i = blockSize;

    while (i > 0u)
    {
        /* copy the input sample to the circular buffer */
        circBuffer[wOffset] = *src;

        /* Update the input pointer */
        src += srcInc;

        /* Circularly update wOffset.  Watch out for positive and negative value */
        wOffset += bufferInc;
        if (wOffset >= L)
            wOffset -= L;

        /* Decrement the loop counter */
        i--;
    }

    /* Update the index pointer */
    *writeOffset = wOffset;
}



/**
 * @brief floating-point Circular Read function.
 */
static void ne10_circular_read_float (ne10_int32_t * circBuffer,
    ne10_int32_t L,
    ne10_int32_t * readOffset,
    ne10_int32_t bufferInc,
    ne10_int32_t * dst,
    ne10_int32_t * dst_base,
    ne10_int32_t dst_length,
    ne10_int32_t dstInc,
    ne10_uint32_t blockSize)
{
    ne10_uint32_t i = 0u;
    ne10_int32_t rOffset, dst_end;

    /* Copy the value of Index pointer that points
     * to the current location from where the input samples to be read */
    rOffset = *readOffset;
    dst_end = (ne10_int32_t) (dst_base + dst_length);

    /* Loop over the blockSize */
    i = blockSize;

    while (i > 0u)
    {
        /* copy the sample from the circular buffer to the destination buffer */
        *dst = circBuffer[rOffset];

        /* Update the input pointer */
        dst += dstInc;

        if (dst == (ne10_int32_t *) dst_end)
        {
            dst = dst_base;
        }

        /* Circularly update rOffset.  Watch out for positive and negative value  */
        rOffset += bufferInc;

        if (rOffset >= L)
        {
            rOffset -= L;
        }

        /* Decrement the loop counter */
        i--;
    }

    /* Update the index pointer */
    *readOffset = rOffset;
}


/**
 * @brief Processing function for the floating-point sparse FIR filter.
 * @param[in]  *S          points to an instance of the floating-point sparse FIR structure.
 * @param[in]  *pSrc       points to the block of input data.
 * @param[out] *pDst       points to the block of output data
 * @param[in]  *pScratchIn points to a temporary buffer of size blockSize.
 * @param[in]  blockSize   number of input samples to process per call.
 * @return none.
 */

void ne10_fir_sparse_float_c (ne10_fir_sparse_instance_f32_t * S,
    ne10_float32_t * pSrc,
    ne10_float32_t * pDst,
    ne10_float32_t * pScratchIn,
    ne10_uint32_t blockSize)
{

    ne10_float32_t *pState = S->pState;                 /* State pointer */
    ne10_float32_t *pCoeffs = S->pCoeffs;               /* Coefficient pointer */
    ne10_float32_t *px;                                 /* Scratch buffer pointer */
    ne10_float32_t *py = pState;                        /* Temporary pointers for state buffer */
    ne10_float32_t *pb = pScratchIn;                    /* Temporary pointers for scratch buffer */
    ne10_float32_t *pOut;                               /* Destination pointer */
    ne10_int32_t *pTapDelay = S->pTapDelay;             /* Pointer to the array containing offset of the non-zero tap values. */
    ne10_uint32_t delaySize = S->maxDelay + blockSize;  /* state length */
    ne10_uint16_t numTaps = S->numTaps;                 /* Number of filter coefficients in the filter  */
    ne10_int32_t readIndex;                             /* Read index of the state buffer */
    ne10_uint32_t tapCnt, blkCnt;                       /* loop counters */
    ne10_float32_t coeff = *pCoeffs++;                  /* Read the first coefficient value */



    /* BlockSize of Input samples are copied into the state buffer */
    /* StateIndex points to the starting position to write in the state buffer */
    ne10_circular_write_float ( (ne10_int32_t *) py, delaySize, &S->stateIndex, 1,
                                (ne10_int32_t *) pSrc, 1, blockSize);


    /* Read Index, from where the state buffer should be read, is calculated. */
    readIndex = ( (ne10_int32_t) S->stateIndex - (ne10_int32_t) blockSize) - *pTapDelay++;

    /* Wraparound of readIndex */
    if (readIndex < 0)
    {
        readIndex += (ne10_int32_t) delaySize;
    }

    /* Working pointer for state buffer is updated */
    py = pState;

    /* blockSize samples are read from the state buffer */
    ne10_circular_read_float ( (ne10_int32_t *) py, delaySize, &readIndex, 1,
                               (ne10_int32_t *) pb, (ne10_int32_t *) pb, blockSize, 1,
                               blockSize);

    /* Working pointer for the scratch buffer */
    px = pb;

    /* Working pointer for destination buffer */
    pOut = pDst;


    /* Run the below code for Cortex-M4 and Cortex-M3 */

    /* Loop over the blockSize. Unroll by a factor of 4.
     * Compute 4 Multiplications at a time. */
    blkCnt = blockSize >> 2u;

    while (blkCnt > 0u)
    {
        /* Perform Multiplications and store in destination buffer */
        *pOut++ = *px++ * coeff;
        *pOut++ = *px++ * coeff;
        *pOut++ = *px++ * coeff;
        *pOut++ = *px++ * coeff;

        /* Decrement the loop counter */
        blkCnt--;
    }

    /* If the blockSize is not a multiple of 4,
     * compute the remaining samples */
    blkCnt = blockSize % 0x4u;

    while (blkCnt > 0u)
    {
        /* Perform Multiplications and store in destination buffer */
        *pOut++ = *px++ * coeff;

        /* Decrement the loop counter */
        blkCnt--;
    }

    /* Load the coefficient value and
     * increment the coefficient buffer for the next set of state values */
    coeff = *pCoeffs++;

    /* Read Index, from where the state buffer should be read, is calculated. */
    readIndex = ( (ne10_int32_t) S->stateIndex - (ne10_int32_t) blockSize) - *pTapDelay++;

    /* Wraparound of readIndex */
    if (readIndex < 0)
    {
        readIndex += (ne10_int32_t) delaySize;
    }

    /* Loop over the number of taps. */
    tapCnt = (ne10_uint32_t) numTaps - 1u;

    while (tapCnt > 0u)
    {

        /* Working pointer for state buffer is updated */
        py = pState;

        /* blockSize samples are read from the state buffer */
        ne10_circular_read_float ( (ne10_int32_t *) py, delaySize, &readIndex, 1,
                                   (ne10_int32_t *) pb, (ne10_int32_t *) pb, blockSize, 1,
                                   blockSize);

        /* Working pointer for the scratch buffer */
        px = pb;

        /* Working pointer for destination buffer */
        pOut = pDst;

        /* Loop over the blockSize. Unroll by a factor of 4.
         * Compute 4 MACS at a time. */
        blkCnt = blockSize >> 2u;

        while (blkCnt > 0u)
        {
            /* Perform Multiply-Accumulate */
            *pOut++ += *px++ * coeff;
            *pOut++ += *px++ * coeff;
            *pOut++ += *px++ * coeff;
            *pOut++ += *px++ * coeff;

            /* Decrement the loop counter */
            blkCnt--;
        }

        /* If the blockSize is not a multiple of 4,
         * compute the remaining samples */
        blkCnt = blockSize % 0x4u;

        while (blkCnt > 0u)
        {
            /* Perform Multiply-Accumulate */
            *pOut++ += *px++ * coeff;

            /* Decrement the loop counter */
            blkCnt--;
        }

        /* Load the coefficient value and
         * increment the coefficient buffer for the next set of state values */
        coeff = *pCoeffs++;

        /* Read Index, from where the state buffer should be read, is calculated. */
        readIndex = ( (ne10_int32_t) S->stateIndex -
                      (ne10_int32_t) blockSize) - *pTapDelay++;

        /* Wraparound of readIndex */
        if (readIndex < 0)
        {
            readIndex += (ne10_int32_t) delaySize;
        }

        /* Decrement the tap loop counter */
        tapCnt--;
    }

}


/**
 * @} end of FIR group
 */
