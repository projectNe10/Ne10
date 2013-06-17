#
#  Copyright 2013 ARM Limited
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

# we run performance test in Android Demo
    add_definitions(-DPERFORMANCE_TEST)

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

# Define math test files.
set(NE10_TEST_MATH_SRCS
    ${PROJECT_SOURCE_DIR}/modules/math/test/test_suite_math.c
)

# Define dsp test files.
set(NE10_TEST_DSP_SRCS
    ${PROJECT_SOURCE_DIR}/modules/dsp/test/test_main.c
    ${PROJECT_SOURCE_DIR}/modules/dsp/test/test_suite_cfft.c
    ${PROJECT_SOURCE_DIR}/modules/dsp/test/test_suite_rfft.c
    ${PROJECT_SOURCE_DIR}/modules/dsp/test/test_suite_fir.c
    ${PROJECT_SOURCE_DIR}/modules/dsp/test/test_suite_fir_decimate.c
    ${PROJECT_SOURCE_DIR}/modules/dsp/test/test_suite_fir_interpolate.c
    ${PROJECT_SOURCE_DIR}/modules/dsp/test/test_suite_fir_lattice.c
    ${PROJECT_SOURCE_DIR}/modules/dsp/test/test_suite_fir_sparse.c
    ${PROJECT_SOURCE_DIR}/modules/dsp/test/test_suite_iir.c
)

add_library(NE10_test_demo SHARED
    ${NE10_TEST_MATH_SRCS}
    ${NE10_TEST_COMMON_SRCS}
    ${NE10_TEST_DSP_SRCS}
    NE10_test_demo.c)

#add the static NE10 to target
target_link_libraries(NE10_test_demo
    NE10
    m
)

#install NE10_test_demo library to android demo apk library directory.
install(TARGETS NE10_test_demo
    DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../libs/armeabi/)
