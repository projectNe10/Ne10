/*
 *  Copyright 2013 ARM Limited
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
 * NE10 Library : imgproc/NE10_rotate.neon.s
 */

        .text
        .syntax   unified

        /**
         * @details
         * This function implements the image rotation (angle < 90)
         *
         * @param[out]  *dst             points to the output pointers
         * @param[in]   *src             points to input pointers
         * @param[in]   swidth           width of input buffer
         * @param[in]   sheight          height of input buffer
         * @param[in]   dwidth           width of output buffer
         * @param[in]   dheight          height of output buffer
         * @param[in]   *matrix          matrix of rotate
         */

        .align   4
        .global   ne10_img_rotate_get_quad_rangle_subpix_neon
        .thumb
        .thumb_func

ne10_img_rotate_get_quad_rangle_subpix_neon:

/*ARM Registers*/
/* long-term variable */
pDst             .req   r0
pSrc             .req   r1
dstHeight        .req   r4
srcStep          .req   r5
dstStep          .req   r6
countX           .req   r7
countY           .req   r8

/* short-term variable */
/* out of loopY */
srcWidth         .req   r2
srcHeight        .req   r3
dstWidth         .req   r9
pMatrix          .req   r10

/* in loopY */
ixs              .req   r2
iys              .req   r3
pTr0             .req   r9
pTr1             .req   r10

/* temp variable */
tmp0             .req   r11
tmp1             .req   r12

/*NEON Registers*/
/* long-term variable */
dA1              .dn    d0
dA2              .dn    d1
dA3              .dn    d2
dY               .dn    d3
dDW              .dn    d4
dSrcSizeSub1     .dn    d5
dSrcSizeSub4     .dn    d6
dValOne          .dn    d7
dValOneF         .dn    d23

/* short-term variable */
/* out of loopY */
dValFour         .dn    d16
dSrcSize         .dn    d17

/* in loopY */
/* temp variable */
qBitSE           .qn    q8
dBitS            .dn    d16
dBitE            .dn    d17
dBitSE           .dn    d18
dBitIS           .dn    d19
dOutFlag         .dn    d19

dPosS            .dn    d20
dPosE            .dn    d21
dPosIS           .dn    d22
dPosAB           .dn    d8
dPosA1           .dn    d9

dIn0_01234567    .dn    d26
qIn0_01234567    .qn    q13
dIn0_0123        .dn    d26
dIn0_4567        .dn    d27

dIn1_01234567    .dn    d28
qIn1_01234567    .qn    q14
dIn1_0123        .dn    d28
dIn1_4567        .dn    d29

qIn0_0123        .qn    q15
qIn0_4567        .qn    q5
qIn1_0123        .qn    q6
qIn1_4567        .qn    q7
qP0              .qn    q13
qP1              .qn    q14
qOut_0123        .qn    q5
dOut_0123        .dn    d10
dOut_hw          .dn    d11

dPosISF          .dn    d30
qTmp0            .qn    q15
dTmp0            .dn    d30
dTmp1            .dn    d31



                     push        {r4-r12, lr}
                     vpush       {d8-d15}

                     /* load parameters from sp*/
                     ldr            dstWidth, [sp, #104]
                     ldr            dstHeight, [sp, #108]
                     ldr            pMatrix, [sp, #112]

                     lsl            srcStep, srcWidth, #2
                     lsl            dstStep, dstWidth, #2
                     vld3.f32       {dA1, dA2, dA3}, [pMatrix]

                     /* set number of loop y  */
                     mov            countY, dstHeight

                     /* set NEON register for 1 and 4*/
                     mov            tmp0, #1
                     vdup.32        dValOne, tmp0
                     vcvt.f32.u32   dValOneF, dValOne
                     vshl.s32       dValFour, dValOne, #2

                     vmov            dSrcSize, srcWidth, srcHeight
                     vsub.i32        dSrcSizeSub4, dSrcSize, dValFour
                     vsub.i32        dSrcSizeSub1, dSrcSize, dValOne

                     /* loop y start */
                     @cbz            countY, GetQuadrangleSubPixEnd

                     sub            tmp1, dstWidth, #1
                     vdup.32        dDW, tmp1
                     vcvt.f32.u32   dDW, dDW
                     vmul.f32       dDW, dA1, dDW
GetQuadrangleSubPixLoopY:

                     sub            tmp0, dstHeight, countY
                     vdup.32        dY, tmp0
                     vcvt.f32.u32   dY, dY

                     /* calculate xs, ys, xe, ye */
                     vmov.f32       dPosS, dA3
                     vmla.f32       dPosS, dA2, dY
                     vadd.f32       dPosE, dDW, dPosS
                     vcvt.s32.f32   dPosIS, dPosS
                     vcvt.s32.f32   dPosE, dPosE
                     vsub.s32       dBitS, dPosIS, dValOne
                     vsub.s32       dPosE, dPosE, dValOne


                     vcgt.u32       dBitS, dSrcSizeSub4, dBitS
                     vcgt.u32       dBitE, dSrcSizeSub4, dPosE
                     vrev64.32      dTmp0, dBitS
                     vrev64.32      dTmp1, dBitE
                     vand.32        dBitS, dBitS, dTmp0
                     vand.32        dBitE, dBitE, dTmp1
                     vand.32        dBitSE, dBitE, dBitS

                     /* set number of loop x  */
                     lsr            countX, dstStep, #2

GetQuadrangleSubPixLoopX:

                     vcvt.s32.f32   dPosIS, dPosS
                     vcvt.f32.s32   dPosISF, dPosIS
                     vsub.f32       dPosAB, dPosS, dPosISF
                     vsub.f32       dPosA1, dValOneF, dPosAB

                     vcgt.u32       dBitIS, dSrcSizeSub1, dPosIS
                     vrev64.32      dTmp0, dBitIS
                     vand.32        dBitIS, dBitIS, dTmp0

                     vadd.f32       dPosS, dPosS, dA1
                     vorr           dOutFlag, dBitIS, dBitSE

                     /* to avoid that ixs/iys is negative. this will result in bad address of pTr0/pTr1 */
                     vabs.s32       dPosIS, dPosIS
                     vmov           ixs, iys, dPosIS
                     lsl            ixs, ixs, #2
                     mla            tmp0, srcStep, iys, ixs
                     add            pTr0, pSrc, tmp0
                     add            pTr1, pTr0, srcStep

                     vld1.8         {dIn0_01234567}, [pTr0]
                     vld1.8         {dIn1_01234567}, [pTr1]
                     vmovl.u8       qIn0_01234567, dIn0_01234567
                     vmovl.u8       qIn1_01234567, dIn1_01234567
                     vmovl.u16      qIn0_0123, dIn0_0123
                     vmovl.u16      qIn1_0123, dIn1_0123
                     vmovl.u16      qIn0_4567, dIn0_4567
                     vmovl.u16      qIn1_4567, dIn1_4567
                     vcvt.f32.u32   qIn0_0123, qIn0_0123
                     vcvt.f32.u32   qIn1_0123, qIn1_0123
                     vcvt.f32.u32   qIn0_4567, qIn0_4567
                     vcvt.f32.u32   qIn1_4567, qIn1_4567
                     vmul.f32       qP0, qIn0_0123, dPosA1[0]
                     vmul.f32       qP1, qIn1_0123, dPosA1[0]
                     vmla.f32       qP0, qIn0_4567, dPosAB[0]
                     vmla.f32       qP1, qIn1_4567, dPosAB[0]
                     vsub.f32       qTmp0, qP1, qP0
                     vmla.f32       qP0, qTmp0, dPosAB[1]
                     vcvt.u32.f32   qOut_0123, qP0
                     vmovn.u32      dOut_0123, qOut_0123
                     vand.u32       dOut_0123, dOut_0123, dOutFlag
                     vmovn.u16      dOut_0123, qOut_0123
                     vst1.32        {dOut_0123[0]}, [pDst]!


                     subs           countX, countX, #1
                     bgt            GetQuadrangleSubPixLoopX


                     subs           countY, countY, #1
                     bgt            GetQuadrangleSubPixLoopY


GetQuadrangleSubPixEnd:
                    /*Return From Function*/
                     vpop           {d8-d15}
                     pop            {r4-r12, pc}

       .end
