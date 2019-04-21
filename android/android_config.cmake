#
#  Copyright 2013-16 ARM Limited and Contributors.
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

#  Usage:
#   $ export ANDROID_NDK=/absolute/path/of/android-ndk
#   $ mkdir build && cd build
#   $ cmake -DCMAKE_TOOLCHAIN_FILE=path/of/android_config.cmake ..
#   $ make
#
#  Option:
#   - Choose android api level
#     Androd api level can be specified by setting ANDROID_API_LEVEL
#     (defaut: 21).
#   - Choose gcc version by setting ARM_ANDROID_TOOLCHAIN_VERSION
#     (defaut: 4.9. And gcc version must be >=4.6.
#   - Choose target architecture
#     Target architecture can be specified by setting NE10_ANDROID_TARGET_ARCH to
#     aarch64 or armv7. Defaut is armv7.

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_CROSSCOMPILING 1)

set(ANDROID_PLATFORM ON)

if(NOT DEFINED ENV{NE10_ANDROID_TARGET_ARCH})
    set(NE10_ANDROID_TARGET_ARCH "armv7")
else()
    set(NE10_ANDROID_TARGET_ARCH $ENV{NE10_ANDROID_TARGET_ARCH})
endif()

if(${NE10_ANDROID_TARGET_ARCH} STREQUAL "armv7")
    set(NE10_TARGET_ARCH "armv7")
else()
    set(NE10_TARGET_ARCH "aarch64")
endif()

if(NOT DEFINED ENV{ANDROID_NDK})
    message(FATAL_ERROR "Could not find Android NDK. You should set an environment variable: export ANDROID_NDK=/your/path/to/android/ndk")
endif()

if(NOT DEFINED ENV{ANDROID_API_LEVEL})
    set(ANDROID_API_LEVEL 21)
else()
    set(ANDROID_API_LEVEL $ENV{ANDROID_API_LEVEL})
endif()
set(CMAKE_SYSTEM_VERSION ${ANDROID_API_LEVEL})

if(${NE10_ANDROID_TARGET_ARCH} STREQUAL "armv7")
    set(ANDROID_NDK_PLATFORMS_ARCH_SUFFIX "arm")
    set(ANDROID_NDK_TOOLCHAIN_CROSS_PREFIX "armv7a-linux-androideabi")
elseif(${NE10_ANDROID_TARGET_ARCH} STREQUAL "aarch64")
    if(${ANDROID_API_LEVEL} LESS "21")
        message(FATAL_ERROR "Aarch64 target is only availiable under ANDROID_API_LEVEL version 21 and later. Current ANDROID_API_LEVEL is ${ANDROID_API_LEVEL}.")
    endif()
    set(ANDROID_NDK_PLATFORMS_ARCH_SUFFIX "arm64")
    set(ANDROID_NDK_TOOLCHAIN_CROSS_PREFIX "aarch64-linux-android")
else()
    message(FATAL_ERROR "No NE10_ANDROID_TARGET_ARCH is specified, availiable target architectures are: armv7, aarch64.")
endif()

#NDK_SYSROOT_PATH is used in compiler's '--sysroot' flags
set(NDK_SYSROOT_PATH "$ENV{ANDROID_NDK}/platforms/android-${ANDROID_API_LEVEL}/arch-${ANDROID_NDK_PLATFORMS_ARCH_SUFFIX}/")
set(NDK_ISYSROOT_PATH "$ENV{ANDROID_NDK}/sysroot -I$ENV{ANDROID_NDK}/sysroot/usr/include/${ANDROID_NDK_TOOLCHAIN_CROSS_PREFIX}")

if(APPLE)
    #TODO: Check whether this path is correct for aarch64 under mac.
    set(ANDROID_TOOLCHAIN_PATH "$ENV{ANDROID_NDK}/toolchains/llvm/prebuilt/darwin-x86_64/bin")
else()
    set(ANDROID_TOOLCHAIN_PATH "$ENV{ANDROID_NDK}/toolchains/llvm/prebuilt/linux-x86_64/bin")
endif()

#change toolchain name according to your configuration
set(CMAKE_C_COMPILER ${ANDROID_TOOLCHAIN_PATH}/${ANDROID_NDK_TOOLCHAIN_CROSS_PREFIX}${ANDROID_API_LEVEL}-clang)
set(CMAKE_CXX_COMPILER ${ANDROID_TOOLCHAIN_PATH}/${ANDROID_NDK_TOOLCHAIN_CROSS_PREFIX}${ANDROID_API_LEVEL}-clang++)

if(${NE10_ANDROID_TARGET_ARCH} STREQUAL "armv7")
	set(CMAKE_ASM_COMPILER "$ENV{ANDROID_NDK}/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/arm-linux-androideabi/bin/as")
    find_program(CMAKE_AR NAMES "$ENV{ANDROID_NDK}/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/arm-linux-androideabi/bin/ar")
    find_program(CMAKE_RANLIB NAMES "$ENV{ANDROID_NDK}/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/arm-linux-androideabi/bin/ranlib")
else()
    set(CMAKE_ASM_COMPILER ${ANDROID_TOOLCHAIN_PATH}/${ANDROID_NDK_TOOLCHAIN_CROSS_PREFIX}-as)
    find_program(CMAKE_AR NAMES "${ANDROID_TOOLCHAIN_PATH}/${ANDROID_NDK_TOOLCHAIN_CROSS_PREFIX}-ar")
    find_program(CMAKE_RANLIB NAMES "${ANDROID_TOOLCHAIN_PATH}/${ANDROID_NDK_TOOLCHAIN_CROSS_PREFIX}-ranlib")
endif()

# Skip the platform compiler checks for cross compiling
set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)
set(CMAKE_ASM_COMPILER_WORKS 1)
mark_as_advanced(CMAKE_AR)
mark_as_advanced(CMAKE_RANLIB)

if(NE10_ARM_HARD_FLOAT)
    set(FLOAT_ABI "hard")
else()
    set(FLOAT_ABI "softfp")
endif()

#TODO: Fine tune pic and pie flag for executable, share library and static library.
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --sysroot=${NDK_SYSROOT_PATH} -pie")
string(APPEND CMAKE_C_FLAGS " -isysroot ${NDK_ISYSROOT_PATH}")
# set(CMAKE_ASM_FLAGS "")
add_definitions(-D__ANDROID_API__=${ANDROID_API_LEVEL})

# Adding cflags for armv7. Aarch64 does not need such flags.
if(${NE10_TARGET_ARCH} STREQUAL "armv7")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mthumb -march=armv7-a -mfloat-abi=${FLOAT_ABI} -mfpu=vfp3")
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
    set(NE10_ASM_OPTIMIZATION OFF)
endif()
