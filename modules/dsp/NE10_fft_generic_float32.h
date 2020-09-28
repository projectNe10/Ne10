/*
 *  Copyright 2014-16 ARM Limited and Contributors.
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
 * NE10 Library : dsp/NE10_fft_generic_float32.h
 */

#ifndef NE10_FFT_GENERIC_FLOAT32_H
#define NE10_FFT_GENERIC_FLOAT32_H

#include "NE10_types.h"
#include "NE10_macros.h"
#include "NE10_fft.h"

///////////////////////////////
// Multiply input with twiddles
///////////////////////////////
static inline void FFT2_MUL_TW (ne10_fft_cpx_float32_t scratch_out[2],
        const ne10_fft_cpx_float32_t scratch_in[2],
        const ne10_fft_cpx_float32_t scratch_tw[1])
{
    scratch_out[0] = scratch_in[0];
    NE10_CPX_MUL_F32 (scratch_out[1], scratch_in[1], scratch_tw[0]);
}

static inline void FFT3_MUL_TW (ne10_fft_cpx_float32_t scratch_out[3],
        const ne10_fft_cpx_float32_t scratch_in[3],
        const ne10_fft_cpx_float32_t scratch_tw[2])
{
    FFT2_MUL_TW (scratch_out, scratch_in, scratch_tw);
    NE10_CPX_MUL_F32 (scratch_out[2], scratch_in[2], scratch_tw[1]);
}

static inline void FFT4_MUL_TW (ne10_fft_cpx_float32_t scratch_out[4],
        const ne10_fft_cpx_float32_t scratch_in[4],
        const ne10_fft_cpx_float32_t scratch_tw[3])
{
    FFT3_MUL_TW (scratch_out, scratch_in, scratch_tw);
    NE10_CPX_MUL_F32 (scratch_out[3], scratch_in[3], scratch_tw[2]);
}

static inline void FFT8_FCU (ne10_fft_cpx_float32_t out[8],
        const ne10_fft_cpx_float32_t in[8])
{
    ne10_fft_cpx_float32_t s[8];

    static const ne10_fft_cpx_float32_t TW_8[4] = {
        {  1.00000,  0.00000 },
        {  0.70711, -0.70711 },
        {  0.00000, -1.00000 },
        { -0.70711, -0.70711 },
    };

#define NE10_BUTTERFLY_INDEX_F32(OUT,IN,OUT_I,OUT_J,IN_I,IN_J) \
    do { \
        NE10_CPX_ADD (OUT[OUT_I],IN[IN_I],IN[IN_J]); \
        NE10_CPX_SUB (OUT[OUT_J],IN[IN_I],IN[IN_J]); \
    } while (0)

    // STAGE - 1
    // in -> s
    {
        NE10_BUTTERFLY_INDEX_F32 (s,in,0,4,0,4);
        NE10_BUTTERFLY_INDEX_F32 (s,in,1,5,1,5);
        NE10_BUTTERFLY_INDEX_F32 (s,in,2,6,2,6);
        NE10_BUTTERFLY_INDEX_F32 (s,in,3,7,3,7);
    }

    // STAGE - 2
    // s -> out
    {
        // TW
#define NE10_CPX_MUL_TW8_F32(OUT,TW_8_TABLE,OUT_I,TW_J) \
        do { \
            ne10_fft_cpx_float32_t TW_TMP = TW_8_TABLE[TW_J]; \
            NE10_CPX_MUL_F32 (OUT[OUT_I],OUT[OUT_I],TW_TMP); \
        } while (0)

        NE10_CPX_MUL_TW8_F32 (s,TW_8,4,0);
        NE10_CPX_MUL_TW8_F32 (s,TW_8,5,1);
        NE10_CPX_MUL_TW8_F32 (s,TW_8,6,2);
        NE10_CPX_MUL_TW8_F32 (s,TW_8,7,3);

        NE10_BUTTERFLY_INDEX_F32 (out,s,0,2,0,2);
        NE10_BUTTERFLY_INDEX_F32 (out,s,1,3,1,3);
        NE10_BUTTERFLY_INDEX_F32 (out,s,4,6,4,6);
        NE10_BUTTERFLY_INDEX_F32 (out,s,5,7,5,7);
    }
    // STAGE - 3
    // out -> s
    {
        // TW
        NE10_CPX_MUL_TW8_F32 (out,TW_8,2,0);
        NE10_CPX_MUL_TW8_F32 (out,TW_8,3,2);
        NE10_CPX_MUL_TW8_F32 (out,TW_8,6,0);
        NE10_CPX_MUL_TW8_F32 (out,TW_8,7,2);
#undef NE10_CPX_MUL_TW8_F32

        NE10_BUTTERFLY_INDEX_F32 (s,out,0,4,0,1);
        NE10_BUTTERFLY_INDEX_F32 (s,out,2,6,2,3);
        NE10_BUTTERFLY_INDEX_F32 (s,out,1,5,4,5);
        NE10_BUTTERFLY_INDEX_F32 (s,out,3,7,6,7);
    }

    out[0] = s[0];
    out[1] = s[1];
    out[2] = s[2];
    out[3] = s[3];
    out[4] = s[4];
    out[5] = s[5];
    out[6] = s[6];
    out[7] = s[7];
}

static inline void FFT5_MUL_TW (ne10_fft_cpx_float32_t scratch_out[5],
        const ne10_fft_cpx_float32_t scratch_in[5],
        const ne10_fft_cpx_float32_t scratch_tw[4])
{
    FFT4_MUL_TW (scratch_out, scratch_in, scratch_tw);
    NE10_CPX_MUL_F32 (scratch_out[4], scratch_in[4], scratch_tw[3]);
}

///////////////////
// FFT Kernel
// F: Forward
// C: Complex
// U: Unscaled
//////////////////
static inline void FFT2_FCU (ne10_fft_cpx_float32_t scratch_out[2],
        const ne10_fft_cpx_float32_t scratch_in[2])
{
    NE10_CPX_ADD (scratch_out[0], scratch_in[0], scratch_in[1]);
    NE10_CPX_SUB (scratch_out[1], scratch_in[0], scratch_in[1]);
}

static inline void FFT3_FCU (ne10_fft_cpx_float32_t Fout[3],
        const ne10_fft_cpx_float32_t Fin[3])
{
    ne10_fft_cpx_float32_t scratch[4];
    ne10_fft_cpx_float32_t scratch_in[3];

    scratch_in[0] = Fin[0];
    scratch_in[1] = Fin[1];
    scratch_in[2] = Fin[2];

    scratch[1] = scratch_in[1];
    scratch[2] = scratch_in[2];

    NE10_CPX_ADD (scratch[3], scratch[1], scratch[2]);
    NE10_CPX_SUB (scratch[0], scratch[1], scratch[2]);

    scratch_in[1].r = scratch_in[0].r - scratch[3].r * 0.5;
    scratch_in[1].i = scratch_in[0].i - scratch[3].i * 0.5;

    scratch[0].r *= -TW_3I_F32;
    scratch[0].i *= -TW_3I_F32;

    scratch_in[0].r += scratch[3].r;
    scratch_in[0].i += scratch[3].i;

    scratch_in[2].r = scratch_in[1].r + scratch[0].i;
    scratch_in[2].i = scratch_in[1].i - scratch[0].r;

    scratch_in[1].r -= scratch[0].i;
    scratch_in[1].i += scratch[0].r;

    Fout[0] = scratch_in[0];
    Fout[1] = scratch_in[1];
    Fout[2] = scratch_in[2];
}

static inline void FFT4_FCU (ne10_fft_cpx_float32_t scratch_out[4],
        const ne10_fft_cpx_float32_t scratch_in[4])
{
    ne10_fft_cpx_float32_t scratch[4];

    NE10_CPX_ADD (scratch[0], scratch_in[0], scratch_in[2]);
    NE10_CPX_SUB (scratch[1], scratch_in[0], scratch_in[2]);
    NE10_CPX_ADD (scratch[2], scratch_in[1], scratch_in[3]);
    NE10_CPX_SUB (scratch[3], scratch_in[1], scratch_in[3]);

    NE10_CPX_SUB (scratch_out[2], scratch[0], scratch[2]);
    NE10_CPX_ADD (scratch_out[0], scratch[0], scratch[2]);

    scratch_out[1].r = scratch[1].r + scratch[3].i;
    scratch_out[1].i = scratch[1].i - scratch[3].r;
    scratch_out[3].r = scratch[1].r - scratch[3].i;
    scratch_out[3].i = scratch[1].i + scratch[3].r;
}

static inline void FFT4_FCU_INPLACE (ne10_fft_cpx_float32_t scratch_out[4])
{
    ne10_fft_cpx_float32_t scratch[4];

    NE10_CPX_ADD (scratch[0], scratch_out[0], scratch_out[2]);
    NE10_CPX_SUB (scratch[1], scratch_out[0], scratch_out[2]);
    NE10_CPX_ADD (scratch[2], scratch_out[1], scratch_out[3]);
    NE10_CPX_SUB (scratch[3], scratch_out[1], scratch_out[3]);

    NE10_CPX_SUB (scratch_out[2], scratch[0], scratch[2]);
    NE10_CPX_ADD (scratch_out[0], scratch[0], scratch[2]);

    scratch_out[1].r = scratch[1].r + scratch[3].i;
    scratch_out[1].i = scratch[1].i - scratch[3].r;
    scratch_out[3].r = scratch[1].r - scratch[3].i;
    scratch_out[3].i = scratch[1].i + scratch[3].r;
}

static inline void FFT5_FCU (ne10_fft_cpx_float32_t Fout[5],
        const ne10_fft_cpx_float32_t Fin[5])
{
    ne10_fft_cpx_float32_t scratch[13], scratch_in[5];

    scratch_in[0] = Fin[0];
    scratch_in[1] = Fin[1];
    scratch_in[2] = Fin[2];
    scratch_in[3] = Fin[3];
    scratch_in[4] = Fin[4];

    scratch[0] = scratch_in[0];
    scratch[1] = scratch_in[1];
    scratch[2] = scratch_in[2];
    scratch[3] = scratch_in[3];
    scratch[4] = scratch_in[4];

    NE10_CPX_ADD (scratch[ 7], scratch[1], scratch[4]);
    NE10_CPX_SUB (scratch[10], scratch[1], scratch[4]);
    NE10_CPX_ADD (scratch[ 8], scratch[2], scratch[3]);
    NE10_CPX_SUB (scratch[ 9], scratch[2], scratch[3]);

    scratch_in[0].r += scratch[7].r + scratch[8].r;
    scratch_in[0].i += scratch[7].i + scratch[8].i;

    scratch[5].r = scratch[0].r
        + NE10_S_MUL (scratch[7].r, TW_5A_F32.r)
        + NE10_S_MUL (scratch[8].r, TW_5B_F32.r);
    scratch[5].i = scratch[0].i
        + NE10_S_MUL (scratch[7].i, TW_5A_F32.r)
        + NE10_S_MUL (scratch[8].i, TW_5B_F32.r);

    scratch[6].r = NE10_S_MUL (scratch[10].i, TW_5A_F32.i)
        + NE10_S_MUL (scratch[9].i, TW_5B_F32.i);
    scratch[6].i = -NE10_S_MUL (scratch[10].r, TW_5A_F32.i)
        - NE10_S_MUL (scratch[9].r, TW_5B_F32.i);

    NE10_CPX_SUB (scratch_in[1], scratch[5], scratch[6]);
    NE10_CPX_ADD (scratch_in[4], scratch[5], scratch[6]);

    scratch[11].r = scratch[0].r
        + NE10_S_MUL (scratch[7].r, TW_5B_F32.r)
        + NE10_S_MUL (scratch[8].r, TW_5A_F32.r);
    scratch[11].i = scratch[0].i
        + NE10_S_MUL (scratch[7].i, TW_5B_F32.r)
        + NE10_S_MUL (scratch[8].i, TW_5A_F32.r);

    scratch[12].r = -NE10_S_MUL (scratch[10].i, TW_5B_F32.i)
        + NE10_S_MUL (scratch[9].i, TW_5A_F32.i);
    scratch[12].i = NE10_S_MUL (scratch[10].r, TW_5B_F32.i)
        - NE10_S_MUL (scratch[9].r, TW_5A_F32.i);

    NE10_CPX_ADD (scratch_in[2], scratch[11], scratch[12]);
    NE10_CPX_SUB (scratch_in[3], scratch[11], scratch[12]);

    Fout[0] = scratch_in[0];
    Fout[1] = scratch_in[1];
    Fout[2] = scratch_in[2];
    Fout[3] = scratch_in[3];
    Fout[4] = scratch_in[4];
}
#endif // NE10_FFT_GENERIC_FLOAT32_H
