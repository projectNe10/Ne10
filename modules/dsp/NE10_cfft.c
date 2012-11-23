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
 * NE10 Library : dsp/NE10_cfft.c
 */

#include "NE10_types.h"


/*
; * @brief  Core radix-4 FFT of floating-point data.
; * @param[out]  *pDst
; * @param[in]  *pSrc             points to the In-place buffer
; * @param[in]  N                 length of FFT
; * @param[in]  *pCoef            points to the twiddle factors
; * @retureq none.
; * The function implements a Radix-4 Complex FFT
; */

void ne10_radix4_butterfly_float_c(
                     ne10_float32_t *pDst,
                     ne10_float32_t *pSrc,
                     ne10_uint16_t N,
                     ne10_float32_t *pCoef)
{
    ne10_int32_t     set,grp;
    ne10_int32_t     setCount,grpCount,grpStep,twidStep;
    ne10_float32_t       *pTw2,*pTw3,*pTw4,*pT1,*pTmp;
    ne10_float32_t       *pOut1,*pOut2,*pOut3,*pOut4;
    ne10_int32_t     SubFFTSize=4,SubFFTNum=N/4;            /*Intial Number of Groups and Group Size*/
    ne10_float32_t       InpRe1,InpIm1,InpRe2,InpIm2,InpRe3,InpIm3,InpRe4,InpIm4;
    ne10_float32_t       OutRe1,OutIm1,OutRe2,OutIm2,OutRe3,OutIm3,OutRe4,OutIm4;
    ne10_float32_t       ReTmp1,ImTmp1,ReTmp2,ImTmp2,ReTmp3,ImTmp3,ReTmp4,ImTmp4;
    ne10_float32_t       TwRe2,TwIm2,TwRe3,TwIm3,TwRe4,TwIm4;
    ne10_float32_t       ReTmpT2,ImTmpT2,ReTmpT3,ImTmpT3,ReTmpT4,ImTmpT4;
    ne10_int32_t     InpStep=(N>>2),OutStep=1;

    /*First stage*/
    grpCount = SubFFTNum;
    pT1 = pSrc;
    pOut1 = pDst;

    for(grp = 0; grp < grpCount; grp++)
    {
        InpRe1 = pT1[0];
        InpIm1 = pT1[1];
        InpRe2 = pT1[InpStep<<1];
        InpIm2 = pT1[(InpStep<<1)+1];
        InpRe3 = pT1[(InpStep<<2)];
        InpIm3 = pT1[(InpStep<<2)+1];
        InpRe4 = pT1[3*(InpStep<<1)];
        InpIm4 = pT1[3*(InpStep<<1)+1];

        //Inp1 + Inp3
        ReTmp1 = InpRe1 + InpRe3;
        ImTmp1 = InpIm1 + InpIm3;
        //Inp1 - Inp3
        ReTmp2 = InpRe1 - InpRe3;
        ImTmp2 = InpIm1 - InpIm3;

        //Inp2 + Inp4
        ReTmp3 = InpRe2 + InpRe4;
        ImTmp3 = InpIm2 + InpIm4;

        //Inp2 - Inp4
        ReTmp4 = InpRe2 - InpRe4;
        ImTmp4 = InpIm2 - InpIm4;

        /*Radix-4 Butterfly calculation*/
        /*Third Result*/
        OutRe3 = ReTmp1 - ReTmp3;
        OutIm3 = ImTmp1 - ImTmp3;
        /*First Result*/
        OutRe1 = ReTmp1 + ReTmp3;
        OutIm1 = ImTmp1 + ImTmp3;
        /*Second result*/
        OutRe2 = ReTmp2 + ImTmp4;
        OutIm2 = ImTmp2 - ReTmp4;
        /*Fourth Result*/
        OutRe4 = ReTmp2 - ImTmp4;
        OutIm4 = ImTmp2 + ReTmp4;

        *pOut1++ = OutRe1;
        *pOut1++ = OutIm1;
        *pOut1++ = OutRe2;
        *pOut1++ = OutIm2;
        *pOut1++ = OutRe3;
        *pOut1++ = OutIm3;
        *pOut1++ = OutRe4;
        *pOut1++ = OutIm4;
        pT1+=2;

    }
    /*Remaining FFT Stages: Second Stage to Last Stage*/
    /* Update the Grp count and size for the next stage */
    SubFFTSize = SubFFTSize<<2;
    SubFFTNum  = SubFFTNum>>2;
    twidStep = 0;
    /*Swap Input and Output*/
    pTmp = pDst;
    pDst = pSrc;
    pSrc= pTmp;

    while(SubFFTNum > 0)
    {
        grpCount = SubFFTNum;                          /*Number of Blocks*/
        setCount = SubFFTSize>>2;                      /* setCount is number of Butterflies */
        grpStep  = 0;
        OutStep     = (OutStep<<2);
        pT1 = pSrc;
        for(grp = 0; grp < grpCount; grp++)
        {
            pOut1 = pDst + (grpStep<<1);
            pOut2 = pOut1 + (OutStep<<1);
            pOut3 = pOut2 + (OutStep<<1);
            pOut4 = pOut3 + (OutStep<<1);

            pTw2 = pCoef + twidStep;
            pTw3 = pTw2  + (setCount<<1);
            pTw4 = pTw3  + (setCount<<1);
            for(set = 0; set < setCount; set++)
            {
                InpRe1 = pT1[0];
                InpIm1 = pT1[1];
                InpRe2 = pT1[InpStep<<1];
                InpIm2 = pT1[(InpStep<<1)+1];
                InpRe3 = pT1[(InpStep<<2)];
                InpIm3 = pT1[(InpStep<<2)+1];
                InpRe4 = pT1[3*(InpStep<<1)];
                InpIm4 = pT1[3*(InpStep<<1)+1];

                /*Load Twiddles*/
                TwRe2 = *pTw2++;
                TwIm2 = *pTw2++;
                TwRe3 = *pTw3++;
                TwIm3 = *pTw3++;
                TwRe4 = *pTw4++;
                TwIm4 = *pTw4++;


                /*Butterfly calculation*/
                //CPLX_MUL (pTmpT2, pTw2, Inp2);
                ReTmpT2 = InpRe2*TwRe2 + InpIm2*TwIm2;
                ImTmpT2 = InpIm2*TwRe2 - InpRe2*TwIm2;

                //CPLX_MUL (pTmpT3, pTw3, Inp3);
                ReTmpT3 = InpRe3*TwRe3 + InpIm3*TwIm3;
                ImTmpT3 = InpIm3*TwRe3 - InpRe3*TwIm3;


                //CPLX_MUL (pTmpT4, pTw4, Inp4);
                ReTmpT4 = InpRe4*TwRe4 + InpIm4*TwIm4;
                ImTmpT4 = InpIm4*TwRe4 - InpRe4*TwIm4;
                //CPLX_ADD (pTmp1, Inp1, pTmpT3);
                ReTmp1 = InpRe1 + ReTmpT3;
                ImTmp1 = InpIm1 + ImTmpT3;

                //CPLX_SUB (pTmp2, pT1, pTmpT3);
                ReTmp2 = InpRe1 - ReTmpT3;
                ImTmp2 = InpIm1 - ImTmpT3;

                //CPLX_ADD (pTmp3, pTmpT2, pTmpT4);
                ReTmp3 = ReTmpT2 + ReTmpT4;
                ImTmp3 = ImTmpT2 + ImTmpT4;

                //CPLX_SUB (pTmp4, pTmpT2, pTmpT4);
                ReTmp4 = ReTmpT2 - ReTmpT4;
                ImTmp4 = ImTmpT2 - ImTmpT4;



                /*Third Result*/
                //CPLX_SUB (pT3, pTmp1, pTmp3);
                OutRe3 = ReTmp1 - ReTmp3;
                OutIm3 = ImTmp1 - ImTmp3;
                /*First Result*/
                //CPLX_ADD (pT1, pTmp1, pTmp3);

                OutRe1 = ReTmp1 + ReTmp3;
                OutIm1 = ImTmp1 + ImTmp3;
                /*Second result*/
                //CPLX_ADD_SUB_X (pT2, pTmp2, pTmp4);
                OutRe2 = ReTmp2 + ImTmp4;
                OutIm2 = ImTmp2 - ReTmp4;
                /*Fourth Result*/
                //CPLX_SUB_ADD_X (pT4, pTmp2, pTmp4);
                OutRe4 = ReTmp2 - ImTmp4;
                OutIm4 = ImTmp2 + ReTmp4;
                /*Store the Result*/
                *pOut1++ = OutRe1;
                *pOut1++ = OutIm1;
                *pOut2++ = OutRe2;
                *pOut2++ = OutIm2;
                *pOut3++ = OutRe3;
                *pOut3++ = OutIm3;
                *pOut4++ = OutRe4;
                *pOut4++ = OutIm4;

                pT1+=2;
            }
            grpStep = grpStep + SubFFTSize;
        }
        /* Update the Grp count and size for the next stage */
        twidStep+= (3*(setCount<<1));
        SubFFTSize = SubFFTSize<<2;
        SubFFTNum  = SubFFTNum>>2;
        /*Swap Input and Output*/
        pTmp = pDst;
        pDst = pSrc;
        pSrc= pTmp;
    }

    /* if the N is even power of 4, copy the output to dst buffer */
    SubFFTNum = 0;
    set = N;
    while (set > 1)
    {
        set = set>>2;
        SubFFTNum++;
    }

    if((SubFFTNum&1) == 0)
    {
        pT1 = pSrc;
        pOut1 = pDst;
        for(grpCount = 0; grpCount < N; grpCount++)
        {
            *pOut1++ = *pT1++;
            *pOut1++ = *pT1++;
        };
    }
}

/*
; * @brief  Core radix-4 IFFT of floating-point data.
; * @param[out]  *pDst
; * @param[in]  *pSrc             points to the In-place buffer
; * @param[in]  N                 length of FFT
; * @param[in]  *pCoef            points to the twiddle factors
; * @retureq none.
; * The function implements a Radix-4 Complex IFFT
; */

void ne10_radix4_butterfly_inverse_float_c(
                     ne10_float32_t *pDst,
                     ne10_float32_t *pSrc,
                     ne10_uint16_t N,
                     ne10_float32_t *pCoef,
                     ne10_float32_t onebyN)
{
    ne10_int32_t     set,grp;
    ne10_int32_t     setCount,grpCount,grpStep,twidStep;
    ne10_float32_t       *pTw2,*pTw3,*pTw4,*pT1,*pTmp;
    ne10_float32_t       *pOut1,*pOut2,*pOut3,*pOut4;
    ne10_int32_t     SubFFTSize=4,SubFFTNum=N/4;            /*Intial Number of Groups and Group Size*/
    ne10_float32_t       InpRe1,InpIm1,InpRe2,InpIm2,InpRe3,InpIm3,InpRe4,InpIm4;
    ne10_float32_t       OutRe1,OutIm1,OutRe2,OutIm2,OutRe3,OutIm3,OutRe4,OutIm4;
    ne10_float32_t       ReTmp1,ImTmp1,ReTmp2,ImTmp2,ReTmp3,ImTmp3,ReTmp4,ImTmp4;
    ne10_float32_t       TwRe2,TwIm2,TwRe3,TwIm3,TwRe4,TwIm4;
    ne10_float32_t       ReTmpT2,ImTmpT2,ReTmpT3,ImTmpT3,ReTmpT4,ImTmpT4;
    ne10_int32_t     InpStep=(N>>2),OutStep=1;

    /*First stage*/
    grpCount = SubFFTNum;
    pT1 = pSrc;
    pOut1 = pDst;

    for(grp = 0; grp < grpCount; grp++)
    {
        InpRe1 = pT1[0];
        InpIm1 = pT1[1];
        InpRe2 = pT1[InpStep<<1];
        InpIm2 = pT1[(InpStep<<1)+1];
        InpRe3 = pT1[(InpStep<<2)];
        InpIm3 = pT1[(InpStep<<2)+1];
        InpRe4 = pT1[3*(InpStep<<1)];
        InpIm4 = pT1[3*(InpStep<<1)+1];

        //Inp1 + Inp3
        ReTmp1 = InpRe1 + InpRe3;
        ImTmp1 = InpIm1 + InpIm3;
        //Inp1 - Inp3
        ReTmp2 = InpRe1 - InpRe3;
        ImTmp2 = InpIm1 - InpIm3;

        //Inp2 + Inp4
        ReTmp3 = InpRe2 + InpRe4;
        ImTmp3 = InpIm2 + InpIm4;

        //Inp2 - Inp4
        ReTmp4 = InpRe2 - InpRe4;
        ImTmp4 = InpIm2 - InpIm4;

        /*Radix-4 Butterfly calculation*/
        /*Third Result*/
        OutRe3 = ReTmp1 - ReTmp3;
        OutIm3 = ImTmp1 - ImTmp3;
        /*First Result*/
        OutRe1 = ReTmp1 + ReTmp3;
        OutIm1 = ImTmp1 + ImTmp3;
        /*Second result*/
        OutRe2 = ReTmp2 - ImTmp4;
        OutIm2 = ImTmp2 + ReTmp4;
        /*Fourth Result*/
        OutRe4 = ReTmp2 + ImTmp4;
        OutIm4 = ImTmp2 - ReTmp4;

        *pOut1++ = OutRe1;
        *pOut1++ = OutIm1;
        *pOut1++ = OutRe2;
        *pOut1++ = OutIm2;
        *pOut1++ = OutRe3;
        *pOut1++ = OutIm3;
        *pOut1++ = OutRe4;
        *pOut1++ = OutIm4;
        pT1+=2;

    }
    /*Intermediate FFT Stages: Second Stage to Last but one Stage*/
    /* Update the Grp count and size for the next stage */
    SubFFTSize = SubFFTSize<<2;
    SubFFTNum  = SubFFTNum>>2;
    twidStep = 0;
    /*Swap Input and Output*/
    pTmp = pDst;
    pDst = pSrc;
    pSrc= pTmp;

    while(SubFFTNum > 1)
    {
        grpCount = SubFFTNum;                          /*Number of Blocks*/
        setCount = SubFFTSize>>2;                      /* setCount is number of Butterflies */
        grpStep  = 0;
        OutStep     = (OutStep<<2);
        pT1 = pSrc;
        for(grp = 0; grp < grpCount; grp++)
        {
            pOut1 = pDst + (grpStep<<1);
            pOut2 = pOut1 + (OutStep<<1);
            pOut3 = pOut2 + (OutStep<<1);
            pOut4 = pOut3 + (OutStep<<1);

            pTw2 = pCoef + twidStep;
            pTw3 = pTw2  + (setCount<<1);
            pTw4 = pTw3  + (setCount<<1);
            for(set = 0; set < setCount; set++)
            {
                InpRe1 = pT1[0];
                InpIm1 = pT1[1];
                InpRe2 = pT1[InpStep<<1];
                InpIm2 = pT1[(InpStep<<1)+1];
                InpRe3 = pT1[(InpStep<<2)];
                InpIm3 = pT1[(InpStep<<2)+1];
                InpRe4 = pT1[3*(InpStep<<1)];
                InpIm4 = pT1[3*(InpStep<<1)+1];

                /*Load Twiddles*/
                TwRe2 = *pTw2++;
                TwIm2 = *pTw2++;
                TwRe3 = *pTw3++;
                TwIm3 = *pTw3++;
                TwRe4 = *pTw4++;
                TwIm4 = *pTw4++;


                /*Butterfly calculation*/
                //CPLX_MUL (pTmpT2, pTw2, Inp2);
                ReTmpT2 = InpRe2*TwRe2 - InpIm2*TwIm2;
                ImTmpT2 = InpIm2*TwRe2 + InpRe2*TwIm2;

                //CPLX_MUL (pTmpT3, pTw3, Inp3);
                ReTmpT3 = InpRe3*TwRe3 - InpIm3*TwIm3;
                ImTmpT3 = InpIm3*TwRe3 + InpRe3*TwIm3;

                //CPLX_MUL (pTmpT4, pTw4, Inp4);
                ReTmpT4 = InpRe4*TwRe4 - InpIm4*TwIm4;
                ImTmpT4 = InpIm4*TwRe4 + InpRe4*TwIm4;

                //CPLX_ADD (pTmp1, Inp1, pTmpT3);
                ReTmp1 = InpRe1 + ReTmpT3;
                ImTmp1 = InpIm1 + ImTmpT3;

                //CPLX_SUB (pTmp2, pT1, pTmpT3);
                ReTmp2 = InpRe1 - ReTmpT3;
                ImTmp2 = InpIm1 - ImTmpT3;

                //CPLX_ADD (pTmp3, pTmpT2, pTmpT4);
                ReTmp3 = ReTmpT2 + ReTmpT4;
                ImTmp3 = ImTmpT2 + ImTmpT4;

                //CPLX_SUB (pTmp4, pTmpT2, pTmpT4);
                ReTmp4 = ReTmpT2 - ReTmpT4;
                ImTmp4 = ImTmpT2 - ImTmpT4;


                /*Third Result*/
                //CPLX_SUB (pT3, pTmp1, pTmp3);
                OutRe3 = ReTmp1 - ReTmp3;
                OutIm3 = ImTmp1 - ImTmp3;

                /*First Result*/
                //CPLX_ADD (pT1, pTmp1, pTmp3);
                OutRe1 = ReTmp1 + ReTmp3;
                OutIm1 = ImTmp1 + ImTmp3;

                /*Second result*/
                //CPLX_SUB_ADD_X (pT2, pTmp2, pTmp4);
                OutRe2 = ReTmp2 - ImTmp4;
                OutIm2 = ImTmp2 + ReTmp4;

                /*Fourth Result*/
                //CPLX_ADD_SUB_X (pT4, pTmp2, pTmp4);
                OutRe4 = ReTmp2 + ImTmp4;
                OutIm4 = ImTmp2 - ReTmp4;

                /*Store the Result*/
                *pOut1++ = OutRe1;
                *pOut1++ = OutIm1;
                *pOut2++ = OutRe2;
                *pOut2++ = OutIm2;
                *pOut3++ = OutRe3;
                *pOut3++ = OutIm3;
                *pOut4++ = OutRe4;
                *pOut4++ = OutIm4;

                pT1+=2;
            }
            grpStep = grpStep + SubFFTSize;
        }
        /* Update the Grp count and size for the next stage */
        twidStep+= (3*(setCount<<1));
        SubFFTSize = SubFFTSize<<2;
        SubFFTNum  = SubFFTNum>>2;
        /*Swap Input and Output*/
        pTmp = pDst;
        pDst = pSrc;
        pSrc= pTmp;
    }

    /* last stage */
    setCount = SubFFTSize>>2;                      /* setCount is number of Butterflies */
    grpStep  = 0;
    OutStep     = (OutStep<<2);
    pT1 = pSrc;

    pOut1 = pDst + (grpStep<<1);
    pOut2 = pOut1 + (OutStep<<1);
    pOut3 = pOut2 + (OutStep<<1);
    pOut4 = pOut3 + (OutStep<<1);

    pTw2 = pCoef + twidStep;
    pTw3 = pTw2  + (setCount<<1);
    pTw4 = pTw3  + (setCount<<1);

    for(set = 0; set < setCount; set++)
    {
        InpRe1 = pT1[0];
        InpIm1 = pT1[1];
        InpRe2 = pT1[InpStep<<1];
        InpIm2 = pT1[(InpStep<<1)+1];
        InpRe3 = pT1[(InpStep<<2)];
        InpIm3 = pT1[(InpStep<<2)+1];
        InpRe4 = pT1[3*(InpStep<<1)];
        InpIm4 = pT1[3*(InpStep<<1)+1];

        /*Load Twiddles*/
        TwRe2 = *pTw2++;
        TwIm2 = *pTw2++;
        TwRe3 = *pTw3++;
        TwIm3 = *pTw3++;
        TwRe4 = *pTw4++;
        TwIm4 = *pTw4++;


        /*Butterfly calculation*/
        //CPLX_MUL (pTmpT2, pTw2, Inp2);
        ReTmpT2 = InpRe2*TwRe2 - InpIm2*TwIm2;
        ImTmpT2 = InpIm2*TwRe2 + InpRe2*TwIm2;

        //CPLX_MUL (pTmpT3, pTw3, Inp3);
        ReTmpT3 = InpRe3*TwRe3 - InpIm3*TwIm3;
        ImTmpT3 = InpIm3*TwRe3 + InpRe3*TwIm3;

        //CPLX_MUL (pTmpT4, pTw4, Inp4);
        ReTmpT4 = InpRe4*TwRe4 - InpIm4*TwIm4;
        ImTmpT4 = InpIm4*TwRe4 + InpRe4*TwIm4;

        //CPLX_ADD (pTmp1, Inp1, pTmpT3);
        ReTmp1 = InpRe1 + ReTmpT3;
        ImTmp1 = InpIm1 + ImTmpT3;

        //CPLX_SUB (pTmp2, pT1, pTmpT3);
        ReTmp2 = InpRe1 - ReTmpT3;
        ImTmp2 = InpIm1 - ImTmpT3;

        //CPLX_ADD (pTmp3, pTmpT2, pTmpT4);
        ReTmp3 = ReTmpT2 + ReTmpT4;
        ImTmp3 = ImTmpT2 + ImTmpT4;

        //CPLX_SUB (pTmp4, pTmpT2, pTmpT4);
        ReTmp4 = ReTmpT2 - ReTmpT4;
        ImTmp4 = ImTmpT2 - ImTmpT4;


        /*Third Result*/
        //CPLX_SUB (pT3, pTmp1, pTmp3);
        OutRe3 = ReTmp1 - ReTmp3;
        OutIm3 = ImTmp1 - ImTmp3;

        /*First Result*/
        //CPLX_ADD (pT1, pTmp1, pTmp3);
        OutRe1 = ReTmp1 + ReTmp3;
        OutIm1 = ImTmp1 + ImTmp3;

        /*Second result*/
        //CPLX_SUB_ADD_X (pT2, pTmp2, pTmp4);
        OutRe2 = ReTmp2 - ImTmp4;
        OutIm2 = ImTmp2 + ReTmp4;

        /*Fourth Result*/
        //CPLX_ADD_SUB_X (pT4, pTmp2, pTmp4);
        OutRe4 = ReTmp2 + ImTmp4;
        OutIm4 = ImTmp2 - ReTmp4;

        /*Store the Result*/
        *pOut1++ = OutRe1 * onebyN;
        *pOut1++ = OutIm1 * onebyN;
        *pOut2++ = OutRe2 * onebyN;
        *pOut2++ = OutIm2 * onebyN;
        *pOut3++ = OutRe3 * onebyN;
        *pOut3++ = OutIm3 * onebyN;
        *pOut4++ = OutRe4 * onebyN;
        *pOut4++ = OutIm4 * onebyN;

        pT1+=2;
    }

    /*Swap Input and Output*/
    pTmp = pDst;
    pDst = pSrc;
    pSrc= pTmp;


    /* if the N is even power of 4, copy the output to dst buffer */
    SubFFTNum = 0;
    set = N;
    while (set > 1)
    {
        set = set>>2;
        SubFFTNum++;
    }

    if((SubFFTNum&1) == 0)
    {
        pT1 = pSrc;
        pOut1 = pDst;
        for(grpCount = 0; grpCount < N; grpCount++)
        {
            *pOut1++ = *pT1++;
            *pOut1++ = *pT1++;
        };
    }
}

