@
@  Copyright 2011-16 ARM Limited and Contributors.
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
@ NE10 Library : math/NE10_transmat.neon.s
@




        .text
        .syntax   unified

.include "NE10header.s"
.include "NE10_detmat.neon.inc.s"




        .balign   4
        .global   ne10_transmat_2x2f_neon
        .thumb
        .thumb_func

ne10_transmat_2x2f_neon:
        @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        @
        @ arm_result_t ne10_transmat_2x2f(arm_mat2x2f_t * dst,
        @                 arm_mat2x2f_t * src,
        @                 unsigned int count)
        @
        @  r0: *dst & current dst entry's address
        @  r1: *src1 & current src1 entry's address
        @  r2: int count & the number of items in the input array that can be
        @                   processed in chunks of 4 vectors
        @
        @  r3:  the number of items that are left to be processed at the end
        @                of the input array
        @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

        and               r3, r2, #3          @ r3 = count % 4;
        sub               r2, r2, r3          @ count = count - r2; This is what's left to be processed after this loop

        cmp               r2, #0
        beq               .L_check_mat2x2

.L_mainloop_mat2x2:

          subs            r2, r2, #4

          vld4.32         {d16, d18, d20, d22}, [r1]!
          vld4.32         {d17, d19, d21, d23}, [r1]!

          vswp            q9, q10

          vst4.32         {d16, d18, d20, d22}, [r0]!
          vst4.32         {d17, d19, d21, d23}, [r0]!

        bgt             .L_mainloop_mat2x2             @ loop if r2 > 0, if we have at least another 4 vectors (8 floats) to process

.L_mainloopend_mat2x2:

.L_check_mat2x2:
     @ check if anything left to process at the end of the input array
        cmp               r3, #0
        ble               .L_return_mat2x2

.L_secondloop_mat2x2:
     @ process the last few items left in the input array
        vld4.32         {d16[0], d18[0], d20[0], d22[0]}, [r1]!

        vswp            d18, d20

        subs              r3, r3, #1

        vst4.32         {d16[0], d18[0], d20[0], d22[0]}, [r0]!

        bgt               .L_secondloop_mat2x2

.L_return_mat2x2:
     @ return
        mov               r0, #0
        bx                lr




        @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        @ This macro calculates the inverse of two 3x3 marices
        @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        .macro STORE_3x3TRNMATS
            @ rearrange the results for use in a "vst3" instruction...
            vtrn.32     q8 , q11
            vtrn.32     q9 , q12
            vtrn.32     q10, q13

          vst3.32         { d16   , d18   , d20   }, [r0]!
          vst3.32         { d17[0], d19[0], d21[0]}, [r0]!
          vst3.32         { d22   , d24   , d26   }, [r0]!
          vst3.32         { d23[0], d25[0], d27[0]}, [r0]!
        .endm




        .align  2
        .global ne10_transmat_3x3f_neon
        .thumb
        .thumb_func
ne10_transmat_3x3f_neon:
        @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        @
        @ arm_result_t ne10_transmat_3x3f(arm_mat3x3f_t * dst,
        @                 arm_mat3x3f_t * src1,
        @                 arm_mat3x3f_t * src2,
        @                 unsigned int count)
        @
        @  r0: *dst & current dst entry's address
        @  r1: *src1 & current src1 entry's address
        @  r2: int count & the number of items in the input array that can be
        @                   processed in chunks of 4 vectors
        @
        @  r3:  the number of items that are left to be processed at the end
        @                   of the input array
        @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

        and               r3, r2, #3          @ r2 = count % 4;
        sub               r2, r2, r3          @ count = count - r2; This is what's left to be processed after this loop

        cmp               r2, #0
        beq               .L_check_mat3x3

.L_mainloop_mat3x3:
          LOAD_3x3MATS_ARGS  d16, d17, d18, d19, d20, d21,  d22, d23, d24, d25, d26, d27,  q8, q9, q10, q11, q12, q13, r1

          subs            r2, r2, #2

            vswp          d20, d17
            vswp          d22, d18
            vswp          d26, d19

          STORE_3x3TRNMATS

	bgt               .L_mainloop_mat3x3             @ loop if r2 > 0, if we have at least another 4 vectors (12 floats) to process

.L_mainloopend_mat3x3:

.L_check_mat3x3:
     @ check if anything left to process at the end of the input array
        cmp               r3, #0
        ble               .L_return_mat3x3

.L_secondloop_mat3x3:
     @ process the last few items left in the input array
       @ load the next (e.g. 3rd) set of values
           vld3.32         { d16   , d18   , d20   }, [r1]!
           vld3.32         { d17[0], d19[0], d21[0]}, [r1]!

             vtrn.32     q8 , q11
             vtrn.32     q9 , q12
             vtrn.32     q10, q13

          subs            r3, r3, #1

            vswp          d20, d17
            vswp          d22, d18
            vswp          d26, d19



        @ store the result for the last (e.g. 3rd) set
            vtrn.32     q8 , q11
            vtrn.32     q9 , q12
            vtrn.32     q10, q13

          vst3.32         { d16   , d18   , d20   }, [r0]!
          vst3.32         { d17[0], d19[0], d21[0]}, [r0]!

        bgt               .L_secondloop_mat3x3

.L_return_mat3x3:
     @ return
        mov               r0, #0
        bx                lr




        @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        @ This macro calculates the inverse of two 4x4 marices
        @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        .macro STORE_4x4INVMATS
            @ rearrange the results for use in a "vst3" instruction...
            vtrn.32      q8, q12
            vtrn.32      q9, q13
            vtrn.32     q10, q14
            vtrn.32     q11, q15

          vst4.32         { d16   , d18   , d20 ,  d22  }, [r0]!
          vst4.32         { d17   , d19   , d21 ,  d23  }, [r0]!
          vst4.32         { d24   , d26   , d28 ,  d30  }, [r0]!
          vst4.32         { d25   , d27   , d29 ,  d31  }, [r0]!
        .endm




        .align  2
        .global ne10_transmat_4x4f_neon
        .thumb
        .thumb_func
ne10_transmat_4x4f_neon:
        @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        @
        @ arm_result_t ne10_transmat_4x4f(arm_mat4x4f_t * dst,
        @                 arm_mat4x4f_t * src1,
        @                 arm_mat4x4f_t * src2,
        @                 unsigned int count)
        @
        @  r0: *dst & current dst entry's address
        @  r1: *src1 & current src1 entry's address
        @  r2: int count & the number of items in the input array that can be
        @                   processed in chunks of 4 vectors
        @
        @  r3:  the number of items that are left to be processed at the end
        @                of the input array
        @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

        and               r3, r2, #3          @ r3 = count % 4;
        sub               r2, r2, r3          @ count = count - r3; This is what's left to be processed after this loop

        cmp               r2, #0
        beq               .L_check_mat4x4

.L_mainloop_mat4x4:

         LOAD_4x4MATS_ARGS  d16, d17, d18, d19, d20, d21, d22, d23,  d24, d25, d26, d27, d28, d29, d30, d31,  q8, q9, q10, q11, q12, q13, q14, q15, r1


          subs            r2, r2, #2

            vswp       d18, d24
            vswp       d17, d20
            vswp       d22, d25
            vswp       d19, d28
            vswp       d27, d30
            vswp       d23, d29


          STORE_4x4INVMATS

        bgt               .L_mainloop_mat4x4             @ loop if r2 > 0, if we have at least another 4 vectors (16 floats) to process

.L_mainloopend_mat4x4:

.L_check_mat4x4:
     @ check if anything left to process at the end of the input array
        cmp               r3, #0
        ble               .L_return_mat4x4

.L_secondloop_mat4x4:
     @ process the last few items left in the input array
          vld4.32         { d16   , d18   , d20 ,  d22  }, [r1]!
          vld4.32         { d17   , d19   , d21 ,  d23  }, [r1]!

            vtrn.32      q8, q12
            vtrn.32      q9, q13
            vtrn.32     q10, q14
            vtrn.32     q11, q15

          subs            r3, r3, #1

            vswp       d18, d24
            vswp       d17, d20
            vswp       d22, d25
            vswp       d19, d28
            vswp       d27, d30
            vswp       d23, d29


        @ store the results
            vtrn.32      q8, q12
            vtrn.32      q9, q13
            vtrn.32     q10, q14
            vtrn.32     q11, q15

          vst4.32         { d16   , d18   , d20 ,  d22  }, [r0]!
          vst4.32         { d17   , d19   , d21 ,  d23  }, [r0]!


        bgt               .L_secondloop_mat4x4

.L_return_mat4x4:
     @ return
        mov               r0, #0
        bx                lr

