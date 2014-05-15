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
 * NE10 Library : dsp/NE10_fft_int16.neon.s
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
        d_in01          .dn   d0
        d_in23          .dn   d1
        d_in0           .dn   d0
        d_in1           .dn   d1
        q_in01          .qn   q0
        d_in2           .dn   d2
        d_in3           .dn   d3
        q_in23          .qn   q1
        d_out0          .dn   d4
        d_out1          .dn   d5
        q_out01         .qn   q2
        d_out2          .dn   d6
        d_out3          .dn   d7
        q_out23         .qn   q3
        d_out01         .dn   d0
        d_out23         .dn   d1
        d_s0            .dn   d16
        d_s1            .dn   d17
        d_s2            .dn   d18

        d_in_r0246      .dn   d0
        d_in_i0246      .dn   d1
        q_in_0246       .qn   q0
        d_in_r1357      .dn   d2
        d_in_i1357      .dn   d3
        q_in_1357       .qn   q1
        q_in_r0246      .qn   q8
        q_in_i0246      .qn   q9
        q_in_r1357      .qn   q10
        q_in_i1357      .qn   q11
        q_out_0246      .qn   q8
        q_out_1357      .qn   q9

        d_in_04         .dn   d0
        d_in_8c         .dn   d1
        q_in_048c       .qn   q0
        d_in_15         .dn   d2
        d_in_9d         .dn   d3
        q_in_159d       .qn   q1
        d_in_26         .dn   d4
        d_in_ae         .dn   d5
        q_in_26ae       .qn   q2
        d_in_37         .dn   d6
        d_in_bf         .dn   d7
        q_in_37bf       .qn   q3
        q_in_04         .qn   q8
        q_in_15         .qn   q9
        q_in_26         .qn   q10
        q_in_37         .qn   q11
        q_in_8c         .qn   q12
        q_in_9d         .qn   q13
        q_in_ae         .qn   q14
        q_in_bf         .qn   q15
        d_out_04        .dn   d0
        d_out_8c        .dn   d1
        q_out_048c      .qn   q0
        d_out_15        .dn   d2
        d_out_9d        .dn   d3
        q_out_159d      .qn   q1
        d_out_26        .dn   d4
        d_out_ae        .dn   d5
        q_out_26ae      .qn   q2
        d_out_37        .dn   d6
        d_out_bf        .dn   d7
        q_out_37bf      .qn   q3
        q_s0_0123       .qn   q8
        q_s1_0123       .qn   q9
        q_s2_0123       .qn   q10

        /*NEON variale Declaration for mstride ge2 loop */
        d_fin0_r        .dn   d0
        d_fin0_i        .dn   d1
        q_fin0          .qn   q0
        d_fin1_r        .dn   d2
        d_fin1_i        .dn   d3
        d_fin2_r        .dn   d4
        d_fin2_i        .dn   d5
        d_fin3_r        .dn   d6
        d_fin3_i        .dn   d7
        q_fin1_r        .qn   q12
        q_fin1_i        .qn   q13
        q_fin2_r        .qn   q14
        q_fin2_i        .qn   q15
        q_fin0_r        .qn   q12
        q_fin0_i        .qn   q13
        q_fin3_r        .qn   q14
        q_fin3_i        .qn   q15
        d_tw1_r         .dn   d16
        d_tw1_i         .dn   d17
        d_tw2_r         .dn   d18
        d_tw2_i         .dn   d19
        d_tw3_r         .dn   d20
        d_tw3_i         .dn   d21
        d_s0_r          .dn   d24
        d_s0_i          .dn   d25
        q_s0            .qn   q12
        d_s1_r          .dn   d26
        d_s1_i          .dn   d27
        q_s1            .qn   q13
        d_s2_r          .dn   d28
        d_s2_i          .dn   d29
        q_s2            .qn   q14
        d_s5_r          .dn   d16
        d_s5_i          .dn   d17
        q_s5            .qn   q8
        d_s4_r          .dn   d18
        d_s4_i          .dn   d19
        q_s4            .qn   q9
        d_s3_r          .dn   d20
        d_s3_i          .dn   d21
        q_s3            .qn   q10
        q_s0_r          .qn   q12
        q_s0_i          .qn   q13
        q_s1_r          .qn   q14
        q_s1_i          .qn   q15
        q_s2_r          .qn   q8
        q_s2_i          .qn   q9
        d_fout0_r       .dn   d0
        d_fout0_i       .dn   d1
        q_fout0         .qn   q0
        d_fout2_r       .dn   d2
        d_fout2_i       .dn   d3
        q_fout2         .qn   q1
        d_fout1_r       .dn   d4
        d_fout1_i       .dn   d5
        d_fout3_r       .dn   d6
        d_fout3_i       .dn   d7

        /*NEON variale Declaration for mstride 2 loop */
        d_fin_r0123     .dn   d0
        d_fin_r4567     .dn   d1
        d_fin_i0123     .dn   d2
        d_fin_i4567     .dn   d3
        d_tw_r2345      .dn   d4
        d_tw_r67xx      .dn   d5
        d_tw_i2345      .dn   d6
        d_tw_i67xx      .dn   d7
        d_fin_r01       .dn   d16
        d_fin_r23       .dn   d17
        q_fin_r0123     .qn   q8
        d_fin_i01       .dn   d18
        d_fin_i23       .dn   d19
        q_fin_i0123     .qn   q9
        d_fin_r45       .dn   d20
        d_fin_r67       .dn   d21
        q_fin_r4567     .qn   q10
        d_fin_i45       .dn   d22
        d_fin_i67       .dn   d23
        q_fin_i4567     .qn   q11
        d_tw_r23        .dn   d24
        d_tw_r45        .dn   d25
        q_tw_r2345      .qn   q12
        d_tw_i23        .dn   d26
        d_tw_i45        .dn   d27
        q_tw_i2345      .qn   q13
        d_tw_r67        .dn   d28
        q_tw_r67xx      .qn   q14
        d_tw_i67        .dn   d30
        q_tw_i67xx      .qn   q15
        d_s0_r01        .dn   d0
        d_s0_i01        .dn   d1
        q_s0_01         .qn   q0
        d_s1_r01        .dn   d2
        d_s1_i01        .dn   d3
        q_s1_01         .qn   q1
        d_s2_r01        .dn   d4
        d_s2_i01        .dn   d5
        q_s2_01         .qn   q2
        d_s5_r01        .dn   d24
        d_s5_i01        .dn   d25
        d_s4_r01        .dn   d26
        d_s4_i01        .dn   d27
        d_s3_r01        .dn   d28
        d_s3_i01        .dn   d29

        d_fout_r01      .dn   d16
        d_fout_r23      .dn   d17
        q_fout_r0123    .qn   q8
        d_fout_i01      .dn   d18
        d_fout_i23      .dn   d19
        q_fout_i0123    .qn   q9
        d_fout_r45      .dn   d20
        d_fout_r67      .dn   d21
        q_fout_r4567    .qn   q10
        d_fout_i45      .dn   d22
        d_fout_i67      .dn   d23
        q_fout_i4567    .qn   q11
        d_fout_r0123    .dn   d0
        d_fout_r4567    .dn   d1
        d_fout_i0123    .dn   d2
        d_fout_i4567    .dn   d3

        d_tmp0          .dn   d30
        d_tmp1          .dn   d31
        q_tmp           .qn   q15
        d_tmp1_0        .dn   d28
        d_tmp1_1        .dn   d29
        q_tmp1          .qn   q14
        d_tmp2_0        .dn   d20
        d_tmp2_1        .dn   d21
        q_tmp2          .qn   q13
        q_tmp3          .qn   q12
        q_shift15       .qn   q5
        d_max_div2      .dn   d8
        d_max_div4      .dn   d9

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
        .global ne10_mixed_radix_butterfly_int16_neon
        .thumb
        .thumb_func

ne10_mixed_radix_butterfly_int16_neon:

        push            {r4-r12,lr}
        vpush           {d8-d11}

        ldr             stage_count, [p_factors]   /* get factors[0]---stage_count */
        ldr             fstride, [p_factors, #4]   /* get factors[1]---fstride */
        add             p_factors, p_factors, stage_count, lsl #3 /* get the address of factors[2*stage_count] */
        ldr             radix, [p_factors]                         /* get factors[2*stage_count]--- the first radix */
        ldr             mstride, [p_factors, #-4]                  /* get factors[2*stage_count-1]--- mstride */

        /* get the constants for converting float to 16 bits fixed point */
        mov             tmp0, #0x1
        lsl             tmp0, tmp0, #15
        vdup.32         q_shift15, tmp0
        lsr             tmp0, tmp0, #1
        sub             tmp0, tmp0, #1
        vdup.16         d_max_div2, tmp0
        lsr             tmp0, tmp0, #1
        vdup.16         d_max_div4, tmp0

        /* the first stage  */
        /* judge the first radix is 2 or 4?  */
        cmp             radix, #2
        mov             p_fin, p_fout
        mov             p_fout0, p_fout
        mov             count, fstride
        bgt             .L_ne10_mixed_radix_butterfly_first_stage_radix4

.L_ne10_mixed_radix_butterfly_first_stage_radix2:
        cmp             fstride, #1
        bgt             .L_ne10_mixed_radix_butterfly_first_stage_radix2_fstride_gt1

        /* fstride eq 1 */
        vld1.16         {d_in01}, [p_fin]
        vqdmull.s16     q_in01, d_in01, d_max_div2
        vaddhn.s32      d_tmp0, q_in01, q_shift15
        vmovl.s16       q_tmp, d_tmp0
        vqsub.s32       d_out1, d_tmp0, d_tmp1
        vqadd.s32       d_out0, d_tmp0, d_tmp1
        vmovn.i32       d_out01, q_out01
        vst1.16         {d_out01}, [p_fout0]
        b               .L_ne10_mixed_radix_butterfly_end

        /* fstride gt 1 */
.L_ne10_mixed_radix_butterfly_first_stage_radix2_fstride_gt1:
        vld4.16         {d_in_r0246, d_in_i0246, d_in_r1357, d_in_i1357}, [p_fin]!
        vqdmull.s16     q_in_r0246, d_in_r0246, d_max_div2
        vqdmull.s16     q_in_i0246, d_in_i0246, d_max_div2
        vqdmull.s16     q_in_r1357, d_in_r1357, d_max_div2
        vqdmull.s16     q_in_i1357, d_in_i1357, d_max_div2
        vaddhn.s32      d_in_r0246, q_in_r0246, q_shift15
        vaddhn.s32      d_in_i0246, q_in_i0246, q_shift15
        vaddhn.s32      d_in_r1357, q_in_r1357, q_shift15
        vaddhn.s32      d_in_i1357, q_in_i1357, q_shift15

        vqsub.s16       q_out_1357, q_in_0246, q_in_1357
        vqadd.s16       q_out_0246, q_in_0246, q_in_1357
        subs            count, count, #4
        vst4.16         {q_out_0246, q_out_1357}, [p_fout0]!

        bgt             .L_ne10_mixed_radix_butterfly_first_stage_radix2_fstride_gt1
        b               .L_ne10_mixed_radix_butterfly_other_stages_judge

.L_ne10_mixed_radix_butterfly_first_stage_radix4:
        cmp             fstride, #1
        bgt             .L_ne10_mixed_radix_butterfly_first_stage_radix4_fstride_gt1

        /* fstride eq 1 */
        vld1.16         {d_in01, d_in23}, [p_fin]
        vqdmull.s16     q_in23, d_in23, d_max_div4
        vqdmull.s16     q_in01, d_in01, d_max_div4
        vaddhn.s32      d_tmp1, q_in23, q_shift15
        vaddhn.s32      d_tmp0, q_in01, q_shift15
        vmovl.s16       q_in23, d_tmp1
        vmovl.s16       q_in01, d_tmp0

        vqsub.s32       d_s2, d_in0, d_in2
        vqadd.s32       d_out0, d_in0, d_in2
        vqadd.s32       d_s0, d_in1, d_in3
        vqsub.s32       d_s1, d_in1, d_in3
        vqsub.s32       d_out2, d_out0, d_s0
        vrev64.32       d_s1, d_s1
        vqadd.s32       d_out0, d_out0, d_s0
        vqadd.s32       d_out1, d_s2, d_s1
        vqsub.s32       d_out3, d_s2, d_s1
        vrev64.32       d_tmp0, d_out1
        vrev64.32       d_tmp1, d_out3
        vtrn.32         d_out3, d_tmp0
        vtrn.32         d_out1, d_tmp1
        vmovn.i32       d_out23, q_out23
        vmovn.i32       d_out01, q_out01
        vst1.32         {d_out01, d_out23}, [p_fout0]

        b               .L_ne10_mixed_radix_butterfly_end

        /* fstride gt 1 */
.L_ne10_mixed_radix_butterfly_first_stage_radix4_fstride_gt1:
        vld4.32         {d_in_04, d_in_15, d_in_26, d_in_37}, [p_fin]!
        vld4.32         {d_in_8c, d_in_9d, d_in_ae, d_in_bf}, [p_fin]!
        vqdmull.s16     q_in_04, d_in_04, d_max_div4
        vqdmull.s16     q_in_15, d_in_15, d_max_div4
        vqdmull.s16     q_in_26, d_in_26, d_max_div4
        vqdmull.s16     q_in_37, d_in_37, d_max_div4
        vqdmull.s16     q_in_8c, d_in_8c, d_max_div4
        vqdmull.s16     q_in_9d, d_in_9d, d_max_div4
        vqdmull.s16     q_in_ae, d_in_ae, d_max_div4
        vqdmull.s16     q_in_bf, d_in_bf, d_max_div4
        vaddhn.s32      d_in_04, q_in_04, q_shift15
        vaddhn.s32      d_in_15, q_in_15, q_shift15
        vaddhn.s32      d_in_26, q_in_26, q_shift15
        vaddhn.s32      d_in_37, q_in_37, q_shift15
        vaddhn.s32      d_in_8c, q_in_8c, q_shift15
        vaddhn.s32      d_in_9d, q_in_9d, q_shift15
        vaddhn.s32      d_in_ae, q_in_ae, q_shift15
        vaddhn.s32      d_in_bf, q_in_bf, q_shift15

        vqsub.s16       q_s2_0123, q_in_048c, q_in_26ae
        vqadd.s16       q_out_048c, q_in_048c, q_in_26ae
        vqsub.s16       q_s1_0123, q_in_159d, q_in_37bf
        vqadd.s16       q_s0_0123, q_in_159d, q_in_37bf
        vrev32.16       q_s1_0123, q_s1_0123
        vqsub.s16       q_out_26ae, q_out_048c, q_s0_0123
        vqadd.s16       q_out_048c, q_out_048c, q_s0_0123
        vqadd.s16       q_out_159d, q_s2_0123, q_s1_0123
        vqsub.s16       q_out_37bf, q_s2_0123, q_s1_0123
        vrev32.16       q_tmp, q_out_159d
        vrev32.16       q_tmp2, q_out_37bf
        vtrn.16         q_out_37bf, q_tmp
        vtrn.16         q_out_159d, q_tmp2
        subs            count, count, #4
        vst4.32         {d_out_04, d_out_15, d_out_26, d_out_37}, [p_fout0]!
        vst4.32         {d_out_8c, d_out_9d, d_out_ae, d_out_bf}, [p_fout0]!

        bgt             .L_ne10_mixed_radix_butterfly_first_stage_radix4_fstride_gt1


        /* the other stages  */
.L_ne10_mixed_radix_butterfly_other_stages_judge:
        subs            stage_count, stage_count, #1
        ble             .L_ne10_mixed_radix_butterfly_end

        /* loop of the other stages  */
.L_ne10_mixed_radix_butterfly_other_stages:
        lsr             fstride, fstride, #2

        /* loop of fstride  */
        mov             count_f, fstride

.L_ne10_mixed_radix_butterfly_other_stages_fstride:
        sub             tmp0, fstride, count_f
        mul             tmp0, tmp0, mstride
        add             p_fout0, p_fout, tmp0, lsl #4
        add             p_fout2, p_fout0, mstride, lsl #3   /* get the address of F[mstride*2] */
        add             p_fout1, p_fout0, mstride, lsl #2   /* get the address of F[mstride] */
        add             p_fout3, p_fout2, mstride, lsl #2   /* get the address of F[mstride*3] */
        mov             p_tw1, p_twiddles
        add             p_tw2, p_tw1, mstride, lsl #2       /* get the address of tw2 */
        add             p_tw3, p_tw1, mstride, lsl #3       /* get the address of tw3 */

        /* loop of mstride  */
        mov             count_m, mstride
        cmp             mstride, #2
        beq             .L_ne10_mixed_radix_butterfly_other_stages_mstride_2

.L_ne10_mixed_radix_butterfly_other_stages_mstride_ge2:
        vld2.16         {d_fin0_r, d_fin0_i}, [p_fout0]
        vld2.16         {d_fin1_r, d_fin1_i}, [p_fout1]
        vld2.16         {d_fin2_r, d_fin2_i}, [p_fout2]
        vld2.16         {d_fin3_r, d_fin3_i}, [p_fout3]
        vld2.16         {d_tw1_r, d_tw1_i}, [p_tw1]!
        vld2.16         {d_tw2_r, d_tw2_i}, [p_tw2]!
        vld2.16         {d_tw3_r, d_tw3_i}, [p_tw3]!
        vqdmull.s16     q_fin1_r, d_fin1_r, d_max_div4
        vqdmull.s16     q_fin1_i, d_fin1_i, d_max_div4
        vqdmull.s16     q_fin2_r, d_fin2_r, d_max_div4
        vqdmull.s16     q_fin2_i, d_fin2_i, d_max_div4
        vaddhn.s32      d_fin1_r, q_fin1_r, q_shift15
        vaddhn.s32      d_fin1_i, q_fin1_i, q_shift15
        vaddhn.s32      d_fin2_r, q_fin2_r, q_shift15
        vaddhn.s32      d_fin2_i, q_fin2_i, q_shift15

        vqdmull.s16     q_fin3_r, d_fin3_r, d_max_div4
        vqdmull.s16     q_fin3_i, d_fin3_i, d_max_div4
        vqdmull.s16     q_fin0_r, d_fin0_r, d_max_div4
        vqdmull.s16     q_fin0_i, d_fin0_i, d_max_div4
        vaddhn.s32      d_fin3_r, q_fin3_r, q_shift15
        vaddhn.s32      d_fin3_i, q_fin3_i, q_shift15
        vaddhn.s32      d_fin0_r, q_fin0_r, q_shift15
        vaddhn.s32      d_fin0_i, q_fin0_i, q_shift15

        vqdmull.s16      q_s0_r, d_fin1_r, d_tw1_r
        vqdmull.s16      q_s0_i, d_fin1_r, d_tw1_i
        vqdmull.s16      q_s1_r, d_fin2_r, d_tw2_r
        vqdmull.s16      q_s1_i, d_fin2_r, d_tw2_i
        vqdmlsl.s16      q_s0_r, d_fin1_i, d_tw1_i
        vqdmlal.s16      q_s0_i, d_fin1_i, d_tw1_r
        vqdmlsl.s16      q_s1_r, d_fin2_i, d_tw2_i
        vqdmlal.s16      q_s1_i, d_fin2_i, d_tw2_r
        vqdmull.s16      q_s2_r, d_fin3_r, d_tw3_r
        vqdmull.s16      q_s2_i, d_fin3_r, d_tw3_i
        vaddhn.s32       d_s0_r, q_s0_r, q_shift15
        vaddhn.s32       d_s0_i, q_s0_i, q_shift15
        vqdmlsl.s16      q_s2_r, d_fin3_i, d_tw3_i
        vqdmlal.s16      q_s2_i, d_fin3_i, d_tw3_r
        vaddhn.s32       d_s1_r, q_s1_r, q_shift15
        vaddhn.s32       d_s1_i, q_s1_i, q_shift15
        vaddhn.s32       d_s2_r, q_s2_r, q_shift15
        vaddhn.s32       d_s2_i, q_s2_i, q_shift15

        vqsub.s16       q_s5, q_fin0, q_s1
        vqadd.s16       q_fout0, q_fin0, q_s1
        vqadd.s16       q_s3, q_s0, q_s2
        vqsub.s16       q_s4, q_s0, q_s2
        vqsub.s16       q_fout2, q_fout0, q_s3
        vqadd.s16       q_fout0, q_fout0, q_s3
        vst2.16         {d_fout2_r, d_fout2_i}, [p_fout2]!

        vqadd.s32       d_fout1_r, d_s5_r, d_s4_i
        vqsub.s32       d_fout1_i, d_s5_i, d_s4_r
        vqsub.s32       d_fout3_r, d_s5_r, d_s4_i
        vqadd.s32       d_fout3_i, d_s5_i, d_s4_r
        vst2.16         {d_fout0_r, d_fout0_i}, [p_fout0]!
        vst2.16         {d_fout1_r, d_fout1_i}, [p_fout1]!
        vst2.16         {d_fout3_r, d_fout3_i}, [p_fout3]!

        subs            count_m, count_m, #4
        bgt             .L_ne10_mixed_radix_butterfly_other_stages_mstride_ge2

        /* end of mstride_ge2 loop */
        b               .L_ne10_mixed_radix_butterfly_other_stages_mstride_end

.L_ne10_mixed_radix_butterfly_other_stages_mstride_2:
        vld2.16         {d_fin_r0123, d_fin_r4567, d_fin_i0123, d_fin_i4567}, [p_fout0]
        vld2.16         {d_tw_r2345, d_tw_r67xx, d_tw_i2345, d_tw_i67xx}, [p_tw1]
        vqdmull.s16     q_tmp, d_fin_r0123, d_max_div4
        vqdmull.s16     q_tmp1, d_fin_r4567, d_max_div4
        vqdmull.s16     q_tmp2, d_fin_i0123, d_max_div4
        vqdmull.s16     q_tmp3, d_fin_i4567, d_max_div4
        vaddhn.s32      d_fin_r0123, q_tmp, q_shift15
        vaddhn.s32      d_fin_r4567, q_tmp1, q_shift15
        vaddhn.s32      d_fin_i0123, q_tmp2, q_shift15
        vaddhn.s32      d_fin_i4567, q_tmp3, q_shift15
        vmovl.s16       q_fin_r0123, d_fin_r0123
        vmovl.s16       q_fin_i0123, d_fin_i0123
        vmovl.s16       q_fin_r4567, d_fin_r4567
        vmovl.s16       q_fin_i4567, d_fin_i4567
        vmovl.s16       q_tw_r2345, d_tw_r2345
        vmovl.s16       q_tw_i2345, d_tw_i2345
        vmovl.s16       q_tw_r67xx, d_tw_r67xx
        vmovl.s16       q_tw_i67xx, d_tw_i67xx

        vmul.s32         d_s0_r01, d_fin_r23, d_tw_r23
        vmul.s32         d_s0_i01, d_fin_r23, d_tw_i23
        vmul.s32         d_s1_r01, d_fin_r45, d_tw_r45
        vmul.s32         d_s1_i01, d_fin_r45, d_tw_i45
        vmul.s32         d_s2_r01, d_fin_r67, d_tw_r67
        vmul.s32         d_s2_i01, d_fin_r67, d_tw_i67
        vmls.s32         d_s0_r01, d_fin_i23, d_tw_i23
        vmla.s32         d_s0_i01, d_fin_i23, d_tw_r23
        vmls.s32         d_s1_r01, d_fin_i45, d_tw_i45
        vmla.s32         d_s1_i01, d_fin_i45, d_tw_r45
        vmls.s32         d_s2_r01, d_fin_i67, d_tw_i67
        vmla.s32         d_s2_i01, d_fin_i67, d_tw_r67
        vshr.s32         q_tmp, q_shift15, #1
        vqadd.s32        q_s0_01, q_s0_01, q_tmp
        vqadd.s32        q_s1_01, q_s1_01, q_tmp
        vqadd.s32        q_s2_01, q_s2_01, q_tmp
        vshr.s32         q_s0_01, q_s0_01, #15
        vshr.s32         q_s1_01, q_s1_01, #15
        vshr.s32         q_s2_01, q_s2_01, #15

        vqsub.s32       d_s5_r01, d_fin_r01, d_s1_r01
        vqsub.s32       d_s5_i01, d_fin_i01, d_s1_i01
        vqadd.s32       d_fout_r01, d_fin_r01, d_s1_r01
        vqadd.s32       d_fout_i01, d_fin_i01, d_s1_i01

        vqadd.s32       d_s3_r01, d_s0_r01, d_s2_r01
        vqadd.s32       d_s3_i01, d_s0_i01, d_s2_i01
        vqsub.s32       d_s4_r01, d_s0_r01, d_s2_r01
        vqsub.s32       d_s4_i01, d_s0_i01, d_s2_i01

        vqsub.s32       d_fout_r45, d_fout_r01, d_s3_r01
        vqsub.s32       d_fout_i45, d_fout_i01, d_s3_i01
        vqadd.s32       d_fout_r01, d_fout_r01, d_s3_r01
        vqadd.s32       d_fout_i01, d_fout_i01, d_s3_i01

        vqadd.s32       d_fout_r23, d_s5_r01, d_s4_i01
        vqsub.s32       d_fout_i23, d_s5_i01, d_s4_r01
        vqsub.s32       d_fout_r67, d_s5_r01, d_s4_i01
        vqadd.s32       d_fout_i67, d_s5_i01, d_s4_r01
        vqmovn.s32      d_fout_r0123, q_fout_r0123
        vqmovn.s32      d_fout_i0123, q_fout_i0123
        vqmovn.s32      d_fout_r4567, q_fout_r4567
        vqmovn.s32      d_fout_i4567, q_fout_i4567
        vst2.16         {d_fout_r0123, d_fout_r4567, d_fout_i0123, d_fout_i4567}, [p_fout0]
        /* end of mstride_2 loop */

.L_ne10_mixed_radix_butterfly_other_stages_mstride_end:
        subs            count_f, count_f, #1
        bgt             .L_ne10_mixed_radix_butterfly_other_stages_fstride

        add             p_twiddles, p_twiddles, mstride, lsl #3
        add             p_twiddles, p_twiddles, mstride, lsl #2 /* get the address of twiddles += mstride*3 */
        lsl             mstride, mstride, #2

        subs            stage_count, stage_count, #1
        bgt             .L_ne10_mixed_radix_butterfly_other_stages


.L_ne10_mixed_radix_butterfly_end:
        /*Return From Function*/
        vpop            {d8-d11}
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
        .global ne10_mixed_radix_butterfly_inverse_int16_neon
        .thumb
        .thumb_func

ne10_mixed_radix_butterfly_inverse_int16_neon:

        push            {r4-r12,lr}
        vpush           {d8-d11}

        ldr             stage_count, [p_factors]   /* get factors[0]---stage_count */
        ldr             fstride, [p_factors, #4]   /* get factors[1]---fstride */
        add             p_factors, p_factors, stage_count, lsl #3 /* get the address of factors[2*stage_count] */
        ldr             radix, [p_factors]                         /* get factors[2*stage_count]--- the first radix */
        ldr             mstride, [p_factors, #-4]                  /* get factors[2*stage_count-1]--- mstride */

        /* get the constants for converting float to 16 bits fixed point */
        mov             tmp0, #0x1
        lsl             tmp0, tmp0, #15
        vdup.32         q_shift15, tmp0
        lsr             tmp0, tmp0, #1
        sub             tmp0, tmp0, #1
        vdup.16         d_max_div2, tmp0
        lsr             tmp0, tmp0, #1
        vdup.16         d_max_div4, tmp0


        /* the first stage  */
        /* judge the first radix is 2 or 4?  */
        cmp             radix, #2
        mov             p_fin, p_fout
        mov             p_fout0, p_fout
        mov             count, fstride
        bgt             .L_ne10_mixed_radix_butterfly_inverse_first_stage_radix4

.L_ne10_mixed_radix_butterfly_inverse_first_stage_radix2:
        cmp             fstride, #1
        bgt             .L_ne10_mixed_radix_butterfly_inverse_first_stage_radix2_fstride_gt1

        /* fstride eq 1 */
        vld1.16         {d_in01}, [p_fin]
        vqdmull.s16     q_in01, d_in01, d_max_div2
        vaddhn.s32      d_tmp0, q_in01, q_shift15
        vmovl.s16       q_tmp, d_tmp0
        vqsub.s32       d_out1, d_tmp0, d_tmp1
        vqadd.s32       d_out0, d_tmp0, d_tmp1
        vmovn.i32       d_out01, q_out01
        vst1.16         {d_out01}, [p_fout0]
        b               .L_ne10_mixed_radix_butterfly_inverse_end


        /* fstride gt 1 */
.L_ne10_mixed_radix_butterfly_inverse_first_stage_radix2_fstride_gt1:
        vld4.16         {d_in_r0246, d_in_i0246, d_in_r1357, d_in_i1357}, [p_fin]!
        vqdmull.s16     q_in_r0246, d_in_r0246, d_max_div2
        vqdmull.s16     q_in_i0246, d_in_i0246, d_max_div2
        vqdmull.s16     q_in_r1357, d_in_r1357, d_max_div2
        vqdmull.s16     q_in_i1357, d_in_i1357, d_max_div2
        vaddhn.s32      d_in_r0246, q_in_r0246, q_shift15
        vaddhn.s32      d_in_i0246, q_in_i0246, q_shift15
        vaddhn.s32      d_in_r1357, q_in_r1357, q_shift15
        vaddhn.s32      d_in_i1357, q_in_i1357, q_shift15

        vqsub.s16       q_out_1357, q_in_0246, q_in_1357
        vqadd.s16       q_out_0246, q_in_0246, q_in_1357
        subs            count, count, #4
        vst4.16         {q_out_0246, q_out_1357}, [p_fout0]!

        bgt             .L_ne10_mixed_radix_butterfly_inverse_first_stage_radix2_fstride_gt1
        b               .L_ne10_mixed_radix_butterfly_inverse_other_stages_judge


.L_ne10_mixed_radix_butterfly_inverse_first_stage_radix4:
        cmp             fstride, #1
        bgt             .L_ne10_mixed_radix_butterfly_inverse_first_stage_radix4_fstride_gt1

        /* fstride eq 1 */
        vld1.16         {d_in01, d_in23}, [p_fin]
        vqdmull.s16     q_in23, d_in23, d_max_div4
        vqdmull.s16     q_in01, d_in01, d_max_div4
        vaddhn.s32      d_tmp1, q_in23, q_shift15
        vaddhn.s32      d_tmp0, q_in01, q_shift15
        vmovl.s16       q_in23, d_tmp1
        vmovl.s16       q_in01, d_tmp0

        vqsub.s32       d_s2, d_in0, d_in2
        vqadd.s32       d_out0, d_in0, d_in2
        vqadd.s32       d_s0, d_in1, d_in3
        vqsub.s32       d_s1, d_in1, d_in3
        vqsub.s32       d_out2, d_out0, d_s0
        vrev64.32       d_s1, d_s1
        vqadd.s32       d_out0, d_out0, d_s0
        vqsub.s32       d_out1, d_s2, d_s1
        vqadd.s32       d_out3, d_s2, d_s1
        vrev64.32       d_tmp0, d_out1
        vrev64.32       d_tmp1, d_out3
        vtrn.32         d_out3, d_tmp0
        vtrn.32         d_out1, d_tmp1
        vmovn.i32       d_out23, q_out23
        vmovn.i32       d_out01, q_out01
        vst1.32         {d_out01, d_out23}, [p_fout0]

        b               .L_ne10_mixed_radix_butterfly_inverse_end

        /* fstride gt 1 */
.L_ne10_mixed_radix_butterfly_inverse_first_stage_radix4_fstride_gt1:
        vld4.32         {d_in_04, d_in_15, d_in_26, d_in_37}, [p_fin]!
        vld4.32         {d_in_8c, d_in_9d, d_in_ae, d_in_bf}, [p_fin]!
        vqdmull.s16     q_in_04, d_in_04, d_max_div4
        vqdmull.s16     q_in_15, d_in_15, d_max_div4
        vqdmull.s16     q_in_26, d_in_26, d_max_div4
        vqdmull.s16     q_in_37, d_in_37, d_max_div4
        vqdmull.s16     q_in_8c, d_in_8c, d_max_div4
        vqdmull.s16     q_in_9d, d_in_9d, d_max_div4
        vqdmull.s16     q_in_ae, d_in_ae, d_max_div4
        vqdmull.s16     q_in_bf, d_in_bf, d_max_div4
        vaddhn.s32      d_in_04, q_in_04, q_shift15
        vaddhn.s32      d_in_15, q_in_15, q_shift15
        vaddhn.s32      d_in_26, q_in_26, q_shift15
        vaddhn.s32      d_in_37, q_in_37, q_shift15
        vaddhn.s32      d_in_8c, q_in_8c, q_shift15
        vaddhn.s32      d_in_9d, q_in_9d, q_shift15
        vaddhn.s32      d_in_ae, q_in_ae, q_shift15
        vaddhn.s32      d_in_bf, q_in_bf, q_shift15

        vqsub.s16       q_s2_0123, q_in_048c, q_in_26ae
        vqadd.s16       q_out_048c, q_in_048c, q_in_26ae
        vqsub.s16       q_s1_0123, q_in_159d, q_in_37bf
        vqadd.s16       q_s0_0123, q_in_159d, q_in_37bf
        vrev32.16       q_s1_0123, q_s1_0123
        vqsub.s16       q_out_26ae, q_out_048c, q_s0_0123
        vqadd.s16       q_out_048c, q_out_048c, q_s0_0123
        vqsub.s16       q_out_159d, q_s2_0123, q_s1_0123
        vqadd.s16       q_out_37bf, q_s2_0123, q_s1_0123
        vrev32.16       q_tmp, q_out_159d
        vrev32.16       q_tmp2, q_out_37bf
        vtrn.16         q_out_37bf, q_tmp
        vtrn.16         q_out_159d, q_tmp2
        subs            count, count, #4
        vst4.32         {d_out_04, d_out_15, d_out_26, d_out_37}, [p_fout0]!
        vst4.32         {d_out_8c, d_out_9d, d_out_ae, d_out_bf}, [p_fout0]!

        bgt             .L_ne10_mixed_radix_butterfly_inverse_first_stage_radix4_fstride_gt1


        /* the other stages  */
.L_ne10_mixed_radix_butterfly_inverse_other_stages_judge:
        subs            stage_count, stage_count, #1
        ble             .L_ne10_mixed_radix_butterfly_inverse_end

        /* loop of the other stages  */
.L_ne10_mixed_radix_butterfly_inverse_other_stages:
        lsr             fstride, fstride, #2

        /* loop of fstride  */
        mov             count_f, fstride

.L_ne10_mixed_radix_butterfly_inverse_other_stages_fstride:
        sub             tmp0, fstride, count_f
        mul             tmp0, tmp0, mstride
        add             p_fout0, p_fout, tmp0, lsl #4
        add             p_fout2, p_fout0, mstride, lsl #3   /* get the address of F[mstride*2] */
        add             p_fout1, p_fout0, mstride, lsl #2   /* get the address of F[mstride] */
        add             p_fout3, p_fout2, mstride, lsl #2   /* get the address of F[mstride*3] */
        mov             p_tw1, p_twiddles
        add             p_tw2, p_tw1, mstride, lsl #2       /* get the address of tw2 */
        add             p_tw3, p_tw1, mstride, lsl #3       /* get the address of tw3 */

        /* loop of mstride  */
        mov             count_m, mstride
        cmp             mstride, #2
        beq             .L_ne10_mixed_radix_butterfly_inverse_other_stages_mstride_2

.L_ne10_mixed_radix_butterfly_inverse_other_stages_mstride_ge2:
        vld2.16         {d_fin0_r, d_fin0_i}, [p_fout0]
        vld2.16         {d_fin1_r, d_fin1_i}, [p_fout1]
        vld2.16         {d_fin2_r, d_fin2_i}, [p_fout2]
        vld2.16         {d_fin3_r, d_fin3_i}, [p_fout3]
        vld2.16         {d_tw1_r, d_tw1_i}, [p_tw1]!
        vld2.16         {d_tw2_r, d_tw2_i}, [p_tw2]!
        vld2.16         {d_tw3_r, d_tw3_i}, [p_tw3]!
        vqdmull.s16     q_fin1_r, d_fin1_r, d_max_div4
        vqdmull.s16     q_fin1_i, d_fin1_i, d_max_div4
        vqdmull.s16     q_fin2_r, d_fin2_r, d_max_div4
        vqdmull.s16     q_fin2_i, d_fin2_i, d_max_div4
        vaddhn.s32      d_fin1_r, q_fin1_r, q_shift15
        vaddhn.s32      d_fin1_i, q_fin1_i, q_shift15
        vaddhn.s32      d_fin2_r, q_fin2_r, q_shift15
        vaddhn.s32      d_fin2_i, q_fin2_i, q_shift15

        vqdmull.s16     q_fin3_r, d_fin3_r, d_max_div4
        vqdmull.s16     q_fin3_i, d_fin3_i, d_max_div4
        vqdmull.s16     q_fin0_r, d_fin0_r, d_max_div4
        vqdmull.s16     q_fin0_i, d_fin0_i, d_max_div4
        vaddhn.s32      d_fin3_r, q_fin3_r, q_shift15
        vaddhn.s32      d_fin3_i, q_fin3_i, q_shift15
        vaddhn.s32      d_fin0_r, q_fin0_r, q_shift15
        vaddhn.s32      d_fin0_i, q_fin0_i, q_shift15

        vqdmull.s16      q_s0_r, d_fin1_r, d_tw1_r
        vqdmull.s16      q_s0_i, d_fin1_i, d_tw1_r
        vqdmull.s16      q_s1_r, d_fin2_r, d_tw2_r
        vqdmull.s16      q_s1_i, d_fin2_i, d_tw2_r
        vqdmlal.s16      q_s0_r, d_fin1_i, d_tw1_i
        vqdmlsl.s16      q_s0_i, d_fin1_r, d_tw1_i
        vqdmlal.s16      q_s1_r, d_fin2_i, d_tw2_i
        vqdmlsl.s16      q_s1_i, d_fin2_r, d_tw2_i
        vqdmull.s16      q_s2_r, d_fin3_r, d_tw3_r
        vqdmull.s16      q_s2_i, d_fin3_i, d_tw3_r
        vaddhn.s32       d_s0_r, q_s0_r, q_shift15
        vaddhn.s32       d_s0_i, q_s0_i, q_shift15
        vqdmlal.s16      q_s2_r, d_fin3_i, d_tw3_i
        vqdmlsl.s16      q_s2_i, d_fin3_r, d_tw3_i
        vaddhn.s32       d_s1_r, q_s1_r, q_shift15
        vaddhn.s32       d_s1_i, q_s1_i, q_shift15
        vaddhn.s32       d_s2_r, q_s2_r, q_shift15
        vaddhn.s32       d_s2_i, q_s2_i, q_shift15

        vqsub.s16       q_s5, q_fin0, q_s1
        vqadd.s16       q_fout0, q_fin0, q_s1
        vqadd.s16       q_s3, q_s0, q_s2
        vqsub.s16       q_s4, q_s0, q_s2
        vqsub.s16       q_fout2, q_fout0, q_s3
        vqadd.s16       q_fout0, q_fout0, q_s3

        vqsub.s32       d_fout1_r, d_s5_r, d_s4_i
        vqadd.s32       d_fout1_i, d_s5_i, d_s4_r
        vqadd.s32       d_fout3_r, d_s5_r, d_s4_i
        vqsub.s32       d_fout3_i, d_s5_i, d_s4_r
        vst2.16         {d_fout2_r, d_fout2_i}, [p_fout2]!
        vst2.16         {d_fout0_r, d_fout0_i}, [p_fout0]!
        vst2.16         {d_fout1_r, d_fout1_i}, [p_fout1]!
        vst2.16         {d_fout3_r, d_fout3_i}, [p_fout3]!

        subs            count_m, count_m, #4
        bgt             .L_ne10_mixed_radix_butterfly_inverse_other_stages_mstride_ge2

        /* end of mstride_ge2 loop */
        b               .L_ne10_mixed_radix_butterfly_inverse_other_stages_mstride_end

.L_ne10_mixed_radix_butterfly_inverse_other_stages_mstride_2:
        vld2.16         {d_fin_r0123, d_fin_r4567, d_fin_i0123, d_fin_i4567}, [p_fout0]
        vld2.16         {d_tw_r2345, d_tw_r67xx, d_tw_i2345, d_tw_i67xx}, [p_tw1]
        vqdmull.s16     q_tmp, d_fin_r0123, d_max_div4
        vqdmull.s16     q_tmp1, d_fin_r4567, d_max_div4
        vqdmull.s16     q_tmp2, d_fin_i0123, d_max_div4
        vqdmull.s16     q_tmp3, d_fin_i4567, d_max_div4
        vaddhn.s32      d_fin_r0123, q_tmp, q_shift15
        vaddhn.s32      d_fin_r4567, q_tmp1, q_shift15
        vaddhn.s32      d_fin_i0123, q_tmp2, q_shift15
        vaddhn.s32      d_fin_i4567, q_tmp3, q_shift15
        vmovl.s16       q_fin_r0123, d_fin_r0123
        vmovl.s16       q_fin_i0123, d_fin_i0123
        vmovl.s16       q_fin_r4567, d_fin_r4567
        vmovl.s16       q_fin_i4567, d_fin_i4567
        vmovl.s16       q_tw_r2345, d_tw_r2345
        vmovl.s16       q_tw_i2345, d_tw_i2345
        vmovl.s16       q_tw_r67xx, d_tw_r67xx
        vmovl.s16       q_tw_i67xx, d_tw_i67xx

        vmul.s32         d_s0_r01, d_fin_r23, d_tw_r23
        vmul.s32         d_s0_i01, d_fin_i23, d_tw_r23
        vmul.s32         d_s1_r01, d_fin_r45, d_tw_r45
        vmul.s32         d_s1_i01, d_fin_i45, d_tw_r45
        vmul.s32         d_s2_r01, d_fin_r67, d_tw_r67
        vmul.s32         d_s2_i01, d_fin_i67, d_tw_r67
        vmla.s32         d_s0_r01, d_fin_i23, d_tw_i23
        vmls.s32         d_s0_i01, d_fin_r23, d_tw_i23
        vmla.s32         d_s1_r01, d_fin_i45, d_tw_i45
        vmls.s32         d_s1_i01, d_fin_r45, d_tw_i45
        vmla.s32         d_s2_r01, d_fin_i67, d_tw_i67
        vmls.s32         d_s2_i01, d_fin_r67, d_tw_i67
        vshr.s32         q_tmp, q_shift15, #1
        vqadd.s32        q_s0_01, q_s0_01, q_tmp
        vqadd.s32        q_s1_01, q_s1_01, q_tmp
        vqadd.s32        q_s2_01, q_s2_01, q_tmp
        vshr.s32         q_s0_01, q_s0_01, #15
        vshr.s32         q_s1_01, q_s1_01, #15
        vshr.s32         q_s2_01, q_s2_01, #15

        vqsub.s32       d_s5_r01, d_fin_r01, d_s1_r01
        vqsub.s32       d_s5_i01, d_fin_i01, d_s1_i01
        vqadd.s32       d_fout_r01, d_fin_r01, d_s1_r01
        vqadd.s32       d_fout_i01, d_fin_i01, d_s1_i01

        vqadd.s32       d_s3_r01, d_s0_r01, d_s2_r01
        vqadd.s32       d_s3_i01, d_s0_i01, d_s2_i01
        vqsub.s32       d_s4_r01, d_s0_r01, d_s2_r01
        vqsub.s32       d_s4_i01, d_s0_i01, d_s2_i01

        vqsub.s32       d_fout_r45, d_fout_r01, d_s3_r01
        vqsub.s32       d_fout_i45, d_fout_i01, d_s3_i01
        vqadd.s32       d_fout_r01, d_fout_r01, d_s3_r01
        vqadd.s32       d_fout_i01, d_fout_i01, d_s3_i01

        vqsub.s32       d_fout_r23, d_s5_r01, d_s4_i01
        vqadd.s32       d_fout_i23, d_s5_i01, d_s4_r01
        vqadd.s32       d_fout_r67, d_s5_r01, d_s4_i01
        vqsub.s32       d_fout_i67, d_s5_i01, d_s4_r01
        vqmovn.s32      d_fout_r0123, q_fout_r0123
        vqmovn.s32      d_fout_i0123, q_fout_i0123
        vqmovn.s32      d_fout_r4567, q_fout_r4567
        vqmovn.s32      d_fout_i4567, q_fout_i4567
        vst2.16         {d_fout_r0123, d_fout_r4567, d_fout_i0123, d_fout_i4567}, [p_fout0]
        /* end of mstride_2 loop */

.L_ne10_mixed_radix_butterfly_inverse_other_stages_mstride_end:
        subs            count_f, count_f, #1
        bgt             .L_ne10_mixed_radix_butterfly_inverse_other_stages_fstride

        add             p_twiddles, p_twiddles, mstride, lsl #3
        add             p_twiddles, p_twiddles, mstride, lsl #2 /* get the address of twiddles += mstride*3 */
        lsl             mstride, mstride, #2

        subs            stage_count, stage_count, #1
        bgt             .L_ne10_mixed_radix_butterfly_inverse_other_stages


.L_ne10_mixed_radix_butterfly_inverse_end:
        /*Return From Function*/
        vpop            {d8-d11}
        pop             {r4-r12,pc}


        /* Registers undefine*/
        /*ARM Registers*/
        .unreq          p_fout
        .unreq          p_factors
        .unreq          p_twiddles
        .unreq          p_fin
        .unreq          p_fout0
        .unreq          p_fout1
        .unreq          p_fout2
        .unreq          p_fout3
        .unreq          stage_count
        .unreq          fstride
        .unreq          mstride
        .unreq          count
        .unreq          count_f
        .unreq          count_m
        .unreq          p_tw1
        .unreq          p_tw2
        .unreq          p_tw3
        .unreq          radix
        .unreq          tmp0

        /*NEON variale Declaration for the first stage*/
        .unreq          d_in01
        .unreq          d_in23
        .unreq          d_in0
        .unreq          d_in1
        .unreq          q_in01
        .unreq          d_in2
        .unreq          d_in3
        .unreq          q_in23
        .unreq          d_out0
        .unreq          d_out1
        .unreq          q_out01
        .unreq          d_out2
        .unreq          d_out3
        .unreq          q_out23
        .unreq          d_out01
        .unreq          d_out23
        .unreq          d_s0
        .unreq          d_s1
        .unreq          d_s2

        .unreq          d_in_r0246
        .unreq          d_in_i0246
        .unreq          q_in_0246
        .unreq          d_in_r1357
        .unreq          d_in_i1357
        .unreq          q_in_1357
        .unreq          q_in_r0246
        .unreq          q_in_i0246
        .unreq          q_in_r1357
        .unreq          q_in_i1357
        .unreq          q_out_0246
        .unreq          q_out_1357

        .unreq          d_in_04
        .unreq          d_in_8c
        .unreq          q_in_048c
        .unreq          d_in_15
        .unreq          d_in_9d
        .unreq          q_in_159d
        .unreq          d_in_26
        .unreq          d_in_ae
        .unreq          q_in_26ae
        .unreq          d_in_37
        .unreq          d_in_bf
        .unreq          q_in_37bf
        .unreq          q_in_04
        .unreq          q_in_15
        .unreq          q_in_26
        .unreq          q_in_37
        .unreq          q_in_8c
        .unreq          q_in_9d
        .unreq          q_in_ae
        .unreq          q_in_bf
        .unreq          d_out_04
        .unreq          d_out_8c
        .unreq          q_out_048c
        .unreq          d_out_15
        .unreq          d_out_9d
        .unreq          q_out_159d
        .unreq          d_out_26
        .unreq          d_out_ae
        .unreq          q_out_26ae
        .unreq          d_out_37
        .unreq          d_out_bf
        .unreq          q_out_37bf
        .unreq          q_s0_0123
        .unreq          q_s1_0123
        .unreq          q_s2_0123

        /*NEON variale Declaration for mstride ge2 loop */
        .unreq          d_fin0_r
        .unreq          d_fin0_i
        .unreq          q_fin0
        .unreq          d_fin1_r
        .unreq          d_fin1_i
        .unreq          d_fin2_r
        .unreq          d_fin2_i
        .unreq          d_fin3_r
        .unreq          d_fin3_i
        .unreq          q_fin1_r
        .unreq          q_fin1_i
        .unreq          q_fin2_r
        .unreq          q_fin2_i
        .unreq          q_fin0_r
        .unreq          q_fin0_i
        .unreq          q_fin3_r
        .unreq          q_fin3_i
        .unreq          d_tw1_r
        .unreq          d_tw1_i
        .unreq          d_tw2_r
        .unreq          d_tw2_i
        .unreq          d_tw3_r
        .unreq          d_tw3_i
        .unreq          d_s0_r
        .unreq          d_s0_i
        .unreq          q_s0
        .unreq          d_s1_r
        .unreq          d_s1_i
        .unreq          q_s1
        .unreq          d_s2_r
        .unreq          d_s2_i
        .unreq          q_s2
        .unreq          d_s5_r
        .unreq          d_s5_i
        .unreq          q_s5
        .unreq          d_s4_r
        .unreq          d_s4_i
        .unreq          q_s4
        .unreq          d_s3_r
        .unreq          d_s3_i
        .unreq          q_s3
        .unreq          q_s0_r
        .unreq          q_s0_i
        .unreq          q_s1_r
        .unreq          q_s1_i
        .unreq          q_s2_r
        .unreq          q_s2_i
        .unreq          d_fout0_r
        .unreq          d_fout0_i
        .unreq          q_fout0
        .unreq          d_fout2_r
        .unreq          d_fout2_i
        .unreq          q_fout2
        .unreq          d_fout1_r
        .unreq          d_fout1_i
        .unreq          d_fout3_r
        .unreq          d_fout3_i

        /*NEON variale Declaration for mstride 2 loop */
        .unreq          d_fin_r0123
        .unreq          d_fin_r4567
        .unreq          d_fin_i0123
        .unreq          d_fin_i4567
        .unreq          d_tw_r2345
        .unreq          d_tw_r67xx
        .unreq          d_tw_i2345
        .unreq          d_tw_i67xx
        .unreq          d_fin_r01
        .unreq          d_fin_r23
        .unreq          q_fin_r0123
        .unreq          d_fin_i01
        .unreq          d_fin_i23
        .unreq          q_fin_i0123
        .unreq          d_fin_r45
        .unreq          d_fin_r67
        .unreq          q_fin_r4567
        .unreq          d_fin_i45
        .unreq          d_fin_i67
        .unreq          q_fin_i4567
        .unreq          d_tw_r23
        .unreq          d_tw_r45
        .unreq          q_tw_r2345
        .unreq          d_tw_i23
        .unreq          d_tw_i45
        .unreq          q_tw_i2345
        .unreq          d_tw_r67
        .unreq          q_tw_r67xx
        .unreq          d_tw_i67
        .unreq          q_tw_i67xx
        .unreq          d_s0_r01
        .unreq          d_s0_i01
        .unreq          q_s0_01
        .unreq          d_s1_r01
        .unreq          d_s1_i01
        .unreq          q_s1_01
        .unreq          d_s2_r01
        .unreq          d_s2_i01
        .unreq          q_s2_01
        .unreq          d_s5_r01
        .unreq          d_s5_i01
        .unreq          d_s4_r01
        .unreq          d_s4_i01
        .unreq          d_s3_r01
        .unreq          d_s3_i01

        .unreq          d_fout_r01
        .unreq          d_fout_r23
        .unreq          q_fout_r0123
        .unreq          d_fout_i01
        .unreq          d_fout_i23
        .unreq          q_fout_i0123
        .unreq          d_fout_r45
        .unreq          d_fout_r67
        .unreq          q_fout_r4567
        .unreq          d_fout_i45
        .unreq          d_fout_i67
        .unreq          q_fout_i4567
        .unreq          d_fout_r0123
        .unreq          d_fout_r4567
        .unreq          d_fout_i0123
        .unreq          d_fout_i4567

        .unreq          d_tmp0
        .unreq          d_tmp1
        .unreq          q_tmp
        .unreq          d_tmp1_0
        .unreq          d_tmp1_1
        .unreq          q_tmp1
        .unreq          d_tmp2_0
        .unreq          d_tmp2_1
        .unreq          q_tmp2
        .unreq          q_tmp3

        /* end of the file */
        .end
