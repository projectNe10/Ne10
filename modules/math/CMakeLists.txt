#
#  Copyright 2011-16 ARM Limited and Contributors.
#  All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions are met:
#    * Redistributions of source code must retain the above copyright
#      notice, this list of conditions and the following disclaimer.
#    * Redistributions in binary form must reproduce the above copyright
#      notice, this list of conditions and the following disclaimer in the
#      documentation and/or other materials provided with the distribution.
#    * Neither the name of ARM Limited nor the
#      names of its contributors may be used to endorse or promote products
#      derived from this software without specific prior written permission.
#
#  THIS SOFTWARE IS PROVIDED BY ARM LIMITED AND CONTRIBUTORS "AS IS" AND
#  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
#  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#  DISCLAIMED. IN NO EVENT SHALL ARM LIMITED AND CONTRIBUTORS BE LIABLE FOR ANY
#  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
#  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
#  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
#  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
#  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
#  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

# Add math C files.
set(NE10_MATH_C_SRCS
    ${PROJECT_SOURCE_DIR}/modules/math/NE10_abs.c
    ${PROJECT_SOURCE_DIR}/modules/math/NE10_addc.c
    ${PROJECT_SOURCE_DIR}/modules/math/NE10_add.c
    ${PROJECT_SOURCE_DIR}/modules/math/NE10_divc.c
    ${PROJECT_SOURCE_DIR}/modules/math/NE10_div.c
    ${PROJECT_SOURCE_DIR}/modules/math/NE10_len.c
    ${PROJECT_SOURCE_DIR}/modules/math/NE10_mlac.c
    ${PROJECT_SOURCE_DIR}/modules/math/NE10_mla.c
    ${PROJECT_SOURCE_DIR}/modules/math/NE10_mulc.c
    ${PROJECT_SOURCE_DIR}/modules/math/NE10_mul.c
    ${PROJECT_SOURCE_DIR}/modules/math/NE10_normalize.c
    ${PROJECT_SOURCE_DIR}/modules/math/NE10_rsbc.c
    ${PROJECT_SOURCE_DIR}/modules/math/NE10_setc.c
    ${PROJECT_SOURCE_DIR}/modules/math/NE10_subc.c
    ${PROJECT_SOURCE_DIR}/modules/math/NE10_sub.c
    ${PROJECT_SOURCE_DIR}/modules/math/NE10_dot.c
    ${PROJECT_SOURCE_DIR}/modules/math/NE10_cross.c
    ${PROJECT_SOURCE_DIR}/modules/math/NE10_addmat.c
    ${PROJECT_SOURCE_DIR}/modules/math/NE10_submat.c
    ${PROJECT_SOURCE_DIR}/modules/math/NE10_mulmat.c
    ${PROJECT_SOURCE_DIR}/modules/math/NE10_mulcmatvec.c
    ${PROJECT_SOURCE_DIR}/modules/math/NE10_detmat.c
    ${PROJECT_SOURCE_DIR}/modules/math/NE10_invmat.c
    ${PROJECT_SOURCE_DIR}/modules/math/NE10_transmat.c
    ${PROJECT_SOURCE_DIR}/modules/math/NE10_identitymat.c
)

# Add math intrinsic NEON files.
set(NE10_MATH_INTRINSIC_SRCS
    ${PROJECT_SOURCE_DIR}/modules/math/NE10_addc.neon.c
    ${PROJECT_SOURCE_DIR}/modules/math/NE10_divc.neon.c
    ${PROJECT_SOURCE_DIR}/modules/math/NE10_mlac.neon.c
    ${PROJECT_SOURCE_DIR}/modules/math/NE10_mulc.neon.c
    ${PROJECT_SOURCE_DIR}/modules/math/NE10_rsbc.neon.c
    ${PROJECT_SOURCE_DIR}/modules/math/NE10_setc.neon.c
    ${PROJECT_SOURCE_DIR}/modules/math/NE10_subc.neon.c
    ${PROJECT_SOURCE_DIR}/modules/math/NE10_addmat.neon.c
    ${PROJECT_SOURCE_DIR}/modules/math/NE10_submat.neon.c
)

# Tell CMake these files need to be compiled with "-mfpu=neon"
if("${NE10_TARGET_ARCH}" STREQUAL "armv7")
  foreach(intrinsic_file ${NE10_MATH_INTRINSIC_SRCS})
    set_source_files_properties(${intrinsic_file} PROPERTIES COMPILE_FLAGS "-mfpu=neon" )
  endforeach(intrinsic_file)
endif()

set(NE10_MATH_NEON_SRCS
    ${PROJECT_SOURCE_DIR}/modules/math/NE10_abs.neon.s
    ${PROJECT_SOURCE_DIR}/modules/math/NE10_add.neon.s
    ${PROJECT_SOURCE_DIR}/modules/math/NE10_div.neon.s
    ${PROJECT_SOURCE_DIR}/modules/math/NE10_len.neon.s
    ${PROJECT_SOURCE_DIR}/modules/math/NE10_mla.neon.s
    ${PROJECT_SOURCE_DIR}/modules/math/NE10_mul.neon.s
    ${PROJECT_SOURCE_DIR}/modules/math/NE10_normalize.neon.s
    ${PROJECT_SOURCE_DIR}/modules/math/NE10_sub.neon.s
    ${PROJECT_SOURCE_DIR}/modules/math/NE10_dot.neon.s
    ${PROJECT_SOURCE_DIR}/modules/math/NE10_cross.neon.s
    ${PROJECT_SOURCE_DIR}/modules/math/NE10_mulmat.neon.s
    ${PROJECT_SOURCE_DIR}/modules/math/NE10_mulcmatvec.neon.s
    ${PROJECT_SOURCE_DIR}/modules/math/NE10_detmat.neon.s
    ${PROJECT_SOURCE_DIR}/modules/math/NE10_invmat.neon.s
    ${PROJECT_SOURCE_DIR}/modules/math/NE10_transmat.neon.s
    ${PROJECT_SOURCE_DIR}/modules/math/NE10_identitymat.neon.s
)

if(IOS_PLATFORM)
    convert_gas(NE10_MATH_NEON_SRCS NE10_MATH_IOS_NEON_SRCS)
    #message(STATUS "convert_gas:${NE10_MATH_IOS_NEON_SRCS}")
    set(NE10_MATH_NEON_SRCS ${NE10_MATH_IOS_NEON_SRCS})
endif(IOS_PLATFORM)

set_file_to_c(NE10_MATH_NEON_SRCS)

# Add math init files.
set(NE10_MATH_INIT_SRCS
    ${PROJECT_SOURCE_DIR}/modules/math/NE10_init_math.c
)

add_library( NE10_math STATIC
    ${NE10_MATH_INIT_SRCS}
    ${NE10_MATH_C_SRCS}
    ${NE10_MATH_INTRINSIC_SRCS}
    ${NE10_MATH_NEON_SRCS}
)

if(NOT NE10_TEST_NAME STREQUAL "none")
    include(${PROJECT_SOURCE_DIR}/cmake/AddTestSuite.cmake)

    # Define math test files.
    set(NE10_TEST_MATH_SRCS
        ${PROJECT_SOURCE_DIR}/modules/math/test/test_main.c
        ${PROJECT_SOURCE_DIR}/modules/math/test/test_suite_math.c
    )

    NE10_ADD_TEST_MODULE("math" NE10_TEST_NAME NE10_TEST_MATH_SRCS)
endif()
