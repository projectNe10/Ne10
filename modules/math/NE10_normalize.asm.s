@
@  Copyright 2011-15 ARM Limited and Contributors.
@  All rights reserved.
@
@  Redistribution and use in source and binary forms, with or without
@  modification, are permitted provided that the following conditions are met:
@    * Redistributions of source code must retain the above copyright
@      notice, this list of conditions and the following disclaimer.
@    * Redistributions in binary form must reproduce the above copyright
@      notice, this list of conditions and the following disclaimer in the
@      documentation and/or other materials provided with the distribution.
@    * Neither the name of ARM Limited nor the
@      names of its contributors may be used to endorse or promote products
@      derived from this software without specific prior written permission.
@
@  THIS SOFTWARE IS PROVIDED BY ARM LIMITED AND CONTRIBUTORS "AS IS" AND
@  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
@  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
@  DISCLAIMED. IN NO EVENT SHALL ARM LIMITED AND CONTRIBUTORS BE LIABLE FOR ANY
@  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
@  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
@  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
@  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
@  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
@  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
@

@
@ NE10 Library : math/NE10_normalize.asm.s
@

        .text
        .syntax   unified

.include "NE10header.s"

        .balign   4
        .global   ne10_normalize_vec2f_asm
        .thumb
        .thumb_func

ne10_normalize_vec2f_asm:
        @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        @
        @ arm_result_t ne10_normalize_vec2f(arm_vec2f_t * dst,
        @                 arm_vec2f_t * src, unsigned int count)
        @
        @  r0: *dst and current destination item's address
        @  r1: *src and current source item's address
        @  r2: int count
        @
        @  r2: loop counter
        @
        @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

        cbz        r2, .LoopEndVec2F
        add        r0, r0, r2, lsl #3        @ r0 = r0 + count * 8
        add        r1, r1, r2, lsl #3        @ r1 = r1 + count * 8

.LoopBeginVec2F:
        vldmdb     r1!, {s10-s11}            @ load s10 = x and S11 = y
        vmul.f32   s14, s10, s10             @ s14 = x*x
        vmla.f32   s14, s11, s11             @ s14 = x*x + y*y
        vsqrt.f32  s15, s14                  @ s15 = sqrt( s14 )
        vdiv.f32   s10, s10, s15             @ s10 = x / length
        vdiv.f32   s11, s11, s15             @ s11 = y / length
        vstmdb     r0!, {s10-s11}            @ store the results and move the pointer
        subs       r2, r2, #1                @ decrement the loop counter
        bne        .LoopBeginVec2F           @ loop if r4 is still positive or zero
.LoopEndVec2F:
        mov        r0, NE10_OK             @ Return NE10_OK
        bx         lr




        .balign   4
        .global   ne10_normalize_vec3f_asm
        .thumb
        .thumb_func

ne10_normalize_vec3f_asm:
        @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        @
        @ arm_result_t ne10_normalize_vec3f(arm_vec3f_t * dst,
        @                 arm_vec3f_t * src, unsigned int count)
        @
        @  r0: *dst and current destination item's address
        @  r1: *src and current source item's address
        @  r2: int count
        @
        @  r2: loop counter
        @
        @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

        cbz        r2, .LoopEndVec3F
        add        r0, r0, r2, lsl #3        @ ...
        add        r0, r0, r2, lsl #2        @ r0 = r0 + count * 12
        add        r1, r1, r2, lsl #3        @ ...
        add        r1, r1, r2, lsl #2        @ r1 = r1 + count * 12

.LoopBeginVec3F:
        vldmdb     r1!, {s10-s12}
        vmul.f32   s14, s10, s10             @ s14 = x*x
        vmla.f32   s14, s11, s11             @ s14 = x*x + y*y
        vmla.f32   s14, s12, s12             @ s14 = x*x + y*y + z*z
        vsqrt.f32  s15, s14                  @ s15 = sqrt( s14 )
        vdiv.f32   s10, s10, s15             @ s10 = x / length
        vdiv.f32   s11, s11, s15             @ s11 = y / length
        vdiv.f32   s12, s12, s15             @ s12 = z / length
        vstmdb     r0!, {s10-s12}            @ store the results and move the pointer
        subs       r2, r2, #1                @ decrement the loop counter
        bne        .LoopBeginVec3F           @ loop if r4 is still positive or zero
.LoopEndVec3F:
        mov        r0, NE10_OK             @ Return NE10_OK
        bx         lr




        .balign   4
        .global   ne10_normalize_vec4f_asm
        .thumb
        .thumb_func

ne10_normalize_vec4f_asm:
        @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        @
        @ arm_result_t ne10_normalize_vec4f(arm_vec4f_t * dst,
        @                 arm_vec4f_t * src, unsigned int count)
        @
        @  r0: *dst and current destination item's address
        @  r1: *src and current source item's address
        @  r2: int count
        @
        @  r2: loop counter
        @
        @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

        cbz        r2, .LoopEndVec4F
        add        r0, r0, r2, lsl #4        @ r0 = r0 + count * 16
        add        r1, r1, r2, lsl #4        @ r1 = r1 + count * 16

.LoopBeginVec4F:
        vldmdb     r1!, {s10-s13}
        vmul.f32   s14, s10, s10             @ s14 = x*x
        vmla.f32   s14, s11, s11             @ s14 = x*x + y*y
        vmla.f32   s14, s12, s12             @ s14 = x*x + y*y + z*z
        vmla.f32   s14, s13, s13             @ s14 = x*x + y*y + z*z + w*w
        vsqrt.f32  s15, s14                  @ s15 = sqrt( s14 )
        vdiv.f32   s10, s10, s15             @ s10 = x / length
        vdiv.f32   s11, s11, s15             @ s11 = y / length
        vdiv.f32   s12, s12, s15             @ s12 = z / length
        vdiv.f32   s13, s13, s15             @ s12 = w / length
        vstmdb     r0!, {s10-s13}            @ store the results and move the pointer
        subs       r2, r2, #1                @ decrement the loop counter
        bne        .LoopBeginVec4F           @ loop if r4 is still positive or zero
.LoopEndVec4F:
        mov        r0, NE10_OK             @ Return NE10_OK
        bx         lr
