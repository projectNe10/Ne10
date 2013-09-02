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
 * NE10 Library : imgproc/NE10_resize.neon.s
 */

        .text
        .syntax   unified


        /**
         * @details
         * This function implements the vertical interpolation
         *
         * @param[in]   **src            points to input pointers
         * @param[out]  *dst             points to the output buffer
         * @param[in]   *beta            points to interpolate parameter
         * @param[in]   width            width of output buffer
         */

        .align   4
        .global   ne10_img_vresize_linear_neon
        .thumb
        .extern ne10_img_vresize_linear_mask_residual_table/* mask of store data */
        .thumb_func
        .equ         BITS,        0x16        /* INTER_RESIZE_COEF_BITS*2 */
        .equ         DELTA,       0x200000    /* 1 << (INTER_RESIZE_COEF_BITS*2 - 1) */

ne10_img_vresize_linear_neon:
                     push    {r4-r6,lr}

/*ARM Registers*/
pSrc             .req   r0
pDst             .req   r1
pBeta            .req   r2
width            .req   r3

pS0              .req   r4
pS1              .req   r5
tmp              .req   r6

beta0            .req   r2
beta1            .req   r6

pMask            .req   r6

/*NEON variale Declaration*/
dBeta0           .dn   d0[0]
dBeta1           .dn   d0[1]
dMask            .dn   d1
qDelta           .qn   q1
qMin             .qn   q12
qMax             .qn   q13

qS0_0123         .qn   q2
qS0_4567         .qn   q3
qS1_0123         .qn   q8
qS1_4567         .qn   q9

qTmp_0123        .qn   q10
qTmp_4567        .qn   q11
dTmp_0123        .dn   d20
dTmp_4567        .dn   d21
qTmp_01234567    .qn   q10
dTmp_01234567    .dn   d20
dDst_01234567    .dn   d21

                     ldr         beta0, [pBeta]
                     ldr         pS0, [pSrc], #4
                     ldr         pS1, [pSrc]
                     lsr         beta1, beta0, #16
                     lsl         beta0, beta0, #16
                     lsr         beta0, beta0, #16
                     vmov.s32    dBeta0, beta0
                     vmov.s32    dBeta1, beta1

                     mov         tmp, DELTA
                     vdup.32     qDelta, tmp
                     veor        qMin, qMin, qMin
                     mov         tmp, #255
                     vdup.32     qMax, tmp

                     vld1.s32    {qS0_0123, qS0_4567}, [pS0]!
                     vld1.s32    {qS1_0123, qS1_4567}, [pS1]!

                     subs        width, width, #8
                     blt         VResizeResidualLoop

VResizeMainLoop:
                     vmul.s32    qTmp_0123, qS0_0123, dBeta0
                     vmul.s32    qTmp_4567, qS0_4567, dBeta0
                     vmla.s32    qTmp_0123, qS1_0123, dBeta1
                     vmla.s32    qTmp_4567, qS1_4567, dBeta1

                     vadd.s32    qTmp_0123, qTmp_0123, qDelta
                     vadd.s32    qTmp_4567, qTmp_4567, qDelta

                     vshr.s32    qTmp_0123, qTmp_0123, #BITS
                     vshr.s32    qTmp_4567, qTmp_4567, #BITS

                     vmax.s32    qTmp_0123, qTmp_0123, qMin
                     vmax.s32    qTmp_4567, qTmp_4567, qMin
                     vmin.s32    qTmp_0123, qTmp_0123, qMax
                     vmin.s32    qTmp_4567, qTmp_4567, qMax

                     vmovn.I32    dTmp_0123, qTmp_0123
                     vmovn.I32    dTmp_4567, qTmp_4567
                     vmovn.I16    dTmp_01234567, qTmp_01234567
                     vst1.8       {dTmp_01234567}, [pDst]!


                     subs        width, width, #8
                     vld1.s32    {qS0_0123, qS0_4567}, [pS0]!
                     vld1.s32    {qS1_0123, qS1_4567}, [pS1]!
                     bge         VResizeMainLoop

VResizeResidualLoop:
                     adds        width, width, #8
                     beq         VResizeEnd

                     sub         width, width, #1
                     ldr         pMask, =ne10_vresize_mask_residual_table
                     add         pMask, pMask, width, lsl #3
                     vld1.64     {dMask}, [pMask]
                     vld1.64     {dDst_01234567}, [pDst]

                     vmul.s32    qTmp_0123, qS0_0123, dBeta0
                     vmul.s32    qTmp_4567, qS0_4567, dBeta0
                     vmla.s32    qTmp_0123, qS1_0123, dBeta1
                     vmla.s32    qTmp_4567, qS1_4567, dBeta1

                     vadd.s32    qTmp_0123, qTmp_0123, qDelta
                     vadd.s32    qTmp_4567, qTmp_4567, qDelta

                     vshr.s32    qTmp_0123, qTmp_0123, #BITS
                     vshr.s32    qTmp_4567, qTmp_4567, #BITS

                     vmax.s32    qTmp_0123, qTmp_0123, qMin
                     vmax.s32    qTmp_4567, qTmp_4567, qMin
                     vmin.s32    qTmp_0123, qTmp_0123, qMax
                     vmin.s32    qTmp_4567, qTmp_4567, qMax

                     vmovn.I32    dTmp_0123, qTmp_0123
                     vmovn.I32    dTmp_4567, qTmp_4567
                     vmovn.I16    dTmp_01234567, qTmp_01234567
                     vbsl         dMask, dTmp_01234567, dDst_01234567
                     vst1.8       {dMask}, [pDst]
VResizeEnd:
                     /*Return From Function*/
                     pop     {r4-r6,pc}


/*ARM Registers*/
.unreq               pSrc
.unreq               pDst
.unreq               pBeta
.unreq               width

.unreq               pS0
.unreq               pS1
.unreq               tmp

.unreq               beta0
.unreq               beta1

.unreq               pMask

 /*NEON variale Declaration*/
.unreq               dBeta0
.unreq               dBeta1
.unreq               qDelta
.unreq               dMask

.unreq               qMin
.unreq               qMax

.unreq               qS0_0123
.unreq               qS0_4567
.unreq               qS1_0123
.unreq               qS1_4567
.unreq               qTmp_0123
.unreq               qTmp_4567
.unreq               dTmp_0123
.unreq               dTmp_4567

.unreq               qTmp_01234567
.unreq               dTmp_01234567
.unreq               dDst_01234567

        /**
         * @details
         * This function implements the horizontal interpolation
         *
         * @param[in]   **src            points to input pointers
         * @param[out]  **dst            points to the output pointers
         * @param[in]   count
         * @param[in]   *xofs            points to interpolate offset
         * @param[in]   *alpha           points to interpolate parameter
         * @param[in]   swidth           width of input buffer
         * @param[in]   dwidth           width of output buffer
         * @param[in]   cn
         * @param[in]   xmin
         * @param[in]   xmax
         */

        .align   4
        .global   ne10_img_hresize_4channels_linear_neon
        .thumb
        .thumb_func
        .equ         INTER_RESIZE_COEF_SCALE,        0x800  /* 1 << INTER_RESIZE_COEF_BITS */

ne10_img_hresize_4channels_linear_neon:
                     push    {r4-r10,lr}

/*ARM Registers*/
pIn0             .req   r0
pIn1             .req   r1
pIn2             .req   r2
pIn3             .req   r3

pSrc             .req   r0
pDst             .req   r1
pXofs            .req   r2
pAlpha           .req   r3

pS0              .req   r4
pS1              .req   r0
pD0              .req   r5
pD1              .req   r1

dwidth           .req   r6
xmax             .req   r7

sx               .req   r8
tmp              .req   r12
pTmp0            .req   r9
pTmp1            .req   r10
count            .req   r2


/*NEON variale Declaration*/
dAlpha_0         .dn   d0
dAlpha_1         .dn   d1
dCoeff           .dn   d2

dS0_01234567     .dn   d4
dS1_01234567     .dn   d5
qS0_01234567     .qn   q11
dS0_0123         .dn   d22
dS0_4567         .dn   d23
qS1_01234567     .qn   q8
dS1_0123         .dn   d16
dS1_4567         .dn   d17

qDst0_0123       .qn   q9
qDst1_0123       .qn   q10


                     subs        tmp, count, #1
                     beq         ne10_img_hresize_4channels_linear_count_1

ne10_img_hresize_4channels_linear_count_2:
                     ldr         pS0, [pSrc], #4
                     ldr         pS1, [pSrc]
                     ldr         pD0, [pDst], #4
                     ldr         pD1, [pDst]

                     mov         tmp, INTER_RESIZE_COEF_SCALE
                     vdup.16     dCoeff, tmp

                     mov         pXofs, pIn3
                     ldr         pAlpha, [sp, #32]
                     ldr         dwidth, [sp, #40]
                     ldr         xmax, [sp, #52]
                     sub         dwidth, dwidth, xmax    /* calculate the residual */

                     ldr         sx, [pXofs], #16     /* for 4 channels only, xofs is changed based on channels */
                     add         pTmp0, pS0, sx     /* find the address of starting element */
                     add         pTmp1, pS1, sx
                     vld2.16     {dAlpha_0, dAlpha_1}, [pAlpha]! /* alpha is repeated based on channels */
                     vld1.8      {dS0_01234567}, [pTmp0]
                     vld1.8      {dS1_01234567}, [pTmp1]

                     subs        xmax, xmax, #4
                     blt         ne10_img_hresize_4channels_linear_count_2_dwidth_loop

ne10_img_hresize_4channels_linear_count_2_xmax_loop:
                     vmovl.u8    qS0_01234567, dS0_01234567
                     vmovl.u8    qS1_01234567, dS1_01234567

                     vmull.u16   qDst0_0123, dS0_0123, dAlpha_0
                     vmull.u16   qDst1_0123, dS1_0123, dAlpha_0
                     vmlal.u16   qDst0_0123, dS0_4567, dAlpha_1
                     vmlal.u16   qDst1_0123, dS1_4567, dAlpha_1

                     vst1.32     {qDst0_0123}, [pD0]!
                     vst1.32     {qDst1_0123}, [pD1]!

                     ldr         sx, [pXofs], #16     /* for 4 channels only, xofs is changed based on channels */
                     add         pTmp0, pS0, sx     /* find the address of starting element */
                     add         pTmp1, pS1, sx
                     vld2.16     {dAlpha_0, dAlpha_1}, [pAlpha]! /* alpha is repeated based on channels */
                     vld1.8      {dS0_01234567}, [pTmp0]
                     vld1.8      {dS1_01234567}, [pTmp1]

                     subs        xmax, xmax, #4
                     bge         ne10_img_hresize_4channels_linear_count_2_xmax_loop

                     cmp         dwidth, #0
                     beq         ne10_img_hresize_4channels_linear_end

ne10_img_hresize_4channels_linear_count_2_dwidth_loop:

                     vmovl.u8    qS0_01234567, dS0_01234567
                     vmovl.u8    qS1_01234567, dS1_01234567

                     vmull.u16   qDst0_0123, dS0_0123, dCoeff
                     vmull.u16   qDst1_0123, dS1_0123, dCoeff

                     vst1.32     {qDst0_0123}, [pD0]!
                     vst1.32     {qDst1_0123}, [pD1]!

                     ldr         sx, [pXofs], #16     /* for 4 channels only, xofs is changed based on channels */
                     add         pTmp0, pS0, sx     /* find the address of starting element */
                     add         pTmp1, pS1, sx
                     vld1.8      {dS0_01234567}, [pTmp0]
                     vld1.8      {dS1_01234567}, [pTmp1]

                     subs        dwidth, dwidth, #4
                     bgt         ne10_img_hresize_4channels_linear_count_2_dwidth_loop

                     b           ne10_img_hresize_4channels_linear_end

ne10_img_hresize_4channels_linear_count_1:

                     ldr         pS0, [pSrc], #4
                     ldr         pD0, [pDst], #4

                     mov         tmp, INTER_RESIZE_COEF_SCALE
                     vdup.16     dCoeff, tmp

                     mov         pXofs, pIn3
                     ldr         pAlpha, [sp, #32]
                     ldr         dwidth, [sp, #40]
                     ldr         xmax, [sp, #52]
                     sub         dwidth, dwidth, xmax    /* calculate the residual */

                     ldr         sx, [pXofs], #16     /* for 4 channels only, xofs is changed based on channels */
                     add         pTmp0, pS0, sx     /* find the address of starting element */
                     vld2.16     {dAlpha_0, dAlpha_1}, [pAlpha]! /* alpha is repeated based on channels */
                     vld1.8      {dS0_01234567}, [pTmp0]

                     subs        xmax, xmax, #4
                     blt         ne10_img_hresize_4channels_linear_count_1_dwidth_loop

ne10_img_hresize_4channels_linear_count_1_xmax_loop:
                     vmovl.u8    qS0_01234567, dS0_01234567

                     vmull.u16   qDst0_0123, dS0_0123, dAlpha_0
                     vmlal.u16   qDst0_0123, dS0_4567, dAlpha_1

                     vst1.32     {qDst0_0123}, [pD0]!

                     ldr         sx, [pXofs], #16     /* for 4 channels only, xofs is changed based on channels */
                     add         pTmp0, pS0, sx     /* find the address of starting element */
                     vld2.16     {dAlpha_0, dAlpha_1}, [pAlpha]! /* alpha is repeated based on channels */
                     vld1.8      {dS0_01234567}, [pTmp0]

                     subs        xmax, xmax, #4
                     bge         ne10_img_hresize_4channels_linear_count_1_xmax_loop

                     cbz         dwidth, ne10_img_hresize_4channels_linear_end

ne10_img_hresize_4channels_linear_count_1_dwidth_loop:

                     vmovl.u8    qS0_01234567, dS0_01234567
                     vmull.u16   qDst0_0123, dS0_0123, dCoeff

                     vst1.32     {qDst0_0123}, [pD0]!

                     ldr         sx, [pXofs], #16     /* for 4 channels only, xofs is changed based on channels */
                     add         pTmp0, pS0, sx     /* find the address of starting element */
                     vld1.8      {dS0_01234567}, [pTmp0]

                     subs        dwidth, dwidth, #4
                     bgt         ne10_img_hresize_4channels_linear_count_1_dwidth_loop

ne10_img_hresize_4channels_linear_end:
                     /*Return From Function*/
                     pop     {r4-r10,pc}


 /*ARM Registers*/
.unreq               pIn0
.unreq               pIn1
.unreq               pIn2
.unreq               pIn3

.unreq               pSrc
.unreq               pDst
.unreq               pXofs
.unreq               pAlpha

.unreq               pS0
.unreq               pS1
.unreq               pD0
.unreq               pD1

.unreq               dwidth
.unreq               xmax
.unreq               sx
.unreq               tmp

/*NEON variale Declaration*/
.unreq               dAlpha_0
.unreq               dAlpha_1
.unreq               dCoeff

.unreq               dS0_01234567
.unreq               dS1_01234567
.unreq               qS0_01234567
.unreq               dS0_0123
.unreq               dS0_4567
.unreq               qS1_01234567
.unreq               dS1_0123
.unreq               dS1_4567

.unreq               qDst0_0123
.unreq               qDst1_0123


       .end
