/*
 *  Copyright 2012 ARM Limited
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
 * NE10 Library : dsp/NE10_rfft.neon.c
 */
#include <arm_neon.h>

#include "NE10_types.h"
#include "NE10_mask_table.h"

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

static void ne10_split_rfft_float_neon(
                     ne10_float32_t * pSrc,
                     ne10_uint32_t N,
                     ne10_float32_t * pReTable,
                     ne10_float32_t * pImTable,
                     ne10_float32_t * pDst)
{
  ne10_uint32_t k,Cnt;                                   /* Loop Counter */
  ne10_float32_t *pCoefRe,*pCoefIm,*pOut1,*pIn1,*pOut2,*pIn2;       /* Temporary pointers for twiddle factors */
  ne10_uint32_t NBy2 = N>>1;
  /*NEON Variable Declarations*/
  float32x4x2_t vin1q2_f32,vin2q2_f32,vtmpq2_f32;
  float32x4_t vtmp1q_f32,vtmp2q_f32;
  float32x4_t vureq_f32,vuimq_f32,vvreq_f32,vvimq_f32;
  float32x4_t vretwdq_f32,vimtwdq_f32;
  float32x4_t vhalfq_f32;
  uint32x4_t vmaskq_u32,vmask1q_u32;

  /*Mask value to select three entries*/
  vmaskq_u32 = vld1q_u32(ne10_qMaskTable32+12);
  vmask1q_u32 = vld1q_u32(ne10_qMaskTable32+4);



  pCoefRe = pReTable+1;
  pCoefIm = pImTable+1;

  /*First Result*/
  pDst[0] = pSrc[0] + pSrc[1];
  pDst[1] = 0;
  /*N/2 th Result*/
  pDst[N] = pSrc[0] - pSrc[1];
  pDst[N+1] = 0;

  pOut1=pDst+2;
  pOut2=pDst+N-8;
  pIn1 = pSrc+2;
  pIn2 = pSrc +N -8;

  Cnt = ((N>>2)-1)>>2;
  vhalfq_f32 = vdupq_n_f32(0.5);

  /*for k=1 to N/4-1 and k=N/4+1 to K=N/2-1*/
  for(k=0;k<Cnt;k++)
  {
      /*b[4] b[3] b[2] b[1]   a[4] a[3] a[2] a[1]*/
      vin1q2_f32 = vld2q_f32(pIn1);
      pIn1+=8;
      /*b[N/2-1] b[N/2-2] b[N/2-3] b[N/2-4]   a[N/2-1] a[N/2-2] a[N/2-3] a[N/2-4]*/
      vin2q2_f32 = vld2q_f32(pIn2);
      pIn2-=8;
      /* a[N/2-2] a[N/2-1] a[N/2-4] a[N/2-3]*/
      vtmp1q_f32 =vrev64q_f32(vin2q2_f32.val[0]);
      /* b[N/2-2] b[N/2-1] b[N/2-4] b[N/2-3]*/
      vtmp2q_f32 =vrev64q_f32(vin2q2_f32.val[1]);
      /*a[N/2-4] a[N/2-3] a[N/2-2] a[N/2-1]*/
      vtmp1q_f32 = vcombine_f32(vget_high_f32(vtmp1q_f32),vget_low_f32(vtmp1q_f32));
      /* b[N/2-4] b[N/2-3] b[N/2-2] b[N/2-1]*/
      vtmp2q_f32 = vcombine_f32(vget_high_f32(vtmp2q_f32),vget_low_f32(vtmp2q_f32));

      /*uRe = (a[k]+a[N/2-k])/2*/
      vureq_f32 = vaddq_f32(vin1q2_f32.val[0],vtmp1q_f32);
      /*uIm = (b[k]-b[N/2-k])/2*/
      vuimq_f32 = vsubq_f32(vin1q2_f32.val[1],vtmp2q_f32);
      /*VRe = (b[k]+b[N/2-k])/2*/
      vvreq_f32 = vaddq_f32(vin1q2_f32.val[1],vtmp2q_f32);
      /*Vim = -(a[k]-a[N/2-k])/2*/
      vvimq_f32 = vsubq_f32(vtmp1q_f32,vin1q2_f32.val[0]);

      vureq_f32 = vmulq_f32(vureq_f32,vhalfq_f32);
      vuimq_f32 = vmulq_f32(vuimq_f32,vhalfq_f32);
      vvreq_f32 = vmulq_f32(vvreq_f32,vhalfq_f32);
      vvimq_f32 = vmulq_f32(vvimq_f32,vhalfq_f32);

      vretwdq_f32 = vld1q_f32(pCoefRe);
      vimtwdq_f32 = vld1q_f32(pCoefIm);
      pCoefRe+=4;
      pCoefIm+=4;

      /*reTmp = vRe*reTwd + vIm*imTwd */
      vtmp1q_f32 = vmulq_f32(vvreq_f32,vretwdq_f32);
      vtmp1q_f32 = vmlaq_f32(vtmp1q_f32,vvimq_f32,vimtwdq_f32);
      /*imTmp = vIm*reTwd - vRe*imTwd */
      vtmp2q_f32 = vmulq_f32(vvimq_f32,vretwdq_f32);
      vtmp2q_f32 = vmlsq_f32(vtmp2q_f32,vvreq_f32,vimtwdq_f32);
      //pDst[2*k] = uRe + reTmp;
      //pDst[2*k+1] = uIm + imTmp;
      vin1q2_f32.val[0] = vaddq_f32(vureq_f32,vtmp1q_f32);
      vin1q2_f32.val[1] = vaddq_f32(vuimq_f32,vtmp2q_f32);
      //pDst[2*(NBy2+k)] = uRe-reTmp;
      //pDst[2*(NBy2+k)+1] = uIm-imTmp;
      vin2q2_f32.val[0] = vsubq_f32(vureq_f32,vtmp1q_f32);
      vin2q2_f32.val[1] = vsubq_f32(vuimq_f32,vtmp2q_f32);
      vst2q_f32(pOut1,vin1q2_f32);
      vst2q_f32(pOut1+N,vin2q2_f32);
      pOut1+=8;

      //pDst[2*(NBy2-k)] = uRe-reTmp;
      //pDst[2*(NBy2-k)+1] = imTmp-uIm;
      vtmp2q_f32 = vsubq_f32(vtmp2q_f32,vuimq_f32);
      vtmp1q_f32 = vrev64q_f32(vin2q2_f32.val[0]);
      vtmp2q_f32 = vrev64q_f32(vtmp2q_f32);

      vin2q2_f32.val[0] = vcombine_f32(vget_high_f32(vtmp1q_f32),vget_low_f32(vtmp1q_f32));
      vin2q2_f32.val[1] = vcombine_f32(vget_high_f32(vtmp2q_f32),vget_low_f32(vtmp2q_f32));

      //pDst[2*(N-k)] = uRe + reTmp;
      //pDst[2*(N-k)+1] = -(uIm + imTmp);
      vtmp2q_f32 = vnegq_f32(vin1q2_f32.val[1]);
      vtmp1q_f32 = vrev64q_f32(vin1q2_f32.val[0]);
      vtmp2q_f32 = vrev64q_f32(vtmp2q_f32);

      vin1q2_f32.val[0] = vcombine_f32(vget_high_f32(vtmp1q_f32),vget_low_f32(vtmp1q_f32));
      vin1q2_f32.val[1] = vcombine_f32(vget_high_f32(vtmp2q_f32),vget_low_f32(vtmp2q_f32));

      vst2q_f32(pOut2,vin2q2_f32);
      vst2q_f32(pOut2+N,vin1q2_f32);
      pOut2-=8;
  }
  /*Lst Three VAlues*/
  /*b[4] b[3] b[2] b[1]   a[4] a[3] a[2] a[1]*/
  vin1q2_f32 = vld2q_f32(pIn1);
  pIn1+=8;
  /*b[N/2-1] b[N/2-2] b[N/2-3] b[N/2-4]   a[N/2-1] a[N/2-2] a[N/2-3] a[N/2-4]*/
  vin2q2_f32 = vld2q_f32(pIn2);
  pIn2-=8;
  /* a[N/2-2] a[N/2-1] a[N/2-4] a[N/2-3]*/
  vtmp1q_f32 =vrev64q_f32(vin2q2_f32.val[0]);
  /* b[N/2-2] b[N/2-1] b[N/2-4] b[N/2-3]*/
  vtmp2q_f32 =vrev64q_f32(vin2q2_f32.val[1]);
  /*a[N/2-4] a[N/2-3] a[N/2-2] a[N/2-1]*/
  vtmp1q_f32 = vcombine_f32(vget_high_f32(vtmp1q_f32),vget_low_f32(vtmp1q_f32));
  /* b[N/2-4] b[N/2-3] b[N/2-2] b[N/2-1]*/
  vtmp2q_f32 = vcombine_f32(vget_high_f32(vtmp2q_f32),vget_low_f32(vtmp2q_f32));

  /*uRe = (a[k]+a[N/2-k])/2*/
  vureq_f32 = vaddq_f32(vin1q2_f32.val[0],vtmp1q_f32);
  /*uIm = (b[k]-b[N/2-k])/2*/
  vuimq_f32 = vsubq_f32(vin1q2_f32.val[1],vtmp2q_f32);
  /*VRe = (b[k]+b[N/2-k])/2*/
  vvreq_f32 = vaddq_f32(vin1q2_f32.val[1],vtmp2q_f32);
  /*Vim = -(a[k]-a[N/2-k])/2*/
  vvimq_f32 = vsubq_f32(vtmp1q_f32,vin1q2_f32.val[0]);

  vureq_f32 = vmulq_f32(vureq_f32,vhalfq_f32);
  vuimq_f32 = vmulq_f32(vuimq_f32,vhalfq_f32);
  vvreq_f32 = vmulq_f32(vvreq_f32,vhalfq_f32);
  vvimq_f32 = vmulq_f32(vvimq_f32,vhalfq_f32);

  vretwdq_f32 = vld1q_f32(pCoefRe);
  vimtwdq_f32 = vld1q_f32(pCoefIm);
  pCoefRe+=4;
  pCoefIm+=4;

  /*reTmp = vRe*reTwd + vIm*imTwd */
  vtmp1q_f32 = vmulq_f32(vvreq_f32,vretwdq_f32);
  vtmp1q_f32 = vmlaq_f32(vtmp1q_f32,vvimq_f32,vimtwdq_f32);
  /*imTmp = vIm*reTwd - vRe*imTwd */
  vtmp2q_f32 = vmulq_f32(vvimq_f32,vretwdq_f32);
  vtmp2q_f32 = vmlsq_f32(vtmp2q_f32,vvreq_f32,vimtwdq_f32);
  //pDst[2*k] = uRe + reTmp;
  //pDst[2*k+1] = uIm + imTmp;
  vin1q2_f32.val[0] = vaddq_f32(vureq_f32,vtmp1q_f32);
  vin1q2_f32.val[1] = vaddq_f32(vuimq_f32,vtmp2q_f32);

  vtmpq2_f32 = vld2q_f32(pOut1);
  vin1q2_f32.val[0] = vbslq_f32(vmaskq_u32,vin1q2_f32.val[0],vtmpq2_f32.val[0]);
  vin1q2_f32.val[1] = vbslq_f32(vmaskq_u32,vin1q2_f32.val[1],vtmpq2_f32.val[1]);
  //pDst[2*(NBy2+k)] = uRe-reTmp;
  //pDst[2*(NBy2+k)+1] = uIm-imTmp;
  vin2q2_f32.val[0] = vsubq_f32(vureq_f32,vtmp1q_f32);
  vin2q2_f32.val[1] = vsubq_f32(vuimq_f32,vtmp2q_f32);

  vtmpq2_f32 = vld2q_f32(pOut1+N);
  vin2q2_f32.val[0] = vbslq_f32(vmaskq_u32,vin2q2_f32.val[0],vtmpq2_f32.val[0]);
  vin2q2_f32.val[1] = vbslq_f32(vmaskq_u32,vin2q2_f32.val[1],vtmpq2_f32.val[1]);

  vst2q_f32(pOut1,vin1q2_f32);
  vst2q_f32(pOut1+N,vin2q2_f32);
  pOut1+=8;


  //pDst[2*(NBy2-k)] = uRe-reTmp;
  //pDst[2*(NBy2-k)+1] = imTmp-uIm;
  vtmp2q_f32 = vnegq_f32(vin2q2_f32.val[1]);
  vtmp1q_f32 = vrev64q_f32(vin2q2_f32.val[0]);
  vtmp2q_f32 = vrev64q_f32(vtmp2q_f32);

  vin2q2_f32.val[0] = vcombine_f32(vget_high_f32(vtmp1q_f32),vget_low_f32(vtmp1q_f32));
  vin2q2_f32.val[1] = vcombine_f32(vget_high_f32(vtmp2q_f32),vget_low_f32(vtmp2q_f32));

  vtmpq2_f32 = vld2q_f32(pOut2);
  vin2q2_f32.val[0] = vbslq_f32(vmask1q_u32,vtmpq2_f32.val[0],vin2q2_f32.val[0]);
  vin2q2_f32.val[1] = vbslq_f32(vmask1q_u32,vtmpq2_f32.val[1],vin2q2_f32.val[1]);


  //pDst[2*(N-k)] = uRe + reTmp;
  //pDst[2*(N-k)+1] = -(uIm + imTmp);
  vtmp2q_f32 = vnegq_f32(vin1q2_f32.val[1]);
  vtmp1q_f32 = vrev64q_f32(vin1q2_f32.val[0]);
  vtmp2q_f32 = vrev64q_f32(vtmp2q_f32);

  vin1q2_f32.val[0] = vcombine_f32(vget_high_f32(vtmp1q_f32),vget_low_f32(vtmp1q_f32));
  vin1q2_f32.val[1] = vcombine_f32(vget_high_f32(vtmp2q_f32),vget_low_f32(vtmp2q_f32));

  vtmpq2_f32 = vld2q_f32(pOut2+N);
  vin1q2_f32.val[0] = vbslq_f32(vmask1q_u32,vtmpq2_f32.val[0],vin1q2_f32.val[0]);
  vin1q2_f32.val[1] = vbslq_f32(vmask1q_u32,vtmpq2_f32.val[1],vin1q2_f32.val[1]);

  vst2q_f32(pOut2,vin2q2_f32);
  vst2q_f32(pOut2+N,vin1q2_f32);
  pOut2-=8;



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


static void ne10_split_rifft_float_neon(
                     ne10_float32_t * pSrc,
                     ne10_uint32_t N,
                     ne10_float32_t * pReTable,
                     ne10_float32_t * pImTable,
                     ne10_float32_t * pDst)
{
    ne10_uint32_t k,Cnt;                                             /* Loop Counter */
    ne10_float32_t *pCoefRe,*pCoefIm,*pOut1,*pOut2,*pIn1,*pIn2;      /* Temporary pointers for twiddle factors */
    ne10_uint32_t NBy2 = N>>1;

    /*NEON Variable Declarations*/
    float32x4x2_t vin1q2_f32,vin2q2_f32,vtmpq2_f32;
    float32x4_t vtmp1q_f32,vtmp2q_f32;
    float32x4_t vureq_f32,vuimq_f32,vvreq_f32,vvimq_f32;
    float32x4_t vretwdq_f32,vimtwdq_f32;
    float32x4_t vhalfq_f32;
    uint32x4_t vmaskq_u32,vmask1q_u32;

    /*Mask value to select three entries*/
    vmaskq_u32 = vld1q_u32(ne10_qMaskTable32+12);
    vmask1q_u32 = vld1q_u32(ne10_qMaskTable32+4);

    pCoefRe = pReTable+1;
    pCoefIm = pImTable+1;

    /*First Result*/
    pDst[0] = (pSrc[0] + pSrc[N])*0.5;
    pDst[1] = (pSrc[0] - pSrc[N])*0.5;

    pOut1=pDst+2;
    pOut2=pDst+N-8;
    pIn1 = pSrc+2;
    pIn2 = pSrc +N -8;

    Cnt = ((N>>2)-1)>>2;
    vhalfq_f32 = vdupq_n_f32(0.5);

    /*for k=1 to N/4-1 and k=N/4+1 to K=N/2-1*/
    for(k=0;k<Cnt;k++)
    {
        /*b[4] b[3] b[2] b[1]   a[4] a[3] a[2] a[1]*/
        vin1q2_f32 = vld2q_f32(pIn1);
        pIn1+=8;
        /*b[N/2-1] b[N/2-2] b[N/2-3] b[N/2-4]   a[N/2-1] a[N/2-2] a[N/2-3] a[N/2-4]*/
        vin2q2_f32 = vld2q_f32(pIn2);
        pIn2-=8;
        /* a[N/2-2] a[N/2-1] a[N/2-4] a[N/2-3]*/
        vtmp1q_f32 =vrev64q_f32(vin2q2_f32.val[0]);
        /* b[N/2-2] b[N/2-1] b[N/2-4] b[N/2-3]*/
        vtmp2q_f32 =vrev64q_f32(vin2q2_f32.val[1]);
        /*a[N/2-4] a[N/2-3] a[N/2-2] a[N/2-1]*/
        vtmp1q_f32 = vcombine_f32(vget_high_f32(vtmp1q_f32),vget_low_f32(vtmp1q_f32));
        /* b[N/2-4] b[N/2-3] b[N/2-2] b[N/2-1]*/
        vtmp2q_f32 = vcombine_f32(vget_high_f32(vtmp2q_f32),vget_low_f32(vtmp2q_f32));

        /*uRe = (a[k]+a[N/2-k])/2*/
        vureq_f32 = vaddq_f32(vin1q2_f32.val[0],vtmp1q_f32);
        /*uIm = (b[k]-b[N/2-k])/2*/
        vuimq_f32 = vsubq_f32(vin1q2_f32.val[1],vtmp2q_f32);

        /*VRe = (a[k]-a[N/2-k])/2*/
        vvreq_f32 = vsubq_f32(vin1q2_f32.val[0],vtmp1q_f32);
        /*Vim = (b[k]+b[N/2-k])/2*/
        vvimq_f32 = vaddq_f32(vin1q2_f32.val[1],vtmp2q_f32);

        vureq_f32 = vmulq_f32(vureq_f32,vhalfq_f32);
        vuimq_f32 = vmulq_f32(vuimq_f32,vhalfq_f32);
        vvreq_f32 = vmulq_f32(vvreq_f32,vhalfq_f32);
        vvimq_f32 = vmulq_f32(vvimq_f32,vhalfq_f32);

        vretwdq_f32 = vld1q_f32(pCoefRe);
        vimtwdq_f32 = vld1q_f32(pCoefIm);
        pCoefRe+=4;
        pCoefIm+=4;

        /*reTmp = vRe*reTwd - vIm*imTwd */
        vtmp1q_f32 = vmulq_f32(vvreq_f32,vretwdq_f32);
        vtmp1q_f32 = vmlsq_f32(vtmp1q_f32,vvimq_f32,vimtwdq_f32);
        /*imTmp = vIm*reTwd + vRe*imTwd */
        vtmp2q_f32 = vmulq_f32(vvimq_f32,vretwdq_f32);
        vtmp2q_f32 = vmlaq_f32(vtmp2q_f32,vvreq_f32,vimtwdq_f32);
        //pDst[2*k] = uRe - imTmp;
        //pDst[2*k+1] = uIm + reTmp;
        vin1q2_f32.val[0] = vsubq_f32(vureq_f32,vtmp2q_f32);
        vin1q2_f32.val[1] = vaddq_f32(vuimq_f32,vtmp1q_f32);

        //pDst[2*(NBy2-k)] = uRe+imTmp;
        //pDst[2*(NBy2-k)+1] = reTmp-uIm;
        vtmp2q_f32 = vaddq_f32(vtmp2q_f32,vureq_f32);
        vtmp1q_f32 = vsubq_f32(vtmp1q_f32,vuimq_f32);
        vtmp2q_f32 = vrev64q_f32(vtmp2q_f32);
        vtmp1q_f32 = vrev64q_f32(vtmp1q_f32);

        vin2q2_f32.val[1] = vcombine_f32(vget_high_f32(vtmp1q_f32),vget_low_f32(vtmp1q_f32));
        vin2q2_f32.val[0] = vcombine_f32(vget_high_f32(vtmp2q_f32),vget_low_f32(vtmp2q_f32));


        vst2q_f32(pOut1,vin1q2_f32);
        vst2q_f32(pOut2,vin2q2_f32);
        pOut1+=8;
        pOut2-=8;
    }
    /*b[4] b[3] b[2] b[1]   a[4] a[3] a[2] a[1]*/
    vin1q2_f32 = vld2q_f32(pIn1);
    pIn1+=8;
    /*b[N/2-1] b[N/2-2] b[N/2-3] b[N/2-4]   a[N/2-1] a[N/2-2] a[N/2-3] a[N/2-4]*/
    vin2q2_f32 = vld2q_f32(pIn2);
    pIn2-=8;
    /* a[N/2-2] a[N/2-1] a[N/2-4] a[N/2-3]*/
    vtmp1q_f32 =vrev64q_f32(vin2q2_f32.val[0]);
    /* b[N/2-2] b[N/2-1] b[N/2-4] b[N/2-3]*/
    vtmp2q_f32 =vrev64q_f32(vin2q2_f32.val[1]);
    /*a[N/2-4] a[N/2-3] a[N/2-2] a[N/2-1]*/
    vtmp1q_f32 = vcombine_f32(vget_high_f32(vtmp1q_f32),vget_low_f32(vtmp1q_f32));
    /* b[N/2-4] b[N/2-3] b[N/2-2] b[N/2-1]*/
    vtmp2q_f32 = vcombine_f32(vget_high_f32(vtmp2q_f32),vget_low_f32(vtmp2q_f32));

    /*uRe = (a[k]+a[N/2-k])/2*/
    vureq_f32 = vaddq_f32(vin1q2_f32.val[0],vtmp1q_f32);
    /*uIm = (b[k]-b[N/2-k])/2*/
    vuimq_f32 = vsubq_f32(vin1q2_f32.val[1],vtmp2q_f32);

    /*VRe = (a[k]-a[N/2-k])/2*/
    vvreq_f32 = vsubq_f32(vin1q2_f32.val[0],vtmp1q_f32);
    /*Vim = (b[k]+b[N/2-k])/2*/
    vvimq_f32 = vaddq_f32(vin1q2_f32.val[1],vtmp2q_f32);

    vureq_f32 = vmulq_f32(vureq_f32,vhalfq_f32);
    vuimq_f32 = vmulq_f32(vuimq_f32,vhalfq_f32);
    vvreq_f32 = vmulq_f32(vvreq_f32,vhalfq_f32);
    vvimq_f32 = vmulq_f32(vvimq_f32,vhalfq_f32);

    vretwdq_f32 = vld1q_f32(pCoefRe);
    vimtwdq_f32 = vld1q_f32(pCoefIm);
    pCoefRe+=4;
    pCoefIm+=4;

    /*reTmp = vRe*reTwd - vIm*imTwd */
    vtmp1q_f32 = vmulq_f32(vvreq_f32,vretwdq_f32);
    vtmp1q_f32 = vmlsq_f32(vtmp1q_f32,vvimq_f32,vimtwdq_f32);
    /*imTmp = vIm*reTwd + vRe*imTwd */
    vtmp2q_f32 = vmulq_f32(vvimq_f32,vretwdq_f32);
    vtmp2q_f32 = vmlaq_f32(vtmp2q_f32,vvreq_f32,vimtwdq_f32);
    //pDst[2*k] = uRe - imTmp;
    //pDst[2*k+1] = uIm + reTmp;
    vin1q2_f32.val[0] = vsubq_f32(vureq_f32,vtmp2q_f32);
    vin1q2_f32.val[1] = vaddq_f32(vuimq_f32,vtmp1q_f32);

    vtmpq2_f32 = vld2q_f32(pOut1);
    vin1q2_f32.val[0] = vbslq_f32(vmaskq_u32,vin1q2_f32.val[0],vtmpq2_f32.val[0]);
    vin1q2_f32.val[1] = vbslq_f32(vmaskq_u32,vin1q2_f32.val[1],vtmpq2_f32.val[1]);

    //pDst[2*(NBy2-k)] = uRe+imTmp;
    //pDst[2*(NBy2-k)+1] = reTmp-uIm;
    vtmp2q_f32 = vaddq_f32(vtmp2q_f32,vureq_f32);
    vtmp1q_f32 = vsubq_f32(vtmp1q_f32,vuimq_f32);
    vtmp2q_f32 = vrev64q_f32(vtmp2q_f32);
    vtmp1q_f32 = vrev64q_f32(vtmp1q_f32);

    vin2q2_f32.val[1] = vcombine_f32(vget_high_f32(vtmp1q_f32),vget_low_f32(vtmp1q_f32));
    vin2q2_f32.val[0] = vcombine_f32(vget_high_f32(vtmp2q_f32),vget_low_f32(vtmp2q_f32));

    vtmpq2_f32 = vld2q_f32(pOut2);
    vin2q2_f32.val[0] = vbslq_f32(vmask1q_u32,vtmpq2_f32.val[0],vin2q2_f32.val[0]);
    vin2q2_f32.val[1] = vbslq_f32(vmask1q_u32,vtmpq2_f32.val[1],vin2q2_f32.val[1]);

    vst2q_f32(pOut1,vin1q2_f32);
    vst2q_f32(pOut2,vin2q2_f32);
    pOut1+=6;
    pOut2-=6;
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
void ne10_rfft_float_neon(
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
        ne10_split_rifft_float_neon(pSrc, S->fft_len_real, S->p_twiddle_A_real,
                S->p_twiddle_B_real, pTemp);
        /* Complex radix-4 IFFT process */
        ne10_radix4_butterfly_inverse_float_neon(pDst, pTemp, S_CFFT->fft_len, S_CFFT->p_twiddle, S_CFFT->one_by_fft_len);
    }
    else
    {
        /* Complex radix-4 FFT process */
        ne10_radix4_butterfly_float_neon(pTemp, pSrc, S_CFFT->fft_len, S_CFFT->p_twiddle);
        /*  Real FFT core process */
        ne10_split_rfft_float_neon(pTemp, S->fft_len_real, S->p_twiddle_A_real,
                S->p_twiddle_B_real, pDst);
    }

}
/**
 * @} end of RFFT_RIFFT group
 */

