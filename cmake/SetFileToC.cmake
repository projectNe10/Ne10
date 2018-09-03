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

#Tell CMake these files need to go to the C compiler
function(set_file_to_c files)
  set(FLAGS
    "-x assembler-with-cpp -mfpu=neon -Wa,-I${PROJECT_SOURCE_DIR}/inc -Wa,-I${PROJECT_SOURCE_DIR}/common -Wa,-I${PROJECT_SOURCE_DIR}/modules/math" )
  foreach(file ${${files}})
    set_property (SOURCE ${file} PROPERTY LANGUAGE C)
    set_source_files_properties(
      ${file} PROPERTIES COMPILE_FLAGS
      ${FLAGS}
      )
  endforeach(file)
endfunction()
