/*
 *  Copyright 2014-16 ARM Limited and Contributors.
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
 *  DISCLAIMED. IN NO EVENT SHALL ARM LIMITED AND CONTRIBUTORS BE LIABLE FOR ANY
 *  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * NE10 Library : dsp/NE10_fft.neonintrinsic.h
 */

#ifndef NE10_FFT_NEONINTRINSIC_H
#define NE10_FFT_NEONINTRINSIC_H

#include "NE10_fft.h"
#include <stdio.h>
#include <arm_neon.h>

#define NE10_CPX_ADD_NEON_F32(Z,A,B) do {           \
    Z.val[0] = A.val[0] + B.val[0];    \
    Z.val[1] = A.val[1] + B.val[1];    \
} while (0);

#define NE10_CPX_SUB_NEON_F32(Z,A,B) do {           \
    Z.val[0] = A.val[0] - B.val[0];    \
    Z.val[1] = A.val[1] - B.val[1];    \
} while (0);

#define NE10_CPX_MUL_NEON_F32(Z,A,B) do {           \
    float32x4_t ARBR = vmulq_f32( A.val[0], B.val[0] ); \
    float32x4_t ARBI = vmulq_f32( A.val[0], B.val[1] ); \
    Z.val[0] = vmlsq_f32(ARBR, A.val[1], B.val[1]); \
    Z.val[1] = vmlaq_f32(ARBI, A.val[1], B.val[0]); \
} while (0);

#define NE10_CPX_MUL_INV_NEON_F32(Z,A,B) do {           \
    float32x4_t ARBR = vmulq_f32( A.val[0], B.val[0] ); \
    float32x4_t AIBI = vmulq_f32( A.val[1], B.val[1] ); \
    float32x4_t ARBI = vmulq_f32( A.val[0], B.val[1] ); \
    float32x4_t AIBR = vmulq_f32( A.val[1], B.val[0] ); \
    Z.val[0] = vaddq_f32(ARBR,AIBI);                \
    Z.val[1] = vsubq_f32(AIBR,ARBI);                \
} while (0);

#define NE10_BUTTERFLY_NEON_F32(O1,O2,I1,I2) do {   \
    NE10_CPX_ADD_NEON(O1,I1,I2);                    \
    NE10_CPX_SUB_NEON(O2,I1,I2);                    \
} while(0);

#define NE10_DECLARE_2(TYPE,NAME)   TYPE NAME ## 0; \
                                    TYPE NAME ## 1;

#define NE10_DECLARE_3(TYPE,NAME)   NE10_DECLARE_2(TYPE,NAME);  \
                                    TYPE NAME ## 2;

#define NE10_DECLARE_4(TYPE,NAME)   NE10_DECLARE_3(TYPE,NAME);  \
                                    TYPE NAME ## 3;

#define NE10_DECLARE_8(TYPE,NAME)   NE10_DECLARE_4(TYPE,NAME);  \
                                    TYPE NAME ## 4; \
                                    TYPE NAME ## 5; \
                                    TYPE NAME ## 6; \
                                    TYPE NAME ## 7;

#define NE10_REVERSE_FLOAT32X4(VECTOR4F) do {                                       \
    VECTOR4F = vrev64q_f32(VECTOR4F);                                               \
    VECTOR4F = vcombine_f32( vget_high_f32( VECTOR4F ), vget_low_f32( VECTOR4F ) ); \
} while (0);

#define NE10_REVERSE_OUT_FLOAT32X4(VECTOR4F_OUT,VECTOR4F) do {                      \
    float32x4_t Q_TMP = vrev64q_f32(VECTOR4F);                                      \
    VECTOR4F_OUT = vcombine_f32( vget_high_f32( Q_TMP ), vget_low_f32( Q_TMP ) );   \
} while (0);

#define NE10_RADIX4X4C_TRANSPOSE_NEON(Q2_OUT,Q2_IN) do {                                                  \
    NE10_DECLARE_4(float32x4x2_t,q2_tmp);                                                                 \
    q2_tmp0 = vtrnq_f32 (Q2_IN ## 0 .val[0], Q2_IN ## 1 .val[0]);                                         \
    q2_tmp1 = vtrnq_f32 (Q2_IN ## 0 .val[1], Q2_IN ## 1 .val[1]);                                         \
    q2_tmp2 = vtrnq_f32 (Q2_IN ## 2 .val[0], Q2_IN ## 3 .val[0]);                                         \
    q2_tmp3 = vtrnq_f32 (Q2_IN ## 2 .val[1], Q2_IN ## 3 .val[1]);                                         \
    Q2_OUT ## 0 .val[0] = vcombine_f32 (vget_low_f32  (q2_tmp0.val[0]), vget_low_f32  (q2_tmp2.val[0]));  \
    Q2_OUT ## 0 .val[1] = vcombine_f32 (vget_low_f32  (q2_tmp1.val[0]), vget_low_f32  (q2_tmp3.val[0]));  \
    Q2_OUT ## 1 .val[0] = vcombine_f32 (vget_low_f32  (q2_tmp0.val[1]), vget_low_f32  (q2_tmp2.val[1]));  \
    Q2_OUT ## 1 .val[1] = vcombine_f32 (vget_low_f32  (q2_tmp1.val[1]), vget_low_f32  (q2_tmp3.val[1]));  \
    Q2_OUT ## 2 .val[0] = vcombine_f32 (vget_high_f32 (q2_tmp0.val[0]), vget_high_f32 (q2_tmp2.val[0]));  \
    Q2_OUT ## 2 .val[1] = vcombine_f32 (vget_high_f32 (q2_tmp1.val[0]), vget_high_f32 (q2_tmp3.val[0]));  \
    Q2_OUT ## 3 .val[0] = vcombine_f32 (vget_high_f32 (q2_tmp0.val[1]), vget_high_f32 (q2_tmp2.val[1]));  \
    Q2_OUT ## 3 .val[1] = vcombine_f32 (vget_high_f32 (q2_tmp1.val[1]), vget_high_f32 (q2_tmp3.val[1]));  \
} while(0);

#define VDUPQ_N_F32(VAR) { VAR, VAR, VAR, VAR }

#define CONST_TW_81   0.70710678
#define CONST_TW_81N -0.70710678

static const float32x4_t Q_TW_81    = VDUPQ_N_F32(CONST_TW_81 );
static const float32x4_t Q_TW_81N   = VDUPQ_N_F32(CONST_TW_81N);

#define DIV_TW81   1.4142136f
#define DIV_TW81N -1.4142136f

static const float32x4_t DIV_TW81_NEON  = VDUPQ_N_F32(DIV_TW81);
static const float32x4_t DIV_TW81N_NEON = VDUPQ_N_F32(DIV_TW81N);

#define NE10_RADIX8x4_R2C_NEON_KERNEL_S1(Q_OUT,Q_IN) do {   \
        Q_OUT ## 0 = vaddq_f32 (Q_IN ## 0, Q_IN ## 4);      \
        Q_OUT ## 1 = vsubq_f32 (Q_IN ## 0, Q_IN ## 4);      \
        Q_OUT ## 2 = vaddq_f32 (Q_IN ## 1, Q_IN ## 5);      \
        Q_OUT ## 3 = vsubq_f32 (Q_IN ## 1, Q_IN ## 5);      \
        Q_OUT ## 4 = vaddq_f32 (Q_IN ## 2, Q_IN ## 6);      \
        Q_OUT ## 5 = vsubq_f32 (Q_IN ## 2, Q_IN ## 6);      \
        Q_OUT ## 6 = vaddq_f32 (Q_IN ## 3, Q_IN ## 7);      \
        Q_OUT ## 7 = vsubq_f32 (Q_IN ## 3, Q_IN ## 7);      \
        Q_OUT ## 3 = vmulq_f32 (Q_OUT ## 3, Q_TW_81 );      \
        Q_OUT ## 7 = vmulq_f32 (Q_OUT ## 7, Q_TW_81N);      \
} while(0);

#define NE10_RADIX8x4_R2C_NEON_KERNEL_S2(Q_OUT,Q_IN) do {   \
        NE10_DECLARE_4(float32x4_t,Q_S);                    \
        Q_S0 =  vaddq_f32 (Q_IN ## 0, Q_IN ## 4);           \
        Q_S1 =  vaddq_f32 (Q_IN ## 2, Q_IN ## 6);           \
        Q_S2 =  vsubq_f32 (Q_IN ## 7, Q_IN ## 3);           \
        Q_S3 =  vaddq_f32 (Q_IN ## 3, Q_IN ## 7);           \
        Q_OUT ## 0 = vaddq_f32 (      Q_S0,      Q_S1 );    \
        Q_OUT ## 1 = vaddq_f32 ( Q_IN ## 1,      Q_S3 );    \
        Q_OUT ## 2 = vsubq_f32 (      Q_S2, Q_IN ## 5 );    \
        Q_OUT ## 3 = vsubq_f32 ( Q_IN ## 0, Q_IN ## 4 );    \
        Q_OUT ## 4 = vsubq_f32 ( Q_IN ## 6, Q_IN ## 2 );    \
        Q_OUT ## 5 = vsubq_f32 ( Q_IN ## 1,      Q_S3 );    \
        Q_OUT ## 6 = vaddq_f32 ( Q_IN ## 5,      Q_S2 );    \
        Q_OUT ## 7 = vsubq_f32 (      Q_S0,      Q_S1 );    \
} while(0);

#define NE10_RADIX8x4_C2R_NEON_KERNEL_S1(Q_OUT,Q_IN) do {   \
        NE10_DECLARE_8(float32x4_t,Q_S_IN);                 \
        Q_S_IN0 = vaddq_f32(Q_IN ## 0, Q_IN ## 7);          \
        Q_S_IN1 = vsubq_f32(Q_IN ## 0, Q_IN ## 7);          \
        Q_S_IN2 = vaddq_f32(Q_IN ## 1, Q_IN ## 5);          \
        Q_S_IN3 = vsubq_f32(Q_IN ## 1, Q_IN ## 5);          \
        Q_S_IN4 = vaddq_f32(Q_IN ## 6, Q_IN ## 2);          \
        Q_S_IN5 = vsubq_f32(Q_IN ## 6, Q_IN ## 2);          \
        Q_S_IN6 = vaddq_f32(Q_IN ## 3, Q_IN ## 3);          \
        Q_S_IN7 = vaddq_f32(Q_IN ## 4, Q_IN ## 4);          \
        Q_OUT ## 0 = vaddq_f32(Q_S_IN0, Q_S_IN6);           \
        Q_OUT ## 1 = vaddq_f32(Q_S_IN2, Q_S_IN2);           \
        Q_OUT ## 2 = vsubq_f32(Q_S_IN1, Q_S_IN7);           \
        Q_OUT ## 3 = vsubq_f32(Q_S_IN3, Q_S_IN4);           \
        Q_OUT ## 4 = vsubq_f32(Q_S_IN0, Q_S_IN6);           \
        Q_OUT ## 5 = vaddq_f32(Q_S_IN5, Q_S_IN5);           \
        Q_OUT ## 6 = vaddq_f32(Q_S_IN1, Q_S_IN7);           \
        Q_OUT ## 7 = vaddq_f32(Q_S_IN4, Q_S_IN3);           \
} while (0);

#define NE10_RADIX8x4_C2R_NEON_KERNEL_S2(Q_OUT,Q_IN) do {   \
        Q_IN ## 3 = vmulq_f32(Q_IN ## 3,DIV_TW81_NEON);     \
        Q_IN ## 7 = vmulq_f32(Q_IN ## 7,DIV_TW81N_NEON);    \
        Q_OUT ## 0 = vaddq_f32(Q_IN ## 0, Q_IN ## 1);       \
        Q_OUT ## 4 = vsubq_f32(Q_IN ## 0, Q_IN ## 1);       \
        Q_OUT ## 1 = vaddq_f32(Q_IN ## 2, Q_IN ## 3);       \
        Q_OUT ## 5 = vsubq_f32(Q_IN ## 2, Q_IN ## 3);       \
        Q_OUT ## 2 = vaddq_f32(Q_IN ## 4, Q_IN ## 5);       \
        Q_OUT ## 6 = vsubq_f32(Q_IN ## 4, Q_IN ## 5);       \
        Q_OUT ## 3 = vaddq_f32(Q_IN ## 6, Q_IN ## 7);       \
        Q_OUT ## 7 = vsubq_f32(Q_IN ## 6, Q_IN ## 7);       \
} while(0);

#define NE10_RADIX8x4_C2R_NEON_KERNEL_SCALE(Q_OUT)  do {    \
    Q_OUT ## 0 = vmulq_f32( Q_OUT ## 0, EIGH_NEON);         \
    Q_OUT ## 1 = vmulq_f32( Q_OUT ## 1, EIGH_NEON);         \
    Q_OUT ## 2 = vmulq_f32( Q_OUT ## 2, EIGH_NEON);         \
    Q_OUT ## 3 = vmulq_f32( Q_OUT ## 3, EIGH_NEON);         \
    Q_OUT ## 4 = vmulq_f32( Q_OUT ## 4, EIGH_NEON);         \
    Q_OUT ## 5 = vmulq_f32( Q_OUT ## 5, EIGH_NEON);         \
    Q_OUT ## 6 = vmulq_f32( Q_OUT ## 6, EIGH_NEON);         \
    Q_OUT ## 7 = vmulq_f32( Q_OUT ## 7, EIGH_NEON);         \
} while(0);

#define NE10_RADIX4x4_C2R_NEON_KERNEL_SCALE(Q_OUT)  do {    \
    Q_OUT ## 0 = vmulq_f32( Q_OUT ## 0, QUAD_NEON);         \
    Q_OUT ## 1 = vmulq_f32( Q_OUT ## 1, QUAD_NEON);         \
    Q_OUT ## 2 = vmulq_f32( Q_OUT ## 2, QUAD_NEON);         \
    Q_OUT ## 3 = vmulq_f32( Q_OUT ## 3, QUAD_NEON);         \
} while(0);

#define NE10_RADIX4x4_C2R_TW_NEON_KERNEL_SCALE(Q2_OUT)  do {            \
    Q2_OUT ## 0 .val[0] = vmulq_f32( Q2_OUT ## 0 .val[0], QUAD_NEON);   \
    Q2_OUT ## 1 .val[0] = vmulq_f32( Q2_OUT ## 1 .val[0], QUAD_NEON);   \
    Q2_OUT ## 2 .val[0] = vmulq_f32( Q2_OUT ## 2 .val[0], QUAD_NEON);   \
    Q2_OUT ## 3 .val[0] = vmulq_f32( Q2_OUT ## 3 .val[0], QUAD_NEON);   \
    Q2_OUT ## 0 .val[1] = vmulq_f32( Q2_OUT ## 0 .val[1], QUAD_NEON);   \
    Q2_OUT ## 1 .val[1] = vmulq_f32( Q2_OUT ## 1 .val[1], QUAD_NEON);   \
    Q2_OUT ## 2 .val[1] = vmulq_f32( Q2_OUT ## 2 .val[1], QUAD_NEON);   \
    Q2_OUT ## 3 .val[1] = vmulq_f32( Q2_OUT ## 3 .val[1], QUAD_NEON);   \
} while(0);

#define NE10_RADIX8x4_R2C_NEON_KERNEL(Q_OUT,Q_IN) do {  \
    NE10_DECLARE_8(float32x4_t,Q_S_IN);                 \
    NE10_RADIX8x4_R2C_NEON_KERNEL_S1(Q_S_IN,Q_IN);      \
    NE10_RADIX8x4_R2C_NEON_KERNEL_S2(Q_OUT,Q_S_IN);     \
} while(0);

#define NE10_RADIX4x4_R2C_NEON_KERNEL(Q_OUT,Q_IN) do {  \
    NE10_DECLARE_4(float32x4_t,Q_S_IN);                 \
    Q_S_IN0 = vaddq_f32 (Q_IN ## 0, Q_IN ## 2);         \
    Q_S_IN1 = vaddq_f32 (Q_IN ## 1, Q_IN ## 3);         \
    Q_OUT ## 0 = vaddq_f32 (Q_S_IN0, Q_S_IN1);          \
    Q_OUT ## 1 = vsubq_f32 (Q_IN##0, Q_IN##2);          \
    Q_OUT ## 2 = vsubq_f32 (Q_IN##3, Q_IN##1);          \
    Q_OUT ## 3 = vsubq_f32 (Q_S_IN0, Q_S_IN1);          \
} while(0);

#define NE10_RADIX4x4_C2R_NEON_KERNEL(Q_OUT,Q_IN) do {  \
    NE10_DECLARE_4(float32x4_t,Q_S_IN);                 \
    Q_S_IN0 = vaddq_f32 (Q_IN##0, Q_IN##3);             \
    Q_S_IN1 = vsubq_f32 (Q_IN##0, Q_IN##3);             \
    Q_S_IN2 = vaddq_f32 (Q_IN##1, Q_IN##1);             \
    Q_S_IN3 = vaddq_f32 (Q_IN##2, Q_IN##2);             \
    Q_OUT ## 0 = vaddq_f32 (Q_S_IN0, Q_S_IN2);          \
    Q_OUT ## 1 = vsubq_f32 (Q_S_IN1, Q_S_IN3);          \
    Q_OUT ## 2 = vsubq_f32 (Q_S_IN0, Q_S_IN2);          \
    Q_OUT ## 3 = vaddq_f32 (Q_S_IN1, Q_S_IN3);          \
} while(0);

#define NE10_RADIX8x4_C2R_NEON_KERNEL(Q_OUT,Q_IN) do {          \
    NE10_DECLARE_8(float32x4_t,Q_S_IN_C2R_KERNEL);              \
    NE10_RADIX8x4_C2R_NEON_KERNEL_S1(Q_S_IN_C2R_KERNEL,Q_IN);   \
    NE10_RADIX8x4_C2R_NEON_KERNEL_S2(Q_OUT,Q_S_IN_C2R_KERNEL);  \
} while(0);

#define NE10_RADIX8x4_R2C_NEON_LOAD(PTR_IN,Q_IN,IN_STEP) do { \
    Q_IN ## 0 = vld1q_f32( (ne10_float32_t*) ( PTR_IN ) );    \
    PTR_IN += IN_STEP;  \
    Q_IN ## 1 = vld1q_f32( (ne10_float32_t*) ( PTR_IN ) );    \
    PTR_IN += IN_STEP;  \
    Q_IN ## 2 = vld1q_f32( (ne10_float32_t*) ( PTR_IN ) );    \
    PTR_IN += IN_STEP;  \
    Q_IN ## 3 = vld1q_f32( (ne10_float32_t*) ( PTR_IN ) );    \
    PTR_IN += IN_STEP;  \
    Q_IN ## 4 = vld1q_f32( (ne10_float32_t*) ( PTR_IN ) );    \
    PTR_IN += IN_STEP;  \
    Q_IN ## 5 = vld1q_f32( (ne10_float32_t*) ( PTR_IN ) );    \
    PTR_IN += IN_STEP;  \
    Q_IN ## 6 = vld1q_f32( (ne10_float32_t*) ( PTR_IN ) );    \
    PTR_IN += IN_STEP;  \
    Q_IN ## 7 = vld1q_f32( (ne10_float32_t*) ( PTR_IN ) );    \
    PTR_IN += IN_STEP;  \
} while(0);

#define NE10_RADIX4x4_R2C_NEON_LOAD(PTR_IN,Q_IN,IN_STEP) do {\
    Q_IN ## 0 = vld1q_f32( (ne10_float32_t*) ( PTR_IN ) );  \
    PTR_IN += IN_STEP;                                      \
    Q_IN ## 1 = vld1q_f32( (ne10_float32_t*) ( PTR_IN ) );  \
    PTR_IN += IN_STEP;                                      \
    Q_IN ## 2 = vld1q_f32( (ne10_float32_t*) ( PTR_IN ) );  \
    PTR_IN += IN_STEP;                                      \
    Q_IN ## 3 = vld1q_f32( (ne10_float32_t*) ( PTR_IN ) );  \
    PTR_IN += IN_STEP;                                      \
} while(0);

#define NE10_RADIX8x4_R2C_NEON_STORE(PTR_OUT,Q_OUT,OUT_STEP) do {           \
     vst1q_f32( (ne10_float32_t*) ( PTR_OUT + 0 * OUT_STEP ), Q_OUT ## 0);  \
     vst1q_f32( (ne10_float32_t*) ( PTR_OUT + 1 * OUT_STEP ), Q_OUT ## 1);  \
     vst1q_f32( (ne10_float32_t*) ( PTR_OUT + 2 * OUT_STEP ), Q_OUT ## 2);  \
     vst1q_f32( (ne10_float32_t*) ( PTR_OUT + 3 * OUT_STEP ), Q_OUT ## 3);  \
     vst1q_f32( (ne10_float32_t*) ( PTR_OUT + 4 * OUT_STEP ), Q_OUT ## 4);  \
     vst1q_f32( (ne10_float32_t*) ( PTR_OUT + 5 * OUT_STEP ), Q_OUT ## 5);  \
     vst1q_f32( (ne10_float32_t*) ( PTR_OUT + 6 * OUT_STEP ), Q_OUT ## 6);  \
     vst1q_f32( (ne10_float32_t*) ( PTR_OUT + 7 * OUT_STEP ), Q_OUT ## 7);  \
} while(0);

#define NE10_RADIX4x4_R2C_NEON_STORE(PTR_OUT,Q_OUT,OUT_STEP) do {           \
     vst1q_f32( (ne10_float32_t*) ( PTR_OUT + 0 * OUT_STEP ), Q_OUT ## 0);  \
     vst1q_f32( (ne10_float32_t*) ( PTR_OUT + 1 * OUT_STEP ), Q_OUT ## 1);  \
     vst1q_f32( (ne10_float32_t*) ( PTR_OUT + 2 * OUT_STEP ), Q_OUT ## 2);  \
     vst1q_f32( (ne10_float32_t*) ( PTR_OUT + 3 * OUT_STEP ), Q_OUT ## 3);  \
} while(0);

#define NE10_RADIX4x4_R2C_TW_MUL_NEON(Q2_OUT,Q2_IN,Q2_TW) do {  \
    Q2_OUT ## 0 = Q2_IN ## 0;                                   \
    NE10_CPX_MUL_NEON_F32(Q2_OUT ## 1,Q2_IN ## 1,Q2_TW ## 0);   \
    NE10_CPX_MUL_NEON_F32(Q2_OUT ## 2,Q2_IN ## 2,Q2_TW ## 1);   \
    NE10_CPX_MUL_NEON_F32(Q2_OUT ## 3,Q2_IN ## 3,Q2_TW ## 2);   \
} while(0);

#define NE10_RADIX4x4_C2R_TW_MUL_NEON(Q2_OUT,Q2_IN,Q2_TW) do {      \
    Q2_OUT ## 0 = Q2_IN ## 0;                                       \
    NE10_CPX_MUL_INV_NEON_F32(Q2_OUT ## 1,Q2_IN ## 1,Q2_TW ## 0);   \
    NE10_CPX_MUL_INV_NEON_F32(Q2_OUT ## 2,Q2_IN ## 2,Q2_TW ## 1);   \
    NE10_CPX_MUL_INV_NEON_F32(Q2_OUT ## 3,Q2_IN ## 3,Q2_TW ## 2);   \
} while(0);

#define NE10_RADIX4x4_R2C_TW_NEON_KERNEL_S1(Q2_OUT,Q2_IN) do {  \
    NE10_CPX_ADD_NEON_F32(Q2_OUT ## 0,Q2_IN ## 0,Q2_IN ## 2);   \
    NE10_CPX_SUB_NEON_F32(Q2_OUT ## 1,Q2_IN ## 0,Q2_IN ## 2);   \
    NE10_CPX_ADD_NEON_F32(Q2_OUT ## 2,Q2_IN ## 1,Q2_IN ## 3);   \
    NE10_CPX_SUB_NEON_F32(Q2_OUT ## 3,Q2_IN ## 1,Q2_IN ## 3);   \
} while(0);

#define NE10_RADIX4x4_R2C_TW_NEON_KERNEL_S2(Q2_OUT,Q2_IN) do {  \
    Q2_OUT ## 0 .val[0] = vaddq_f32(Q2_IN ## 0 .val[0] , Q2_IN ## 2 .val[0]); \
    Q2_OUT ## 0 .val[1] = vaddq_f32(Q2_IN ## 0 .val[1] , Q2_IN ## 2 .val[1]); \
    Q2_OUT ## 2 .val[0] = vsubq_f32(Q2_IN ## 0 .val[0] , Q2_IN ## 2 .val[0]); \
    Q2_OUT ## 2 .val[1] = vsubq_f32(Q2_IN ## 2 .val[1] , Q2_IN ## 0 .val[1]); \
    Q2_OUT ## 1 .val[0] = vaddq_f32(Q2_IN ## 1 .val[0] , Q2_IN ## 3 .val[1]); \
    Q2_OUT ## 1 .val[1] = vsubq_f32(Q2_IN ## 1 .val[1] , Q2_IN ## 3 .val[0]); \
    Q2_OUT ## 3 .val[0] = vsubq_f32(Q2_IN ## 1 .val[0] , Q2_IN ## 3 .val[1]); \
    Q2_OUT ## 3 .val[1] = vaddq_f32(Q2_IN ## 3 .val[0] , Q2_IN ## 1 .val[1]); \
    Q2_OUT ## 3 .val[1] = vnegq_f32(Q2_OUT ## 3 .val[1]); \
} while(0);

#define NE10_RADIX4x4_R2C_TW_NEON_KERNEL_LAST(Q_OUT,Q_IN) do {  \
    float32x4_t Q_TMP;  \
    Q_IN ## 1 = vmulq_f32(Q_IN ## 1, Q_TW_81);  \
    Q_IN ## 3 = vmulq_f32(Q_IN ## 3, Q_TW_81);  \
    Q_TMP = vsubq_f32(Q_IN ## 1, Q_IN ## 3);    \
    Q_IN ## 3 = vaddq_f32(Q_IN ## 1, Q_IN ## 3);    \
    Q_IN ## 1 = Q_TMP;                      \
    Q_OUT ## 0 = vaddq_f32(Q_IN ## 0, Q_IN ## 1);   \
    Q_OUT ## 1 = vaddq_f32(Q_IN ## 2, Q_IN ## 3);   \
    Q_OUT ## 2 = vsubq_f32(Q_IN ## 0, Q_IN ## 1);   \
    Q_OUT ## 3 = vsubq_f32(Q_IN ## 2, Q_IN ## 3);   \
    Q_OUT ## 1 = vnegq_f32(Q_OUT ## 1);         \
} while(0);

#define NE10_RADIX4x4_C2R_TW_NEON_KERNEL_LAST(Q_OUT,Q_IN) do {  \
    float32x4_t Q_TMP;  \
    Q_IN ## 1 = vnegq_f32(Q_IN ## 1 );  \
    Q_OUT ## 0 = vaddq_f32(Q_IN ## 0, Q_IN ## 2);   \
    Q_OUT ## 1 = vsubq_f32(Q_IN ## 0, Q_IN ## 2);   \
    Q_OUT ## 2 = vaddq_f32(Q_IN ## 1, Q_IN ## 3);   \
    Q_OUT ## 3 = vsubq_f32(Q_IN ## 1, Q_IN ## 3);   \
    Q_TMP = vaddq_f32(Q_OUT ## 1, Q_OUT ## 3);  \
    Q_OUT ## 3 = vsubq_f32(Q_OUT ## 3, Q_OUT ## 1);  \
    Q_OUT ## 1 = Q_TMP; \
    Q_OUT ## 1 = vmulq_f32( Q_OUT ## 1, DIV_TW81_NEON); \
    Q_OUT ## 3 = vmulq_f32( Q_OUT ## 3, DIV_TW81_NEON); \
    Q_OUT ## 0 = vaddq_f32( Q_OUT ## 0, Q_OUT ## 0 );   \
    Q_OUT ## 2 = vaddq_f32( Q_OUT ## 2, Q_OUT ## 2 );   \
} while(0);

#define NE10_RADIX4x4_C2R_TW_NEON_KERNEL_S1(Q2_OUT,Q2_IN) do {  \
    Q2_IN ## 3 .val[1] = vnegq_f32(Q2_IN ## 3 .val[1]); \
    Q2_OUT ## 0 .val[0] = vaddq_f32(Q2_IN ## 0 .val[0] , Q2_IN ## 2 .val[0]); \
    Q2_OUT ## 0 .val[1] = vsubq_f32(Q2_IN ## 0 .val[1] , Q2_IN ## 2 .val[1]); \
    Q2_OUT ## 2 .val[0] = vsubq_f32(Q2_IN ## 0 .val[0] , Q2_IN ## 2 .val[0]); \
    Q2_OUT ## 2 .val[1] = vaddq_f32(Q2_IN ## 2 .val[1] , Q2_IN ## 0 .val[1]); \
    Q2_OUT ## 1 .val[0] = vaddq_f32(Q2_IN ## 1 .val[0] , Q2_IN ## 3 .val[0]); \
    Q2_OUT ## 1 .val[1] = vaddq_f32(Q2_IN ## 1 .val[1] , Q2_IN ## 3 .val[1]); \
    Q2_OUT ## 3 .val[0] = vsubq_f32(Q2_IN ## 3 .val[1] , Q2_IN ## 1 .val[1]); \
    Q2_OUT ## 3 .val[1] = vsubq_f32(Q2_IN ## 1 .val[0] , Q2_IN ## 3 .val[0]); \
} while(0);

#define NE10_RADIX4x4_C2R_TW_NEON_KERNEL_S2(Q2_OUT,Q2_IN) do {  \
    NE10_CPX_ADD_NEON_F32(Q2_OUT ## 0,Q2_IN ## 0,Q2_IN ## 1);   \
    NE10_CPX_SUB_NEON_F32(Q2_OUT ## 2,Q2_IN ## 0,Q2_IN ## 1);   \
    NE10_CPX_ADD_NEON_F32(Q2_OUT ## 1,Q2_IN ## 2,Q2_IN ## 3);   \
    NE10_CPX_SUB_NEON_F32(Q2_OUT ## 3,Q2_IN ## 2,Q2_IN ## 3);   \
} while(0);

#define NE10_RADIX4x4_R2C_TW_NEON_KERNEL(Q2_OUT,Q2_IN,Q2_TW) do {   \
    NE10_RADIX4x4_R2C_TW_MUL_NEON(Q2_OUT,Q2_IN,Q2_TW);              \
    NE10_RADIX4x4_R2C_TW_NEON_KERNEL_S1(Q2_IN,Q2_OUT);              \
    NE10_RADIX4x4_R2C_TW_NEON_KERNEL_S2(Q2_OUT,Q2_IN);              \
} while(0);

#define NE10_RADIX4x4_C2R_TW_NEON_KERNEL(Q2_OUT,Q2_IN,Q2_TW) do {   \
    NE10_RADIX4x4_C2R_TW_NEON_KERNEL_S1(Q2_OUT,Q2_IN);              \
    NE10_RADIX4x4_C2R_TW_NEON_KERNEL_S2(Q2_IN,Q2_OUT);              \
    NE10_RADIX4x4_C2R_TW_MUL_NEON(Q2_OUT,Q2_IN,Q2_TW);              \
} while(0);

#ifdef NE10_VERBOSE
  #define NE10_PRINT_Qx8_VECTOR(Q_VECTOR) do {                                                                    \
      fprintf(stderr,"inside %s\n", __FUNCTION__ );                                                               \
      fprintf(stderr, #Q_VECTOR "\n");                                                                            \
      fprintf(stderr,"0:%+8.2e,%+8.2e,%+8.2e,%+8.2e\n", Q_VECTOR ## 0[0], Q_VECTOR ## 0[1], Q_VECTOR ## 0[2], Q_VECTOR ## 0[3] ); \
      fprintf(stderr,"1:%+8.2e,%+8.2e,%+8.2e,%+8.2e\n", Q_VECTOR ## 1[0], Q_VECTOR ## 1[1], Q_VECTOR ## 1[2], Q_VECTOR ## 1[3] ); \
      fprintf(stderr,"2:%+8.2e,%+8.2e,%+8.2e,%+8.2e\n", Q_VECTOR ## 2[0], Q_VECTOR ## 2[1], Q_VECTOR ## 2[2], Q_VECTOR ## 2[3] ); \
      fprintf(stderr,"3:%+8.2e,%+8.2e,%+8.2e,%+8.2e\n", Q_VECTOR ## 3[0], Q_VECTOR ## 3[1], Q_VECTOR ## 3[2], Q_VECTOR ## 3[3] ); \
      fprintf(stderr,"4:%+8.2e,%+8.2e,%+8.2e,%+8.2e\n", Q_VECTOR ## 4[0], Q_VECTOR ## 4[1], Q_VECTOR ## 4[2], Q_VECTOR ## 4[3] ); \
      fprintf(stderr,"5:%+8.2e,%+8.2e,%+8.2e,%+8.2e\n", Q_VECTOR ## 5[0], Q_VECTOR ## 5[1], Q_VECTOR ## 5[2], Q_VECTOR ## 5[3] ); \
      fprintf(stderr,"6:%+8.2e,%+8.2e,%+8.2e,%+8.2e\n", Q_VECTOR ## 6[0], Q_VECTOR ## 6[1], Q_VECTOR ## 6[2], Q_VECTOR ## 6[3] ); \
      fprintf(stderr,"7:%+8.2e,%+8.2e,%+8.2e,%+8.2e\n", Q_VECTOR ## 7[0], Q_VECTOR ## 7[1], Q_VECTOR ## 7[2], Q_VECTOR ## 7[3] ); \
  } while(0);
  #define NE10_PRINT_Qx4_VECTOR(Q_VECTOR) do {                                                                    \
      fprintf(stderr,"inside %s\n", __FUNCTION__ );                                                               \
      fprintf(stderr, #Q_VECTOR "\n");                                                                            \
      fprintf(stderr,"0:%+8.2e,%+8.2e,%+8.2e,%+8.2e\n", Q_VECTOR ## 0[0], Q_VECTOR ## 0[1], Q_VECTOR ## 0[2], Q_VECTOR ## 0[3] ); \
      fprintf(stderr,"1:%+8.2e,%+8.2e,%+8.2e,%+8.2e\n", Q_VECTOR ## 1[0], Q_VECTOR ## 1[1], Q_VECTOR ## 1[2], Q_VECTOR ## 1[3] ); \
      fprintf(stderr,"2:%+8.2e,%+8.2e,%+8.2e,%+8.2e\n", Q_VECTOR ## 2[0], Q_VECTOR ## 2[1], Q_VECTOR ## 2[2], Q_VECTOR ## 2[3] ); \
      fprintf(stderr,"3:%+8.2e,%+8.2e,%+8.2e,%+8.2e\n", Q_VECTOR ## 3[0], Q_VECTOR ## 3[1], Q_VECTOR ## 3[2], Q_VECTOR ## 3[3] ); \
  } while(0);
  #define NE10_PRINT_Q2x4_VECTOR(Q_VECTOR) do {                                                                   \
      fprintf(stderr,"inside %s\n", __FUNCTION__ );                                                               \
      fprintf(stderr, #Q_VECTOR "\n");                                                                            \
      fprintf(stderr,"0R:%+8.2e,%+8.2e,%+8.2e,%+8.2e\n", Q_VECTOR ## 0 .val[0][0], Q_VECTOR ## 0 .val[0][1], Q_VECTOR ## 0 .val[0][2], Q_VECTOR ## 0 .val[0][3] ); \
      fprintf(stderr,"1R:%+8.2e,%+8.2e,%+8.2e,%+8.2e\n", Q_VECTOR ## 1 .val[0][0], Q_VECTOR ## 1 .val[0][1], Q_VECTOR ## 1 .val[0][2], Q_VECTOR ## 1 .val[0][3] ); \
      fprintf(stderr,"2R:%+8.2e,%+8.2e,%+8.2e,%+8.2e\n", Q_VECTOR ## 2 .val[0][0], Q_VECTOR ## 2 .val[0][1], Q_VECTOR ## 2 .val[0][2], Q_VECTOR ## 2 .val[0][3] ); \
      fprintf(stderr,"3R:%+8.2e,%+8.2e,%+8.2e,%+8.2e\n", Q_VECTOR ## 3 .val[0][0], Q_VECTOR ## 3 .val[0][1], Q_VECTOR ## 3 .val[0][2], Q_VECTOR ## 3 .val[0][3] ); \
      fprintf(stderr,"0I:%+8.2e,%+8.2e,%+8.2e,%+8.2e\n", Q_VECTOR ## 0 .val[1][0], Q_VECTOR ## 0 .val[1][1], Q_VECTOR ## 0 .val[1][2], Q_VECTOR ## 0 .val[1][3] ); \
      fprintf(stderr,"1I:%+8.2e,%+8.2e,%+8.2e,%+8.2e\n", Q_VECTOR ## 1 .val[1][0], Q_VECTOR ## 1 .val[1][1], Q_VECTOR ## 1 .val[1][2], Q_VECTOR ## 1 .val[1][3] ); \
      fprintf(stderr,"2I:%+8.2e,%+8.2e,%+8.2e,%+8.2e\n", Q_VECTOR ## 2 .val[1][0], Q_VECTOR ## 2 .val[1][1], Q_VECTOR ## 2 .val[1][2], Q_VECTOR ## 2 .val[1][3] ); \
      fprintf(stderr,"3I:%+8.2e,%+8.2e,%+8.2e,%+8.2e\n", Q_VECTOR ## 3 .val[1][0], Q_VECTOR ## 3 .val[1][1], Q_VECTOR ## 3 .val[1][2], Q_VECTOR ## 3 .val[1][3] ); \
  } while(0);
#else // NE10_VERBOSE not defined
  #define NE10_PRINT_Qx8_VECTOR(Q_VECTOR) ;
  #define NE10_PRINT_Qx4_VECTOR(Q_VECTOR) ;
  #define NE10_PRINT_Q2x4_VECTOR(Q2_VECTOR) ;
#endif // NE10_VERBOSE
#endif // header
