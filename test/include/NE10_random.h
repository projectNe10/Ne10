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
 * NE10 Library : test/include/NE10_random.h
 */


#ifndef __NE10_RANDOM__
#define __NE10_RANDOM__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <float.h>
#include <math.h>
#include <assert.h>


// used for creating different instances of random number generators with different seeds and states
typedef struct
{
    // these are used as internal values, please do not change them directly
    uint32_t _private_m_A              ;// = 1103515245L; // a, must be 0 < _A < _M
    uint32_t _private_m_C              ;// = 12345L; // c, must be 0 < _C < _M
    uint32_t _private_m_X_NM1          ;// = 0; // X(n-1), at first this value is the seed or the start value
} NE10_rng_t;

typedef struct
{
    // these are used as internal values, please do not change them directly
    // there are three separate seeds for 1) the sign, 2) the exponent, 3) and the fraction bits.
    NE10_rng_t _private_m_rngs[3];
} NE10_float_rng_t;

// a random number generator that generates IEEE 754 float numbers
// NAN_OR_INF is to check whether the value is a NAN or an INF
#define NAN_OR_INF (0xFF << 23)
#define IS_NAN_OR_INF(x) ( ((x & NAN_OR_INF) == NAN_OR_INF)?1:0 )

#define EXPONENT_MASK 0x807FFFFF
#define IS_SUBNORMAL(x) ( ((x & EXPONENT_MASK) == x)?1:0 )

// generic functions
extern void NE10_rng_init_g (NE10_rng_t *rng, uint32_t seed);

extern uint32_t NE10_rng_next_g (NE10_rng_t *rng);

extern const uint32_t NE10_rng_max_g (NE10_rng_t *rng);

extern void NE10_rng_init (uint32_t seed);

extern uint32_t NE10_rng_next();

extern const uint32_t NE10_rng_max();

extern void NE10_float_rng_init_g (NE10_float_rng_t* float_rng, uint32_t seed);

extern float NE10_float_rng_next_g (NE10_float_rng_t* float_rng);

extern float NE10_float_rng_max_g (NE10_float_rng_t* float_rng);

extern void NE10_float_rng_init (uint32_t seed);

extern float NE10_float_rng_next();

extern float NE10_float_rng_max();

extern void NE10_float_rng_limit_init (uint32_t seed);

extern float NE10_float_rng_limit_next();

extern float NE10_float_rng_limit_max();

extern void NE10_float_rng_limit_gt1_init (uint32_t seed);

extern float NE10_float_rng_limit_gt1_next();

extern float NE10_float_rng_limit_gt1_max();

#endif // NE10_RANDOM

