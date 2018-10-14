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
#   $ mkdir build && cd build
#   $ cmake -DCMAKE_TOOLCHAIN_FILE=path/of/GNUlinux_config.cmake ..
#   $ make
#
#  Option:
#   - Choose target architecture
#     Target architecture can be specified by setting NE10_LINUX_TARGET_ARCH to
#     armv7 or aarch64 (Not done yet). Defaut is armv7.

set(GNULINUX_PLATFORM ON)
set(CMAKE_SYSTEM_NAME "Linux")
set(CMAKE_SYSTEM_PROCESSOR "arm")

if(NOT DEFINED ENV{NE10_LINUX_TARGET_ARCH})
   set(NE10_LINUX_TARGET_ARCH "armv7")
else()
   set(NE10_LINUX_TARGET_ARCH $ENV{NE10_LINUX_TARGET_ARCH})
endif()

if(NE10_LINUX_TARGET_ARCH STREQUAL "armv7")
   set(CMAKE_C_COMPILER arm-linux-gnueabihf-gcc)
   set(CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++)
   set(CMAKE_ASM_COMPILER arm-linux-gnueabihf-as)
   find_program(CMAKE_AR NAMES "arm-linux-gnueabihf-ar")
   find_program(CMAKE_RANLIB NAMES "arm-linux-gnueabihf-ranlib")
elseif(NE10_LINUX_TARGET_ARCH STREQUAL "aarch64")
   set(CMAKE_C_COMPILER aarch64-linux-gnu-gcc)
   set(CMAKE_CXX_COMPILER aarch64-linux-gnu-g++)
   set(CMAKE_ASM_COMPILER aarch64-linux-gnu-as)
   find_program(CMAKE_AR NAMES "aarch64-linux-gnu-ar")
   find_program(CMAKE_RANLIB NAMES "aarch64-linux-gnu-ranlib")
endif()

mark_as_advanced(CMAKE_AR)
mark_as_advanced(CMAKE_RANLIB)
