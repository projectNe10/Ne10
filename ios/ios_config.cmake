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
#   $ mkdir build && cd build
#   $ cmake -DCMAKE_TOOLCHAIN_FILE=path/of/ios_config.cmake ..
#   $ make
#
#   Note:
#   you may specify your ios developer path by setting IOS_DEVELOPER_PATH
#   environment variable by:
#       export IOS_DEVELOPER_PATH=/absolute/path/of/ios/developer/path,
#   IOS_DEVELOPER_PATH is set to /Applications/Xcode.app/Contents/Developer by
#   default.

set(IOS_PLATFORM ON)
set(IOS_DEMO ON)

if(NOT DEFINED ENV{IOS_DEVELOPER_PATH})
    set(ENV{IOS_DEVELOPER_PATH} "/Applications/Xcode.app/Contents/Developer")
endif()

if(NOT EXISTS $ENV{IOS_DEVELOPER_PATH})
    message(FATAL_ERROR "Could not find directory $ENV{IOS_DEVELOPER_PATH}."
      "Please check whether environment variable IOS_DEVELOPER is set"
      "correctly. (default is set to"
      "\"/Applications/Xcode.app/Contents/Developer\")")
endif()

set(IOS_TOOLCHAIN_PATH
  "$ENV{IOS_DEVELOPER_PATH}/Toolchains/XcodeDefault.xctoolchain/usr/bin")

set(CMAKE_C_COMPILER ${IOS_TOOLCHAIN_PATH}/clang)
set(CMAKE_CXX_COMPILER ${IOS_TOOLCHAIN_PATH}/clang++)
set(CMAKE_ASM_COMPILER ${IOS_TOOLCHAIN_PATH}/clang)

# Skip the platform compiler checks for cross compiling
set(CMAKE_CXX_COMPILER_WORKS TRUE)
set(CMAKE_C_COMPILER_WORKS TRUE)
set(CMAKE_ASM_COMPILER TRUE)

set(CMAKE_IOS_SDK_PATH
  "$ENV{IOS_DEVELOPER_PATH}/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS6.1.sdk/")

set(CMAKE_OSX_SYSROOT ${CMAKE_IOS_SDK_PATH})
set(CMAKE_FIND_ROOT_PATH ${CMAKE_IOS_SDK_PATH})

#only search the iOS sdks, not the remainder of the host filesystem
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
