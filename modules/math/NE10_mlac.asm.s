@
@  Copyright 2011-12 ARM Limited
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
@  DISCLAIMED. IN NO EVENT SHALL ARM LIMITED BE LIABLE FOR ANY
@  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
@  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
@  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
@  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
@  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
@  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
@

@
@ NE10 Library : math/NE10_mlac.asm.s
@

        .text
        .syntax   unified

.include "NE10header.s"

        .balign   4
        .global   ne10_mlac_float_asm
        .thumb
        .thumb_func

ne10_mlac_float_asm:
        @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        @
        @ arm_result_t ne10_mlac_float(arm_vec2f_t * dst, arm_vec2f_t * acc,
        @                 arm_float_t * src, const arm_float_t * cst,
        @                 unsigned int count)
        @
        @  r0: *dst
        @  r1: *acc
        @  r2: *src
        @  r3: cst
        @  r4: int count
        @
        @  r4: loop counter
        @  r5: current item's offset in acc[], src[], and dst[]
        @  r6: current accumulator item's address made of base(r1)+offset(r5)
        @  r7: current source item's address made of base(r2)+offset(r5)
        @  r8: current destination item's address made of base(r0)+offset(r5)
        @
        @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

        push    {r4, r5, r6, r7, r8}
        ldr     r4, [r13, #20]             @ r4 = cst ( off the stack pointer (sp) - which is r13 )
        cbz     r4, .LoopEndFloat
        mov     r5, #0

.LoopBeginFloat:
        add       r6, r1, r5              @ Get current accumulator item's address in memory
        vldr      s10, [r6, #0]           @ Load acc[i]
        add       r7, r2, r5              @ Get current source item's address in memory
        vldr      s2, [r7, #0]            @ Load src[i]
        vmov      s3, r3                  @ Get cst into register s3
        vmla.f32  s10, s2, s3             @ s10 = acc[i] + ( src[i] * cst )
        add       r8, r0, r5              @ Get current destination item's address in memory
        vstr      s10, [r8, #0]           @ Store the result back into the main memory
        add       r5, r5, #4              @ increase the offset by 1*sizeof(float) @@ (for x and y)
        subs      r4, r4, #1              @ count down using the current index (i--)
        bne        .LoopBeginFloat        @ Continue if  "i < count"

.LoopEndFloat:
        mov     r0, NE10_OK             @ Return NE10_OK
        pop     {r4, r5, r6, r7, r8}
        bx      lr




        .balign   4
        .global   ne10_mlac_vec2f_asm
        .thumb
        .thumb_func

ne10_mlac_vec2f_asm:
        @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        @
        @ arm_result_t ne10_mlac_vec2f(arm_vec2f_t * dst,
        @                 arm_vec2f_t * src, const arm_vec2f_t * cst,
        @                 unsigned int count)
        @
        @  r0: *dst
        @  r1: *acc
        @  r2: *src
        @  r3: *cst
        @  r4: int count
        @
        @  r4: loop counter
        @  r5: current item's offset in acc[], src[], and dst[]
        @  r6: current accumulator item's address made of base(r1)+offset(r5)
        @  r7: current source item's address made of base(r2)+offset(r5)
        @  r8: current destination item's address made of base(r0)+offset(r5)
        @
        @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

        push    {r4, r5, r6, r7, r8}
        ldr     r4, [r13, #20]             @ r4 = cst ( off the stack pointer (sp) - which is r13 )
        cbz     r4, .LoopEndVec2F
        mov     r5, #0

.LoopBeginVec2F:
        add       r6, r1, r5              @ Get current accumulator item's address in memory
        vldr      s10, [r6, #0]           @ Load acc[i].x and acc[i].y
        vldr      s11, [r6, #4]
        add       r7, r2, r5              @ Get current source item's address in memory
        vldr      s1, [r7, #0]            @ Load src[i].x and src[i].y
        vldr      s2, [r7, #4]
        vldr      s3, [r3, #0]            @ Load cst->x and cst->y
        vldr      s4, [r3, #4]
        vmla.f32  s10, s1, s3             @ s10 = acc[i].x + ( src[i].x * cst->x )
        vmla.f32  s11, s2, s4
        add       r8, r0, r5              @ Get current destination item's address in memory
        vstr      s10, [r8, #0]           @ Store the results back into the main memory
        vstr      s11, [r8, #4]
        add       r5, r5, #8              @ increase the offset by 2*sizeof(float) @@ (for x and y)
        subs      r4, r4, #1              @ count down using the current index (i--)
        bne        .LoopBeginVec2F        @ Continue if  "i < count"

.LoopEndVec2F:
        mov     r0, NE10_OK             @ Return NE10_OK
        pop     {r4, r5, r6, r7, r8}
        bx      lr




        .balign   4
        .global   ne10_mlac_vec3f_asm
        .thumb
        .thumb_func

ne10_mlac_vec3f_asm:
        @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        @
        @ arm_result_t ne10_mlac_vec3f(arm_vec3f_t * dst,
        @                 arm_vec3f_t * src, const arm_vec3f_t * cst,
        @                 unsigned int count)
        @
        @  r0: *dst
        @  r1: *acc
        @  r2: *src
        @  r3: *cst
        @  r4: int count
        @
        @  r4: loop counter
        @  r5: current item's offset in acc[], src[], and dst[]
        @  r6: current accumulator item's address made of base(r1)+offset(r5)
        @  r7: current source item's address made of base(r2)+offset(r5)
        @  r8: current destination item's address made of base(r0)+offset(r5)
        @
        @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

        push    {r4, r5, r6, r7, r8}
        ldr     r4, [r13, #20]             @ r4 = cst ( off the stack pointer (sp) - which is r13 )
        cbz     r4, .LoopEndVec3F
        mov     r5, #0

.LoopBeginVec3F:
        add       r6, r1, r5              @ Get current accumulator item's address in memory
        vldr      s10, [r6, #0]           @ Load acc[i].x, acc[i].y , and acc[i].z
        vldr      s11, [r6, #4]
        vldr      s12, [r6, #8]
        add       r7, r2, r5              @ Get current source item's address in memory
        vldr      s1, [r7, #0]            @ Load src[i].x, src[i].y , and src[i].z
        vldr      s2, [r7, #4]
        vldr      s3, [r7, #8]
        vldr      s4, [r3, #0]            @ Load cst->x, cst->y, and cst->z
        vldr      s5, [r3, #4]
        vldr      s6, [r3, #8]
        vmla.f32  s10, s1, s4             @ s10 = acc[i].x + ( src[i].x * cst->x )
        vmla.f32  s11, s2, s5             @  same for 'y'
        vmla.f32  s12, s3, s6             @  same for 'z'
        add       r8, r0, r5              @ Get current destination item's address in memory
        vstr      s10, [r8, #0]           @ Store the results back into the main memory
        vstr      s11, [r8, #4]
        vstr      s12, [r8, #8]
        add       r5, r5, #12             @ increase the offset by 3*sizeof(float) @@ (for x, y, and z)
        subs      r4, r4, #1              @ count down using the current index (i--)
        bne        .LoopBeginVec3F        @ Continue if  "i < count"

.LoopEndVec3F:
        mov     r0, NE10_OK             @ Return NE10_OK
        pop     {r4, r5, r6, r7, r8}
        bx      lr




        .balign   4
        .global   ne10_mlac_vec4f_asm
        .thumb
        .thumb_func

ne10_mlac_vec4f_asm:
        @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        @
        @ arm_result_t ne10_mlac_vec4f(arm_vec4f_t * dst,
        @                 arm_vec4f_t * src, const arm_vec4f_t * cst,
        @                 unsigned int count)
        @
        @  r0: *dst
        @  r1: *acc
        @  r2: *src
        @  r3: *cst
        @  r4: int count
        @
        @  r4: loop counter
        @  r5: current item's offset in acc[], src[], and dst[]
        @  r6: current accumulator item's address made of base(r1)+offset(r5)
        @  r7: current source item's address made of base(r2)+offset(r5)
        @  r8: current destination item's address made of base(r0)+offset(r5)
        @
        @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

        push    {r4, r5, r6, r7, r8}
        ldr     r4, [r13, #20]             @ r4 = cst ( off the stack pointer (sp) - which is r13 )
        cbz     r4, .LoopEndVec4F
        mov     r5, #0

.LoopBeginVec4F:
        add       r6, r1, r5              @ Get current accumulator item's address in memory
        vldr      s10, [r6, #0]           @ Load acc[i].x, acc[i].y , acc[i].z, and w
        vldr      s11, [r6, #4]
        vldr      s12, [r6, #8]
        vldr      s13, [r6, #12]
        add       r7, r2, r5              @ Get current source item's address in memory
        vldr      s1, [r7, #0]            @ Load src[i].x, src[i].y , src[i].z, and w
        vldr      s2, [r7, #4]
        vldr      s3, [r7, #8]
        vldr      s4, [r7, #12]
        vldr      s5, [r3, #0]            @ Load cst->x, cst->y, cst->z, and w
        vldr      s6, [r3, #4]
        vldr      s7, [r3, #8]
        vldr      s8, [r3, #12]
        vmla.f32  s10, s1, s5             @ s10 = acc[i].x + ( src[i].x * cst->x )
        vmla.f32  s11, s2, s6             @  same for 'y'
        vmla.f32  s12, s3, s7             @  same for 'z'
        vmla.f32  s13, s4, s8             @  same for 'w'
        add       r8, r0, r5              @ Get current destination item's address in memory
        vstr      s10, [r8, #0]           @ Store the results back into the main memory
        vstr      s11, [r8, #4]
        vstr      s12, [r8, #8]
        vstr      s13, [r8, #12]
        add       r5, r5, #16             @ increase the offset by 4*sizeof(float) @@ (for x, y, z, and w)
        subs      r4, r4, #1              @ count down using the current index (i--)
        bne        .LoopBeginVec4F        @ Continue if  "i < count"

.LoopEndVec4F:
        mov     r0, NE10_OK             @ Return NE10_OK
        pop     {r4, r5, r6, r7, r8}
        bx      lr
