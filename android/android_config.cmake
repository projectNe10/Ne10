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

#  Usage:
#   $ export ANDROID_NDK=/absolute/path/of/android-ndk
#   $ mkdir build && cd build
#   $ cmake -DCMAKE_TOOLCHAIN_FILE=path/of/android_config.cmake ..
#   $ make
#
#  Option:
#   you can also specify android api level and gcc version by
#   setting ANDROID_API_LEVEL(default:14) and
#   ARM_ANDROID_TOOLCHAIN_VERSION(default:4.6) environment variable.
#
#  Note:
#  arm linux androideabi version must be >=4.6, namely, gcc version must be >=4.6

set(CMAKE_SYSTEM_NAME Linux)

set(ANDROID_PLATFORM ON)

if(ANDROID_PLATFORM)
    set(ANDROID_DEMO ON)
endif()

if(ANDROID_DEMO)
    add_definitions(-DNE10_ANDROID_DEMO)
endif()

if(DEFINED ENV{ANDROID_NDK})
    if(NOT DEFINED ENV{ANDROID_API_LEVEL})
        set(ANDROID_API_LEVEL 14)
    else()
        set(ANDROID_API_LEVEL $ENV{ANDROID_API_LEVEL})
    endif()

    if(NOT DEFINED ENV{ARM_ANDROID_TOOLCHAIN_VERSION})
        set(ARM_ANDROID_TOOLCHAIN_VERSION 4.6)
    else()
        set(ARM_ANDROID_TOOLCHAIN_VERSION $ENV{ARM_ANDROID_TOOLCHAIN_VERSION})
    endif()
    #NDK_SYSROOT_PATH is used in compiler's '--sysroot' flags
    set(NDK_SYSROOT_PATH "$ENV{ANDROID_NDK}/platforms/android-${ANDROID_API_LEVEL}/arch-arm/")

    set(ANDROID_TOOLCHAIN_PATH "$ENV{ANDROID_NDK}/toolchains/arm-linux-androideabi-${ARM_ANDROID_TOOLCHAIN_VERSION}/prebuilt/linux-x86_64/bin")

    #change toolchain name according to your configuration
    set(CMAKE_C_COMPILER ${ANDROID_TOOLCHAIN_PATH}/arm-linux-androideabi-gcc)
    set(CMAKE_CXX_COMPILER ${ANDROID_TOOLCHAIN_PATH}/arm-linux-androideabi-g++)
    set(CMAKE_ASM_COMPILER ${ANDROID_TOOLCHAIN_PATH}/arm-linux-androideabi-as)

    # Skip the platform compiler checks for cross compiling
    set(CMAKE_CXX_COMPILER_WORKS TRUE)
    set(CMAKE_C_COMPILER_WORKS TRUE)
    set(CMAKE_ASM_COMPILER_WORKS TRUE)

    find_program(CMAKE_AR NAMES "${ANDROID_TOOLCHAIN_PATH}/arm-linux-androideabi-ar")
    mark_as_advanced(CMAKE_AR)

    find_program(CMAKE_RANLIB NAMES "${ANDROID_TOOLCHAIN_PATH}/arm-linux-androideabi-ranlib")
    mark_as_advanced(CMAKE_RANLIB)

else()
    message(FATAL_ERROR "Could not find Android NDK. You should set an environment variable: export ANDROID_NDK=/your/path/to/android/ndk")
endif()
