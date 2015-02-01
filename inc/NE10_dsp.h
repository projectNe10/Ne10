/*
 *  Copyright 2012-15 ARM Limited and Contributors.
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
 * NE10 Library : inc/NE10_dsp.h
 */


#include "NE10_types.h"

#ifndef NE10_DSP_H
#define NE10_DSP_H

#ifdef __cplusplus
extern "C" {
#endif

///////////////////////////
// function prototypes:
///////////////////////////

    /* fft functions*/

    /* function pointers*/
    extern ne10_fft_cfg_float32_t (*ne10_fft_alloc_c2c_float32) (ne10_int32_t nfft);
    extern ne10_fft_cfg_int32_t (*ne10_fft_alloc_c2c_int32) (ne10_int32_t nfft);

    extern void (*ne10_fft_c2c_1d_float32) (ne10_fft_cpx_float32_t *fout,
                                            ne10_fft_cpx_float32_t *fin,
                                            ne10_fft_cfg_float32_t cfg,
                                            ne10_int32_t inverse_fft);

    extern void (*ne10_fft_r2c_1d_float32) (ne10_fft_cpx_float32_t *fout,
                                            ne10_float32_t *fin,
                                            ne10_fft_r2c_cfg_float32_t cfg);

    extern void (*ne10_fft_c2r_1d_float32) (ne10_float32_t *fout,
                                            ne10_fft_cpx_float32_t *fin,
                                            ne10_fft_r2c_cfg_float32_t cfg);

    extern void (*ne10_fft_c2c_1d_int32) (ne10_fft_cpx_int32_t *fout,
                                          ne10_fft_cpx_int32_t *fin,
                                          ne10_fft_cfg_int32_t cfg,
                                          ne10_int32_t inverse_fft,
                                          ne10_int32_t scaled_flag);

    extern void (*ne10_fft_r2c_1d_int32) (ne10_fft_cpx_int32_t *fout,
                                          ne10_int32_t *fin,
                                          ne10_fft_r2c_cfg_int32_t cfg,
                                          ne10_int32_t scaled_flag);

    extern void (*ne10_fft_c2r_1d_int32) (ne10_int32_t *fout,
                                          ne10_fft_cpx_int32_t *fin,
                                          ne10_fft_r2c_cfg_int32_t cfg,
                                          ne10_int32_t scaled_flag);

    extern void (*ne10_fft_c2c_1d_int16) (ne10_fft_cpx_int16_t *fout,
                                          ne10_fft_cpx_int16_t *fin,
                                          ne10_fft_cfg_int16_t cfg,
                                          ne10_int32_t inverse_fft,
                                          ne10_int32_t scaled_flag);

    extern void (*ne10_fft_r2c_1d_int16) (ne10_fft_cpx_int16_t *fout,
                                          ne10_int16_t *fin,
                                          ne10_fft_r2c_cfg_int16_t cfg,
                                          ne10_int32_t scaled_flag);

    extern void (*ne10_fft_c2r_1d_int16) (ne10_int16_t *fout,
                                          ne10_fft_cpx_int16_t *fin,
                                          ne10_fft_r2c_cfg_int16_t cfg,
                                          ne10_int32_t scaled_flag);

    /* init functions*/
    extern ne10_fft_cfg_int16_t ne10_fft_alloc_c2c_int16 (ne10_int32_t nfft);

    extern ne10_fft_r2c_cfg_float32_t ne10_fft_alloc_r2c_float32 (ne10_int32_t nfft);
    extern ne10_fft_r2c_cfg_int32_t ne10_fft_alloc_r2c_int32 (ne10_int32_t nfft);
    extern ne10_fft_r2c_cfg_int16_t ne10_fft_alloc_r2c_int16 (ne10_int32_t nfft);

    /* C version*/
    extern ne10_fft_cfg_float32_t ne10_fft_alloc_c2c_float32_c (ne10_int32_t nfft);
    extern ne10_fft_cfg_int32_t ne10_fft_alloc_c2c_int32_c (ne10_int32_t nfft);

    extern void ne10_fft_c2c_1d_float32_c (ne10_fft_cpx_float32_t *fout,
                                           ne10_fft_cpx_float32_t *fin,
                                           ne10_fft_cfg_float32_t cfg,
                                           ne10_int32_t inverse_fft);

    extern void ne10_fft_r2c_1d_float32_c (ne10_fft_cpx_float32_t *fout,
                                           ne10_float32_t *fin,
                                           ne10_fft_r2c_cfg_float32_t cfg);

    extern void ne10_fft_c2r_1d_float32_c (ne10_float32_t *fout,
                                           ne10_fft_cpx_float32_t *fin,
                                           ne10_fft_r2c_cfg_float32_t cfg);

    extern void ne10_fft_c2c_1d_int32_c (ne10_fft_cpx_int32_t *fout,
                                         ne10_fft_cpx_int32_t *fin,
                                         ne10_fft_cfg_int32_t cfg,
                                         ne10_int32_t inverse_fft,
                                         ne10_int32_t scaled_flag);

    extern void ne10_fft_r2c_1d_int32_c (ne10_fft_cpx_int32_t *fout,
                                         ne10_int32_t *fin,
                                         ne10_fft_r2c_cfg_int32_t cfg,
                                         ne10_int32_t scaled_flag);

    extern void ne10_fft_c2r_1d_int32_c (ne10_int32_t *fout,
                                         ne10_fft_cpx_int32_t *fin,
                                         ne10_fft_r2c_cfg_int32_t cfg,
                                         ne10_int32_t scaled_flag);

    extern void ne10_fft_c2c_1d_int16_c (ne10_fft_cpx_int16_t *fout,
                                         ne10_fft_cpx_int16_t *fin,
                                         ne10_fft_cfg_int16_t cfg,
                                         ne10_int32_t inverse_fft,
                                         ne10_int32_t scaled_flag);

    extern void ne10_fft_r2c_1d_int16_c (ne10_fft_cpx_int16_t *fout,
                                         ne10_int16_t *fin,
                                         ne10_fft_r2c_cfg_int16_t cfg,
                                         ne10_int32_t scaled_flag);

    extern void ne10_fft_c2r_1d_int16_c (ne10_int16_t *fout,
                                         ne10_fft_cpx_int16_t *fin,
                                         ne10_fft_r2c_cfg_int16_t cfg,
                                         ne10_int32_t scaled_flag);


    /* NEON version*/
    extern ne10_fft_cfg_float32_t ne10_fft_alloc_c2c_float32_neon (ne10_int32_t nfft);
    extern ne10_fft_cfg_int32_t ne10_fft_alloc_c2c_int32_neon (ne10_int32_t nfft);

    extern void ne10_fft_c2c_1d_float32_neon (ne10_fft_cpx_float32_t *fout,
            ne10_fft_cpx_float32_t *fin,
            ne10_fft_cfg_float32_t cfg,
            ne10_int32_t inverse_fft);

    extern void ne10_fft_r2c_1d_float32_neon (ne10_fft_cpx_float32_t *fout,
            ne10_float32_t *fin,
            ne10_fft_r2c_cfg_float32_t cfg);

    extern void ne10_fft_c2r_1d_float32_neon (ne10_float32_t *fout,
            ne10_fft_cpx_float32_t *fin,
            ne10_fft_r2c_cfg_float32_t cfg);

    extern void ne10_fft_c2c_1d_int32_neon (ne10_fft_cpx_int32_t *fout,
                                            ne10_fft_cpx_int32_t *fin,
                                            ne10_fft_cfg_int32_t cfg,
                                            ne10_int32_t inverse_fft,
                                            ne10_int32_t scaled_flag);

    extern void ne10_fft_r2c_1d_int32_neon (ne10_fft_cpx_int32_t *fout,
                                            ne10_int32_t *fin,
                                            ne10_fft_r2c_cfg_int32_t cfg,
                                            ne10_int32_t scaled_flag);

    extern void ne10_fft_c2r_1d_int32_neon (ne10_int32_t *fout,
                                            ne10_fft_cpx_int32_t *fin,
                                            ne10_fft_r2c_cfg_int32_t cfg,
                                            ne10_int32_t scaled_flag);

    extern void ne10_fft_c2c_1d_int16_neon (ne10_fft_cpx_int16_t *fout,
                                            ne10_fft_cpx_int16_t *fin,
                                            ne10_fft_cfg_int16_t cfg,
                                            ne10_int32_t inverse_fft,
                                            ne10_int32_t scaled_flag);

    extern void ne10_fft_r2c_1d_int16_neon (ne10_fft_cpx_int16_t *fout,
                                            ne10_int16_t *fin,
                                            ne10_fft_r2c_cfg_int16_t cfg,
                                            ne10_int32_t scaled_flag);

    extern void ne10_fft_c2r_1d_int16_neon (ne10_int16_t *fout,
                                            ne10_fft_cpx_int16_t *fin,
                                            ne10_fft_r2c_cfg_int16_t cfg,
                                            ne10_int32_t scaled_flag);

    /* fir functions*/

    /* function pointers*/
    extern void (*ne10_fir_float) (const ne10_fir_instance_f32_t * S,
                                   ne10_float32_t * pSrc,
                                   ne10_float32_t * pDst,
                                   ne10_uint32_t blockSize);

    extern void (*ne10_fir_decimate_float) (const ne10_fir_decimate_instance_f32_t * S,
                                            ne10_float32_t * pSrc,
                                            ne10_float32_t * pDst,
                                            ne10_uint32_t blockSize);

    extern void (*ne10_fir_interpolate_float) (const ne10_fir_interpolate_instance_f32_t * S,
            ne10_float32_t * pSrc,
            ne10_float32_t * pDst,
            ne10_uint32_t blockSize);

    extern void (*ne10_fir_lattice_float) (const ne10_fir_lattice_instance_f32_t * S,
                                           ne10_float32_t * pSrc,
                                           ne10_float32_t * pDst,
                                           ne10_uint32_t blockSize);

    extern void (*ne10_fir_sparse_float) (ne10_fir_sparse_instance_f32_t * S,
                                          ne10_float32_t * pSrc,
                                          ne10_float32_t * pDst,
                                          ne10_float32_t * pScratchIn,
                                          ne10_uint32_t blockSize);


    /* init functions*/
    extern ne10_result_t ne10_fir_init_float (ne10_fir_instance_f32_t * S,
            ne10_uint16_t numTaps,
            ne10_float32_t * pCoeffs,
            ne10_float32_t * pState,
            ne10_uint32_t blockSize);

    extern ne10_result_t ne10_fir_decimate_init_float (ne10_fir_decimate_instance_f32_t * S,
            ne10_uint16_t numTaps,
            ne10_uint8_t M,
            ne10_float32_t * pCoeffs,
            ne10_float32_t * pState,
            ne10_uint32_t blockSize);

    extern ne10_result_t ne10_fir_interpolate_init_float (ne10_fir_interpolate_instance_f32_t * S,
            ne10_uint8_t L,
            ne10_uint16_t numTaps,
            ne10_float32_t * pCoeffs,
            ne10_float32_t * pState,
            ne10_uint32_t blockSize);

    extern ne10_result_t ne10_fir_lattice_init_float (ne10_fir_lattice_instance_f32_t * S,
            ne10_uint16_t numStages,
            ne10_float32_t * pCoeffs,
            ne10_float32_t * pState);

    extern ne10_result_t ne10_fir_sparse_init_float (ne10_fir_sparse_instance_f32_t * S,
            ne10_uint16_t numTaps,
            ne10_float32_t * pCoeffs,
            ne10_float32_t * pState,
            ne10_int32_t * pTapDelay,
            ne10_uint16_t maxDelay,
            ne10_uint32_t blockSize);

    /* C version*/
    extern void ne10_fir_float_c (const ne10_fir_instance_f32_t * S,
                                  ne10_float32_t * pSrc,
                                  ne10_float32_t * pDst,
                                  ne10_uint32_t blockSize);

    extern void ne10_fir_decimate_float_c (const ne10_fir_decimate_instance_f32_t * S,
                                           ne10_float32_t * pSrc,
                                           ne10_float32_t * pDst,
                                           ne10_uint32_t blockSize);

    extern void ne10_fir_interpolate_float_c (const ne10_fir_interpolate_instance_f32_t * S,
            ne10_float32_t * pSrc,
            ne10_float32_t * pDst,
            ne10_uint32_t blockSize);

    extern void ne10_fir_lattice_float_c (const ne10_fir_lattice_instance_f32_t * S,
                                          ne10_float32_t * pSrc,
                                          ne10_float32_t * pDst,
                                          ne10_uint32_t blockSize);

    extern void ne10_fir_sparse_float_c (ne10_fir_sparse_instance_f32_t * S,
                                         ne10_float32_t * pSrc,
                                         ne10_float32_t * pDst,
                                         ne10_float32_t * pScratchIn,
                                         ne10_uint32_t blockSize);


    /* NEON version*/

    /**
     * @addtogroup FIR
     * @{
     */
#ifdef ENABLE_NE10_FIR_FLOAT_NEON
    extern void ne10_fir_float_neon (const ne10_fir_instance_f32_t * S,
                                     ne10_float32_t * pSrc,
                                     ne10_float32_t * pDst,
                                     ne10_uint32_t blockSize)
    asm ("ne10_fir_float_neon");
#endif // ENABLE_NE10_FIR_FLOAT_NEON
    /** @} */ //end of FIR group

    /**
     * @addtogroup FIR_Decimate
     * @{
     */
#ifdef ENABLE_NE10_FIR_DECIMATE_FLOAT_NEON
    extern void ne10_fir_decimate_float_neon (const ne10_fir_decimate_instance_f32_t * S,
            ne10_float32_t *pSrc,
            ne10_float32_t *pDst,
            ne10_uint32_t blockSize) asm ("ne10_fir_decimate_float_neon");

    /** @} */ //end of FIR_decimate group
#endif // ENABLE_NE10_FIR_DECIMATE_FLOAT_NEON
    /**
     * @addtogroup FIR_Interpolate
     * @{
     */
#ifdef ENABLE_NE10_FIR_INTERPOLATE_FLOAT_NEON
    extern void ne10_fir_interpolate_float_neon (const ne10_fir_interpolate_instance_f32_t * S,
            ne10_float32_t *pSrc,
            ne10_float32_t *pDst,
            ne10_uint32_t blockSize) asm ("ne10_fir_interpolate_float_neon");
#endif // ENABLE_NE10_FIR_INTERPOLATE_FLOAT_NEON
    /** @} */ //end of FIR_interpolate group

    /**
     * @addtogroup FIR_Lattice
     * @{
     */
#ifdef ENABLE_NE10_FIR_LATTICE_FLOAT_NEON
    extern void ne10_fir_lattice_float_neon (const ne10_fir_lattice_instance_f32_t * S,
            ne10_float32_t * pSrc,
            ne10_float32_t * pDst,
            ne10_uint32_t blockSize) asm ("ne10_fir_lattice_float_neon");
#endif // ENABLE_NE10_FIR_LATTICE_FLOAT_NEON
    /** @} */ //end of FIR_Lattice group

    /**
     * @addtogroup FIR_Sparse
     * @{
     */
#ifdef ENABLE_NE10_FIR_SPARSE_FLOAT_NEON
    extern void ne10_fir_sparse_float_neon (ne10_fir_sparse_instance_f32_t * S,
                                            ne10_float32_t * pSrc,
                                            ne10_float32_t * pDst,
                                            ne10_float32_t * pScratch,
                                            ne10_uint32_t blockSize)
    asm ("ne10_fir_sparse_float_neon");
#endif // ENABLE_NE10_FIR_SPARSE_FLOAT_NEON
    /** @} */ //end of FIR_sparse group


    /* iir functions*/

    /* function pointers*/
    extern void (*ne10_iir_lattice_float) (const ne10_iir_lattice_instance_f32_t * S,
                                           ne10_float32_t * pSrc,
                                           ne10_float32_t * pDst,
                                           ne10_uint32_t blockSize);

    /* init functions*/
    extern ne10_result_t ne10_iir_lattice_init_float (ne10_iir_lattice_instance_f32_t * S,
            ne10_uint16_t numStages,
            ne10_float32_t * pkCoeffs,
            ne10_float32_t * pvCoeffs,
            ne10_float32_t * pState,
            ne10_uint32_t blockSize);


    /* C version*/
    extern void ne10_iir_lattice_float_c (const ne10_iir_lattice_instance_f32_t * S,
                                          ne10_float32_t * pSrc,
                                          ne10_float32_t * pDst,
                                          ne10_uint32_t blockSize);

    /* NEON version*/

    /**
     * @addtogroup IIR_Lattice
     * @{
     */
#ifdef ENABLE_NE10_IIR_LATTICE_FLOAT_NEON
    extern void ne10_iir_lattice_float_neon (const ne10_iir_lattice_instance_f32_t * S,
            ne10_float32_t * pSrc,
            ne10_float32_t * pDst,
            ne10_uint32_t blockSize) asm ("ne10_iir_lattice_float_neon");
#endif // ENABLE_NE10_IIR_LATTICE_FLOAT_NEON
    /** @} */ //end of IIR_Lattice group

#ifdef __cplusplus
}
#endif

#endif
