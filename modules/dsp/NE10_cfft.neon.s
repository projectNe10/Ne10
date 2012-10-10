;/*
; * Copyright 2012 ARM Limited
; *
; * Licensed under the Apache License, Version 2.0 (the "License");
; * you may not use this file except in compliance with the License.
; * You may obtain a copy of the License at
; *
; *     http://www.apache.org/licenses/LICENSE-2.0
; *
; * Unless required by applicable law or agreed to in writing, software
; * distributed under the License is distributed on an "AS IS" BASIS,
; * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
; * See the License for the specific language governing permissions and
; * limitations under the License.
; */

;/*
; * NE10 Library : dsp/NE10_cfft.neon.s
; */

        .text
        .syntax   unified

        @/*
        @ * @brief  Core radix-4 FFT of floating-point data.  Do not call this function directly.
        @ * @param[out]  *pDst            points to the output buffer
        @ * @param[in]  *pSrc             points to the input buffer
        @ * @param[in]  N                 length of FFT
        @ * @param[in]  *pCoef            points to the twiddle factors
        @ * @retureq none.
        @ * The function implements a Radix-4 Complex FFT
        @ */

        .align   4
        .global   ne10_radix4_butterfly_float_neon
        .thumb
        .thumb_func

ne10_radix4_butterfly_float_neon:

        PUSH    {r4-r12,lr}
        VPUSH   {d8-d15}

        qInp1   .qn Q0.F32
        qInp2   .qn Q1.F32
        qInp3   .qn Q2.F32
        qInp4   .qn Q3.F32
        qInp5   .qn Q4.F32
        qInp6   .qn Q5.F32
        qInp7   .qn Q6.F32
        qInp8   .qn Q7.F32

        qTwd2Re .qn Q8.F32
        qTwd2Im .qn Q9.F32
        qTwd3Re .qn Q10.F32
        qTwd3Im .qn Q11.F32
        qTwd4Re .qn Q12.F32
        qTwd4Im .qn Q13.F32

        qReTmpT2 .qn Q14.F32
        qImTmpT2 .qn Q15.F32

        qReTmpT3 .qn Q2.F32
        qImTmpT3 .qn Q3.F32

        qReTmpT4 .qn Q4.F32
        qImTmpT4 .qn Q5.F32

        qRe1     .qn Q8.F32
        qIm1     .qn Q9.F32
        qRe2     .qn Q10.F32
        qIm2     .qn Q11.F32
        qRe3     .qn Q12.F32
        qIm3     .qn Q13.F32
        qRe4     .qn Q14.F32
        qIm4     .qn Q15.F32

        pDst        .req  R0
        pSrc        .req  R1
        fftSize     .req  R2
        pCoef       .req  R3


        SubFFTSize  .req  R4
        SubFFTNum   .req  R5
        grpCount    .req  R6
        twidStep    .req  R8
        setCount    .req  R9
        grpStep     .req  R10

        pT1         .req  R7
        pOut1       .req  R11
        pTw2        .req  R12
        TwdStep     .req  R14
        pTmp        .req  R7

        LSR     SubFFTNum,fftSize,#2
        MOV     SubFFTSize,#4
        MOV     pT1,pSrc
        LSR     grpCount,SubFFTNum,#2
        MOV     pOut1,pDst
        LSL     fftSize,#1

fftGrpLoop:
        VLD2        {qInp1,qInp2},[pT1],fftSize  @/*Load Input Values*/
        VLD2        {qInp3,qInp4},[pT1],fftSize
        VLD2        {qInp5,qInp6},[pT1],fftSize
        VLD2        {qInp7,qInp8},[pT1],fftSize

        @/*pSrc[0] + pSrc[2]*/
        VADD    qRe1,qInp1,qInp5
        VADD    qIm1,qInp2,qInp6
        @/*pSrc[0] - pSrc[2]*/
        VSUB    qRe2,qInp1,qInp5
        VSUB    qIm2,qInp2,qInp6
        @/*pSrc[1] + pSrc[3]*/
        VADD    qRe3,qInp3,qInp7
        VADD    qIm3,qInp4,qInp8
        @/*pSrc[1] - pSrc[3]*/
        VSUB    qRe4,qInp3,qInp7
        VSUB    qIm4,qInp4,qInp8

        @/*Radix-4 Butterfly calculation*/
        @/*Third Result*/
        VSUB    qInp5,qRe1,qRe3
        VSUB    qInp6,qIm1,qIm3
        @/*First Result*/
        VADD    qInp1,qRe1,qRe3
        VADD    qInp2,qIm1,qIm3
        @/*Second result*/
        VADD    qInp3,qRe2,qIm4
        VSUB    qInp4,qIm2,qRe4
        @/*Fourth Result*/
        VSUB    qInp7,qRe2,qIm4
        VADD    qInp8,qIm2,qRe4

        @/*Get Result in correct order for storing*/
        @/*4Re2,4Re0,3Re2,3Re0 2Re2,2Re0,1Re2,1Re0*/
        VZIP    qInp1,qInp5
        @/*4Re3,4Re1,3Re3,3Re1 2Re3,2Re1,1Re3,1Re1*/
        VZIP    qInp3,qInp7

        @/*4Im2,4Im0,3Im2,3Im0 2Im2,2Im0,1Im2,1Im0*/
        VZIP    qInp2,qInp6
        @/*4Im3,4Im1,3Im2,3Im1 2Im3,2Im1,1Im3,1Im1*/
        VZIP    qInp4,qInp8

        SUB     pT1,pT1,fftSize, LSL #2

        VST4.F32    {d0,d2,d4,d6},[pOut1]!
        VST4.F32    {d1,d3,d5,d7},[pOut1]!
        SUBS        grpCount,#1
        ADD         pT1,pT1,#32
        VST4.F32    {d8,d10,d12,d14},[pOut1]!
        VST4.F32    {d9,d11,d13,d15},[pOut1]!

        BGT     fftGrpLoop

        @/* Swap Input and Output*/
        MOV     pTmp,pDst
        MOV     pDst,pSrc
        MOV     pSrc,pTmp

        @/*Remaining FFT Stages: Second Stage to Last Stage*/
        @/* Update the Grp count and size for the next stage */
        LSR     SubFFTNum,#2
        LSL     SubFFTSize,#2

fftStageLoop:
        MOV     grpCount,SubFFTNum
        MOV     grpStep,#0
        ADD     pT1,pSrc,fftSize
        LSL     TwdStep,SubFFTSize,#1

fftGrpLoop1:
        LSR     setCount,SubFFTSize,#2
        ADD     pOut1,pDst,grpStep,LSL #3
        MOV     pTw2,pCoef

        LSL     SubFFTSize,#1

fftSetLoop:
        VLD2    {qTwd2Re,qTwd2Im},[pTw2],TwdStep
        VLD2    {qInp3,qInp4},[pT1],fftSize
        @/*CPLX_MUL (pTmpT2, pTw2, pT2);*/
        VMUL    qReTmpT2,qTwd2Re,qInp3
        VMUL    qImTmpT2,qTwd2Re,qInp4
        VLD2    {qTwd3Re,qTwd3Im},[pTw2],TwdStep
        VLD2    {qInp5,qInp6},[pT1],fftSize
        VMLA    qReTmpT2,qTwd2Im,qInp4
        VMLS    qImTmpT2,qTwd2Im,qInp3


        @/*CPLX_MUL (pTmpT3, pTw3, pT3);*/
        VMUL    qReTmpT3,qTwd3Re,qInp5
        VMUL    qImTmpT3,qTwd3Re,qInp6
        VLD2    {qTwd4Re,qTwd4Im},[pTw2]
        VLD2    {qInp7,qInp8},[pT1],fftSize
        VMLA    qReTmpT3,qTwd3Im,qInp6
        VMLS    qImTmpT3,qTwd3Im,qInp5

        SUB     pT1,pT1,fftSize, LSL #2


        @/*CPLX_MUL (pTmpT4, pTw4, pT4);*/
        VMUL    qReTmpT4,qTwd4Re,qInp7
        VMUL    qImTmpT4,qTwd4Re,qInp8
        VLD2    {qInp1,qInp2},[pT1],fftSize
        VMLA    qReTmpT4,qTwd4Im,qInp8
        VMLS    qImTmpT4,qTwd4Im,qInp7


        @/*CPLX_ADD (pTmp1, pT1, pTmpT3);*/
        VADD    qRe1,qInp1,qReTmpT3
        VADD    qIm1,qInp2,qImTmpT3
        @/*CPLX_SUB (pTmp2, pT1, pTmpT3);*/
        VSUB    qRe2,qInp1,qReTmpT3
        VSUB    qIm2,qInp2,qImTmpT3
        @/*CPLX_ADD (pTmp3, pTmpT2, pTmpT4);*/
        VADD    qRe3,qReTmpT2,qReTmpT4
        VADD    qIm3,qImTmpT2,qImTmpT4
        @/*CPLX_SUB (pTmp4, pTmpT2, pTmpT4);*/
        VSUB    qRe4,qReTmpT2,qReTmpT4
        VSUB    qIm4,qImTmpT2,qImTmpT4

        @/*CPLX_ADD (pT1, pTmp1, pTmp3);*/
        VADD    qInp1,qRe1,qRe3
        VADD    qInp2,qIm1,qIm3

        @/*CPLX_ADD_SUB_X (pT2, pTmp2, pTmp4);*/
        VADD    qInp3,qRe2,qIm4
        VSUB    qInp4,qIm2,qRe4

        @/*CPLX_SUB (pT3, pTmp1, pTmp3);*/
        VSUB    qInp5,qRe1,qRe3
        VSUB    qInp6,qIm1,qIm3
        @/*CPLX_SUB_ADD_X (pT4, pTmp2, pTmp4);*/
        VSUB    qInp7,qRe2,qIm4
        VADD    qInp8,qIm2,qRe4

        SUBS    setCount,#4
        @/* Store the Result*/

        VST2    {qInp1,qInp2},[pOut1],SubFFTSize
        VST2    {qInp3,qInp4},[pOut1],SubFFTSize

        VST2    {qInp5,qInp6},[pOut1],SubFFTSize
        VST2    {qInp7,qInp8},[pOut1],SubFFTSize

        SUB     pTw2,pTw2,TwdStep, LSL #1
        SUB     pOut1,pOut1,SubFFTSize, LSL #2

        ADD     pT1,pT1,#32
        ADD     pTw2,pTw2,#32
        ADD     pOut1,pOut1,#32

        BGT     fftSetLoop
        LSR     SubFFTSize,#1
        SUBS    grpCount,grpCount,#1
        ADD     grpStep,grpStep,SubFFTSize

        BGT     fftGrpLoop1
        @/* Update the Grp count and size for the next stage */
        ADD     twidStep,SubFFTSize,SubFFTSize, LSL #1
        LSRS    SubFFTNum,SubFFTNum,#2

        @/* Swap Input and Output*/
        MOV     pTmp,pDst
        MOV     pDst,pSrc
        MOV     pSrc,pTmp

        ADD     pCoef,pCoef,twidStep,LSL #1

        LSL     SubFFTSize,SubFFTSize,#2

        BGT     fftStageLoop

        @/* if the N is even power of 4, copy the output to dst buffer */
        ASR     fftSize,fftSize,#1
        CLZ     SubFFTNum,fftSize
        MOV     setCount, #32
        SUB     SubFFTNum, setCount, SubFFTNum
        ASR     SubFFTNum,SubFFTNum,#1
        ANDS    SubFFTNum, SubFFTNum, #1

        BNE     fftEnd

        ASR     grpCount, fftSize, #4
fftCopyLoop:
        VLD1.F32    {d0,d1,d2,d3},[pSrc]!
        VLD1.F32    {d4,d5,d6,d7},[pSrc]!
        VLD1.F32    {d8,d9,d10,d11},[pSrc]!
        VLD1.F32    {d12,d13,d14,d15},[pSrc]!

        SUBS        grpCount,#1
        VST1.F32    {d0,d1,d2,d3},[pDst]!
        VST1.F32    {d4,d5,d6,d7},[pDst]!
        VST1.F32    {d8,d9,d10,d11},[pDst]!
        VST1.F32    {d12,d13,d14,d15},[pDst]!

        BGT         fftCopyLoop

fftEnd:
        @/* Retureq From Function*/
        VPOP    {d8-d15}
        POP     {r4-r12,pc}

        @/*
        @ * @brief  Core radix-4 IFFT of floating-point data.  Do not call this function directly.
        @ * @param[out]  *pDst            points to the output buffer
        @ * @param[in]  *pSrc             points to the input buffer
        @ * @param[in]  N                 length of FFT
        @ * @param[in]  *pCoef            points to the twiddle factors
        @ * @retureq none.
        @ * The function implements a Radix-4 Complex FFT
        @ */

        .align   4
        .global   ne10_radix4_butterfly_inverse_float_neon
        .thumb
        .thumb_func

ne10_radix4_butterfly_inverse_float_neon:

        PUSH    {r4-r12,lr}
        VPUSH   {d8-d15}

        qInp1   .qn Q0.F32
        qInp2   .qn Q1.F32
        qInp3   .qn Q2.F32
        qInp4   .qn Q3.F32
        qInp5   .qn Q4.F32
        qInp6   .qn Q5.F32
        qInp7   .qn Q6.F32
        qInp8   .qn Q7.F32

        qTwd2Re .qn Q8.F32
        qTwd2Im .qn Q9.F32
        qTwd3Re .qn Q10.F32
        qTwd3Im .qn Q11.F32
        qTwd4Re .qn Q12.F32
        qTwd4Im .qn Q13.F32

        qReTmpT2 .qn Q14.F32
        qImTmpT2 .qn Q15.F32

        qReTmpT3 .qn Q2.F32
        qImTmpT3 .qn Q3.F32

        qReTmpT4 .qn Q4.F32
        qImTmpT4 .qn Q5.F32

        qRe1     .qn Q8.F32
        qIm1     .qn Q9.F32
        qRe2     .qn Q10.F32
        qIm2     .qn Q11.F32
        qRe3     .qn Q12.F32
        qIm3     .qn Q13.F32
        qRe4     .qn Q14.F32
        qIm4     .qn Q15.F32

        pDst        .req  R0
        pSrc        .req  R1
        fftSize     .req  R2
        pCoef       .req  R3


        SubFFTSize  .req  R4
        SubFFTNum   .req  R5
        grpCount    .req  R6
        twidStep    .req  R8
        setCount    .req  R9
        grpStep     .req  R10

        pT1         .req  R7
        pOut1       .req  R11
        pTw2        .req  R12
        TwdStep     .req  R14
        pTmp        .req  R7

        LSR     SubFFTNum,fftSize,#2
        MOV     SubFFTSize,#4
        MOV     pT1,pSrc
        LSR     grpCount,SubFFTNum,#2
        MOV     pOut1,pDst
        LSL     fftSize,#1

ifftGrpLoop:
        VLD2        {qInp1,qInp2},[pT1],fftSize  @/*Load Input Values*/
        VLD2        {qInp3,qInp4},[pT1],fftSize
        VLD2        {qInp5,qInp6},[pT1],fftSize
        VLD2        {qInp7,qInp8},[pT1],fftSize

        @/*pSrc[0] + pSrc[2]*/
        VADD    qRe1,qInp1,qInp5
        VADD    qIm1,qInp2,qInp6
        @/*pSrc[0] - pSrc[2]*/
        VSUB    qRe2,qInp1,qInp5
        VSUB    qIm2,qInp2,qInp6
        @/*pSrc[1] + pSrc[3]*/
        VADD    qRe3,qInp3,qInp7
        VADD    qIm3,qInp4,qInp8
        @/*pSrc[1] - pSrc[3]*/
        VSUB    qRe4,qInp3,qInp7
        VSUB    qIm4,qInp4,qInp8

        @/*Radix-4 Butterfly calculation*/
        @/*Third Result*/
        VSUB    qInp5,qRe1,qRe3
        VSUB    qInp6,qIm1,qIm3
        @/*First Result*/
        VADD    qInp1,qRe1,qRe3
        VADD    qInp2,qIm1,qIm3
        @/*Second result*/
        VSUB    qInp3,qRe2,qIm4
        VADD    qInp4,qIm2,qRe4
        @/*Fourth Result*/
        VADD    qInp7,qRe2,qIm4
        VSUB    qInp8,qIm2,qRe4

        @/*Get Result in correct order for storing*/
        @/*4Re2,4Re0,3Re2,3Re0 2Re2,2Re0,1Re2,1Re0*/
        VZIP    qInp1,qInp5
        @/*4Re3,4Re1,3Re3,3Re1 2Re3,2Re1,1Re3,1Re1*/
        VZIP    qInp3,qInp7

        @/*4Im2,4Im0,3Im2,3Im0 2Im2,2Im0,1Im2,1Im0*/
        VZIP    qInp2,qInp6
        @/*4Im3,4Im1,3Im2,3Im1 2Im3,2Im1,1Im3,1Im1*/
        VZIP    qInp4,qInp8


        SUB     pT1,pT1,fftSize, LSL #2


        VST4.F32    {d0,d2,d4,d6},[pOut1]!
        VST4.F32    {d1,d3,d5,d7},[pOut1]!
        SUBS        grpCount,#1
        ADD         pT1,pT1,#32
        VST4.F32    {d8,d10,d12,d14},[pOut1]!
        VST4.F32    {d9,d11,d13,d15},[pOut1]!


        BGT     ifftGrpLoop

        @/* Swap Input and Output*/
        MOV     pTmp,pDst
        MOV     pDst,pSrc
        MOV     pSrc,pTmp

        @/*Intermediate FFT Stages: Second Stage to Last but one Stage*/
        @/* Update the Grp count and size for the next stage */

        LSR     SubFFTNum,#2
        LSL     SubFFTSize,#2
        SUBS    pTmp, SubFFTNum, #1
        BEQ     ifftLastStageLoop

ifftStageLoop:
        MOV     grpCount,SubFFTNum
        MOV     grpStep,#0
        ADD     pT1,pSrc,fftSize
        LSL     TwdStep,SubFFTSize,#1

ifftGrpLoop1:
        LSR     setCount,SubFFTSize,#2
        ADD     pOut1,pDst,grpStep,LSL #3
        MOV     pTw2,pCoef

        LSL     SubFFTSize,#1

ifftSetLoop:
        VLD2    {qTwd2Re,qTwd2Im},[pTw2],TwdStep
        VLD2    {qInp3,qInp4},[pT1],fftSize
        @/*CPLX_MUL (pTmpT2, pTw2, pT2);*/
        VMUL    qReTmpT2,qTwd2Re,qInp3
        VMUL    qImTmpT2,qTwd2Re,qInp4
        VLD2    {qTwd3Re,qTwd3Im},[pTw2],TwdStep
        VLD2    {qInp5,qInp6},[pT1],fftSize
        VMLS    qReTmpT2,qTwd2Im,qInp4
        VMLA    qImTmpT2,qTwd2Im,qInp3


        @/*CPLX_MUL (pTmpT3, pTw3, pT3);*/
        VMUL    qReTmpT3,qTwd3Re,qInp5
        VMUL    qImTmpT3,qTwd3Re,qInp6
        VLD2    {qTwd4Re,qTwd4Im},[pTw2]
        VLD2    {qInp7,qInp8},[pT1],fftSize
        VMLS    qReTmpT3,qTwd3Im,qInp6
        VMLA    qImTmpT3,qTwd3Im,qInp5

        SUB     pT1,pT1,fftSize, LSL #2


        @/*CPLX_MUL (pTmpT4, pTw4, pT4);*/
        VMUL    qReTmpT4,qTwd4Re,qInp7
        VMUL    qImTmpT4,qTwd4Re,qInp8
        VLD2    {qInp1,qInp2},[pT1],fftSize
        VMLS    qReTmpT4,qTwd4Im,qInp8
        VMLA    qImTmpT4,qTwd4Im,qInp7


        @/*CPLX_ADD (pTmp1, pT1, pTmpT3);*/
        VADD    qRe1,qInp1,qReTmpT3
        VADD    qIm1,qInp2,qImTmpT3
        @/*CPLX_SUB (pTmp2, pT1, pTmpT3);*/
        VSUB    qRe2,qInp1,qReTmpT3
        VSUB    qIm2,qInp2,qImTmpT3
        @/*CPLX_ADD (pTmp3, pTmpT2, pTmpT4);*/
        VADD    qRe3,qReTmpT2,qReTmpT4
        VADD    qIm3,qImTmpT2,qImTmpT4
        @/*CPLX_SUB (pTmp4, pTmpT2, pTmpT4);*/
        VSUB    qRe4,qReTmpT2,qReTmpT4
        VSUB    qIm4,qImTmpT2,qImTmpT4

        @/*CPLX_ADD (pT1, pTmp1, pTmp3);*/
        VADD    qInp1,qRe1,qRe3
        VADD    qInp2,qIm1,qIm3

        @/*CPLX_SUB_ADD_X (pT2, pTmp2, pTmp4);*/
        VSUB    qInp3,qRe2,qIm4
        VADD    qInp4,qIm2,qRe4

        @/*CPLX_SUB (pT3, pTmp1, pTmp3);*/
        VSUB    qInp5,qRe1,qRe3
        VSUB    qInp6,qIm1,qIm3
        @/*CPLX_ADD_SUB_X (pT4, pTmp2, pTmp4);*/
        VADD    qInp7,qRe2,qIm4
        VSUB    qInp8,qIm2,qRe4

        SUBS    setCount,#4
        @/* Store the Result*/

        VST2    {qInp1,qInp2},[pOut1],SubFFTSize
        VST2    {qInp3,qInp4},[pOut1],SubFFTSize

        VST2    {qInp5,qInp6},[pOut1],SubFFTSize
        VST2    {qInp7,qInp8},[pOut1],SubFFTSize

        SUB     pTw2,pTw2,TwdStep, LSL #1
        SUB     pOut1,pOut1,SubFFTSize, LSL #2

        ADD     pT1,pT1,#32
        ADD     pTw2,pTw2,#32
        ADD     pOut1,pOut1,#32


        BGT     ifftSetLoop
        LSR     SubFFTSize,#1
        SUBS    grpCount,grpCount,#1
        ADD     grpStep,grpStep,SubFFTSize

        BGT     ifftGrpLoop1
        @/* Update the Grp count and size for the next stage */
        ADD     twidStep,SubFFTSize,SubFFTSize, LSL #1
        LSR     SubFFTNum,SubFFTNum,#2
        SUBS    pTmp, SubFFTNum, #1

        @/* Swap Input and Output*/
        MOV     pTmp,pDst
        MOV     pDst,pSrc
        MOV     pSrc,pTmp

        ADD     pCoef,pCoef,twidStep,LSL #1

        LSL     SubFFTSize,SubFFTSize,#2

        BGT     ifftStageLoop

        @/* last stage */
ifftLastStageLoop:
        MOV     grpStep,#0
        ADD     pT1,pSrc,fftSize
        LSL     TwdStep,SubFFTSize,#1

@ifftLastStageGrpLoop1:
        LSR     setCount,SubFFTSize,#2
        ADD     pOut1,pDst,grpStep,LSL #3
        MOV     pTw2,pCoef

        LSL     SubFFTSize,#1

ifftLastStageSetLoop:
        VLD2    {qTwd2Re,qTwd2Im},[pTw2],TwdStep
        VLD2    {qInp3,qInp4},[pT1],fftSize
        @/*CPLX_MUL (pTmpT2, pTw2, pT2);*/
        VMUL    qReTmpT2,qTwd2Re,qInp3
        VMUL    qImTmpT2,qTwd2Re,qInp4
        VLD2    {qTwd3Re,qTwd3Im},[pTw2],TwdStep
        VLD2    {qInp5,qInp6},[pT1],fftSize
        VMLS    qReTmpT2,qTwd2Im,qInp4
        VMLA    qImTmpT2,qTwd2Im,qInp3


        @/*CPLX_MUL (pTmpT3, pTw3, pT3);*/
        VMUL    qReTmpT3,qTwd3Re,qInp5
        VMUL    qImTmpT3,qTwd3Re,qInp6
        VLD2    {qTwd4Re,qTwd4Im},[pTw2]
        VLD2    {qInp7,qInp8},[pT1],fftSize
        VMLS    qReTmpT3,qTwd3Im,qInp6
        VMLA    qImTmpT3,qTwd3Im,qInp5

        SUB     pT1,pT1,fftSize, LSL #2


        @/*CPLX_MUL (pTmpT4, pTw4, pT4);*/
        VMUL    qReTmpT4,qTwd4Re,qInp7
        VMUL    qImTmpT4,qTwd4Re,qInp8
        VLD2    {qInp1,qInp2},[pT1],fftSize
        VMLS    qReTmpT4,qTwd4Im,qInp8
        VMLA    qImTmpT4,qTwd4Im,qInp7


        @/*CPLX_ADD (pTmp1, pT1, pTmpT3);*/
        VADD    qRe1,qInp1,qReTmpT3
        VADD    qIm1,qInp2,qImTmpT3
        @/*CPLX_SUB (pTmp2, pT1, pTmpT3);*/
        VSUB    qRe2,qInp1,qReTmpT3
        VSUB    qIm2,qInp2,qImTmpT3
        @/*CPLX_ADD (pTmp3, pTmpT2, pTmpT4);*/
        VADD    qRe3,qReTmpT2,qReTmpT4
        VADD    qIm3,qImTmpT2,qImTmpT4
        @/*CPLX_SUB (pTmp4, pTmpT2, pTmpT4);*/
        VSUB    qRe4,qReTmpT2,qReTmpT4
        VSUB    qIm4,qImTmpT2,qImTmpT4

        @/*CPLX_ADD (pT1, pTmp1, pTmp3);*/
        VADD    qInp1,qRe1,qRe3
        VADD    qInp2,qIm1,qIm3

        @/*CPLX_SUB_ADD_X (pT2, pTmp2, pTmp4);*/
        VSUB    qInp3,qRe2,qIm4
        VADD    qInp4,qIm2,qRe4

        @/*CPLX_SUB (pT3, pTmp1, pTmp3);*/
        VSUB    qInp5,qRe1,qRe3
        VSUB    qInp6,qIm1,qIm3
        @/*CPLX_ADD_SUB_X (pT4, pTmp2, pTmp4);*/
        VADD    qInp7,qRe2,qIm4
        VSUB    qInp8,qIm2,qRe4

        @/* multiply onebyN */
        ASR           grpCount,fftSize,#1          @revert the original value
        VDUP.S32      q8,grpCount
        VCVT.F32.S32  q8,  q8
        VRECPE.F32    q8,  q8

        VMUL    qInp1,qInp1,qRe1
        VMUL    qInp2,qInp2,qRe1
        VMUL    qInp3,qInp3,qRe1
        VMUL    qInp4,qInp4,qRe1
        VMUL    qInp5,qInp5,qRe1
        VMUL    qInp6,qInp6,qRe1
        VMUL    qInp7,qInp7,qRe1
        VMUL    qInp8,qInp8,qRe1

        SUBS    setCount,#4
        @/* Store the Result*/

        VST2    {qInp1,qInp2},[pOut1],SubFFTSize
        VST2    {qInp3,qInp4},[pOut1],SubFFTSize

        VST2    {qInp5,qInp6},[pOut1],SubFFTSize
        VST2    {qInp7,qInp8},[pOut1],SubFFTSize

        SUB     pTw2,pTw2,TwdStep, LSL #1
        SUB     pOut1,pOut1,SubFFTSize, LSL #2

        ADD     pT1,pT1,#32
        ADD     pTw2,pTw2,#32
        ADD     pOut1,pOut1,#32

        BGT     ifftLastStageSetLoop

        @/* Swap Input and Output*/
        MOV     pTmp,pDst
        MOV     pDst,pSrc
        MOV     pSrc,pTmp

        @/* if the N is even power of 4, copy the output to dst buffer */
        ASR     fftSize,fftSize,#1
        CLZ     SubFFTNum,fftSize
        MOV     setCount, #32
        SUB     SubFFTNum, setCount, SubFFTNum
        ASR     SubFFTNum,SubFFTNum,#1
        ANDS    SubFFTNum, SubFFTNum, #1

        BNE     ifftEnd

        ASR     grpCount, fftSize, #4

ifftCopyLoop:
        VLD1.F32    {d0,d1,d2,d3},[pSrc]!
        VLD1.F32    {d4,d5,d6,d7},[pSrc]!
        VLD1.F32    {d8,d9,d10,d11},[pSrc]!
        VLD1.F32    {d12,d13,d14,d15},[pSrc]!

        SUBS       grpCount,#1
        VST1.F32    {d0,d1,d2,d3},[pDst]!
        VST1.F32    {d4,d5,d6,d7},[pDst]!
        VST1.F32    {d8,d9,d10,d11},[pDst]!
        VST1.F32    {d12,d13,d14,d15},[pDst]!

        BGT        ifftCopyLoop

ifftEnd:
        @/* Retureq From Function*/
        VPOP    {d8-d15}
        POP     {r4-r12,pc}


        .end

