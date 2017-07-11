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
cmake_minimum_required(VERSION 2.6)

project(NE10 C CXX ASM)

option(NE10_BUILD_SHARED "Build NE10 shared libraries" OFF)
option(NE10_BUILD_STATIC "Build NE10 static libraries" ON)
option(NE10_BUILD_EXAMPLES "Build NE10 examples" ON)

#hard float abi
option(NE10_ARM_HARD_FLOAT "Build NE10 for hard float abi" ON)

#unit test options
option(NE10_BUILD_UNIT_TEST "Build NE10 unit test" OFF)
if (NE10_BUILD_UNIT_TEST)
    #decide the test is smoke, regression or performance test, only one of
    #three options can be ON! And if you want to run performance test, you
    #should build the releave version, see the BUILD_DEBUG below.
    option(NE10_SMOKE_TEST "Run smoke test" OFF)
    option(NE10_REGRESSION_TEST "Run regression test" OFF)
    option(NE10_PERFORMANCE_TEST "Run performance test" ON)

    option(NE10_DEBUG_TRACE "Print debug trace" OFF)
endif()

#check if proper platform is set.
if((NOT ANDROID_PLATFORM) AND (NOT GNULINUX_PLATFORM) AND (NOT IOS_PLATFORM))
    message(FATAL_ERROR "No platform is defined! see doc/building.md for build instructions.")
endif()

if(DEFINED NE10_ANDROID_TARGET_ARCH)
    if(${NE10_ANDROID_TARGET_ARCH} STREQUAL "armv7")
        set(NE10_TARGET_ARCH "armv7")
    else()
        set(NE10_TARGET_ARCH "aarch64")
    endif()
endif()

if(NOT DEFINED NE10_LINUX_TARGET_ARCH AND DEFINED ENV{NE10_LINUX_TARGET_ARCH})
    set(NE10_LINUX_TARGET_ARCH $ENV{NE10_LINUX_TARGET_ARCH})
endif()
if(DEFINED NE10_LINUX_TARGET_ARCH)
    if(${NE10_LINUX_TARGET_ARCH} STREQUAL "armv7")
        set(NE10_TARGET_ARCH "armv7")
    elsif(${NE10_LINUX_TARGET_ARCH} STREQUAL "aarch64")
        set(NE10_TARGET_ARCH "aarch64")
    else()
        # Catch all. Will only build plain C variant of library, with no intrinsics
        set(NE10_TARGET_ARCH "other")
    endif()
endif()

if(DEFINED NE10_IOS_TARGET_ARCH)
    if(${NE10_IOS_TARGET_ARCH} STREQUAL "armv7")
        set(NE10_TARGET_ARCH "armv7")
    elsif(${NE10_LINUX_TARGET_ARCH} STREQUAL "aarch64")
        set(NE10_TARGET_ARCH "aarch64")
    else()
        # Catch all. Will only build plain C variant of library, with no intrinsics
        set(NE10_TARGET_ARCH "other")
    endif()
endif()

if(NOT DEFINED NE10_IOS_TARGET_ARCH AND NOT DEFINED NE10_LINUX_TARGET_ARCH AND NOT DEFINED NE10_ANDROID_TARGET_ARCH)
    set(NE10_TARGET_ARCH "other")
endif()
message("-- Target architecture:" ${NE10_TARGET_ARCH})

#select functionalities to be compiled
if("${NE10_TARGET_ARCH}" STREQUAL "armv7")
    # Math module has only been optimized for armv7
    option(NE10_ENABLE_MATH "Build math functionalities to NE10" ON)
    # Physics module has only been optimized for armv7
    option(NE10_ENABLE_PHYSICS "Build physics functionalities to NE10" ON)
endif()
option(NE10_ENABLE_DSP "Build dsp functionalities to NE10" ON)
option(NE10_ENABLE_IMGPROC "Build image processing functionalities to NE10" ON)

set(NE10_VERSION 10)

if(BUILD_DEBUG)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-strict-aliasing -O0 -DDEBUG -g -Wall -Wno-unused-but-set-variable")
    message("-- Building type: DEBUG")
else(BUILD_DEBUG)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-strict-aliasing -O2 -DNDEBUG")
    message("-- Building type: RELEASE")
endif(BUILD_DEBUG)

# By default, we compile c intrinsic.
set(NE10_ASM_OPTIMIZATION off)

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
        # Turn on asm optimization for Android on ARM v7.
        set(NE10_ASM_OPTIMIZATION on)
    endif()
    message("-- Loaded toolchain:
    ${CMAKE_C_COMPILER}
    ${CMAKE_CXX_COMPILER}
    ${CMAKE_ASM_COMPILER}")
    message("-- CMAKE_C_FLAGS:
    ${CMAKE_C_FLAGS}")
elseif(GNULINUX_PLATFORM)
    if("${NE10_TARGET_ARCH}" STREQUAL "armv7")
      set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mthumb-interwork -mthumb -march=armv7-a -mfpu=vfp3 -funsafe-math-optimizations")
      set(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS} -mthumb-interwork -mthumb -march=armv7-a -mfpu=neon")
      # Turn on asm optimization for Linux on ARM v7.
      set(NE10_ASM_OPTIMIZATION on)
    endif()
elseif(IOS_PLATFORM)
    #set minimal target ios version.If not provided this option, Xcode
    #5.0.2 would generate code for iOS 7.0, which would lead runtime
    #error when run on device with earlier iOS version.
    if(NOT MIN_IOS_VER)
      set(MIN_IOS_VER "5.0")
    endif()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -miphoneos-version-min=" ${MIN_IOS_VER})
    set(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS} -mfpu=neon -miphoneos-version-min=" ${MIN_IOS_VER})

    if(${NE10_TARGET_ARCH} STREQUAL "armv7")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mfpu=vfp3 -arch armv7 -arch armv7s")
        set(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS} -arch armv7 -arch armv7s")
    else()
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -arch arm64")
    endif()

    message("-- CFLAGS: ${CMAKE_C_FLAGS}")

    string(REPLACE ";" "" CMAKE_C_FLAGS ${CMAKE_C_FLAGS})
    string(REPLACE ";" "" CMAKE_ASM_FLAGS ${CMAKE_ASM_FLAGS})
endif()

set(CMAKE_CXX_FLAGS ${CMAKE_C_FLAGS})
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99")

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
