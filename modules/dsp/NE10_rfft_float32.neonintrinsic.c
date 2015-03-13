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

/* license of Kiss FFT */
/*
Copyright (c) 2003-2010, Mark Borgerding

All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
    * Neither the author nor the names of any contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/*
 * NE10 Library : dsp/NE10_rfft_float32.neonintrinsic.c
 */

#include <arm_neon.h>

#include "NE10_types.h"
#include "NE10_macros.h"
#include "NE10_fft.h"
#include "NE10_dsp.h"
#include "NE10_fft.neonintrinsic.h"

NE10_INLINE void ne10_radix8x4_r2c_neon (ne10_fft_cpx_float32_t *Fout,
                                  const ne10_fft_cpx_float32_t *Fin,
                                  const ne10_int32_t fstride,
                                  const ne10_int32_t mstride,
                                  const ne10_int32_t nfft)
{
    ne10_int32_t f_count;

    NE10_DECLARE_8(float32x4_t,q_in);
    NE10_DECLARE_8(float32x4_t,q_out);

    const float32x4_t *Fin_neon  = (float32x4_t*) Fin;  // 8 x fstride
          float32x4_t *Fout_neon = (float32x4_t*) Fout; // fstride x 8

    for (f_count = fstride; f_count > 0; f_count --)
    {
        // from Fin_neon load 8 float32x4_t into q_in0 ~ q_in7, by step = fstride
        NE10_RADIX8x4_R2C_NEON_LOAD(Fin_neon,q_in,fstride);

        // print q_in0 ~ q_in7
        // NE10_PRINT_Qx8_VECTOR(q_in);

        // do r2c fft, size = 8
        NE10_RADIX8x4_R2C_NEON_KERNEL(q_out,q_in);

        // print q_out0 ~ q_out7
        // NE10_PRINT_Qx8_VECTOR(q_out);

        // store q_out0 ~ q_out7 to Fout_neon, by step = 1
        NE10_RADIX8x4_R2C_NEON_STORE(Fout_neon,q_out,1);

        Fin_neon = Fin_neon - fstride * 8 + 1;
        Fout_neon += 8; // next column
    }
}

NE10_INLINE void ne10_radix8x4_c2r_neon (ne10_fft_cpx_float32_t *Fout,
                                  const ne10_fft_cpx_float32_t *Fin,
                                  const ne10_int32_t fstride,
                                  const ne10_int32_t mstride,
                                  const ne10_int32_t nfft)
{
    ne10_int32_t f_count;

    NE10_DECLARE_8(float32x4_t,q_in);
    NE10_DECLARE_8(float32x4_t,q_out);

    const ne10_float32_t one_by_N = 0.25 / nfft;
    const float32x4_t one_by_N_neon = vdupq_n_f32(one_by_N);

    const float32x4_t *Fin_neon  = (float32x4_t*) Fin;
          float32x4_t *Fout_neon = (float32x4_t*) Fout;

    for (f_count = fstride; f_count > 0; f_count --)
    {
        // from Fin_neon load 8 float32x4_t into q_in0 ~ q_in7, by step = 1
        NE10_RADIX8x4_R2C_NEON_LOAD(Fin_neon,q_in,1);

        // NE10_PRINT_Qx8_VECTOR(q_in);

        NE10_RADIX8x4_C2R_NEON_KERNEL(q_out,q_in);

        // NE10_PRINT_Qx8_VECTOR(q_out);

#ifdef NE10_DSP_RFFT_SCALING
        q_out0 = vmulq_f32(q_out0,one_by_N_neon);
        q_out1 = vmulq_f32(q_out1,one_by_N_neon);
        q_out2 = vmulq_f32(q_out2,one_by_N_neon);
        q_out3 = vmulq_f32(q_out3,one_by_N_neon);
        q_out4 = vmulq_f32(q_out4,one_by_N_neon);
        q_out5 = vmulq_f32(q_out5,one_by_N_neon);
        q_out6 = vmulq_f32(q_out6,one_by_N_neon);
        q_out7 = vmulq_f32(q_out7,one_by_N_neon);
#endif

        // store
        NE10_RADIX8x4_R2C_NEON_STORE(Fout_neon,q_out,fstride);

        Fout_neon ++;
    }
}

NE10_INLINE void ne10_radix4x4_r2c_neon (ne10_fft_cpx_float32_t *Fout,
                                  const ne10_fft_cpx_float32_t *Fin,
                                  const ne10_int32_t fstride,
                                  const ne10_int32_t mstride,
                                  const ne10_int32_t nfft)
{
    ne10_int32_t f_count;

    const float32x4_t *Fin_neon  = (float32x4_t*) Fin;
          float32x4_t *Fout_neon = (float32x4_t*) Fout;

    for (f_count = 0; f_count < fstride; f_count ++)
    {
        NE10_DECLARE_4(float32x4_t,q_in);
        NE10_DECLARE_4(float32x4_t,q_out);

        // load
        NE10_RADIX4x4_R2C_NEON_LOAD(Fin_neon,q_in,fstride);

        NE10_RADIX4x4_R2C_NEON_KERNEL(q_out,q_in)

        // store
        NE10_RADIX4x4_R2C_NEON_STORE(Fout_neon,q_out,1);

        Fin_neon = Fin_neon - 4*fstride + 1;
        Fout_neon += 4;
    }
}

NE10_INLINE void ne10_radix4x4_c2r_neon (ne10_fft_cpx_float32_t *Fout,
                                  const ne10_fft_cpx_float32_t *Fin,
                                  const ne10_int32_t fstride,
                                  const ne10_int32_t mstride,
                                  const ne10_int32_t nfft)
{
    ne10_int32_t f_count;

    const float32x4_t *Fin_neon  = (float32x4_t*) Fin;
          float32x4_t *Fout_neon = (float32x4_t*) Fout;

    const ne10_float32_t one_by_N = 0.25 / nfft;
    const float32x4_t one_by_N_neon = vdupq_n_f32(one_by_N);

    for (f_count = 0; f_count < fstride; f_count ++)
    {
        NE10_DECLARE_4(float32x4_t,q_in);
        NE10_DECLARE_4(float32x4_t,q_out);

        // load
        NE10_RADIX4x4_R2C_NEON_LOAD(Fin_neon,q_in,1);

        // NE10_PRINT_Qx4_VECTOR(q_in);

        NE10_RADIX4x4_C2R_NEON_KERNEL(q_out,q_in)

        // NE10_PRINT_Qx4_VECTOR(q_out);

#ifdef NE10_DSP_RFFT_SCALING
        q_out0 = vmulq_f32(q_out0,one_by_N_neon);
        q_out1 = vmulq_f32(q_out1,one_by_N_neon);
        q_out2 = vmulq_f32(q_out2,one_by_N_neon);
        q_out3 = vmulq_f32(q_out3,one_by_N_neon);
#endif

        // store
        NE10_RADIX4x4_R2C_NEON_STORE(Fout_neon,q_out,fstride);

        Fout_neon ++;
    }
}

NE10_INLINE void ne10_radix4x4_r2c_with_twiddles_first_butterfly_neon (float32x4_t *Fout_neon,
                                                            const float32x4_t *Fin_neon,
                                                            const ne10_int32_t out_step,
                                                            const ne10_int32_t in_step,
                                                            const ne10_fft_cpx_float32_t *twiddles)
{
    NE10_DECLARE_4(float32x4_t,q_in);
    NE10_DECLARE_4(float32x4_t,q_out);

    // load
    NE10_RADIX4x4_R2C_NEON_LOAD(Fin_neon,q_in,in_step);

    NE10_RADIX4x4_R2C_NEON_KERNEL(q_out,q_in);

    // store
    vst1q_f32( (ne10_float32_t*) (Fout_neon                          ), q_out0);
    vst1q_f32( (ne10_float32_t*) (Fout_neon +     (out_step << 1) - 1), q_out1);
    vst1q_f32( (ne10_float32_t*) (Fout_neon +     (out_step << 1)    ), q_out2);
    vst1q_f32( (ne10_float32_t*) (Fout_neon + 2 * (out_step << 1) - 1), q_out3);
}

NE10_INLINE void ne10_radix4x4_c2r_with_twiddles_first_butterfly_neon (float32x4_t *Fout_neon,
                                                            const float32x4_t *Fin_neon,
                                                            const ne10_int32_t out_step,
                                                            const ne10_int32_t in_step,
                                                            const ne10_fft_cpx_float32_t *twiddles)
{
    NE10_DECLARE_4(float32x4_t,q_in);
    NE10_DECLARE_4(float32x4_t,q_out);

    // load
    q_in0 = vld1q_f32( (ne10_float32_t*) (Fin_neon                          ) );
    q_in1 = vld1q_f32( (ne10_float32_t*) (Fin_neon +     (out_step << 1) - 1) );
    q_in2 = vld1q_f32( (ne10_float32_t*) (Fin_neon +     (out_step << 1)    ) );
    q_in3 = vld1q_f32( (ne10_float32_t*) (Fin_neon + 2 * (out_step << 1) - 1) );

    // NE10_PRINT_Qx4_VECTOR(q_in);

    NE10_RADIX4x4_C2R_NEON_KERNEL(q_out,q_in);

    // NE10_PRINT_Qx4_VECTOR(q_out);

    // store
    NE10_RADIX4x4_R2C_NEON_STORE(Fout_neon,q_out,in_step);
}

NE10_INLINE void ne10_radix4x4_r2c_with_twiddles_other_butterfly_neon (float32x4_t *Fout_neon,
                                                                const float32x4_t *Fin_neon,
                                                                const ne10_int32_t out_step,
                                                                const ne10_int32_t in_step,
                                                                const ne10_fft_cpx_float32_t *twiddles)
{
    ne10_int32_t m_count;
    ne10_int32_t loop_count = (out_step>>1) -1;
    float32x4_t *Fout_b = Fout_neon + (((out_step<<1)-1)<<1) - 2; // reversed

    NE10_DECLARE_3(float32x4x2_t,q2_tw);
    NE10_DECLARE_4(float32x4x2_t,q2_in);
    NE10_DECLARE_4(float32x4x2_t,q2_out);

    for (m_count = loop_count; m_count > 0; m_count -- )
    {
#ifndef NE10_INLINE_ASM_OPT
        // load
        q2_in0.val[0] = vld1q_f32( (ne10_float32_t*) (Fin_neon + 0*in_step    ) );
        q2_in0.val[1] = vld1q_f32( (ne10_float32_t*) (Fin_neon + 0*in_step + 1) );

        q2_in1.val[0] = vld1q_f32( (ne10_float32_t*) (Fin_neon + 1*in_step    ) );
        q2_in1.val[1] = vld1q_f32( (ne10_float32_t*) (Fin_neon + 1*in_step + 1) );

        q2_in2.val[0] = vld1q_f32( (ne10_float32_t*) (Fin_neon + 2*in_step    ) );
        q2_in2.val[1] = vld1q_f32( (ne10_float32_t*) (Fin_neon + 2*in_step + 1) );

        q2_in3.val[0] = vld1q_f32( (ne10_float32_t*) (Fin_neon + 3*in_step    ) );
        q2_in3.val[1] = vld1q_f32( (ne10_float32_t*) (Fin_neon + 3*in_step + 1) );

        q2_tw0.val[0] = vdupq_n_f32(twiddles[0].r);
        q2_tw0.val[1] = vdupq_n_f32(twiddles[0].i);

        q2_tw1.val[0] = vdupq_n_f32(twiddles[out_step].r);
        q2_tw1.val[1] = vdupq_n_f32(twiddles[out_step].i);

        q2_tw2.val[0] = vdupq_n_f32(twiddles[out_step*2].r);
        q2_tw2.val[1] = vdupq_n_f32(twiddles[out_step*2].i);

        // R2C TW KERNEL
        NE10_RADIX4x4_R2C_TW_MUL_NEON (q2_out, q2_in, q2_tw);
#else // NE10_INLINE_ASM_OPT
#ifndef __aarch64__
#error Currently, inline assembly optimizations are only available on AArch64.
#else // __aarch64__
        const ne10_float32_t *ptr_inr = ((const ne10_float32_t *) Fin_neon);
        const ne10_float32_t *ptr_ini = ((const ne10_float32_t *) Fin_neon) + 4;
        const ne10_float32_t *ptr_tw  = (const ne10_float32_t *) twiddles;

        asm volatile (
            "ld1 {%[q2_out0r].4s}, [%[ptr_inr]], %[offset_in] \n\t"
            "ld1 {%[q2_out0i].4s}, [%[ptr_ini]] \n\t"
            "ld1 {v10.4s, v11.4s}, [%[ptr_inr]], %[offset_in] \n\t"
            "ld1 {v12.4s, v13.4s}, [%[ptr_inr]], %[offset_in] \n\t"
            "ld1 {v14.4s, v15.4s}, [%[ptr_inr]] \n\t"
            "ld1 {v0.1d},  [%[ptw0]], %[offset_out] \n\t"
            "ld1 {v1.1d},  [%[ptw1]], %[offset_out] \n\t"
            "ld1 {v2.1d},  [%[ptw2]] \n\t"

            "fmul %[q2_out1r].4s, v10.4s, v0.4s[0] \n\t" // RR
            "fmul %[q2_out1i].4s, v10.4s, v0.4s[1] \n\t" // RI
            "fmls %[q2_out1r].4s, v11.4s, v0.4s[1] \n\t" // RR - II
            "fmla %[q2_out1i].4s, v11.4s, v0.4s[0] \n\t" // RI + IR

            "fmul %[q2_out2r].4s, v12.4s, v1.4s[0] \n\t" // RR
            "fmul %[q2_out2i].4s, v12.4s, v1.4s[1] \n\t" // RI
            "fmls %[q2_out2r].4s, v13.4s, v1.4s[1] \n\t" // RR - II
            "fmla %[q2_out2i].4s, v13.4s, v1.4s[0] \n\t" // RI + IR

            "fmul %[q2_out3r].4s, v14.4s, v2.4s[0] \n\t" // RR
            "fmul %[q2_out3i].4s, v14.4s, v2.4s[1] \n\t" // RI
            "fmls %[q2_out3r].4s, v15.4s, v2.4s[1] \n\t" // RR - II
            "fmla %[q2_out3i].4s, v15.4s, v2.4s[0] \n\t" // RI + IR
        : [q2_out0r]"+w"(q2_out0.val[0]),
          [q2_out0i]"+w"(q2_out0.val[1]),
          [q2_out1r]"+w"(q2_out1.val[0]),
          [q2_out1i]"+w"(q2_out1.val[1]),
          [q2_out2r]"+w"(q2_out2.val[0]),
          [q2_out2i]"+w"(q2_out2.val[1]),
          [q2_out3r]"+w"(q2_out3.val[0]),
          [q2_out3i]"+w"(q2_out3.val[1]),
          [ptr_inr]"+r"(ptr_inr),
          [ptr_ini]"+r"(ptr_ini),
          [ptr_tw]"+r"(ptr_tw)
        : [ptw0]"r"(twiddles),
          [ptw1]"r"(twiddles + out_step),
          [ptw2]"r"(twiddles + out_step * 2),
          [offset_in]"r"(in_step * 16),
          [offset_out]"r"(out_step * 4)
        : "memory", "v0", "v1", "v2",
          "v10", "v11", "v12", "v13", "v14", "v15"
        );
#endif // __aarch64__
#endif // NE10_INLINE_ASM_OPT

        NE10_RADIX4x4_R2C_TW_NEON_KERNEL_S1 (q2_in, q2_out);
        NE10_RADIX4x4_R2C_TW_NEON_KERNEL_S2 (q2_out, q2_in);

        // store
        vst1q_f32( (ne10_float32_t*) ( Fout_neon                      ), q2_out0.val[0] );
        vst1q_f32( (ne10_float32_t*) ( Fout_neon                   + 1), q2_out0.val[1] );

        vst1q_f32( (ne10_float32_t*) ( Fout_neon + (out_step << 1)    ), q2_out1.val[0] );
        vst1q_f32( (ne10_float32_t*) ( Fout_neon + (out_step << 1) + 1), q2_out1.val[1] );

        vst1q_f32( (ne10_float32_t*) ( Fout_b                         ), q2_out2.val[0] );
        vst1q_f32( (ne10_float32_t*) ( Fout_b                      + 1), q2_out2.val[1] );

        vst1q_f32( (ne10_float32_t*) ( Fout_b    - (out_step << 1)    ), q2_out3.val[0] );
        vst1q_f32( (ne10_float32_t*) ( Fout_b    - (out_step << 1) + 1), q2_out3.val[1] );

        // update pointers
        Fin_neon  += 2;
        Fout_neon += 2;
        Fout_b    -= 2;
        twiddles ++;
    }
}

NE10_INLINE void ne10_radix4x4_c2r_with_twiddles_other_butterfly_neon (float32x4_t *Fout_neon,
                                                                const float32x4_t *Fin_neon,
                                                                const ne10_int32_t out_step,
                                                                const ne10_int32_t in_step,
                                                                const ne10_fft_cpx_float32_t *twiddles)
{
    ne10_int32_t m_count;
    ne10_int32_t loop_count = (out_step>>1) -1;
    const float32x4_t *Fin_b = Fin_neon + (((out_step<<1)-1)<<1) - 2; // reversed

    NE10_DECLARE_3(float32x4x2_t,q2_tw);
    NE10_DECLARE_4(float32x4x2_t,q2_in);
    NE10_DECLARE_4(float32x4x2_t,q2_out);

    for (m_count = loop_count; m_count > 0; m_count -- )
    {
        // load
        q2_in0.val[0] = vld1q_f32( (ne10_float32_t*) ( Fin_neon                      ) );
        q2_in0.val[1] = vld1q_f32( (ne10_float32_t*) ( Fin_neon                   + 1) );

        q2_in1.val[0] = vld1q_f32( (ne10_float32_t*) ( Fin_neon + (out_step << 1)    ) );
        q2_in1.val[1] = vld1q_f32( (ne10_float32_t*) ( Fin_neon + (out_step << 1) + 1) );

        q2_in2.val[0] = vld1q_f32( (ne10_float32_t*) ( Fin_b                         ) );
        q2_in2.val[1] = vld1q_f32( (ne10_float32_t*) ( Fin_b                      + 1) );

        q2_in3.val[0] = vld1q_f32( (ne10_float32_t*) ( Fin_b    - (out_step << 1)    ) );
        q2_in3.val[1] = vld1q_f32( (ne10_float32_t*) ( Fin_b    - (out_step << 1) + 1) );

        q2_tw0.val[0] = vdupq_n_f32(twiddles[0].r);
        q2_tw0.val[1] = vdupq_n_f32(twiddles[0].i);

        q2_tw1.val[0] = vdupq_n_f32(twiddles[out_step].r);
        q2_tw1.val[1] = vdupq_n_f32(twiddles[out_step].i);

        q2_tw2.val[0] = vdupq_n_f32(twiddles[out_step*2].r);
        q2_tw2.val[1] = vdupq_n_f32(twiddles[out_step*2].i);

        // NE10_PRINT_Q2x4_VECTOR(q2_in);

        // R2C TW KERNEL
        NE10_RADIX4x4_C2R_TW_NEON_KERNEL(q2_out,q2_in,q2_tw);

        // NE10_PRINT_Q2x4_VECTOR(q2_out);

        // store
        vst1q_f32( (ne10_float32_t*) (Fout_neon + 0*in_step    ), q2_out0.val[0] );
        vst1q_f32( (ne10_float32_t*) (Fout_neon + 0*in_step + 1), q2_out0.val[1] );

        vst1q_f32( (ne10_float32_t*) (Fout_neon + 1*in_step    ), q2_out1.val[0] );
        vst1q_f32( (ne10_float32_t*) (Fout_neon + 1*in_step + 1), q2_out1.val[1] );

        vst1q_f32( (ne10_float32_t*) (Fout_neon + 2*in_step    ), q2_out2.val[0] );
        vst1q_f32( (ne10_float32_t*) (Fout_neon + 2*in_step + 1), q2_out2.val[1] );

        vst1q_f32( (ne10_float32_t*) (Fout_neon + 3*in_step    ), q2_out3.val[0] );
        vst1q_f32( (ne10_float32_t*) (Fout_neon + 3*in_step + 1), q2_out3.val[1] );

        // update pointers
        Fin_neon  += 2;
        Fout_neon += 2;
        Fin_b    -= 2;
        twiddles ++;
    }
}

NE10_INLINE void ne10_radix4x4_r2c_with_twiddles_last_butterfly_neon (float32x4_t *Fout_neon,
                                                            const float32x4_t *Fin_neon,
                                                            const ne10_int32_t out_step,
                                                            const ne10_int32_t in_step,
                                                            const ne10_fft_cpx_float32_t *twiddles)
{
    NE10_DECLARE_4(float32x4_t,q_in);
    NE10_DECLARE_4(float32x4_t,q_out);

    // load
    NE10_RADIX4x4_R2C_NEON_LOAD(Fin_neon,q_in,in_step);

    NE10_RADIX4x4_R2C_TW_NEON_KERNEL_LAST(q_out,q_in);

    // store
    vst1q_f32( (ne10_float32_t*) (Fout_neon    ), q_out0);
    vst1q_f32( (ne10_float32_t*) (Fout_neon + 1), q_out1);
    vst1q_f32( (ne10_float32_t*) (Fout_neon + (out_step << 1)    ), q_out2);
    vst1q_f32( (ne10_float32_t*) (Fout_neon + (out_step << 1) + 1), q_out3);
}

NE10_INLINE void ne10_radix4x4_c2r_with_twiddles_last_butterfly_neon (float32x4_t *Fout_neon,
                                                            const float32x4_t *Fin_neon,
                                                            const ne10_int32_t out_step,
                                                            const ne10_int32_t in_step,
                                                            const ne10_fft_cpx_float32_t *twiddles)
{
    NE10_DECLARE_4(float32x4_t,q_in);
    NE10_DECLARE_4(float32x4_t,q_out);

    // load
    q_in0 = vld1q_f32( (ne10_float32_t*) (Fin_neon    ) );
    q_in1 = vld1q_f32( (ne10_float32_t*) (Fin_neon + 1) );
    q_in2 = vld1q_f32( (ne10_float32_t*) (Fin_neon + (out_step << 1)    ) );
    q_in3 = vld1q_f32( (ne10_float32_t*) (Fin_neon + (out_step << 1) + 1) );

    // NE10_PRINT_Qx4_VECTOR(q_in);

    NE10_RADIX4x4_C2R_TW_NEON_KERNEL_LAST(q_out,q_in);

    // NE10_PRINT_Qx4_VECTOR(q_out);

    // store
    NE10_RADIX4x4_R2C_NEON_STORE(Fout_neon,q_out,in_step);
}

NE10_INLINE void ne10_radix4x4_r2c_with_twiddles_neon (ne10_fft_cpx_float32_t *Fout,
                                                        const ne10_fft_cpx_float32_t *Fin,
                                                        const ne10_int32_t fstride,
                                                        const ne10_int32_t mstride,
                                                        const ne10_int32_t nfft,
                                                        const ne10_fft_cpx_float32_t *twiddles)
{
    ne10_int32_t f_count;
    const ne10_int32_t in_step = nfft >> 2;
    const ne10_int32_t out_step = mstride;

    const float32x4_t *Fin_neon  = (float32x4_t*) Fin;
          float32x4_t *Fout_neon = (float32x4_t*) Fout;
    const ne10_fft_cpx_float32_t *tw;

    for (f_count = fstride; f_count; f_count --)
    {
        tw = twiddles;

        // first butterfly
        ne10_radix4x4_r2c_with_twiddles_first_butterfly_neon ( Fout_neon, Fin_neon, out_step, in_step, tw);

        tw ++;
        Fin_neon ++;
        Fout_neon ++;

        // other butterfly
        ne10_radix4x4_r2c_with_twiddles_other_butterfly_neon ( Fout_neon, Fin_neon, out_step, in_step, tw);

        // update Fin_r, Fout_r, twiddles
        tw        +=     ( (out_step >> 1) - 1);
        Fin_neon  += 2 * ( (out_step >> 1) - 1);
        Fout_neon += 2 * ( (out_step >> 1) - 1);

        // last butterfly
        ne10_radix4x4_r2c_with_twiddles_last_butterfly_neon (Fout_neon, Fin_neon, out_step, in_step, tw);
        Fin_neon ++;
        tw++;
        Fout_neon ++;

        Fout_neon = Fout_neon + 3 * out_step;
    } // f_count
}

NE10_INLINE void ne10_radix4x4_c2r_with_twiddles_neon (ne10_fft_cpx_float32_t *Fout,
                                                        const ne10_fft_cpx_float32_t *Fin,
                                                        const ne10_int32_t fstride,
                                                        const ne10_int32_t mstride,
                                                        const ne10_int32_t nfft,
                                                        const ne10_fft_cpx_float32_t *twiddles)
{
    ne10_int32_t f_count;
    const ne10_int32_t in_step = nfft >> 2;
    const ne10_int32_t out_step = mstride;

    const float32x4_t *Fin_neon  = (float32x4_t*) Fin;
          float32x4_t *Fout_neon = (float32x4_t*) Fout;
    const ne10_fft_cpx_float32_t *tw;

    for (f_count = fstride; f_count; f_count --)
    {
        tw = twiddles;

        // first butterfly
        ne10_radix4x4_c2r_with_twiddles_first_butterfly_neon ( Fout_neon, Fin_neon, out_step, in_step, tw);

        tw ++;
        Fin_neon ++;
        Fout_neon ++;

        // other butterfly
        ne10_radix4x4_c2r_with_twiddles_other_butterfly_neon ( Fout_neon, Fin_neon, out_step, in_step, tw);

        // update Fin_r, Fout_r, twiddles
        tw        +=     ( (out_step >> 1) - 1);
        Fin_neon  += 2 * ( (out_step >> 1) - 1);
        Fout_neon += 2 * ( (out_step >> 1) - 1);

        // last butterfly
        ne10_radix4x4_c2r_with_twiddles_last_butterfly_neon (Fout_neon, Fin_neon, out_step, in_step, tw);
        Fin_neon ++;
        tw++;
        Fout_neon ++;

        Fin_neon = Fin_neon + 3 * out_step;
    } // f_count
}

NE10_INLINE void ne10_mixed_radix_r2c_butterfly_float32_neon (ne10_fft_cpx_float32_t * Fout,
                                                        const ne10_fft_cpx_float32_t * Fin,
                                                        const ne10_int32_t * factors,
                                                        const ne10_fft_cpx_float32_t * twiddles,
                                                        ne10_fft_cpx_float32_t * buffer)
{
    ne10_int32_t fstride, mstride, nfft;
    ne10_int32_t radix;
    ne10_int32_t stage_count;

    // PRINT_STAGE_INFO;

    // init fstride, mstride, radix, nfft
    stage_count = factors[0];
    fstride     = factors[1];
    mstride     = factors[ (stage_count << 1) - 1 ];
    radix       = factors[  stage_count << 1 ];
    nfft        = radix * fstride; // not the real nfft

    // PRINT_STAGE_INFO;

    if (radix == 2)
    {
        // combine one radix-4 and one radix-2 into one radix-8
        mstride <<= 2;
        fstride >>= 2;
        twiddles += 6;
        stage_count --;
    }

    if (stage_count % 2 == 1) // since there is another stage outside
    {
        ne10_swap_ptr (buffer, Fout);
    }

    // the first stage
    if (radix == 2)   // length of FFT is 2^n (n is odd)
    {
        ne10_radix8x4_r2c_neon (Fout, Fin, fstride, mstride, nfft);
    }
    else if (radix == 4)   // length of FFT is 2^n (n is even)
    {
        ne10_radix4x4_r2c_neon (Fout, Fin, fstride, mstride, nfft);
    }
    // end of first stage

    // others
    for (; fstride > 1;)
    {
        fstride >>= 2;
        ne10_swap_ptr (buffer, Fout);

        ne10_radix4x4_r2c_with_twiddles_neon (Fout, buffer, fstride, mstride, nfft, twiddles);
        twiddles += 3 * mstride;
        mstride <<= 2;
    } // other stage
}

NE10_INLINE void ne10_mixed_radix_c2r_butterfly_float32_neon (ne10_fft_cpx_float32_t * Fout,
                                                        const ne10_fft_cpx_float32_t * Fin,
                                                        const ne10_int32_t * factors,
                                                        const ne10_fft_cpx_float32_t * twiddles,
                                                        ne10_fft_cpx_float32_t * buffer)
{
    ne10_int32_t fstride, mstride, nfft;
    ne10_int32_t radix;
    ne10_int32_t stage_count;

    // PRINT_STAGE_INFO;

    // init fstride, mstride, radix, nfft
    stage_count = factors[0];
    fstride     = factors[1];

    mstride     = factors[ (stage_count << 1) - 1 ];
    radix       = factors[  stage_count << 1 ];
    nfft        = radix * fstride; // not the real nfft

    // fstride, mstride for last last stage
    fstride = 1;
    mstride = nfft >> 2;

    if (radix == 2)
    {
        // combine one radix-4 and one radix-2 into one radix-8
        stage_count --;
    }

    if (stage_count % 2 == 0)
    {
        ne10_swap_ptr(Fout,buffer);
    }

    // others but the first stage
    for (; stage_count > 1;)
    {
        twiddles -= 3 * mstride;

        // PRINT_STAGE_INFO;
        // PRINT_POINTERS_INFO(Fin,Fout,buffer,twiddles);
        ne10_radix4x4_c2r_with_twiddles_neon (Fout, buffer, fstride, mstride, nfft, twiddles);

        fstride <<= 2;
        mstride >>= 2;
        stage_count --;
        ne10_swap_ptr (buffer, Fout);
    }

    // first stage -- inversed
    if (radix == 2)   // length of FFT is 2^n (n is odd)
    {
        mstride >>= 1;

        // PRINT_STAGE_INFO;
        // PRINT_POINTERS_INFO(Fin,Fout,buffer,twiddles);
        ne10_radix8x4_c2r_neon (Fout, buffer, fstride, mstride, nfft);
    }
    else if (radix == 4)   // length of FFT is 2^n (n is even)
    {
        // PRINT_STAGE_INFO;
        // PRINT_POINTERS_INFO(Fin,Fout,buffer,twiddles);
        ne10_radix4x4_c2r_neon (Fout, buffer, fstride, mstride, nfft);
    }
}

NE10_INLINE void ne10_radix4_r2c_with_twiddles_last_stage_first_butterfly (ne10_fft_cpx_float32_t *dst,
                                            const ne10_fft_cpx_float32_t *src,
                                            const ne10_fft_cpx_float32_t *twiddles,
                                            const ne10_int32_t nfft)
{
    // b0
    {
        ne10_float32_t q_4r_out[4];
        const ne10_float32_t *p_src_r = (const ne10_float32_t*) src;

        NE10_FFT_R2C_4R_RCR(q_4r_out,p_src_r);

        dst[0].r = q_4r_out[0];
        dst[0].i = q_4r_out[3];
        dst += (nfft>>2);
        dst[0].r = q_4r_out[1];
        dst[0].i = q_4r_out[2];
        dst -= (nfft>>2);
    }

    // b2
    {
        const ne10_float32_t *p_src_r = (const ne10_float32_t*) (src);
        p_src_r  += nfft;
        p_src_r  -= 4;

        ne10_float32_t q_4r_out[4];

        NE10_FFT_R2C_4R_CC(q_4r_out,p_src_r);

        dst += (nfft>>3);
        dst[0].r = q_4r_out[0];
        dst[0].i = q_4r_out[1];
        dst += (nfft>>2);
        dst[0].r = q_4r_out[2];
        dst[0].i = q_4r_out[3];
        dst -= (nfft>>3);
        dst -= (nfft>>2);
    }

    // b1
    ne10_fft_cpx_float32_t cc_out[4];
    ne10_fft_cpx_float32_t cc_in [4];
    const ne10_float32_t *p_src_r = (const ne10_float32_t*) src;
    p_src_r += 4;

    cc_out[0].r = *(p_src_r ++);
    cc_out[1].r = *(p_src_r ++);
    cc_out[2].r = *(p_src_r ++);
    cc_out[3].r = *(p_src_r ++);

    cc_out[0].i = *(p_src_r ++);
    cc_out[1].i = *(p_src_r ++);
    cc_out[2].i = *(p_src_r ++);
    cc_out[3].i = *(p_src_r ++);

    NE10_PRINT_Q2_VECTOR(cc_out);

    // twiddles[0] = ( 1.0, 0.0);
    // NE10_CPX_MUL_F32(cc_in[0],cc_out[0],twiddles[0]);
    cc_in[0] = cc_out[0];
    twiddles ++;

    NE10_CPX_MUL_F32(cc_in[1],cc_out[1],twiddles[0]);
    twiddles ++;

    NE10_CPX_MUL_F32(cc_in[2],cc_out[2],twiddles[0]);
    twiddles ++;

    NE10_CPX_MUL_F32(cc_in[3],cc_out[3],twiddles[0]);

    // NE10_PRINT_Q2_VECTOR(cc_in);

    NE10_FFT_R2C_CC_CC(cc_out,cc_in);

    // NE10_PRINT_Q2_VECTOR(cc_out);

    dst[1] = cc_out[0];
    dst += (nfft>>2);
    dst[ 1] = cc_out[1];
    dst[-1] = cc_out[3];
    dst += (nfft>>2);
    dst[-1] = cc_out[2];
}

NE10_INLINE void ne10_radix4_c2r_with_twiddles_first_stage_first_butterfly (ne10_fft_cpx_float32_t *dst,
                                            const ne10_fft_cpx_float32_t *src,
                                            const ne10_fft_cpx_float32_t *twiddles,
                                            const ne10_int32_t nfft)
{
    // b0
    {
        ne10_float32_t q_4r_in[4];
        ne10_float32_t *p_dst_r = (ne10_float32_t*) dst;

        q_4r_in[0] = src[0].r;
        q_4r_in[3] = src[0].i;
        src += (nfft>>2);
        q_4r_in[1] = src[0].r;
        q_4r_in[2] = src[0].i;
        src -= (nfft>>2);

        NE10_FFT_C2R_RCR_4R(p_dst_r,q_4r_in);
    }

    // b2
    {
        // float32x4_t q_in;
        ne10_float32_t *p_dst_r = (ne10_float32_t*) (dst);
        p_dst_r  += nfft;
        p_dst_r  -= 4;

        ne10_float32_t q_4r_in[4];
        src += (nfft>>3);
        q_4r_in[0] = src[0].r;
        q_4r_in[1] = src[0].i;
        src += (nfft>>2);
        q_4r_in[2] = src[0].r;
        q_4r_in[3] = src[0].i;
        src -= (nfft>>3);
        src -= (nfft>>2);

        NE10_FFT_C2R_CC_4R(p_dst_r,q_4r_in);
    }

    // b1
    ne10_fft_cpx_float32_t cc_out[4];
    ne10_fft_cpx_float32_t cc_in [4];
    ne10_float32_t *p_dst_r = (ne10_float32_t*) dst;
    p_dst_r += 4;

    // load
    cc_out[0] = src[1];
    src += (nfft>>2);
    cc_out[2] = src[ 1];
    cc_out[3] = src[-1];
    src += (nfft>>2);
    cc_out[1] = src[-1];

    // NE10_PRINT_Q2_VECTOR(cc_out);

    NE10_FFT_C2R_CC_CC(cc_in,cc_out);

    // NE10_PRINT_Q2_VECTOR(cc_in);

    // twiddles[0] = ( 1.0, 0.0);
    // NE10_CPX_MUL_F32(cc_in[0],cc_out[0],twiddles[0]);
    cc_out[0] = cc_in[0];
    twiddles ++;

    NE10_CPX_CONJ_MUL_F32(cc_out[1],cc_in[1],twiddles[0]);
    twiddles ++;

    NE10_CPX_CONJ_MUL_F32(cc_out[2],cc_in[2],twiddles[0]);
    twiddles ++;

    NE10_CPX_CONJ_MUL_F32(cc_out[3],cc_in[3],twiddles[0]);

    // NE10_PRINT_Q2_VECTOR(cc_out);

    *(p_dst_r ++) = cc_out[0].r;
    *(p_dst_r ++) = cc_out[1].r;
    *(p_dst_r ++) = cc_out[2].r;
    *(p_dst_r ++) = cc_out[3].r;

    *(p_dst_r ++) = cc_out[0].i;
    *(p_dst_r ++) = cc_out[1].i;
    *(p_dst_r ++) = cc_out[2].i;
    *(p_dst_r ++) = cc_out[3].i;
}

NE10_INLINE void ne10_radix4_r2c_with_twiddles_last_stage_second_butterfly (ne10_fft_cpx_float32_t *dst,
                                            const ne10_fft_cpx_float32_t *src,
                                            const ne10_fft_cpx_float32_t *twiddles,
                                            const ne10_int32_t nfft)
{
    // assert ( nfft % 4 == 0 );
    const ne10_float32_t *fin_r  = (const ne10_float32_t*) src + 12;
          ne10_float32_t *fout_r =       (ne10_float32_t*) dst;
    const ne10_float32_t *tw     = (const ne10_float32_t*) twiddles + 8;

    ne10_float32_t q_in0[4],    q_out0[4],
                   q_in1[4],    q_out1[4],
                   q_in2[4],    q_out2[4],
                   q_in3[4],    q_out3[4];

    ne10_float32_t q2_tw0[2][4],
                   q2_tw1[2][4];

    /*  INPUT & OUTPUT
     *  0R  1R  2R  3R      Q0
     *  0I  1I  2I  3I      Q1
     *  4R  5R  6R  7R      Q2
     *  4I  5I  6I  7I      Q3
     */

    q_in0[0] = *(fin_r++);
    q_in0[1] = *(fin_r++);
    q_in0[2] = *(fin_r++);
    q_in0[3] = *(fin_r++);
    q_in1[0] = *(fin_r++);
    q_in1[1] = *(fin_r++);
    q_in1[2] = *(fin_r++);
    q_in1[3] = *(fin_r++);
    q_in2[0] = *(fin_r++);
    q_in2[1] = *(fin_r++);
    q_in2[2] = *(fin_r++);
    q_in2[3] = *(fin_r++);
    q_in3[0] = *(fin_r++);
    q_in3[1] = *(fin_r++);
    q_in3[2] = *(fin_r++);
    q_in3[3] = *(fin_r++);

    // NE10_PRINT_Q_VECTOR(q_in0);
    // NE10_PRINT_Q_VECTOR(q_in1);
    // NE10_PRINT_Q_VECTOR(q_in2);
    // NE10_PRINT_Q_VECTOR(q_in3);

    q2_tw0[0][0] = tw[0];
    q2_tw0[0][1] = tw[2];
    q2_tw0[0][2] = tw[4];
    q2_tw0[0][3] = tw[6];
    q2_tw0[1][0] = tw[1];
    q2_tw0[1][1] = tw[3];
    q2_tw0[1][2] = tw[5];
    q2_tw0[1][3] = tw[7];

    q2_tw1[0][0] = tw[0+8];
    q2_tw1[0][1] = tw[2+8];
    q2_tw1[0][2] = tw[4+8];
    q2_tw1[0][3] = tw[6+8];
    q2_tw1[1][0] = tw[1+8];
    q2_tw1[1][1] = tw[3+8];
    q2_tw1[1][2] = tw[5+8];
    q2_tw1[1][3] = tw[7+8];

    // TW: in->out
    q_out0[0] = q_in0[0];
    q_out1[0] = q_in1[0];
    q_out2[0] = q_in2[0];
    q_out3[0] = q_in3[0];

    //----------------------------------------------------------//
    // first 2 lines
    //   R          R             R           I             I
    q_out0[1] = q_in0[1] * q2_tw0[0][1] - q_in1[1] * q2_tw0[1][1];
    //   I          R             I           I             R
    q_out1[1] = q_in0[1] * q2_tw0[1][1] + q_in1[1] * q2_tw0[0][1];

    //   R          R             R           I             I
    q_out0[2] = q_in0[2] * q2_tw0[0][2] - q_in1[2] * q2_tw0[1][2];
    //   I          R             I           I             R
    q_out1[2] = q_in0[2] * q2_tw0[1][2] + q_in1[2] * q2_tw0[0][2];

    //   R          R             R           I             I
    q_out0[3] = q_in0[3] * q2_tw0[0][3] - q_in1[3] * q2_tw0[1][3];
    //   I          R             I           I             R
    q_out1[3] = q_in0[3] * q2_tw0[1][3] + q_in1[3] * q2_tw0[0][3];

    //---------------------------------------------------------//
    // second 2 lines
    //   R          R             R           I             I
    q_out2[1] = q_in2[1] * q2_tw1[0][1] - q_in3[1] * q2_tw1[1][1];
    //   I          R             I           I             R
    q_out3[1] = q_in2[1] * q2_tw1[1][1] + q_in3[1] * q2_tw1[0][1];

    //   R          R             R           I             I
    q_out2[2] = q_in2[2] * q2_tw1[0][2] - q_in3[2] * q2_tw1[1][2];
    //   I          R             I           I             R
    q_out3[2] = q_in2[2] * q2_tw1[1][2] + q_in3[2] * q2_tw1[0][2];

    //   R          R             R           I             I
    q_out2[3] = q_in2[3] * q2_tw1[0][3] - q_in3[3] * q2_tw1[1][3];
    //   I          R             I           I             R
    q_out3[3] = q_in2[3] * q2_tw1[1][3] + q_in3[3] * q2_tw1[0][3];

    // NE10_PRINT_Q_VECTOR(q_out0);
    // NE10_PRINT_Q_VECTOR(q_out1);
    // NE10_PRINT_Q_VECTOR(q_out2);
    // NE10_PRINT_Q_VECTOR(q_out3);

    // BUTTERFLY - radix 4x2
    // STAGE
    // q_out -> q_in
    //  R i         R j         R k
    q_in0[0] = q_out0[0] + q_out0[2];
    q_in1[0] = q_out1[0] + q_out1[2];

    q_in0[1] = q_out0[0] - q_out0[2];
    q_in1[1] = q_out1[0] - q_out1[2];

    //  R i         R j         R k
    q_in0[2] = q_out0[1] + q_out0[3];
    q_in1[2] = q_out1[1] + q_out1[3];

    q_in0[3] = q_out0[1] - q_out0[3];
    q_in1[3] = q_out1[1] - q_out1[3];

    //  R i         R j         R k
    q_in2[0] = q_out2[0] + q_out2[2];
    q_in3[0] = q_out3[0] + q_out3[2];

    q_in2[1] = q_out2[0] - q_out2[2];
    q_in3[1] = q_out3[0] - q_out3[2];

    //  R i         R j         R k
    q_in2[2] = q_out2[1] + q_out2[3];
    q_in3[2] = q_out3[1] + q_out3[3];

    q_in2[3] = q_out2[1] - q_out2[3];
    q_in3[3] = q_out3[1] - q_out3[3];

    // NE10_PRINT_Q_VECTOR(q_in0);
    // NE10_PRINT_Q_VECTOR(q_in1);
    // NE10_PRINT_Q_VECTOR(q_in2);
    // NE10_PRINT_Q_VECTOR(q_in3);

    // STAGE
    // q_in -> q_out
    // and transpose
    //   R i          R j        R k
    q_out0[0] =   q_in0[0] + q_in0[2];
    q_out0[1] =   q_in1[0] + q_in1[2];

    q_out2[2] =   q_in0[0] - q_in0[2];
    q_out2[3] = - q_in1[0] + q_in1[2];// CONJ

    //   R i          R j        R k
    q_out3[2] =   q_in0[1] - q_in1[3];
    q_out3[3] = - q_in1[1] - q_in0[3];// CONJ

    q_out1[0] =   q_in0[1] + q_in1[3];
    q_out1[1] =   q_in1[1] - q_in0[3];

    //   R i          R j        R k
    q_out0[2] =   q_in2[0] + q_in2[2];
    q_out0[3] =   q_in3[0] + q_in3[2];

    q_out2[0] =   q_in2[0] - q_in2[2];
    q_out2[1] = - q_in3[0] + q_in3[2];// CONJ

    //   R i          R j        R k
    q_out3[0] =   q_in2[1] - q_in3[3];
    q_out3[1] = - q_in3[1] - q_in2[3]; // CONJ

    q_out1[2] =   q_in2[1] + q_in3[3];
    q_out1[3] =   q_in3[1] - q_in2[3];

    // NE10_PRINT_Q_VECTOR(q_out0);
    // NE10_PRINT_Q_VECTOR(q_out1);
    // NE10_PRINT_Q_VECTOR(q_out2);
    // NE10_PRINT_Q_VECTOR(q_out3);

    // STORE
    fout_r += 4;
    fout_r[0] = q_out0[0];
    fout_r[1] = q_out0[1];
    fout_r[2] = q_out0[2];
    fout_r[3] = q_out0[3];

    fout_r += (nfft>>1);
    fout_r[0] = q_out1[0];
    fout_r[1] = q_out1[1];
    fout_r[2] = q_out1[2];
    fout_r[3] = q_out1[3];

    fout_r -= 10;
    fout_r[0] = q_out3[0];
    fout_r[1] = q_out3[1];
    fout_r[2] = q_out3[2];
    fout_r[3] = q_out3[3];

    fout_r += (nfft>>1);
    fout_r[0] = q_out2[0];
    fout_r[1] = q_out2[1];
    fout_r[2] = q_out2[2];
    fout_r[3] = q_out2[3];
}

NE10_INLINE void ne10_radix4_c2r_with_twiddles_first_stage_second_butterfly (ne10_fft_cpx_float32_t *dst,
                                            const ne10_fft_cpx_float32_t *src,
                                            const ne10_fft_cpx_float32_t *twiddles,
                                            const ne10_int32_t nfft)
{
    const ne10_float32_t *fin_r  = (const ne10_float32_t*) src;
          ne10_float32_t *fout_r =       (ne10_float32_t*) dst + 12;
    const ne10_float32_t *tw     = (const ne10_float32_t*) twiddles + 8;

    ne10_float32_t q_in0[4],    q_out0[4],
                   q_in1[4],    q_out1[4],
                   q_in2[4],    q_out2[4],
                   q_in3[4],    q_out3[4];

    ne10_float32_t q2_tw0[2][4],
                   q2_tw1[2][4];

    /*  INPUT & OUTPUT
     *  0R  1R  2R  3R      Q0
     *  0I  1I  2I  3I      Q1
     *  4R  5R  6R  7R      Q2
     *  4I  5I  6I  7I      Q3
     */

    // load
    fin_r += 4;
    q_in0[0] = fin_r[0];
    q_in0[1] = fin_r[1];
    q_in0[2] = fin_r[2];
    q_in0[3] = fin_r[3];

    fin_r += (nfft>>1);
    q_in1[0] = fin_r[0];
    q_in1[1] = fin_r[1];
    q_in1[2] = fin_r[2];
    q_in1[3] = fin_r[3];

    fin_r -= 10;
    q_in3[0] = fin_r[0];
    q_in3[1] = fin_r[1];
    q_in3[2] = fin_r[2];
    q_in3[3] = fin_r[3];

    fin_r += (nfft>>1);
    q_in2[0] = fin_r[0];
    q_in2[1] = fin_r[1];
    q_in2[2] = fin_r[2];
    q_in2[3] = fin_r[3];

    // NE10_PRINT_Q_VECTOR(q_in0);
    // NE10_PRINT_Q_VECTOR(q_in1);
    // NE10_PRINT_Q_VECTOR(q_in2);
    // NE10_PRINT_Q_VECTOR(q_in3);

    // OUTPUT
    // INPUT
#define NE10_INV_BUTTERFLY_TMP(I1,I2,J1,J2,K1,K2,S1,S2) do {    \
    q_out ## I1 [I2] = ( q_in ## K1 [K2] + q_in ## S1 [S2] ); \
    q_out ## J1 [J2] = ( q_in ## K1 [K2] - q_in ## S1 [S2] ); \
} while(0);

    // STAGE
    // q_in -> q_out
    // and transpose
    NE10_INV_BUTTERFLY_TMP( 0,0, 0,2,
                            0,0, 2,2);

    NE10_INV_BUTTERFLY_TMP( 1,2, 1,0,
                            0,1, 2,3);

    NE10_INV_BUTTERFLY_TMP( 0,1, 1,3,
                            1,0, 3,2);

    q_in3[3] *= - 1.0f;
    NE10_INV_BUTTERFLY_TMP( 1,1, 0,3,
                            3,3, 1,1);

    NE10_INV_BUTTERFLY_TMP( 2,0, 2,2,
                            0,2, 2,0);

    NE10_INV_BUTTERFLY_TMP( 3,2, 3,0,
                            0,3, 2,1);

    NE10_INV_BUTTERFLY_TMP( 2,1, 3,3,
                            1,2, 3,0);

    q_in3[1] *= - 1.0f;
    NE10_INV_BUTTERFLY_TMP( 3,1, 2,3,
                            3,1, 1,3);
#undef NE10_INV_BUTTERFLY_TMP

    // NE10_PRINT_Q_VECTOR(q_out0);
    // NE10_PRINT_Q_VECTOR(q_out1);
    // NE10_PRINT_Q_VECTOR(q_out2);
    // NE10_PRINT_Q_VECTOR(q_out3);

    // BUTTERFLY - radix 4x2
    // STAGE
    // q_out -> q_in

    // OUTPUT
    // INPUT
#define NE10_INV_BUTTERFLY_TMP(I1,I2,J1,J2,K1,K2,S1,S2) do {    \
    q_in ## I1 [I2] = ( q_out ## K1 [K2] + q_out ## S1 [S2] ); \
    q_in ## J1 [J2] = ( q_out ## K1 [K2] - q_out ## S1 [S2] ); \
} while(0);

    NE10_INV_BUTTERFLY_TMP(0,0, 0,2,
                           0,0, 0,1);

    NE10_INV_BUTTERFLY_TMP(1,0, 1,2,
                           1,0, 1,1);

    NE10_INV_BUTTERFLY_TMP(0,1, 0,3,
                           0,2, 0,3);

    NE10_INV_BUTTERFLY_TMP(1,1, 1,3,
                           1,2, 1,3);

    NE10_INV_BUTTERFLY_TMP(2,0, 2,2,
                           2,0, 2,1);

    NE10_INV_BUTTERFLY_TMP(3,0, 3,2,
                           3,0, 3,1);


    NE10_INV_BUTTERFLY_TMP(2,1, 2,3,
                           2,2, 2,3);

    NE10_INV_BUTTERFLY_TMP(3,1, 3,3,
                           3,2, 3,3);

    // NE10_PRINT_Q_VECTOR(q_in0);
    // NE10_PRINT_Q_VECTOR(q_in1);
    // NE10_PRINT_Q_VECTOR(q_in2);
    // NE10_PRINT_Q_VECTOR(q_in3);
#undef NE10_INV_BUTTERFLY_TMP

    // load tw
    q2_tw0[0][0] = tw[0];
    q2_tw0[0][1] = tw[2];
    q2_tw0[0][2] = tw[4];
    q2_tw0[0][3] = tw[6];
    q2_tw0[1][0] = tw[1];
    q2_tw0[1][1] = tw[3];
    q2_tw0[1][2] = tw[5];
    q2_tw0[1][3] = tw[7];

    q2_tw1[0][0] = tw[0+8];
    q2_tw1[0][1] = tw[2+8];
    q2_tw1[0][2] = tw[4+8];
    q2_tw1[0][3] = tw[6+8];
    q2_tw1[1][0] = tw[1+8];
    q2_tw1[1][1] = tw[3+8];
    q2_tw1[1][2] = tw[5+8];
    q2_tw1[1][3] = tw[7+8];

    // TW: in->out
    q_out0[0] = q_in0[0];
    q_out1[0] = q_in1[0];
    q_out2[0] = q_in2[0];
    q_out3[0] = q_in3[0];

    //----------------------------------------------------------//
    // first 2 lines
    //   R          R             R           I             I
    q_out0[1] = q_in0[1] * q2_tw0[0][1] + q_in1[1] * q2_tw0[1][1];
    //   I          R             I           I             R
    q_out1[1] = q_in0[1] * q2_tw0[1][1] - q_in1[1] * q2_tw0[0][1];

    //   R          R             R           I             I
    q_out0[2] = q_in0[2] * q2_tw0[0][2] + q_in1[2] * q2_tw0[1][2];
    //   I          R             I           I             R
    q_out1[2] = q_in0[2] * q2_tw0[1][2] - q_in1[2] * q2_tw0[0][2];

    //   R          R             R           I             I
    q_out0[3] = q_in0[3] * q2_tw0[0][3] + q_in1[3] * q2_tw0[1][3];
    //   I          R             I           I             R
    q_out1[3] = q_in0[3] * q2_tw0[1][3] - q_in1[3] * q2_tw0[0][3];

    //----------------------------------------------------------//
    // second 2 lines
    //   R          R             R           I             I
    q_out2[1] = q_in2[1] * q2_tw1[0][1] + q_in3[1] * q2_tw1[1][1];
    //   I          R             I           I             R
    q_out3[1] = q_in2[1] * q2_tw1[1][1] - q_in3[1] * q2_tw1[0][1];

    //   R          R             R           I             I
    q_out2[2] = q_in2[2] * q2_tw1[0][2] + q_in3[2] * q2_tw1[1][2];
    //   I          R             I           I             R
    q_out3[2] = q_in2[2] * q2_tw1[1][2] - q_in3[2] * q2_tw1[0][2];

    //   R          R             R           I             I
    q_out2[3] = q_in2[3] * q2_tw1[0][3] + q_in3[3] * q2_tw1[1][3];
    //   I          R             I           I             R
    q_out3[3] = q_in2[3] * q2_tw1[1][3] - q_in3[3] * q2_tw1[0][3];

    // STORE
    *(fout_r++) =   q_out0[0];
    *(fout_r++) =   q_out0[1];
    *(fout_r++) =   q_out0[2];
    *(fout_r++) =   q_out0[3];
    *(fout_r++) =   q_out1[0];
    *(fout_r++) = - q_out1[1];
    *(fout_r++) = - q_out1[2];
    *(fout_r++) = - q_out1[3];
    *(fout_r++) =   q_out2[0];
    *(fout_r++) =   q_out2[1];
    *(fout_r++) =   q_out2[2];
    *(fout_r++) =   q_out2[3];
    *(fout_r++) =   q_out3[0];
    *(fout_r++) = - q_out3[1];
    *(fout_r++) = - q_out3[2];
    *(fout_r++) = - q_out3[3];
}

NE10_INLINE void ne10_radix4_r2c_with_twiddles_last_stage_other_butterfly (ne10_fft_cpx_float32_t *dst,
                                            const ne10_fft_cpx_float32_t *src,
                                            const ne10_fft_cpx_float32_t *twiddles,
                                            const ne10_int32_t nfft)
{
    const ne10_float32_t *fin_r = ((const ne10_float32_t*) src) + 12 + 16;
    ne10_float32_t *fout_r = (ne10_float32_t*) dst + 8;
    ne10_float32_t *fout_b = (ne10_float32_t*) dst - 14;
    const ne10_float32_t *tw = ((const ne10_float32_t*) twiddles) + 8  + 16;

    // Take 4 elements as a set.
    // The leading 8 sets are already transformed in first and seconds butterflies.
    // This function transforms 8 sets in each loop.
    ne10_int32_t loop_count = ((nfft >> 2) - 8) >> 3;

    for (; loop_count > 0; loop_count--)
    {
        NE10_DECLARE_4 (float32x4x2_t, q2_in);    // 8Q
        NE10_DECLARE_3 (float32x4x2_t, q2_tw);    // 6Q
        NE10_DECLARE_4 (float32x4x2_t, q2_out);   // 8Q

        /*  INPUT
         *  0R  1R  2R  3R      Q0
         *  0I  1I  2I  3I      Q1
         *  4R  5R  6R  7R      Q2
         *  4I  5I  6I  7I      Q3
         *  8R  9R  aR  bR      Q4
         *  8I  9I  aI  bI      Q5
         *  cR  dR  eR  fR      Q6
         *  cI  dI  eI  fI      Q7
         */

        // transpose
        // q2_out -> q2_in
        /*
         *      val[0]
         *  0R  4R  8R  cR      Q0
         *  1R  5R  9R  dR      Q2
         *  2R  6R  aR  eR      Q4
         *  3R  7R  bR  fR      Q6
         *
         *      val[1]
         *  0I  4I  8I  cI      Q1
         *  1I  5I  9I  dI      Q3
         *  2I  6I  aI  eI      Q5
         *  3I  7I  bI  fI      Q7
         */

#ifndef NE10_INLINE_ASM_OPT
        q2_out0.val[0] = vld1q_f32 (fin_r);
        fin_r += 4;
        q2_out0.val[1] = vld1q_f32 (fin_r);
        fin_r += 4;
        q2_out1.val[0] = vld1q_f32 (fin_r);
        fin_r += 4;
        q2_out1.val[1] = vld1q_f32 (fin_r);
        fin_r += 4;
        q2_out2.val[0] = vld1q_f32 (fin_r);
        fin_r += 4;
        q2_out2.val[1] = vld1q_f32 (fin_r);
        fin_r += 4;
        q2_out3.val[0] = vld1q_f32 (fin_r);
        fin_r += 4;
        q2_out3.val[1] = vld1q_f32 (fin_r);
        fin_r += 4;

        NE10_RADIX4X4C_TRANSPOSE_NEON (q2_in, q2_out);
#else // NE10_INLINE_ASM_OPT
#ifndef __aarch64__
#error Currently, inline assembly optimizations are only available on AArch64.
#else // __aarch64__
        asm volatile (
            "ld1 {v0.4s}, [%[fin_r0]] \n\t" // q2_in0.val[0]
            "ld1 {v4.4s}, [%[fin_r1]] \n\t" // q2_in0.val[1]
            "ld1 {v1.4s}, [%[fin_r2]] \n\t" // q2_in1.val[0]
            "ld1 {v5.4s}, [%[fin_r3]] \n\t" // q2_in1.val[1]
            "ld1 {v2.4s}, [%[fin_r4]] \n\t" // q2_in2.val[0]
            "ld1 {v6.4s}, [%[fin_r5]] \n\t" // q2_in2.val[1]
            "ld1 {v3.4s}, [%[fin_r6]] \n\t" // q2_in3.val[0]
            "ld1 {v7.4s}, [%[fin_r7]] \n\t" // q2_in3.val[1]
            // NE10_RADIX4X4C_TRANSPOSE_NEON (q2_in,q2_out);
            "trn1 v8.4s, v0.4s, v1.4s \n\t"
            "trn2 v9.4s, v0.4s, v1.4s \n\t"
            "trn1 v10.4s, v2.4s, v3.4s \n\t"
            "trn2 v11.4s, v2.4s, v3.4s \n\t"

            "trn1 %[q2_in0r].2d, v8.2d, v10.2d \n\t"
            "trn1 %[q2_in1r].2d, v9.2d, v11.2d \n\t"
            "trn2 %[q2_in2r].2d, v8.2d, v10.2d \n\t"
            "trn2 %[q2_in3r].2d, v9.2d, v11.2d \n\t"

            "trn1 v8.4s, v4.4s, v5.4s \n\t"
            "trn2 v9.4s, v4.4s, v5.4s \n\t"
            "trn1 v10.4s, v6.4s, v7.4s \n\t"
            "trn2 v11.4s, v6.4s, v7.4s \n\t"

            "trn1 %[q2_in0i].2d, v8.2d, v10.2d \n\t"
            "trn1 %[q2_in1i].2d, v9.2d, v11.2d \n\t"
            "trn2 %[q2_in2i].2d, v8.2d, v10.2d \n\t"
            "trn2 %[q2_in3i].2d, v9.2d, v11.2d \n\t"

        : [q2_in0r]"+w"(q2_in0.val[0]),
          [q2_in0i]"+w"(q2_in0.val[1]),
          [q2_in1r]"+w"(q2_in1.val[0]),
          [q2_in1i]"+w"(q2_in1.val[1]),
          [q2_in2r]"+w"(q2_in2.val[0]),
          [q2_in2i]"+w"(q2_in2.val[1]),
          [q2_in3r]"+w"(q2_in3.val[0]),
          [q2_in3i]"+w"(q2_in3.val[1])
        : [fin_r0]"r"(fin_r),
          [fin_r1]"r"(fin_r + 4),
          [fin_r2]"r"(fin_r + 8),
          [fin_r3]"r"(fin_r + 12),
          [fin_r4]"r"(fin_r + 16),
          [fin_r5]"r"(fin_r + 20),
          [fin_r6]"r"(fin_r + 24),
          [fin_r7]"r"(fin_r + 28)
        : "memory", "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7",
          "v8", "v9", "v10", "v11"
        );
        fin_r += 32;
#endif // __aarch64__
#endif // NE10_INLINE_ASM_OPT

#ifndef NE10_INLINE_ASM_OPT
        // Load twiddles
        q2_tw0 = vld2q_f32 (tw);
        tw += 8;
        q2_tw1 = vld2q_f32 (tw);
        tw += 8;
        q2_tw2 = vld2q_f32 (tw);
        tw += 8;

        // tw
        // q2_in -> q2_out
        q2_out0 = q2_in0;
        NE10_CPX_MUL_NEON_F32 (q2_out1, q2_in1, q2_tw0);
        NE10_CPX_MUL_NEON_F32 (q2_out2, q2_in2, q2_tw1);
        NE10_CPX_MUL_NEON_F32 (q2_out3, q2_in3, q2_tw2);
#else // NE10_INLINE_ASM_OPT
#ifndef __aarch64__
#error Currently, inline assembly optimizations are only available on AArch64.
#else // __aarch64__
        asm volatile (
            // Load twiddles
            "ld2 {v0.4s, v1.4s}, [%[tw0]] \n\t" // q2_tw0
            "ld2 {v2.4s, v3.4s}, [%[tw1]] \n\t" // q2_tw1
            "ld2 {v4.4s, v5.4s}, [%[tw2]] \n\t" // q2_tw2
            // tw
            // q2_in -> q2_out
            // NE10_CPX_MUL_NEON_F32(q2_out1,q2_in1,q2_tw0);
            "fmul %[q2_out1r].4s, v0.4s, %[q2_in1r].4s \n\t" // RR
            "fmul %[q2_out1i].4s, v0.4s, %[q2_in1i].4s \n\t" // RI
            "fmls %[q2_out1r].4s, v1.4s, %[q2_in1i].4s \n\t" // RR - II
            "fmla %[q2_out1i].4s, v1.4s, %[q2_in1r].4s \n\t" // RI + IR
            // NE10_CPX_MUL_NEON_F32(q2_out2,q2_in2,q2_tw1);
            "fmul %[q2_out2r].4s, v2.4s, %[q2_in2r].4s \n\t" // RR
            "fmul %[q2_out2i].4s, v2.4s, %[q2_in2i].4s \n\t" // RI
            "fmls %[q2_out2r].4s, v3.4s, %[q2_in2i].4s \n\t" // RR - II
            "fmla %[q2_out2i].4s, v3.4s, %[q2_in2r].4s \n\t" // RI + IR
            // NE10_CPX_MUL_NEON_F32(q2_out3,q2_in3,q2_tw2);
            "fmul %[q2_out3r].4s, v4.4s, %[q2_in3r].4s \n\t" // RR
            "fmul %[q2_out3i].4s, v4.4s, %[q2_in3i].4s \n\t" // RI
            "fmls %[q2_out3r].4s, v5.4s, %[q2_in3i].4s \n\t" // RR - II
            "fmla %[q2_out3i].4s, v5.4s, %[q2_in3r].4s \n\t" // RI + IR
        : [q2_out1r]"+w"(q2_out1.val[0]),
          [q2_out1i]"+w"(q2_out1.val[1]),
          [q2_out2r]"+w"(q2_out2.val[0]),
          [q2_out2i]"+w"(q2_out2.val[1]),
          [q2_out3r]"+w"(q2_out3.val[0]),
          [q2_out3i]"+w"(q2_out3.val[1])
        : [tw0]"r"(tw),
          [tw1]"r"(tw + 8),
          [tw2]"r"(tw + 16),
          [q2_in1r]"w"(q2_in1.val[0]),
          [q2_in1i]"w"(q2_in1.val[1]),
          [q2_in2r]"w"(q2_in2.val[0]),
          [q2_in2i]"w"(q2_in2.val[1]),
          [q2_in3r]"w"(q2_in3.val[0]),
          [q2_in3i]"w"(q2_in3.val[1])
        : "memory", "v0", "v1", "v2", "v3", "v4", "v5"
        );
        q2_out0 = q2_in0;
        tw += 24;
#endif // __aarch64__
#endif // NE10_INLINE_ASM_OPT

        // butterfly
        // out -> in
        q2_in0.val[0] = vaddq_f32 (q2_out0.val[0], q2_out2.val[0]);
        q2_in0.val[1] = vaddq_f32 (q2_out0.val[1], q2_out2.val[1]);
        q2_in1.val[0] = vsubq_f32 (q2_out0.val[0], q2_out2.val[0]);
        q2_in1.val[1] = vsubq_f32 (q2_out0.val[1], q2_out2.val[1]);
        q2_in2.val[0] = vaddq_f32 (q2_out1.val[0], q2_out3.val[0]);
        q2_in2.val[1] = vaddq_f32 (q2_out1.val[1], q2_out3.val[1]);
        q2_in3.val[0] = vsubq_f32 (q2_out1.val[0], q2_out3.val[0]);
        q2_in3.val[1] = vsubq_f32 (q2_out1.val[1], q2_out3.val[1]);

        // in -> out
        q2_out2.val[0] = vsubq_f32 (q2_in0.val[0], q2_in2.val[0]);
        q2_out2.val[1] = vsubq_f32 (q2_in0.val[1], q2_in2.val[1]);
        q2_out3.val[0] = vsubq_f32 (q2_in1.val[0], q2_in3.val[1]);
        q2_out3.val[1] = vaddq_f32 (q2_in1.val[1], q2_in3.val[0]);

        q2_out3.val[1] = vnegq_f32 (q2_out3.val[1]);
        q2_out2.val[1] = vnegq_f32 (q2_out2.val[1]);

#ifndef NE10_INLINE_ASM_OPT
        q2_out0.val[0] = vaddq_f32 (q2_in0.val[0], q2_in2.val[0]);
        q2_out0.val[1] = vaddq_f32 (q2_in0.val[1], q2_in2.val[1]);

        q2_out1.val[0] = vaddq_f32 (q2_in1.val[0], q2_in3.val[1]);
        q2_out1.val[1] = vsubq_f32 (q2_in1.val[1], q2_in3.val[0]);

        // reverse -- CONJ
        NE10_REVERSE_FLOAT32X4 (q2_out2.val[0]);
        NE10_REVERSE_FLOAT32X4 (q2_out2.val[1]);
        NE10_REVERSE_FLOAT32X4 (q2_out3.val[0]);
        NE10_REVERSE_FLOAT32X4 (q2_out3.val[1]);

        // store
        vst2q_f32 (fout_r, q2_out0);
        vst2q_f32 (fout_r + (nfft >> 1), q2_out1);
        vst2q_f32 (fout_b + (nfft >> 1), q2_out3);
        vst2q_f32 (fout_b + nfft, q2_out2);
#else // NE10_INLINE_ASM_OPT
#ifndef __aarch64__
#error Currently, inline assembly optimizations are only available on AArch64.
#else // __aarch64__
        asm volatile (
            "fadd v0.4s, %[q2_in0r].4s, %[q2_in2r].4s \n\t"
            "fadd v1.4s, %[q2_in0i].4s, %[q2_in2i].4s \n\t"
            "fadd v2.4s, %[q2_in1r].4s, %[q2_in3i].4s \n\t"
            "fsub v3.4s, %[q2_in1i].4s, %[q2_in3r].4s \n\t"
            // reverse -- CONJ
            "rev64 %[q2_in2r].4s, %[q2_out2r].4s \n\t"
            "rev64 %[q2_in2i].4s, %[q2_out2i].4s \n\t"
            "rev64 %[q2_in3r].4s, %[q2_out3r].4s \n\t"
            "rev64 %[q2_in3i].4s, %[q2_out3i].4s \n\t"
            "ext v4.16b, %[q2_in2r].16b, %[q2_in2r].16b, #8 \n\t"
            "ext v5.16b, %[q2_in2i].16b, %[q2_in2i].16b, #8 \n\t"
            "ext v6.16b, %[q2_in3r].16b, %[q2_in3r].16b, #8 \n\t"
            "ext v7.16b, %[q2_in3i].16b, %[q2_in3i].16b, #8 \n\t"
            // store
            "st2 {v0.4s, v1.4s}, [%[fout0]] \n\t"
            "st2 {v2.4s, v3.4s}, [%[fout1]] \n\t"
            "st2 {v4.4s, v5.4s}, [%[fout2]] \n\t"
            "st2 {v6.4s, v7.4s}, [%[fout3]] \n\t"
        :
        : [fout0]"r"(fout_r),
          [fout1]"r"(fout_r + (nfft>>1)),
          [fout2]"r"(fout_b + nfft),
          [fout3]"r"(fout_b + (nfft>>1)),
          [q2_out2r]"w"(q2_out2.val[0]),
          [q2_out2i]"w"(q2_out2.val[1]),
          [q2_out3r]"w"(q2_out3.val[0]),
          [q2_out3i]"w"(q2_out3.val[1]),
          [q2_in0r]"w"(q2_in0.val[0]),
          [q2_in0i]"w"(q2_in0.val[1]),
          [q2_in1r]"w"(q2_in1.val[0]),
          [q2_in1i]"w"(q2_in1.val[1]),
          [q2_in2r]"w"(q2_in2.val[0]),
          [q2_in2i]"w"(q2_in2.val[1]),
          [q2_in3r]"w"(q2_in3.val[0]),
          [q2_in3i]"w"(q2_in3.val[1])
        : "memory", "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7"
        );
#endif // __aarch64__
#endif // NE10_INLINE_ASM_OPT

        fout_r += 8;
        fout_b -= 8;
    }
}

NE10_INLINE void ne10_radix4_c2r_with_twiddles_first_stage_other_butterfly (ne10_fft_cpx_float32_t *dst,
                                            const ne10_fft_cpx_float32_t *src,
                                            const ne10_fft_cpx_float32_t *twiddles,
                                            const ne10_int32_t nfft)
{
          ne10_float32_t *fout_r  =       ((ne10_float32_t*) dst ) + 12 + 16 ;
    const ne10_float32_t *fin_r =    (const ne10_float32_t*) src + 8;
    const ne10_float32_t *fin_b =    (const ne10_float32_t*) src - 14;
    const ne10_float32_t *tw     = ((const ne10_float32_t*) twiddles) + 8  + 16;
    ne10_int32_t loop_count = ((nfft>>2)-8)>>3;

    for ( ; loop_count>0; loop_count -- )
    {
        NE10_DECLARE_4(float32x4x2_t,q2_in);    // 8Q
        NE10_DECLARE_3(float32x4x2_t,q2_tw);    // 6Q
        NE10_DECLARE_4(float32x4x2_t,q2_out);   // 8Q

        /*  INPUT
         *  0R  1R  2R  3R      Q0
         *  0I  1I  2I  3I      Q1
         *  4R  5R  6R  7R      Q2
         *  4I  5I  6I  7I      Q3
         *  8R  9R  aR  bR      Q4
         *  8I  9I  aI  bI      Q5
         *  cR  dR  eR  fR      Q6
         *  cI  dI  eI  fI      Q7
         */

        q2_in0 = vld2q_f32(fin_r            );
        q2_in1 = vld2q_f32(fin_r + (nfft>>1));
        fin_r += 8;

        q2_in3 = vld2q_f32(fin_b + (nfft>>1));
        q2_in2 = vld2q_f32(fin_b + nfft     );
        fin_b -= 8;

        q2_tw0 = vld2q_f32(tw);
        tw += 8;
        q2_tw1 = vld2q_f32(tw);
        tw += 8;
        q2_tw2 = vld2q_f32(tw);
        tw += 8;

        // reverse -- CONJ
        NE10_REVERSE_FLOAT32X4( q2_in3.val[0] );
        NE10_REVERSE_FLOAT32X4( q2_in3.val[1] );
        NE10_REVERSE_FLOAT32X4( q2_in2.val[0] );
        NE10_REVERSE_FLOAT32X4( q2_in2.val[1] );

        q2_in2.val[1] = vnegq_f32( q2_in2.val[1] );
        q2_in3.val[1] = vnegq_f32( q2_in3.val[1] );

        // in -> out
        q2_out0.val[0] = vaddq_f32 (q2_in0.val[0], q2_in2.val[0]);
        q2_out2.val[0] = vsubq_f32 (q2_in0.val[0], q2_in2.val[0]);

        q2_out0.val[1] = vaddq_f32 (q2_in0.val[1], q2_in2.val[1]);
        q2_out2.val[1] = vsubq_f32 (q2_in0.val[1], q2_in2.val[1]);

        q2_out1.val[0] = vaddq_f32 (q2_in1.val[0], q2_in3.val[0]);
        q2_out3.val[1] = vsubq_f32 (q2_in1.val[0], q2_in3.val[0]);

        q2_out1.val[1] = vaddq_f32 (q2_in3.val[1], q2_in1.val[1]);
        q2_out3.val[0] = vsubq_f32 (q2_in3.val[1], q2_in1.val[1]);

        // out -> in
        q2_in0.val[0] = vaddq_f32 (q2_out0.val[0], q2_out1.val[0]);
        q2_in2.val[0] = vsubq_f32 (q2_out0.val[0], q2_out1.val[0]);

        q2_in0.val[1] = vaddq_f32 (q2_out0.val[1], q2_out1.val[1]);
        q2_in2.val[1] = vsubq_f32 (q2_out0.val[1], q2_out1.val[1]);

        q2_in1.val[0] = vaddq_f32 (q2_out2.val[0], q2_out3.val[0]);
        q2_in3.val[0] = vsubq_f32 (q2_out2.val[0], q2_out3.val[0]);

        q2_in1.val[1] = vaddq_f32 (q2_out2.val[1], q2_out3.val[1]);
        q2_in3.val[1] = vsubq_f32 (q2_out2.val[1], q2_out3.val[1]);

        // tw
        // q2_in -> q2_out
        q2_out0 = q2_in0;
        NE10_CPX_MUL_INV_NEON_F32(q2_out1,q2_in1,q2_tw0);
        NE10_CPX_MUL_INV_NEON_F32(q2_out2,q2_in2,q2_tw1);
        NE10_CPX_MUL_INV_NEON_F32(q2_out3,q2_in3,q2_tw2);

        // transpose
        // q2_out -> q2_in
        NE10_RADIX4X4C_TRANSPOSE_NEON (q2_in,q2_out);

        // store
        vst1q_f32(fout_r, q2_in0.val[0]);
        fout_r += 4;
        vst1q_f32(fout_r, q2_in0.val[1]);
        fout_r += 4;
        vst1q_f32(fout_r, q2_in1.val[0]);
        fout_r += 4;
        vst1q_f32(fout_r, q2_in1.val[1]);
        fout_r += 4;
        vst1q_f32(fout_r, q2_in2.val[0]);
        fout_r += 4;
        vst1q_f32(fout_r, q2_in2.val[1]);
        fout_r += 4;
        vst1q_f32(fout_r, q2_in3.val[0]);
        fout_r += 4;
        vst1q_f32(fout_r, q2_in3.val[1]);
        fout_r += 4;
    }
}

NE10_INLINE void ne10_radix4_r2c_with_twiddles_last_stage( ne10_fft_cpx_float32_t *dst,
                                            const ne10_fft_cpx_float32_t *src,
                                            const ne10_fft_cpx_float32_t *twiddles,
                                            const ne10_int32_t nfft)
{
    ne10_radix4_r2c_with_twiddles_last_stage_first_butterfly(dst,src,twiddles,nfft);

    if (nfft==16)
    {
        return;
    }

    ne10_radix4_r2c_with_twiddles_last_stage_second_butterfly(dst,src,twiddles,nfft);

    if (nfft==32)
    {
        return;
    }

    ne10_radix4_r2c_with_twiddles_last_stage_other_butterfly(dst,src,twiddles,nfft);
}

NE10_INLINE void ne10_radix4_c2r_with_twiddles_first_stage( ne10_fft_cpx_float32_t *dst,
                                            const ne10_fft_cpx_float32_t *src,
                                            const ne10_fft_cpx_float32_t *twiddles,
                                            const ne10_int32_t nfft)
{
    ne10_radix4_c2r_with_twiddles_first_stage_first_butterfly(dst,src,twiddles,nfft);

    if (nfft==16)
    {
        return;
    }

    ne10_radix4_c2r_with_twiddles_first_stage_second_butterfly(dst,src,twiddles,nfft);

    if (nfft==32)
    {
        return;
    }

    ne10_radix4_c2r_with_twiddles_first_stage_other_butterfly(dst,src,twiddles,nfft);
}

/**
 * @addtogroup R2C_FFT_IFFT
 * @{
 */

/**
 * @brief Mixed radix-2/4 FFT (real to complex) of float(32-bit) data.
 * @param[out]  *fout            point to the output buffer
 * @param[in]   *fin             point to the input buffer
 * @param[in]   cfg              point to the config struct
 * @return none.
 * The function implements a mixed radix-2/4 FFT (real to complex). The length of 2^N(N is 3, 4, 5, 6 ....etc) is supported.
 * Otherwise, we alloc a temp buffer(the size is same as input buffer) for storing intermedia.
 * For the usage of this function, please check test/test_suite_fft_float32.c
 */
void ne10_fft_r2c_1d_float32_neon (ne10_fft_cpx_float32_t *fout,
                                   ne10_float32_t *fin,
                                   ne10_fft_r2c_cfg_float32_t cfg)
{
    typedef         ne10_float32_t REAL;
    typedef ne10_fft_cpx_float32_t CPLX;

    ne10_fft_cpx_float32_t * tmpbuf = cfg->buffer;
    ne10_float32_t *fout_r = (ne10_float32_t*) fout;

    switch (cfg->nfft)
    {
        case 8:
            ne10_radix8_r2c_c ( (CPLX*) fout_r, (const CPLX*) fin, 1, 1, 8);
            fout[0].r = fout[0].i;
            break;
        default:
            ne10_mixed_radix_r2c_butterfly_float32_neon (fout, (CPLX*) fin, cfg->r_factors_neon, cfg->r_twiddles_neon, tmpbuf);
            ne10_radix4_r2c_with_twiddles_last_stage(fout, tmpbuf, cfg->r_super_twiddles_neon, cfg->nfft);
            fout[cfg->nfft / 2].r = fout[0].i;
            break;
    }
    fout[0].i = fout[cfg->nfft / 2].i = 0.0f;
}

/**
 * @brief Mixed radix-2/4 IFFT (complex to real) of float(32-bit) data.
 * @param[out]  *fout            point to the output buffer
 * @param[in]   *fin             point to the input buffer
 * @param[in]   cfg              point to the config struct
 * @return none.
 * The function implements a mixed radix-2/4 FFT (complex to real). The length of 2^N(N is 3, 4, 5, 6 ....etc) is supported.
 * Otherwise, we alloc a temp buffer(the size is same as input buffer) for storing intermedia.
 * For the usage of this function, please check test/test_suite_fft_float32.c
 */
void ne10_fft_c2r_1d_float32_neon (ne10_float32_t *fout,
                                   ne10_fft_cpx_float32_t *fin,
                                   ne10_fft_r2c_cfg_float32_t cfg)
{
    typedef         ne10_float32_t REAL;
    typedef ne10_fft_cpx_float32_t CPLX;

    ne10_fft_cpx_float32_t * tmpbuf = cfg->buffer;
    ne10_fft_cpx_float32_t * fout_c;
    ne10_int32_t stage_count;
    ne10_int32_t radix;

    switch (cfg->nfft)
    {
        case 8:
            fin[0].i = fin[0].r;
            fin[0].r = 0.0f;
            ne10_radix8_c2r_c  ( (CPLX*) fout, (const CPLX*) &fin[0].i, 1, 1, 8);
            fin[0].r = fin[0].i;
            break;
        default:
            stage_count = cfg->r_factors_neon[0];
            radix       = cfg->r_factors_neon[  stage_count << 1 ];
            if (radix==2)
            {
                stage_count --;
            }
            fin[0].i = fin[cfg->nfft>>1].r;
            fout_c = (stage_count % 2==1) ? tmpbuf : (CPLX*)fout;
            ne10_radix4_c2r_with_twiddles_first_stage( (CPLX*) fout_c, fin, cfg->r_super_twiddles_neon, cfg->nfft);
            ne10_mixed_radix_c2r_butterfly_float32_neon ( (CPLX*) fout, (CPLX*) NULL, cfg->r_factors_neon, cfg->r_twiddles_neon_backward, tmpbuf);
            break;
    }
    fin[0].i = 0.0f;
}

/**
 * @} end of R2C_FFT_IFFT group
 */
