#
#  Copyright 2011-12 ARM Limited
#
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.
#

# Define files.
set(NE10_INIT_SRCS ${PROJECT_SOURCE_DIR}/modules/NE10_init.c)
set(NE10_C_SRCS )
set(NE10_INTRINSIC_SRCS )
set(NE10_NEON_SRCS )

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
    foreach(intrinsic_file ${NE10_MATH_INTRINSIC_SRCS})
    set_source_files_properties(${intrinsic_file} PROPERTIES COMPILE_FLAGS "-mfpu=neon" )
    endforeach(intrinsic_file)

    # Add math NEON files.
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

    # Tell CMake these files need to go to the C compiler
    set(FLAGS "-mfpu=neon -Wa,-I${PROJECT_SOURCE_DIR}/inc -Wa,-I${PROJECT_SOURCE_DIR}/common -Wa,-I${PROJECT_SOURCE_DIR}/modules/math" )
    foreach(neon_file ${NE10_MATH_NEON_SRCS})
        set_property (SOURCE ${neon_file} PROPERTY LANGUAGE C)
        set_source_files_properties(
            ${neon_file} PROPERTIES COMPILE_FLAGS
            ${FLAGS}
        )
    endforeach(neon_file)

    # Add math init files.
    set(NE10_MATH_INIT_SRCS
        ${PROJECT_SOURCE_DIR}/modules/math/NE10_init_math.c
    )

    # Add math files
    set(NE10_INIT_SRCS ${NE10_INIT_SRCS} ${NE10_MATH_INIT_SRCS})
    set(NE10_C_SRCS ${NE10_C_SRCS} ${NE10_MATH_C_SRCS})
    set(NE10_INTRINSIC_SRCS ${NE10_INTRINSIC_SRCS} ${NE10_MATH_INTRINSIC_SRCS})
    set(NE10_NEON_SRCS ${NE10_NEON_SRCS} ${NE10_MATH_NEON_SRCS})
endif()

if(NE10_ENABLE_DSP)
    #enable NE10_init_dsp
    add_definitions(-DNE10_ENABLE_DSP)
    # Add dsp C files.
    set(NE10_DSP_C_SRCS
        ${PROJECT_SOURCE_DIR}/common/NE10_mask_table.c
        ${PROJECT_SOURCE_DIR}/modules/dsp/NE10_cfft.c
        ${PROJECT_SOURCE_DIR}/modules/dsp/NE10_cfft_init.c
        ${PROJECT_SOURCE_DIR}/modules/dsp/NE10_rfft.c
        ${PROJECT_SOURCE_DIR}/modules/dsp/NE10_rfft_init.c
        ${PROJECT_SOURCE_DIR}/modules/dsp/NE10_fir.c
        ${PROJECT_SOURCE_DIR}/modules/dsp/NE10_fir_init.c
        ${PROJECT_SOURCE_DIR}/modules/dsp/NE10_iir.c
        ${PROJECT_SOURCE_DIR}/modules/dsp/NE10_iir_init.c
    )

    # Add dsp intrinsic NEON files.
    set(NE10_DSP_INTRINSIC_SRCS
        ${PROJECT_SOURCE_DIR}/modules/dsp/NE10_rfft.neon.c
    )

    # Tell CMake these files need to be compiled with "-mfpu=neon"
    foreach(intrinsic_file ${NE10_DSP_INTRINSIC_SRCS})
    set_source_files_properties(${intrinsic_file} PROPERTIES COMPILE_FLAGS "-mfpu=neon" )
    endforeach(intrinsic_file)

    # Add dsp NEON files.
    set(NE10_DSP_NEON_SRCS
        ${PROJECT_SOURCE_DIR}/modules/dsp/NE10_cfft.neon.s
        ${PROJECT_SOURCE_DIR}/modules/dsp/NE10_fir.neon.s
        ${PROJECT_SOURCE_DIR}/modules/dsp/NE10_iir.neon.s
    )

    # Tell CMake these files need to go to the C compiler
    set(FLAGS "-mfpu=neon -Wa,-I${PROJECT_SOURCE_DIR}/inc -Wa,-I${PROJECT_SOURCE_DIR}/common" )
    foreach(neon_file ${NE10_DSP_NEON_SRCS})
        set_property (SOURCE ${neon_file} PROPERTY LANGUAGE C)
        set_source_files_properties(
            ${neon_file} PROPERTIES COMPILE_FLAGS
            ${FLAGS}
        )
    endforeach(neon_file)

    # Add dsp init files.
    set(NE10_DSP_INIT_SRCS
        ${PROJECT_SOURCE_DIR}/modules/dsp/NE10_init_dsp.c
    )

    # Add math files
    set(NE10_INIT_SRCS ${NE10_INIT_SRCS} ${NE10_DSP_INIT_SRCS})
    set(NE10_C_SRCS ${NE10_C_SRCS} ${NE10_DSP_C_SRCS})
    set(NE10_INTRINSIC_SRCS ${NE10_INTRINSIC_SRCS} ${NE10_DSP_INTRINSIC_SRCS})
    set(NE10_NEON_SRCS ${NE10_NEON_SRCS} ${NE10_DSP_NEON_SRCS})
endif()

include_directories (
    ${PROJECT_SOURCE_DIR}/inc
    ${PROJECT_SOURCE_DIR}/common
)

if(NE10_BUILD_STATIC)
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
endif()

if(NE10_BUILD_SHARED)

    add_library( NE10_shared SHARED
        ${NE10_C_SRCS}
        ${NE10_INTRINSIC_SRCS}
        ${NE10_NEON_SRCS}
        ${NE10_INIT_SRCS}
    )

    set_target_properties(NE10_shared PROPERTIES
        OUTPUT_NAME "NE10"
        CLEAN_DIRECT_OUTPUT 1
        VERSION ${NE10_VERSION}
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

endif()

