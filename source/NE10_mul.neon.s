@
@  Copyright 2011-12 ARM Limited
@
@  Licensed under the Apache License, Version 2.0 (the "License");
@  you may not use this file except in compliance with the License.
@  You may obtain a copy of the License at
@
@      http://www.apache.org/licenses/LICENSE-2.0
@
@  Unless required by applicable law or agreed to in writing, software
@  distributed under the License is distributed on an "AS IS" BASIS,
@  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
@  See the License for the specific language governing permissions and
@  limitations under the License.
@

@
@ NE10 Library : source/NE10_mul.neon.s
@

        .text
        .syntax   unified

.include "headers/NE10header.s"




        .balign   4
        .global   mul_float_neon
        .thumb
        .thumb_func

mul_float_neon:
        @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        @
        @ arm_result_t mul_float(arm_float_t * dst,
        @                 arm_float_t * src1,
        @                 arm_float_t * src2,
        @                 unsigned int count)
        @
        @  r0: *dst & current dst entry's address
        @  r1: *src1 & current src1 entry's address
        @  r2: *src2 & current src2 entry's address
        @  r3: int count & the number of items in the input array that can be
        @                   processed in chunks of 4 vectors
        @
        @  r4:  the number of items that are left to be processed at the end of
        @                   the input array
        @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

        push              {r4}
        and               r4, r3, #3          @ r4 = count % 4;
        sub               r3, r3, r4          @ count = count - r4; This is what's left to be processed after this loop

        cbz               r3, .L_check_float

        @ load the 1st set of values
          vld1.32         {q0}, [r1]!
          vld1.32         {q1}, [r2]!
          subs            r3, r3, #8          @ 4 for this set, and 4 for the 2nd set

        @ calculate values for the 1st set
          vmul.f32        q3, q0, q1         @ q3 = q0 * q1

        @ load the 2nd set of values
          vld1.32         {q0}, [r1]!
          vld1.32         {q1}, [r2]!

          ble             .L_mainloopend_float

.L_mainloop_float:
        @ store the result for the 1st/next (e.g. 3rd) set
          vst1.32         {d6,d7}, [r0]!

        @ calculate values for the 2nd/next (e.g. 3rd) set
          vmul.f32        q3, q0, q1         @ q3 = q0 * q1

       @ load the next (e.g. 3rd) set of values
          vld1.32         {q0}, [r1]!
          vld1.32         {q1}, [r2]!
          subs            r3, r3, #4

        bgt             .L_mainloop_float             @ loop if r2 is > r3, if we have at least another 4 floats

.L_mainloopend_float:
        @ the last iteration for this call
        @ store the result for the set of values before the last one (e.g 2nd set)
          vst1.32         {d6,d7}, [r0]!

        @ calculate values for the last (e.g. 3rd) set
          vmul.f32        q3, q0, q1         @ q3 = q0 * q1

        @ store the result for the last (e.g. 3rd) set
          vst1.32         {d6,d7}, [r0]!


.L_check_float:
     @ check if anything left to process at the end of the input array
        cmp               r4, #0
        ble               .L_return_float

.L_secondloop_float:
     @ process the last few items left in the input array
        vld1.f32          d0[0], [r1]!           @ Fill in d0[0]
        vld1.f32          d1[0], [r2]!           @ Fill in d1[1]


        subs              r4, r4, #1

        @ values
        vmul.f32          d0, d0, d1

        vst1.32           {d0[0]}, [r0]!

        bgt               .L_secondloop_float

.L_return_float:
     @ return
        pop               {r4}
        mov               r0, #0
        bx                lr




        .balign   4
        .global   vmul_vec2f_neon
        .thumb
        .thumb_func

vmul_vec2f_neon:
        @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        @
        @ arm_result_t mul_float(arm_vec2f_t * dst,
        @                 arm_vec2f_t * src1,
        @                 arm_vec2f_t * src2,
        @                 unsigned int count)
        @
        @  r0: *dst & current dst entry's address
        @  r1: *src1 & current src1 entry's address
        @  r2: *src2 & current src2 entry's address
        @  r3: int count & the number of items in the input array that can be
        @                   processed in chunks of 4 vectors
        @
        @  r4:  the number of items that are left to be processed at the end of
        @                   the input array
        @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

        push              {r4}
        and               r4, r3, #3          @ r4 = count % 4;
        sub               r3, r3, r4          @ count = count - r3; This is what's left to be processed after this loop

        cbz               r3, .L_check_vec2

        @ load the 1st set of values
          vld2.32         {q0-q1}, [r1]!
          vld2.32         {q2-q3}, [r2]!
          subs            r3, r3, #8          @ 4 for this set, and 4 for the 2nd set

        @ calculate values for the 1st set
          vmul.f32        q4, q0, q2
          vmul.f32        q5, q1, q3

        @ load the 2nd set of values
          vld2.32         {q0-q1}, [r1]!
          vld2.32         {q2-q3}, [r2]!

          ble             .L_mainloopend_vec2

.L_mainloop_vec2:
        @ store the result for the 1st/next (e.g. 3rd) set
          vst2.32         {d8,d9,d10,d11}, [r0]!

        @ calculate values for the 2nd/next (e.g. 3rd) set
          vmul.f32        q4, q0, q2
          vmul.f32        q5, q1, q3

       @ load the next (e.g. 3rd) set of values
          vld2.32         {q0-q1}, [r1]!
          vld2.32         {q2-q3}, [r2]!
          subs            r3, r3, #4

        bgt             .L_mainloop_vec2             @ loop if r2 is > r3, if we have at least another 4 vectors (8 floats) to process

.L_mainloopend_vec2:
        @ the last iteration for this call
        @ store the result for the set of values before the last one (e.g 2nd set)
          vst2.32         {d8,d9,d10,d11}, [r0]!

        @ calculate values for the last (e.g. 3rd) set
          vmul.f32        q4, q0, q2
          vmul.f32        q5, q1, q3

        @ store the result for the last (e.g. 3rd) set
          vst2.32         {d8,d9,d10,d11}, [r0]!

.L_check_vec2:
     @ check if anything left to process at the end of the input array
        cmp               r4, #0
        ble               .L_return_vec2

.L_secondloop_vec2:
     @ process the last few items left in the input array
        vld1.f32          d0, [r1]!
        vld1.f32          d1, [r2]!

        subs              r4, r4, #1

        @ calculate values
        vmul.f32          d0, d0, d1

        vst1.32           {d0}, [r0]!

        bgt               .L_secondloop_vec2

.L_return_vec2:
     @ return
        pop               {r4}
        mov               r0, #0
        bx                lr




        .align  2
        .global vmul_vec3f_neon
        .thumb
        .thumb_func
vmul_vec3f_neon:
        @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        @
        @ arm_result_t mul_float(arm_vec3f_t * dst,
        @                 arm_vec3f_t * src1,
        @                 arm_vec3f_t * src2,
        @                 unsigned int count)
        @
        @  r0: *dst & current dst entry's address
        @  r1: *src1 & current src1 entry's address
        @  r2: *src2 & current src2 entry's address
        @  r3: int count & the number of items in the input array that can be
        @                   processed in chunks of 4 vectors
        @
        @  r4:  the number of items that are left to be processed at the end of
        @                   the input array
        @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

        push              {r4}
        and               r4, r3, #3          @ r3 = count % 4;
        sub               r3, r3, r4          @ count = count - r3; This is what's left to be processed after this loop

        cmp               r3, #0
        beq               .L_check_vec3

        @ load the 1st set of values
          vld3.32         {d0, d2, d4}, [r1]!
          vld3.32         {d1, d3, d5}, [r1]!
          vld3.32         {d6, d8, d10}, [r2]!
          vld3.32         {d7, d9, d11}, [r2]!
          subs            r3, r3, #4

        @ calculate values for the 1st set
          vmul.f32        q10, q0, q3
          vmul.f32        q11, q1, q4
          vmul.f32        q12, q2, q5

        @ load the 2nd set of values
          vld3.32         {d0, d2, d4}, [r1]!
          vld3.32         {d1, d3, d5}, [r1]!
          vld3.32         {d6, d8, d10}, [r2]!
          vld3.32         {d7, d9, d11}, [r2]!
          subs            r3, r3, #4

          ble             .L_mainloopend_vec3

.L_mainloop_vec3:
        @ store the result for the 1st/next (e.g. 3rd) set
          vst3.32         {d20, d22, d24}, [r0]!
          vst3.32         {d21, d23, d25}, [r0]!

        @ calculate values for the 2nd/next (e.g. 3rd) set
          vmul.f32        q10, q0, q3
          vmul.f32        q11, q1, q4
          vmul.f32        q12, q2, q5

       @ load the next (e.g. 3rd) set of values
          vld3.32         {d0, d2, d4}, [r1]!
          vld3.32         {d1, d3, d5}, [r1]!
          vld3.32         {d6, d8, d10}, [r2]!
          vld3.32         {d7, d9, d11}, [r2]!
          subs            r3, r3, #4

        bgt               .L_mainloop_vec3             @ loop if r2 is > r3, if we have at least another 4 vectors (12 floats) to process

.L_mainloopend_vec3:
        @ the last iteration for this call
        @ store the result for the set of values before the last one (e.g 2nd set)
          vst3.32         {d20, d22, d24}, [r0]!
          vst3.32         {d21, d23, d25}, [r0]!

        @ calculate values for the last (e.g. 3rd) set
          vmul.f32        q10, q0, q3
          vmul.f32        q11, q1, q4
          vmul.f32        q12, q2, q5

        @ store the result for the last (e.g. 3rd) set
          vst3.32         {d20, d22, d24}, [r0]!
          vst3.32         {d21, d23, d25}, [r0]!

.L_check_vec3:
     @ check if anything left to process at the end of the input array
        cmp               r4, #0
        ble               .L_return_vec3

.L_secondloop_vec3:
     @ process the last few items left in the input array
        vld3.f32          {d0[0], d2[0], d4[0]}, [r1]!     @ The values are loaded like so:
                                                           @      q0 = { V1.x, -, -, - };
                                                           @      q1 = { V1.y, -, -, - };
                                                           @      q2 = { V1.z, -, -, - };
        vld3.f32          {d1[0], d3[0], d5[0]}, [r2]!     @ The values are loaded like so:
                                                           @      q0 = { V1.x, -, V2.x, - };
                                                           @      q1 = { V1.y, -, V2.y, - };
                                                           @      q2 = { V1.z, -, V2.z, - };

        subs              r4, r4, #1

        @ calculate values for
        vmul.f32          d0, d0, d1
        vmul.f32          d2, d2, d3
        vmul.f32          d4, d4, d5

        vst3.32           {d0[0], d2[0], d4[0]}, [r0]!

        bgt               .L_secondloop_vec3

.L_return_vec3:
     @ return
        pop               {r4}
        mov               r0, #0
        bx                lr




        .align  2
        .global vmul_vec4f_neon
        .thumb
        .thumb_func
vmul_vec4f_neon:
        @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        @
        @ arm_result_t mul_float(arm_vec4f_t * dst,
        @                 arm_vec4f_t * src1,
        @                 arm_vec4f_t * src2,
        @                 unsigned int count)
        @
        @  r0: *dst & current dst entry's address
        @  r1: *src1 & current src1 entry's address
        @  r2: *src2 & current src2 entry's address
        @  r3: int count & the number of items in the input array that can be
        @                   processed in chunks of 4 vectors
        @
        @  r4:  the number of items that are left to be processed at the end of
        @                   the input array
        @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

        push              {r4}
        and               r4, r3, #3          @ r4 = count % 4;
        sub               r3, r3, r4          @ count = count - r4; This is what's left to be processed after this loop

        cmp               r3, #0
        beq               .L_check_vec4

        @ load the 1st set of values
          vld4.32         {d0, d2, d4, d6}, [r1]!
          vld4.32         {d1, d3, d5, d7}, [r1]!
          vld4.32         {d8, d10, d12, d14}, [r2]!
          vld4.32         {d9, d11, d13, d15}, [r2]!

          subs            r3, r3, #4

        @ calculate values for the 1st set
          vmul.f32        q10, q0, q4
          vmul.f32        q11, q1, q5
          vmul.f32        q12, q2, q6
          vmul.f32        q13, q3, q7

        @ load the 2nd set of values
          vld4.32         {d0, d2, d4, d6}, [r1]!
          vld4.32         {d1, d3, d5, d7}, [r1]!
          vld4.32         {d8, d10, d12, d14}, [r2]!
          vld4.32         {d9, d11, d13, d15}, [r2]!
          subs            r3, r3, #4

          ble             .L_mainloopend_vec4

.L_mainloop_vec4:
        @ store the result for the 1st/next (e.g. 3rd) set
          vst4.32         {d20, d22, d24, d26}, [r0]!
          vst4.32         {d21, d23, d25, d27}, [r0]!

        @ calculate values for the 2nd/next (e.g. 3rd) set
          vmul.f32        q10, q0, q4
          vmul.f32        q11, q1, q5
          vmul.f32        q12, q2, q6
          vmul.f32        q13, q3, q7

       @ load the next (e.g. 3rd) set of values
          vld4.32         {d0, d2, d4, d6}, [r1]!
          vld4.32         {d1, d3, d5, d7}, [r1]!
          vld4.32         {d8, d10, d12, d14}, [r2]!
          vld4.32         {d9, d11, d13, d15}, [r2]!
          subs            r3, r3, #4

        bgt               .L_mainloop_vec4             @ loop if r2 is > r3, if we have at least another 4 vectors (16 floats) to process

.L_mainloopend_vec4:
        @ the last iteration for this call
        @ store the result for the set of values before the last one (e.g 2nd set)
          vst4.32         {d20, d22, d24, d26}, [r0]!
          vst4.32         {d21, d23, d25, d27}, [r0]!

        @ calculate values for the last (e.g. 3rd) set
          vmul.f32        q10, q0, q4
          vmul.f32        q11, q1, q5
          vmul.f32        q12, q2, q6
          vmul.f32        q13, q3, q7

        @ store the result for the last (e.g. 3rd) set
          vst4.32         {d20, d22, d24, d26}, [r0]!
          vst4.32         {d21, d23, d25, d27}, [r0]!

.L_check_vec4:
     @ check if anything left to process at the end of the input array
        cmp               r4, #0
        ble               .L_return_vec4

.L_secondloop_vec4:
     @ process the last few items left in the input array
        vld4.f32          {d0[0], d2[0], d4[0], d6[0]}, [r1]!     @ The values are loaded like so:
                                                                  @      q0 = { V1.x, -, -, - };
                                                                  @      q1 = { V1.y, -, -, - };
                                                                  @      q2 = { V1.z, -, -, - };
                                                                  @      q3 = { V1.w, -, -, - };
        vld4.f32          {d1[0], d3[0], d5[0], d7[0]}, [r2]!     @ The values are loaded like so:
                                                                  @      q0 = { V1.x, -, V2.x, - };
                                                                  @      q1 = { V1.y, -, V2.y, - };
                                                                  @      q2 = { V1.z, -, V2.z, - };
                                                                  @      q3 = { V1.w, -, V2.w, - };

        subs              r4, r4, #1

        @ calculate values
        vmul.f32          d0, d0, d1
        vmul.f32          d2, d2, d3
        vmul.f32          d4, d4, d5
        vmul.f32          d6, d6, d7

        vst4.32          {d0[0], d2[0], d4[0], d6[0]}, [r0]!

        bgt               .L_secondloop_vec4

.L_return_vec4:
     @ return
        pop               {r4}
        mov               r0, #0
        bx                lr
