@
@  Copyright 2012 ARM Limited
@  All rights reserved.
@
@  Redistribution and use in source and binary forms, with or without
@  modification, are permitted provided that the following conditions are met:
@    * Redistributions of source code must retain the above copyright
@      notice, this list of conditions and the following disclaimer.
@    * Redistributions in binary form must reproduce the above copyright
@      notice, this list of conditions and the following disclaimer in the
@      documentation and/or other materials provided with the distribution.
@    * Neither the name of ARM Limited nor the
@      names of its contributors may be used to endorse or promote products
@      derived from this software without specific prior written permission.
@
@  THIS SOFTWARE IS PROVIDED BY ARM LIMITED AND CONTRIBUTORS "AS IS" AND
@  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
@  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
@  DISCLAIMED. IN NO EVENT SHALL ARM LIMITED BE LIABLE FOR ANY
@  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
@  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
@  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
@  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
@  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
@  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
@

@/*
@ * NE10 Library : dsp/NE10_fir.neon.s
@ */

@/*
@ * Note:
@ * 1. Currently, this is for soft VFP EABI, not for hard vfpv3 ABI yet
@ * 2. In the assembly code, we use D0-D31 registers. So VFPv3-D32 is used. In VFPv3-D16, there will be failure
@ */

        .text
        .syntax   unified

        @/**
        @ * @details
        @ * This function operates on floating-point data types.
        @ * There are no restrictions on numTaps and blockSize.
        @ *
        @ * The order of the coefficients in *coeffs should be
        @ * bN, bN-1, bN-2, .....b1, b0
        @ *
        @ * <b>Cycle Count:</b>
        @ *
        @ * <code>45 + 8 * numTaps + 12.25 * blockSize + 4.375 * numTaps * blockSize</code>
        @ *
        @ * when the block size > 32, the tap is > 4, you could get
        @ * maximized improvement
        @ *
        @ * @param[in]  *S                points to struct parameter
        @ * @param[in]  *pSrc             points to the input buffer
        @ * @param[out]  *pDst            points to the output buffer
        @ * @param[in]  blockSize         block size of filter
        @ */

        .align   4
        .global   ne10_fir_float_neon
        .extern   ne10_qMaskTable32
        .thumb
        .thumb_func

ne10_fir_float_neon:
                    PUSH    {r4-r12,lr}    @push r12: to keep stack 8 bytes aligned
@/*ARM Registers*/
pStateStruct     .req   R0
pSrc             .req   R1
pDst             .req   R2
blockSize        .req   R3

pState           .req   R4             @/* State pointer */
pCoeffs          .req   R5             @/* Coefficient pointer */
pStateCurnt      .req   R6             @/* Points to the current sample of the state */

pX               .req   R7             @/* Temporary pointers for state buffer */
pB               .req   R8             @/* Temporary pointers for coefficient buffer */
numTaps          .req   R9             @/* Length of the filter */

tapCnt           .req   R10            @ /* Loop counter */
Count            .req   R11            @ /* Loop counter */
pTemp            .req   R11
pMask            .req   R14            @  /* Mask Table */

mask             .req   R12

@/*NEON variale Declaration*/
qInp             .qn   Q0.F32
dInp_0           .dn   D0.F32
dInp_1           .dn   D1.F32
qCoeff           .qn   Q1.F32
dCoeff_0         .dn   D2.F32
dCoeff_1         .dn   D3.F32
qZero            .qn   Q2.F32

qMask            .qn   Q3.U32
dMask_0          .dn   D6.U32
dMask_1          .dn   D7.U32
dOut_0           .dn   D6.F32
dOut_1           .dn   D7.F32

qAcc0            .qn   Q8.F32
dAcc0_0          .dn   D16.F32
dAcc0_1          .dn   D17.F32


qTemp            .qn   Q9.F32
dTemp_0          .dn   D18.F32
dTemp_1          .dn   D19.F32

qTemp1           .qn   Q10.F32
dTemp1_0         .dn   D20.F32
dTemp1_1         .dn   D21.F32
qTemp2           .qn   Q11.F32
qTemp3           .qn   Q12.F32
qMask1           .qn   Q13.U32
dMask1_0         .dn   D26.U32
dMask1_1         .dn   D27.U32
qMaskTmp         .qn   Q14.U32
dMaskTmp_0       .dn   D28.U32
dMaskTmp_1       .dn   D29.U32

qAcc1            .qn   Q3.F32
qAcc2            .qn   Q13.F32
qAcc3            .qn   Q15.F32




                    LDRH        numTaps,[pStateStruct],#4
                    LDR         pState,[pStateStruct],#4
                    LDR         pCoeffs,[pStateStruct],#4

                    @/* S->state buffer contains previous frame (numTaps - 1) samples */
                    @/* pStateCurnt points to the location where the new input data should be written */
                    @/*pStateCurnt = &(S->state[(numTaps - 1u)])@*/
                    SUB         mask,numTaps,#1
                    LDR         pMask,=ne10_qMaskTable32
                    AND         tapCnt,numTaps,#3
                    ADD         pStateCurnt,pState,mask,LSL #2
                    AND         mask,blockSize,#3


                    @/* Apply loop unrolling and compute 4 output values simultaneously.
                    @* The variables acc0 ... acc3 hold output values that are being computed:
                    @*
                    @*    acc0 =  b[numTaps-1] * x[n-numTaps-1] + b[numTaps-2] * x[n-numTaps-2] + b[numTaps-3] * x[n-numTaps-3] +...+ b[0] * x[0]
                    @*    acc1 =  b[numTaps-1] * x[n-numTaps] +   b[numTaps-2] * x[n-numTaps-1] + b[numTaps-3] * x[n-numTaps-2] +...+ b[0] * x[1]
                    @*    acc2 =  b[numTaps-1] * x[n-numTaps+1] + b[numTaps-2] * x[n-numTaps] +   b[numTaps-3] * x[n-numTaps-1] +...+ b[0] * x[2]
                    @*    acc3 =  b[numTaps-1] * x[n-numTaps+2] + b[numTaps-2] * x[n-numTaps+1] + b[numTaps-3] * x[n-numTaps]   +...+ b[0] * x[3]
                    @*/

                    @/*If numTaps,blockSize are not  multiples of 4,  Get the appropriate Masks*/


                    ADD         pTemp,pMask,tapCnt,LSL #4
                    VEOR        qZero,qZero
                    ADD         pX,pMask,mask,LSL #4
                    VLD1        {dMaskTmp_0,dMaskTmp_1},[pTemp]
                    VLD1        {dMask1_0,dMask1_1},[pX]


                    @/* Copy blockCnt number of  new input samples into the state buffer */

                    SUBS        blockSize,#4
                    BLT         firEndOuterLoop

                    @/* Compute 4 outputs at a time*/

firOuterLoop:

                    VLD1        {dTemp_0,dTemp_1},[pSrc]!
                    MOV         pX,pState
                    MOV         pB,pCoeffs
                    @/* Read the first four samples from the state buffer:
                    @* x[n-numTaps], x[n-numTaps-1], x[n-numTaps-2],x[n-numTaps-3] */

                    VST1        {dTemp_0,dTemp_1},[pStateCurnt]!
                    @/* Zero the Accumulators*/
                    VEOR        qAcc0,qAcc0
                    VLD1        {dInp_0,dInp_1},[pX]!

                    @//* Read the first four coefficients b[numTaps] to b[numTaps-3] */
                    VLD1        {dCoeff_0,dCoeff_1},[pB]!
                    @/* Loop unrolling.  Process 4 taps at a time. */
                    SUBS        tapCnt,numTaps,#4
                    VLD1        {dTemp_0,dTemp_1},[pX]!

                    BLT         firEndInnerLoop

firInnerLoop:
                    VEXT        qTemp1,qInp,qTemp,#1
                    @/* acc0 +=  b[numTaps] * x[n-numTaps-1]+ b[numTaps] * x[n-numTaps-2] +
                    @* b[numTaps] * x[n-numTaps-3] +  b[numTaps] * x[n-numTaps-4]*/
                    VMLA        qAcc0,qInp,dCoeff_0[0]
                    @/* acc1 +=  b[numTaps-1] * x[n-numTaps-2]+ b[numTaps-1] * x[n-numTaps-3] +
                    @b[numTaps-1] * x[n-numTaps-4] +*b[numTaps-1] * x[n-numTaps-5]*/
                    VMUL        qAcc1,qTemp1,dCoeff_0[1]

                    VEXT        qTemp2,qInp,qTemp,#2
                    @/* acc2 +=  b[numTaps-2] * x[n-numTaps-3]+ b[numTaps-2] * x[n-numTaps-4] +
                    @b[numTaps-2] * x[n-numTaps-5] + *b[numTaps-2] * x[n-numTaps-6]*/
                    VMUL        qAcc2,qTemp2,dCoeff_1[0]
                    VADD        qAcc0, qAcc0, qAcc1

                    VEXT        qTemp3,qInp,qTemp,#3
                    @/* acc3 +=  b[numTaps-3] * x[n-numTaps-4]+ b[numTaps-3] * x[n-numTaps-5] +
                    @b[numTaps-3] * x[n-numTaps-6] +*b[numTaps-3] * x[n-numTaps-7]  */
                    VMUL        qAcc3,qTemp3,dCoeff_1[1]
                    VADD        qAcc0, qAcc0, qAcc2

                    VMOV        qInp,qTemp
                    VLD1        {dTemp_0,dTemp_1},[pX]!
                    VADD        qAcc0, qAcc0, qAcc3

                    SUBS        tapCnt,#4
                    @/* Read the b[numTaps-4] to b[numTaps-7]  coefficients */
                    VLD1        {dCoeff_0,dCoeff_1},[pB]!


                    BGE         firInnerLoop
firEndInnerLoop:

                    ADDS        tapCnt, tapCnt, #4
                    BEQ         firStoreOutput

                    @/* If the filter length is not a multiple of 4, compute the remaining filter taps */
                    @/*Select only the remaining filter Taps*/
                    VMOV        qMask,qMaskTmp
                    VBSL        qMask,qCoeff,qZero
                    VEXT        qTemp1,qInp,qTemp,#1
                    VMLA        qAcc0,qInp,dOut_0[0]
                    VEXT        qTemp2,qInp,qTemp,#2
                    VMLA        qAcc0,qTemp1,dOut_0[1]
                    VMLA        qAcc0,qTemp2,dOut_1[0]

firStoreOutput:
                    @/* Advance the state pointer by 4 to process the next group of 4 samples */
                    ADD         pState,#16

                    @/* The results in the 4 accumulators are in 2.30 format.  Convert to 1.31
                    @ * Then store the 4 outputs in the destination buffer. */
                    SUBS        blockSize,#4
                    VST1        {dAcc0_0,dAcc0_1},[pDst]!

                    BGE         firOuterLoop

firEndOuterLoop:
                    @/*Handle BlockSize Not a Multiple of 4*/
                    ADDS        blockSize,#4
                    BEQ         firCopyData
                    @/*Copy the Remaining BlockSize Number of Input Sample to state Buffer*/
                    VMOV        qMask,qMask1
                    VLD1        {dTemp1_0,dTemp1_1},[pStateCurnt]
                    VLD1        {dTemp_0,dTemp_1},[pSrc]

                    ADD         pSrc,pSrc,blockSize,LSL #2
                    MOV         pX,pState
                    MOV         pB,pCoeffs

                    VBSL        qMask,qTemp,qTemp1
                    VST1        {dMask_0,dMask_1},[pStateCurnt]
                    VLD1        {dInp_0,dInp_1},[pX]!

                    ADD         pStateCurnt,pStateCurnt,blockSize, LSL #2

                    @/* Zero the Accumulators*/
                    VEOR        qAcc0,qAcc0
                    VLD1        {dCoeff_0,dCoeff_1},[pB]!
                    SUBS        tapCnt,numTaps,#4
                    VLD1        {dTemp_0,dTemp_1},[pX]!

                    BLT         firEndInnerLoop1

firInnerLoop1:

                    VEXT        qTemp1,qInp,qTemp,#1
                    VMLA        qAcc0,qInp,dCoeff_0[0]
                    VEXT        qTemp2,qInp,qTemp,#2
                    VMLA        qAcc0,qTemp1,dCoeff_0[1]
                    VEXT        qTemp3,qInp,qTemp,#3
                    VMLA        qAcc0,qTemp2,dCoeff_1[0]
                    VMOV        qInp,qTemp
                    VMLA        qAcc0,qTemp3,dCoeff_1[1]
                    VLD1        {dCoeff_0,dCoeff_1},[pB]!
                    SUBS        tapCnt,#4
                    VLD1        {dTemp_0,dTemp_1},[pX]!

                    BGE         firInnerLoop1
firEndInnerLoop1:


                    VMOV        qMask,qMaskTmp
                    VBSL        qMask,qCoeff,qZero
                    VEXT        qTemp1,qInp,qTemp,#1
                    VMLA        qAcc0,qInp,dOut_0[0]
                    VEXT        qTemp2,qInp,qTemp,#2
                    VMLA        qAcc0,qTemp1,dOut_0[1]
                    VMLA        qAcc0,qTemp2,dOut_1[0]
                    VMOV        qMask,qMask1
                    VLD1        {dTemp_0,dTemp_1},[pDst]


                    @/* If the blockSize is not a multiple of 4, Mask the unwanted Output */

                    VBSL        qMask,qAcc0,qTemp
                    VST1        {dMask_0,dMask_1},[pDst]
                    ADD         pDst,pDst,blockSize,LSL #2
                    ADD         pState,pState,blockSize,LSL #2


firCopyData:
                    @/* Processing is complete.  Now shift the data in the state buffer down by
                    @** blockSize samples.  This prepares the state buffer for the next function
                    @** call. */

                    @/* Points to the start of the state buffer */

                    SUB         numTaps,numTaps,#1
                    AND         mask,numTaps,#3
                    LDR         pStateCurnt,[pStateStruct,#-8]
                    ADD         pTemp,pMask,mask,LSL #4
                    VLD1        {dInp_0,dInp_1},[pState]!
                    VLD1        {dMask_0,dMask_1},[pTemp]


                    @/* copy data */

                    SUBS        Count,numTaps,#4
                    BLT         firEnd
firCopyLoop:
                    VST1        {dInp_0,dInp_1},[pStateCurnt]!
                    SUBS        Count,#4
                    VLD1        {dInp_0,dInp_1},[pState]!
                    BGE         firCopyLoop

firEnd:

                    VLD1        {dTemp_0,dTemp_1},[pStateCurnt]
                    VBSL        qMask,qInp,qTemp
                    VST1        {dOut_0,dOut_1},[pStateCurnt]
                    ADD         pStateCurnt,pStateCurnt,mask, LSL #2

                    @/*Return From Function*/
                    POP     {r4-r12,pc}
@/*ARM Registers*/
.unreq    pStateStruct
.unreq    pSrc
.unreq    pDst
.unreq    blockSize

.unreq    pState
.unreq    pCoeffs
.unreq    pStateCurnt

.unreq    pX
.unreq    pB
.unreq    numTaps

.unreq    tapCnt
.unreq    Count
.unreq    pTemp
.unreq    pMask

.unreq    mask

@/*NEON variale Declaration*/
.unreq    qInp
.unreq    dInp_0
.unreq    dInp_1
.unreq    qCoeff
.unreq    dCoeff_0
.unreq    dCoeff_1
.unreq    qZero

.unreq    qMask
.unreq    dMask_0
.unreq    dMask_1
.unreq    dOut_0
.unreq    dOut_1

.unreq    qAcc0
.unreq    dAcc0_0
.unreq    dAcc0_1

.unreq    qTemp
.unreq    dTemp_0
.unreq    dTemp_1

.unreq    qTemp1
.unreq    dTemp1_0
.unreq    dTemp1_1
.unreq    qTemp2
.unreq    qTemp3
.unreq    qMask1
.unreq    dMask1_0
.unreq    dMask1_1
.unreq    qMaskTmp
.unreq    dMaskTmp_0
.unreq    dMaskTmp_1

.unreq    qAcc1
.unreq    qAcc2
.unreq    qAcc3

        @/**
        @ * @details
        @ * This function operates on floating-point data types.
        @ * There are no restrictions on numTaps and blockSize.
        @ *
        @ * The order of the coefficients in *coeffs should be
        @ * bN, bN-1, bN-2, .....b1, b0
        @ *
        @ * <b>Cycle Count:</b>
        @ *
        @ * <code> Co + C1 * numTaps + C3 * blockSize * decimation Factor + c4 * numTaps * blockSize</code>
        @ *
        @ * when the block size > 32, the tap > 4, you could get
        @ * maximized improvement
        @ *
        @ * @param[in]  *S                points to struct parameter
        @ * @param[in]  *pSrc             points to the input buffer
        @ * @param[out]  *pDst            points to the output buffer
        @ * @param[in]  blockSize         block size of filter
        @ */

        .align   4
        .global   ne10_fir_decimate_float_neon
        .extern   ne10_qMaskTable32
        .extern   ne10_divLookUpTable
        .thumb
        .thumb_func

ne10_fir_decimate_float_neon:

                            PUSH    {r4-r12,lr}    @push r12: to keep stack 8 bytes aligned
                            VPUSH   {d8-d9}

@/*ARM Registers*/
pStateStruct     .req   R0
pSrc             .req   R1
pDst             .req   R2
blockSize        .req   R3

pState           .req   R4             @/* State pointer */
pCoeffs          .req   R5             @/* Coefficient pointer */
decimationFact   .req   R6
outBlockSize     .req   R7

pX               .req   R6             @/* Temporary pointers for state buffer */
pB               .req   R8             @/* Temporary pointers for coefficient buffer */
numTaps          .req   R9             @/* Length of the filter */

tapCnt           .req   R10            @ /* Loop counter */
Count            .req   R11            @ /* Loop counter */
pTemp            .req   R11
blkCnt           .req   R11
pMask            .req   R14            @  /* Mask Table */

mask             .req   R12
Offset           .req   R12

@/*NEON variale Declaration*/
qInp0            .qn   Q0.F32
dInp0_0          .dn   D0.F32
dInp0_1          .dn   D1.F32

qCoeff           .qn   Q1.F32
dCoeff_0         .dn   D2.F32
dCoeff_1         .dn   D3.F32

qZero            .qn   Q2.F32
qMask            .qn   Q3.U32
qMaskF32         .qn   Q3.F32
dMask_0          .dn   D6.U32
dMask_1          .dn   D7.U32
dOut_0           .dn   D6.F32
dOut_1           .dn   D7.F32

qInp3            .qn   Q4.F32
dInp3_0          .dn   D8.F32
dInp3_1          .dn   D9.F32

qAcc0            .qn   Q8.F32
dAcc0_0          .dn   D16.F32
dAcc0_1          .dn   D17.F32


qTemp            .qn   Q9.F32
dTemp_0          .dn   D18.F32
dTemp_1          .dn   D19.F32

qInp1            .qn   Q9.F32
dInp1_0          .dn   D18.F32
dInp1_1          .dn   D19.F32

qAcc1            .qn   Q10.F32
dAcc1_0          .dn   D20.F32
dAcc1_1          .dn   D21.F32
qAcc2            .qn   Q11.F32
dAcc2_0          .dn   D22.F32
dAcc2_1          .dn   D23.F32
qAcc3            .qn   Q12.F32
dAcc3_0          .dn   D24.F32
dAcc3_1          .dn   D25.F32

qMask1           .qn   Q13.U32
dMask1_0         .dn   D26.U32
dMask1_1         .dn   D27.U32

qMaskTmp         .qn   Q14.U32
dMaskTmp_0       .dn   D28.U32
dMaskTmp_1       .dn   D29.U32


qInp2            .qn   Q15.F32
dInp2_0          .dn   D30.F32
dInp2_1          .dn   D31.F32




                    LDRB        decimationFact,[pStateStruct],#2
                    LDRH        numTaps,[pStateStruct],#2
                    LDR         pCoeffs,[pStateStruct],#4
                    LDR         pState,[pStateStruct],#4

                    @//outBlockSize = blockSize / S->M
                    LDR         pTemp,=ne10_divLookUpTable
                    SUBS        mask,decimationFact,#1
                    ADD         pTemp,pTemp,mask,LSL #2
                    LDR         mask,[pTemp]
                    @//MOV         pX,#0


                    SMULWB      outBlockSize,blockSize,mask
                    CMP         outBlockSize,#0
                    IT          LT
                    RSBLT       outBlockSize,#0


                    @/* S->state buffer contains previous frame (numTaps - 1) samples */
                    @/* pStateCurnt points to the location where the new input data should be written */
                    @//pStateCurnt = S->state + (numTaps - 1u)@


                    @/* Copy Blocksize number of new input samples into the state buffer */

                    LDR         pMask,=ne10_qMaskTable32
                    SUB         tapCnt,numTaps,#1
                    AND         mask,blockSize,#3

                    ADD         pB,pState,tapCnt,LSL #2
                    ADD         mask,pMask,mask,LSL #4
                    VLD1        {dTemp_0,dTemp_1},[pSrc]!
                    VLD1        {dMask1_0,dMask1_1},[mask]


                    SUBS        Count,blockSize,#4
                    LSL         Offset,decimationFact, #2
                    VMOV        qMask,qMask1

                    BLT         firDecimateEndCopy
firDecimateCopyLoop:

                    VST1        {dTemp_0,dTemp_1},[pB]!
                    SUBS        Count,#4
                    VLD1        {dTemp_0,dTemp_1},[pSrc]!
                    BGE         firDecimateCopyLoop
firDecimateEndCopy:
                    VLD1        {dCoeff_0,dCoeff_1},[pB]

                    VBSL        qMask,qTemp,qCoeff
                    VST1        {dMask_0,dMask_1},[pB]
                    ADD         pB,pB,tapCnt,LSL #2

                    @// Load Mask Value
                    AND         blkCnt,outBlockSize,#3
                    ADD         blkCnt,pMask,blkCnt,LSL #4
                    VLD1        {dMask1_0,dMask1_1},[blkCnt]

                    @/*Load Mask Table Values*/

                    AND         tapCnt,numTaps,#3
                    ADD         pTemp,pMask,tapCnt,LSL #4
                    VEOR        qZero,qZero,qZero
                    VLD1        {dMaskTmp_0,dMaskTmp_1},[pTemp]

                    @/*Handle 4 output samples at a time */
                    SUBS        blkCnt,outBlockSize,#4
                    BLT        firDecimateEndOuterLoop

                    @//blkCnt = outBlockSize>>2@
firDecimateOuterLoop:
                    @/* Set accumulator to zero */
                    VEOR        qAcc0,qAcc0,qAcc0
                    VEOR        qAcc1,qAcc1,qAcc1
                    VEOR        qAcc2,qAcc2,qAcc2
                    VEOR        qAcc3,qAcc3,qAcc3
                    @/* Initialize state pointer */
                    MOV         pX,pState
                    @/* Initialize coeff pointer */
                    MOV         pB,pCoeffs

                    SUBS        tapCnt,numTaps,#4
                    VLD1        {dCoeff_0,dCoeff_1},[pB]!

                    VLD1        {dInp0_0,dInp0_1},[pX],Offset
                    VLD1        {dInp1_0,dInp1_1},[pX],Offset
                    VLD1        {dInp2_0,dInp2_1},[pX],Offset
                    VLD1        {dInp3_0,dInp3_1},[pX],Offset
                    SUB         pX,pX,Offset, LSL #2
                    ADD         pX,pX,#16

                    BLT         firDecimateEndInnerLoop
firDecimateInnerLoop:
                    VMLA        qAcc0,qCoeff,qInp0
                    VMLA        qAcc1,qCoeff,qInp1
                    VMLA        qAcc2,qCoeff,qInp2
                    VMLA        qAcc3,qCoeff,qInp3

                    VLD1        {dCoeff_0,dCoeff_1},[pB]!
                    VLD1        {dInp0_0,dInp0_1},[pX],Offset
                    VLD1        {dInp1_0,dInp1_1},[pX],Offset
                    VLD1        {dInp2_0,dInp2_1},[pX],Offset
                    VLD1        {dInp3_0,dInp3_1},[pX],Offset
                    SUB         pX,pX,Offset, LSL #2
                    ADD         pX,pX,#16

                    SUBS        tapCnt,#4
                    BGE         firDecimateInnerLoop
firDecimateEndInnerLoop:
                    @/* If the filter length is not a multiple of 4, compute the remaining filter taps */

                    VMOV        qMask,qMaskTmp
                    VBSL        qMask,qCoeff,qZero

                    VMLA        qAcc0,qMaskF32,qInp0
                    VMLA        qAcc1,qMaskF32,qInp1
                    VMLA        qAcc2,qMaskF32,qInp2
                    VMLA        qAcc3,qMaskF32,qInp3

                    VADD        dAcc0_0,dAcc0_0,dAcc0_1
                    VADD        dAcc1_0,dAcc1_0,dAcc1_1
                    VADD        dAcc2_0,dAcc2_0,dAcc2_1
                    VADD        dAcc3_0,dAcc3_0,dAcc3_1

                    VPADD       dAcc0_0,dAcc0_0,dAcc1_0
                    VPADD       dAcc0_1,dAcc2_0,dAcc3_0
                    ADD         pState,pState,Offset,LSL #2
                    VST1        {dAcc0_0,dAcc0_1},[pDst]!

                    SUBS        blkCnt,#4
                    BGE         firDecimateOuterLoop

firDecimateEndOuterLoop:
                    @/*Handle BlockSize Not a Multiple of 4*/
                    ADDS        blkCnt,#4
                    BEQ         firDecimateCopyData


                    @/* Set accumulator to zero */
                    VEOR        qAcc0,qAcc0,qAcc0
                    VEOR        qAcc1,qAcc1,qAcc1
                    VEOR        qAcc2,qAcc2,qAcc2
                    VEOR        qAcc3,qAcc3,qAcc3
                    @/* Initialize state pointer */
                    MOV         pX,pState
                    @/* Initialize coeff pointer */
                    MOV         pB,pCoeffs
                    SUBS        tapCnt,numTaps,#4
                    VLD1        {dCoeff_0,dCoeff_1},[pB]!

                    VLD1        {dInp0_0,dInp0_1},[pX],Offset
                    VLD1        {dInp1_0,dInp1_1},[pX],Offset
                    VLD1        {dInp2_0,dInp2_1},[pX],Offset
                    VLD1        {dInp3_0,dInp3_1},[pX],Offset
                    SUB         pX,pX,Offset, LSL #2
                    ADD         pX,pX,#16

                    BLT         firDecimateEndInnerLoop1
firDecimateInnerLoop1:
                    VMLA        qAcc0,qCoeff,qInp0
                    VMLA        qAcc1,qCoeff,qInp1
                    VMLA        qAcc2,qCoeff,qInp2
                    VMLA        qAcc3,qCoeff,qInp3

                    VLD1        {dCoeff_0,dCoeff_1},[pB]!
                    VLD1        {dInp0_0,dInp0_1},[pX],Offset
                    VLD1        {dInp1_0,dInp1_1},[pX],Offset
                    VLD1        {dInp2_0,dInp2_1},[pX],Offset
                    VLD1        {dInp3_0,dInp3_1},[pX],Offset
                    SUB         pX,pX,Offset, LSL #2
                    ADD         pX,pX,#16

                    SUBS        tapCnt,#4
                    BGE         firDecimateInnerLoop1
firDecimateEndInnerLoop1:
                    @/* If the filter length is not a multiple of 4, compute the remaining filter taps */

                    VMOV        qMask,qMaskTmp
                    VBSL        qMask,qCoeff,qZero

                    VMLA        qAcc0,qMaskF32,qInp0
                    VMLA        qAcc1,qMaskF32,qInp1
                    VMLA        qAcc2,qMaskF32,qInp2
                    VMLA        qAcc3,qMaskF32,qInp3

                    VADD        dAcc0_0,dAcc0_0,dAcc0_1
                    VADD        dAcc1_0,dAcc1_0,dAcc1_1
                    VADD        dAcc2_0,dAcc2_0,dAcc2_1
                    VADD        dAcc3_0,dAcc3_0,dAcc3_1


                    MUL         Offset,Offset,blkCnt
                    VPADD       dAcc0_0,dAcc0_0,dAcc1_0
                    VPADD       dAcc0_1,dAcc2_0,dAcc3_0
                    ADD         pState,pState,Offset

                    VMOV        qMask,qMask1
                    VLD1        {dTemp_0,dTemp_1},[pDst]
                    VBSL        qMask,qAcc0,qTemp

                    VST1        {dMask_0,dMask_1},[pDst]
                    ADD         pDst,pDst,blkCnt,LSL #2


firDecimateCopyData:
                    @/* Processing is complete.  Now shift the data in the state buffer down by
                    @** blockSize samples.  This prepares the state buffer for the next function
                    @** call. */

                    @/* Points to the start of the state buffer */

                    SUB         numTaps,numTaps,#1
                    AND         mask,numTaps,#3
                    LDR         pX,[pStateStruct,#-4]
                    ADD         pTemp,pMask,mask,LSL #4
                    VLD1        {dInp0_0,dInp0_1},[pState]!
                    VLD1        {dMask_0,dMask_1},[pTemp]

                    @/* copy data */

                    SUBS        Count,numTaps,#4
                    BLT         firDecimateEnd
firDecimateCopyLoop1:
                    VST1        {dInp0_0,dInp0_1},[pX]!
                    SUBS        Count,#4
                    VLD1        {dInp0_0,dInp0_1},[pState]!
                    BGE         firDecimateCopyLoop1
firDecimateEnd:
                    VLD1        {dTemp_0,dTemp_1},[pX]
                    VBSL        qMask,qInp0,qTemp
                    VST1        {dOut_0,dOut_1},[pX]
                    ADD         pX,pX,mask, LSL #2

                    @// Return From Function
                    VPOP    {d8-d9}
                    POP     {r4-r12,pc}

@/*ARM Registers*/
.unreq    pStateStruct
.unreq    pSrc
.unreq    pDst
.unreq    blockSize

.unreq    pState
.unreq    pCoeffs
.unreq    decimationFact
.unreq    outBlockSize

.unreq    pX
.unreq    pB
.unreq    numTaps

.unreq    tapCnt
.unreq    Count
.unreq    pTemp
.unreq    blkCnt
.unreq    pMask

.unreq    mask
.unreq    Offset

@/*NEON variale Declaration*/
.unreq    qInp0
.unreq    dInp0_0
.unreq    dInp0_1

.unreq    qCoeff
.unreq    dCoeff_0
.unreq    dCoeff_1

.unreq    qZero
.unreq    qMask
.unreq    qMaskF32
.unreq    dMask_0
.unreq    dMask_1
.unreq    dOut_0
.unreq    dOut_1

.unreq    qInp3
.unreq    dInp3_0
.unreq    dInp3_1

.unreq    qAcc0
.unreq    dAcc0_0
.unreq    dAcc0_1

.unreq    qTemp
.unreq    dTemp_0
.unreq    dTemp_1

.unreq    qInp1
.unreq    dInp1_0
.unreq    dInp1_1

.unreq    qAcc1
.unreq    dAcc1_0
.unreq    dAcc1_1
.unreq    qAcc2
.unreq    dAcc2_0
.unreq    dAcc2_1
.unreq    qAcc3
.unreq    dAcc3_0
.unreq    dAcc3_1

.unreq    qMask1
.unreq    dMask1_0
.unreq    dMask1_1

.unreq    qMaskTmp
.unreq    dMaskTmp_0
.unreq    dMaskTmp_1

.unreq    qInp2
.unreq    dInp2_0
.unreq    dInp2_1


        @/**
        @ * @details
        @ * This function operates on floating-point data types.
        @ * There are no restrictions on numTaps and blockSize.
        @ *
        @ * The order of the coefficients in *coeffs should be
        @ * bN, bN-1, bN-2, .....b1, b0
        @ *
        @ * <b>Cycle Count:</b>
        @ *
        @ * when the S->tapnumTaps/S->L is big , you could get
        @ * maximized improvement
        @ *
        @ * <code> C0 + C2 * blockSize + C3 * blockSize * interpolateFactor + C4 * numTaps * blockSize * interpolateFactor </code>
        @ *
        @ * @param[in]  *S                points to struct parameter
        @ * @param[in]  *pSrc             points to the input buffer
        @ * @param[out]  *pDst            points to the output buffer
        @ * @param[in]  blockSize         block size of filter
        @ */

        .align   4
        .global   ne10_fir_interpolate_float_neon
        .extern   ne10_qMaskTable32
        .thumb
        .thumb_func

ne10_fir_interpolate_float_neon:
                            PUSH    {r4-r12,lr}    @push r12: to keep stack 8 bytes aligned


@/*ARM Registers*/
pStateStruct     .req   R0
pSrc             .req   R1
pDst             .req   R2
blockSize        .req   R3

pState           .req   R4             @/* State pointer */

pB               .req   R5             @/* Temporary pointers for coefficient buffer */
pCoeffs          .req   R5             @/* Coefficient pointer */
pStateCurnt      .req   R5             @/* Points to the current sample of the state */

pX               .req   R6             @/* Temporary pointers for state buffer */

interpolationFact .req  R7
intFact          .req   R8
phaseLen         .req   R9
Offset           .req   R10

Count            .req   R11            @ /* Loop counter */
pTemp            .req   R11

mask             .req   R12

pMask            .req   R14            @  /* Mask Table */
index            .req   R14

@/*NEON variale Declaration*/
qInp             .qn   Q0.F32
dInp_0           .dn   D0.F32
dInp_1           .dn   D1.F32
qCoeff0          .qn   Q1.F32
dCoeff0_0        .dn   D2.F32
dCoeff0_1        .dn   D3.F32
qZero            .qn   Q2.F32

qMask            .qn   Q3.U32
dMask_0          .dn   D6.U32
dMask_1          .dn   D7.U32
dOut_0           .dn   D6.F32
dOut_1           .dn   D7.F32

qAcc0            .qn   Q8.F32
dAcc0_0          .dn   D16.F32
dAcc0_1          .dn   D17.F32


qTemp            .qn   Q9.F32
dTemp_0          .dn   D18.F32
dTemp_1          .dn   D19.F32

qCoeff1          .qn   Q10.F32
dCoeff1_0        .dn   D20.F32
dCoeff1_1        .dn   D21.F32
qCoeff2          .qn   Q11.F32
dCoeff2_0        .dn   D22.F32
dCoeff2_1        .dn   D23.F32
qCoeff3          .qn   Q12.F32
dCoeff3_0        .dn   D24.F32
dCoeff3_1        .dn   D25.F32

qMask1           .qn   Q13.F32
dMask1_0         .dn   D26.F32
dMask1_1         .dn   D27.F32


qMaskTemp         .qn   Q14.U32
dMaskTemp_0       .dn   D28.U32
dMaskTemp_1       .dn   D29.U32

                    LDRB        interpolationFact,[pStateStruct],#2
                    LDRH        phaseLen,[pStateStruct],#2
                    LDR         pCoeffs,[pStateStruct],#4
                    LDR         pState,[pStateStruct],#4

                    LSL         Offset,interpolationFact, #2



                    @/* S->state buffer contains previous frame (phaseLen - 1) samples */
                    @/* pStateCurnt points to the location where the new input data should be written */


                    AND         phaseLen,#3
                    LDR         pMask,=ne10_qMaskTable32

                    @/* Total number of intput samples */
                    @/*Load Mask Value*/

                    AND         mask,interpolationFact,#3
                    ADD         pTemp,pMask,phaseLen,LSL #4
                    ADD         mask,pMask,mask,LSL #4

                    VLD1        {dMaskTemp_0,dMaskTemp_1},[pTemp]
                    VLD1        {dMask1_0,dMask1_1},[mask]


                    VEOR        qZero,qZero,qZero



                    @/* Loop over the blockSize. */
                    CMP         blockSize,#0
                    BEQ         firInterpolateCopyData
firInterpolateBlockLoop:
                    @/* Copy new input sample into the state buffer */
                    LDRH        phaseLen,[pStateStruct,#-10]
                    LDR         mask,[pSrc],#4
                    SUB         phaseLen,#1
                    ADD         pStateCurnt,pState,phaseLen, LSL #2

                    LDRB        interpolationFact,[pStateStruct,#-12]
                    STR         mask,[pStateCurnt]


                    SUBS        intFact,interpolationFact,#4
                    MOV         index,#4

                    BLT         firInterpolateEndIntplLoop
firInterpolateInterpolLoop:
                    VEOR        qAcc0,qAcc0,qAcc0
                    LDRH        phaseLen,[pStateStruct,#-10]
                    LDR         pCoeffs,[pStateStruct,#-8]
                    MOV         pX,pState
                    SUB         mask,interpolationFact,index
                    ADD         pB,pCoeffs,mask, LSL #2
                    @/*Load Coefficients*/
                    @/*c0 c1 c2 c3*/
                    VLD1        {dCoeff0_0,dCoeff0_1},[pB],Offset
                    @/*c0 c1 c2 c3*/
                    VLD1        {dCoeff1_0,dCoeff1_1},[pB],Offset
                    @/*c0 c1 c2 c3*/
                    VLD1        {dCoeff2_0,dCoeff2_1},[pB],Offset
                    @/*c0 c1 c2 c3*/
                    VLD1        {dCoeff3_0,dCoeff3_1},[pB],Offset
                    VLD1        {dInp_0,dInp_1},[pX]!
                    @/* Loop over the polyPhase length. Unroll by a factor of 4.
                    @  ** Repeat until we've computed numTaps-(4*S->L) coefficients. */
                    SUBS        phaseLen,#4
                    BLT         firInterpolateEndPhaseLoop
firInterpolatePhaseLoop:
                    @/* Perform the multiply-accumulate */
                    VMLA        qAcc0,qCoeff0,dInp_0[0]
                    VMLA        qAcc0,qCoeff1,dInp_0[1]
                    VMLA        qAcc0,qCoeff2,dInp_1[0]
                    VMLA        qAcc0,qCoeff3,dInp_1[1]

                    VLD1        {dInp_0,dInp_1},[pX]!
                    @ /*Load Coefficients*/
                    @/*c0 c1 c2 c3*/
                    VLD1        {dCoeff0_0,dCoeff0_1},[pB],Offset
                    @/*c0 c1 c2 c3*/
                    VLD1        {dCoeff1_0,dCoeff1_1},[pB],Offset
                    @/*c0 c1 c2 c3*/
                    VLD1        {dCoeff2_0,dCoeff2_1},[pB],Offset
                    @/*c0 c1 c2 c3*/
                    VLD1        {dCoeff3_0,dCoeff3_1},[pB],Offset

                    SUBS        phaseLen,#4
                    BGE         firInterpolatePhaseLoop
firInterpolateEndPhaseLoop:
                    @/* If the polyPhase length is not a multiple of 4, compute the remaining filter taps */
                    VMOV        qMask,qMaskTemp
                    VBSL        qMask,qInp,qZero
                    @/* Perform the multiply-accumulate */
                    VMLA        qAcc0,qCoeff0,dOut_0[0]
                    VMLA        qAcc0,qCoeff1,dOut_0[1]
                    VMLA        qAcc0,qCoeff2,dOut_1[0]
                    VMLA        qAcc0,qCoeff3,dOut_1[1]
                    @ /* The result is in the accumulator is in Reverse Order*/
                    VREV64      qAcc0,qAcc0
                    @/*Swap the D-Regs of Acc*/
                    VMOV        dCoeff0_0,dAcc0_1
                    VMOV        dCoeff0_1,dAcc0_0

                    VST1        {dCoeff0_0,dCoeff0_1},[pDst]!
                    ADD         index,#4
                    SUBS        intFact,#4
                    BGE         firInterpolateInterpolLoop

firInterpolateEndIntplLoop:
                    ADDS        intFact,#4
                    BEQ         firInterpolateNextSample
                    @/*Handle the Remaining Samples*/
                    VEOR        qAcc0,qAcc0,qAcc0
                    LDRH        phaseLen,[pStateStruct,#-10]
                    LDR         pCoeffs,[pStateStruct,#-8]
                    MOV         pX,pState
                    SUB         mask,interpolationFact,index
                    ADD         pB,pCoeffs,mask, LSL #2
                    @/*Load Coefficients*/
                    @/*c0 c1 c2 c3*/
                    VLD1        {dCoeff0_0,dCoeff0_1},[pB],Offset
                    @/*c0 c1 c2 c3*/
                    VLD1        {dCoeff1_0,dCoeff1_1},[pB],Offset
                    @/*c0 c1 c2 c3*/
                    VLD1        {dCoeff2_0,dCoeff2_1},[pB],Offset
                    @/*c0 c1 c2 c3*/
                    VLD1        {dCoeff3_0,dCoeff3_1},[pB],Offset
                    VLD1        {dInp_0,dInp_1},[pX]!
                    @/* Loop over the polyPhase length. Unroll by a factor of 4.
                    @  ** Repeat until we've computed numTaps-(4*S->L) coefficients. */
                    SUBS        phaseLen,#4
                    BLT         firInterpolateEndPhaseLoop1
firInterpolatePhaseLoop1:
                    @/* Perform the multiply-accumulate */
                    VMLA        qAcc0,qCoeff0,dInp_0[0]
                    VMLA        qAcc0,qCoeff1,dInp_0[1]
                    VMLA        qAcc0,qCoeff2,dInp_1[0]
                    VMLA        qAcc0,qCoeff3,dInp_1[1]

                    VLD1        {dInp_0,dInp_1},[pX]!
                    @ /*Load Coefficients*/
                    @/*c0 c1 c2 c3*/
                    VLD1        {dCoeff0_0,dCoeff0_1},[pB],Offset
                    @/*c0 c1 c2 c3*/
                    VLD1        {dCoeff1_0,dCoeff1_1},[pB],Offset
                    @/*c0 c1 c2 c3*/
                    VLD1        {dCoeff2_0,dCoeff2_1},[pB],Offset
                    @/*c0 c1 c2 c3*/
                    VLD1        {dCoeff3_0,dCoeff3_1},[pB],Offset
                    SUBS        phaseLen,#4
                    BGE         firInterpolatePhaseLoop1

firInterpolateEndPhaseLoop1:
                    @/* If the polyPhase length is not a multiple of 4, compute the remaining filter taps */
                    VMOV        qMask,qMaskTemp
                    VBSL        qMask,qInp,qZero
                    @/* Perform the multiply-accumulate */
                    VMLA        qAcc0,qCoeff0,dOut_0[0]
                    VMLA        qAcc0,qCoeff1,dOut_0[1]
                    VMLA        qAcc0,qCoeff2,dOut_1[0]
                    VMLA        qAcc0,qCoeff3,dOut_1[1]
                    @ /* The result is in the accumulator is in Reverse Order*/
                    VREV64      qAcc0,qAcc0

                    VMOV        qMask,qMask1
                    VLD1        {dTemp_0,dTemp_1},[pDst]
                    @/*Swap the D-Regs of Acc*/
                    VMOV        dCoeff0_0,dAcc0_1
                    VMOV        dCoeff0_1,dAcc0_0

                    VBSL        qMask,qCoeff0,qTemp
                    VST1        {dMask_0,dMask_1},[pDst]
                    ADD         pDst,pDst,intFact, LSL #2


firInterpolateNextSample:
                    SUBS        blockSize,#1
                    ADD         pState,#4
                    BGT         firInterpolateBlockLoop

                    @/*End of Processing*/

firInterpolateCopyData:

                    @/* Save previous phaseLen - 1 samples and get rid of other samples  */
                    @/* Points to the start of the state buffer */
                    LDRH        phaseLen,[pStateStruct,#-10]
                    LDR         pMask,=ne10_qMaskTable32
                    LDR         pStateCurnt,[pStateStruct,#-4]

                    SUB         phaseLen,phaseLen,#1
                    AND         mask,phaseLen,#3
                    ADD         pTemp,pMask,mask,LSL #4

                    VLD1        {dInp_0,dInp_1},[pState]!
                    VLD1        {dMask_0,dMask_1},[pTemp]

                    @/* copy data */

                    SUBS        Count,phaseLen,#4
                    BLT         firInterpolateEnd
firInterpolateCopyLoop:
                    VST1        {dInp_0,dInp_1},[pStateCurnt]!
                    SUBS        Count,#4
                    VLD1        {dInp_0,dInp_1},[pState]!
                    BGE         firInterpolateCopyLoop
firInterpolateEnd:

                    VLD1        {dTemp_0,dTemp_1},[pStateCurnt]
                    VBSL        qMask,qInp,qTemp
                    VST1        {dOut_0,dOut_1},[pStateCurnt]

                    ADD         pStateCurnt,pStateCurnt,mask, LSL #2

                    @/*Return From Function*/
                    POP     {r4-r12,pc}
@/*ARM Registers*/
.unreq    pStateStruct
.unreq    pSrc
.unreq    pDst
.unreq    blockSize

.unreq    pState

.unreq    pB
.unreq    pCoeffs
.unreq    pStateCurnt

.unreq    pX

.unreq    interpolationFact
.unreq    intFact
.unreq    phaseLen
.unreq    Offset

.unreq    Count
.unreq    pTemp

.unreq    mask

.unreq    pMask
.unreq    index

@/*NEON variale Declaration*/
.unreq    qInp
.unreq    dInp_0
.unreq    dInp_1
.unreq    qCoeff0
.unreq    dCoeff0_0
.unreq    dCoeff0_1
.unreq    qZero

.unreq    qMask
.unreq    dMask_0
.unreq    dMask_1
.unreq    dOut_0
.unreq    dOut_1

.unreq    qAcc0
.unreq    dAcc0_0
.unreq    dAcc0_1

.unreq    qTemp
.unreq    dTemp_0
.unreq    dTemp_1

.unreq    qCoeff1
.unreq    dCoeff1_0
.unreq    dCoeff1_1
.unreq    qCoeff2
.unreq    dCoeff2_0
.unreq    dCoeff2_1
.unreq    qCoeff3
.unreq    dCoeff3_0
.unreq    dCoeff3_1

.unreq    qMask1
.unreq    dMask1_0
.unreq    dMask1_1

.unreq    qMaskTemp
.unreq    dMaskTemp_0
.unreq    dMaskTemp_1


        @/**
        @ * @details
        @ * This function operates on floating-point data types.
        @ * There are no restrictions on numStages and blockSize.
        @ *
        @ * The order of the coefficients in *coeffs should be
        @ * k1, k2, ...kM-1
        @ *
        @ * <b>Cycle Count:</b>
        @ *
        @ * when the block size >= 32, the tap is little, you could get
        @ * maximized improvement
        @ *
        @ * <code>c0 + c1 * blockSize + c2 * numStages * blockSize</code>
        @ *
        @ * @param[in]  *S                points to struct parameter
        @ * @param[in]  *pSrc             points to the input buffer
        @ * @param[out]  *pDst            points to the output buffer
        @ * @param[in]  blockSize         block size of filter
        @ */

        .align   4
        .global   ne10_fir_lattice_float_neon
        .extern   ne10_qMaskTable32
        .thumb
        .thumb_func

ne10_fir_lattice_float_neon:

                        PUSH    {r4-r12,lr}    @push r12: to keep stack 8 bytes aligned

@/*ARM Registers*/
pStateStruct     .req   R0
pSrc             .req   R1
pDst             .req   R2
blockSize        .req   R3

pState           .req   R4             @/* State pointer */
pCoeffs          .req   R5             @/* Coefficient pointer */

pX               .req   R7             @/* Temporary pointers for state buffer */
pB               .req   R8             @/* Temporary pointers for coefficient buffer */
numStages        .req   R9             @/* Length of the filter */

stageCnt         .req   R10            @ /* Loop counter */


pTemp            .req   R11
pMask            .req   R14            @  /* Mask Table */
mask             .req   R12

@/*NEON variale Declaration*/
qFcurr           .qn   Q0.F32
dFcurr_0         .dn   D0.F32
dFcurr_1         .dn   D1.F32
qCoeff           .qn   Q1.F32
dCoeff_0         .dn   D2.F32
dCoeff_1         .dn   D3.F32

qZero            .qn   Q2.F32

qMask            .qn   Q3.U32
dMask_0          .dn   D6.U32
dMask_1          .dn   D7.U32
dOut_0           .dn   D6.F32
dOut_1           .dn   D7.F32

qAcc0            .qn   Q8.F32
dAcc0_0          .dn   D16.F32
dAcc0_1          .dn   D17.F32

qTemp            .qn   Q9.F32
dTemp_0          .dn   D18.F32
dTemp_1          .dn   D19.F32

qFnext           .qn   Q10.F32
dFnext_0         .dn   D20.F32
dFnext_1         .dn   D21.F32
qGcurr           .qn   Q11.F32
dGcurr_0         .dn   D22.F32
dGcurr_1         .dn   D23.F32
qGnext           .qn   Q12.F32
dGnext_0         .dn   D24.F32
dGnext_1         .dn   D25.F32

qMask1           .qn   Q13.U32
dMask1_0         .dn   D26.U32
dMask1_1         .dn   D27.U32
qMaskTmp         .qn   Q14.U32
dMaskTmp_0       .dn   D28.U32
dMaskTmp_1       .dn   D29.U32
qTemp1           .qn   Q15.F32
dTemp1_0         .dn   D30.F32
dTemp1_1         .dn   D31.F32

fNext            .dn   D0.F32
gCurr            .dn   D1.F32
gNext            .dn   D2.F32
fCurr            .dn   D3.F32
Coeff            .dn   D4.F32

                            @/* Length of the filter */
                            LDRH        numStages,[pStateStruct],#4
                            @/* State pointer */
                            LDR         pState,[pStateStruct],#4
                            @/* Coefficient pointer */
                            LDR         pCoeffs,[pStateStruct],#4


                            @// Get the Mask Values

                            LDR         pMask,=ne10_qMaskTable32
                            SUB         numStages,#1
                            AND         mask,numStages, #3
                            AND         stageCnt,blockSize,#3

                            ADD         pTemp,pMask,mask,LSL #4
                            ADD         stageCnt,pMask,stageCnt,LSL #4
                            VLD1        {dMaskTmp_0,dMaskTmp_1},[pTemp]
                            VLD1        {dMask1_0,dMask1_1},[stageCnt]
                            VEOR        qZero,qZero,qZero


                            SUBS        blockSize,#4
                            BLT         firLatticeEndOuterLoop
firLatticeOuterLoop:
                            @/* Initialize coeff pointer */
                            MOV         pB,pCoeffs
                            @/* Initialize state pointer */
                            MOV         pX,pState
                            @/* Read Four samples from input buffer: fcurr0, fcurr1,fcurr2,fcurr3*/
                            @/* f0(n) = x(n) */
                            VLD1        {dFcurr_0,dFcurr_1},[pSrc]!
                            @/*Read one Sample from the State Buffer*/
                            VLD1        {dGcurr_1[1]},[pX]
                            VEXT        qGnext,qGcurr,qFcurr,#3

                            VLD1        {dCoeff_0[],dCoeff_1[]},[pB]!
                            VMOV        qFnext,qFcurr
                            VST1        {dFcurr_1[1]},[pX]!
                            @/* fi(n) = fi-1(n) + Ki * gi-1(n-1) */
                            @/* gi(n) = fi-1(n) * Ki + gi-1(n-1) */
                            @/* ki*gcurr4+fcurr4 ki*gcurr3+fcurr3 ki*gcurr2+fcurr2 ki*gcurr1+fcurr1*/

                            VMLA        qFcurr,qGnext,qCoeff
                            @/* ki*fcurr4+gcurr4 ki*fcurr3+gcurr3 ki*fcurr2+gcurr2 ki*fcurr1+gcurr1*/
                            VMLA        qGnext,qFnext,qCoeff


                            @/* Loop unrolling.  Process 4 taps at a time . */
                            SUBS        stageCnt,numStages,#4
                            BLT         firLatticeEndInnerLoop
                            @/* Loop over the number of taps.  Unroll by a factor of 4.
                            @  * Repeat until we've computed numStages-3 coefficients. */
                            @/* Process 2nd, 3rd, 4th and 5th taps ... here */
firLatticeInnerLoop:
                            VLD1        {dGcurr_1[1]},[pX]!
                            VREV64      dTemp_0,dGnext_1

                            VLD1        {dCoeff_0[],dCoeff_1[]},[pB]!
                            VEXT        qGcurr,qGcurr,qGnext,#3

                            @ /* fi(n) = fi-1(n) + Ki * gi-1(n-1) */
                            @/* gi(n) = fi-1(n) * Ki + gi-1(n-1) */
                            @/* ki*gcurr4+fcurr4 ki*gcurr3+fcurr3 ki*gcurr2+fcurr2 ki*gcurr1+fcurr1*/
                            VMOV        qFnext,qFcurr
                            VMOV        qGnext,qGcurr
                            @/* ki*fcurr4+gcurr4 ki*fcurr3+gcurr3 ki*fcurr2+gcurr2 ki*fcurr1+gcurr1*/
                            VMLA        qGnext,qFnext,qCoeff
                            VMLA        qFcurr,qGcurr,qCoeff

                            @/*Prepare for Next Stage*/
                            VLD1        {dGcurr_1[1]},[pX]!

                            VLD1        {dCoeff_0[],dCoeff_1[]},[pB]!
                            VEXT        dTemp_0,dGnext_1,dTemp_0,#1
                            VEXT        qGcurr,qGcurr,qGnext,#3

                            @/*Next Stage*/
                            VMOV        qFnext,qFcurr
                            VMOV        qGnext,qGcurr
                            VMLA        qGnext,qFnext,qCoeff
                            VMLA        qFcurr,qGcurr,qCoeff

                            @/*Prepare for Next Stage*/
                            VLD1        {dGcurr_1[1]},[pX]!
                            VLD1        {dCoeff_0[],dCoeff_1[]},[pB]!
                            VEXT        dTemp_1,dGnext_1,dTemp_1,#1
                            VEXT        qGcurr,qGcurr,qGnext,#3


                            @/*Next Stage*/
                            VMOV        qFnext,qFcurr
                            VMOV        qGnext,qGcurr
                            VMLA        qGnext,qFnext,qCoeff
                            VMLA        qFcurr,qGcurr,qCoeff


                            @/*Prepare for Next Stage*/
                            VLD1        {dGcurr_1[1]},[pX]!
                            VLD1        {dCoeff_0[],dCoeff_1[]},[pB]!
                            VEXT        dTemp_1,dGnext_1,dTemp_1,#1
                            VEXT        qGcurr,qGcurr,qGnext,#3

                            VREV64      qTemp,qTemp
                            @/*Next Stage*/
                            VMOV        qFnext,qFcurr
                            VMOV        qGnext,qGcurr
                            VMLA        qFcurr,qGcurr,qCoeff
                            VMLA        qGnext,qFnext,qCoeff
                            SUB         pX,#16

                            @/*Store the samples in the state buffer for next frame*/
                            VST1        {dTemp_0,dTemp_1},[pX]!
                            SUBS        stageCnt,#4
                            BGE         firLatticeInnerLoop
firLatticeEndInnerLoop:
                            ADDS        stageCnt,#4
                            BEQ         firLatticeFinishInner
                            VMOV        qMask,qMaskTmp
                            VLD1        {dCoeff_0,dCoeff_1},[pB]!

                            VLD1        {dGcurr_1[1]},[pX]!
                            VREV64      dTemp_0,dGnext_1
                            VBSL        qMask,qCoeff,qZero


                            VEXT        qGcurr,qGcurr,qGnext,#3
                            VDUP        qCoeff,dMask_0[0]
                            VMOV        qFnext,qFcurr
                            VMOV        qGnext,qGcurr
                            VMLA        qGnext,qFnext,qCoeff
                            VMLA        qFcurr,qGcurr,qCoeff

                            VLD1        {dGcurr_1[1]},[pX]!

                            VDUP        qCoeff,dMask_0[1]
                            VEXT        dTemp_0,dGnext_1,dTemp_0,#1
                            VEXT        qGcurr,qGcurr,qGnext,#3

                            VMOV        qFnext,qFcurr
                            VMOV        qGnext,qGcurr
                            VMLA        qGnext,qFnext,qCoeff
                            VMLA        qFcurr,qGcurr,qCoeff

                            VLD1        {dGcurr_1[1]},[pX]!
                            VDUP        qCoeff,dMask_1[0]
                            VEXT        dTemp_1,dGnext_1,dTemp_1,#1
                            VEXT        qGcurr,qGcurr,qGnext,#3

                            VMOV        qFnext,qFcurr
                            VMOV        qGnext,qGcurr
                            VMLA        qGnext,qFnext,qCoeff
                            VMLA        qFcurr,qGcurr,qCoeff

                            VLD1        {dGcurr_1[1]},[pX]!
                            VDUP        qCoeff,dMask_1[1]
                            VEXT        dTemp_1,dGnext_1,dTemp_1,#1
                            VEXT        qGcurr,qGcurr,qGnext,#3

                            VREV64      qTemp,qTemp

                            VMOV        qFnext,qFcurr
                            VMOV        qGnext,qGcurr
                            SUB         pX,pX,#16

                            VMOV        qMask,qMaskTmp
                            VMLA        qFcurr,qGcurr,qCoeff
                            VLD1        {dTemp1_0,dTemp1_1},[pX]
                            VMLA        qGnext,qFnext,qCoeff
                            VBSL        qMask,qTemp,qTemp1
                            VST1        {dMask_0,dMask_1},[pX]
                            ADD         pX,pX,stageCnt, LSL #2

firLatticeFinishInner:

                            VST1        {dFcurr_0,dFcurr_1},[pDst]!
                            SUBS        blockSize,#4
                            BGE         firLatticeOuterLoop

firLatticeEndOuterLoop:
                            ADDS        blockSize,#4
                            BEQ         firLatticeEnd

firLatticeOuterLoop1:
                            VLD1        {fCurr[0]},[pSrc]!
                            MOV         pB,pCoeffs
                            MOV         pX,pState
                            VLD1        {gCurr[0]},[pX]
                            VLD1        {Coeff[0]},[pB]!

                            VST1        {fCurr[0]},[pX]!
                            VMOV        gNext,gCurr
                            VMLA        gNext,Coeff,fCurr
                            VMLA        fCurr,Coeff,gCurr

                            SUBS        stageCnt,numStages,#1
                            BLE         firLatticeEndinnerLoop1
firLatticeInnerLoop1:
                            VLD1        {gCurr[0]},[pX]
                            VST1        {gNext[0]},[pX]!

                            VLD1        {Coeff[0]},[pB]!

                            VMOV        gNext,gCurr
                            VMLA        gNext,Coeff,fCurr
                            VMLA        fCurr,Coeff,gCurr
                            SUBS        stageCnt,#1
                            BGE         firLatticeInnerLoop1
firLatticeEndinnerLoop1:
                            VST1        {fCurr[0]},[pDst]!
                            SUBS        blockSize,#1
                            BGT         firLatticeOuterLoop1

firLatticeEnd:
                            @/*Return From Function*/
                            POP     {r4-r12,pc}

@/*ARM Registers*/
.unreq    pStateStruct
.unreq    pSrc
.unreq    pDst
.unreq    blockSize

.unreq    pState
.unreq    pCoeffs

.unreq    pX
.unreq    pB
.unreq    numStages

.unreq    stageCnt

.unreq    pTemp
.unreq    pMask
.unreq    mask

.unreq    fNext
.unreq    gCurr
.unreq    gNext
.unreq    fCurr
.unreq    Coeff

@/*NEON variale Declaration*/
.unreq    qFcurr
.unreq    dFcurr_0
.unreq    dFcurr_1
.unreq    qCoeff
.unreq    dCoeff_0
.unreq    dCoeff_1

.unreq    qZero

.unreq    qMask
.unreq    dMask_0
.unreq    dMask_1
.unreq    dOut_0
.unreq    dOut_1

.unreq    qAcc0
.unreq    dAcc0_0
.unreq    dAcc0_1

.unreq    qTemp
.unreq    dTemp_0
.unreq    dTemp_1

.unreq    qFnext
.unreq    dFnext_0
.unreq    dFnext_1
.unreq    qGcurr
.unreq    dGcurr_0
.unreq    dGcurr_1
.unreq    qGnext
.unreq    dGnext_0
.unreq    dGnext_1

.unreq    qMask1
.unreq    dMask1_0
.unreq    dMask1_1
.unreq    qMaskTmp
.unreq    dMaskTmp_0
.unreq    dMaskTmp_1
.unreq    qTemp1
.unreq    dTemp1_0
.unreq    dTemp1_1

        @/**
        @ * @details
        @ * This function operates on floating-point data types.
        @ * There are no restrictions on numTaps and blockSize.
        @ *
        @ * The scratch buffer, pScratch is internally used for holding the state values temporarily.
        @ * <b>Cycle Count:</b>
        @ *
        @ * <code> C0 * blockSize + C1 * numTaps + C2 * numTaps * blockSize</code>
        @ *
        @ * when the block size >= 32, you could get
        @ * maximized improvement
        @ *
        @ * @param[in]  *S                points to struct parameter
        @ * @param[in]  *pSrc             points to the input buffer
        @ * @param[out]  *pDst            points to the output buffer
        @ * @param[out]  *pScratch        points to the scratch buffer
        @ * @param[in]  blockSize         block size of filter
        @ */

        .align   4
        .global   ne10_fir_sparse_float_neon
        .extern   ne10_qMaskTable32
        .thumb
        .thumb_func

ne10_fir_sparse_float_neon:
                            @ save the point of struct(r0) to stack for stateIndex update
                            PUSH    {r0,r4-r11,lr}

@/*ARM Registers*/
pStateStruct     .req   R0
pSrc             .req   R1
pDst             .req   R2
pScratch         .req   R3
blockSize        .req   R4
size2            .req   R4

pYtmp1           .req   R0
pOut             .req   R0
Offset           .req   R0

readIndex        .req   R1

numTaps          .req   R5             @/* Length of the filter */

pState           .req   R6             @/* State pointer */
pCoeffs          .req   R7             @/* Coefficient pointer */
stateIndex       .req   R8

maxDelay         .req   R9
delaySize        .req   R9

pTapDelay        .req   R10

blkCnt           .req   R11
size1            .req   R11
temp             .req   R1
mask             .req   R11
pMask            .req   R11

pX               .req   R12

pY               .req   R14
pYtmp2           .req   R14


@/*NEON variale Declaration*/
qInp             .qn   Q0.F32
dInp_0           .dn   D0.F32
dInp_1           .dn   D1.F32

qCoeff           .qn   Q1.F32
dCoeff_0         .dn   D2.F32
dCoeff_1         .dn   D3.F32

qZero            .qn   Q2.F32

qMask            .qn   Q3.U32
qMaskF32         .qn   Q3.F32
dMask_0          .dn   D6.U32
dMask_1          .dn   D7.U32


qAcc0            .qn   Q8.F32
dAcc0_0          .dn   D16.F32
dAcc0_1          .dn   D17.F32


qTemp            .qn   Q9.F32
dTemp_0          .dn   D18.F32
dTemp_1          .dn   D19.F32


qMaskTmp         .qn   Q10.U32
dMaskTmp_0       .dn   D20.U32
dMaskTmp_1       .dn   D21.U32


                    /*Load Mask Table*/

                    LDRH        numTaps,[pStateStruct],#2
                    LDRH        stateIndex,[pStateStruct],#2
                    LDR         pState,[pStateStruct],#4
                    LDR         pCoeffs,[pStateStruct],#4
                    LDRH        maxDelay,[pStateStruct],#4
                    LDR         pTapDelay,[pStateStruct],#4

                    @// Load blockSize from Stack
                    LDR         blockSize,[SP,#40]
                    LDR         pMask,=ne10_qMaskTable32
                    ADD         delaySize,blockSize,maxDelay

                    VEOR        qZero,qZero
                    AND         pY,blockSize,#3
                    ADD         pY,pMask,pY,LSL #4
                    VLD1        {dMaskTmp_0,dMaskTmp_1},[pY]


                    @/* BlockSize of Input samples are copied into the state buffer */
                    @/* StateIndex points to the starting position to write in the state buffer */
                    MOV         pX,pState
                    LSL         Offset,stateIndex,#2

                    SUBS        blkCnt,blockSize,#1
                    BLT         firSparseEndSrcCopy
firSparseSrcCopyLoop:

                    LDR         pY,[pSrc],#4
                    STR         pY,[pX,Offset]
                    ADD         Offset,#4
                    CMP         delaySize,Offset,LSR #2
                    IT          LE
                    SUBLE       Offset,Offset,delaySize, LSL #2
                    SUBS        blkCnt,#1
                    BGE         firSparseSrcCopyLoop
firSparseEndSrcCopy:

                    LSR         stateIndex,Offset,#2
                    LDR         Offset,[SP,#0]
                    STRH        stateIndex,[Offset,#2]

                    LDR         readIndex,[pTapDelay],#4
                    ADD         readIndex,readIndex,blockSize
                    SUBS        readIndex,stateIndex,readIndex

                    @/*Wrap arround index*/
                    IT          LT
                    ADDLT       readIndex,readIndex,delaySize


                    @/*Processing begins*/
                    @/*First stage*/
                    MOV         pY,pState
                    MOV         pX,pScratch

                    @/* copy the sample from the circular buffer to the destination buffer */
                    SUB         size1,delaySize,readIndex
                    CMP         size1,blockSize
                    IT          GT
                    MOVGT       size1,blockSize

                    ADD         pYtmp1,pY,readIndex, LSL #2
                    SUB         size2,blockSize,size1
                    MOV         pYtmp2,pY

                    CMP         size1,#0
                    BLE         firSparseEndcopy1
firSparseCopy1:
                    LDR         temp,[pYtmp1],#4
                    SUBS        size1,#1
                    STR         temp,[pScratch],#4
                    BGT         firSparseCopy1
firSparseEndcopy1:

                    CMP         size2,#0
                    BLE         firSparseEndcopy2
firSparseCopy2:
                    LDR         temp,[pYtmp2],#4
                    SUBS        size2,#1
                    STR         temp,[pScratch],#4
                    BGT         firSparseCopy2
firSparseEndcopy2:


                    @// Load blockSize from Stack
                    LDR         blockSize,[SP,#40]


                    MOV         pOut,pDst
                    VLD1         {dCoeff_0[],dCoeff_1[]},[pCoeffs]!
                    @//CMP         tapCnt,numTaps

                    @//Complete the case of tapCnt=numTaps
                    SUBS        blkCnt,blockSize,#4
                    VLD1        {dInp_0,dInp_1},[pX]!
                    BLT         firSparseEndInnerLoop
firSparseInnerLoop:
                    VMUL        qAcc0,qInp,qCoeff
                    VLD1        {dInp_0,dInp_1},[pX]!
                    SUBS        blkCnt,#4
                    VST1        {dAcc0_0,dAcc0_1},[pOut]!
                    BGE         firSparseInnerLoop

firSparseEndInnerLoop:
                    ADDS        blkCnt,#4
                    @/* If the blockSize is not a multiple of 4,
                    @* * compute the remaining samples */

                    VLD1        {dTemp_0,dTemp_1},[pOut]
                    VMUL        qAcc0,qInp,qCoeff
                    VMOV        qMask,qMaskTmp
                    VBSL        qMask,qAcc0,qTemp
                    VST1        {dMask_0,dMask_1},[pOut]
                    ADD         pOut,pOut,blkCnt,LSL #2

                    LDR         readIndex,[pTapDelay],#4
                    ADD         readIndex,readIndex,blockSize
                    SUBS        readIndex,stateIndex,readIndex

                    @/*Wrap arround index*/
                    IT          LT
                    ADDLT       readIndex,readIndex,delaySize

                    SUBS        numTaps,#1
                    BLE         firSparseEnd
firSparseOuterLoop:

                    @// Load blockSize from Stack
                    LDR         blockSize,[SP,#40]

                    MOV         pY,pState
                    MOV         pX,pScratch

                    @/* copy the sample from the circular buffer to the destination buffer */
                    SUB         size1,delaySize,readIndex
                    CMP         size1,blockSize
                    IT          GT
                    MOVGT       size1,blockSize

                    ADD         pYtmp1,pY,readIndex, LSL #2
                    SUB         size2,blockSize,size1
                    MOV         pYtmp2,pY


                    CMP         size1,#0
                    BLE         firSparseEndcopy3
firSparseCopy3:
                    LDR         temp,[pYtmp1],#4
                    SUBS        size1,#1
                    STR         temp,[pScratch],#4
                    BGT         firSparseCopy3
firSparseEndcopy3:
                    CMP         size2,#0
                    BLE         firSparseEndcopy4
firSparseCopy4:
                    LDR         temp,[pYtmp2],#4
                    SUBS        size2,#1
                    STR         temp,[pScratch],#4
                    BGT         firSparseCopy4
firSparseEndcopy4:

                    @// Load blockSize from Stack
                    LDR         blockSize,[SP,#40]


                    MOV         pOut,pDst
                    VLD1         {dCoeff_0[],dCoeff_1[]},[pCoeffs]!


                    @//Complete the case of tapCnt=numTaps
                    SUBS        blkCnt,blockSize,#4
                    VLD1        {dInp_0,dInp_1},[pX]!
                    VLD1        {dAcc0_0,dAcc0_1},[pOut]
                    BLT         firSparseEndInnerLoop1
firSparseInnerLoop1:
                    VMLA        qAcc0,qInp,qCoeff
                    VLD1        {dInp_0,dInp_1},[pX]!
                    SUBS        blkCnt,#4
                    VST1        {dAcc0_0,dAcc0_1},[pOut]!
                    VLD1        {dAcc0_0,dAcc0_1},[pOut]
                    BGE         firSparseInnerLoop1

firSparseEndInnerLoop1:
                    ADDS        blkCnt,#4
                    @/* If the blockSize is not a multiple of 4,
                    @* * compute the remaining samples */


                    VMOV        qMask,qMaskTmp
                    VBSL        qMask,qInp,qZero
                    VMLA        qAcc0,qMaskF32,qCoeff

                    VST1        {dAcc0_0,dAcc0_1},[pOut]
                    ADD         pOut,pOut,blkCnt,LSL #2

                    LDR         readIndex,[pTapDelay],#4
                    ADD         readIndex,readIndex,blockSize
                    SUBS        readIndex,stateIndex,readIndex

                    @/*Wrap arround index*/
                    IT          LT
                    ADDLT       readIndex,readIndex,delaySize

                    SUBS        numTaps,#1

                    BGT         firSparseOuterLoop
firSparseEnd:
                    @// Return From Function
                    POP     {r0,r4-r11,pc}

@/*ARM Registers*/
.unreq    pStateStruct
.unreq    pSrc
.unreq    pDst
.unreq    pScratch
.unreq    blockSize
.unreq    size2

.unreq    pYtmp1
.unreq    pOut
.unreq    Offset

.unreq    readIndex

.unreq    numTaps

.unreq    pState
.unreq    pCoeffs
.unreq    stateIndex

.unreq    maxDelay
.unreq    delaySize

.unreq    pTapDelay

.unreq    blkCnt
.unreq    size1
.unreq    temp
.unreq    mask
.unreq    pMask

.unreq    pX

.unreq    pY
.unreq    pYtmp2

@/*NEON variale Declaration*/
.unreq    qInp
.unreq    dInp_0
.unreq    dInp_1

.unreq    qCoeff
.unreq    dCoeff_0
.unreq    dCoeff_1

.unreq    qZero

.unreq    qMask
.unreq    qMaskF32
.unreq    dMask_0
.unreq    dMask_1

.unreq    qAcc0
.unreq    dAcc0_0
.unreq    dAcc0_1

.unreq    qTemp
.unreq    dTemp_0
.unreq    dTemp_1

.unreq    qMaskTmp
.unreq    dMaskTmp_0
.unreq    dMaskTmp_1

        .end
