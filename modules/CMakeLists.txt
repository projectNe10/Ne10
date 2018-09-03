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
include(../cmake/SetFileToC.cmake)
include(../cmake/ConvertGas.cmake)

if(NE10_ARM_HARD_FLOAT)
    add_definitions(-DNE10_ENABLE_HF)
endif()
# Define files.
set(NE10_INIT_SRCS ${PROJECT_SOURCE_DIR}/modules/NE10_init.c)
set(NE10_C_SRCS )
set(NE10_INTRINSIC_SRCS )
set(NE10_NEON_SRCS )

include_directories (
    ${PROJECT_SOURCE_DIR}/inc
    ${PROJECT_SOURCE_DIR}/common
)


if(NE10_ENABLE_MATH)
    #enable NE10_init_math
    add_definitions(-DNE10_ENABLE_MATH)
    add_subdirectory(math)
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

if(NE10_BUILD_STATIC OR ANDROID_PLATFORM OR IOS_DEMO)
    add_library( NE10 STATIC
        ${NE10_C_SRCS}
        ${NE10_INTRINSIC_SRCS}
        ${NE10_NEON_SRCS}
        ${NE10_INIT_SRCS}
    )

    if(NE10_ENABLE_MATH)
        target_link_libraries(NE10 NE10_math)
    endif(NE10_ENABLE_MATH)

    set_target_properties(NE10 PROPERTIES
        CLEAN_DIRECT_OUTPUT 1
        VERSION ${NE10_VERSION}
    )

    set_target_properties(NE10 PROPERTIES
        LINKER_LANGUAGE C
    )

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
    if(NE10_ENABLE_MATH)
        target_link_libraries(NE10_shared NE10_math)
    endif(NE10_ENABLE_MATH)

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
endif()
