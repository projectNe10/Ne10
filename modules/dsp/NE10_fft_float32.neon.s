/*
 *  Copyright 2013 ARM Limited
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

        /*NEON variale Declaration for butterfly length 8 */
        d_fin0_r        .dn   d0
        d_fin0_i        .dn   d1
        d_fin1_r        .dn   d2
        d_fin1_i        .dn   d3
        d_tw1_r         .dn   d4
        d_tw1_i         .dn   d5
        d_fin2_r        .dn   d6
        d_fin2_i        .dn   d7
        d_tw2_r         .dn   d16
        d_tw2_i         .dn   d17
        d_fin3_r        .dn   d18
        d_fin3_i        .dn   d19
        d_tw3_r         .dn   d20
        d_tw3_i         .dn   d21
        d_s0_r          .dn   d22
        d_s0_i          .dn   d23
        d_s1_r          .dn   d24
        d_s1_i          .dn   d25
        d_s2_r          .dn   d26
        d_s2_i          .dn   d27
        d_s5_r          .dn   d28
        d_s5_i          .dn   d29
        d_s4_r          .dn   d30
        d_s4_i          .dn   d31
        d_s3_r          .dn   d16
        d_s3_i          .dn   d17
        d_fout0_r       .dn   d0
        d_fout0_i       .dn   d1
        d_fout2_r       .dn   d2
        d_fout2_i       .dn   d3
        d_fout1_r       .dn   d4
        d_fout1_i       .dn   d5
        d_fout3_r       .dn   d6
        d_fout3_i       .dn   d7

        /*NEON variale Declaration for second stage */
        d_fin_r01       .dn   d0
        d_fin_r23       .dn   d1
        q_fin0_r0189    .qn   q0
        d_fin_i01       .dn   d2
        d_fin_i23       .dn   d3
        q_fin0_i0189    .qn   q1
        d_fin_r45       .dn   d4
        d_fin_r67       .dn   d5
        q_fin2_r45cd    .qn   q2
        d_fin_i45       .dn   d6
        d_fin_i67       .dn   d7
        q_fin2_i45cd    .qn   q3
        d_fin_r89       .dn   d8
        d_fin_rab       .dn   d9
        q_fin1_r23ab    .qn   q4
        d_fin_i89       .dn   d10
        d_fin_iab       .dn   d11
        q_fin1_i23ab    .qn   q5
        d_fin_rcd       .dn   d12
        d_fin_ref       .dn   d13
        q_fin3_r67ef    .qn   q6
        d_fin_icd       .dn   d14
        d_fin_ief       .dn   d15
        q_fin3_i67ef    .qn   q7
        d_tw1_r01       .dn   d16
        d_tw1_rd        .dn   d17
        q_tw1_r01       .qn   q8
        d_tw1_i01       .dn   d18
        d_tw1_id        .dn   d19
        q_tw1_i01       .qn   q9
        d_tw2_r01       .dn   d20
        d_tw2_rd        .dn   d21
        q_tw2_r01       .qn   q10
        d_tw2_i01       .dn   d22
        d_tw2_id        .dn   d23
        q_tw2_i01       .qn   q11
        d_tw3_r01       .dn   d24
        d_tw3_rd        .dn   d25
        q_tw3_r01       .qn   q12
        d_tw3_i01       .dn   d26
        d_tw3_id        .dn   d27
        q_tw3_i01       .qn   q13
        q_s0_r01        .qn   q14
        q_s0_i01        .qn   q15
        q_s1_r01        .qn   q4
        q_s1_i01        .qn   q5
        q_s2_r01        .qn   q8
        q_s2_i01        .qn   q9
        q_s3_r01        .qn   q2
        q_s3_i01        .qn   q3
        q_s4_r01        .qn   q6
        q_s4_i01        .qn   q7
        q_s5_r01        .qn   q10
        q_s5_i01        .qn   q11
        d_fout_r01      .dn   d0
        d_fout_r23      .dn   d1
        q_fout_r0189    .qn   q0
        d_fout_i01      .dn   d2
        d_fout_i23      .dn   d3
        q_fout_i0189    .qn   q1
        d_fout_r45      .dn   d8
        d_fout_r67      .dn   d9
        q_fout_r45cd    .qn   q4
        d_fout_i45      .dn   d10
        d_fout_i67      .dn   d11
        q_fout_i45cd    .qn   q5
        d_fout_r89      .dn   d16
        d_fout_rab      .dn   d17
        q_fout_r23ab    .qn   q8
        d_fout_i89      .dn   d18
        d_fout_iab      .dn   d19
        q_fout_i23ab    .qn   q9
        d_fout_rcd      .dn   d24
        d_fout_ref      .dn   d25
        q_fout_r67ef    .qn   q12
        d_fout_icd      .dn   d26
        d_fout_ief      .dn   d27
        q_fout_i67ef    .qn   q13

        d_tmp0          .dn   d30
        d_tmp1          .dn   d31
        q_tmp           .qn   q15
        d_tmp2_0        .dn   d28
        d_tmp2_1        .dn   d29
        q_tmp2          .qn   q14

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
        .global ne10_butterfly_length16_float32_neon
        .thumb
        .thumb_func
ne10_butterfly_length16_float32_neon:
        push            {r4-r7,r10,r11,lr}

        mov             p_fin, p_fout
        mov             p_fout0, p_fout
        mov             count, #4

        /* the first stage  */
.L_ne10_butterfly_length16_first_stage:
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
        bgt             .L_ne10_butterfly_length16_first_stage

        /* the second stage  */
        mov             mstride, #4
        mov             p_fout0, p_fout
        add             p_fout2, p_fout0, mstride, lsl #4   /* get the address of F[mstride*2] */
        add             p_fout1, p_fout0, mstride, lsl #3   /* get the address of F[mstride] */
        add             p_fout3, p_fout2, mstride, lsl #3   /* get the address of F[mstride*3] */
        mov             p_tw1, p_twiddles
        add             p_tw2, p_tw1, mstride, lsl #3       /* get the address of tw2 */
        add             p_tw3, p_tw1, mstride, lsl #4       /* get the address of tw3 */

        vld2.32         {q_fin1_r, q_fin1_i}, [p_fout1]
        vld2.32         {q_tw1_r, q_tw1_i}, [p_tw1]
        vld2.32         {q_fin2_r, q_fin2_i}, [p_fout2]
        vld2.32         {q_tw2_r, q_tw2_i}, [p_tw2]
        vmul.f32        q_s0_r, q_fin1_r, q_tw1_r
        vmul.f32        q_s0_i, q_fin1_i, q_tw1_r
        vmls.f32        q_s0_r, q_fin1_i, q_tw1_i
        vmla.f32        q_s0_i, q_fin1_r, q_tw1_i

        vld2.32         {q_fin3_r, q_fin3_i}, [p_fout3]
        vld2.32         {q_tw3_r, q_tw3_i}, [p_tw3]
        vmul.f32        q_s1_r, q_fin2_r, q_tw2_r
        vmul.f32        q_s1_i, q_fin2_i, q_tw2_r
        vmls.f32        q_s1_r, q_fin2_i, q_tw2_i
        vmla.f32        q_s1_i, q_fin2_r, q_tw2_i

        vld2.32         {q_fin0_r, q_fin0_i}, [p_fout0]
        vmul.f32        q_s2_r, q_fin3_r, q_tw3_r
        vmul.f32        q_s2_i, q_fin3_i, q_tw3_r
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
        vst2.32         {q_fout2_r, q_fout2_i}, [p_fout2]!

        vadd.f32        q_fout1_r, q_s5_r, q_s4_i
        vsub.f32        q_fout1_i, q_s5_i, q_s4_r
        vsub.f32        q_fout3_r, q_s5_r, q_s4_i
        vadd.f32        q_fout3_i, q_s5_i, q_s4_r
        vst2.32         {q_fout0_r, q_fout0_i}, [p_fout0]
        vst2.32         {q_fout1_r, q_fout1_i}, [p_fout1]
        vst2.32         {q_fout3_r, q_fout3_i}, [p_fout3]

        pop             {r4-r7,r10,r11,pc}
        /* end of ne10_butterfly_length16_float32_neon */


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
        .global ne10_butterfly_length8_float32_neon
        .thumb
        .thumb_func
ne10_butterfly_length8_float32_neon:
        push            {r4-r7,r10,r11,lr}

        /* the first stage  */
        mov             p_fin, p_fout
        mov             p_fout0, p_fout

        vld4.32         {d_in0_r01, d_in0_i01, d_in1_r01, d_in1_i01}, [p_fin]!
        vld4.32         {d_in0_r23, d_in0_i23, d_in1_r23, d_in1_i23}, [p_fin]!
        vsub.f32        q_out1_r0123, q_in0_r0123, q_in1_r0123
        vsub.f32        q_out1_i0123, q_in0_i0123, q_in1_i0123
        vadd.f32        q_out0_r0123, q_in0_r0123, q_in1_r0123
        vadd.f32        q_out0_i0123, q_in0_i0123, q_in1_i0123
        vst4.32         {d_out0_r01, d_out0_i01, d_out1_r01, d_out1_i01}, [p_fout0]!
        vst4.32         {d_out0_r23, d_out0_i23, d_out1_r23, d_out1_i23}, [p_fout0]!

        /* the second stage  */
        mov             mstride, #2
        mov             p_fout0, p_fout
        add             p_fout2, p_fout0, mstride, lsl #4   /* get the address of F[mstride*2] */
        add             p_fout1, p_fout0, mstride, lsl #3   /* get the address of F[mstride] */
        add             p_fout3, p_fout2, mstride, lsl #3   /* get the address of F[mstride*3] */
        mov             p_tw1, p_twiddles
        add             p_tw2, p_tw1, mstride, lsl #3       /* get the address of tw2 */
        add             p_tw3, p_tw1, mstride, lsl #4       /* get the address of tw3 */

        vld2.32         {d_fin1_r, d_fin1_i}, [p_fout1]
        vld2.32         {d_tw1_r, d_tw1_i}, [p_tw1]!
        vld2.32         {d_fin2_r, d_fin2_i}, [p_fout2]
        vld2.32         {d_tw2_r, d_tw2_i}, [p_tw2]!
        vld2.32         {d_fin3_r, d_fin3_i}, [p_fout3]
        vld2.32         {d_tw3_r, d_tw3_i}, [p_tw3]!
        vld2.32         {d_fin0_r, d_fin0_i}, [p_fout0]
        vmul.f32        d_s0_r, d_fin1_r, d_tw1_r
        vmul.f32        d_s0_i, d_fin1_i, d_tw1_r
        vmul.f32        d_s1_r, d_fin2_r, d_tw2_r
        vmul.f32        d_s1_i, d_fin2_i, d_tw2_r
        vmul.f32        d_s2_r, d_fin3_r, d_tw3_r
        vmul.f32        d_s2_i, d_fin3_i, d_tw3_r
        vmls.f32        d_s0_r, d_fin1_i, d_tw1_i
        vmla.f32        d_s0_i, d_fin1_r, d_tw1_i
        vmls.f32        d_s1_r, d_fin2_i, d_tw2_i
        vmla.f32        d_s1_i, d_fin2_r, d_tw2_i
        vmls.f32        d_s2_r, d_fin3_i, d_tw3_i
        vmla.f32        d_s2_i, d_fin3_r, d_tw3_i

        vsub.f32        d_s5_r, d_fin0_r, d_s1_r
        vsub.f32        d_s5_i, d_fin0_i, d_s1_i
        vadd.f32        d_fout0_r, d_fin0_r, d_s1_r
        vadd.f32        d_fout0_i, d_fin0_i, d_s1_i

        vadd.f32        d_s3_r, d_s0_r, d_s2_r
        vadd.f32        d_s3_i, d_s0_i, d_s2_i
        vsub.f32        d_s4_r, d_s0_r, d_s2_r
        vsub.f32        d_s4_i, d_s0_i, d_s2_i

        vsub.f32        d_fout2_r, d_fout0_r, d_s3_r
        vsub.f32        d_fout2_i, d_fout0_i, d_s3_i
        vadd.f32        d_fout0_r, d_fout0_r, d_s3_r
        vadd.f32        d_fout0_i, d_fout0_i, d_s3_i
        vst2.32         {d_fout2_r, d_fout2_i}, [p_fout2]!

        vadd.f32        d_fout1_r, d_s5_r, d_s4_i
        vsub.f32        d_fout1_i, d_s5_i, d_s4_r
        vsub.f32        d_fout3_r, d_s5_r, d_s4_i
        vadd.f32        d_fout3_i, d_s5_i, d_s4_r
        vst2.32         {d_fout0_r, d_fout0_i}, [p_fout0]!
        vst2.32         {d_fout1_r, d_fout1_i}, [p_fout1]!
        vst2.32         {d_fout3_r, d_fout3_i}, [p_fout3]!


        pop             {r4-r7,r10,r11,pc}

        /* end of ne10_butterfly_length8_float32_neon */

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
        .global ne10_butterfly_length_odd_power2_float32_neon
        .thumb
        .thumb_func

ne10_butterfly_length_odd_power2_float32_neon:
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
.L_ne10_butterfly_length_odd_power2_second_stage:
        sub             tmp0, fstride, count_f
        mul             tmp0, tmp0, mstride
        add             p_fout0, p_fout, tmp0, lsl #5       /* get the address of F = &Fout[i * mstride * 4] */
        add             p_fout1, p_fout0, mstride, lsl #4   /* get the address of F[mstride*2] */
        add             p_fout2, p_fout0, mstride, lsl #5   /* get the address of F = &Fout[(i + 1) * mstride * 4] */
        add             p_fout3, p_fout2, mstride, lsl #4   /* get the address of F[mstride*2] */
        mov             p_tw1, p_twiddles
        add             p_tw2, p_tw1, mstride, lsl #3       /* get the address of tw2 */
        add             p_tw3, p_tw1, mstride, lsl #4       /* get the address of tw3 */

        vld2.32         {d_fin_r01, d_fin_r23, d_fin_i01, d_fin_i23}, [p_fout0]
        vld2.32         {d_fin_r45, d_fin_r67, d_fin_i45, d_fin_i67}, [p_fout1]
        vld2.32         {d_fin_r89, d_fin_rab, d_fin_i89, d_fin_iab}, [p_fout2]
        vld2.32         {d_fin_rcd, d_fin_ref, d_fin_icd, d_fin_ief}, [p_fout3]
        vld2.32         {d_tw1_r01, d_tw1_i01}, [p_tw1]!
        vld2.32         {d_tw2_r01, d_tw2_i01}, [p_tw2]!
        vld2.32         {d_tw3_r01, d_tw3_i01}, [p_tw2]!
        vswp            d_fin_r23, d_fin_r89
        vswp            d_fin_i23, d_fin_i89
        vswp            d_fin_r67, d_fin_rcd
        vswp            d_fin_i67, d_fin_icd
        vmov            d_tw1_rd, d_tw1_r01
        vmov            d_tw1_id, d_tw1_i01
        vmov            d_tw2_rd, d_tw2_r01
        vmov            d_tw2_id, d_tw2_i01
        vmov            d_tw3_rd, d_tw3_r01
        vmov            d_tw3_id, d_tw3_i01

        vmul.f32        q_s0_r01, q_fin1_r23ab, q_tw1_r01
        vmul.f32        q_s0_i01, q_fin1_r23ab, q_tw1_i01
        vmls.f32        q_s0_r01, q_fin1_i23ab, q_tw1_i01
        vmla.f32        q_s0_i01, q_fin1_i23ab, q_tw1_r01
        vmul.f32        q_s1_r01, q_fin2_r45cd, q_tw2_r01
        vmul.f32        q_s1_i01, q_fin2_r45cd, q_tw2_i01
        vmul.f32        q_s2_r01, q_fin3_r67ef, q_tw3_r01
        vmul.f32        q_s2_i01, q_fin3_r67ef, q_tw3_i01
        vmls.f32        q_s1_r01, q_fin2_i45cd, q_tw2_i01
        vmla.f32        q_s1_i01, q_fin2_i45cd, q_tw2_r01
        vmls.f32        q_s2_r01, q_fin3_i67ef, q_tw3_i01
        vmla.f32        q_s2_i01, q_fin3_i67ef, q_tw3_r01

        vsub.f32        q_s5_r01, q_fin0_r0189, q_s1_r01
        vsub.f32        q_s5_i01, q_fin0_i0189, q_s1_i01
        vadd.f32        q_fout_r0189, q_fin0_r0189, q_s1_r01
        vadd.f32        q_fout_i0189, q_fin0_i0189, q_s1_i01

        vadd.f32        q_s3_r01, q_s0_r01, q_s2_r01
        vadd.f32        q_s3_i01, q_s0_i01, q_s2_i01
        vsub.f32        q_s4_r01, q_s0_r01, q_s2_r01
        vsub.f32        q_s4_i01, q_s0_i01, q_s2_i01

        vsub.f32        q_fout_r45cd, q_fout_r0189, q_s3_r01
        vsub.f32        q_fout_i45cd, q_fout_i0189, q_s3_i01
        vadd.f32        q_fout_r0189, q_fout_r0189, q_s3_r01
        vadd.f32        q_fout_i0189, q_fout_i0189, q_s3_i01

        vadd.f32        q_fout_r23ab, q_s5_r01, q_s4_i01
        vsub.f32        q_fout_i23ab, q_s5_i01, q_s4_r01
        vsub.f32        q_fout_r67ef, q_s5_r01, q_s4_i01
        vadd.f32        q_fout_i67ef, q_s5_i01, q_s4_r01

        vswp            d_fout_r23, d_fout_r89
        vswp            d_fout_i23, d_fout_i89
        vswp            d_fout_r67, d_fout_rcd
        vswp            d_fout_i67, d_fout_icd
        vst2.32         {d_fout_r01, d_fout_r23, d_fout_i01, d_fout_i23}, [p_fout0]
        vst2.32         {d_fout_r45, d_fout_r67, d_fout_i45, d_fout_i67}, [p_fout1]
        vst2.32         {d_fout_r89, d_fout_rab, d_fout_i89, d_fout_iab}, [p_fout2]
        vst2.32         {d_fout_rcd, d_fout_ref, d_fout_icd, d_fout_ief}, [p_fout3]

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
        .global ne10_butterfly_length_even_power2_float32_neon
        .thumb
        .thumb_func

ne10_butterfly_length_even_power2_float32_neon:
        push            {r4-r12,lr}

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
        vld2.32         {q_fin1_r, q_fin1_i}, [p_fout1]
        vld2.32         {q_tw1_r, q_tw1_i}, [p_tw1]!
        vld2.32         {q_fin2_r, q_fin2_i}, [p_fout2]
        vld2.32         {q_tw2_r, q_tw2_i}, [p_tw2]!
        vmul.f32        q_s0_r, q_fin1_r, q_tw1_r
        vmul.f32        q_s0_i, q_fin1_i, q_tw1_r
        vmls.f32        q_s0_r, q_fin1_i, q_tw1_i
        vmla.f32        q_s0_i, q_fin1_r, q_tw1_i

        vld2.32         {q_fin3_r, q_fin3_i}, [p_fout3]
        vld2.32         {q_tw3_r, q_tw3_i}, [p_tw3]!
        vmul.f32        q_s1_r, q_fin2_r, q_tw2_r
        vmul.f32        q_s1_i, q_fin2_i, q_tw2_r
        vmls.f32        q_s1_r, q_fin2_i, q_tw2_i
        vmla.f32        q_s1_i, q_fin2_r, q_tw2_i

        vld2.32         {q_fin0_r, q_fin0_i}, [p_fout0]
        vmul.f32        q_s2_r, q_fin3_r, q_tw3_r
        vmul.f32        q_s2_i, q_fin3_i, q_tw3_r
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
        vst2.32         {q_fout2_r, q_fout2_i}, [p_fout2]!

        vadd.f32        q_fout1_r, q_s5_r, q_s4_i
        vsub.f32        q_fout1_i, q_s5_i, q_s4_r
        vsub.f32        q_fout3_r, q_s5_r, q_s4_i
        vadd.f32        q_fout3_i, q_s5_i, q_s4_r
        vst2.32         {q_fout0_r, q_fout0_i}, [p_fout0]!
        vst2.32         {q_fout1_r, q_fout1_i}, [p_fout1]!
        vst2.32         {q_fout3_r, q_fout3_i}, [p_fout3]!

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
        .global ne10_butterfly_inverse_length16_float32_neon
        .thumb
        .thumb_func
ne10_butterfly_inverse_length16_float32_neon:
        push            {r4-r7,r10,r11,lr}

        mov             p_fin, p_fout
        mov             p_fout0, p_fout
        mov             count, #4

        /* the first stage  */
.L_ne10_butterfly_inverse_length16_first_stage:
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
        bgt             .L_ne10_butterfly_inverse_length16_first_stage

        /* the second stage  */
        mov             mstride, 4
        mov             p_fout0, p_fout
        add             p_fout2, p_fout0, mstride, lsl #4   /* get the address of F[mstride*2] */
        add             p_fout1, p_fout0, mstride, lsl #3   /* get the address of F[mstride] */
        add             p_fout3, p_fout2, mstride, lsl #3   /* get the address of F[mstride*3] */
        mov             p_tw1, p_twiddles
        add             p_tw2, p_tw1, mstride, lsl #3       /* get the address of tw2 */
        add             p_tw3, p_tw1, mstride, lsl #4       /* get the address of tw3 */

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

        pop             {r4-r7,r10,r11,pc}
        /* end of ne10_butterfly_inverse_length16_float32_neon */


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
        .global ne10_butterfly_inverse_length8_float32_neon
        .thumb
        .thumb_func
ne10_butterfly_inverse_length8_float32_neon:
        push            {r4-r7,r10,r11,lr}

        /* the first stage  */
        mov             p_fin, p_fout
        mov             p_fout0, p_fout

        vld4.32         {d_in0_r01, d_in0_i01, d_in1_r01, d_in1_i01}, [p_fin]!
        vld4.32         {d_in0_r23, d_in0_i23, d_in1_r23, d_in1_i23}, [p_fin]!
        vsub.f32        q_out1_r0123, q_in0_r0123, q_in1_r0123
        vsub.f32        q_out1_i0123, q_in0_i0123, q_in1_i0123
        vadd.f32        q_out0_r0123, q_in0_r0123, q_in1_r0123
        vadd.f32        q_out0_i0123, q_in0_i0123, q_in1_i0123
        vst4.32         {d_out0_r01, d_out0_i01, d_out1_r01, d_out1_i01}, [p_fout0]!
        vst4.32         {d_out0_r23, d_out0_i23, d_out1_r23, d_out1_i23}, [p_fout0]!

        /* the second stage  */
        mov             mstride, #2
        mov             p_fout0, p_fout
        add             p_fout2, p_fout0, mstride, lsl #4   /* get the address of F[mstride*2] */
        add             p_fout1, p_fout0, mstride, lsl #3   /* get the address of F[mstride] */
        add             p_fout3, p_fout2, mstride, lsl #3   /* get the address of F[mstride*3] */
        mov             p_tw1, p_twiddles
        add             p_tw2, p_tw1, mstride, lsl #3       /* get the address of tw2 */
        add             p_tw3, p_tw1, mstride, lsl #4       /* get the address of tw3 */

        vld2.32         {d_fin1_r, d_fin1_i}, [p_fout1]
        vld2.32         {d_tw1_r, d_tw1_i}, [p_tw1]!
        vld2.32         {d_fin2_r, d_fin2_i}, [p_fout2]
        vld2.32         {d_tw2_r, d_tw2_i}, [p_tw2]!
        vld2.32         {d_fin3_r, d_fin3_i}, [p_fout3]
        vld2.32         {d_tw3_r, d_tw3_i}, [p_tw3]!
        vld2.32         {d_fin0_r, d_fin0_i}, [p_fout0]
        vmul.f32        d_s0_r, d_fin1_r, d_tw1_r
        vmul.f32        d_s0_i, d_fin1_i, d_tw1_r
        vmul.f32        d_s1_r, d_fin2_r, d_tw2_r
        vmul.f32        d_s1_i, d_fin2_i, d_tw2_r
        vmul.f32        d_s2_r, d_fin3_r, d_tw3_r
        vmul.f32        d_s2_i, d_fin3_i, d_tw3_r
        vmla.f32        d_s0_r, d_fin1_i, d_tw1_i
        vmls.f32        d_s0_i, d_fin1_r, d_tw1_i
        vmla.f32        d_s1_r, d_fin2_i, d_tw2_i
        vmls.f32        d_s1_i, d_fin2_r, d_tw2_i
        vmla.f32        d_s2_r, d_fin3_i, d_tw3_i
        vmls.f32        d_s2_i, d_fin3_r, d_tw3_i

        vsub.f32        d_s5_r, d_fin0_r, d_s1_r
        vsub.f32        d_s5_i, d_fin0_i, d_s1_i
        vadd.f32        d_fout0_r, d_fin0_r, d_s1_r
        vadd.f32        d_fout0_i, d_fin0_i, d_s1_i

        vadd.f32        d_s3_r, d_s0_r, d_s2_r
        vadd.f32        d_s3_i, d_s0_i, d_s2_i
        vsub.f32        d_s4_r, d_s0_r, d_s2_r
        vsub.f32        d_s4_i, d_s0_i, d_s2_i

        vsub.f32        d_fout2_r, d_fout0_r, d_s3_r
        vsub.f32        d_fout2_i, d_fout0_i, d_s3_i
        vadd.f32        d_fout0_r, d_fout0_r, d_s3_r
        vadd.f32        d_fout0_i, d_fout0_i, d_s3_i
        vst2.32         {d_fout2_r, d_fout2_i}, [p_fout2]!

        vsub.f32        d_fout1_r, d_s5_r, d_s4_i
        vadd.f32        d_fout1_i, d_s5_i, d_s4_r
        vadd.f32        d_fout3_r, d_s5_r, d_s4_i
        vsub.f32        d_fout3_i, d_s5_i, d_s4_r
        vst2.32         {d_fout0_r, d_fout0_i}, [p_fout0]!
        vst2.32         {d_fout1_r, d_fout1_i}, [p_fout1]!
        vst2.32         {d_fout3_r, d_fout3_i}, [p_fout3]!

        pop             {r4-r7,r10,r11,pc}
        /* end of ne10_butterfly_inverse_length8_float32_neon */


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
        .global ne10_butterfly_inverse_length_odd_power2_float32_neon
        .thumb
        .thumb_func

ne10_butterfly_inverse_length_odd_power2_float32_neon:
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
.L_ne10_butterfly_inverse_length_odd_power2_second_stage:
        sub             tmp0, fstride, count_f
        mul             tmp0, tmp0, mstride
        add             p_fout0, p_fout, tmp0, lsl #5       /* get the address of F = &Fout[i * mstride * 4] */
        add             p_fout1, p_fout0, mstride, lsl #4   /* get the address of F[mstride*2] */
        add             p_fout2, p_fout0, mstride, lsl #5   /* get the address of F = &Fout[(i + 1) * mstride * 4] */
        add             p_fout3, p_fout2, mstride, lsl #4   /* get the address of F[mstride*2] */
        mov             p_tw1, p_twiddles
        add             p_tw2, p_tw1, mstride, lsl #3       /* get the address of tw2 */
        add             p_tw3, p_tw1, mstride, lsl #4       /* get the address of tw3 */

        vld2.32         {d_fin_r01, d_fin_r23, d_fin_i01, d_fin_i23}, [p_fout0]
        vld2.32         {d_fin_r45, d_fin_r67, d_fin_i45, d_fin_i67}, [p_fout1]
        vld2.32         {d_fin_r89, d_fin_rab, d_fin_i89, d_fin_iab}, [p_fout2]
        vld2.32         {d_fin_rcd, d_fin_ref, d_fin_icd, d_fin_ief}, [p_fout3]
        vld2.32         {d_tw1_r01, d_tw1_i01}, [p_tw1]!
        vld2.32         {d_tw2_r01, d_tw2_i01}, [p_tw2]!
        vld2.32         {d_tw3_r01, d_tw3_i01}, [p_tw2]!
        vswp            d_fin_r23, d_fin_r89
        vswp            d_fin_i23, d_fin_i89
        vswp            d_fin_r67, d_fin_rcd
        vswp            d_fin_i67, d_fin_icd
        vmov            d_tw1_rd, d_tw1_r01
        vmov            d_tw1_id, d_tw1_i01
        vmov            d_tw2_rd, d_tw2_r01
        vmov            d_tw2_id, d_tw2_i01
        vmov            d_tw3_rd, d_tw3_r01
        vmov            d_tw3_id, d_tw3_i01

        vmul.f32        q_s0_r01, q_fin1_r23ab, q_tw1_r01
        vmul.f32        q_s0_i01, q_fin1_i23ab, q_tw1_r01
        vmla.f32        q_s0_r01, q_fin1_i23ab, q_tw1_i01
        vmls.f32        q_s0_i01, q_fin1_r23ab, q_tw1_i01
        vmul.f32        q_s1_r01, q_fin2_r45cd, q_tw2_r01
        vmul.f32        q_s1_i01, q_fin2_i45cd, q_tw2_r01
        vmul.f32        q_s2_r01, q_fin3_r67ef, q_tw3_r01
        vmul.f32        q_s2_i01, q_fin3_i67ef, q_tw3_r01
        vmla.f32        q_s1_r01, q_fin2_i45cd, q_tw2_i01
        vmls.f32        q_s1_i01, q_fin2_r45cd, q_tw2_i01
        vmla.f32        q_s2_r01, q_fin3_i67ef, q_tw3_i01
        vmls.f32        q_s2_i01, q_fin3_r67ef, q_tw3_i01

        vsub.f32        q_s5_r01, q_fin0_r0189, q_s1_r01
        vsub.f32        q_s5_i01, q_fin0_i0189, q_s1_i01
        vadd.f32        q_fout_r0189, q_fin0_r0189, q_s1_r01
        vadd.f32        q_fout_i0189, q_fin0_i0189, q_s1_i01

        vadd.f32        q_s3_r01, q_s0_r01, q_s2_r01
        vadd.f32        q_s3_i01, q_s0_i01, q_s2_i01
        vsub.f32        q_s4_r01, q_s0_r01, q_s2_r01
        vsub.f32        q_s4_i01, q_s0_i01, q_s2_i01

        vsub.f32        q_fout_r45cd, q_fout_r0189, q_s3_r01
        vsub.f32        q_fout_i45cd, q_fout_i0189, q_s3_i01
        vadd.f32        q_fout_r0189, q_fout_r0189, q_s3_r01
        vadd.f32        q_fout_i0189, q_fout_i0189, q_s3_i01

        vsub.f32        q_fout_r23ab, q_s5_r01, q_s4_i01
        vadd.f32        q_fout_i23ab, q_s5_i01, q_s4_r01
        vadd.f32        q_fout_r67ef, q_s5_r01, q_s4_i01
        vsub.f32        q_fout_i67ef, q_s5_i01, q_s4_r01

        vswp            d_fout_r23, d_fout_r89
        vswp            d_fout_i23, d_fout_i89
        vswp            d_fout_r67, d_fout_rcd
        vswp            d_fout_i67, d_fout_icd
        vst2.32         {d_fout_r01, d_fout_r23, d_fout_i01, d_fout_i23}, [p_fout0]
        vst2.32         {d_fout_r45, d_fout_r67, d_fout_i45, d_fout_i67}, [p_fout1]
        vst2.32         {d_fout_r89, d_fout_rab, d_fout_i89, d_fout_iab}, [p_fout2]
        vst2.32         {d_fout_rcd, d_fout_ref, d_fout_icd, d_fout_ief}, [p_fout3]

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
        .global ne10_butterfly_inverse_length_even_power2_float32_neon
        .thumb
        .thumb_func

ne10_butterfly_inverse_length_even_power2_float32_neon:
        push            {r4-r12,lr}

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
        pop             {r4-r12,pc}

        /* end of ne10_butterfly_inverse_length_even_power2_float32_neon */


        /* end of the file */
        .end
