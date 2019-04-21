#
#  Copyright 2015-16 ARM Limited and Contributors.
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

# Following definition is only available under armv7.
if("${NE10_TARGET_ARCH}" STREQUAL "armv7")
    if(ANDROID_PLATFORM)
        if(NE10_ENABLE_PHYSICS)
            add_definitions(-DENABLE_NE10_PHYSICS_COMPUTE_AABB_VEC2F_NEON)
            add_definitions(-DENABLE_NE10_PHYSICS_RELATIVE_V_VEC2F_NEON)
            add_definitions(-DENABLE_NE10_PHYSICS_APPLY_IMPULSE_VEC2F_NEON)
        endif()
        if(NE10_ENABLE_IMGPROC)
            add_definitions(-DENABLE_NE10_IMG_ROTATE_RGBA_NEON)
        endif()
    endif()

    if (GNULINUX_PLATFORM)
        if(NE10_ENABLE_DSP)
            add_definitions(-DENABLE_NE10_FIR_FLOAT_NEON)
            add_definitions(-DENABLE_NE10_FIR_DECIMATE_FLOAT_NEON)
            add_definitions(-DENABLE_NE10_FIR_INTERPOLATE_FLOAT_NEON)
            add_definitions(-DENABLE_NE10_FIR_LATTICE_FLOAT_NEON)
            add_definitions(-DENABLE_NE10_FIR_SPARSE_FLOAT_NEON)
            add_definitions(-DENABLE_NE10_IIR_LATTICE_FLOAT_NEON)
        endif()
        if(NE10_ENABLE_PHYSICS)
            add_definitions(-DENABLE_NE10_PHYSICS_COMPUTE_AABB_VEC2F_NEON)
            add_definitions(-DENABLE_NE10_PHYSICS_RELATIVE_V_VEC2F_NEON)
            add_definitions(-DENABLE_NE10_PHYSICS_APPLY_IMPULSE_VEC2F_NEON)
        endif()
        if(NE10_ENABLE_IMGPROC)
            add_definitions(-DENABLE_NE10_IMG_ROTATE_RGBA_NEON)
        endif()
    endif()

    if(IOS_PLATFORM)
        # We use intrinsic for iOS, no definition is needed.
    endif()
endif()
