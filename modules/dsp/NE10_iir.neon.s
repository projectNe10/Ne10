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

@
@ Currently, this is for soft VFP EABI, not for hard vfpv3 ABI yet
@

@/*
@ * NE10 Library : dsp/NE10_iir.neon.s
@ */


        .text
        .syntax   unified

        @/**
        @ * @brief Processing function for the floating-point IIR lattice filter.
        @ * @param[in] *S points to an instance of the floating-point IIR lattice structure.
        @ * @param[in] *pSrc points to the block of input data.
        @ * @param[out] *pDst points to the block of output data.
        @ * @param[in] blockSize number of samples to process.
        @ * @return none.
        @ */

        .align   4
        .global   ne10_iir_lattice_float_neon
        .extern   ne10_qMaskTable32
        .thumb
        .thumb_func

ne10_iir_lattice_float_neon:
                        PUSH    {r4-r12,lr}

@/*ARM Registers*/
pStateStruct     .req   R0
pSrc             .req   R1
pDst             .req   R2
blockSize        .req   R3

pState           .req   R4             @/* State pointer */
pKcoeffs         .req   R5             @/* Coefficient pointer */
pVcoeffs         .req   R6             @/* Coefficient pointer */

pX               .req   R7             @/* Temporary pointers for state buffer */
pK               .req   R8             @/* Temporary pointers for coefficient buffer */
numStages        .req   R9             @/* Length of the filter */

tapCnt           .req   R10            @ /* Loop counter */
pTemp            .req   R11


pMask            .req   R14            @  /* Mask Table */

mask             .req   R12
pV               .req   R12

@/*NEON variale Declaration*/
dTemp3a_0        .dn    D0.U32
dTemp3_0         .dn    D0.F32
dMask2           .dn    D1.U32

qGcurr           .qn   Q1.F32
dGcurr_0         .dn   D2.F32
dGcurr_1         .dn   D3.F32

qZero            .qn   Q2.F32

qMask            .qn   Q3.U32
dMask_0          .dn   D6.U32
dMask_1          .dn   D7.U32
dOut_0           .dn   D6.F32
dOut_1           .dn   D7.F32

qGK              .qn   Q4.F32
dGK_0            .dn   D8.F32
dGK_1            .dn   D9.F32

qAcc0            .qn   Q8.F32
dAcc0_0          .dn   D16.F32
dAcc0_1          .dn   D17.F32

qTemp            .qn   Q9.F32
dTemp_0          .dn   D18.F32
dTemp_1          .dn   D19.F32

qFnext           .qn   Q10.F32
dFnext_0         .dn   D20.F32
dFnext_1         .dn   D21.F32

qFcurr           .qn   Q11.F32
dFcurr_0         .dn   D22.F32
dFcurr_1         .dn   D23.F32

qCoeff0          .qn   Q12.F32
dCoeff0_0        .dn   D24.F32
dCoeff0_1        .dn   D25.F32

qMask1           .qn   Q13.U32
dMask1_0         .dn   D26.U32
dMask1_1         .dn   D27.U32


qMaskTmp         .qn   Q14.U32
dMaskTmp_0       .dn   D28.U32
dMaskTmp_1       .dn   D29.U32

qGnext           .qn   Q15.F32
dGnext_0         .dn   D30.F32
dGnext_1         .dn   D31.F32


                            @/* Length of the filter */
                            LDRH         numStages,[pStateStruct],#4
                            @/* State pointer */
                            LDR         pState,[pStateStruct],#4
                            @/* Coefficient pointer */
                            LDR         pKcoeffs,[pStateStruct],#4
                            LDR         pVcoeffs,[pStateStruct],#4


                            @/*Load Mask Valies*/
                            LDR         pMask,=ne10_qMaskTable32
                            AND         mask,numStages,#3
                            ADD         tapCnt,mask,#1

                            ADD         pTemp,pMask,mask,LSL #4
                            ADD         tapCnt,pMask,tapCnt,LSL #4

                            VLD1        {dMaskTmp_0,dMaskTmp_1},[pTemp]
                            VLD1        {dMask1_0,dMask1_1},[tapCnt]

                            ADD         pTemp,pMask,#16
                            VEOR        qZero,qZero
                            VLD1        {dMask2},[pTemp]

                            @/*while blockSize > 0*/
                            CMP         blockSize, #0
                            BEQ         firLatticeCopy


firLatticeOuterLoop:
                            VLD1        {dFcurr_0[],dFcurr_1[]},[pSrc]!
                            MOV         pX,pState
                            VEOR        qAcc0,qAcc0
                            @/* Initialize Ladder coeff pointer */
                            ADD         pV,pVcoeffs,numStages, LSL #2
                            MOV         pK,pKcoeffs

                            VLD1        {dGcurr_0,dGcurr_1},[pX]
                            @/* Load the filter Taps */
                            VLD1        {dCoeff0_0,dCoeff0_1},[pK]!

                            SUBS        tapCnt,numStages,#4
                            ADD         pV,pV,#4
                            BLT         firLatticeEndInnerLoop


firLatticeInnerLoop:



                            VMUL        qGK,qGcurr,qCoeff0

                            @/* g4k4+g5k5 g6k6+g7k7*/
                            VPADD       dTemp_0,dGK_1,dGK_0
                            @/*g6k6 g4k4+g5k5*/
                            VEXT        dTemp_1,dTemp_0,dGK_1,#1
                            @/*g7k7+g6k6+g5k5+g4k4 g6k6+g5k5+g4k4*/
                            VPADD       dTemp_1,dTemp_1,dTemp_0
                            VMOV        dTemp3a_0,dMask2
                            VBSL        dTemp3a_0,dGK_0,dTemp_0
                            VMOV        dTemp_0,dTemp3_0
                            VSUB        qFnext,qFcurr,qTemp

                            @/* gN(n) = kN * fN-1(n) + gN-1(n-1) */
                            VMLA        qGcurr,qFnext,qCoeff0

                            @/* y(n) += gN(n) * vN  */
                            SUB         pV,pV,#16
                            VLD1        {dCoeff0_0,dCoeff0_1},[pV]

                            @/* write gN-1(n-1) into state for next sample processing */
                            VST1        {dGcurr_0,dGcurr_1},[pX]!
                            VREV64      qCoeff0,qCoeff0
                            @/* acc0 += gnext * (*pv--)@  */
                            VMLA        dAcc0_0,dGcurr_0,dCoeff0_1
                            VMLA        dAcc0_1,dGcurr_1,dCoeff0_0

                            @/* Update f values for next coefficients processing */

                            VDUP        qFcurr,dFnext_1[1]

                            VLD1        {dGcurr_0,dGcurr_1},[pX]
                            @/* Load the filter Taps */
                            VLD1        {dCoeff0_0,dCoeff0_1},[pK]!

                            SUBS        tapCnt,#4
                            BGE         firLatticeInnerLoop
firLatticeEndInnerLoop:
                            @/* If the filter length is not a multiple of 4, compute the remaining filter taps */
                            ADDS        tapCnt,#4
                            IT          GT
                            SUBGT       tapCnt,#1


                            VMUL        qGK,qGcurr,qCoeff0

                            VPADD       dTemp_0,dGK_1,dGK_0
                            VEXT        dTemp_1,dTemp_0,dGK_1,#1
                            VPADD       dTemp_1,dTemp_1,dTemp_0
                            VMOV        dTemp3a_0,dMask2
                            VBSL        dTemp3a_0,dGK_0,dTemp_0
                            VMOV        dTemp_0,dTemp3_0

                            @/*Mask the Uncessary f values*/
                            VMOV        qFnext,qMaskTmp
                            VBSL        qFnext,qTemp,qZero
                            VSUB        qFnext,qFcurr,qFnext

                            VMOV        qGnext,qGcurr
                            VMLA        qGnext,qFnext,qCoeff0

                            @/*Store on to stack for getting proper Fnext*/
                            SUB         pTemp,SP,#20
                            VST1        {dFnext_0,dFnext_1},[pTemp]

                            ADD         pTemp,pTemp,tapCnt, LSL #2
                            VLD1        {dTemp_0[],dTemp_1[]},[pTemp]

                            VMOV        qGcurr,qMaskTmp
                            VBSL        qGcurr,qGnext,qTemp

                            VLD1        {dTemp_0,dTemp_1},[pX]
                            VMOV        qMask,qMask1
                            VBSL        qMask,qGcurr,qTemp
                            VST1        {dMask_0,dMask_1},[pX]

                            ADD         pX,pX,tapCnt,LSL #2

                            SUB         pV,pV,#16
                            VLD1        {dCoeff0_0,dCoeff0_1},[pV]

                            @// MASk the Gnext value used for Output calculation
                            VMOV        qGnext,qMask1
                            VBSL        qGnext,qGcurr,qZero
                            ADD         pX,pX,#4

                            VREV64      qCoeff0,qCoeff0

                            VMLA        dAcc0_0,dGnext_0,dCoeff0_1
                            VMLA        dAcc0_1,dGnext_1,dCoeff0_0

                            /*Get Accumulated Result in to single Value*/

                            VLD1        {dTemp_1},[pDst]
                            VPADD       dTemp_0,dAcc0_0,dAcc0_1
                            VPADD       dTemp_0,dTemp_0

                            VMOV        dMask_0,dMask2
                            VBSL        dMask_0,dTemp_0,dTemp_1

                            VST1        {dMask_0},[pDst]
                            ADD         pDst,#4
                            ADD         pState,#4

                            SUBS        blockSize,#1

                            BGT         firLatticeOuterLoop


                            @/* copy last S->numStages samples to start of the buffer
                            @for next frame process */

firLatticeCopy:
                            AND         mask,numStages,#3
                            ADD         pTemp,pMask,mask,LSL #4
                            LDR         pX,[pStateStruct,#-12]

                            VLD1        {dFcurr_0,dFcurr_1},[pState]!
                            VLD1        {dMask_0,dMask_1},[pTemp]
                            SUBS        tapCnt,numStages,#4
                            BLT         firLatticeEnd
firLatticeCopyLoop:
                            VST1        {dFcurr_0,dFcurr_1},[pX]!
                            SUBS        tapCnt,#4
                            VLD1        {dFcurr_0,dFcurr_1},[pState]!
                            BGE         firLatticeCopyLoop
firLatticeEnd:
                            VLD1        {dTemp_0,dTemp_1},[pX]
                            VBSL        qMask,qFcurr,qTemp
                            VST1        {dOut_0,dOut_1},[pX]
                            ADD         pX,pX,mask, LSL #2
@/*ARM Registers*/
.unreq    pStateStruct
.unreq    pSrc
.unreq    pDst
.unreq    blockSize

.unreq    pState
.unreq    pKcoeffs
.unreq    pVcoeffs

.unreq    pX
.unreq    pK
.unreq    numStages

.unreq    tapCnt
.unreq    pTemp
.unreq    pMask
.unreq    mask
.unreq    pV

@/*NEON variale Declaration*/
.unreq    dTemp3a_0
.unreq    dTemp3_0
.unreq    dMask2

.unreq    qGcurr
.unreq    dGcurr_0
.unreq    dGcurr_1

.unreq    qZero
.unreq    qMask
.unreq    dMask_0
.unreq    dMask_1
.unreq    dOut_0
.unreq    dOut_1

.unreq    qGK
.unreq    dGK_0
.unreq    dGK_1

.unreq    qAcc0
.unreq    dAcc0_0
.unreq    dAcc0_1

.unreq    qTemp
.unreq    dTemp_0
.unreq    dTemp_1

.unreq    qFnext
.unreq    dFnext_0
.unreq    dFnext_1

.unreq    qFcurr
.unreq    dFcurr_0
.unreq    dFcurr_1

.unreq    qCoeff0
.unreq    dCoeff0_0
.unreq    dCoeff0_1

.unreq    qMask1
.unreq    dMask1_0
.unreq    dMask1_1

.unreq    qMaskTmp
.unreq    dMaskTmp_0
.unreq    dMaskTmp_1

.unreq    qGnext
.unreq    dGnext_0
.unreq    dGnext_1

                            POP         {r4-r12,pc}

        .end
