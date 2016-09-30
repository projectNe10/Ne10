/*
 *  Copyright 2011-15 ARM Limited and Contributors.
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
 * NE10 Library : inc/NE10.h
 */

/**
 \mainpage Welcome to Ne10 Documentation!
 *
 *
 *\par Introduction
 *
 * [Ne10](http://projectne10.github.com/Ne10/) is a library of common, useful functions
 * that have been heavily optimized for ARM-based CPUs equipped with
 * [NEON](http://www.arm.com/products/processors/technologies/neon.php) SIMD capabilities.
 * These functions provide consistent, well-tested behavior that can be easily
 * incorporated into applications, enabling developers to get the most out of ARMv7/NEON
 * without arduous assembly programming. Ne10 is structured in a modular fashion such
 * that it can be used as a convenient "drop and go" pre-built library, but also allows
 * the programmer to "pick and mix" modules as appropriate for a particular application.
 *
 *\image html ne10_library.png "Ne10 Library Description"
 *
 *\par Top-Level Overview
 *
 * The Ne10 directory structure is roughly as follows:
 * <pre>
 * ├── android
 * │   └── Android reference files
 * ├── build
 * │   └── directory for build-related files
 * ├── common
 * │   └── directory for common header, table and macro definition files
 * ├── doc
 * │   └── directory for documentation
 * ├── inc
 * │   └── directory for function header files
 * ├── modules
 * │   ├── dsp
 * │   │   ├── @link groupDSPs dsp module@endlink that provides a set of signal processing functions, such as real/complex FFT/IFFT, FIR and IIR
 * │   │   └── test
 * │   │       └──  directory for test files
 * │   ├── imgproc
 * │   │   ├── @link groupIMGPROCs imgproc module@endlink that provides a set of image processing functions, such as image resize and rotate
 * │   │   └── test
 * │   │       └──  directory for test files
 * │   ├── math
 * │   │   ├── @link groupMaths math module@endlink that provides a set of vector/matrix algebra functions
 * │   │   └── test
 * │   │       └──  directory for test files
 * │   ├── physics
 * │   │   ├── @link groupPhysics physics module@endlink that provides a set of collision detection functions
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
 *\par Modules
 *
 * Ne10's modular structure means that it actually consists of several smaller
 * libraries. Currently, these include:
 *
 * - @link groupMaths Math Functions@endlink
 * - @link groupDSPs Signal Processing Functions@endlink
 * - @link groupIMGPROCs Image Processing Functions@endlink
 * - @link groupPhysics Physics Functions@endlink
 *
 *\par Usage
 *
 * Ne10's function call interface can be understood by reading the relevant documentation
 * pages, or by examining sets of provided @link groupSamples sample code@endlink.
 *
 *\par Building Ne10
 *
 * See BuildingNe10.txt in the "doc" folder.
 *
 *\par Automated Code Formatting
 *
 * See Formatter.txt in the "doc" folder.
 *
 *\par License
 *
 * Ne10 is provided free of charge by ARM Limited and Contributors, and is licensed
 * under the New BSD License (http://opensource.org/licenses/BSD-3-Clause). See the LICENSE
 * file in the "doc" directory for details.
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
 * @defgroup groupPhysics Physics Functions
 *
 *
 * This set of functions provide some APIs used for collision detection,
 * such as compute AABB, caculate relative velocity and apply contact impulse.
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

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "NE10_types.h"
#include "NE10_macros.h"
#include "NE10_init.h"
#include "NE10_math.h"
#include "NE10_dsp.h"
#include "NE10_imgproc.h"
#include "NE10_physics.h"

#ifdef __cplusplus
}
#endif

#endif
