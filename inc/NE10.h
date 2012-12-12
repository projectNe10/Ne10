/*
 *  Copyright 2011-12 ARM Limited
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
 * NE10 Library : inc/NE10.h
 */

/**
   \mainpage Ne10 Software Library
   *
   *
   * <b>Introduction</b>
   *
   * Ne10 is a library of the most commonly used functions that have been heavily
   * optimized for ARM-based CPUs with NEON. These functions provide a consistent
   * well tested behavior that can be easily incorporated into applications enabling
   * developers to get the most out of the ARM V7/NEON without arduous assembly coding.
   * Ne10 is usable as a 'drop and go' pre-built library or as a set of modular functions
   * that can be incorporated in a more modular "pick and mix" form where binary size might
   * be an issue.
   *
   * The Ne10 components are:
   *
   * - math functions
   * - DSP functions
   * - Physics functions
   * - Image Processing functions
   * - Others
   *
   *
   * <b>License</b>
   *
   * The Ne10 is provided free of charge by ARM Limited and licensed under New BSD license.
   */

#ifndef NE10_H
#define NE10_H

#ifdef __cplusplus
extern "C" {
#endif

#include "NE10_types.h"
#include "NE10_init.h"
#include "NE10_math.h"
#include "NE10_dsp.h"

#ifdef __cplusplus
}
#endif

#endif
