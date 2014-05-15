/*
 *  Copyright 2012-14 ARM Limited
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
 * NE10 Library : dsp/NE10_rfft.c
 */

#include "NE10_types.h"
#include "NE10_dsp.h"

/**
 * @ingroup groupDSPs
 */

/**
 * @defgroup RFFT_RIFFT Real FFT
 *
 * \par
 * Complex FFT/IFFT typically assumes complex input and output. However many applications use real valued data in time domain.
 * Real FFT/IFFT efficiently process real valued sequences with the advantage of requirement of low memory and with less complexity.
 *
 * \par
 * This set of functions implements Real Fast Fourier Transforms(RFFT) and Real Inverse Fast Fourier Transform(RIFFT)
 * for floating-point data types.
 *
 *
 * \par Algorithm:
 *
 * <b>Real Fast Fourier Transform:</b>
 * \par
 * Real FFT of N-point is calculated using CFFT of N/2-point and Split RFFT process as shown below figure.
 * \par
 * \image html RFFT.gif "Real Fast Fourier Transform"
 * \par
 * The RFFT functions operate on blocks of input and output data and each call to the function processes
 * <code>fftLenR</code> samples through the transform.  <code>pSrc</code>  points to input array containing <code>fftLenR</code> values.
 * <code>pDst</code>  points to output array containing <code>2*fftLenR</code> values. \n
 * Input for real FFT is in the order of
 * <pre>{real[0], real[1], real[2], real[3], ..}</pre>
 * Output for real FFT is complex and are in the order of
 * <pre>{real(0), imag(0), real(1), imag(1), ...}</pre>
 *
 * <b>Real Inverse Fast Fourier Transform:</b>
 * \par
 * Real IFFT of N-point is calculated using Split RIFFT process and CFFT of N/2-point as shown below figure.
 * \par
 * \image html RIFFT.gif "Real Inverse Fast Fourier Transform"
 * \par
 * The RIFFT functions operate on blocks of input and output data and each call to the function processes
 * <code>2*fftLenR</code> samples through the transform.  <code>pSrc</code>  points to input array containing <code>2*fftLenR</code> values.
 * <code>pDst</code>  points to output array containing <code>fftLenR</code> values. \n
 * Input for real IFFT is complex and are in the order of
 * <pre>{real(0), imag(0), real(1), imag(1), ...}</pre>
 *  Output for real IFFT is real and in the order of
 * <pre>{real[0], real[1], real[2], real[3], ..}</pre>
 *
 * \par Lengths supported by the transform:
 * \par
 * Real FFT/IFFT supports the lengths [128, 512, 2048], as it internally uses CFFT/CIFFT.
 *
 * \par Instance Structure
 * A separate instance structure must be defined for each Instance but the twiddle factors can be reused.
 * There are separate instance structure declarations for each of the 3 supported data types.
 *
 * \par Initialization Functions
 * There is also an associated initialization function for each data type.
 * The initialization function performs the following operations:
 * - Sets the values of the internal structure fields.
 * - Initializes twiddle factor tables.
 * - Initializes CFFT data structure fields.
 * \par
 * Use of the initialization function is optional.
 * However, if the initialization function is used, then the instance structure cannot be placed into a const data section.
 * To place an instance structure into a const data section, the instance structure must be manually initialized.
 * Manually initialize the instance structure as follows:
 * <pre>
 *ne10_rfft_instance_f32_t S = {fft_len_real, fft_len_by2, ifft_flag_r, bit_reverse_flag_r, twid_coef_r_modifier, p_twiddle_A_real, p_twiddle_B_real, p_cfft};
 * </pre>
 * where <code>fft_len_real</code> length of RFFT/RIFFT; <code>fft_len_by2</code> length of CFFT/CIFFT.
 * <code>ifft_flag_r</code> Flag for selection of RFFT or RIFFT(Set ifftFlagR to calculate RIFFT otherwise calculates RFFT);
 * <code>bit_reverse_flag_r</code> Flag for selection of output order(Set bitReverseFlagR to output in normal order otherwise output in bit reversed order);
 * <code>twid_coef_r_modifier</code> modifier for twiddle factor table which supports 128, 512, 2048 RFFT lengths with same table;
 * <code>p_twiddle_A_real</code>points to A array of twiddle coefficients; <code>p_twiddle_B_real</code>points to B array of twiddle coefficients;
 * <code>p_cfft</code> points to the CFFT Instance structure. The CFFT structure also needs to be initialized, refer to arm_cfft_radix4_f32() for details regarding
 * static initialization of cfft structure.
 *
 */

/**
 * @brief  Core Real FFT process
 * @param[in]   *pSrc                points to the Input buffer
 * @param[in]   N                    length of Real FFT
 * @param[in]   *pATable             points to the twiddle Coef A buffer
 * @param[in]   *pBTable             points to the twiddle Coef B buffer
 * @param[out]  *pDst                points to the Output buffer
 * @return none.
 * The function implements a Real FFT
 */

static void ne10_split_rfft_float_c(
                     ne10_float32_t * pSrc,
                     ne10_uint32_t N,
                     ne10_float32_t * pReTable,
                     ne10_float32_t * pImTable,
                     ne10_float32_t * pDst)
{
    ne10_uint32_t k;                                    /* Loop Counter */
    ne10_float32_t uRe,vRe,uIm,vIm;                    /* Temporary variables for output */
    ne10_float32_t reTwd,imTwd,reTmp,imTmp;
    ne10_float32_t *pCoefRe,*pCoefIm;                             /* Temporary pointers for twiddle factors */
    ne10_uint32_t NBy2 = N>>1;

    pCoefRe = pReTable;
    pCoefIm = pImTable;

    /*First Result*/
    pDst[0] = pSrc[0] + pSrc[1];
    pDst[1] = 0;
    /*N/2 th Result*/
    pDst[N] = pSrc[0] - pSrc[1];
    pDst[N+1] = 0;

    /*for k=1 to N/4-1 and k=N/4+1 to K=N/2-1*/
    for(k=1;k<(N>>2);k++)
    {
        /*uRe = (a[k]+a[N/2-k])/2*/
        uRe = (pSrc[2*k]+pSrc[2*(NBy2-k)])*0.5;
        /*uIm = (b[k]-b[N/2-k])/2*/
        uIm = (pSrc[2*k+1]-pSrc[2*(NBy2-k)+1])*0.5;
        /*VRe = (b[k]+b[N/2-k])/2*/
        vRe = (pSrc[2*k+1]+pSrc[2*(NBy2-k)+1])*0.5;
        /*Vim = -(a[k]-a[N/2-k])/2*/
        vIm = (pSrc[2*(NBy2-k)]-pSrc[2*k])*0.5;
        reTwd = pCoefRe[k];
        imTwd = pCoefIm[k];
        reTmp = vRe*reTwd + vIm*imTwd;
        imTmp = vIm*reTwd - vRe*imTwd;
        pDst[2*k] = uRe + reTmp;
        pDst[2*k+1] = uIm + imTmp;
        pDst[2*(NBy2-k)] = uRe-reTmp;
        pDst[2*(NBy2-k)+1] = imTmp-uIm;

        /*Out Put from K=N/2+1 till k=N-1*/
        /*y[N-k] = conjugate(y[k] k=0 to k<N/2)*/
        pDst[2*(N-k)] = uRe + reTmp;
        pDst[2*(N-k)+1] = -(uIm + imTmp);
        pDst[2*(NBy2+k)] = uRe-reTmp;
        pDst[2*(NBy2+k)+1] = uIm-imTmp;
    }
    /*y[N/4] = a[N/4]-jb[N/4]; y[3*N/4] = a[N/4] + jb[N/4]*/
    pDst[NBy2] = pSrc[NBy2];
    pDst[NBy2+1] = -pSrc[NBy2+1];
    pDst[N+NBy2] = pSrc[NBy2];
    pDst[N+NBy2+1] = pSrc[NBy2+1];

}


/**
 * @brief  Core Real IFFT process
 * @param[in]   *pSrc                 points to the Input buffer
 * @param[in]   N                      length of Real FFT
 * @param[in]   *pATable             points to the twiddle Coef A buffer
 * @param[in]   *pBTable             points to the twiddle Coef B buffer
 * @param[out]  *pDst                 points to the Output buffer
 * @return none.
 * The function implements a Real FFT
 */


static void ne10_split_rifft_float_c(
                     ne10_float32_t * pSrc,
                     ne10_uint32_t N,
                     ne10_float32_t * pReTable,
                     ne10_float32_t * pImTable,
                     ne10_float32_t * pDst)
{
    ne10_uint32_t k;                                    /* Loop Counter */
    ne10_float32_t uRe,vRe,uIm,vIm;                    /* Temporary variables for output */
    ne10_float32_t reTwd,imTwd,reTmp,imTmp;
    ne10_float32_t *pCoefRe,*pCoefIm;                             /* Temporary pointers for twiddle factors */
    ne10_uint32_t NBy2 = N>>1;

    pCoefRe = pReTable;
    pCoefIm = pImTable;

    /*First Result*/
    pDst[0] = (pSrc[0] + pSrc[N])*0.5;
    pDst[1] = (pSrc[0] - pSrc[N])*0.5;

    /*for k=1 to N/4-1 and k=N/4+1 to K=N/2-1*/
    for(k=1;k< (N>>2);k++)
    {
        /*uRe = (a[k]+a[N/2-k])/2*/
        uRe = (pSrc[2*k] + pSrc[2*(NBy2-k)])*0.5;
        /*uIm = (b[k]-b[N/2-k])/2*/
        uIm = (pSrc[2*k+1] - pSrc[2*(NBy2-k)+1])*0.5;

        reTmp = (pSrc[2*k] - pSrc[2*(NBy2-k)])*0.5;
        imTmp = (pSrc[2*k+1] + pSrc[2*(NBy2-k)+1])*0.5;

        reTwd = pCoefRe[k];
        imTwd = pCoefIm[k];

        /*VRe = (b[k]+b[N/2-k])/2*/
        vRe =  reTmp*reTwd - imTmp*imTwd;
        /*Vim = -(a[k]-a[N/2-k])/2*/
        vIm =  imTmp*reTwd + reTmp*imTwd;
        pDst[2*k] = (uRe-vIm);
        pDst[2*k+1] = (uIm+vRe);

        pDst[2*(NBy2-k)] = (uRe+vIm);
        pDst[2*(NBy2-k)+1] = (vRe-uIm);
    }
    /*y[N/4] = a[N/4]-jb[N/4]*/
    pDst[NBy2] = pSrc[NBy2];
    pDst[NBy2+1] = -pSrc[NBy2+1];

}

/**
 * @addtogroup RFFT_RIFFT
 * @{
 */

/**
 * @brief  Real FFT process
 * @param[in]  *S is an instance for the structure
 * @param[in]  *pSrc point to the input buffer (out-of-place: it's also a tmp buffer, so the input buffer is destroyed)
 * @param[out]  *pDst point to the output buffer (out-of-place)
 * @param[in]  *pTemp point to the temp buffer (used for intermedia buffer)
 * @return none.
 * The function implements a Real FFT/ Real IFFT depending
 * on the direction flag
 * Can support FFT lengths of 128, 512, 2048
 *
 */
void ne10_rfft_float_c(
                     const ne10_rfft_instance_f32_t * S,
                     ne10_float32_t * pSrc,
                     ne10_float32_t * pDst,
                     ne10_float32_t * pTemp)
{
    const ne10_cfft_radix4_instance_f32_t *S_CFFT = S->p_cfft;

    /* Caluclation of Real IFFT of input */
    if(S->ifft_flag_r == 1u)
    {
        /*  Real IFFT core process */
        ne10_split_rifft_float_c(pSrc, S->fft_len_real, S->p_twiddle_A_real,
                S->p_twiddle_B_real, pTemp);
        /* Complex radix-4 IFFT process */
        ne10_radix4_butterfly_inverse_float_c(pDst, pTemp, S_CFFT->fft_len, S_CFFT->p_twiddle, S_CFFT->one_by_fft_len);
    }
    else
    {
        /* Complex radix-4 FFT process */
        ne10_radix4_butterfly_float_c(pTemp, pSrc, S_CFFT->fft_len, S_CFFT->p_twiddle);
        /*  Real FFT core process */
        ne10_split_rfft_float_c(pTemp, S->fft_len_real, S->p_twiddle_A_real,
                S->p_twiddle_B_real, pDst);
    }

}

/**
 * @} end of RFFT_RIFFT group
 */
