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
 * NE10 Library : dsp/NE10_iir_init.c
 */
#include "NE10_types.h"


/**
 * @brief Initialization function for the floating-point IIR lattice filter.
 * @param[in] *S points to an instance of the floating-point IIR lattice structure.
 * @param[in] numStages number of stages in the filter.
 * @param[in] *pkCoeffs points to the reflection coefficient buffer.  The array is of length numStages.
 * @param[in] *pvCoeffs points to the ladder coefficient buffer.  The array is of length numStages+1.
 * @param[in] *pState points to the state buffer.  The array is of length numStages+blockSize.
 * @param[in] blockSize number of samples to process.
 * @return none.
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
