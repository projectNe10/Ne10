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
 * NE10 Library : test/src/unit_test_common.c
 */

#include "unit_test_common.h"

void FILL_FLOAT_ARRAY (ne10_float32_t *arr, ne10_uint32_t count)
{
    ne10_uint32_t i = 0;

    sleep (1);

    NE10_float_rng_init (time (NULL));

    for (i = 0; i < count; i++)
    {
        arr[i] = NE10_float_rng_next();
    }
}

void FILL_FLOAT_ARRAY_LIMIT (ne10_float32_t *arr, ne10_uint32_t count)
{
    ne10_uint32_t i = 0;

    sleep (1);

    NE10_float_rng_limit_init (time (NULL));

    for (i = 0; i < count; i++)
    {
        arr[ i ] = NE10_float_rng_limit_next();
    }
}

void FILL_FLOAT_ARRAY_LIMIT_GT1 (ne10_float32_t *arr, ne10_uint32_t count)
{
    ne10_uint32_t i = 0;

    sleep (1);

    NE10_float_rng_limit_gt1_init (time (NULL));

    for (i = 0; i < count; i++)
    {
        arr[ i ] = NE10_float_rng_limit_gt1_next();
    }
}

// this function checks whether the difference between two ne10_float32_tvalues is within the acceptable error range
ne10_int32_t EQUALS_FLOAT (ne10_float32_t fa, ne10_float32_t fb , ne10_uint32_t err)
{
    union
    {
        ne10_int32_t          vi;
        ne10_float32_t       vf;
    } conv1, conv2;

    ne10_uint32_t ui1, ui2;

    if (fa == fb) return 1;   // if identical, then return TRUE

    conv1.vf = fa;
    conv2.vf = fb;

    if ( (conv1.vi & NAN_OR_INF) == NAN_OR_INF)
    {
        //fprintf( stderr, "HINT: The 1st floating-pone10_int32_t value is either \'Not a number\' or \'Infinity\'. " );
        return 0; // INF or NAN, unacceptable return FALSE
    }

    if ( (conv2.vi & NAN_OR_INF) == NAN_OR_INF)
    {
        //fprintf( stderr, "HINT: The 1st floating-pone10_int32_t value is either \'Not a number\' or \'Infinity\'. " );
        return 0; // INF or NAN, unacceptable return FALSE
    }

    ne10_int32_t cut1 = conv1.vi & SIGNBIT_MASK; // drop the sign bit - i.e. the left most bit
    ne10_int32_t cut2 = conv2.vi & SIGNBIT_MASK;

    if ( (cut1 & EXPONENT_MASK) == cut1)
    {
        cut1 = 0;    // zero out subnormal ne10_float32_tvalues
    }
    if ( (cut2 & EXPONENT_MASK) == cut2)
    {
        cut2 = 0;    // zero out subnormal ne10_float32_tvalues
    }

    memcpy (&ui1,  &fa, sizeof (ne10_float32_t));
    memcpy (&ui2,  &fb, sizeof (ne10_float32_t));

    if (abs (cut1 - cut2) > err)    // this is the log() of the actual error
    {
        // then we have an unacceptable error

        // report an unacceptable error
        //fprintf( stderr, "HINT: %e (0x%04X) != %e (0x%04X) ", fa, ui1, fb, ui2 );

        return 0;
    }

    if (fb * fa < 0.0f)
    {

        //fprintf( stderr, "HINT: %e (0x%04X) is the opposite of %e (0x%04X) ", fa, ui1, fb, ui2 );

        return 0;
    }

    return 1; // acceptable, return TRUE
}

ne10_float32_t ARRAY_GUARD_SIG[ARRAY_GUARD_LEN] = { 10.0f, 20.0f, 30.0f, 40.0f };
// this function adds a ARRAY_GUARD_LEN signature to the begining and the end of an array, minimum acceptable size for the array is 2*ARRAY_GUARD_LEN.
ne10_int32_t GUARD_ARRAY (ne10_float32_t* array, ne10_uint32_t array_length)
{
    ne10_float32_t* the_array = array - ARRAY_GUARD_LEN;
    memcpy (the_array, ARRAY_GUARD_SIG, sizeof (ARRAY_GUARD_SIG));
    the_array = array + array_length;
    memcpy (the_array, ARRAY_GUARD_SIG, sizeof (ARRAY_GUARD_SIG));
    return 1;
}

// this function returns TRUE if the signature matches the ARRAY_GUARD_SIGguard and returns FALSE otherwise
ne10_int32_t CHECK_ARRAY_GUARD (ne10_float32_t* array, ne10_uint32_t array_length)
{
    ne10_float32_t* the_array = array - ARRAY_GUARD_LEN;
    ne10_int32_t i;
    for (i = 0; i < ARRAY_GUARD_LEN; i++)
    {
        if (! EQUALS_FLOAT (the_array[i], ARRAY_GUARD_SIG[i], ERROR_MARGIN_SMALL))
        {
            fprintf (stderr, " ERROR: prefix array guard signature is wrong. \n");
            return 0; // Match not found, return FALSE
        }
    }

    the_array = array + array_length;
    for (i = 0; i < ARRAY_GUARD_LEN; i++)
    {
        if (! EQUALS_FLOAT (the_array[i], ARRAY_GUARD_SIG[i], ERROR_MARGIN_SMALL))
        {
            fprintf (stderr, " ERROR: suffix array guard signature is wrong. \n");
            return 0; // Match not found, return FALSE
        }
    }

    return 1;
}
/**
 * @brief  Caluclation of SNR
 * @param  ne10_float32_t*  Pointer to the reference buffer
 * @param  ne10_float32_t*  Pointer to the test buffer
 * @param  ne10_uint32_t    total number of samples
 * @return ne10_float32_t   SNR
 * The function Caluclates signal to noise ratio for the reference output
 * and test output
 */

ne10_float32_t CAL_SNR_FLOAT32 (ne10_float32_t *pRef, ne10_float32_t *pTest, ne10_uint32_t buffSize)
{
    ne10_float32_t EnergySignal = 0.0, EnergyError = 0.0;
    ne10_uint32_t i;
    ne10_float32_t SNR;

    for (i = 0; i < buffSize; i++)
    {
        EnergySignal += pRef[i] * pRef[i];
        EnergyError += (pRef[i] - pTest[i]) * (pRef[i] - pTest[i]);
    }
    SNR = 10 * log10 (EnergySignal / EnergyError);
    return (SNR);

}

