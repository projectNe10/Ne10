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
        p_fin           .req   r1
        p_factors       .req   r2
        p_twiddles      .req   r3
        stage_count     .req   r4
        fstride         .req   r5
        mstride         .req   r6

        radix           .req   r12
        p_fin0          .req   r7
        p_fin1          .req   r8
        p_fin2          .req   r9
        p_fin3          .req   r10
        p_tmp           .req   r11
        count           .req   r2
        fstride1        .req   r2
        fstep           .req   r7

        p_out_ls        .req   r14
        nstep           .req   r2
        mstep           .req   r7
        count_f         .req   r8
        count_m         .req   r9
        p_tw1           .req   r10
        p_in1           .req   r11
        p_out1          .req   r12
        tmp0            .req   r9

        /*NEON variale Declaration for the first stage*/
        q_in0_01        .qn   q0
        q_in1_01        .qn   q1
        q_in2_01        .qn   q2
        q_in3_01        .qn   q3
        q_s0_2          .qn   q4
        q_s1_2          .qn   q5
        q_s2_2          .qn   q6
        q_s3_2          .qn   q7
        d_s1_r2         .dn   d10
        d_s1_i2         .dn   d11
        d_s3_r2         .dn   d14
        d_s3_i2         .dn   d15
        q_out0_2        .qn   q8
        q_out1_2        .qn   q9
        q_out2_2        .qn   q10
        q_out3_2        .qn   q11
        d_out1_r15      .dn   d18
        d_out1_i15      .dn   d19
        d_out3_r37      .dn   d22
        d_out3_i37      .dn   d23

        d_in0_r         .dn   d0
        d_in0_i         .dn   d1
        d_in1_r         .dn   d2
        d_in1_i         .dn   d3
        d_in2_r         .dn   d4
        d_in2_i         .dn   d5
        d_in3_r         .dn   d6
        d_in3_i         .dn   d7
        d_in4_r         .dn   d8
        d_in4_i         .dn   d9
        d_in5_r         .dn   d10
        d_in5_i         .dn   d11
        d_in6_r         .dn   d12
        d_in6_i         .dn   d13
        d_in7_r         .dn   d14
        d_in7_i         .dn   d15
        q_in0           .qn   q0
        q_in1           .qn   q1
        q_in2           .qn   q2
        q_in3           .qn   q3
        q_in4           .qn   q4
        q_in5           .qn   q5
        q_in6           .qn   q6
        q_in7           .qn   q7
        q_sin0          .qn   q8
        q_sin1          .qn   q9
        q_sin2          .qn   q10
        q_sin3          .qn   q11
        q_sin4          .qn   q12
        q_sin5          .qn   q13
        q_sin6          .qn   q14
        q_sin7          .qn   q15
        d_sin3_r        .dn   d22
        d_sin3_i        .dn   d23
        d_sin5_r        .dn   d26
        d_sin5_i        .dn   d27
        d_sin7_r        .dn   d30
        d_sin7_i        .dn   d31

        d_tw_twn        .dn   d0
        d_s3_r          .dn   d2
        d_s3_i          .dn   d3
        d_s7_r          .dn   d4
        d_s7_i          .dn   d5
        q_s3            .qn   q1
        q_s7            .qn   q2
        q_s8            .qn   q11
        q_s9            .qn   q15
        q_s10           .qn   q3
        q_s11           .qn   q4
        q_s12           .qn   q5
        q_s13           .qn   q6
        q_s14           .qn   q7
        q_s15           .qn   q0
        q_out0          .qn   q1
        q_out1          .qn   q2
        q_out2          .qn   q8
        q_out3          .qn   q9
        q_out4          .qn   q10
        q_out5          .qn   q12
        q_out6          .qn   q13
        q_out7          .qn   q14
        d_s10_r         .dn   d6
        d_s10_i         .dn   d7
        d_s11_r         .dn   d8
        d_s11_i         .dn   d9
        d_s14_r         .dn   d14
        d_s14_i         .dn   d15
        d_s15_r         .dn   d0
        d_s15_i         .dn   d1
        d_out2_r        .dn   d16
        d_out2_i        .dn   d17
        d_out3_r        .dn   d18
        d_out3_i        .dn   d19
        d_out6_r        .dn   d26
        d_out6_i        .dn   d27
        d_out7_r        .dn   d28
        d_out7_i        .dn   d29


        /*NEON variale Declaration for mstride loop */
        d_fin0_r        .dn   d0
        d_fin0_i        .dn   d1
        d_fin1_r        .dn   d2
        d_fin1_i        .dn   d3
        d_fin2_r        .dn   d4
        d_fin2_i        .dn   d5
        d_fin3_r        .dn   d6
        d_fin3_i        .dn   d7
        d_tw0_r         .dn   d8
        d_tw0_i         .dn   d9
        d_tw1_r         .dn   d10
        d_tw1_i         .dn   d11
        d_tw2_r         .dn   d12
        d_tw2_i         .dn   d13
        q_fin0          .qn   q0
        q_scr0          .qn   q15
        q_scr1          .qn   q7
        q_scr2          .qn   q8
        q_scr3          .qn   q9
        q_scr4          .qn   q10
        q_scr5          .qn   q11
        q_scr6          .qn   q12
        q_scr7          .qn   q13
        d_scr1_r        .dn   d14
        d_scr1_i        .dn   d15
        d_scr2_r        .dn   d16
        d_scr2_i        .dn   d17
        d_scr3_r        .dn   d18
        d_scr3_i        .dn   d19
        d_scr5_r        .dn   d22
        d_scr5_i        .dn   d23
        d_scr7_r        .dn   d26
        d_scr7_i        .dn   d27
        q_fout0         .qn   q7
        q_fout2         .qn   q8
        q_fout1         .qn   q14
        q_fout3         .qn   q15
        d_fout0_r       .dn   d14
        d_fout0_i       .dn   d15
        d_fout1_r       .dn   d28
        d_fout1_i       .dn   d29
        d_fout2_r       .dn   d16
        d_fout2_i       .dn   d17
        d_fout3_r       .dn   d30
        d_fout3_i       .dn   d31
        d_one_by_nfft   .dn   d14
        q_one_by_nfft   .qn   q9

        .macro BUTTERFLY4X2_WITHOUT_TWIDDLES inverse

        /* radix 4 butterfly without twiddles */
        vadd.f32        q_s0_2, q_in0_01, q_in2_01
        vsub.f32        q_s1_2, q_in0_01, q_in2_01
        vld2.32         {q_in0_01}, [p_fin0:64]!
        vld2.32         {q_in2_01}, [p_fin2:64]!
        vadd.f32        q_s2_2, q_in1_01, q_in3_01
        vsub.f32        q_s3_2, q_in1_01, q_in3_01
        vld2.32         {q_in1_01}, [p_fin1:64]!
        vld2.32         {q_in3_01}, [p_fin3:64]!

        vsub.f32        q_out2_2, q_s0_2, q_s2_2
        vadd.f32        q_out0_2, q_s0_2, q_s2_2

        .ifeqs "\inverse", "TRUE"
        vsub.f32        d_out1_r15, d_s1_r2, d_s3_i2
        vadd.f32        d_out1_i15, d_s1_i2, d_s3_r2
        vadd.f32        d_out3_r37, d_s1_r2, d_s3_i2
        vsub.f32        d_out3_i37, d_s1_i2, d_s3_r2
        .else
        vadd.f32        d_out1_r15, d_s1_r2, d_s3_i2
        vsub.f32        d_out1_i15, d_s1_i2, d_s3_r2
        vsub.f32        d_out3_r37, d_s1_r2, d_s3_i2
        vadd.f32        d_out3_i37, d_s1_i2, d_s3_r2
        .endif

        vtrn.32         q_out0_2, q_out1_2
        vtrn.32         q_out2_2, q_out3_2
        vst2.32         {q_out0_2}, [p_tmp]!
        vst2.32         {q_out2_2}, [p_tmp]!
        vst2.32         {q_out1_2}, [p_tmp]!
        vst2.32         {q_out3_2}, [p_tmp]!
        .endm

        .macro BUTTERFLY4X2_WITH_TWIDDLES inverse, last_stage

        sub             p_in1, p_in1, nstep, lsl #2
        add             p_in1, p_in1, #16
        sub             p_tw1, p_tw1, mstep, lsl #1
        add             p_tw1, p_tw1, #16
        vmov            q_scr0, q_fin0
        vmul.f32        d_scr1_r, d_fin1_r, d_tw0_r
        vmul.f32        d_scr1_i, d_fin1_i, d_tw0_r
        vmul.f32        d_scr2_r, d_fin2_r, d_tw1_r
        vmul.f32        d_scr2_i, d_fin2_i, d_tw1_r
        vmul.f32        d_scr3_r, d_fin3_r, d_tw2_r
        vmul.f32        d_scr3_i, d_fin3_i, d_tw2_r
        vld2.32         {d_fin0_r, d_fin0_i}, [p_in1:64], nstep

        .ifeqs "\inverse", "TRUE"
        vmla.f32        d_scr1_r, d_fin1_i, d_tw0_i
        vmls.f32        d_scr1_i, d_fin1_r, d_tw0_i
        vmla.f32        d_scr2_r, d_fin2_i, d_tw1_i
        vmls.f32        d_scr2_i, d_fin2_r, d_tw1_i
        vmla.f32        d_scr3_r, d_fin3_i, d_tw2_i
        vmls.f32        d_scr3_i, d_fin3_r, d_tw2_i
        .else
        vmls.f32        d_scr1_r, d_fin1_i, d_tw0_i
        vmla.f32        d_scr1_i, d_fin1_r, d_tw0_i
        vmls.f32        d_scr2_r, d_fin2_i, d_tw1_i
        vmla.f32        d_scr2_i, d_fin2_r, d_tw1_i
        vmls.f32        d_scr3_r, d_fin3_i, d_tw2_i
        vmla.f32        d_scr3_i, d_fin3_r, d_tw2_i
        .endif

        vld2.32         {d_fin1_r, d_fin1_i}, [p_in1:64], nstep
        vld2.32         {d_tw0_r, d_tw0_i}, [p_tw1:64], mstep
        vld2.32         {d_fin2_r, d_fin2_i}, [p_in1:64], nstep
        vld2.32         {d_fin3_r, d_fin3_i}, [p_in1:64], nstep
        vld2.32         {d_tw1_r, d_tw1_i}, [p_tw1:64], mstep
        vld2.32         {d_tw2_r, d_tw2_i}, [p_tw1:64]

        vadd.f32        q_scr4, q_scr0, q_scr2
        vsub.f32        q_scr5, q_scr0, q_scr2
        vadd.f32        q_scr6, q_scr1, q_scr3
        vsub.f32        q_scr7, q_scr1, q_scr3

        .ifeqs "\inverse", "TRUE"
        .ifeqs "\last_stage", "TRUE"
        vld1.32         {d_one_by_nfft}, [sp]
        vdup.32         q_one_by_nfft, d_one_by_nfft[0]
        .endif
        .endif

        vadd.f32        q_fout0, q_scr4, q_scr6
        vsub.f32        q_fout2, q_scr4, q_scr6

        .ifeqs "\inverse", "TRUE"
        vsub.f32        d_fout1_r, d_scr5_r, d_scr7_i
        vadd.f32        d_fout1_i, d_scr5_i, d_scr7_r
        vadd.f32        d_fout3_r, d_scr5_r, d_scr7_i
        vsub.f32        d_fout3_i, d_scr5_i, d_scr7_r
        .else
        vadd.f32        d_fout1_r, d_scr5_r, d_scr7_i
        vsub.f32        d_fout1_i, d_scr5_i, d_scr7_r
        vsub.f32        d_fout3_r, d_scr5_r, d_scr7_i
        vadd.f32        d_fout3_i, d_scr5_i, d_scr7_r
        .endif

        .ifeqs "\inverse", "TRUE"
        .ifeqs "\last_stage", "TRUE"
        vmul.f32        q_fout0, q_fout0, q_one_by_nfft
        vmul.f32        q_fout2, q_fout2, q_one_by_nfft
        vmul.f32        q_fout1, q_fout1, q_one_by_nfft
        vmul.f32        q_fout3, q_fout3, q_one_by_nfft
        .endif
        .endif

        vst2.32         {d_fout0_r, d_fout0_i}, [p_out1], mstep
        vst2.32         {d_fout1_r, d_fout1_i}, [p_out1], mstep
        vst2.32         {d_fout2_r, d_fout2_i}, [p_out1], mstep
        vst2.32         {d_fout3_r, d_fout3_i}, [p_out1], mstep
        sub             p_out1, p_out1, mstep, lsl #2
        add             p_out1, p_out1, #16

        .endm


        .macro BUTTERFLY8X2_WITHOUT_TWIDDLES inverse
        /**
         *   q_in0: Fin1[0]
         *   q_in1: Fin1[0 + fstride]
         *   q_in2: Fin1[fstride1]
         *   q_in3: Fin1[fstride1 + fstride]
         *   q_in4: Fin1[fstride1*2]
         *   q_in5: Fin1[fstride1*2 + fstride]
         *   q_in6: Fin1[fstride1*3]
         *   q_in7: Fin1[fstride1*3 + fstride]
         *
         */

        ldr             tmp0, =TW_81
        vld2.32         {d_in0_r, d_in0_i}, [p_in1:64], fstep
        vld2.32         {d_in2_r, d_in2_i}, [p_in1:64], fstep
        vld2.32         {d_in4_r, d_in4_i}, [p_in1:64], fstep
        vld2.32         {d_in6_r, d_in6_i}, [p_in1:64], fstep
        vld2.32         {d_in1_r, d_in1_i}, [p_in1:64], fstep
        vld2.32         {d_in3_r, d_in3_i}, [p_in1:64], fstep
        vld2.32         {d_in5_r, d_in5_i}, [p_in1:64], fstep
        vld2.32         {d_in7_r, d_in7_i}, [p_in1:64], fstep

        // radix 4 butterfly without twiddles
        vadd.f32        q_sin0, q_in0, q_in1
        vsub.f32        q_sin1, q_in0, q_in1
        vld1.32         {d_tw_twn}, [tmp0]
        vadd.f32        q_sin2, q_in2, q_in3
        vsub.f32        q_sin3, q_in2, q_in3
        vadd.f32        q_sin4, q_in4, q_in5
        vsub.f32        q_sin5, q_in4, q_in5
        vadd.f32        q_sin6, q_in6, q_in7
        vsub.f32        q_sin7, q_in6, q_in7

        .ifeqs "\inverse", "TRUE"
        vneg.f32        d_sin5_i, d_sin5_i
        vsub.f32        d_s3_r, d_sin3_r, d_sin3_i
        vadd.f32        d_s3_i, d_sin3_i, d_sin3_r
        vadd.f32        d_s7_r, d_sin7_r, d_sin7_i
        vsub.f32        d_s7_i, d_sin7_i, d_sin7_r
        .else
        vneg.f32        d_sin5_r, d_sin5_r
        vadd.f32        d_s3_r, d_sin3_r, d_sin3_i
        vsub.f32        d_s3_i, d_sin3_i, d_sin3_r
        vsub.f32        d_s7_r, d_sin7_r, d_sin7_i
        vadd.f32        d_s7_i, d_sin7_i, d_sin7_r
        .endif
        vswp            d_sin5_r, d_sin5_i

        vmul.f32        q_s3, q_s3, d_tw_twn[0]
        vmul.f32        q_s7, q_s7, d_tw_twn[1]

        // radix 2 butterfly
        vadd.f32        q_s8, q_sin0, q_sin4
        vadd.f32        q_s9, q_sin1, q_sin5
        vsub.f32        q_s10, q_sin0, q_sin4
        vsub.f32        q_s11, q_sin1, q_sin5

        // radix 2 butterfly
        vadd.f32        q_s12, q_sin2, q_sin6
        vadd.f32        q_s13, q_s3, q_s7
        vsub.f32        q_s14, q_sin2, q_sin6
        vsub.f32        q_s15, q_s3, q_s7

        vsub.f32        q_out4, q_s8, q_s12
        vsub.f32        q_out5, q_s9, q_s13
        vadd.f32        q_out0, q_s8, q_s12
        vadd.f32        q_out1, q_s9, q_s13

        .ifeqs "\inverse", "TRUE"
        vsub.f32        d_out2_r, d_s10_r, d_s14_i
        vadd.f32        d_out2_i, d_s10_i, d_s14_r
        vsub.f32        d_out3_r, d_s11_r, d_s15_i
        vadd.f32        d_out3_i, d_s11_i, d_s15_r
        vadd.f32        d_out6_r, d_s10_r, d_s14_i
        vsub.f32        d_out6_i, d_s10_i, d_s14_r
        vadd.f32        d_out7_r, d_s11_r, d_s15_i
        vsub.f32        d_out7_i, d_s11_i, d_s15_r
        .else
        vadd.f32        d_out2_r, d_s10_r, d_s14_i
        vsub.f32        d_out2_i, d_s10_i, d_s14_r
        vadd.f32        d_out3_r, d_s11_r, d_s15_i
        vsub.f32        d_out3_i, d_s11_i, d_s15_r
        vsub.f32        d_out6_r, d_s10_r, d_s14_i
        vadd.f32        d_out6_i, d_s10_i, d_s14_r
        vsub.f32        d_out7_r, d_s11_r, d_s15_i
        vadd.f32        d_out7_i, d_s11_i, d_s15_r
        .endif

        vtrn.32         q_out0, q_out1
        vtrn.32         q_out2, q_out3
        vtrn.32         q_out4, q_out5
        vtrn.32         q_out6, q_out7


        vst2.32         {q_out0}, [p_out1]!
        vst2.32         {q_out2}, [p_out1]!
        vst2.32         {q_out4}, [p_out1]!
        vst2.32         {q_out6}, [p_out1]!
        vst2.32         {q_out1}, [p_out1]!
        vst2.32         {q_out3}, [p_out1]!
        vst2.32         {q_out5}, [p_out1]!
        vst2.32         {q_out7}, [p_out1]!

        sub             p_in1, p_in1, fstep, lsl #3
        add             p_in1, p_in1, #16

        .endm

        .global TW_81
TW_81:
.float 0.70710678
.float -0.70710678


        /**
         * @details
         * This function implements the radix4/8 forward FFT
         *
         * @param[in/out] *Fout        points to input/output pointers
         * @param[in]     *factors     factors pointer:
                                        * 0: stage number
                                        * 1: stride for the first stage
                                        * others: factor out powers of 4, powers of 2
         * @param[in]     *twiddles     twiddles coeffs of FFT
         */

        .align 4
        .global ne10_mixed_radix_fft_forward_float32_neon
        .thumb
        .thumb_func

ne10_mixed_radix_fft_forward_float32_neon:
        push            {r4-r12,lr}
        vpush           {q4-q7}

        ldr             stage_count, [p_factors]   /* get factors[0]---stage_count */
        ldr             fstride, [p_factors, #4]   /* get factors[1]---fstride */
        add             p_factors, p_factors, stage_count, lsl #3 /* get the address of factors[2*stage_count] */
        ldr             radix, [p_factors]                         /* get factors[2*stage_count]--- the first radix */
        ldr             mstride, [p_factors, #-4]                  /* get factors[2*stage_count-1]--- mstride */

        /* save the output buffer for the last stage  */
        mov             p_out_ls, p_fout

        /* ---------------the first stage---------------  */
        /* judge the radix is 2 or 4  */
        cmp             radix, #2
        beq             .L_ne10_radix8_butterfly_first_stage

        /* ---------------first stage: radix 4  */
        mov             count, fstride
        mov             p_fin0, p_fin
        mov             p_tmp, p_fout
        add             p_fin2, p_fin0, fstride, lsl #4   /* get the address of F[fstride*2] */
        add             p_fin1, p_fin0, fstride, lsl #3   /* get the address of F[fstride] */
        add             p_fin3, p_fin2, fstride, lsl #3   /* get the address of F[fstride*3] */
        vld2.32         {q_in0_01}, [p_fin0:64]!
        vld2.32         {q_in2_01}, [p_fin2:64]!
        vld2.32         {q_in1_01}, [p_fin1:64]!
        vld2.32         {q_in3_01}, [p_fin3:64]!

.L_ne10_radix4_butterfly_first_stage_fstride:
        BUTTERFLY4X2_WITHOUT_TWIDDLES "FALSE"

        subs            count, count, #2
        bgt             .L_ne10_radix4_butterfly_first_stage_fstride

        /* swap input/output buffer  */
        ldr             tmp0, [sp, #104]
        mov             p_fin, p_fout
        mov             p_fout, tmp0

        /* (stage_count-2): reduce the counter for the last stage  */
        sub             stage_count, stage_count, #2
        lsl             nstep, fstride, #3
        lsr             fstride, fstride, #2

        b               .L_ne10_butterfly_other_stages
        /* ---------------end of first stage: radix 4  */



        /* ---------------first stage: radix 8  */
.L_ne10_radix8_butterfly_first_stage:
        lsr             fstride1, fstride, #2
        mov             p_in1, p_fin
        mov             p_out1, p_fout
        lsl             fstep, fstride, #1

.L_ne10_radix8_butterfly_first_stage_fstride1:
        BUTTERFLY8X2_WITHOUT_TWIDDLES "FALSE"

        subs            fstride1, fstride1, #2
        bgt             .L_ne10_radix8_butterfly_first_stage_fstride1

        lsl             nstep, fstride, #2
        sub             stage_count, stage_count, #2
        lsr             fstride, fstride, #4
        lsl             mstride, mstride, #2
        add             p_twiddles, p_twiddles, #48 /* get the address of twiddles += 6 */

        /* swap input/output buffer  */
        ldr             tmp0, [sp, #104]
        mov             p_fin, p_fout
        mov             p_fout, tmp0

        /* if the last stage  */
        cmp            stage_count, #1
        beq            .L_ne10_butterfly_last_stages

        /* (stage_count-1): reduce the counter for the last stage  */
        sub            stage_count, stage_count, #1
        /*--------------- end of first stage: radix 8  */
        /* ---------------end of first stage---------------  */


        /* ---------------other stages  except last stage---------------  */
        /* loop of other stages  */
.L_ne10_butterfly_other_stages:
        lsl             mstep, mstride, #3
        mov             p_in1, p_fin
        vld2.32         {d_fin0_r, d_fin0_i}, [p_in1:64], nstep
        vld2.32         {d_fin1_r, d_fin1_i}, [p_in1:64], nstep
        vld2.32         {d_fin2_r, d_fin2_i}, [p_in1:64], nstep
        vld2.32         {d_fin3_r, d_fin3_i}, [p_in1:64], nstep

        /* loop of fstride  */
        mov             count_f, fstride
.L_ne10_butterfly_other_stages_fstride:
        mov             p_tw1, p_twiddles
        sub             tmp0, fstride, count_f
        mul             tmp0, tmp0, mstride
        add             p_out1, p_fout, tmp0, lsl #5
        vld2.32         {d_tw0_r, d_tw0_i}, [p_tw1:64], mstep
        vld2.32         {d_tw1_r, d_tw1_i}, [p_tw1:64], mstep
        vld2.32         {d_tw2_r, d_tw2_i}, [p_tw1:64]

        /* loop of mstride  */
        mov             count_m, mstride

.L_ne10_butterfly_other_stages_mstride:
        BUTTERFLY4X2_WITH_TWIDDLES "FALSE", "FALSE"

        subs            count_m, count_m, #2
        bgt             .L_ne10_butterfly_other_stages_mstride
        /* end of mstride loop */

        subs            count_f, count_f, #1
        bgt             .L_ne10_butterfly_other_stages_fstride

        add             p_twiddles, p_twiddles, mstride, lsl #4
        add             p_twiddles, p_twiddles, mstride, lsl #3 /* get the address of twiddles += mstride*3 */
        lsl             mstride, mstride, #2
        lsr             fstride, fstride, #2

        /* swap input/output buffer  */
        mov             tmp0, p_fout
        mov             p_fout, p_fin
        mov             p_fin, tmp0

        subs            stage_count, stage_count, #1
        bgt             .L_ne10_butterfly_other_stages
        /* ---------------end other stages  except last stage---------------  */


        /* ---------------last stage---------------  */
.L_ne10_butterfly_last_stages:
        mov             p_in1, p_fin
        mov             p_out1, p_out_ls
        mov             p_tw1, p_twiddles
        mov             mstep, nstep
        vld2.32         {d_fin0_r, d_fin0_i}, [p_in1:64], nstep
        vld2.32         {d_fin1_r, d_fin1_i}, [p_in1:64], nstep
        vld2.32         {d_fin2_r, d_fin2_i}, [p_in1:64], nstep
        vld2.32         {d_fin3_r, d_fin3_i}, [p_in1:64], nstep
        vld2.32         {d_tw0_r, d_tw0_i}, [p_tw1:64], mstep
        vld2.32         {d_tw1_r, d_tw1_i}, [p_tw1:64], mstep
        vld2.32         {d_tw2_r, d_tw2_i}, [p_tw1:64]

        /* loop of mstride  */
        mov             count_m, mstride
.L_ne10_butterfly_last_stages_mstride:
        BUTTERFLY4X2_WITH_TWIDDLES "FALSE", "TRUE"

        subs            count_m, count_m, #2
        bgt             .L_ne10_butterfly_last_stages_mstride
        /* end of mstride loop */
        /* ---------------end of last stage---------------  */

.L_ne10_butterfly_end:
        /*Return From Function*/
        vpop            {q4-q7}
        pop             {r4-r12,pc}

        /* end of ne10_mixed_radix_fft_forward_float32_neon */

        /**
         * @details
         * This function implements the radix4/8 backward FFT
         *
         * @param[in/out] *Fout        points to input/output pointers
         * @param[in]     *factors     factors pointer:
                                        * 0: stage number
                                        * 1: stride for the first stage
                                        * others: factor out powers of 4, powers of 2
         * @param[in]     *twiddles     twiddles coeffs of FFT
         */

        .align 4
        .global ne10_mixed_radix_fft_backward_float32_neon
        .thumb
        .thumb_func

ne10_mixed_radix_fft_backward_float32_neon:
        push            {r4-r12,lr}
        vpush           {q4-q7}

        ldr             stage_count, [p_factors]   /* get factors[0]---stage_count */
        ldr             fstride, [p_factors, #4]   /* get factors[1]---fstride */
        add             p_factors, p_factors, stage_count, lsl #3 /* get the address of factors[2*stage_count] */
        ldr             radix, [p_factors]                         /* get factors[2*stage_count]--- the first radix */
        ldr             mstride, [p_factors, #-4]                  /* get factors[2*stage_count-1]--- mstride */


        /* calculate 1/nfft for the last stage  */
        mul             tmp0, radix, fstride
        vmov            s1, tmp0
        vmov.f32        s0, #1.0
        vcvt.f32.s32    s1, s1
        vdiv.f32        s0, s0, s1
        vpush           {s0, s1}


        /* save the output buffer for the last stage  */
        mov             p_out_ls, p_fout

        /* ---------------the first stage---------------  */
        /* judge the radix is 2 or 4  */
        cmp             radix, #2
        beq             .L_ne10_radix8_butterfly_inverse_first_stage

        /* ---------------first stage: radix 4  */
        mov             count, fstride
        mov             p_fin0, p_fin
        mov             p_tmp, p_fout
        add             p_fin2, p_fin0, fstride, lsl #4   /* get the address of F[fstride*2] */
        add             p_fin1, p_fin0, fstride, lsl #3   /* get the address of F[fstride] */
        add             p_fin3, p_fin2, fstride, lsl #3   /* get the address of F[fstride*3] */
        vld2.32         {q_in0_01}, [p_fin0:64]!
        vld2.32         {q_in2_01}, [p_fin2:64]!
        vld2.32         {q_in1_01}, [p_fin1:64]!
        vld2.32         {q_in3_01}, [p_fin3:64]!

.L_ne10_radix4_butterfly_inverse_first_stage_fstride:
        BUTTERFLY4X2_WITHOUT_TWIDDLES "TRUE"

        subs            count, count, #2
        bgt             .L_ne10_radix4_butterfly_inverse_first_stage_fstride

        /* swap input/output buffer  */
        ldr             tmp0, [sp, #112]
        mov             p_fin, p_fout
        mov             p_fout, tmp0

        /* (stage_count-2): reduce the counter for the last stage  */
        sub             stage_count, stage_count, #2
        lsl             nstep, fstride, #3
        lsr             fstride, fstride, #2

        b               .L_ne10_butterfly_inverse_other_stages
        /* ---------------end of first stage: radix 4  */



        /* ---------------first stage: radix 8  */
.L_ne10_radix8_butterfly_inverse_first_stage:
        lsr             fstride1, fstride, #2
        mov             p_in1, p_fin
        mov             p_out1, p_fout
        lsl             fstep, fstride, #1

.L_ne10_radix8_butterfly_inverse_first_stage_fstride1:
        BUTTERFLY8X2_WITHOUT_TWIDDLES "TRUE"

        subs            fstride1, fstride1, #2
        bgt             .L_ne10_radix8_butterfly_inverse_first_stage_fstride1

        lsl             nstep, fstride, #2
        sub             stage_count, stage_count, #2
        lsr             fstride, fstride, #4
        lsl             mstride, mstride, #2
        add             p_twiddles, p_twiddles, #48 /* get the address of twiddles += 6 */

        /* swap input/output buffer  */
        ldr             tmp0, [sp, #112]
        mov             p_fin, p_fout
        mov             p_fout, tmp0

        /* if the last stage  */
        cmp            stage_count, #1
        beq            .L_ne10_butterfly_inverse_last_stages

        /* (stage_count-1): reduce the counter for the last stage  */
        sub            stage_count, stage_count, #1
        /*--------------- end of first stage: radix 8  */
        /* ---------------end of first stage---------------  */


        /* ---------------other stages  except last stage---------------  */
        /* loop of other stages  */
.L_ne10_butterfly_inverse_other_stages:
        lsl             mstep, mstride, #3
        mov             p_in1, p_fin
        vld2.32         {d_fin0_r, d_fin0_i}, [p_in1:64], nstep
        vld2.32         {d_fin1_r, d_fin1_i}, [p_in1:64], nstep
        vld2.32         {d_fin2_r, d_fin2_i}, [p_in1:64], nstep
        vld2.32         {d_fin3_r, d_fin3_i}, [p_in1:64], nstep

        /* loop of fstride  */
        mov             count_f, fstride
.L_ne10_butterfly_inverse_other_stages_fstride:
        mov             p_tw1, p_twiddles
        sub             tmp0, fstride, count_f
        mul             tmp0, tmp0, mstride
        add             p_out1, p_fout, tmp0, lsl #5
        vld2.32         {d_tw0_r, d_tw0_i}, [p_tw1:64], mstep
        vld2.32         {d_tw1_r, d_tw1_i}, [p_tw1:64], mstep
        vld2.32         {d_tw2_r, d_tw2_i}, [p_tw1:64]

        /* loop of mstride  */
        mov             count_m, mstride

.L_ne10_butterfly_inverse_other_stages_mstride:
        BUTTERFLY4X2_WITH_TWIDDLES "TRUE", "FALSE"

        subs            count_m, count_m, #2
        bgt             .L_ne10_butterfly_inverse_other_stages_mstride
        /* end of mstride loop */

        subs            count_f, count_f, #1
        bgt             .L_ne10_butterfly_inverse_other_stages_fstride

        add             p_twiddles, p_twiddles, mstride, lsl #4
        add             p_twiddles, p_twiddles, mstride, lsl #3 /* get the address of twiddles += mstride*3 */
        lsl             mstride, mstride, #2
        lsr             fstride, fstride, #2

        /* swap input/output buffer  */
        mov             tmp0, p_fout
        mov             p_fout, p_fin
        mov             p_fin, tmp0

        subs            stage_count, stage_count, #1
        bgt             .L_ne10_butterfly_inverse_other_stages
        /* ---------------end other stages  except last stage---------------  */


        /* ---------------last stage---------------  */
.L_ne10_butterfly_inverse_last_stages:
        mov             p_in1, p_fin
        mov             p_out1, p_out_ls
        mov             p_tw1, p_twiddles
        mov             mstep, nstep
        vld2.32         {d_fin0_r, d_fin0_i}, [p_in1:64], nstep
        vld2.32         {d_fin1_r, d_fin1_i}, [p_in1:64], nstep
        vld2.32         {d_fin2_r, d_fin2_i}, [p_in1:64], nstep
        vld2.32         {d_fin3_r, d_fin3_i}, [p_in1:64], nstep
        vld2.32         {d_tw0_r, d_tw0_i}, [p_tw1:64], mstep
        vld2.32         {d_tw1_r, d_tw1_i}, [p_tw1:64], mstep
        vld2.32         {d_tw2_r, d_tw2_i}, [p_tw1:64]

        /* loop of mstride  */
        mov             count_m, mstride
.L_ne10_butterfly_inverse_last_stages_mstride:
        BUTTERFLY4X2_WITH_TWIDDLES "TRUE", "TRUE"

        subs            count_m, count_m, #2
        bgt             .L_ne10_butterfly_inverse_last_stages_mstride
        /* end of mstride loop */
        /* ---------------end of last stage---------------  */

.L_ne10_butterfly_inverse_end:
        /*Return From Function*/
        vpop            {s0, s1}
        vpop            {q4-q7}
        pop             {r4-r12,pc}

        /* end of ne10_mixed_radix_fft_forward_float32_neon */




        /* end of the file */
        .end
