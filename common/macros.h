/*
 *  Copyright 2011-16 ARM Limited and Contributors.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    * Neither the name of the <organization> nor the
 *      names of its contributors may be used to endorse or promote products
 *      derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL ARM Limited and Contributors. BE LIABLE FOR ANY
 *  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * NE10 Library : common/macros.h
 */

#include "factor.h"

/*
 * The following macros are used within some of the NEON math function implementations.
 *
 * With the exception of the VEC4F macros, which can always wholly fill NEON SIMD vectors,
 * each macro takes two parameters -- loopCode1 for the code to be run within the main
 * SIMD loop, and loopCode2 for the code to be run in processing any leftover elements.
 * The details of the variables that are exposed within these macros can be viewed in the
 * specific MAINLOOP and SECONDLOOP sub-macros that each macro utilizes.
 */

#define NE10_DstSrcCst_DO_COUNT_TIMES_FLOAT_NEON(loopCode1, loopCode2) { \
    NE10_CHECKPOINTER_DstSrcCst; \
    float32x4_t n_cst = { cst, cst, cst, cst }; \
    NE10_DstSrcCst_OPERATION_FLOAT_NEON(  \
        NE10_DstSrcCst_MAINLOOP_FLOAT_NEON(loopCode1); , \
        NE10_DstSrcCst_SECONDLOOP_FLOAT_NEON(loopCode2); \
    ); \
}

#define NE10_DstSrcCst_DO_COUNT_TIMES_VEC2F_NEON(loopCode1, loopCode2) { \
    NE10_CHECKPOINTER_DstSrcCst; \
    NE10_DstSrcCst_OPERATION_VEC2F_NEON(  \
        NE10_DstSrcCst_MAINLOOP_VEC2F_NEON(loopCode1); , \
        NE10_DstSrcCst_SECONDLOOP_VEC2F_NEON(loopCode2); \
    ); \
}

/* This macro uses interleaving to boost the performance */
#define NE10_DstSrcCst_DO_COUNT_TIMES_VEC3F_NEON(loopCode1, loopCode2) { \
    NE10_CHECKPOINTER_DstSrcCst; \
    NE10_DstSrcCst_OPERATION_VEC3F_NEON(  \
        NE10_DstSrcCst_MAINLOOP_VEC3F_NEON(loopCode1); , \
        NE10_DstSrcCst_SECONDLOOP_VEC3F_NEON(loopCode2); \
    ); \
}

#define NE10_DstSrcCst_DO_COUNT_TIMES_VEC4F_NEON(loopCode) { \
    NE10_CHECKPOINTER_DstSrcCst; \
        NE10_DstSrcCst_OPERATION_VEC4F_NEON( \
        NE10_DstSrcCst_MAINLOOP_VEC4F_NEON(loopCode); \
    ); \
}

#define NE10_DstAccSrcCst_DO_COUNT_TIMES_FLOAT_NEON(loopCode1, loopCode2) { \
    float32x4_t n_acc; \
    float32x4_t n_cst = { cst, cst, cst, cst }; \
    NE10_CHECKPOINTER_DstAccSrcCst; \
    NE10_DstAccSrcCst_OPERATION_FLOAT_NEON(  \
        NE10_DstAccSrcCst_MAINLOOP_FLOAT_NEON(loopCode1); , \
        NE10_DstAccSrcCst_SECONDLOOP_FLOAT_NEON(loopCode2); \
    ); \
}

#define NE10_DstAccSrcCst_DO_COUNT_TIMES_VEC2F_NEON(loopCode1, loopCode2) { \
    float32x4_t n_acc; \
    NE10_CHECKPOINTER_DstAccSrcCst; \
    NE10_DstAccSrcCst_OPERATION_VEC2F_NEON(  \
        NE10_DstAccSrcCst_MAINLOOP_VEC2F_NEON(loopCode1); , \
        NE10_DstAccSrcCst_SECONDLOOP_VEC2F_NEON(loopCode2); \
    ); \
}

#define NE10_DstAccSrcCst_DO_COUNT_TIMES_VEC3F_NEON(loopCode1, loopCode2) { \
    float32x4_t n_acc1, n_acc2, n_acc3; \
    NE10_CHECKPOINTER_DstAccSrcCst; \
    NE10_DstAccSrcCst_OPERATION_VEC3F_NEON(  \
        NE10_DstAccSrcCst_MAINLOOP_VEC3F_NEON(loopCode1); , \
        NE10_DstAccSrcCst_SECONDLOOP_VEC3F_NEON(loopCode2); \
    ); \
}

#define NE10_DstAccSrcCst_DO_COUNT_TIMES_VEC4F_NEON(loopCode) { \
    float32x4_t n_acc; \
    NE10_CHECKPOINTER_DstAccSrcCst; \
        NE10_DstAccSrcCst_OPERATION_VEC4F_NEON( \
        NE10_DstAccSrcCst_MAINLOOP_VEC4F_NEON(loopCode); \
    ); \
}

#define NE10_DstCst_DO_COUNT_TIMES_FLOAT_NEON(loopCode1, loopCode2) { \
    float32x4_t n_cst = { cst, cst, cst, cst }; \
    NE10_CHECKPOINTER_DstCst; \
    NE10_DstCst_OPERATION_FLOAT_NEON(  \
        NE10_DstCst_MAINLOOP_FLOAT_NEON(loopCode1); , \
        NE10_DstCst_SECONDLOOP_FLOAT_NEON(loopCode2); \
    ); \
}

#define NE10_DstCst_DO_COUNT_TIMES_VEC2F_NEON(loopCode1, loopCode2) { \
    NE10_CHECKPOINTER_DstCst; \
    NE10_DstCst_OPERATION_VEC2F_NEON(  \
        NE10_DstCst_MAINLOOP_VEC2F_NEON(loopCode1); , \
        NE10_DstCst_SECONDLOOP_VEC2F_NEON(loopCode2); \
    ); \
}

/* This macro uses interleaving to boost the performance */
#define NE10_DstCst_DO_COUNT_TIMES_VEC3F_NEON(loopCode1, loopCode2) { \
    NE10_CHECKPOINTER_DstCst; \
    NE10_DstCst_OPERATION_VEC3F_NEON(  \
        NE10_DstCst_MAINLOOP_VEC3F_NEON(loopCode1); , \
        NE10_DstCst_SECONDLOOP_VEC3F_NEON(loopCode2); \
    ); \
}

#define NE10_DstCst_DO_COUNT_TIMES_VEC4F_NEON(loopCode) { \
    NE10_CHECKPOINTER_DstCst; \
    NE10_DstCst_OPERATION_VEC4F_NEON( \
        NE10_DstCst_MAINLOOP_VEC4F_NEON(loopCode); \
    ); \
}

#define NE10_DstSrc1Src2_DO_COUNT_TIMES_FLOAT_NEON(loopCode1, loopCode2) { \
    float32x4_t n_src2; \
    NE10_CHECKPOINTER_DstSrc1Src2; \
    NE10_DstSrc1Src2_OPERATION_FLOAT_NEON(  \
        NE10_DstSrc1Src2_MAINLOOP_FLOAT_NEON(loopCode1); , \
        NE10_DstSrc1Src2_SECONDLOOP_FLOAT_NEON(loopCode2); \
    ); \
}

#define NE10_DstAccSrc1Src2_DO_COUNT_TIMES_FLOAT_NEON(loopCode1, loopCode2) { \
    float32x4_t n_acc; \
    float32x4_t n_src2; \
    NE10_CHECKPOINTER_DstAccSrc1Src2; \
    NE10_DstAccSrc1Src2_OPERATION_FLOAT_NEON(  \
        NE10_DstAccSrc1Src2_MAINLOOP_FLOAT_NEON(loopCode1); , \
        NE10_DstAccSrc1Src2_SECONDLOOP_FLOAT_NEON(loopCode2); \
    ); \
}
