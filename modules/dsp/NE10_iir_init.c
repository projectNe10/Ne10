/*
 *  Copyright 2012-16 ARM Limited and Contributors.
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
 * NE10 Library : dsp/NE10_iir_init.c
 */
#include "NE10_types.h"
#include <string.h>


/**
 * @brief Initialization function for the floating-point IIR lattice filter.
 * @param[in] *S points to an instance of the floating-point IIR lattice structure.
 * @param[in] numStages number of stages in the filter.
 * @param[in] *pkCoeffs points to the reflection coefficient buffer.  The array is of length numStages.
 * @param[in] *pvCoeffs points to the ladder coefficient buffer.  The array is of length numStages+1.
 * @param[in] *pState points to the state buffer.  The array is of length numStages+blockSize.
 * @param[in] blockSize number of samples to process.
 */

ne10_result_t ne10_iir_lattice_init_float (ne10_iir_lattice_instance_f32_t * S,
        ne10_uint16_t numStages,
        ne10_float32_t * pkCoeffs,
        ne10_float32_t * pvCoeffs,
        ne10_float32_t * pState,
        ne10_uint32_t blockSize)
{
    /* Assign filter taps */
    S->numStages = numStages;

    /* Assign reflection coefficient pointer */
    S->pkCoeffs = pkCoeffs;

    /* Assign ladder coefficient pointer */
    S->pvCoeffs = pvCoeffs;

    /* Clear state buffer and size is always blockSize + numStages */
    memset (pState, 0, (numStages + blockSize) * sizeof (ne10_float32_t));

    /* Assign state pointer */
    S->pState = pState;

    return NE10_OK;
}

/**
 * @} end of IIR_Lattice group
 */
