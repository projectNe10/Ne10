@
@  Copyright 2011-14 ARM Limited
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
@ NE10 Library : math/NE10_mla.asm.s
@

        .text
        .syntax   unified

.include "NE10header.s"

        .balign   4
        .global   ne10_mla_float_asm
        .thumb
        .thumb_func

ne10_mla_float_asm:
        @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        @
        @ arm_result_t ne10_mla_float(arm_vec2f_t * dst, arm_float_t * acc,
        @                 arm_float_t * src1, const arm_float_t * src2,
        @                 unsigned int count)
        @
        @  r0: *dst & current src1 entry's address - made of base(r0)+offset
        @  r1: *acc & current acc entry's address - made of base(r1)+offset
        @  r2: *src1 & current src1 entry's address - made of base(r2)+offset
        @  r3: *src2 & current src2 entry's address - made of base(r3)+offset
        @  r4: int count
        @
        @  r4: loop counter
        @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

        push    {r4}
        ldr     r4, [r13, #4]             @ r4 = cst ( off the stack pointer (sp) - which is r13 )
        cbz     r4, .LoopEndFloat

.LoopBeginFloat:
        vldr      s10, [r1]               @ Load s10 = acc[i]
        vldr      s1, [r2]                @ Load s1 = src1[i]
        vldr      s2, [r3]                @ Load s2 = src2[i]
        add       r1, r1, #4              @ move to the next acc entry
        add       r2, r2, #4              @ move to the next src1 entry
        add       r3, r3, #4              @ next entry in src2
        vmla.f32  s10, s1, s2             @ s10 = acc[i] + (src1[i] * src2[i])
        vstr      s10, [r0]               @ Store the result back into the main memory
        add       r0, r0, #4              @ next entry in the dst
        subs      r4, r4, #1              @ count down using the current index (i--)
        bne        .LoopBeginFloat        @ Continue if  "i < count"

.LoopEndFloat:
        mov     r0, NE10_OK             @ Return NE10_OK
        pop     {r4}
        bx      lr
