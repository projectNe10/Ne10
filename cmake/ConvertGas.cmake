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

if(IOS_PLATFORM)
  # because ios's ARM assembler(Clang) doesn't support GNU as(GAS)'s assembly
  # well, we convert all GAS assembly to Clang assembly here.
  function(convert_gas gas_files ios_files)
    #message(STATUS "in convert_gas function:${${gas_files}}")
    foreach(gas_file ${${gas_files}})
      string(REGEX REPLACE
        ".*\(NE10_.*\).neon.s"
        "${NE10_BINARY_DIR}/modules/CMakeFiles/NE10.dir/\\1_ios.neon.s"
        ios_file
        ${gas_file})
      list(APPEND ios_files_tmp ${ios_file})

      #call a script to convert each assembly file in GAS to Clang
      add_custom_command(
        OUTPUT ${ios_file}
        COMMAND ${NE10_SOURCE_DIR}/tools/gas2ios_convert.py
        ${gas_file} ${ios_file}
        DEPENDS ${gas_file}
        )
    endforeach()

    #return value by setting the variable to PARENT_SCOPE
    set(${ios_files} ${ios_files_tmp} PARENT_SCOPE)
  endfunction()
endif(IOS_PLATFORM)

