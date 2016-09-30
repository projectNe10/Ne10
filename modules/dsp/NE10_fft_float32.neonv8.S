/*
 *  Copyright 2014-15 ARM Limited and Contributors.
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
 *  DISCLAIMED. IN NO EVENT SHALL ARM LIMITED AND CONTRIBUTORS BE LIABLE FOR ANY
 *  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * NE10 Library : dsp/NE10_fft_float32.neonv8.S
 */

        .text

        /* ARM register aliases */
#define        p_fout             x0
#define        p_fin              x1
#define        p_factors          x2
#define        p_twiddles         x3
#define        p_buffer           x4
#define        stage_count        x5
#define        fstride            x6
#define        mstride            x7
#define        p_out_ls          x17

#define        radix              x8
#define        p_fin0             x9
#define        p_fin1             x10
#define        p_fin2             x11
#define        p_fin3             x12
#define        p_tmp              x13
#define        count              x14
#define        fstride1           x15
#define        fstep              x8

#define        nstep              x9
#define        mstep              x10
#define        count_f            x11
#define        count_m            x12
#define        p_tw1              x13
#define        p_in1              x14
#define        p_out1             x15
#define        tmp0               x16

        /* NEON register aliases for the first stage */
#define        q_in0_r            v0.4s
#define        q_in0_i            v1.4s
#define        q_in1_r            v2.4s
#define        q_in1_i            v3.4s
#define        q_in2_r            v4.4s
#define        q_in2_i            v5.4s
#define        q_in3_r            v6.4s
#define        q_in3_i            v7.4s
#define        q_s0_r             v8.4s
#define        q_s0_i             v9.4s
#define        q_s1_r             v10.4s
#define        q_s1_i             v11.4s
#define        q_s2_r             v12.4s
#define        q_s2_i             v13.4s
#define        q_s3_r             v14.4s
#define        q_s3_i             v15.4s
#define        q_out0_r           v16.4s
#define        q_out0_i           v17.4s
#define        q_out1_r           v18.4s
#define        q_out1_i           v19.4s
#define        q_out2_r           v20.4s
#define        q_out2_i           v21.4s
#define        q_out3_r           v22.4s
#define        q_out3_i           v23.4s
#define        q_out_r0246        v8.4s
#define        q_out_i0246        v9.4s
#define        q_out_r1357        v10.4s
#define        q_out_i1357        v11.4s
#define        q_out_r8ace        v12.4s
#define        q_out_i8ace        v13.4s
#define        q_out_r9bdf        v14.4s
#define        q_out_i9bdf        v15.4s

#define        q_in4_r            v8.4s
#define        q_in4_i            v9.4s
#define        q_in5_r            v10.4s
#define        q_in5_i            v11.4s
#define        q_in6_r            v12.4s
#define        q_in6_i            v13.4s
#define        q_in7_r            v14.4s
#define        q_in7_i            v15.4s
#define        q_sin0_r           v16.4s
#define        q_sin0_i           v17.4s
#define        q_sin1_r           v18.4s
#define        q_sin1_i           v19.4s
#define        q_sin2_r           v20.4s
#define        q_sin2_i           v21.4s
#define        q_sin3_r           v22.4s
#define        q_sin3_i           v23.4s
#define        q_sin4_r           v24.4s
#define        q_sin4_i           v25.4s
#define        q_sin5_r           v26.4s
#define        q_sin5_i           v27.4s
#define        q_sin6_r           v28.4s
#define        q_sin6_i           v29.4s
#define        q_sin7_r           v30.4s
#define        q_sin7_i           v31.4s
#define        d_tw_twn           v0.2s
#define        q_s5_r             v12.4s
#define        q_s5_i             v13.4s
#define        q_s7_r             v10.4s
#define        q_s7_i             v11.4s
#define        q_s8_r             v0.4s
#define        q_s8_i             v1.4s
#define        q_s9_r             v2.4s
#define        q_s9_i             v3.4s
#define        q_s10_r            v4.4s
#define        q_s10_i            v5.4s
#define        q_s11_r            v6.4s
#define        q_s11_i            v7.4s
#define        q_s12_r            v8.4s
#define        q_s12_i            v9.4s
#define        q_s13_r            v16.4s
#define        q_s13_i            v17.4s
#define        q_s14_r            v18.4s
#define        q_s14_i            v19.4s
#define        q_s15_r            v24.4s
#define        q_s15_i            v25.4s
#define        q_out_r0           v10.4s
#define        q_out_i0           v11.4s
#define        q_out_r1           v12.4s
#define        q_out_i1           v13.4s
#define        q_out_r2           v14.4s
#define        q_out_i2           v15.4s
#define        q_out_r3           v20.4s
#define        q_out_i3           v21.4s
#define        q_out_r4           v22.4s
#define        q_out_i4           v23.4s
#define        q_out_r5           v26.4s
#define        q_out_i5           v27.4s
#define        q_out_r6           v28.4s
#define        q_out_i6           v29.4s
#define        q_out_r7           v30.4s
#define        q_out_i7           v31.4s
#define        q_out_r028a        v16.4s
#define        q_out_i028a        v18.4s
#define        q_out_r139b        v0.4s
#define        q_out_i139b        v2.4s
#define        q_out_r46ce        v4.4s
#define        q_out_i46ce        v6.4s
#define        q_out_r57df        v8.4s
#define        q_out_i57df        v10.4s
#define        q_out_r028a_h      v17.4s
#define        q_out_i028a_h      v19.4s
#define        q_out_r139b_h      v1.4s
#define        q_out_i139b_h      v3.4s
#define        q_out_r46ce_h      v5.4s
#define        q_out_i46ce_h      v7.4s
#define        q_out_r57df_h      v9.4s
#define        q_out_i57df_h      v11.4s
#define        q_out0_r0246       v12.4s
#define        q_out0_i0246       v13.4s
#define        q_out1_r1357       v14.4s
#define        q_out1_i1357       v15.4s
#define        q_out2_r8ace       v20.4s
#define        q_out2_i8ace       v21.4s
#define        q_out3_r9bdf       v22.4s
#define        q_out3_i9bdf       v23.4s
#define        q_out0_r0246_h     v24.4s
#define        q_out0_i0246_h     v25.4s
#define        q_out1_r1357_h     v26.4s
#define        q_out1_i1357_h     v27.4s
#define        q_out2_r8ace_h     v28.4s
#define        q_out2_i8ace_h     v29.4s
#define        q_out3_r9bdf_h     v30.4s
#define        q_out3_i9bdf_h     v31.4s


        /* NEON register aliases for the mstride loop */
#define        q_tw0_r            v8.4s
#define        q_tw0_i            v9.4s
#define        q_tw1_r            v10.4s
#define        q_tw1_i            v11.4s
#define        q_tw2_r            v12.4s
#define        q_tw2_i            v13.4s
#define        q_scr1_r           v14.4s
#define        q_scr1_i           v15.4s
#define        q_scr2_r           v16.4s
#define        q_scr2_i           v17.4s
#define        q_scr3_r           v18.4s
#define        q_scr3_i           v19.4s
#define        q_scr4_r           v20.4s
#define        q_scr4_i           v21.4s
#define        q_scr5_r           v22.4s
#define        q_scr5_i           v23.4s
#define        q_scr6_r           v24.4s
#define        q_scr6_i           v25.4s
#define        q_scr7_r           v26.4s
#define        q_scr7_i           v27.4s
#define        q_fout0_r          v14.4s
#define        q_fout0_i          v15.4s
#define        q_fout1_r          v16.4s
#define        q_fout1_i          v17.4s
#define        q_fout2_r          v18.4s
#define        q_fout2_i          v19.4s
#define        q_fout3_r          v20.4s
#define        q_fout3_i          v21.4s
#define        d_one_by_nfft      v31.2s
#define        q_one_by_nfft      v31.4s

        /* radix 4 butterfly without twiddles */
        .macro BUTTERFLY4X4_WITHOUT_TWIDDLES inverse

        fadd            q_s0_r, q_in0_r, q_in2_r
        fadd            q_s0_i, q_in0_i, q_in2_i
        fsub            q_s1_r, q_in0_r, q_in2_r
        fsub            q_s1_i, q_in0_i, q_in2_i
        fadd            q_s2_r, q_in1_r, q_in3_r
        fadd            q_s2_i, q_in1_i, q_in3_i
        fsub            q_s3_r, q_in1_r, q_in3_r
        fsub            q_s3_i, q_in1_i, q_in3_i
        ld2             {q_in0_r, q_in0_i}, [p_fin0], #32
        ld2             {q_in2_r, q_in2_i}, [p_fin2], #32
        ld2             {q_in1_r, q_in1_i}, [p_fin1], #32
        ld2             {q_in3_r, q_in3_i}, [p_fin3], #32

        fsub            q_out2_r, q_s0_r, q_s2_r
        fsub            q_out2_i, q_s0_i, q_s2_i
        fadd            q_out0_r, q_s0_r, q_s2_r
        fadd            q_out0_i, q_s0_i, q_s2_i

        .ifeqs "\inverse", "TRUE"
        fsub            q_out1_r, q_s1_r, q_s3_i
        fadd            q_out1_i, q_s1_i, q_s3_r
        fadd            q_out3_r, q_s1_r, q_s3_i
        fsub            q_out3_i, q_s1_i, q_s3_r
        .else
        fadd            q_out1_r, q_s1_r, q_s3_i
        fsub            q_out1_i, q_s1_i, q_s3_r
        fsub            q_out3_r, q_s1_r, q_s3_i
        fadd            q_out3_i, q_s1_i, q_s3_r
        .endif

        zip1            q_out_r0246, q_out0_r, q_out2_r
        zip2            q_out_r8ace, q_out0_r, q_out2_r
        zip1            q_out_r1357, q_out1_r, q_out3_r
        zip2            q_out_r9bdf, q_out1_r, q_out3_r
        zip1            q_out_i0246, q_out0_i, q_out2_i
        zip2            q_out_i8ace, q_out0_i, q_out2_i
        zip1            q_out_i1357, q_out1_i, q_out3_i
        zip2            q_out_i9bdf, q_out1_i, q_out3_i
        st4             {q_out_r0246, q_out_i0246, q_out_r1357, q_out_i1357}, [p_tmp], #64
        st4             {q_out_r8ace, q_out_i8ace, q_out_r9bdf, q_out_i9bdf}, [p_tmp], #64
        .endm

        /* radix 4 butterfly with twiddles */
        .macro BUTTERFLY4X4_WITH_TWIDDLES inverse, last_stage

        sub             p_in1, p_in1, nstep, lsl #2
        add             p_in1, p_in1, #32
        sub             p_tw1, p_tw1, mstep, lsl #1
        add             p_tw1, p_tw1, #32
        fmul            q_scr1_r, q_in1_r, q_tw0_r
        fmul            q_scr1_i, q_in1_i, q_tw0_r
        fmul            q_scr2_r, q_in2_r, q_tw1_r
        fmul            q_scr2_i, q_in2_i, q_tw1_r
        fmul            q_scr3_r, q_in3_r, q_tw2_r
        fmul            q_scr3_i, q_in3_i, q_tw2_r

        .ifeqs "\inverse", "TRUE"
        fmla            q_scr1_r, q_in1_i, q_tw0_i
        fmls            q_scr1_i, q_in1_r, q_tw0_i
        fmla            q_scr2_r, q_in2_i, q_tw1_i
        fmls            q_scr2_i, q_in2_r, q_tw1_i
        fmla            q_scr3_r, q_in3_i, q_tw2_i
        fmls            q_scr3_i, q_in3_r, q_tw2_i
        .else
        fmls            q_scr1_r, q_in1_i, q_tw0_i
        fmla            q_scr1_i, q_in1_r, q_tw0_i
        fmls            q_scr2_r, q_in2_i, q_tw1_i
        fmla            q_scr2_i, q_in2_r, q_tw1_i
        fmls            q_scr3_r, q_in3_i, q_tw2_i
        fmla            q_scr3_i, q_in3_r, q_tw2_i
        .endif

        fadd            q_scr4_r, q_in0_r, q_scr2_r
        fadd            q_scr4_i, q_in0_i, q_scr2_i
        fsub            q_scr5_r, q_in0_r, q_scr2_r
        fsub            q_scr5_i, q_in0_i, q_scr2_i
        fadd            q_scr6_r, q_scr1_r, q_scr3_r
        fadd            q_scr6_i, q_scr1_i, q_scr3_i
        fsub            q_scr7_r, q_scr1_r, q_scr3_r
        fsub            q_scr7_i, q_scr1_i, q_scr3_i

        ld2             {q_in0_r, q_in0_i}, [p_in1], nstep
        ld2             {q_in1_r, q_in1_i}, [p_in1], nstep
        ld2             {q_in2_r, q_in2_i}, [p_in1], nstep
        ld2             {q_in3_r, q_in3_i}, [p_in1], nstep
        ld2             {q_tw0_r, q_tw0_i}, [p_tw1], mstep
        ld2             {q_tw1_r, q_tw1_i}, [p_tw1], mstep
        ld2             {q_tw2_r, q_tw2_i}, [p_tw1]

        .ifeqs "\inverse", "TRUE"
        .ifeqs "\last_stage", "TRUE"
        ld1             {d_one_by_nfft}, [sp]
        dup             q_one_by_nfft, d_one_by_nfft[0]
        .endif
        .endif

        fadd            q_fout0_r, q_scr4_r, q_scr6_r
        fadd            q_fout0_i, q_scr4_i, q_scr6_i
        fsub            q_fout2_r, q_scr4_r, q_scr6_r
        fsub            q_fout2_i, q_scr4_i, q_scr6_i

        .ifeqs "\inverse", "TRUE"
        fsub            q_fout1_r, q_scr5_r, q_scr7_i
        fadd            q_fout1_i, q_scr5_i, q_scr7_r
        fadd            q_fout3_r, q_scr5_r, q_scr7_i
        fsub            q_fout3_i, q_scr5_i, q_scr7_r
        .else
        fadd            q_fout1_r, q_scr5_r, q_scr7_i
        fsub            q_fout1_i, q_scr5_i, q_scr7_r
        fsub            q_fout3_r, q_scr5_r, q_scr7_i
        fadd            q_fout3_i, q_scr5_i, q_scr7_r
        .endif

        .ifeqs "\inverse", "TRUE"
        .ifeqs "\last_stage", "TRUE"
        fmul            q_fout0_r, q_fout0_r, q_one_by_nfft
        fmul            q_fout0_i, q_fout0_i, q_one_by_nfft
        fmul            q_fout2_r, q_fout2_r, q_one_by_nfft
        fmul            q_fout2_i, q_fout2_i, q_one_by_nfft
        fmul            q_fout1_r, q_fout1_r, q_one_by_nfft
        fmul            q_fout1_i, q_fout1_i, q_one_by_nfft
        fmul            q_fout3_r, q_fout3_r, q_one_by_nfft
        fmul            q_fout3_i, q_fout3_i, q_one_by_nfft
        .endif
        .endif

        st2             {q_fout0_r, q_fout0_i}, [p_out1], mstep
        st2             {q_fout1_r, q_fout1_i}, [p_out1], mstep
        st2             {q_fout2_r, q_fout2_i}, [p_out1], mstep
        st2             {q_fout3_r, q_fout3_i}, [p_out1], mstep
        sub             p_out1, p_out1, mstep, lsl #2
        add             p_out1, p_out1, #32

        .endm

        /* radix 8 butterfly without twiddles */
        .macro BUTTERFLY8X4_WITHOUT_TWIDDLES inverse
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

        adr             tmp0, .L_TW_81
        ld2             {q_in0_r, q_in0_i}, [p_in1], fstep
        ld2             {q_in2_r, q_in2_i}, [p_in1], fstep
        ld2             {q_in4_r, q_in4_i}, [p_in1], fstep
        ld2             {q_in6_r, q_in6_i}, [p_in1], fstep
        ld2             {q_in1_r, q_in1_i}, [p_in1], fstep
        ld2             {q_in3_r, q_in3_i}, [p_in1], fstep
        ld2             {q_in5_r, q_in5_i}, [p_in1], fstep
        ld2             {q_in7_r, q_in7_i}, [p_in1], fstep

        /* radix 4 butterfly without twiddles */
        fadd            q_sin0_r, q_in0_r, q_in1_r
        fadd            q_sin0_i, q_in0_i, q_in1_i
        fsub            q_sin1_r, q_in0_r, q_in1_r
        fsub            q_sin1_i, q_in0_i, q_in1_i
        ld1             {d_tw_twn}, [tmp0]
        fadd            q_sin2_r, q_in2_r, q_in3_r
        fadd            q_sin2_i, q_in2_i, q_in3_i
        fsub            q_sin3_r, q_in2_r, q_in3_r
        fsub            q_sin3_i, q_in2_i, q_in3_i
        fadd            q_sin4_r, q_in4_r, q_in5_r
        fadd            q_sin4_i, q_in4_i, q_in5_i
        fsub            q_sin5_r, q_in4_r, q_in5_r
        fsub            q_sin5_i, q_in4_i, q_in5_i
        fadd            q_sin6_r, q_in6_r, q_in7_r
        fadd            q_sin6_i, q_in6_i, q_in7_i
        fsub            q_sin7_r, q_in6_r, q_in7_r
        fsub            q_sin7_i, q_in6_i, q_in7_i

        .ifeqs "\inverse", "TRUE"
        fneg            q_s5_r, q_sin5_i
        shl             q_s5_i, q_sin5_r, #0
        fsub            q_s3_r, q_sin3_r, q_sin3_i
        fadd            q_s3_i, q_sin3_i, q_sin3_r
        fadd            q_s7_r, q_sin7_r, q_sin7_i
        fsub            q_s7_i, q_sin7_i, q_sin7_r
        .else
        fneg            q_s5_i, q_sin5_r
        shl             q_s5_r, q_sin5_i, 0
        fadd            q_s3_r, q_sin3_r, q_sin3_i
        fsub            q_s3_i, q_sin3_i, q_sin3_r
        fsub            q_s7_r, q_sin7_r, q_sin7_i
        fadd            q_s7_i, q_sin7_i, q_sin7_r
        .endif

        fmul            q_s3_r, q_s3_r, d_tw_twn[0]
        fmul            q_s3_i, q_s3_i, d_tw_twn[0]
        fmul            q_s7_r, q_s7_r, d_tw_twn[1]
        fmul            q_s7_i, q_s7_i, d_tw_twn[1]

        /* radix 2 butterfly */
        fadd            q_s8_r, q_sin0_r, q_sin4_r
        fadd            q_s8_i, q_sin0_i, q_sin4_i
        fadd            q_s9_r, q_sin1_r, q_s5_r
        fadd            q_s9_i, q_sin1_i, q_s5_i
        fsub            q_s10_r, q_sin0_r, q_sin4_r
        fsub            q_s10_i, q_sin0_i, q_sin4_i
        fsub            q_s11_r, q_sin1_r, q_s5_r
        fsub            q_s11_i, q_sin1_i, q_s5_i

        /* radix 2 butterfly */
        fadd            q_s12_r, q_sin2_r, q_sin6_r
        fadd            q_s12_i, q_sin2_i, q_sin6_i
        fadd            q_s13_r, q_s3_r, q_s7_r
        fadd            q_s13_i, q_s3_i, q_s7_i
        fsub            q_s14_r, q_sin2_r, q_sin6_r
        fsub            q_s14_i, q_sin2_i, q_sin6_i
        fsub            q_s15_r, q_s3_r, q_s7_r
        fsub            q_s15_i, q_s3_i, q_s7_i

        fsub            q_out_r4, q_s8_r, q_s12_r
        fsub            q_out_i4, q_s8_i, q_s12_i
        fsub            q_out_r5, q_s9_r, q_s13_r
        fsub            q_out_i5, q_s9_i, q_s13_i
        fadd            q_out_r0, q_s8_r, q_s12_r
        fadd            q_out_i0, q_s8_i, q_s12_i
        fadd            q_out_r1, q_s9_r, q_s13_r
        fadd            q_out_i1, q_s9_i, q_s13_i

        .ifeqs "\inverse", "TRUE"
        fsub            q_out_r2, q_s10_r, q_s14_i
        fadd            q_out_i2, q_s10_i, q_s14_r
        fsub            q_out_r3, q_s11_r, q_s15_i
        fadd            q_out_i3, q_s11_i, q_s15_r
        fadd            q_out_r6, q_s10_r, q_s14_i
        fsub            q_out_i6, q_s10_i, q_s14_r
        fadd            q_out_r7, q_s11_r, q_s15_i
        fsub            q_out_i7, q_s11_i, q_s15_r
        .else
        fadd            q_out_r2, q_s10_r, q_s14_i
        fsub            q_out_i2, q_s10_i, q_s14_r
        fadd            q_out_r3, q_s11_r, q_s15_i
        fsub            q_out_i3, q_s11_i, q_s15_r
        fsub            q_out_r6, q_s10_r, q_s14_i
        fadd            q_out_i6, q_s10_i, q_s14_r
        fsub            q_out_r7, q_s11_r, q_s15_i
        fadd            q_out_i7, q_s11_i, q_s15_r
        .endif

        zip1            q_out_r028a, q_out_r0, q_out_r2
        zip2            q_out_r028a_h, q_out_r0, q_out_r2
        zip1            q_out_i028a, q_out_i0, q_out_i2
        zip2            q_out_i028a_h, q_out_i0, q_out_i2

        zip1            q_out_r139b, q_out_r1, q_out_r3
        zip2            q_out_r139b_h, q_out_r1, q_out_r3
        zip1            q_out_i139b, q_out_i1, q_out_i3
        zip2            q_out_i139b_h, q_out_i1, q_out_i3

        zip1            q_out_r46ce, q_out_r4, q_out_r6
        zip2            q_out_r46ce_h, q_out_r4, q_out_r6
        zip1            q_out_i46ce, q_out_i4, q_out_i6
        zip2            q_out_i46ce_h, q_out_i4, q_out_i6

        zip1            q_out_r57df, q_out_r5, q_out_r7
        zip2            q_out_r57df_h, q_out_r5, q_out_r7
        zip1            q_out_i57df, q_out_i5, q_out_i7
        zip2            q_out_i57df_h, q_out_i5, q_out_i7

        zip1            v12.2d, v16.2d, v4.2d
        zip2            v20.2d, v16.2d, v4.2d
        zip1            v24.2d, v17.2d, v5.2d
        zip2            v28.2d, v17.2d, v5.2d
        zip1            v13.2d, v18.2d, v6.2d
        zip2            v21.2d, v18.2d, v6.2d
        zip1            v25.2d, v19.2d, v7.2d
        zip2            v29.2d, v19.2d, v7.2d
        zip1            v14.2d, v0.2d, v8.2d
        zip2            v22.2d, v0.2d, v8.2d
        zip1            v26.2d, v1.2d, v9.2d
        zip2            v30.2d, v1.2d, v9.2d
        zip1            v15.2d, v2.2d, v10.2d
        zip2            v23.2d, v2.2d, v10.2d
        zip1            v27.2d, v3.2d, v11.2d
        zip2            v31.2d, v3.2d, v11.2d

        st4             {q_out0_r0246, q_out0_i0246, q_out1_r1357, q_out1_i1357}, [p_tmp], #64
        st4             {q_out2_r8ace, q_out2_i8ace, q_out3_r9bdf, q_out3_i9bdf}, [p_tmp], #64
        st4             {q_out0_r0246_h, q_out0_i0246_h, q_out1_r1357_h, q_out1_i1357_h}, [p_tmp], #64
        st4             {q_out2_r8ace_h, q_out2_i8ace_h, q_out3_r9bdf_h, q_out3_i9bdf_h}, [p_tmp], #64

        sub             p_in1, p_in1, fstep, lsl #3
        add             p_in1, p_in1, #32

        .endm

        .align 4
.L_TW_81:
.float 0.70710678
.float -0.70710678


        /**
         * @details This function implements a radix-4/8 forwards FFT.
         *
         * @param[in,out] *Fout        points to input/output pointers
         * @param[in]     *factors     factors pointer:
                                        * 0: stage number
                                        * 1: stride for the first stage
                                        * others: factor out powers of 4, powers of 2
         * @param[in]     *twiddles     twiddles coeffs of FFT
         */

        .align 4
        .global ne10_mixed_radix_fft_forward_float32_neon
        .type	ne10_mixed_radix_fft_forward_float32_neon, %function

ne10_mixed_radix_fft_forward_float32_neon:

        sub            sp, sp, #16
        stp            x29, x30, [sp]

        sub            sp, sp, #64
        st1            {v8.4s, v9.4s, v10.4s, v11.4s}, [sp]
        sub            sp, sp, #64
        st1            {v12.4s, v13.4s, v14.4s, v15.4s}, [sp]
        /* get factors[0]---stage_count factors[1]---fstride*/
        ldr            stage_count, [p_factors]
        lsr            fstride, stage_count, 32
        lsl            stage_count, stage_count, 32
        lsr            stage_count, stage_count, 32
        add            p_factors, p_factors, stage_count, lsl #3 /* get the address of factors[2*stage_count] */

        /* get factors[2*stage_count]--- the first radix */
        /* get factors[2*stage_count-1]--- mstride */
        sub            p_factors, p_factors, #4 /* get the address of factors[2*stage_count-1] */
        ldr            mstride, [p_factors]
        lsr            radix, mstride, 32
        lsl            mstride, mstride, 32
        lsr            mstride, mstride, 32

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
        ld2             {q_in0_r, q_in0_i}, [p_fin0], #32
        ld2             {q_in2_r, q_in2_i}, [p_fin2], #32
        ld2             {q_in1_r, q_in1_i}, [p_fin1], #32
        ld2             {q_in3_r, q_in3_i}, [p_fin3], #32

.L_ne10_radix4_butterfly_first_stage_fstride:
        BUTTERFLY4X4_WITHOUT_TWIDDLES "FALSE"

        subs            count, count, #4
        bgt             .L_ne10_radix4_butterfly_first_stage_fstride

        /* swap input/output buffer  */
        mov             p_fin, p_fout
        mov             p_fout, p_buffer

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
        mov             p_tmp, p_fout
        lsl             fstep, fstride, #1

.L_ne10_radix8_butterfly_first_stage_fstride1:
        BUTTERFLY8X4_WITHOUT_TWIDDLES "FALSE"

        subs            fstride1, fstride1, #4
        bgt             .L_ne10_radix8_butterfly_first_stage_fstride1

        lsl             nstep, fstride, #2
        sub             stage_count, stage_count, #2
        lsr             fstride, fstride, #4
        lsl             mstride, mstride, #2
        add             p_twiddles, p_twiddles, #48 /* get the address of twiddles += 6 */

        /* swap input/output buffer  */
        mov             p_fin, p_fout
        mov             p_fout, p_buffer

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
        ld2             {q_in0_r, q_in0_i}, [p_in1], nstep
        ld2             {q_in1_r, q_in1_i}, [p_in1], nstep
        ld2             {q_in2_r, q_in2_i}, [p_in1], nstep
        ld2             {q_in3_r, q_in3_i}, [p_in1], nstep

        /* loop of fstride  */
        mov             count_f, fstride
.L_ne10_butterfly_other_stages_fstride:
        mov             p_tw1, p_twiddles
        sub             tmp0, fstride, count_f
        mul             tmp0, tmp0, mstride
        add             p_out1, p_fout, tmp0, lsl #5
        ld2             {q_tw0_r, q_tw0_i}, [p_tw1], mstep
        ld2             {q_tw1_r, q_tw1_i}, [p_tw1], mstep
        ld2             {q_tw2_r, q_tw2_i}, [p_tw1]

        /* loop of mstride  */
        mov             count_m, mstride

.L_ne10_butterfly_other_stages_mstride:
        BUTTERFLY4X4_WITH_TWIDDLES "FALSE", "FALSE"

        subs            count_m, count_m, #4
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
        ld2             {q_in0_r, q_in0_i}, [p_in1], nstep
        ld2             {q_in1_r, q_in1_i}, [p_in1], nstep
        ld2             {q_in2_r, q_in2_i}, [p_in1], nstep
        ld2             {q_in3_r, q_in3_i}, [p_in1], nstep
        ld2             {q_tw0_r, q_tw0_i}, [p_tw1], mstep
        ld2             {q_tw1_r, q_tw1_i}, [p_tw1], mstep
        ld2             {q_tw2_r, q_tw2_i}, [p_tw1]

        /* loop of mstride  */
        mov             count_m, mstride
.L_ne10_butterfly_last_stages_mstride:
        BUTTERFLY4X4_WITH_TWIDDLES "FALSE", "TRUE"

        subs            count_m, count_m, #4
        bgt             .L_ne10_butterfly_last_stages_mstride
        /* end of mstride loop */
        /* ---------------end of last stage---------------  */

.L_ne10_butterfly_end:
        /*Return From Function*/
        ld1            {v12.4s, v13.4s, v14.4s, v15.4s}, [sp]
        add            sp, sp, #64
        ld1            {v8.4s, v9.4s, v10.4s, v11.4s}, [sp]
        add            sp, sp, #64

        ldp            x29, x30, [sp]
        add            sp, sp, #16

        ret

        /* end of ne10_mixed_radix_fft_forward_float32_neon */

        /**
         * @details This function implements a radix-4/8 backwards FFT.
         *
         * @param[in,out] *Fout        points to input/output pointers
         * @param[in]     *factors     factors pointer:
                                        * 0: stage number
                                        * 1: stride for the first stage
                                        * others: factor out powers of 4, powers of 2
         * @param[in]     *twiddles     twiddles coeffs of FFT
         */

        .align 4
        .global ne10_mixed_radix_fft_backward_float32_neon
        .type	ne10_mixed_radix_fft_backward_float32_neon, %function

ne10_mixed_radix_fft_backward_float32_neon:
        sub            sp, sp, #16
        stp            x29, x30, [sp]

        sub            sp, sp, #64
        st1            {v8.4s, v9.4s, v10.4s, v11.4s}, [sp]
        sub            sp, sp, #64
        st1            {v12.4s, v13.4s, v14.4s, v15.4s}, [sp]

        /* get factors[0]---stage_count factors[1]---fstride*/
        ldr            stage_count, [p_factors]
        lsr            fstride, stage_count, 32
        lsl            stage_count, stage_count, 32
        lsr            stage_count, stage_count, 32
        add            p_factors, p_factors, stage_count, lsl #3 /* get the address of factors[2*stage_count] */

        /* get factors[2*stage_count]--- the first radix */
        /* get factors[2*stage_count-1]--- mstride */
        sub            p_factors, p_factors, #4 /* get the address of factors[2*stage_count-1] */
        ldr            mstride, [p_factors]
        lsr            radix, mstride, 32
        lsl            mstride, mstride, 32
        lsr            mstride, mstride, 32

        /* calculate 1/nfft for the last stage  */
        mul             tmp0, radix, fstride
        fmov            s0, #0.5
        scvtf           s1, tmp0, #1
        fdiv            s0, s0, s1
        sub             sp, sp, #16
        stp             d0, d1, [sp]


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
        ld2             {q_in0_r, q_in0_i}, [p_fin0], #32
        ld2             {q_in2_r, q_in2_i}, [p_fin2], #32
        ld2             {q_in1_r, q_in1_i}, [p_fin1], #32
        ld2             {q_in3_r, q_in3_i}, [p_fin3], #32

.L_ne10_radix4_butterfly_inverse_first_stage_fstride:
        BUTTERFLY4X4_WITHOUT_TWIDDLES "TRUE"

        subs            count, count, #4
        bgt             .L_ne10_radix4_butterfly_inverse_first_stage_fstride

        /* swap input/output buffer  */
        mov             p_fin, p_fout
        mov             p_fout, p_buffer

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
        mov             p_tmp, p_fout
        lsl             fstep, fstride, #1

.L_ne10_radix8_butterfly_inverse_first_stage_fstride1:
        BUTTERFLY8X4_WITHOUT_TWIDDLES "TRUE"

        subs            fstride1, fstride1, #4
        bgt             .L_ne10_radix8_butterfly_inverse_first_stage_fstride1

        lsl             nstep, fstride, #2
        sub             stage_count, stage_count, #2
        lsr             fstride, fstride, #4
        lsl             mstride, mstride, #2
        add             p_twiddles, p_twiddles, #48 /* get the address of twiddles += 6 */

        /* swap input/output buffer  */
        mov             p_fin, p_fout
        mov             p_fout, p_buffer

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
        ld2             {q_in0_r, q_in0_i}, [p_in1], nstep
        ld2             {q_in1_r, q_in1_i}, [p_in1], nstep
        ld2             {q_in2_r, q_in2_i}, [p_in1], nstep
        ld2             {q_in3_r, q_in3_i}, [p_in1], nstep

        /* loop of fstride  */
        mov             count_f, fstride
.L_ne10_butterfly_inverse_other_stages_fstride:
        mov             p_tw1, p_twiddles
        sub             tmp0, fstride, count_f
        mul             tmp0, tmp0, mstride
        add             p_out1, p_fout, tmp0, lsl #5
        ld2             {q_tw0_r, q_tw0_i}, [p_tw1], mstep
        ld2             {q_tw1_r, q_tw1_i}, [p_tw1], mstep
        ld2             {q_tw2_r, q_tw2_i}, [p_tw1]

        /* loop of mstride  */
        mov             count_m, mstride

.L_ne10_butterfly_inverse_other_stages_mstride:
        BUTTERFLY4X4_WITH_TWIDDLES "TRUE", "FALSE"

        subs            count_m, count_m, #4
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
        ld2             {q_in0_r, q_in0_i}, [p_in1], nstep
        ld2             {q_in1_r, q_in1_i}, [p_in1], nstep
        ld2             {q_in2_r, q_in2_i}, [p_in1], nstep
        ld2             {q_in3_r, q_in3_i}, [p_in1], nstep
        ld2             {q_tw0_r, q_tw0_i}, [p_tw1], mstep
        ld2             {q_tw1_r, q_tw1_i}, [p_tw1], mstep
        ld2             {q_tw2_r, q_tw2_i}, [p_tw1]

        /* loop of mstride  */
        mov             count_m, mstride
.L_ne10_butterfly_inverse_last_stages_mstride:
        BUTTERFLY4X4_WITH_TWIDDLES "TRUE", "TRUE"

        subs            count_m, count_m, #4
        bgt             .L_ne10_butterfly_inverse_last_stages_mstride
        /* end of mstride loop */
        /* ---------------end of last stage---------------  */

.L_ne10_butterfly_inverse_end:
        /*Return From Function*/
        ldp             d0, d1, [sp]
        add             sp, sp, #16

        ld1            {v12.4s, v13.4s, v14.4s, v15.4s}, [sp]
        add            sp, sp, #64
        ld1            {v8.4s, v9.4s, v10.4s, v11.4s}, [sp]
        add            sp, sp, #64

        ldp            x29, x30, [sp]
        add            sp, sp, #16

        ret
        /* end of ne10_mixed_radix_fft_backward_float32_neon */




        /* end of the file */
        .end
