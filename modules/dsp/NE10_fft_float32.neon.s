/*
 *  Copyright 2013-14 ARM Limited
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    * Neither the name of ARM Limited nor the
 *      names of its contributors may be used to endorse or promote products
 *      derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY ARM LIMITED AND CONTRIBUTORS "AS IS" AND
 *  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL ARM LIMITED BE LIABLE FOR ANY
 *  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * NE10 Library : dsp/NE10_fft_float32.neon.s
 */

        .text
        .syntax   unified

        /* Registers define*/
        /*ARM Registers*/
        p_fout          .req   r0
        p_factors       .req   r1
        p_twiddles      .req   r2
        p_fin           .req   r3
        p_fout0         .req   r4
        p_fout1         .req   r5
        p_fout2         .req   r6
        p_fout3         .req   r7
        stage_count     .req   r8
        fstride         .req   r9
        mstride         .req   r10
        count           .req   r1
        count_f         .req   r1
        count_m         .req   r12
        p_tw1           .req   r3
        p_tw2           .req   r11
        p_tw3           .req   r14
        radix           .req   r5
        tmp0            .req   r12

        /*NEON variale Declaration for the first stage*/
        d_in0_r01       .dn   d0
        d_in0_i01       .dn   d2
        d_in1_r01       .dn   d4
        d_in1_i01       .dn   d6
        d_in0_r23       .dn   d1
        d_in0_i23       .dn   d3
        d_in1_r23       .dn   d5
        d_in1_i23       .dn   d7
        q_in0_r0123     .qn   q0
        q_in0_i0123     .qn   q1
        q_in1_r0123     .qn   q2
        q_in1_i0123     .qn   q3
        d_out0_r01      .dn   d16
        d_out0_i01      .dn   d18
        d_out1_r01      .dn   d20
        d_out1_i01      .dn   d22
        d_out0_r23      .dn   d17
        d_out0_i23      .dn   d19
        d_out1_r23      .dn   d21
        d_out1_i23      .dn   d23
        q_out0_r0123    .qn   q8
        q_out0_i0123    .qn   q9
        q_out1_r0123    .qn   q10
        q_out1_i0123    .qn   q11

        d_in0_0         .dn   d0
        d_in1_0         .dn   d1
        d_in2_0         .dn   d2
        d_in3_0         .dn   d3
        d_in0_1         .dn   d4
        d_in1_1         .dn   d5
        d_in2_1         .dn   d6
        d_in3_1         .dn   d7
        q_in0_01        .qn   q0
        q_in1_01        .qn   q2
        q_in2_01        .qn   q1
        q_in3_01        .qn   q3
        d_out0_0        .dn   d16
        d_out1_0        .dn   d17
        d_out2_0        .dn   d18
        d_out3_0        .dn   d19
        d_out0_1        .dn   d20
        d_out1_1        .dn   d21
        d_out2_1        .dn   d22
        d_out3_1        .dn   d23
        q_out0_01       .qn   q8
        q_out1_01       .qn   q10
        q_out2_01       .qn   q9
        q_out3_01       .qn   q11
        d_s0            .dn   d24
        q_s0_01         .qn   q12
        d_s1            .dn   d26
        q_s1_01         .qn   q13
        d_s2            .dn   d28
        q_s2_01         .qn   q14

        /*NEON variale Declaration for mstride loop */
        q_fin0_r        .qn   q0
        q_fin0_i        .qn   q1
        q_fin1_r        .qn   q0
        q_fin1_i        .qn   q1
        q_tw1_r         .qn   q2
        q_tw1_i         .qn   q3
        q_fin2_r        .qn   q8
        q_fin2_i        .qn   q9
        q_tw2_r         .qn   q10
        q_tw2_i         .qn   q11
        q_fin3_r        .qn   q4
        q_fin3_i        .qn   q5
        q_tw3_r         .qn   q6
        q_tw3_i         .qn   q7
        q_s0_r          .qn   q12
        q_s0_i          .qn   q13
        q_s1_r          .qn   q14
        q_s1_i          .qn   q15
        q_s2_r          .qn   q2
        q_s2_i          .qn   q10
        q_s5_r          .qn   q4
        q_s5_i          .qn   q5
        q_s4_r          .qn   q6
        q_s4_i          .qn   q7
        q_s3_r          .qn   q8
        q_s3_i          .qn   q9
        q_fout0_r       .qn   q0
        q_fout0_i       .qn   q1
        q_fout2_r       .qn   q2
        q_fout2_i       .qn   q3
        q_fout1_r       .qn   q12
        q_fout1_i       .qn   q13
        q_fout3_r       .qn   q14
        q_fout3_i       .qn   q15

        /*NEON variale Declaration for mstride 2 loop */
        d_tw1_r01       .dn   d16
        d_tw2_r01       .dn   d17
        d_tw1_i01       .dn   d18
        d_tw2_i01       .dn   d19
        d_tw3_r01       .dn   d20
        d_tw3_i01       .dn   d21
        q_fin0_r0123    .qn   q0
        q_fin0_i0123    .qn   q1
        d_fin0_r01      .dn   d0
        d_fin1_r01      .dn   d1
        d_fin0_i01      .dn   d2
        d_fin1_i01      .dn   d3
        d_fin2_r01      .dn   d4
        d_fin3_r01      .dn   d5
        d_fin2_i01      .dn   d6
        d_fin3_i01      .dn   d7
        d_fin0_r23      .dn   d22
        d_fin1_r23      .dn   d23
        d_fin0_i23      .dn   d24
        d_fin1_i23      .dn   d25
        d_fin2_r23      .dn   d26
        d_fin3_r23      .dn   d27
        d_fin2_i23      .dn   d28
        d_fin3_i23      .dn   d29
        q_s0_r0123      .qn   q13
        q_s0_i0123      .qn   q14
        d_s0_r01        .dn   d26
        d_s0_r23        .dn   d27
        d_s0_i01        .dn   d28
        d_s0_i23        .dn   d29
        q_s1_r0123      .qn   q5
        q_s1_i0123      .qn   q6
        d_s1_r01        .dn   d10
        d_s1_r23        .dn   d11
        d_s1_i01        .dn   d12
        d_s1_i23        .dn   d13
        q_s2_r0123      .qn   q15
        q_s2_i0123      .qn   q4
        d_s2_r01        .dn   d30
        d_s2_r23        .dn   d31
        d_s2_i01        .dn   d8
        d_s2_i23        .dn   d9
        q_s5_r0123      .qn   q11
        q_s5_i0123      .qn   q12
        q_s4_r0123      .qn   q5
        q_s4_i0123      .qn   q10
        q_s3_r0123      .qn   q6
        q_s3_i0123      .qn   q7
        q_fout0_r0123   .qn   q0
        q_fout0_i0123   .qn   q1
        q_fout2_r0123   .qn   q2
        q_fout2_i0123   .qn   q3
        q_fout1_r0123   .qn   q13
        q_fout1_i0123   .qn   q14
        q_fout3_r0123   .qn   q6
        q_fout3_i0123   .qn   q7
        d_fout0_r01     .dn   d0
        d_fout1_r01     .dn   d1
        d_fout0_i01     .dn   d2
        d_fout1_i01     .dn   d3
        d_fout2_r01     .dn   d4
        d_fout3_r01     .dn   d5
        d_fout2_i01     .dn   d6
        d_fout3_i01     .dn   d7
        d_fout0_r23     .dn   d26
        d_fout1_r23     .dn   d27
        d_fout0_i23     .dn   d28
        d_fout1_i23     .dn   d29
        d_fout2_r23     .dn   d12
        d_fout3_r23     .dn   d13
        d_fout2_i23     .dn   d14
        d_fout3_i23     .dn   d15

        d_tmp0          .dn   d30
        d_tmp1          .dn   d31
        q_tmp           .qn   q15
        d_tmp2_0        .dn   d28
        d_tmp2_1        .dn   d29
        q_tmp2          .qn   q14

        .macro RADIX4_BUTTERFLY_P4
        vld2.32         {q_fin1_r, q_fin1_i}, [p_fout1]
        vld2.32         {q_tw1_r, q_tw1_i}, [p_tw1]!
        vld2.32         {q_fin2_r, q_fin2_i}, [p_fout2]
        vld2.32         {q_tw2_r, q_tw2_i}, [p_tw2]!
        vld2.32         {q_fin3_r, q_fin3_i}, [p_fout3]
        vld2.32         {q_tw3_r, q_tw3_i}, [p_tw3]!

        vmul.f32        q_s0_r, q_fin1_r, q_tw1_r
        vmul.f32        q_s0_i, q_fin1_i, q_tw1_r
        vmul.f32        q_s1_r, q_fin2_r, q_tw2_r
        vmul.f32        q_s1_i, q_fin2_i, q_tw2_r
        vmul.f32        q_s2_r, q_fin3_r, q_tw3_r
        vmul.f32        q_s2_i, q_fin3_i, q_tw3_r
        vmls.f32        q_s0_r, q_fin1_i, q_tw1_i
        vmla.f32        q_s0_i, q_fin1_r, q_tw1_i
        vmls.f32        q_s1_r, q_fin2_i, q_tw2_i
        vmla.f32        q_s1_i, q_fin2_r, q_tw2_i
        vld2.32         {q_fin0_r, q_fin0_i}, [p_fout0]
        vmls.f32        q_s2_r, q_fin3_i, q_tw3_i
        vmla.f32        q_s2_i, q_fin3_r, q_tw3_i

        vsub.f32        q_s5_r, q_fin0_r, q_s1_r
        vsub.f32        q_s5_i, q_fin0_i, q_s1_i
        vadd.f32        q_fout0_r, q_fin0_r, q_s1_r
        vadd.f32        q_fout0_i, q_fin0_i, q_s1_i

        vadd.f32        q_s3_r, q_s0_r, q_s2_r
        vadd.f32        q_s3_i, q_s0_i, q_s2_i
        vsub.f32        q_s4_r, q_s0_r, q_s2_r
        vsub.f32        q_s4_i, q_s0_i, q_s2_i

        vsub.f32        q_fout2_r, q_fout0_r, q_s3_r
        vsub.f32        q_fout2_i, q_fout0_i, q_s3_i
        vadd.f32        q_fout0_r, q_fout0_r, q_s3_r
        vadd.f32        q_fout0_i, q_fout0_i, q_s3_i

        vadd.f32        q_fout1_r, q_s5_r, q_s4_i
        vsub.f32        q_fout1_i, q_s5_i, q_s4_r
        vsub.f32        q_fout3_r, q_s5_r, q_s4_i
        vadd.f32        q_fout3_i, q_s5_i, q_s4_r

        vst2.32         {q_fout2_r, q_fout2_i}, [p_fout2]!
        vst2.32         {q_fout0_r, q_fout0_i}, [p_fout0]!
        vst2.32         {q_fout1_r, q_fout1_i}, [p_fout1]!
        vst2.32         {q_fout3_r, q_fout3_i}, [p_fout3]!
        .endm

        .macro RADIX4_BUTTERFLY_INVERSE_P4
        vld2.32         {q_fin1_r, q_fin1_i}, [p_fout1]
        vld2.32         {q_tw1_r, q_tw1_i}, [p_tw1]!
        vld2.32         {q_fin2_r, q_fin2_i}, [p_fout2]
        vld2.32         {q_tw2_r, q_tw2_i}, [p_tw2]!
        vmul.f32        q_s0_r, q_fin1_r, q_tw1_r
        vmul.f32        q_s0_i, q_fin1_i, q_tw1_r
        vmla.f32        q_s0_r, q_fin1_i, q_tw1_i
        vmls.f32        q_s0_i, q_fin1_r, q_tw1_i

        vld2.32         {q_fin3_r, q_fin3_i}, [p_fout3]
        vld2.32         {q_tw3_r, q_tw3_i}, [p_tw3]!
        vmul.f32        q_s1_r, q_fin2_r, q_tw2_r
        vmul.f32        q_s1_i, q_fin2_i, q_tw2_r
        vmla.f32        q_s1_r, q_fin2_i, q_tw2_i
        vmls.f32        q_s1_i, q_fin2_r, q_tw2_i

        vld2.32         {q_fin0_r, q_fin0_i}, [p_fout0]
        vmul.f32        q_s2_r, q_fin3_r, q_tw3_r
        vmul.f32        q_s2_i, q_fin3_i, q_tw3_r
        vmla.f32        q_s2_r, q_fin3_i, q_tw3_i
        vmls.f32        q_s2_i, q_fin3_r, q_tw3_i

        vsub.f32        q_s5_r, q_fin0_r, q_s1_r
        vsub.f32        q_s5_i, q_fin0_i, q_s1_i
        vadd.f32        q_fout0_r, q_fin0_r, q_s1_r
        vadd.f32        q_fout0_i, q_fin0_i, q_s1_i

        vadd.f32        q_s3_r, q_s0_r, q_s2_r
        vadd.f32        q_s3_i, q_s0_i, q_s2_i
        vsub.f32        q_s4_r, q_s0_r, q_s2_r
        vsub.f32        q_s4_i, q_s0_i, q_s2_i

        vsub.f32        q_fout2_r, q_fout0_r, q_s3_r
        vsub.f32        q_fout2_i, q_fout0_i, q_s3_i
        vadd.f32        q_fout0_r, q_fout0_r, q_s3_r
        vadd.f32        q_fout0_i, q_fout0_i, q_s3_i
        vst2.32         {q_fout2_r, q_fout2_i}, [p_fout2]!

        vsub.f32        q_fout1_r, q_s5_r, q_s4_i
        vadd.f32        q_fout1_i, q_s5_i, q_s4_r
        vadd.f32        q_fout3_r, q_s5_r, q_s4_i
        vsub.f32        q_fout3_i, q_s5_i, q_s4_r
        vst2.32         {q_fout0_r, q_fout0_i}, [p_fout0]!
        vst2.32         {q_fout1_r, q_fout1_i}, [p_fout1]!
        vst2.32         {q_fout3_r, q_fout3_i}, [p_fout3]!
        .endm

        .macro RADIX24_BUTTERFLY_P4
        vld2.32         {d_tw3_r01, d_tw3_i01}, [p_tw1]
        vld2.32         {d_fin0_r01, d_fin1_r01, d_fin0_i01, d_fin1_i01}, [p_fout0]!
        vld2.32         {d_fin2_r01, d_fin3_r01, d_fin2_i01, d_fin3_i01}, [p_fout0], tmp0
        vld2.32         {d_fin0_r23, d_fin1_r23, d_fin0_i23, d_fin1_i23}, [p_fout1]!
        vld2.32         {d_fin2_r23, d_fin3_r23, d_fin2_i23, d_fin3_i23}, [p_fout1], tmp0

        vmul.f32       d_s2_r01, d_fin3_r01, d_tw3_r01
        vmul.f32       d_s2_i01, d_fin3_r01, d_tw3_i01
        vmul.f32       d_s2_r23, d_fin3_r23, d_tw3_r01
        vmul.f32       d_s2_i23, d_fin3_r23, d_tw3_i01
        vmls.f32       d_s2_r01, d_fin3_i01, d_tw3_i01
        vmla.f32       d_s2_i01, d_fin3_i01, d_tw3_r01
        vmls.f32       d_s2_r23, d_fin3_i23, d_tw3_i01
        vmla.f32       d_s2_i23, d_fin3_i23, d_tw3_r01

        vmul.f32       d_s1_r01, d_fin2_r01, d_tw2_r01
        vmul.f32       d_s1_r23, d_fin2_r23, d_tw2_r01
        vmul.f32       d_s1_i01, d_fin2_r01, d_tw2_i01
        vmul.f32       d_s1_i23, d_fin2_r23, d_tw2_i01
        vmls.f32       d_s1_r01, d_fin2_i01, d_tw2_i01
        vmls.f32       d_s1_r23, d_fin2_i23, d_tw2_i01
        vmla.f32       d_s1_i01, d_fin2_i01, d_tw2_r01
        vmla.f32       d_s1_i23, d_fin2_i23, d_tw2_r01

        vmul.f32       d_s0_r01, d_fin1_r01, d_tw1_r01
        vmul.f32       d_s0_r23, d_fin1_r23, d_tw1_r01
        vmul.f32       d_s0_i01, d_fin1_r01, d_tw1_i01
        vmul.f32       d_s0_i23, d_fin1_r23, d_tw1_i01
        vmls.f32       d_s0_r01, d_fin1_i01, d_tw1_i01
        vmls.f32       d_s0_r23, d_fin1_i23, d_tw1_i01
        vmla.f32       d_s0_i01, d_fin1_i01, d_tw1_r01
        vmla.f32       d_s0_i23, d_fin1_i23, d_tw1_r01

        vmov            d_fin1_r01, d_fin0_r23
        vmov            d_fin1_i01, d_fin0_i23

        vsub.f32        q_s5_r0123, q_fin0_r0123, q_s1_r0123
        vsub.f32        q_s5_i0123, q_fin0_i0123, q_s1_i0123
        vadd.f32        q_fout0_r0123, q_fin0_r0123, q_s1_r0123
        vadd.f32        q_fout0_i0123, q_fin0_i0123, q_s1_i0123

        vadd.f32        q_s3_r0123, q_s0_r0123, q_s2_r0123
        vadd.f32        q_s3_i0123, q_s0_i0123, q_s2_i0123
        vsub.f32        q_s4_r0123, q_s0_r0123, q_s2_r0123
        vsub.f32        q_s4_i0123, q_s0_i0123, q_s2_i0123
        vsub.f32        q_fout2_r0123, q_fout0_r0123, q_s3_r0123
        vsub.f32        q_fout2_i0123, q_fout0_i0123, q_s3_i0123
        vadd.f32        q_fout0_r0123, q_fout0_r0123, q_s3_r0123
        vadd.f32        q_fout0_i0123, q_fout0_i0123, q_s3_i0123

        vadd.f32        q_fout1_r0123, q_s5_r0123, q_s4_i0123
        vsub.f32        q_fout1_i0123, q_s5_i0123, q_s4_r0123
        vsub.f32        q_fout3_r0123, q_s5_r0123, q_s4_i0123
        vadd.f32        q_fout3_i0123, q_s5_i0123, q_s4_r0123

        vswp            d_fout1_r01, d_fout0_r23
        vswp            d_fout1_i01, d_fout0_i23
        vswp            d_fout3_r01, d_fout2_r23
        vswp            d_fout3_i01, d_fout2_i23

        vst2.32         {d_fout0_r01, d_fout1_r01, d_fout0_i01, d_fout1_i01}, [p_fout2]!
        vst2.32         {d_fout0_r23, d_fout1_r23, d_fout0_i23, d_fout1_i23}, [p_fout3]!
        vst2.32         {d_fout2_r01, d_fout3_r01, d_fout2_i01, d_fout3_i01}, [p_fout2], tmp0
        vst2.32         {d_fout2_r23, d_fout3_r23, d_fout2_i23, d_fout3_i23}, [p_fout3], tmp0
        .endm

        .macro RADIX24_BUTTERFLY_INVERSE_P4
        vld2.32         {d_tw3_r01, d_tw3_i01}, [p_tw1]
        vld2.32         {d_fin0_r01, d_fin1_r01, d_fin0_i01, d_fin1_i01}, [p_fout0]!
        vld2.32         {d_fin2_r01, d_fin3_r01, d_fin2_i01, d_fin3_i01}, [p_fout0], tmp0
        vld2.32         {d_fin0_r23, d_fin1_r23, d_fin0_i23, d_fin1_i23}, [p_fout1]!
        vld2.32         {d_fin2_r23, d_fin3_r23, d_fin2_i23, d_fin3_i23}, [p_fout1], tmp0

        vmul.f32       d_s2_r01, d_fin3_r01, d_tw3_r01
        vmul.f32       d_s2_i01, d_fin3_i01, d_tw3_r01
        vmul.f32       d_s2_r23, d_fin3_r23, d_tw3_r01
        vmul.f32       d_s2_i23, d_fin3_i23, d_tw3_r01
        vmla.f32       d_s2_r01, d_fin3_i01, d_tw3_i01
        vmls.f32       d_s2_i01, d_fin3_r01, d_tw3_i01
        vmla.f32       d_s2_r23, d_fin3_i23, d_tw3_i01
        vmls.f32       d_s2_i23, d_fin3_r23, d_tw3_i01

        vmul.f32       d_s1_r01, d_fin2_r01, d_tw2_r01
        vmul.f32       d_s1_r23, d_fin2_r23, d_tw2_r01
        vmul.f32       d_s1_i01, d_fin2_i01, d_tw2_r01
        vmul.f32       d_s1_i23, d_fin2_i23, d_tw2_r01
        vmla.f32       d_s1_r01, d_fin2_i01, d_tw2_i01
        vmla.f32       d_s1_r23, d_fin2_i23, d_tw2_i01
        vmls.f32       d_s1_i01, d_fin2_r01, d_tw2_i01
        vmls.f32       d_s1_i23, d_fin2_r23, d_tw2_i01

        vmul.f32       d_s0_r01, d_fin1_r01, d_tw1_r01
        vmul.f32       d_s0_r23, d_fin1_r23, d_tw1_r01
        vmul.f32       d_s0_i01, d_fin1_i01, d_tw1_r01
        vmul.f32       d_s0_i23, d_fin1_i23, d_tw1_r01
        vmla.f32       d_s0_r01, d_fin1_i01, d_tw1_i01
        vmla.f32       d_s0_r23, d_fin1_i23, d_tw1_i01
        vmls.f32       d_s0_i01, d_fin1_r01, d_tw1_i01
        vmls.f32       d_s0_i23, d_fin1_r23, d_tw1_i01

        vmov            d_fin1_r01, d_fin0_r23
        vmov            d_fin1_i01, d_fin0_i23

        vsub.f32        q_s5_r0123, q_fin0_r0123, q_s1_r0123
        vsub.f32        q_s5_i0123, q_fin0_i0123, q_s1_i0123
        vadd.f32        q_fout0_r0123, q_fin0_r0123, q_s1_r0123
        vadd.f32        q_fout0_i0123, q_fin0_i0123, q_s1_i0123

        vadd.f32        q_s3_r0123, q_s0_r0123, q_s2_r0123
        vadd.f32        q_s3_i0123, q_s0_i0123, q_s2_i0123
        vsub.f32        q_s4_r0123, q_s0_r0123, q_s2_r0123
        vsub.f32        q_s4_i0123, q_s0_i0123, q_s2_i0123
        vsub.f32        q_fout2_r0123, q_fout0_r0123, q_s3_r0123
        vsub.f32        q_fout2_i0123, q_fout0_i0123, q_s3_i0123
        vadd.f32        q_fout0_r0123, q_fout0_r0123, q_s3_r0123
        vadd.f32        q_fout0_i0123, q_fout0_i0123, q_s3_i0123

        vsub.f32        q_fout1_r0123, q_s5_r0123, q_s4_i0123
        vadd.f32        q_fout1_i0123, q_s5_i0123, q_s4_r0123
        vadd.f32        q_fout3_r0123, q_s5_r0123, q_s4_i0123
        vsub.f32        q_fout3_i0123, q_s5_i0123, q_s4_r0123

        vswp            d_fout1_r01, d_fout0_r23
        vswp            d_fout1_i01, d_fout0_i23
        vswp            d_fout3_r01, d_fout2_r23
        vswp            d_fout3_i01, d_fout2_i23

        vst2.32         {d_fout0_r01, d_fout1_r01, d_fout0_i01, d_fout1_i01}, [p_fout2]!
        vst2.32         {d_fout0_r23, d_fout1_r23, d_fout0_i23, d_fout1_i23}, [p_fout3]!
        vst2.32         {d_fout2_r01, d_fout3_r01, d_fout2_i01, d_fout3_i01}, [p_fout2], tmp0
        vst2.32         {d_fout2_r23, d_fout3_r23, d_fout2_i23, d_fout3_i23}, [p_fout3], tmp0
        .endm


        .align 4
        .global ne10_radix4_butterfly_forward_float32_neon
        .thumb
        .thumb_func

ne10_radix4_butterfly_forward_float32_neon:

        push            {r4-r12,lr}

        ldr             stage_count, [p_factors]   /* get factors[0]---stage_count */
        ldr             fstride, [p_factors, #4]   /* get factors[1]---fstride */
        add             p_factors, p_factors, stage_count, lsl #3 /* get the address of factors[2*stage_count] */
        ldr             mstride, [p_factors, #-4]                  /* get factors[2*stage_count-1]--- mstride */
        sub             stage_count, stage_count, #1

        /* loop of the stages  */
.L_ne10_radix4_butterfly_forward_stages:
        lsr             fstride, fstride, #2

        /* loop of fstride  */
        mov             count_f, fstride
.L_ne10_radix4_butterfly_forward_stages_fstride:
        sub             tmp0, fstride, count_f
        mul             tmp0, tmp0, mstride
        add             p_fout0, p_fout, tmp0, lsl #5
        add             p_fout2, p_fout0, mstride, lsl #4   /* get the address of F[mstride*2] */
        add             p_fout1, p_fout0, mstride, lsl #3   /* get the address of F[mstride] */
        add             p_fout3, p_fout2, mstride, lsl #3   /* get the address of F[mstride*3] */
        mov             p_tw1, p_twiddles
        add             p_tw2, p_tw1, mstride, lsl #3       /* get the address of tw2 */
        add             p_tw3, p_tw1, mstride, lsl #4       /* get the address of tw3 */

        /* loop of mstride  */
        mov             count_m, mstride

.L_ne10_radix4_butterfly_forward_stages_mstride:

        RADIX4_BUTTERFLY_P4

        subs            count_m, count_m, #4
        bgt             .L_ne10_radix4_butterfly_forward_stages_mstride

        /* end of mstride_loop */

        subs            count_f, count_f, #1
        bgt             .L_ne10_radix4_butterfly_forward_stages_fstride

        add             p_twiddles, p_twiddles, mstride, lsl #4
        add             p_twiddles, p_twiddles, mstride, lsl #3 /* get the address of twiddles += mstride*3 */
        lsl             mstride, mstride, #2

        subs            stage_count, stage_count, #1
        bgt             .L_ne10_radix4_butterfly_forward_stages

.L_ne10_radix4_butterfly_forward_end:
        /*Return From Function*/
        pop             {r4-r12,pc}


        .align 4
        .global ne10_radix2_butterfly_forward_float32_neon
        .thumb
        .thumb_func

ne10_radix2_butterfly_forward_float32_neon:

        push            {r4-r12,lr}
        vpush           {q4-q7}

        ldr             stage_count, [p_factors]   /* get factors[0]---stage_count */
        ldr             fstride, [p_factors, #4]   /* get factors[1]---fstride */
        add             p_factors, p_factors, stage_count, lsl #3 /* get the address of factors[2*stage_count] */
        ldr             mstride, [p_factors, #-4]                  /* get factors[2*stage_count-1]--- mstride */
        sub             stage_count, stage_count, #2


        /* loop of the second stages  */
.L_ne10_radix2_butterfly_forwards_second_stage:
        lsr             fstride, fstride, #2

        /* loop of fstride  */
        mov             count_f, fstride
        mov             p_tw1, p_twiddles
        mov             p_fout0, p_fout
        add             p_fout1, p_fout, mstride, lsl #5
        mov             p_fout2, p_fout
        mov             p_fout3, p_fout1
        mov             tmp0, #96
        vld2.32         {d_tw1_r01, d_tw2_r01, d_tw1_i01, d_tw2_i01}, [p_tw1]!

.L_ne10_radix2_butterfly_forwards_second_stage_fstride:
        @RADIX24_BUTTERFLY_P4
        vld2.32         {d_tw3_r01, d_tw3_i01}, [p_tw1]
        vld2.32         {d_fin0_r01, d_fin1_r01, d_fin0_i01, d_fin1_i01}, [p_fout0]!
        vld2.32         {d_fin2_r01, d_fin3_r01, d_fin2_i01, d_fin3_i01}, [p_fout0], tmp0
        vld2.32         {d_fin0_r23, d_fin1_r23, d_fin0_i23, d_fin1_i23}, [p_fout1]!
        vld2.32         {d_fin2_r23, d_fin3_r23, d_fin2_i23, d_fin3_i23}, [p_fout1], tmp0

        vmul.f32       d_s2_r01, d_fin3_r01, d_tw3_r01
        vmul.f32       d_s2_i01, d_fin3_r01, d_tw3_i01
        vmul.f32       d_s2_r23, d_fin3_r23, d_tw3_r01
        vmul.f32       d_s2_i23, d_fin3_r23, d_tw3_i01
        vmls.f32       d_s2_r01, d_fin3_i01, d_tw3_i01
        vmla.f32       d_s2_i01, d_fin3_i01, d_tw3_r01
        vmls.f32       d_s2_r23, d_fin3_i23, d_tw3_i01
        vmla.f32       d_s2_i23, d_fin3_i23, d_tw3_r01

        vmul.f32       d_s1_r01, d_fin2_r01, d_tw2_r01
        vmul.f32       d_s1_r23, d_fin2_r23, d_tw2_r01
        vmul.f32       d_s1_i01, d_fin2_r01, d_tw2_i01
        vmul.f32       d_s1_i23, d_fin2_r23, d_tw2_i01
        vmls.f32       d_s1_r01, d_fin2_i01, d_tw2_i01
        vmls.f32       d_s1_r23, d_fin2_i23, d_tw2_i01
        vmla.f32       d_s1_i01, d_fin2_i01, d_tw2_r01
        vmla.f32       d_s1_i23, d_fin2_i23, d_tw2_r01

        vmul.f32       d_s0_r01, d_fin1_r01, d_tw1_r01
        vmul.f32       d_s0_r23, d_fin1_r23, d_tw1_r01
        vmul.f32       d_s0_i01, d_fin1_r01, d_tw1_i01
        vmul.f32       d_s0_i23, d_fin1_r23, d_tw1_i01
        vmls.f32       d_s0_r01, d_fin1_i01, d_tw1_i01
        vmls.f32       d_s0_r23, d_fin1_i23, d_tw1_i01
        vmla.f32       d_s0_i01, d_fin1_i01, d_tw1_r01
        vmla.f32       d_s0_i23, d_fin1_i23, d_tw1_r01

        vmov            d_fin1_r01, d_fin0_r23
        vmov            d_fin1_i01, d_fin0_i23

        vsub.f32        q_s5_r0123, q_fin0_r0123, q_s1_r0123
        vsub.f32        q_s5_i0123, q_fin0_i0123, q_s1_i0123
        vadd.f32        q_fout0_r0123, q_fin0_r0123, q_s1_r0123
        vadd.f32        q_fout0_i0123, q_fin0_i0123, q_s1_i0123

        vadd.f32        q_s3_r0123, q_s0_r0123, q_s2_r0123
        vadd.f32        q_s3_i0123, q_s0_i0123, q_s2_i0123
        vsub.f32        q_s4_r0123, q_s0_r0123, q_s2_r0123
        vsub.f32        q_s4_i0123, q_s0_i0123, q_s2_i0123
        vsub.f32        q_fout2_r0123, q_fout0_r0123, q_s3_r0123
        vsub.f32        q_fout2_i0123, q_fout0_i0123, q_s3_i0123
        vadd.f32        q_fout0_r0123, q_fout0_r0123, q_s3_r0123
        vadd.f32        q_fout0_i0123, q_fout0_i0123, q_s3_i0123

        vadd.f32        q_fout1_r0123, q_s5_r0123, q_s4_i0123
        vsub.f32        q_fout1_i0123, q_s5_i0123, q_s4_r0123
        vsub.f32        q_fout3_r0123, q_s5_r0123, q_s4_i0123
        vadd.f32        q_fout3_i0123, q_s5_i0123, q_s4_r0123

        vswp            d_fout1_r01, d_fout0_r23
        vswp            d_fout1_i01, d_fout0_i23
        vswp            d_fout3_r01, d_fout2_r23
        vswp            d_fout3_i01, d_fout2_i23

        vst2.32         {d_fout0_r01, d_fout1_r01, d_fout0_i01, d_fout1_i01}, [p_fout2]!
        vst2.32         {d_fout0_r23, d_fout1_r23, d_fout0_i23, d_fout1_i23}, [p_fout3]!
        vst2.32         {d_fout2_r01, d_fout3_r01, d_fout2_i01, d_fout3_i01}, [p_fout2], tmp0
        vst2.32         {d_fout2_r23, d_fout3_r23, d_fout2_i23, d_fout3_i23}, [p_fout3], tmp0
        subs            count_f, count_f, #2
        bgt             .L_ne10_radix2_butterfly_forwards_second_stage_fstride

        add             p_twiddles, p_twiddles, mstride, lsl #4
        add             p_twiddles, p_twiddles, mstride, lsl #3 /* get the address of twiddles += mstride*3 */
        lsl             mstride, mstride, #2


        /* loop of the other stages  */
.L_ne10_radix2_butterfly_forwards_stages:
        lsr             fstride, fstride, #2

        /* loop of fstride  */
        mov             count_f, fstride
.L_ne10_radix2_butterfly_forwards_stages_fstride:
        sub             tmp0, fstride, count_f
        mul             tmp0, tmp0, mstride
        add             p_fout0, p_fout, tmp0, lsl #5
        add             p_fout2, p_fout0, mstride, lsl #4   /* get the address of F[mstride*2] */
        add             p_fout1, p_fout0, mstride, lsl #3   /* get the address of F[mstride] */
        add             p_fout3, p_fout2, mstride, lsl #3   /* get the address of F[mstride*3] */
        mov             p_tw1, p_twiddles
        add             p_tw2, p_tw1, mstride, lsl #3       /* get the address of tw2 */
        add             p_tw3, p_tw1, mstride, lsl #4       /* get the address of tw3 */

        /* loop of mstride  */
        mov             count_m, mstride

.L_ne10_radix2_butterfly_forwards_stages_mstride:
        RADIX4_BUTTERFLY_P4

        subs            count_m, count_m, #4
        bgt             .L_ne10_radix2_butterfly_forwards_stages_mstride

        /* end of mstride_loop */

        subs            count_f, count_f, #1
        bgt             .L_ne10_radix2_butterfly_forwards_stages_fstride

        add             p_twiddles, p_twiddles, mstride, lsl #4
        add             p_twiddles, p_twiddles, mstride, lsl #3 /* get the address of twiddles += mstride*3 */
        lsl             mstride, mstride, #2

        subs            stage_count, stage_count, #1
        bgt             .L_ne10_radix2_butterfly_forwards_stages

.L_ne10_radix2_butterfly_forwards_end:
        /*Return From Function*/
        vpop            {q4-q7}
        pop             {r4-r12,pc}

        .align 4
        .global ne10_radix2_butterfly_backward_float32_neon
        .thumb
        .thumb_func

ne10_radix2_butterfly_backward_float32_neon:

        push            {r4-r12,lr}
        vpush           {q4-q7}

        ldr             stage_count, [p_factors]   /* get factors[0]---stage_count */
        ldr             fstride, [p_factors, #4]   /* get factors[1]---fstride */
        add             p_factors, p_factors, stage_count, lsl #3 /* get the address of factors[2*stage_count] */
        ldr             mstride, [p_factors, #-4]                  /* get factors[2*stage_count-1]--- mstride */
        sub             stage_count, stage_count, #2


        /* loop of the second stages  */
.L_ne10_radix2_butterfly_backward_second_stage:
        lsr             fstride, fstride, #2

        /* loop of fstride  */
        mov             count_f, fstride
        mov             p_tw1, p_twiddles
        mov             p_fout0, p_fout
        add             p_fout1, p_fout, mstride, lsl #5
        mov             p_fout2, p_fout
        mov             p_fout3, p_fout1
        mov             tmp0, #96
        vld2.32         {d_tw1_r01, d_tw2_r01, d_tw1_i01, d_tw2_i01}, [p_tw1]!

.L_ne10_radix2_butterfly_backward_second_stage_fstride:
        RADIX24_BUTTERFLY_INVERSE_P4

        subs            count_f, count_f, #2
        bgt             .L_ne10_radix2_butterfly_backward_second_stage_fstride

        add             p_twiddles, p_twiddles, mstride, lsl #4
        add             p_twiddles, p_twiddles, mstride, lsl #3 /* get the address of twiddles += mstride*3 */
        lsl             mstride, mstride, #2


        /* loop of the other stages  */
.L_ne10_radix2_butterfly_backward_stages:
        lsr             fstride, fstride, #2

        /* loop of fstride  */
        mov             count_f, fstride
.L_ne10_radix2_butterfly_backward_stages_fstride:
        sub             tmp0, fstride, count_f
        mul             tmp0, tmp0, mstride
        add             p_fout0, p_fout, tmp0, lsl #5
        add             p_fout2, p_fout0, mstride, lsl #4   /* get the address of F[mstride*2] */
        add             p_fout1, p_fout0, mstride, lsl #3   /* get the address of F[mstride] */
        add             p_fout3, p_fout2, mstride, lsl #3   /* get the address of F[mstride*3] */
        mov             p_tw1, p_twiddles
        add             p_tw2, p_tw1, mstride, lsl #3       /* get the address of tw2 */
        add             p_tw3, p_tw1, mstride, lsl #4       /* get the address of tw3 */

        /* loop of mstride  */
        mov             count_m, mstride

.L_ne10_radix2_butterfly_backward_stages_mstride:
        RADIX4_BUTTERFLY_INVERSE_P4

        subs            count_m, count_m, #4
        bgt             .L_ne10_radix2_butterfly_backward_stages_mstride

        /* end of mstride_loop */

        subs            count_f, count_f, #1
        bgt             .L_ne10_radix2_butterfly_backward_stages_fstride

        add             p_twiddles, p_twiddles, mstride, lsl #4
        add             p_twiddles, p_twiddles, mstride, lsl #3 /* get the address of twiddles += mstride*3 */
        lsl             mstride, mstride, #2

        subs            stage_count, stage_count, #1
        bgt             .L_ne10_radix2_butterfly_backward_stages


.L_ne10_radix2_butterfly_backward_end:
        /*Return From Function*/
        vpop            {q4-q7}
        pop             {r4-r12,pc}


        .align 4
        .global ne10_radix4_butterfly_backward_float32_neon
        .thumb
        .thumb_func

ne10_radix4_butterfly_backward_float32_neon:

        push            {r4-r12,lr}
        vpush           {q4-q7}

        ldr             stage_count, [p_factors]   /* get factors[0]---stage_count */
        ldr             fstride, [p_factors, #4]   /* get factors[1]---fstride */
        add             p_factors, p_factors, stage_count, lsl #3 /* get the address of factors[2*stage_count] */
        ldr             mstride, [p_factors, #-4]                  /* get factors[2*stage_count-1]--- mstride */
        sub             stage_count, stage_count, #1

        /* loop of the stages  */
.L_ne10_radix4_butterfly_backward_stages:
        lsr             fstride, fstride, #2

        /* loop of fstride  */
        mov             count_f, fstride
.L_ne10_radix4_butterfly_backward_stages_fstride:
        sub             tmp0, fstride, count_f
        mul             tmp0, tmp0, mstride
        add             p_fout0, p_fout, tmp0, lsl #5
        add             p_fout2, p_fout0, mstride, lsl #4   /* get the address of F[mstride*2] */
        add             p_fout1, p_fout0, mstride, lsl #3   /* get the address of F[mstride] */
        add             p_fout3, p_fout2, mstride, lsl #3   /* get the address of F[mstride*3] */
        mov             p_tw1, p_twiddles
        add             p_tw2, p_tw1, mstride, lsl #3       /* get the address of tw2 */
        add             p_tw3, p_tw1, mstride, lsl #4       /* get the address of tw3 */

        /* loop of mstride  */
        mov             count_m, mstride

.L_ne10_radix4_butterfly_backward_stages_mstride:
        RADIX4_BUTTERFLY_INVERSE_P4
        subs            count_m, count_m, #4
        bgt             .L_ne10_radix4_butterfly_backward_stages_mstride

        /* end of mstride_loop */

        subs            count_f, count_f, #1
        bgt             .L_ne10_radix4_butterfly_backward_stages_fstride

        add             p_twiddles, p_twiddles, mstride, lsl #4
        add             p_twiddles, p_twiddles, mstride, lsl #3 /* get the address of twiddles += mstride*3 */
        lsl             mstride, mstride, #2

        subs            stage_count, stage_count, #1
        bgt             .L_ne10_radix4_butterfly_backward_stages


.L_ne10_radix4_inverse_butterfly_backward_end:
        /*Return From Function*/
        vpop            {q4-q7}
        pop             {r4-r12,pc}



        /**
         * @details
         * This function implements the 4 butterfly
         *
         * @param[in/out] *Fout        points to input/output pointers
         * @param[in]     *factors     factors pointer:
                                        * 0: stage number
                                        * 1: stride for the first stage
                                        * others: factor out powers of 4, powers of 2
         * @param[in]     *twiddles     twiddles coeffs of FFT
         */

        .align 4
        .global ne10_mixed_radix_butterfly_length_odd_power2_float32_neon
        .thumb
        .thumb_func

ne10_mixed_radix_butterfly_length_odd_power2_float32_neon:
        push            {r4-r12,lr}
        vpush           {q4-q7}

        ldr             stage_count, [p_factors]   /* get factors[0]---stage_count */
        ldr             fstride, [p_factors, #4]   /* get factors[1]---fstride */
        add             p_factors, p_factors, stage_count, lsl #3 /* get the address of factors[2*stage_count] */
        ldr             radix, [p_factors]                         /* get factors[2*stage_count]--- the first radix */
        ldr             mstride, [p_factors, #-4]                  /* get factors[2*stage_count-1]--- mstride */

        mov             p_fin, p_fout
        mov             p_fout0, p_fout
        mov             count, fstride

        /* the first stage  */
.L_ne10_butterfly_length_odd_power2_first_stage:
        vld4.32         {d_in0_r01, d_in0_i01, d_in1_r01, d_in1_i01}, [p_fin]!
        vld4.32         {d_in0_r23, d_in0_i23, d_in1_r23, d_in1_i23}, [p_fin]!
        vsub.f32        q_out1_r0123, q_in0_r0123, q_in1_r0123
        vsub.f32        q_out1_i0123, q_in0_i0123, q_in1_i0123
        vadd.f32        q_out0_r0123, q_in0_r0123, q_in1_r0123
        vadd.f32        q_out0_i0123, q_in0_i0123, q_in1_i0123
        subs            count, count, #4
        vst4.32         {d_out0_r01, d_out0_i01, d_out1_r01, d_out1_i01}, [p_fout0]!
        vst4.32         {d_out0_r23, d_out0_i23, d_out1_r23, d_out1_i23}, [p_fout0]!

        bgt             .L_ne10_butterfly_length_odd_power2_first_stage

        /* the second stages  */
        subs            stage_count, stage_count, #1
        lsr             fstride, fstride, #2

        /* loop of fstride  */
        mov             count_f, fstride
        mov             p_tw1, p_twiddles
        mov             p_fout0, p_fout
        add             p_fout1, p_fout, mstride, lsl #5
        mov             p_fout2, p_fout
        mov             p_fout3, p_fout1
        mov             tmp0, #96
        vld2.32         {d_tw1_r01, d_tw2_r01, d_tw1_i01, d_tw2_i01}, [p_tw1]!

.L_ne10_butterfly_length_odd_power2_second_stage:
        RADIX24_BUTTERFLY_P4

        subs            count_f, count_f, #2
        bgt             .L_ne10_butterfly_length_odd_power2_second_stage

        add             p_twiddles, p_twiddles, mstride, lsl #4
        add             p_twiddles, p_twiddles, mstride, lsl #3 /* get the address of twiddles += mstride*3 */
        lsl             mstride, mstride, #2

        /* other stages  */
        subs            stage_count, stage_count, #1

        /* loop of other stages  */
.L_ne10_butterfly_length_odd_power2_other_stages:
        lsr             fstride, fstride, #2

        /* loop of fstride  */
        mov             count_f, fstride
.L_ne10_butterfly_length_odd_power2_other_stages_fstride:
        sub             tmp0, fstride, count_f
        mul             tmp0, tmp0, mstride
        add             p_fout0, p_fout, tmp0, lsl #5
        add             p_fout2, p_fout0, mstride, lsl #4   /* get the address of F[mstride*2] */
        add             p_fout1, p_fout0, mstride, lsl #3   /* get the address of F[mstride] */
        add             p_fout3, p_fout2, mstride, lsl #3   /* get the address of F[mstride*3] */
        mov             p_tw1, p_twiddles
        add             p_tw2, p_tw1, mstride, lsl #3       /* get the address of tw2 */
        add             p_tw3, p_tw1, mstride, lsl #4       /* get the address of tw3 */

        /* loop of mstride  */
        mov             count_m, mstride

.L_ne10_butterfly_length_odd_power2_other_stages_mstride:
        RADIX4_BUTTERFLY_P4

        subs            count_m, count_m, #4
        bgt             .L_ne10_butterfly_length_odd_power2_other_stages_mstride
        /* end of mstride loop */

        subs            count_f, count_f, #1
        bgt             .L_ne10_butterfly_length_odd_power2_other_stages_fstride

        add             p_twiddles, p_twiddles, mstride, lsl #4
        add             p_twiddles, p_twiddles, mstride, lsl #3 /* get the address of twiddles += mstride*3 */
        lsl             mstride, mstride, #2

        subs            stage_count, stage_count, #1
        bgt             .L_ne10_butterfly_length_odd_power2_other_stages

.L_ne10_butterfly_length_odd_power2_end:
        /*Return From Function*/
        vpop            {q4-q7}
        pop             {r4-r12,pc}

        /* end of ne10_butterfly_length_odd_power2_float32_neon */


        /**
         * @details
         * This function implements the 4 butterfly
         *
         * @param[in/out] *Fout        points to input/output pointers
         * @param[in]     *factors     factors pointer:
                                        * 0: stage number
                                        * 1: stride for the first stage
                                        * others: factor out powers of 4, powers of 2
         * @param[in]     *twiddles     twiddles coeffs of FFT
         */

        .align 4
        .global ne10_mixed_radix_butterfly_length_even_power2_float32_neon
        .thumb
        .thumb_func

ne10_mixed_radix_butterfly_length_even_power2_float32_neon:
        push            {r4-r12,lr}
        vpush           {q4-q7}

        ldr             stage_count, [p_factors]   /* get factors[0]---stage_count */
        ldr             fstride, [p_factors, #4]   /* get factors[1]---fstride */
        add             p_factors, p_factors, stage_count, lsl #3 /* get the address of factors[2*stage_count] */
        ldr             radix, [p_factors]                         /* get factors[2*stage_count]--- the first radix */
        ldr             mstride, [p_factors, #-4]                  /* get factors[2*stage_count-1]--- mstride */

        mov             p_fin, p_fout
        mov             p_fout0, p_fout
        mov             count, fstride

        /* the first stage  */
.L_ne10_butterfly_length_even_power2_first_stage:
        vld1.32         {d_in0_0, d_in1_0, d_in2_0, d_in3_0}, [p_fin]!
        vld1.32         {d_in0_1, d_in1_1, d_in2_1, d_in3_1}, [p_fin]!
        vswp            d_in1_0, d_in0_1
        vswp            d_in3_0, d_in2_1
        vsub.f32        q_s2_01, q_in0_01, q_in2_01
        vadd.f32        q_out0_01, q_in0_01, q_in2_01
        vadd.f32        q_s0_01, q_in1_01, q_in3_01
        vsub.f32        q_s1_01, q_in1_01, q_in3_01
        vsub.f32        q_out2_01, q_out0_01, q_s0_01
        vrev64.32       q_s1_01, q_s1_01
        vadd.f32        q_out0_01, q_out0_01, q_s0_01
        vadd.f32        q_out1_01, q_s2_01, q_s1_01
        vsub.f32        q_out3_01, q_s2_01, q_s1_01
        vrev64.32       q_tmp, q_out1_01
        vrev64.32       q_tmp2, q_out3_01
        vtrn.32         q_out3_01, q_tmp
        vtrn.32         q_out1_01, q_tmp2
        vswp            d_out1_0, d_out0_1
        vswp            d_out3_0, d_out2_1
        subs            count, count, #2
        vst1.32         {d_out0_0, d_out1_0, d_out2_0, d_out3_0}, [p_fout0]!
        vst1.32         {d_out0_1, d_out1_1, d_out2_1, d_out3_1}, [p_fout0]!
        bgt             .L_ne10_butterfly_length_even_power2_first_stage

        /* other stages  */
        subs            stage_count, stage_count, #1

        /* loop of other stages  */
.L_ne10_butterfly_length_even_power2_other_stages:
        lsr             fstride, fstride, #2

        /* loop of fstride  */
        mov             count_f, fstride
.L_ne10_butterfly_length_even_power2_other_stages_fstride:
        sub             tmp0, fstride, count_f
        mul             tmp0, tmp0, mstride
        add             p_fout0, p_fout, tmp0, lsl #5
        add             p_fout2, p_fout0, mstride, lsl #4   /* get the address of F[mstride*2] */
        add             p_fout1, p_fout0, mstride, lsl #3   /* get the address of F[mstride] */
        add             p_fout3, p_fout2, mstride, lsl #3   /* get the address of F[mstride*3] */
        mov             p_tw1, p_twiddles
        add             p_tw2, p_tw1, mstride, lsl #3       /* get the address of tw2 */
        add             p_tw3, p_tw1, mstride, lsl #4       /* get the address of tw3 */

        /* loop of mstride  */
        mov             count_m, mstride

.L_ne10_butterfly_length_even_power2_other_stages_mstride:
        RADIX4_BUTTERFLY_P4        

        subs            count_m, count_m, #4
        bgt             .L_ne10_butterfly_length_even_power2_other_stages_mstride
        /* end of mstride loop */

        subs            count_f, count_f, #1
        bgt             .L_ne10_butterfly_length_even_power2_other_stages_fstride

        add             p_twiddles, p_twiddles, mstride, lsl #4
        add             p_twiddles, p_twiddles, mstride, lsl #3 /* get the address of twiddles += mstride*3 */
        lsl             mstride, mstride, #2

        subs            stage_count, stage_count, #1
        bgt             .L_ne10_butterfly_length_even_power2_other_stages

.L_ne10_butterfly_length_even_power2_end:
        /*Return From Function*/
        vpop            {q4-q7}
        pop             {r4-r12,pc}

        /* end of ne10_butterfly_length_even_power2_float32_neon */


        /**
         * @details
         * This function implements the 4 butterfly
         *
         * @param[in/out] *Fout        points to input/output pointers
         * @param[in]     *factors     factors pointer:
                                        * 0: stage number
                                        * 1: stride for the first stage
                                        * others: factor out powers of 4, powers of 2
         * @param[in]     *twiddles     twiddles coeffs of FFT
         */

        .align 4
        .global ne10_mixed_radix_butterfly_inverse_length_odd_power2_float32_neon
        .thumb
        .thumb_func

ne10_mixed_radix_butterfly_inverse_length_odd_power2_float32_neon:
        push            {r4-r12,lr}
        vpush           {q4-q7}

        ldr             stage_count, [p_factors]   /* get factors[0]---stage_count */
        ldr             fstride, [p_factors, #4]   /* get factors[1]---fstride */
        add             p_factors, p_factors, stage_count, lsl #3 /* get the address of factors[2*stage_count] */
        ldr             radix, [p_factors]                         /* get factors[2*stage_count]--- the first radix */
        ldr             mstride, [p_factors, #-4]                  /* get factors[2*stage_count-1]--- mstride */

        mov             p_fin, p_fout
        mov             p_fout0, p_fout
        mov             count, fstride

        /* the first stage  */
.L_ne10_butterfly_inverse_length_odd_power2_first_stage:
        vld4.32         {d_in0_r01, d_in0_i01, d_in1_r01, d_in1_i01}, [p_fin]!
        vld4.32         {d_in0_r23, d_in0_i23, d_in1_r23, d_in1_i23}, [p_fin]!
        vsub.f32        q_out1_r0123, q_in0_r0123, q_in1_r0123
        vsub.f32        q_out1_i0123, q_in0_i0123, q_in1_i0123
        vadd.f32        q_out0_r0123, q_in0_r0123, q_in1_r0123
        vadd.f32        q_out0_i0123, q_in0_i0123, q_in1_i0123
        subs            count, count, #4
        vst4.32         {d_out0_r01, d_out0_i01, d_out1_r01, d_out1_i01}, [p_fout0]!
        vst4.32         {d_out0_r23, d_out0_i23, d_out1_r23, d_out1_i23}, [p_fout0]!

        bgt             .L_ne10_butterfly_inverse_length_odd_power2_first_stage

        /* the second stages  */
        subs            stage_count, stage_count, #1
        lsr             fstride, fstride, #2

        /* loop of fstride  */
        mov             count_f, fstride
        mov             p_tw1, p_twiddles
        mov             p_fout0, p_fout
        add             p_fout1, p_fout, mstride, lsl #5
        mov             p_fout2, p_fout
        mov             p_fout3, p_fout1
        mov             tmp0, #96
        vld2.32         {d_tw1_r01, d_tw2_r01, d_tw1_i01, d_tw2_i01}, [p_tw1]!

.L_ne10_butterfly_inverse_length_odd_power2_second_stage:
        RADIX24_BUTTERFLY_INVERSE_P4

        subs            count_f, count_f, #2
        bgt             .L_ne10_butterfly_inverse_length_odd_power2_second_stage

        add             p_twiddles, p_twiddles, mstride, lsl #4
        add             p_twiddles, p_twiddles, mstride, lsl #3 /* get the address of twiddles += mstride*3 */
        lsl             mstride, mstride, #2

        /* other stages  */
        subs            stage_count, stage_count, #1

        /* loop of other stages  */
.L_ne10_butterfly_inverse_length_odd_power2_other_stages:
        lsr             fstride, fstride, #2

        /* loop of fstride  */
        mov             count_f, fstride
.L_ne10_butterfly_inverse_length_odd_power2_other_stages_fstride:
        sub             tmp0, fstride, count_f
        mul             tmp0, tmp0, mstride
        add             p_fout0, p_fout, tmp0, lsl #5
        add             p_fout2, p_fout0, mstride, lsl #4   /* get the address of F[mstride*2] */
        add             p_fout1, p_fout0, mstride, lsl #3   /* get the address of F[mstride] */
        add             p_fout3, p_fout2, mstride, lsl #3   /* get the address of F[mstride*3] */
        mov             p_tw1, p_twiddles
        add             p_tw2, p_tw1, mstride, lsl #3       /* get the address of tw2 */
        add             p_tw3, p_tw1, mstride, lsl #4       /* get the address of tw3 */

        /* loop of mstride  */
        mov             count_m, mstride

.L_ne10_butterfly_inverse_length_odd_power2_other_stages_mstride:
        RADIX4_BUTTERFLY_INVERSE_P4

        subs            count_m, count_m, #4
        bgt             .L_ne10_butterfly_inverse_length_odd_power2_other_stages_mstride
        /* end of mstride loop */

        subs            count_f, count_f, #1
        bgt             .L_ne10_butterfly_inverse_length_odd_power2_other_stages_fstride

        add             p_twiddles, p_twiddles, mstride, lsl #4
        add             p_twiddles, p_twiddles, mstride, lsl #3 /* get the address of twiddles += mstride*3 */
        lsl             mstride, mstride, #2

        subs            stage_count, stage_count, #1
        bgt             .L_ne10_butterfly_inverse_length_odd_power2_other_stages

.L_ne10_butterfly_inverse_length_odd_power2_end:
        /*Return From Function*/
        vpop            {q4-q7}
        pop             {r4-r12,pc}

        /* end of ne10_butterfly_inverse_length_odd_power2_float32_neon */


        /**
         * @details
         * This function implements the 4 butterfly
         *
         * @param[in/out] *Fout        points to input/output pointers
         * @param[in]     *factors     factors pointer:
                                        * 0: stage number
                                        * 1: stride for the first stage
                                        * others: factor out powers of 4, powers of 2
         * @param[in]     *twiddles     twiddles coeffs of FFT
         */

        .align 4
        .global ne10_mixed_radix_butterfly_inverse_length_even_power2_float32_neon
        .thumb
        .thumb_func

ne10_mixed_radix_butterfly_inverse_length_even_power2_float32_neon:
        push            {r4-r12,lr}
        vpush           {q4-q7}

        ldr             stage_count, [p_factors]   /* get factors[0]---stage_count */
        ldr             fstride, [p_factors, #4]   /* get factors[1]---fstride */
        add             p_factors, p_factors, stage_count, lsl #3 /* get the address of factors[2*stage_count] */
        ldr             radix, [p_factors]                         /* get factors[2*stage_count]--- the first radix */
        ldr             mstride, [p_factors, #-4]                  /* get factors[2*stage_count-1]--- mstride */

        mov             p_fin, p_fout
        mov             p_fout0, p_fout
        mov             count, fstride

        /* the first stage  */
.L_ne10_butterfly_inverse_length_even_power2_first_stage:
        vld1.32         {d_in0_0, d_in1_0, d_in2_0, d_in3_0}, [p_fin]!
        vld1.32         {d_in0_1, d_in1_1, d_in2_1, d_in3_1}, [p_fin]!
        vswp            d_in1_0, d_in0_1
        vswp            d_in3_0, d_in2_1
        vsub.f32        q_s2_01, q_in0_01, q_in2_01
        vadd.f32        q_out0_01, q_in0_01, q_in2_01
        vadd.f32        q_s0_01, q_in1_01, q_in3_01
        vsub.f32        q_s1_01, q_in1_01, q_in3_01
        vsub.f32        q_out2_01, q_out0_01, q_s0_01
        vrev64.32       q_s1_01, q_s1_01
        vadd.f32        q_out0_01, q_out0_01, q_s0_01
        vsub.f32        q_out1_01, q_s2_01, q_s1_01
        vadd.f32        q_out3_01, q_s2_01, q_s1_01
        vrev64.32       q_tmp, q_out1_01
        vrev64.32       q_tmp2, q_out3_01
        vtrn.32         q_out3_01, q_tmp
        vtrn.32         q_out1_01, q_tmp2
        vswp            d_out1_0, d_out0_1
        vswp            d_out3_0, d_out2_1
        subs            count, count, #2
        vst1.32         {d_out0_0, d_out1_0, d_out2_0, d_out3_0}, [p_fout0]!
        vst1.32         {d_out0_1, d_out1_1, d_out2_1, d_out3_1}, [p_fout0]!
        bgt             .L_ne10_butterfly_inverse_length_even_power2_first_stage

        /* other stages  */
        subs            stage_count, stage_count, #1

        /* loop of other stages  */
.L_ne10_butterfly_inverse_length_even_power2_other_stages:
        lsr             fstride, fstride, #2

        /* loop of fstride  */
        mov             count_f, fstride
.L_ne10_butterfly_inverse_length_even_power2_other_stages_fstride:
        sub             tmp0, fstride, count_f
        mul             tmp0, tmp0, mstride
        add             p_fout0, p_fout, tmp0, lsl #5
        add             p_fout2, p_fout0, mstride, lsl #4   /* get the address of F[mstride*2] */
        add             p_fout1, p_fout0, mstride, lsl #3   /* get the address of F[mstride] */
        add             p_fout3, p_fout2, mstride, lsl #3   /* get the address of F[mstride*3] */
        mov             p_tw1, p_twiddles
        add             p_tw2, p_tw1, mstride, lsl #3       /* get the address of tw2 */
        add             p_tw3, p_tw1, mstride, lsl #4       /* get the address of tw3 */

        /* loop of mstride  */
        mov             count_m, mstride

.L_ne10_butterfly_inverse_length_even_power2_other_stages_mstride:
        RADIX4_BUTTERFLY_INVERSE_P4

        subs            count_m, count_m, #4
        bgt             .L_ne10_butterfly_inverse_length_even_power2_other_stages_mstride
        /* end of mstride loop */

        subs            count_f, count_f, #1
        bgt             .L_ne10_butterfly_inverse_length_even_power2_other_stages_fstride

        add             p_twiddles, p_twiddles, mstride, lsl #4
        add             p_twiddles, p_twiddles, mstride, lsl #3 /* get the address of twiddles += mstride*3 */
        lsl             mstride, mstride, #2

        subs            stage_count, stage_count, #1
        bgt             .L_ne10_butterfly_inverse_length_even_power2_other_stages

.L_ne10_butterfly_inverse_length_even_power2_end:
        /*Return From Function*/
        vpop            {q4-q7}
        pop             {r4-r12,pc}
        /* end of ne10_butterfly_inverse_length_even_power2_float32_neon */


        /* end of the file */
        .end
