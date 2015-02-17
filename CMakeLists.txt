#
#  Copyright 2011-15 ARM Limited and Contributors.
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
cmake_minimum_required(VERSION 2.6)

project(NE10 C CXX ASM)

option(NE10_BUILD_SHARED "Build NE10 shared libraries" OFF)
option(NE10_BUILD_STATIC "Build NE10 static libraries" ON)
option(NE10_BUILD_EXAMPLES "Build NE10 examples" ON)

#hard float abi
option(NE10_ARM_HARD_FLOAT "Build NE10 for hard float abi" ON)

#unit test options
option(NE10_BUILD_UNIT_TEST "Build NE10 unit test" ON)
if (NE10_BUILD_UNIT_TEST)
    #decide the test is smoke, regression or performance test, only one of
    #three options can be ON! And if you want to run performance test, you
    #should build the releave version, see the BUILD_DEBUG below.
    option(NE10_SMOKE_TEST "Run smoke test" ON)
    option(NE10_REGRESSION_TEST "Run regression test" OFF)
    option(NE10_PERFORMANCE_TEST "Run performance test" OFF)

    option(NE10_DEBUG_TRACE "Print debug trace" OFF)
endif()

#check if proper platform is set.
if((NOT ANDROID_PLATFORM) AND (NOT GNULINUX_PLATFORM) AND (NOT IOS_PLATFORM))
    message(FATAL_ERROR "No platform is defined! see CMakeBuilding.txt under doc for details.")
endif()

if(DEFINED NE10_ANDROID_TARGET_ARCH)
    if(${NE10_ANDROID_TARGET_ARCH} STREQUAL "armv7")
        set(NE10_TARGET_ARCH "armv7")
    else()
        set(NE10_TARGET_ARCH "aarch64")
    endif()
endif()

if(DEFINED NE10_LINUX_TARGET_ARCH)
    if(${NE10_LINUX_TARGET_ARCH} STREQUAL "armv7")
        set(NE10_TARGET_ARCH "armv7")
    else()
        set(NE10_TARGET_ARCH "aarch64")
    endif()
endif()

message("-- Target architecture : ${NE10_TARGET_ARCH}")

#select functionalities to be compiled
if(${NE10_TARGET_ARCH} STREQUAL "armv7")
    # Math module has not been optimized for aarch64.
    option(NE10_ENABLE_MATH "Build math functionalities to NE10" ON)
endif()
option(NE10_ENABLE_DSP "Build dsp functionalities to NE10" ON)
option(NE10_ENABLE_IMGPROC "Build image processing functionalities to NE10" ON)
option(NE10_ENABLE_PHYSICS "Build physics functionalities to NE10" ON)

set(NE10_VERSION 10)

if(BUILD_DEBUG)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -Wall -g -DDEBUG")
    message("-- Building type: DEBUG")
else(BUILD_DEBUG)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -DNDEBUG")
    message("-- Building type: RELEASE")
endif(BUILD_DEBUG)

if(ANDROID_PLATFORM)
    if(NE10_ARM_HARD_FLOAT)
        set(FLOAT_ABI "hard")
    else()
        set(FLOAT_ABI "softfp")
    endif()

    #TODO: Fine tune pic and pie flag for executable, share library and static library.
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --sysroot=${NDK_SYSROOT_PATH} -pie")

    # Adding cflags for armv7. Aarch64 does not need such flags.
    if(${NE10_TARGET_ARCH} STREQUAL "armv7")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mthumb-interwork -mthumb -march=armv7-a -mfloat-abi=${FLOAT_ABI} -mfpu=vfp3")
        if(NE10_ARM_HARD_FLOAT)
            # "--no-warn-mismatch" is needed for linker to suppress linker error about not all functions use VFP register to pass argument, eg.
            #   .../arm-linux-androideabi/bin/ld: error: ..../test-float.o
            #           uses VFP register arguments, output does not
            # There is call convension mismatch between NDK's crt*.o and ne10's object files.
            # crt*.o still uses softfp while ne10's object files use hard floating point.
            # Refer $NDK/tests/device/hard-float/jni/Android.mk for more details.
            set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wl,--no-warn-mismatch")
        endif()
    endif()
    message("-- Loaded toolchain:
    ${CMAKE_C_COMPILER}
    ${CMAKE_CXX_COMPILER}
    ${CMAKE_ASM_COMPILER}")
    message("-- CMAKE_C_FLAGS:
    ${CMAKE_C_FLAGS}")
elseif(GNULINUX_PLATFORM)
    #TODO Development has been moved to android, GNULINUX_PLATFORM is not
    # needed any more. GNULINUX_PLATFORM support needs to be removed.
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mthumb-interwork -mthumb -march=armv7-a -mfpu=vfp3")
    set(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS} -mthumb-interwork -mthumb -march=armv7-a -mfpu=neon")
elseif(IOS_PLATFORM)
    #set minimal target ios version.If not provided this option, Xcode
    #5.0.2 would generate code for iOS 7.0, which would lead runtime
    #error when run on device with earlier iOS version.
    if(NOT MIN_IOS_VER)
      set(MIN_IOS_VER "5.0")
    endif()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -arch armv7 -arch armv7s -mfpu=vfp3 -miphoneos-version-min=" ${MIN_IOS_VER})
    set(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS} -arch armv7 -arch armv7s -mfpu=neon -miphoneos-version-min=" ${MIN_IOS_VER})
    message("-- CFLAGS: ${CMAKE_C_FLAGS}")

    string(REPLACE ";" "" CMAKE_C_FLAGS ${CMAKE_C_FLAGS})
    string(REPLACE ";" "" CMAKE_ASM_FLAGS ${CMAKE_ASM_FLAGS})
endif()

set(CMAKE_CXX_FLAGS ${CMAKE_C_FLAGS})

# Add NE10 library sub-directory.
add_subdirectory(modules)

if(NE10_BUILD_EXAMPLES AND NE10_ENABLE_MATH)
    add_subdirectory(samples)
endif()

if(NE10_BUILD_UNIT_TEST)
    add_subdirectory(test)
endif()

if(ANDROID_PLATFORM AND ANDROID_DEMO)
    add_subdirectory(android/NE10Demo/jni)
endif()

if(IOS_PLATFORM AND IOS_DEMO)
    add_subdirectory(ios)
endif()
