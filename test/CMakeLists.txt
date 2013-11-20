#
#  Copyright 2011-13 ARM Limited
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
#  DISCLAIMED. IN NO EVENT SHALL ARM LIMITED BE LIABLE FOR ANY
#  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
#  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
#  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
#  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
#  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
#  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

#add definition for regression, smoke or regression test
if (NE10_SMOKE_TEST)
    add_definitions(-DSMOKE_TEST)
elseif (NE10_REGRESSION_TEST)
    add_definitions(-DREGRESSION_TEST)
elseif (NE10_PERFORMANCE_TEST)
    add_definitions(-DPERFORMANCE_TEST)
endif()

if (NE10_DEBUG_TRACE)
    add_definitions(-DDEBUG_TRACE)
endif()

add_definitions("-O0") # use -O0 to avoid compiler bug

# header
include_directories (
    ${PROJECT_SOURCE_DIR}/inc
    ${PROJECT_SOURCE_DIR}/test/include
)

# Define common test files.
set(NE10_TEST_COMMON_SRCS
    ${PROJECT_SOURCE_DIR}/test/src/seatest.c
    ${PROJECT_SOURCE_DIR}/test/src/unit_test_common.c
    ${PROJECT_SOURCE_DIR}/test/src/NE10_random.c
)

if(NE10_ENABLE_MATH)
    # Define math test files.
    set(NE10_TEST_MATH_SRCS
        ${PROJECT_SOURCE_DIR}/modules/math/test/test_main.c
        ${PROJECT_SOURCE_DIR}/modules/math/test/test_suite_math.c
    )

    if(NE10_BUILD_STATIC)
        add_executable(NE10_math_unit_test_static ${NE10_TEST_MATH_SRCS} ${NE10_TEST_COMMON_SRCS})
        if(ANDROID_PLATFORM OR IOS_PLATFORM)
            target_link_libraries (
                NE10_math_unit_test_static
                NE10
                m
            )
        elseif(GNULINUX_PLATFORM)
            target_link_libraries (
                NE10_math_unit_test_static
                NE10
                m
                rt
            )
        endif()

        if(NE10_SMOKE_TEST)
            set_target_properties(NE10_math_unit_test_static PROPERTIES
                OUTPUT_NAME "NE10_math_unit_test_smoke"
            )
        elseif (NE10_REGRESSION_TEST)
            set_target_properties(NE10_math_unit_test_static PROPERTIES
                OUTPUT_NAME "NE10_math_unit_test_regression"
            )
        elseif (NE10_PERFORMANCE_TEST)
            set_target_properties(NE10_math_unit_test_static PROPERTIES
                OUTPUT_NAME "NE10_math_unit_test_performance"
            )
        endif()
    endif()
endif()

if(NE10_ENABLE_DSP)
    # Define dsp test files.
    set(NE10_TEST_DSP_SRCS
        ${PROJECT_SOURCE_DIR}/modules/dsp/test/test_main.c
        ${PROJECT_SOURCE_DIR}/modules/dsp/test/test_suite_cfft.c
        ${PROJECT_SOURCE_DIR}/modules/dsp/test/test_suite_rfft.c
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
    #TODO add dynamic version of test
    if(NE10_BUILD_STATIC)
        add_executable(NE10_dsp_unit_test_static ${NE10_TEST_DSP_SRCS} ${NE10_TEST_COMMON_SRCS})
        if(ANDROID_PLATFORM OR IOS_PLATFORM)
            target_link_libraries (
                NE10_dsp_unit_test_static
                NE10
                m
            )
        elseif(GNULINUX_PLATFORM)
            target_link_libraries (
                NE10_dsp_unit_test_static
                NE10
                m
                rt
            )
        endif()
        if(NE10_SMOKE_TEST)
            set_target_properties(NE10_dsp_unit_test_static PROPERTIES
                OUTPUT_NAME "NE10_dsp_unit_test_smoke"
            )
        elseif (NE10_REGRESSION_TEST)
            set_target_properties(NE10_dsp_unit_test_static PROPERTIES
                OUTPUT_NAME "NE10_dsp_unit_test_regression"
            )
        elseif (NE10_PERFORMANCE_TEST)
            set_target_properties(NE10_dsp_unit_test_static PROPERTIES
                OUTPUT_NAME "NE10_dsp_unit_test_performance"
            )
        endif()

    endif()
endif()


if(NE10_ENABLE_IMGPROC)
    # Define imgproc test files.
    set(NE10_TEST_IMGPROC_SRCS
        ${PROJECT_SOURCE_DIR}/modules/imgproc/test/test_main.c
        ${PROJECT_SOURCE_DIR}/modules/imgproc/test/test_suite_resize.c
        ${PROJECT_SOURCE_DIR}/modules/imgproc/test/test_suite_rotate.c
    )

    if(NE10_BUILD_STATIC)
        add_executable(NE10_imgproc_unit_test_static ${NE10_TEST_IMGPROC_SRCS} ${NE10_TEST_COMMON_SRCS})
        if(ANDROID_PLATFORM OR IOS_PLATFORM)
            target_link_libraries (
                NE10_imgproc_unit_test_static
                NE10
                m
            )
        elseif(GNULINUX_PLATFORM)
            target_link_libraries (
                NE10_imgproc_unit_test_static
                NE10
                m
                rt
            )
        endif()

        if(NE10_SMOKE_TEST)
            set_target_properties(NE10_imgproc_unit_test_static PROPERTIES
                OUTPUT_NAME "NE10_imgproc_unit_test_smoke"
            )
        elseif (NE10_REGRESSION_TEST)
            set_target_properties(NE10_imgproc_unit_test_static PROPERTIES
                OUTPUT_NAME "NE10_imgproc_unit_test_regression"
            )
        elseif (NE10_PERFORMANCE_TEST)
            set_target_properties(NE10_imgproc_unit_test_static PROPERTIES
                OUTPUT_NAME "NE10_imgproc_unit_test_performance"
            )
        endif()
    endif()
endif()

