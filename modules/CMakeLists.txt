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

# Define Function Enabling Macros
include(../cmake/FunctionSwitch.cmake)

if(NE10_ARM_HARD_FLOAT)
    add_definitions(-DNE10_ENABLE_HF)
endif()
# Define files.
set(NE10_INIT_SRCS ${PROJECT_SOURCE_DIR}/modules/NE10_init.c)
set(NE10_C_SRCS )
set(NE10_INTRINSIC_SRCS )
set(NE10_NEON_SRCS )

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

if(NE10_ENABLE_MATH)
    #enable NE10_init_math
    add_definitions(-DNE10_ENABLE_MATH)
    # Add math C files.
    set(NE10_MATH_C_SRCS
        ${PROJECT_SOURCE_DIR}/modules/math/NE10_abs.c
        ${PROJECT_SOURCE_DIR}/modules/math/NE10_addc.c
        ${PROJECT_SOURCE_DIR}/modules/math/NE10_add.c
        ${PROJECT_SOURCE_DIR}/modules/math/NE10_divc.c
        ${PROJECT_SOURCE_DIR}/modules/math/NE10_div.c
        ${PROJECT_SOURCE_DIR}/modules/math/NE10_len.c
        ${PROJECT_SOURCE_DIR}/modules/math/NE10_mlac.c
        ${PROJECT_SOURCE_DIR}/modules/math/NE10_mla.c
        ${PROJECT_SOURCE_DIR}/modules/math/NE10_mulc.c
        ${PROJECT_SOURCE_DIR}/modules/math/NE10_mul.c
        ${PROJECT_SOURCE_DIR}/modules/math/NE10_normalize.c
        ${PROJECT_SOURCE_DIR}/modules/math/NE10_rsbc.c
        ${PROJECT_SOURCE_DIR}/modules/math/NE10_setc.c
        ${PROJECT_SOURCE_DIR}/modules/math/NE10_subc.c
        ${PROJECT_SOURCE_DIR}/modules/math/NE10_sub.c
        ${PROJECT_SOURCE_DIR}/modules/math/NE10_dot.c
        ${PROJECT_SOURCE_DIR}/modules/math/NE10_cross.c
        ${PROJECT_SOURCE_DIR}/modules/math/NE10_addmat.c
        ${PROJECT_SOURCE_DIR}/modules/math/NE10_submat.c
        ${PROJECT_SOURCE_DIR}/modules/math/NE10_mulmat.c
        ${PROJECT_SOURCE_DIR}/modules/math/NE10_mulcmatvec.c
        ${PROJECT_SOURCE_DIR}/modules/math/NE10_detmat.c
        ${PROJECT_SOURCE_DIR}/modules/math/NE10_invmat.c
        ${PROJECT_SOURCE_DIR}/modules/math/NE10_transmat.c
        ${PROJECT_SOURCE_DIR}/modules/math/NE10_identitymat.c
    )

    # Add math intrinsic NEON files.
    set(NE10_MATH_INTRINSIC_SRCS
        ${PROJECT_SOURCE_DIR}/modules/math/NE10_addc.neon.c
        ${PROJECT_SOURCE_DIR}/modules/math/NE10_divc.neon.c
        ${PROJECT_SOURCE_DIR}/modules/math/NE10_mlac.neon.c
        ${PROJECT_SOURCE_DIR}/modules/math/NE10_mulc.neon.c
        ${PROJECT_SOURCE_DIR}/modules/math/NE10_rsbc.neon.c
        ${PROJECT_SOURCE_DIR}/modules/math/NE10_setc.neon.c
        ${PROJECT_SOURCE_DIR}/modules/math/NE10_subc.neon.c
        ${PROJECT_SOURCE_DIR}/modules/math/NE10_addmat.neon.c
        ${PROJECT_SOURCE_DIR}/modules/math/NE10_submat.neon.c
    )

    # Tell CMake these files need to be compiled with "-mfpu=neon"
    if("${NE10_TARGET_ARCH}" STREQUAL "armv7")
      foreach(intrinsic_file ${NE10_MATH_INTRINSIC_SRCS})
        set_source_files_properties(${intrinsic_file} PROPERTIES COMPILE_FLAGS "-mfpu=neon" )
      endforeach(intrinsic_file)
    endif()

    set(NE10_MATH_NEON_SRCS
        ${PROJECT_SOURCE_DIR}/modules/math/NE10_abs.neon.s
        ${PROJECT_SOURCE_DIR}/modules/math/NE10_add.neon.s
        ${PROJECT_SOURCE_DIR}/modules/math/NE10_div.neon.s
        ${PROJECT_SOURCE_DIR}/modules/math/NE10_len.neon.s
        ${PROJECT_SOURCE_DIR}/modules/math/NE10_mla.neon.s
        ${PROJECT_SOURCE_DIR}/modules/math/NE10_mul.neon.s
        ${PROJECT_SOURCE_DIR}/modules/math/NE10_normalize.neon.s
        ${PROJECT_SOURCE_DIR}/modules/math/NE10_sub.neon.s
        ${PROJECT_SOURCE_DIR}/modules/math/NE10_dot.neon.s
        ${PROJECT_SOURCE_DIR}/modules/math/NE10_cross.neon.s
        ${PROJECT_SOURCE_DIR}/modules/math/NE10_mulmat.neon.s
        ${PROJECT_SOURCE_DIR}/modules/math/NE10_mulcmatvec.neon.s
        ${PROJECT_SOURCE_DIR}/modules/math/NE10_detmat.neon.s
        ${PROJECT_SOURCE_DIR}/modules/math/NE10_invmat.neon.s
        ${PROJECT_SOURCE_DIR}/modules/math/NE10_transmat.neon.s
        ${PROJECT_SOURCE_DIR}/modules/math/NE10_identitymat.neon.s
    )
if(IOS_PLATFORM)
    convert_gas(NE10_MATH_NEON_SRCS NE10_MATH_IOS_NEON_SRCS)
    #message(STATUS "convert_gas:${NE10_MATH_IOS_NEON_SRCS}")
    set_file_to_c(NE10_MATH_IOS_NEON_SRCS)
else(IOS_PLATFORM)
    set_file_to_c(NE10_MATH_NEON_SRCS)
endif(IOS_PLATFORM)

    # Add math init files.
    set(NE10_MATH_INIT_SRCS
        ${PROJECT_SOURCE_DIR}/modules/math/NE10_init_math.c
    )

    # Add math files
    set(NE10_INIT_SRCS ${NE10_INIT_SRCS} ${NE10_MATH_INIT_SRCS})
    set(NE10_C_SRCS ${NE10_C_SRCS} ${NE10_MATH_C_SRCS})
    set(NE10_INTRINSIC_SRCS ${NE10_INTRINSIC_SRCS} ${NE10_MATH_INTRINSIC_SRCS})
    if(IOS_PLATFORM)
        set(NE10_NEON_SRCS ${NE10_NEON_SRCS} ${NE10_MATH_IOS_NEON_SRCS})
    else(IOS_PLATFORM)
        set(NE10_NEON_SRCS ${NE10_NEON_SRCS} ${NE10_MATH_NEON_SRCS})
    endif(IOS_PLATFORM)

endif()

if(NE10_ENABLE_DSP)
    #enable NE10_init_dsp
    add_definitions(-DNE10_ENABLE_DSP)

    set(NE10_DSP_INTRINSIC_SRCS )
    set(NE10_DSP_NEON_SRCS )

    # Add dsp C files.
    set(NE10_DSP_C_SRCS
        ${PROJECT_SOURCE_DIR}/common/NE10_mask_table.c
        ${PROJECT_SOURCE_DIR}/modules/dsp/NE10_fft.c
        ${PROJECT_SOURCE_DIR}/modules/dsp/NE10_fft_float32.c
        ${PROJECT_SOURCE_DIR}/modules/dsp/NE10_fft_generic_float32.c
        ${PROJECT_SOURCE_DIR}/modules/dsp/NE10_fft_generic_int32.cpp
        ${PROJECT_SOURCE_DIR}/modules/dsp/NE10_rfft_float32.c
        ${PROJECT_SOURCE_DIR}/modules/dsp/NE10_fft_int32.c
        ${PROJECT_SOURCE_DIR}/modules/dsp/NE10_fft_int16.c
        ${PROJECT_SOURCE_DIR}/modules/dsp/NE10_fir.c
        ${PROJECT_SOURCE_DIR}/modules/dsp/NE10_fir_init.c
        ${PROJECT_SOURCE_DIR}/modules/dsp/NE10_iir.c
        ${PROJECT_SOURCE_DIR}/modules/dsp/NE10_iir_init.c
    )

    if("${NE10_TARGET_ARCH}" STREQUAL "armv7")
        if(IOS_PLATFORM)
        else()
            # Add dsp NEON files.
            set(NE10_DSP_NEON_SRCS
                ${NE10_DSP_NEON_SRCS}
                ${PROJECT_SOURCE_DIR}/modules/dsp/NE10_fir.neon.s
                ${PROJECT_SOURCE_DIR}/modules/dsp/NE10_iir.neon.s
                )
        endif()
    endif()

    if(NE10_ASM_OPTIMIZATION)
        set(NE10_DSP_NEON_SRCS
            ${NE10_DSP_NEON_SRCS}
            ${PROJECT_SOURCE_DIR}/modules/dsp/NE10_fft_float32.neon.s
            ${PROJECT_SOURCE_DIR}/modules/dsp/NE10_fft_int32.neon.s
            ${PROJECT_SOURCE_DIR}/modules/dsp/NE10_fft_int16.neon.s
        )
        set(NE10_DSP_INTRINSIC_SRCS
            ${NE10_DSP_INTRINSIC_SRCS}
            ${PROJECT_SOURCE_DIR}/modules/dsp/NE10_fft_float32.neon.c
            ${PROJECT_SOURCE_DIR}/modules/dsp/NE10_fft_int32.neon.c
            ${PROJECT_SOURCE_DIR}/modules/dsp/NE10_fft_int16.neon.c
        )
    else()
        add_definitions(-DNE10_UNROLL_LEVEL=1)
        set(NE10_DSP_INTRINSIC_SRCS
            ${NE10_DSP_INTRINSIC_SRCS}
            ${PROJECT_SOURCE_DIR}/modules/dsp/NE10_fft_float32.neonintrinsic.c
            ${PROJECT_SOURCE_DIR}/modules/dsp/NE10_fft_int32.neonintrinsic.c
            ${PROJECT_SOURCE_DIR}/modules/dsp/NE10_fft_int16.neonintrinsic.c
            ${PROJECT_SOURCE_DIR}/modules/dsp/NE10_rfft_float32.neonintrinsic.c
            )
    endif()

    # Add dsp intrinsic NEON files.
    set(NE10_DSP_INTRINSIC_SRCS
        ${NE10_DSP_INTRINSIC_SRCS}
        ${PROJECT_SOURCE_DIR}/modules/dsp/NE10_fft_generic_float32.neonintrinsic.cpp
        ${PROJECT_SOURCE_DIR}/modules/dsp/NE10_fft_generic_int32.neonintrinsic.cpp
    )

    if("${NE10_TARGET_ARCH}" STREQUAL "armv7")
        foreach(intrinsic_file ${NE10_DSP_INTRINSIC_SRCS})
            set_source_files_properties(${intrinsic_file} PROPERTIES COMPILE_FLAGS "-mfpu=neon" )
        endforeach(intrinsic_file)
    endif()

if(IOS_PLATFORM)
    set_file_to_c(NE10_DSP_NEON_SRCS)
else(IOS_PLATFORM)
    set_file_to_c(NE10_DSP_NEON_SRCS)
endif(IOS_PLATFORM)
    # Add dsp init files.
    set(NE10_DSP_INIT_SRCS
        ${PROJECT_SOURCE_DIR}/modules/dsp/NE10_init_dsp.c
    )
    # Add dsp files
    set(NE10_INIT_SRCS ${NE10_INIT_SRCS} ${NE10_DSP_INIT_SRCS})
    set(NE10_C_SRCS ${NE10_C_SRCS} ${NE10_DSP_C_SRCS})
    set(NE10_INTRINSIC_SRCS ${NE10_INTRINSIC_SRCS} ${NE10_DSP_INTRINSIC_SRCS})
    if(IOS_PLATFORM)
      set(NE10_NEON_SRCS ${NE10_NEON_SRCS} ${NE10_DSP_IOS_NEON_SRCS})
    else(IOS_PLATFORM)
      set(NE10_NEON_SRCS ${NE10_NEON_SRCS} ${NE10_DSP_NEON_SRCS})
    endif(IOS_PLATFORM)
endif()

if(NE10_ENABLE_IMGPROC)
    #enable NE10_init_imgproc
    add_definitions(-DNE10_ENABLE_IMGPROC)
    # Add image processing C files.
    set(NE10_IMGPROC_C_SRCS
        ${PROJECT_SOURCE_DIR}/common/NE10_mask_table.c
        ${PROJECT_SOURCE_DIR}/modules/imgproc/NE10_resize.c
        ${PROJECT_SOURCE_DIR}/modules/imgproc/NE10_rotate.c
        ${PROJECT_SOURCE_DIR}/modules/imgproc/NE10_boxfilter.c
    )

    if("${NE10_TARGET_ARCH}" STREQUAL "armv7")
        # Add image processing NEON files.
        set(NE10_IMGPROC_NEON_SRCS
            ${PROJECT_SOURCE_DIR}/modules/imgproc/NE10_rotate.neon.s
            )
    endif()
        # Add image intrinsic NEON files.
    set(NE10_IMGPROC_INTRINSIC_SRCS
        ${PROJECT_SOURCE_DIR}/modules/imgproc/NE10_resize.neon.c
        ${PROJECT_SOURCE_DIR}/modules/imgproc/NE10_boxfilter.neon.c
    )
    # Tell CMake these files need to be compiled with "-mfpu=neon"
    if("${NE10_TARGET_ARCH}" STREQUAL "armv7")
      foreach(intrinsic_file ${NE10_IMGPROC_INTRINSIC_SRCS})
        set_source_files_properties(${intrinsic_file} PROPERTIES COMPILE_FLAGS "-mfpu=neon" )
      endforeach(intrinsic_file)
    endif()
    # Add image processing init files.
    set(NE10_IMGPROC_INIT_SRCS
        ${PROJECT_SOURCE_DIR}/modules/imgproc/NE10_init_imgproc.c
    )

if(IOS_PLATFORM)
    convert_gas(NE10_IMGPROC_NEON_SRCS NE10_IMGPROC_IOS_NEON_SRCS)
    set_file_to_c(NE10_IMGPROC_IOS_NEON_SRCS)
else(IOS_PLATFORM)
    set_file_to_c(NE10_IMGPROC_NEON_SRCS)
endif(IOS_PLATFORM)
    # Add image processing files
    set(NE10_INIT_SRCS ${NE10_INIT_SRCS} ${NE10_IMGPROC_INIT_SRCS})
    set(NE10_C_SRCS ${NE10_C_SRCS} ${NE10_IMGPROC_C_SRCS})
    set(NE10_INTRINSIC_SRCS ${NE10_INTRINSIC_SRCS} ${NE10_IMGPROC_INTRINSIC_SRCS})
    if(IOS_PLATFORM)
      set(NE10_NEON_SRCS ${NE10_NEON_SRCS} ${NE10_IMGPROC_IOS_NEON_SRCS})
    else(IOS_PLATFORM)
      set(NE10_NEON_SRCS ${NE10_NEON_SRCS} ${NE10_IMGPROC_NEON_SRCS})
    endif(IOS_PLATFORM)
endif(NE10_ENABLE_IMGPROC)

if(NE10_ENABLE_PHYSICS)
    #enable NE10_init_physics
    add_definitions(-DNE10_ENABLE_PHYSICS)
    # Add physics C files.
    set(NE10_PHYSICS_C_SRCS
        ${PROJECT_SOURCE_DIR}/modules/physics/NE10_physics.c
    )

    if("${NE10_TARGET_ARCH}" STREQUAL "armv7")
        # Add physics NEON files.
        set(NE10_PHYSICS_NEON_SRCS
            ${PROJECT_SOURCE_DIR}/modules/physics/NE10_physics.neon.s
            )
    endif()
    # Add physics NEON files.
    set(NE10_PHYSICS_INTRINSIC_SRCS
        ${PROJECT_SOURCE_DIR}/modules/physics/NE10_physics.neon.c
    )
    # Tell CMake these files need to be compiled with "-mfpu=neon"
    if("${NE10_TARGET_ARCH}" STREQUAL "armv7")
      foreach(intrinsic_file ${NE10_PHYSICS_INTRINSIC_SRCS})
        set_source_files_properties(${intrinsic_file} PROPERTIES COMPILE_FLAGS "-mfpu=neon" )
      endforeach(intrinsic_file)
    endif()
    # Add physics init files.
    set(NE10_PHYSICS_INIT_SRCS
        ${PROJECT_SOURCE_DIR}/modules/physics/NE10_init_physics.c
    )

if(IOS_PLATFORM)
    convert_gas(NE10_PHYSICS_NEON_SRCS NE10_PHYSICS_IOS_NEON_SRCS)
    set_file_to_c(NE10_PHYSICS_IOS_NEON_SRCS)
else(IOS_PLATFORM)
    set_file_to_c(NE10_PHYSICS_NEON_SRCS)
endif(IOS_PLATFORM)
    # Add physics files
    set(NE10_INIT_SRCS ${NE10_INIT_SRCS} ${NE10_PHYSICS_INIT_SRCS})
    set(NE10_C_SRCS ${NE10_C_SRCS} ${NE10_PHYSICS_C_SRCS})
    set(NE10_INTRINSIC_SRCS ${NE10_INTRINSIC_SRCS} ${NE10_PHYSICS_INTRINSIC_SRCS})
    if(IOS_PLATFORM)
      set(NE10_NEON_SRCS ${NE10_NEON_SRCS} ${NE10_PHYSICS_IOS_NEON_SRCS})
    else(IOS_PLATFORM)
      set(NE10_NEON_SRCS ${NE10_NEON_SRCS} ${NE10_PHYSICS_NEON_SRCS})
    endif(IOS_PLATFORM)
endif(NE10_ENABLE_PHYSICS)

include_directories (
    ${PROJECT_SOURCE_DIR}/inc
    ${PROJECT_SOURCE_DIR}/common
)

if(NE10_BUILD_STATIC OR ANDROID_PLATFORM OR IOS_DEMO)
    add_library( NE10 STATIC
        ${NE10_C_SRCS}
        ${NE10_INTRINSIC_SRCS}
        ${NE10_NEON_SRCS}
        ${NE10_INIT_SRCS}
    )
    set_target_properties(NE10 PROPERTIES
        CLEAN_DIRECT_OUTPUT 1
        VERSION ${NE10_VERSION}
    )

    target_link_libraries(NE10 stdc++)

    set_target_properties(NE10 PROPERTIES
        LINKER_LANGUAGE C
    )

    # If the CMake version supports it, attach header directory information
    # to the targets for when we are part of a parent build (ie being pulled
    # in via add_subdirectory() rather than being a standalone build).
    if (DEFINED CMAKE_VERSION AND NOT "${CMAKE_VERSION}" VERSION_LESS "2.8.11")
        target_include_directories(NE10 INTERFACE "${PROJECT_SOURCE_DIR}/inc")
	target_include_directories(NE10 INTERFACE "${PROJECT_SOURCE_DIR}/common")
    endif()

  if(IOS_DEMO)
    install(TARGETS NE10
      DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../ios/NE10Demo/libs/)
  endif(IOS_DEMO)
endif()

if(NE10_BUILD_SHARED)

    add_library( NE10_shared SHARED
        ${NE10_C_SRCS}
        ${NE10_INTRINSIC_SRCS}
        ${NE10_NEON_SRCS}
        ${NE10_INIT_SRCS}
    )

    target_link_libraries(NE10_shared m)

    set_target_properties(NE10_shared PROPERTIES
        OUTPUT_NAME "NE10"
        CLEAN_DIRECT_OUTPUT 1
        VERSION ${NE10_VERSION}
        LINKER_LANGUAGE C
    )

    add_library( NE10_test SHARED
        ${NE10_C_SRCS}
        ${NE10_INTRINSIC_SRCS}
        ${NE10_NEON_SRCS}
        ${NE10_INIT_SRCS}
    )

    set_target_properties(NE10_test PROPERTIES
        OUTPUT_NAME "NE10_test"
        CLEAN_DIRECT_OUTPUT 1
        VERSION ${NE10_VERSION}
    )

    target_link_libraries(NE10_test m)

    # If the CMake version supports it, attach header directory information
    # to the targets for when we are part of a parent build (ie being pulled
    # in via add_subdirectory() rather than being a standalone build).
    if (DEFINED CMAKE_VERSION AND NOT "${CMAKE_VERSION}" VERSION_LESS "2.8.11")
        target_include_directories(NE10_shared INTERFACE "${PROJECT_SOURCE_DIR}/inc")
	target_include_directories(NE10_shared INTERFACE "${PROJECT_SOURCE_DIR}/common")
    endif()

endif()
