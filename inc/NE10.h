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
   \mainpage Welcome to Ne10 Documentation!
   *
   *
   *\par Introduction
   *
   * Ne10 (http://projectne10.github.com/Ne10/) is a library of the most commonly used functions that have been heavily
   * optimized for ARM-based CPUs with NEON. These functions provide a consistent
   * well tested behavior that can be easily incorporated into applications enabling
   * developers to get the most out of the ARM V7/NEON without arduous assembly coding.
   * Ne10 is usable as a 'drop and go' pre-built library or as a set of modular functions
   * that can be incorporated in a more modular "pick and mix" form where binary size might
   * be an issue.
   *
   * The following figure illustrates the basic concepts of "What's Ne10"
   *\image html ne10_library.png "Ne10 Library Description"
   *
   *\par Top-Level Overview
   * When you checkout Ne10, you will notice a number of directories. These directories are as follows:
   * <pre>
   * ├── android
   * │   └── Android reference files
   * ├── build
   * │   └── directory for build-related files
   * ├── common
   * │   └── directory for common header, table and macro definition files
   * ├── doc
   * │   └── directory for documentations
   * ├── inc
   * │   └── directory for functions'heaeder files
   * ├── modules
   * │   ├── dsp
   * │   │   ├── @link groupDSPs dsp module@endlink that provides a set of signal processing functions, such as complex/real FFT/IFFT, FIR and IIR
   * │   │   └── test
   * │   │       └──  directory for test files
   * │   ├── math
   * │   │   ├── @link groupMaths math module@endlink that provides a set of vector/matrix algebra functions
   * │   │   └── test
   * │   │       └──  directory for test files
   * ├── samples
   * │   └── @link groupSamples sample code@endlink
   * ├── test
   * │   ├── directory for test framework
   * ├── tools
   * │   ├── directory for tools such as Cformatter, doxygen, etc
   * </pre>
   *
   *\par Modules Description
   * Ne10 has a modular structure, which means that the package includes several shared or static libraries.
   * Currently, the following modules are available or in plan:
   *
   * - @link groupMaths Math Functions@endlink
   * - @link groupDSPs Signal Processing Functions@endlink
   * - @link groupIMGPROCs Image Processing Functions@endlink
   * - Physics functions
   * - Image Processing functions
   * - Others
   *
   *\par Usage
   *
   * Ne10 library provides directly and indirectly function call. you could check the @link groupSamples sample code@endlink for details
   *
   *\par Build
   *
   * See CMakeBuilding.txt file in the "doc" folder
   *
   *\par Code formatter
   *
   * See Formatter.txt file in the "doc" folder
   *
   *\par License
   *
   * The Ne10 is provided free of charge by ARM Limited and licensed under both New BSD License
   * (http://opensource.org/licenses/BSD-3-Clause) and Apache 2.0 (http://www.apache.org/licenses/LICENSE-2.0).
   * You could also check the LICENSE file in "doc" directory
   */


/**
 * @defgroup groupMaths Math Functions
 *
 *
 * This set of functions provide vector/matrix algebra functions that include
 * add, sub, multiply, div and so on. Currently, only the float (single precision)
 * data type is supported.
 */

/**
 * @defgroup groupDSPs Signal Processing Functions
 *
 *
 * This set of functions provide some commonly used functions in signal processing,
 * such as complex/real FFT/IFFT, FIR and IIR. Currently, only the float (single precision)
 * data type is supported.
 */

/**
 * @defgroup groupIMGPROCs Image Processing Functions
 *
 *
 * This set of functions provide some commonly used functions in image processing,
 * such as image scale, image rotate.
 */

/**
 * @defgroup groupSamples Sample Functions
 *
 *
 * This set of functions provide some sample functions.
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
#include "NE10_imgproc.h"

#ifdef __cplusplus
}
#endif

#endif
