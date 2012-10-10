/*
 *  Copyright 2012 ARM Limited
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/*
 * NE10 Library : dsp/NE10_rfft.c
 */

#include "NE10_types.h"

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
 * @brief  Real FFT process
 * @param  *S is an instance for the structure
 * @param  *pSrc points to the input buffer
 * @return none.
 * The function implements a Real FFT/ Real IFFT depending
 * on the direction flag
 * Can support FFT lengths of 128, 512, 2048
 *
 * <b>Approximate Cycle Calculation for M4: </b>
 *
 * <code>C0 + C1 * fftLen </code>
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
        ne10_radix4_butterfly_inverse_float_c(pDst, pTemp, S_CFFT->fft_len, S_CFFT->p_twiddle);
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


