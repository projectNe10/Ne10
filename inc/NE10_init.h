/*
 *  Copyright 2011-12 ARM Limited
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

#include "NE10.h"

#ifndef NE10_init_H
#define NE10_init_H

#ifdef __cplusplus
extern "C" {
#endif

/*!
    This routine returns NE10_OK if the running platform supports NEON, otherwise it returns NE10_ERR
 */
extern ne10_result_t ne10_HasNEON();

/*!
    This routine initializes all the function pointers.
 */
extern ne10_result_t ne10_init();

/*!
    This routine initializes all the math function pointers defined in "NE10_math.h" with pointers to ARM NEON or ARM VFP implementations.
 */
extern ne10_result_t ne10_init_math(ne10_int32_t is_NEON_available);
extern ne10_result_t ne10_init_dsp (ne10_int32_t is_NEON_available);

#ifdef __cplusplus
}
#endif

#endif
