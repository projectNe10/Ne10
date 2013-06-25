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
cmake_minimum_required(VERSION 2.6)

project(NE10 C ASM)

option(NE10_BUILD_SHARED "Build NE10 shared libraries" OFF)
option(NE10_BUILD_STATIC "Build NE10 static libraries" ON)
option(NE10_BUILD_EXAMPLES "Build NE10 examples" ON)

#unit test options
option(NE10_BUILD_UNIT_TEST "Build NE10 unit test" OFF)
if (NE10_BUILD_UNIT_TEST)
    #decide the test is smoke, regression or performance test, only one of three options is ON!
    option(NE10_SMOKE_TEST "Run smoke test" OFF)
    option(NE10_REGRESSION_TEST "Run regression test" OFF)
    option(NE10_PERFORMANCE_TEST "Run performance test" OFF)

    option(NE10_DEBUG_TRACE "Print debug trace" OFF)
endif()

#check if proper platform is set.
if((NOT ANDROID_PLATFORM) AND (NOT GNULINUX_PLATFORM) AND (NOT IOS_PLATFORM))
    message(FATAL_ERROR "No platform is defined! see CMakeBuilding.txt under doc for details.")
endif()

#select functionalities to be compiled
option(NE10_ENABLE_MATH "Build math functionalities to NE10" ON)
option(NE10_ENABLE_DSP "Build dsp functionalities to NE10" ON)
option(NE10_ENABLE_IMGPROC "Build image processing functionalities to NE10" ON)

set(NE10_VERSION 10)

# set complile flags for ARM.
if(ANDROID_PLATFORM)
    set(CMAKE_C_FLAGS "-O2 -mthumb-interwork -march=armv7-a -mfloat-abi=softfp -mfpu=vfp3 --sysroot=${NDK_SYSROOT_PATH}")
    set(CMAKE_ASM_FLAGS "-mthumb-interwork -march=armv7-a -mfloat-abi=softfp -mfpu=neon --sysroot=${NDK_SYSROOT_PATH}")
    message("loaded toolchain:
    ${ANDROID_TOOLCHAIN_PATH}/arm-linux-androideabi-gcc
    ${ANDROID_TOOLCHAIN_PATH}/arm-linux-androideabi-g++
    ${ANDROID_TOOLCHAIN_PATH}/arm-linux-androideabi-as
    ${ANDROID_TOOLCHAIN_PATH}/arm-linux-androideabi-ar
    ${ANDROID_TOOLCHAIN_PATH}/arm-linux-androideabi-ranlib")
    if(ANDROID_DEMO)
      add_subdirectory(android/NE10Demo/jni)
    endif()
elseif(GNULINUX_PLATFORM)
    set(CMAKE_C_FLAGS "-O2 -mthumb-interwork -march=armv7-a -mfloat-abi=softfp -mfpu=vfp3")
    set(CMAKE_ASM_FLAGS "-mthumb-interwork -march=armv7-a -mfloat-abi=softfp -mfpu=neon")
elseif(IOS_PLATFORM)
    set(CMAKE_C_FLAGS "-O2 -arch armv7 -arch armv7s -mfpu=vfp3")
    set(CMAKE_ASM_FLAGS "-arch armv7 -arch armv7s -mfpu=neon")
    if(IOS_DEMO)
        add_subdirectory(ios)
    endif()
endif()

# The NE10 library.
add_subdirectory(modules)

if(NE10_BUILD_EXAMPLES)
    add_subdirectory(samples)
endif()

if(NE10_BUILD_UNIT_TEST)
    add_subdirectory(test)
endif()
