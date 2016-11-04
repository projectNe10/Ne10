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
 * NE10 Library : dsp/NE10_rfft_float32.c
 */

#include "NE10_types.h"
#include "NE10_macros.h"
#include "NE10_fft.h"
#include "NE10_dsp.h"

#if (NE10_UNROLL_LEVEL > 0)

void ne10_radix8_r2c_c (ne10_fft_cpx_float32_t *Fout,
                        const ne10_fft_cpx_float32_t *Fin,
                        const ne10_int32_t fstride,
                        const ne10_int32_t mstride,
                        const ne10_int32_t nfft)
{
    const ne10_int32_t in_step = nfft >> 3;
          ne10_int32_t f_count;

    ne10_float32_t scratch_in[8];
    ne10_float32_t scratch   [4];

    /* real pointers */
    const ne10_float32_t* Fin_r  = (ne10_float32_t*) Fin;
          ne10_float32_t* Fout_r = (ne10_float32_t*) Fout;
    Fout_r ++; // always leave the first real empty

    for (f_count = fstride; f_count; f_count --)
    {
        scratch_in[0] = Fin_r[in_step * 0] + Fin_r[in_step * (0 + 4)];
        scratch_in[1] = Fin_r[in_step * 0] - Fin_r[in_step * (0 + 4)];
        scratch_in[2] = Fin_r[in_step * 1] + Fin_r[in_step * (1 + 4)];
        scratch_in[3] = Fin_r[in_step * 1] - Fin_r[in_step * (1 + 4)];
        scratch_in[4] = Fin_r[in_step * 2] + Fin_r[in_step * (2 + 4)];
        scratch_in[5] = Fin_r[in_step * 2] - Fin_r[in_step * (2 + 4)];
        scratch_in[6] = Fin_r[in_step * 3] + Fin_r[in_step * (3 + 4)];
        scratch_in[7] = Fin_r[in_step * 3] - Fin_r[in_step * (3 + 4)];

        scratch_in[3] *= TW_81_F32;
        scratch_in[7] *= TW_81N_F32;

        // radix 2 butterfly
        scratch[0] =   scratch_in[0] + scratch_in[4];
        scratch[1] =   scratch_in[2] + scratch_in[6];
        scratch[2] =   scratch_in[7] - scratch_in[3];
        scratch[3] =   scratch_in[3] + scratch_in[7];

        Fout_r[0] = scratch   [0] + scratch   [1];
        Fout_r[7] = scratch   [0] - scratch   [1];

        Fout_r[1] = scratch_in[1] + scratch   [3];
        Fout_r[5] = scratch_in[1] - scratch   [3];

        Fout_r[2] = scratch   [2] - scratch_in[5];
        Fout_r[6] = scratch   [2] + scratch_in[5];

        Fout_r[3] = scratch_in[0] - scratch_in[4];

        Fout_r[4] = scratch_in[6] - scratch_in[2];

        Fin_r  ++;
        Fout_r += 8;
    }
}

void ne10_radix8_c2r_c (ne10_fft_cpx_float32_t *Fout,
                        const ne10_fft_cpx_float32_t *Fin,
                        const ne10_int32_t fstride,
                        const ne10_int32_t mstride,
                        const ne10_int32_t nfft)
{
    const ne10_int32_t in_step = nfft >> 3;
          ne10_int32_t f_count;

    ne10_float32_t scratch_in[8];

    const ne10_float32_t one_by_N = 1.0 / nfft;

    /* real pointers */
    const ne10_float32_t* Fin_r  = (ne10_float32_t*) Fin;
          ne10_float32_t* Fout_r = (ne10_float32_t*) Fout;

    for (f_count = fstride; f_count; f_count --)
    {
        scratch_in[0] =   Fin_r[0] + Fin_r[3] + Fin_r[3] + Fin_r[7];
        scratch_in[1] =   Fin_r[1] + Fin_r[1] + Fin_r[5] + Fin_r[5];
        scratch_in[2] =   Fin_r[0] - Fin_r[4] - Fin_r[4] - Fin_r[7];
        scratch_in[3] =   Fin_r[1] - Fin_r[2] - Fin_r[5] - Fin_r[6];
        scratch_in[4] =   Fin_r[0] - Fin_r[3] - Fin_r[3] + Fin_r[7];
        scratch_in[5] = - Fin_r[2] - Fin_r[2] + Fin_r[6] + Fin_r[6];
        scratch_in[6] =   Fin_r[0] + Fin_r[4] + Fin_r[4] - Fin_r[7];
        scratch_in[7] =   Fin_r[1] + Fin_r[2] - Fin_r[5] + Fin_r[6];

        scratch_in[3] /= TW_81_F32;
        scratch_in[7] /= TW_81N_F32;

        Fout_r[0 * in_step] = scratch_in[0] + scratch_in[1];
        Fout_r[4 * in_step] = scratch_in[0] - scratch_in[1];
        Fout_r[1 * in_step] = scratch_in[2] + scratch_in[3];
        Fout_r[5 * in_step] = scratch_in[2] - scratch_in[3];
        Fout_r[2 * in_step] = scratch_in[4] + scratch_in[5];
        Fout_r[6 * in_step] = scratch_in[4] - scratch_in[5];
        Fout_r[3 * in_step] = scratch_in[6] + scratch_in[7];
        Fout_r[7 * in_step] = scratch_in[6] - scratch_in[7];

#if defined(NE10_DSP_RFFT_SCALING)
        Fout_r[0 * in_step] *= one_by_N;
        Fout_r[4 * in_step] *= one_by_N;
        Fout_r[1 * in_step] *= one_by_N;
        Fout_r[5 * in_step] *= one_by_N;
        Fout_r[2 * in_step] *= one_by_N;
        Fout_r[6 * in_step] *= one_by_N;
        Fout_r[3 * in_step] *= one_by_N;
        Fout_r[7 * in_step] *= one_by_N;
#endif

        Fin_r  += 8;
        Fout_r ++;
    }
}

NE10_INLINE void ne10_radix4_r2c_c (ne10_fft_cpx_float32_t *Fout,
                                  const ne10_fft_cpx_float32_t *Fin,
                                  const ne10_int32_t fstride,
                                  const ne10_int32_t mstride,
                                  const ne10_int32_t nfft)
{
    const ne10_int32_t in_step = nfft >> 2;
          ne10_int32_t f_count;

    ne10_float32_t  scratch_in [4];
    ne10_float32_t  scratch_out[4];

    /* real pointers */
    const ne10_float32_t *Fin_r  = (ne10_float32_t*) Fin;
          ne10_float32_t *Fout_r = (ne10_float32_t*) Fout;
    Fout_r ++; // always leave the first real empty

    for (f_count = fstride; f_count; f_count --)
    {
        scratch_in[0] = Fin_r[0 * in_step];
        scratch_in[1] = Fin_r[1 * in_step];
        scratch_in[2] = Fin_r[2 * in_step];
        scratch_in[3] = Fin_r[3 * in_step];

        // NE10_PRINT_Q_VECTOR(scratch_in);

        NE10_FFT_R2C_4R_RCR(scratch_out,scratch_in);

        // NE10_PRINT_Q_VECTOR(scratch_out);

        Fout_r[0] = scratch_out[0];
        Fout_r[1] = scratch_out[1];
        Fout_r[2] = scratch_out[2];
        Fout_r[3] = scratch_out[3];

        Fin_r  ++;
        Fout_r += 4;
    }
}

NE10_INLINE void ne10_radix4_c2r_c (ne10_fft_cpx_float32_t *Fout,
                                  const ne10_fft_cpx_float32_t *Fin,
                                  const ne10_int32_t fstride,
                                  const ne10_int32_t mstride,
                                  const ne10_int32_t nfft)
{
    ne10_int32_t f_count;
    const ne10_int32_t in_step = nfft >> 2;
    ne10_float32_t  scratch_in [4];
    ne10_float32_t  scratch_out[4];

    const ne10_float32_t one_by_N = 1.0 / nfft;

    /* real pointers */
    const ne10_float32_t *Fin_r  = (ne10_float32_t*) Fin;
          ne10_float32_t *Fout_r = (ne10_float32_t*) Fout;

    for (f_count = fstride; f_count; f_count --)
    {
        scratch_in[0] = Fin_r[0];
        scratch_in[1] = Fin_r[1];
        scratch_in[2] = Fin_r[2];
        scratch_in[3] = Fin_r[3];

        // NE10_PRINT_Q_VECTOR(scratch_in);

        NE10_FFT_C2R_RCR_4R(scratch_out,scratch_in);

        // NE10_PRINT_Q_VECTOR(scratch_out);

#if defined(NE10_DSP_RFFT_SCALING)
        scratch_out[0] *= one_by_N;
        scratch_out[1] *= one_by_N;
        scratch_out[2] *= one_by_N;
        scratch_out[3] *= one_by_N;
#endif

        // store
        Fout_r[0 * in_step] = scratch_out[0];
        Fout_r[1 * in_step] = scratch_out[1];
        Fout_r[2 * in_step] = scratch_out[2];
        Fout_r[3 * in_step] = scratch_out[3];

        Fin_r  += 4;
        Fout_r ++;
    }
}

NE10_INLINE void ne10_radix4_r2c_with_twiddles_first_butterfly_c (ne10_float32_t *Fout_r,
        const ne10_float32_t *Fin_r,
        const ne10_int32_t out_step,
        const ne10_int32_t in_step,
        const ne10_fft_cpx_float32_t *twiddles)
{
    ne10_float32_t scratch_out[4];
    ne10_float32_t scratch_in [4];

    // load
    scratch_in[0] = Fin_r[0 * in_step];
    scratch_in[1] = Fin_r[1 * in_step];
    scratch_in[2] = Fin_r[2 * in_step];
    scratch_in[3] = Fin_r[3 * in_step];

    // NE10_PRINT_Q_VECTOR(scratch_in);

    NE10_FFT_R2C_4R_RCR(scratch_out,scratch_in);

    // NE10_PRINT_Q_VECTOR(scratch_out);

    // store
    Fout_r[                      0] = scratch_out[0];
    Fout_r[    (out_step << 1) - 1] = scratch_out[1];
    Fout_r[    (out_step << 1)    ] = scratch_out[2];
    Fout_r[2 * (out_step << 1) - 1] = scratch_out[3];
}

NE10_INLINE void ne10_radix4_c2r_with_twiddles_first_butterfly_c (ne10_float32_t *Fout_r,
        const ne10_float32_t *Fin_r,
        const ne10_int32_t out_step,
        const ne10_int32_t in_step,
        const ne10_fft_cpx_float32_t *twiddles)
{
    ne10_float32_t scratch      [8];
    ne10_float32_t scratch_in_r [4];
    ne10_float32_t scratch_out_r[4];

    // load
    scratch_in_r[0] = Fin_r[0                ];
    scratch_in_r[1] = Fin_r[1*(out_step<<1)-1];
    scratch_in_r[2] = Fin_r[1*(out_step<<1)  ];
    scratch_in_r[3] = Fin_r[2*(out_step<<1)-1];

    // NE10_PRINT_Q_VECTOR(scratch_in_r);

    // radix 4 butterfly without twiddles
    scratch[0] = scratch_in_r[0] + scratch_in_r[3];
    scratch[1] = scratch_in_r[0] - scratch_in_r[3];
    scratch[2] = scratch_in_r[1] + scratch_in_r[1];
    scratch[3] = scratch_in_r[2] + scratch_in_r[2];

    scratch_out_r[0] = scratch[0] + scratch[2];
    scratch_out_r[1] = scratch[1] - scratch[3];
    scratch_out_r[2] = scratch[0] - scratch[2];
    scratch_out_r[3] = scratch[1] + scratch[3];

    // NE10_PRINT_Q_VECTOR(scratch_out_r);

    // store
    Fout_r[0 * in_step] = scratch_out_r[0];
    Fout_r[1 * in_step] = scratch_out_r[1];
    Fout_r[2 * in_step] = scratch_out_r[2];
    Fout_r[3 * in_step] = scratch_out_r[3];

}

NE10_INLINE void ne10_radix4_r2c_with_twiddles_other_butterfly_c (ne10_float32_t *Fout_r,
        const ne10_float32_t *Fin_r,
        const ne10_int32_t out_step,
        const ne10_int32_t in_step,
        const ne10_fft_cpx_float32_t *twiddles)
{
    ne10_int32_t m_count;
    ne10_float32_t *Fout_b = Fout_r + (((out_step<<1)-1)<<1) - 2; // reversed
    ne10_fft_cpx_float32_t scratch_tw[3], scratch_in[4];

    ne10_fft_cpx_float32_t scratch[4], scratch_out[4];

    for (m_count = (out_step >> 1) - 1; m_count; m_count --)
    {
        scratch_tw  [0] = twiddles[0 * out_step];
        scratch_tw  [1] = twiddles[1 * out_step];
        scratch_tw  [2] = twiddles[2 * out_step];

        scratch_in[0].r = Fin_r[0 * in_step    ];
        scratch_in[0].i = Fin_r[0 * in_step + 1];
        scratch_in[1].r = Fin_r[1 * in_step    ];
        scratch_in[1].i = Fin_r[1 * in_step + 1];
        scratch_in[2].r = Fin_r[2 * in_step    ];
        scratch_in[2].i = Fin_r[2 * in_step + 1];
        scratch_in[3].r = Fin_r[3 * in_step    ];
        scratch_in[3].i = Fin_r[3 * in_step + 1];

        // NE10_PRINT_Q2_VECTOR(scratch_in);

        // radix 4 butterfly with twiddles
        scratch[0].r = scratch_in[0].r;
        scratch[0].i = scratch_in[0].i;
        scratch[1].r = scratch_in[1].r * scratch_tw[0].r - scratch_in[1].i * scratch_tw[0].i;
        scratch[1].i = scratch_in[1].i * scratch_tw[0].r + scratch_in[1].r * scratch_tw[0].i;

        scratch[2].r = scratch_in[2].r * scratch_tw[1].r - scratch_in[2].i * scratch_tw[1].i;
        scratch[2].i = scratch_in[2].i * scratch_tw[1].r + scratch_in[2].r * scratch_tw[1].i;

        scratch[3].r = scratch_in[3].r * scratch_tw[2].r - scratch_in[3].i * scratch_tw[2].i;
        scratch[3].i = scratch_in[3].i * scratch_tw[2].r + scratch_in[3].r * scratch_tw[2].i;

        NE10_FFT_R2C_CC_CC(scratch_out,scratch);

        // NE10_PRINT_Q2_VECTOR(scratch_in);

        // result
        Fout_r[                    0] = scratch_out[0].r;
        Fout_r[                    1] = scratch_out[0].i;
        Fout_r[  (out_step << 1)    ] = scratch_out[1].r;
        Fout_r[  (out_step << 1) + 1] = scratch_out[1].i;
        Fout_b[                    0] = scratch_out[2].r;
        Fout_b[                    1] = scratch_out[2].i;
        Fout_b[- (out_step << 1)    ] = scratch_out[3].r;
        Fout_b[- (out_step << 1) + 1] = scratch_out[3].i;

        // update pointers
        Fin_r  += 2;
        Fout_r += 2;
        Fout_b -= 2;
        twiddles ++;
    }
}

NE10_INLINE void ne10_radix4_c2r_with_twiddles_other_butterfly_c (ne10_float32_t *Fout_r,
        const ne10_float32_t *Fin_r,
        const ne10_int32_t out_step,
        const ne10_int32_t in_step,
        const ne10_fft_cpx_float32_t *twiddles)
{
    ne10_int32_t m_count;
    const ne10_float32_t *Fin_b = Fin_r + (((out_step<<1)-1)<<1) - 2; // reversed
    ne10_fft_cpx_float32_t scratch_tw [3],
                           scratch    [8],
                           scratch_in [4],
                           scratch_out[4];

    for (m_count = (out_step >> 1) - 1; m_count; m_count --)
    {
        scratch_tw[0] = twiddles[0 * out_step];
        scratch_tw[1] = twiddles[1 * out_step];
        scratch_tw[2] = twiddles[2 * out_step];

        scratch_in[0].r = Fin_r[0];
        scratch_in[0].i = Fin_r[1];

        scratch_in[1].r = Fin_b[0];
        scratch_in[1].i = Fin_b[1];

        scratch_in[2].r = Fin_r[(out_step<<1) + 0];
        scratch_in[2].i = Fin_r[(out_step<<1) + 1];

        scratch_in[3].r = Fin_b[-(out_step<<1) + 0];
        scratch_in[3].i = Fin_b[-(out_step<<1) + 1];

        // NE10_PRINT_Q2_VECTOR(scratch_in);

        // // inverse of "result"
        NE10_FFT_C2R_CC_CC(scratch,scratch_in);

        // inverse of "mutltipy twiddles"
        scratch_out[0] = scratch[0];

        scratch_out[1].r = scratch[1].r * scratch_tw[0].r + scratch[1].i * scratch_tw[0].i;
        scratch_out[1].i = scratch[1].i * scratch_tw[0].r - scratch[1].r * scratch_tw[0].i;

        scratch_out[2].r = scratch[2].r * scratch_tw[1].r + scratch[2].i * scratch_tw[1].i;
        scratch_out[2].i = scratch[2].i * scratch_tw[1].r - scratch[2].r * scratch_tw[1].i;

        scratch_out[3].r = scratch[3].r * scratch_tw[2].r + scratch[3].i * scratch_tw[2].i;
        scratch_out[3].i = scratch[3].i * scratch_tw[2].r - scratch[3].r * scratch_tw[2].i;

        // NE10_PRINT_Q2_VECTOR(scratch_out);

        // store
        Fout_r[0 * in_step    ] = scratch_out[0].r;
        Fout_r[0 * in_step + 1] = scratch_out[0].i;
        Fout_r[1 * in_step    ] = scratch_out[1].r;
        Fout_r[1 * in_step + 1] = scratch_out[1].i;
        Fout_r[2 * in_step    ] = scratch_out[2].r;
        Fout_r[2 * in_step + 1] = scratch_out[2].i;
        Fout_r[3 * in_step    ] = scratch_out[3].r;
        Fout_r[3 * in_step + 1] = scratch_out[3].i;

        // update pointers
        Fin_r  += 2;
        Fout_r += 2;
        Fin_b -= 2;
        twiddles ++;
    }
}

NE10_INLINE void ne10_radix4_r2c_with_twiddles_last_butterfly_c (ne10_float32_t *Fout_r,
        const ne10_float32_t *Fin_r,
        const ne10_int32_t out_step,
        const ne10_int32_t in_step,
        const ne10_fft_cpx_float32_t *twiddles)
{
    ne10_float32_t scratch_in [4];
    ne10_float32_t scratch_out[4];

    scratch_in[0] = Fin_r[0 * in_step];
    scratch_in[1] = Fin_r[1 * in_step];
    scratch_in[2] = Fin_r[2 * in_step];
    scratch_in[3] = Fin_r[3 * in_step];

    // NE10_PRINT_Q_VECTOR(scratch_in);

    NE10_FFT_R2C_4R_CC(scratch_out,scratch_in);

    // NE10_PRINT_Q_VECTOR(scratch_out);

    Fout_r[                   0] = scratch_out[0];
    Fout_r[                   1] = scratch_out[1];
    Fout_r[ (out_step << 1)    ] = scratch_out[2];
    Fout_r[ (out_step << 1) + 1] = scratch_out[3];
}

NE10_INLINE void ne10_radix4_c2r_with_twiddles_last_butterfly_c (ne10_float32_t *Fout_r,
        const ne10_float32_t *Fin_r,
        const ne10_int32_t out_step,
        const ne10_int32_t in_step,
        const ne10_fft_cpx_float32_t *twiddles)
{
    // inverse operation of ne10_radix4_r2c_with_twiddles_last_butterfly_c
    ne10_float32_t scratch_in [4];
    ne10_float32_t scratch_out[4];

    // load
    scratch_in[0] = Fin_r[                   0];
    scratch_in[1] = Fin_r[                   1];
    scratch_in[2] = Fin_r[ (out_step << 1)    ];
    scratch_in[3] = Fin_r[ (out_step << 1) + 1];

    // NE10_PRINT_Q_VECTOR(scratch_in);

    NE10_FFT_C2R_CC_4R(scratch_out,scratch_in);

    // NE10_PRINT_Q_VECTOR(scratch_out);

    // store
    Fout_r[0 * in_step] = scratch_out[0];
    Fout_r[1 * in_step] = scratch_out[1];
    Fout_r[2 * in_step] = scratch_out[2];
    Fout_r[3 * in_step] = scratch_out[3];
}

NE10_INLINE void ne10_radix4_r2c_with_twiddles_c (ne10_fft_cpx_float32_t *Fout,
        const ne10_fft_cpx_float32_t *Fin,
        const ne10_int32_t fstride,
        const ne10_int32_t mstride,
        const ne10_int32_t nfft,
        const ne10_fft_cpx_float32_t *twiddles)
{
    ne10_int32_t f_count;
    const ne10_int32_t in_step = nfft >> 2;
    const ne10_int32_t out_step = mstride;

    const ne10_float32_t *Fin_r  = (ne10_float32_t*) Fin;
    ne10_float32_t *Fout_r = (ne10_float32_t*) Fout;
    const ne10_fft_cpx_float32_t *tw;

    Fout_r ++;
    Fin_r ++;

    for (f_count = fstride; f_count; f_count --)
    {
        tw = twiddles;

        // first butterfly
        ne10_radix4_r2c_with_twiddles_first_butterfly_c (Fout_r, Fin_r, out_step, in_step, tw);

        tw ++;
        Fin_r ++;
        Fout_r ++;

        // other butterfly
        ne10_radix4_r2c_with_twiddles_other_butterfly_c (Fout_r, Fin_r, out_step, in_step, tw);

        // update Fin_r, Fout_r, twiddles
        tw     +=     ( (out_step >> 1) - 1);
        Fin_r  += 2 * ( (out_step >> 1) - 1);
        Fout_r += 2 * ( (out_step >> 1) - 1);

        // last butterfly
        ne10_radix4_r2c_with_twiddles_last_butterfly_c (Fout_r, Fin_r, out_step, in_step, tw);
        tw ++;
        Fin_r ++;
        Fout_r ++;

        Fout_r += 3 * out_step;
    } // f_count
}

NE10_INLINE void ne10_radix4_c2r_with_twiddles_c (ne10_fft_cpx_float32_t *Fout,
        const ne10_fft_cpx_float32_t *Fin,
        const ne10_int32_t fstride,
        const ne10_int32_t mstride,
        const ne10_int32_t nfft,
        const ne10_fft_cpx_float32_t *twiddles)
{
    ne10_int32_t f_count;
    const ne10_int32_t in_step = nfft >> 2;
    const ne10_int32_t out_step = mstride;

    const ne10_float32_t *Fin_r  = (ne10_float32_t*) Fin;
          ne10_float32_t *Fout_r = (ne10_float32_t*) Fout;
    const ne10_fft_cpx_float32_t *tw;

    for (f_count = fstride; f_count; f_count --)
    {
        tw = twiddles;

        // first butterfly
        ne10_radix4_c2r_with_twiddles_first_butterfly_c (Fout_r, Fin_r, out_step, in_step, tw);

        tw ++;
        Fin_r  ++;
        Fout_r ++;

        // other butterfly
        ne10_radix4_c2r_with_twiddles_other_butterfly_c (Fout_r, Fin_r, out_step, in_step, tw);

        // update Fin_r, Fout_r, twiddles
        tw     +=     ( (out_step >> 1) - 1);
        Fin_r  += 2 * ( (out_step >> 1) - 1);
        Fout_r += 2 * ( (out_step >> 1) - 1);

        // last butterfly
        ne10_radix4_c2r_with_twiddles_last_butterfly_c (Fout_r, Fin_r, out_step, in_step, tw);
        tw ++;
        Fin_r  ++;
        Fout_r ++;

        Fin_r += 3 * out_step;
    } // f_count
}

NE10_INLINE void ne10_mixed_radix_r2c_butterfly_float32_c (
    ne10_fft_cpx_float32_t * Fout,
    const ne10_fft_cpx_float32_t * Fin,
    const ne10_int32_t * factors,
    const ne10_fft_cpx_float32_t * twiddles,
    ne10_fft_cpx_float32_t * buffer)
{
    // PRINT_POINTERS_INFO(Fin,Fout,buffer,twiddles);

    ne10_int32_t fstride, mstride, nfft;
    ne10_int32_t radix;
    ne10_int32_t stage_count;

    // init fstride, mstride, radix, nfft
    stage_count = factors[0];
    fstride     = factors[1];
    mstride     = factors[ (stage_count << 1) - 1 ];
    radix       = factors[  stage_count << 1      ];
    nfft        = radix * fstride;

    // PRINT_STAGE_INFO;

    if (stage_count % 2 == 0)
    {
        ne10_swap_ptr (buffer, Fout);
    }

    // the first stage
    if (radix == 8)   // length of FFT is 2^n (n is odd)
    {
        // PRINT_POINTERS_INFO(Fin,Fout,buffer,twiddles);
        ne10_radix8_r2c_c (Fout, Fin, fstride, mstride, nfft);
    }
    else if (radix == 4)   // length of FFT is 2^n (n is even)
    {
        // PRINT_POINTERS_INFO(Fin,Fout,buffer,twiddles);
        ne10_radix4_r2c_c (Fout, Fin, fstride, mstride, nfft);
    }
    // end of first stage

    // others
    for (; fstride > 1;)
    {
        fstride >>= 2;
        ne10_swap_ptr (buffer, Fout);

        // PRINT_POINTERS_INFO(Fin,Fout,buffer,twiddles);
        ne10_radix4_r2c_with_twiddles_c (Fout, buffer, fstride, mstride, nfft, twiddles);
        twiddles += 3 * mstride;
        mstride <<= 2;

    } // other stage
}

NE10_INLINE void ne10_mixed_radix_c2r_butterfly_float32_c (
    ne10_fft_cpx_float32_t * Fout,
    const ne10_fft_cpx_float32_t * Fin,
    const ne10_int32_t * factors,
    const ne10_fft_cpx_float32_t * twiddles,
    ne10_fft_cpx_float32_t * buffer)
{
    // PRINT_POINTERS_INFO(Fin,Fout,buffer,twiddles);

    ne10_int32_t fstride, mstride, nfft;
    ne10_int32_t radix;
    ne10_int32_t stage_count;

    // init fstride, mstride, radix, nfft
    stage_count = factors[0];
    fstride     = factors[1];
    mstride     = factors[ (stage_count << 1) - 1 ];
    radix       = factors[  stage_count << 1      ];
    nfft        = radix * fstride;

    // fstride, mstride for the last stage
    fstride = 1;
    mstride = nfft >> 2;
    // PRINT_STAGE_INFO;

    if (stage_count % 2 == 1)
    {
        ne10_swap_ptr (buffer, Fout);
    }

    // last butterfly -- inversed
    if (stage_count > 1)
    {
        twiddles -= 3 * mstride;
        // PRINT_STAGE_INFO;
        // PRINT_POINTERS_INFO(Fin,Fout,buffer,twiddles);
        ne10_radix4_c2r_with_twiddles_c (buffer, Fin, fstride, mstride, nfft, twiddles);
        fstride <<= 2;
        mstride >>= 2;
        stage_count --;
    }

    // others but the last stage
    for (; stage_count > 1;)
    {
        twiddles -= 3 * mstride;
        // PRINT_STAGE_INFO;
        // PRINT_POINTERS_INFO(Fin,Fout,buffer,twiddles);
        ne10_radix4_c2r_with_twiddles_c (Fout, buffer, fstride, mstride, nfft, twiddles);
        fstride <<= 2;
        mstride >>= 2;
        stage_count --;
        ne10_swap_ptr (buffer, Fout);
    } // other stage

    // first stage -- inversed
    if (radix == 8)   // length of FFT is 2^n (n is odd)
    {
        // PRINT_STAGE_INFO;
        // PRINT_POINTERS_INFO(Fin,Fout,buffer,twiddles);
        ne10_radix8_c2r_c (Fout, buffer, fstride, mstride, nfft);
    }
    else if (radix == 4)   // length of FFT is 2^n (n is even)
    {
        // PRINT_STAGE_INFO;
        // PRINT_POINTERS_INFO(Fin,Fout,buffer,twiddles);
        ne10_radix4_c2r_c (Fout, buffer, fstride, mstride, nfft);
    }
}

/**
 * @brief User-callable function to create a configuration structure for the R2C/C2R FFT/IFFT.
 * @param[in]   nfft             length of FFT
 * @retval      st               pointer to the FFT configuration memory, allocated with malloc.
 *
 * This function allocates and initialises an ne10_fft_r2c_cfg_float32_t configuration structure for the
 * real-to-complex and complex-to-real FFT/IFFT. As part of this, it reserves a buffer used internally
 * by the FFT algorithm, factors the length of the FFT into simpler chunks, and generates a "twiddle
 * table" of coefficients used in the FFT "butterfly" calculations.
 */
ne10_fft_r2c_cfg_float32_t ne10_fft_alloc_r2c_float32 (ne10_int32_t nfft)
{
    ne10_fft_r2c_cfg_float32_t st = NULL;
    ne10_int32_t ncfft = nfft >> 1;
    ne10_int32_t result;

    ne10_uint32_t memneeded =   sizeof (ne10_fft_r2c_state_float32_t)
                              + sizeof (ne10_fft_cpx_float32_t) * nfft              /* buffer*/
                              + sizeof (ne10_int32_t) * (NE10_MAXFACTORS * 2)       /* r_factors */
                              + sizeof (ne10_int32_t) * (NE10_MAXFACTORS * 2)       /* r_factors_neon */
                              + sizeof (ne10_fft_cpx_float32_t) * nfft              /* r_twiddles */
                              + sizeof (ne10_fft_cpx_float32_t) * nfft/4            /* r_twiddles_neon */
                              + sizeof (ne10_fft_cpx_float32_t) * (12 + nfft/32*12) /* r_super_twiddles_neon */
                              + NE10_FFT_BYTE_ALIGNMENT;     /* 64-bit alignment*/

    st = (ne10_fft_r2c_cfg_float32_t) NE10_MALLOC (memneeded);

    if (!st)
    {
        return st;
    }

    ne10_int32_t i,j;
    ne10_fft_cpx_float32_t *tw;
    const ne10_float32_t pi = NE10_PI;
    ne10_float32_t phase1;

    st->nfft = nfft;

    uintptr_t address = (uintptr_t) st + sizeof (ne10_fft_r2c_state_float32_t);
    NE10_BYTE_ALIGNMENT (address, NE10_FFT_BYTE_ALIGNMENT);

    st->buffer = (ne10_fft_cpx_float32_t*) address;
    st->r_twiddles = st->buffer + nfft;
    st->r_factors = (ne10_int32_t*) (st->r_twiddles + nfft);
    st->r_twiddles_neon = (ne10_fft_cpx_float32_t*) (st->r_factors + (NE10_MAXFACTORS * 2));
    st->r_factors_neon = (ne10_int32_t*) (st->r_twiddles_neon + nfft/4);
    st->r_super_twiddles_neon = (ne10_fft_cpx_float32_t*) (st->r_factors_neon + (NE10_MAXFACTORS * 2));

    if (nfft<16)
    {
        return st;
    }

    // factors and twiddles for rfft C
    ne10_factor (nfft, st->r_factors, NE10_FACTOR_EIGHT_FIRST_STAGE);

    // backward twiddles pointers
    st->r_twiddles_backward = ne10_fft_generate_twiddles_float32 (st->r_twiddles, st->r_factors, nfft);

    // factors and twiddles for rfft neon
    result = ne10_factor (nfft/4, st->r_factors_neon, NE10_FACTOR_EIGHT_FIRST_STAGE);
    if (result == NE10_ERR)
    {
        return st;
    }

    // Twiddle table is transposed here to improve cache access performance.
    st->r_twiddles_neon_backward = ne10_fft_generate_twiddles_transposed_float32 (
        st->r_twiddles_neon,
        st->r_factors_neon,
        nfft/4);

    // nfft/4 x 4
    tw = st->r_super_twiddles_neon;
    for (i = 1; i < 4; i ++)
    {
        for (j = 0; j < 4; j++)
        {
            phase1 = - 2 * pi * ( (ne10_float32_t) (i * j) / nfft);
            tw[4*i-4+j].r = (ne10_float32_t) cos (phase1);
            tw[4*i-4+j].i = (ne10_float32_t) sin (phase1);
        }
    }

    ne10_int32_t k,s;
    // [nfft/32] x [3] x [4]
    //     k        s     j
    for (k=1; k<nfft/32; k++)
    {
        // transposed
        for (s = 1; s < 4; s++)
        {
            for (j = 0; j < 4; j++)
            {
                phase1 = - 2 * pi * ( (ne10_float32_t) ((k*4+j) * s) / nfft);
                tw[12*k+j+4*(s-1)].r = (ne10_float32_t) cos (phase1);
                tw[12*k+j+4*(s-1)].i = (ne10_float32_t) sin (phase1);
            }
        }
    }
    return st;
}

/**
 * @brief Mixed radix-2/4 real-to-complex C FFT of single precision floating point data.
 * @param[out]  *fout            pointer to the output buffer
 * @param[in]   *fin             pointer to the input buffer
 * @param[in]   cfg              pointer to the configuration structure
 *
 * The function implements a mixed radix-2/4 real-to-complex FFT, supporting input lengths of
 * the form 2^N (N > 0). This is an out-of-place algorithm. For usage information, please check
 * test/test_suite_fft_float32.c.
 */
void ne10_fft_r2c_1d_float32_c (ne10_fft_cpx_float32_t *fout,
                                ne10_float32_t *fin,
                                ne10_fft_r2c_cfg_float32_t cfg)
{
    ne10_fft_cpx_float32_t * tmpbuf = cfg->buffer;

    switch(cfg->nfft)
    {
        case 8:
            ne10_radix8_r2c_c( (ne10_fft_cpx_float32_t*) fout, ( ne10_fft_cpx_float32_t*) fin,1,1,8);
            break;
        default:
            ne10_mixed_radix_r2c_butterfly_float32_c (
                    fout,
                    (ne10_fft_cpx_float32_t*) fin,
                    cfg->r_factors,
                    cfg->r_twiddles,
                    tmpbuf);
            break;
    }

    fout[0].r = fout[0].i;
    fout[0].i = 0.0f;
    fout[(cfg->nfft) >> 1].i = 0.0f;
}

/**
 * @brief Mixed radix-2/4 complex-to-real C IFFT of single precision floating point data.
 * @param[out]  *fout            pointer to the output buffer
 * @param[in]   *fin             pointer to the input buffer
 * @param[in]   cfg              pointer to the configuration structure
 *
 * The function implements a mixed radix-2/4 complex-to-real IFFT, supporting input lengths of
 * the form 2^N (N > 0). This is an out-of-place algorithm. For usage information, please check
 * test/test_suite_fft_float32.c.
 */
void ne10_fft_c2r_1d_float32_c (ne10_float32_t *fout,
                                ne10_fft_cpx_float32_t *fin,
                                ne10_fft_r2c_cfg_float32_t cfg)
{
    ne10_fft_cpx_float32_t * tmpbuf = cfg->buffer;

    fin[0].i = fin[0].r;
    fin[0].r = 0.0f;
    switch(cfg->nfft)
    {
        case 8:
            ne10_radix8_c2r_c( (ne10_fft_cpx_float32_t*) fout, ( ne10_fft_cpx_float32_t*) &fin[0].i,1,1,8);
            break;
        default:
            ne10_mixed_radix_c2r_butterfly_float32_c (
                    (ne10_fft_cpx_float32_t*)fout,
                    (ne10_fft_cpx_float32_t*)&fin[0].i, // first real is moved to first image
                    cfg->r_factors,
                    cfg->r_twiddles_backward,
                    tmpbuf);
            break;
    }
    fin[0].r = fin[0].i;
    fin[0].i = 0.0f;
}

/**
 * @} end of R2C_FFT_IFFT group
 */

#endif // NE10_UNROLL_LEVEL
