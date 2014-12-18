@
@  Copyright 2011-14 ARM Limited and Contributors.
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
@ NE10 Library : math/NE10_setc.asm.s
@

        .text
        .syntax   unified

.include "NE10header.s"

        .balign   4
        .global   ne10_setc_float_asm
        .thumb
        .thumb_func

ne10_setc_float_asm:
        @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        @
        @ arm_result_t ne10_setc_float(arm_float_t * dst,
        @                 const arm_float_t cst,
        @                 unsigned int count)
        @
        @  r0: *dst
        @  r1: cst
        @  r2: int count
        @
        @  r2: loop counter
        @
        @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

        cbz     r2, .LoopEndFloat

.LoopBeginFloat:
        str       r1, [r0], #4            @ Store it back into the main memory
        subs      r2, r2, #1              @ count down using the current index (i--)
        bne        .LoopBeginFloat        @ Continue if  "i < count"

.LoopEndFloat:
        mov     r0, NE10_OK             @ Return NE10_OK
        bx      lr




        .balign   4
        .global   ne10_setc_vec2f_asm
        .thumb
        .thumb_func

ne10_setc_vec2f_asm:
        @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        @
        @ arm_result_t ne10_setc_vec2f(arm_vec2f_t * dst,
        @                 const arm_vec2f_t * cst,
        @                 unsigned int count)
        @
        @  r0: *dst
        @  r1: *cst
        @  r2: int count
        @
        @  r2: loop counter
        @
        @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

        push    {r4, r5}
        cbz     r2, .LoopEndVec2F
        ldr       r4, [r1, #0]            @ Load cst->x into r4
        ldr       r5, [r1, #4]            @ Load cst->y into r5

.LoopBeginVec2F:
        str       r4, [r0], #4            @ Store them in the destination
        str       r5, [r0], #4
        subs      r2, r2, #1              @ count down using the current index (i--)
        bne        .LoopBeginVec2F        @ Continue if  "i < count"

.LoopEndVec2F:
        mov     r0, NE10_OK             @ Return NE10_OK
        pop     {r4, r5}
        bx      lr




        .balign   4
        .global   ne10_setc_vec3f_asm
        .thumb
        .thumb_func

ne10_setc_vec3f_asm:
        @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        @
        @ arm_result_t ne10_setc_vec3f(arm_vec3f_t * dst,
        @                 const arm_vec3f_t * cst,
        @                 unsigned int count)
        @
        @  r0: *dst
        @  r1: *cst
        @  r2: int count
        @
        @  r2: loop counter
        @
        @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

        push    {r4, r5, r6}
        cbz     r2, .LoopEndVec3F
        ldr       r4, [r1, #0]            @ Load cst->x into r4
        ldr       r5, [r1, #4]            @ Load cst->y into r5
        ldr       r6, [r1, #8]            @ r6 = cst->z

.LoopBeginVec3F:
        str       r4, [r0], #4            @ Store them in the destination
        str       r5, [r0], #4
        str       r6, [r0], #4
        subs      r2, r2, #1              @ count down using the current index (i--)
        bne        .LoopBeginVec3F        @ Continue if  "i < count"

.LoopEndVec3F:
        mov     r0, NE10_OK             @ Return NE10_OK
        pop     {r4, r5, r6}
        bx      lr




        .balign   4
        .global   ne10_setc_vec4f_asm
        .thumb
        .thumb_func

ne10_setc_vec4f_asm:
        @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        @
        @ arm_result_t ne10_setc_vec4f(arm_vec4f_t * dst,
        @                 const arm_vec4f_t * cst,
        @                 unsigned int count)
        @
        @  r0: *dst
        @  r1: *cst
        @  r2: int count
        @
        @  r2: loop counter
        @
        @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

        push    {r4, r5, r6, r7}
        cbz     r2, .LoopEndVec4F
        ldr       r4, [r1, #0]            @ Load cst->x into r4
        ldr       r5, [r1, #4]            @ Load cst->y into r5
        ldr       r6, [r1, #8]            @ r6 = cst->z
        ldr       r7, [r1, #12]           @ r7 = cst->w

.LoopBeginVec4F:
        str       r4, [r0], #4            @ Store them in the destination
        str       r5, [r0], #4
        str       r6, [r0], #4
        str       r7, [r0], #4
        subs      r2, r2, #1              @ count down using the current index (i--)
        bne        .LoopBeginVec4F        @ Continue if  "i < count"

.LoopEndVec4F:
        mov     r0, NE10_OK             @ Return NE10_OK
        pop     {r4, r5, r6, r7}
        bx      lr
