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
@ NE10 Library : math/NE10_identitymat.neon.s
@




        .text
        .syntax   unified

.include "NE10header.s"




        .balign   4
        .global   ne10_identitymat_2x2f_neon
        .thumb
        .thumb_func

ne10_identitymat_2x2f_neon:
        @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        @
        @ arm_result_t ne10_identitymat_2x2f(arm_mat2x2f_t * dst,
        @                 arm_mat2x2f_t * src,
        @                 unsigned int count)
        @
        @  r0: *dst & current dst entry's address
        @  r1: int count & the number of items in the input array that can be
        @                   processed in chunks of 4 vectors
        @
        @  r2:  the number of items that are left to be processed at the end
        @                of the input array
        @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

        and               r2, r1, #3          @ r2 = count % 4;
        sub               r1, r1, r2          @ count = count - r1; This is what's left to be processed after this loop

        vmov.f32              d2, #0.0
        vmov.f32              d3, #0.0
        vmov.f32              d0, #1.0
        vmov.f32              d1, #1.0


        vmov              q3, q0
        vmov              q2, q1

        cmp               r1, #0
        beq               .L_check_mat2x2

.L_mainloop_mat2x2:

          subs            r1, r1, #4

          vst4.32         {d0, d2, d4, d6}, [r0]!
          vst4.32         {d1, d3, d5, d7}, [r0]!

        bgt             .L_mainloop_mat2x2             @ loop if r1 > 0, if we have at least another 4 vectors (8 floats) to process

.L_mainloopend_mat2x2:

.L_check_mat2x2:
     @ check if anything left to process at the end of the input array
        cmp               r2, #0
        ble               .L_return_mat2x2

.L_secondloop_mat2x2:
     @ process the last few items left in the input array
        vswp            d18, d20

        subs              r2, r2, #1

          vst4.32         {d0[0], d2[0], d4[0], d6[0]}, [r0]!

        bgt               .L_secondloop_mat2x2

.L_return_mat2x2:
     @ return
        mov               r0, #0
        bx                lr




        .align  2
        .global ne10_identitymat_3x3f_neon
        .thumb
        .thumb_func
ne10_identitymat_3x3f_neon:
        @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        @
        @ arm_result_t ne10_identitymat_3x3f(arm_mat3x3f_t * dst,
        @                 arm_mat3x3f_t * src1,
        @                 arm_mat3x3f_t * src2,
        @                 unsigned int count)
        @
        @  r0: *dst & current dst entry's address
        @  r1: int count & the number of items in the input array that can be
        @                   processed in chunks of 4 vectors
        @
        @  r2:  the number of items that are left to be processed at the end
        @                   of the input array
        @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

        and               r2, r1, #3          @ r1 = count % 4;
        sub               r1, r1, r2          @ count = count - r1; This is what's left to be processed after this loop

        vmov.f32              d2, #0.0
        vmov.f32              d3, #0.0
        vmov.f32              d0, #1.0
        vmov.f32              d1, #1.0

        vmov              q8 , q1
        vmov              q9 , q1
        vmov              q10, q1
        vmov              q11, q1
        vmov              q12, q1
        vmov              q13, q1

        vtrn.32           d2, d0                @  d0 = {0.0f, 1.0f}
        vtrn.32           d1, d3                @  d1 = {1.0f, 0.0f}

        vmov              d16, d1
        vmov              d18, d0
        vmov              d21, d1
        vmov              d22, d1
        vmov              d24, d0
        vmov              d27, d1

        cmp               r1, #0
        beq               .L_check_mat3x3

.L_mainloop_mat3x3:

          subs            r1, r1, #2

          vst3.32         { d16   , d18   , d20   }, [r0]!
          vst3.32         { d17[0], d19[0], d21[0]}, [r0]!
          vst3.32         { d22   , d24   , d26   }, [r0]!
          vst3.32         { d23[0], d25[0], d27[0]}, [r0]!

	bgt               .L_mainloop_mat3x3             @ loop if r1 > 0, if we have at least another 4 vectors (12 floats) to process

.L_mainloopend_mat3x3:

.L_check_mat3x3:
     @ check if anything left to process at the end of the input array
        cmp               r2, #0
        ble               .L_return_mat3x3

.L_secondloop_mat3x3:
     @ process the last few items left in the input array

          subs            r2, r2, #1

          vst3.32         { d16   , d18   , d20   }, [r0]!
          vst3.32         { d17[0], d19[0], d21[0]}, [r0]!

        bgt               .L_secondloop_mat3x3

.L_return_mat3x3:
     @ return
        mov               r0, #0
        bx                lr




        .align  2
        .global ne10_identitymat_4x4f_neon
        .thumb
        .thumb_func
ne10_identitymat_4x4f_neon:
        @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        @
        @ arm_result_t ne10_identitymat_4x4f(arm_mat4x4f_t * dst,
        @                 arm_mat4x4f_t * src1,
        @                 arm_mat4x4f_t * src2,
        @                 unsigned int count)
        @
        @  r0: *dst & current dst entry's address
        @  r1: int count & the number of items in the input array that can be
        @                   processed in chunks of 4 vectors
        @
        @  r2:  the number of items that are left to be processed at the end
        @                of the input array
        @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

        and               r2, r1, #3          @ r2 = count % 4;
        sub               r1, r1, r2          @ count = count - r2; This is what's left to be processed after this loop

        vmov.f32              d2, #0.0
        vmov.f32              d3, #0.0
        vmov.f32              d0, #1.0
        vmov.f32              d1, #1.0

        vmov              q8 , q1
        vmov              q9 , q1
        vmov              q10, q1
        vmov              q11, q1
        vmov              q12, q1
        vmov              q13, q1
        vmov              q14, q1
        vmov              q15, q1

        vtrn.32           d2, d0                @  d0 = {0.0f, 1.0f}
        vtrn.32           d1, d3                @  d1 = {1.0f, 0.0f}

        vmov              d16, d1
        vmov              d18, d0
        vmov              d21, d1
        vmov              d23, d0

        vmov              d24, d1
        vmov              d26, d0
        vmov              d29, d1
        vmov              d31, d0

        cmp               r1, #0
        beq               .L_check_mat4x4

.L_mainloop_mat4x4:

          subs            r1, r1, #2

          vst4.32         { d16   , d18   , d20 ,  d22  }, [r0]!
          vst4.32         { d17   , d19   , d21 ,  d23  }, [r0]!
          vst4.32         { d24   , d26   , d28 ,  d30  }, [r0]!
          vst4.32         { d25   , d27   , d29 ,  d31  }, [r0]!

        bgt               .L_mainloop_mat4x4             @ loop if r1 > 0, if we have at least another 4 vectors (16 floats) to process

.L_mainloopend_mat4x4:

.L_check_mat4x4:
     @ check if anything left to process at the end of the input array
        cmp               r2, #0
        ble               .L_return_mat4x4

.L_secondloop_mat4x4:
     @ process the last few items left in the input array

          subs            r2, r2, #1

          vst4.32         { d16   , d18   , d20 ,  d22  }, [r0]!
          vst4.32         { d17   , d19   , d21 ,  d23  }, [r0]!


        bgt               .L_secondloop_mat4x4

.L_return_mat4x4:
     @ return
        mov               r0, #0
        bx                lr

