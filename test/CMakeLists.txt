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

# Define common test variables
set(NE10_TEST_COMMON_SRCS
    ${PROJECT_SOURCE_DIR}/test/src/seatest.c
    ${PROJECT_SOURCE_DIR}/test/src/unit_test_common.c
    ${PROJECT_SOURCE_DIR}/test/src/NE10_random.c
)
if(NE10_BUILD_SHARED)
    set(NE10_TEST_LIBS "NE10_test" "m")
    set(NE10_TEST_OUTPUT_NAME "unit_test_dynamic")
elseif(NE10_BUILD_STATIC)
    set(NE10_TEST_LIBS "NE10" "m")
    set(NE10_TEST_OUTPUT_NAME "unit_test_static")
endif()
if (NE10_TEST_NAME STREQUAL "smoke")
    add_definitions(-DSMOKE_TEST)
elseif (NE10_TEST_NAME STREQUAL "regression")
    add_definitions(-DREGRESSION_TEST)
elseif (NE10_TEST_NAME STREQUAL "performance")
    add_definitions(-DPERFORMANCE_TEST)
endif()
set(NE10_TEST_OUTPUT_NAME "${NE10_TEST_OUTPUT_NAME}_${NE10_TEST_NAME}")
if(GNULINUX_PLATFORM)
    list(APPEND NE10_TEST_LIBS "rt")
endif()
message("-- Building unit test: ${NE10_TEST_NAME}")

if (NE10_DEBUG_TRACE)
    add_definitions(-DDEBUG_TRACE)
endif()

add_definitions("-O3")

# header
include_directories (
    ${PROJECT_SOURCE_DIR}/inc
    ${PROJECT_SOURCE_DIR}/test/include
)

# Define Function Enabling Macros
include(../cmake/FunctionSwitch.cmake)

if(NE10_ENABLE_MATH)
    # Define math test files.
    set(NE10_TEST_MATH_SRCS
        ${PROJECT_SOURCE_DIR}/modules/math/test/test_main.c
        ${PROJECT_SOURCE_DIR}/modules/math/test/test_suite_math.c
    )

    # Math unit tests
    add_executable("NE10_math_${NE10_TEST_OUTPUT_NAME}" ${NE10_TEST_MATH_SRCS} ${NE10_TEST_COMMON_SRCS})
    target_link_libraries (
        "NE10_math_${NE10_TEST_OUTPUT_NAME}"
        ${NE10_TEST_LIBS}
    )
endif()

if(NE10_ENABLE_DSP)
    # Define dsp test files.
    set(NE10_TEST_DSP_SRCS
        ${PROJECT_SOURCE_DIR}/modules/dsp/test/test_main.c
        ${PROJECT_SOURCE_DIR}/modules/dsp/test/test_suite_fft_float32.c
        ${PROJECT_SOURCE_DIR}/modules/dsp/test/test_suite_fft_int32.c
        ${PROJECT_SOURCE_DIR}/modules/dsp/test/test_suite_fft_int16.c
        ${PROJECT_SOURCE_DIR}/modules/dsp/test/test_suite_fir.c
        ${PROJECT_SOURCE_DIR}/modules/dsp/test/test_suite_fir_decimate.c
        ${PROJECT_SOURCE_DIR}/modules/dsp/test/test_suite_fir_interpolate.c
        ${PROJECT_SOURCE_DIR}/modules/dsp/test/test_suite_fir_lattice.c
        ${PROJECT_SOURCE_DIR}/modules/dsp/test/test_suite_fir_sparse.c
        ${PROJECT_SOURCE_DIR}/modules/dsp/test/test_suite_iir.c
    )

    # DSP unit tests
    add_executable("NE10_dsp_${NE10_TEST_OUTPUT_NAME}" ${NE10_TEST_DSP_SRCS} ${NE10_TEST_COMMON_SRCS})
    target_link_libraries (
        "NE10_dsp_${NE10_TEST_OUTPUT_NAME}"
        ${NE10_TEST_LIBS}
    )
    set_target_properties("NE10_dsp_${NE10_TEST_OUTPUT_NAME}" PROPERTIES LINKER_LANGUAGE C)
endif()


if(NE10_ENABLE_IMGPROC)
    # Define imgproc test files.
    set(NE10_TEST_IMGPROC_SRCS
        ${PROJECT_SOURCE_DIR}/modules/imgproc/test/test_main.c
        ${PROJECT_SOURCE_DIR}/modules/imgproc/test/test_suite_resize.c
        ${PROJECT_SOURCE_DIR}/modules/imgproc/test/test_suite_rotate.c
        ${PROJECT_SOURCE_DIR}/modules/imgproc/test/test_suite_boxfilter.c
    )

    # imgproc unit tests
    add_executable("NE10_imgproc_${NE10_TEST_OUTPUT_NAME}" ${NE10_TEST_IMGPROC_SRCS} ${NE10_TEST_COMMON_SRCS})
    target_link_libraries (
        "NE10_imgproc_${NE10_TEST_OUTPUT_NAME}"
        ${NE10_TEST_LIBS}
    )
endif()

if(NE10_ENABLE_PHYSICS)
    # Define physics test files.
    set(NE10_TEST_PHYSICS_SRCS
        ${PROJECT_SOURCE_DIR}/modules/physics/test/test_main.c
        ${PROJECT_SOURCE_DIR}/modules/physics/test/test_suite_physics.c
    )

    # Physics unit tests
    add_executable("NE10_physics_${NE10_TEST_OUTPUT_NAME}" ${NE10_TEST_PHYSICS_SRCS} ${NE10_TEST_COMMON_SRCS})
    target_link_libraries (
        "NE10_physics_${NE10_TEST_OUTPUT_NAME}"
        ${NE10_TEST_LIBS}
    )
endif()
