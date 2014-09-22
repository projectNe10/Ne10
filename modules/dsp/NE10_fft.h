/*
 *  Copyright 2013-14 ARM Limited
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
 * NE10 Library : dsp/NE10_fft.h
 */


#include <NE10_types.h>

#ifdef __cplusplus
#include <algorithm>
#endif

#ifndef NE10_FFT_H
#define NE10_FFT_H

#ifdef __cplusplus
extern "C" {
#endif

///////////////////////////
// internal macro define
///////////////////////////
#define NE10_FFT_BYTE_ALIGNMENT 8
#define NE10_INLINE inline static

/* algorithms used in FFT */
#define NE10_FFT_ALG_24 0
#define NE10_FFT_ALG_ANY 1
// Comment when do not want to scale output result
#define NE10_DSP_RFFT_SCALING

#ifdef __cplusplus
  #define ne10_swap_ptr(X,Y) std::swap((X),(Y));
#else
  #define ne10_swap_ptr(X,Y) do { void *ptr = (X); (X) = (Y); (Y) = ptr; } while(0);
#endif

#define NE10_CPX_ADD(Z,A,B) do { Z.r = A.r + B.r; Z.i = A.i + B.i; } while (0);
#define NE10_CPX_SUB(Z,A,B) do { Z.r = A.r - B.r; Z.i = A.i - B.i; } while (0);
#define NE10_CPX_MUL_F32(Z,A,B) do {        \
    ne10_float32_t ARBR = A.r * B.r;        \
    ne10_float32_t AIBI = A.i * B.i;        \
    ne10_float32_t ARBI = A.r * B.i;        \
    ne10_float32_t AIBR = A.i * B.r;        \
    Z.r = ARBR - AIBI;                      \
    Z.i = AIBR + ARBI;                      \
    } while (0);
#define NE10_CPX_CONJ_MUL_F32(Z,A,B) do {   \
    ne10_float32_t ARBR = A.r * B.r;        \
    ne10_float32_t AIBI = A.i * B.i;        \
    ne10_float32_t ARBI = A.r * B.i;        \
    ne10_float32_t AIBR = A.i * B.r;        \
    Z.r = ARBR + AIBI;                      \
    Z.i = AIBR - ARBI;                      \
    } while (0);
#define NE10_CPX_MUL_TW_F32(Z,TW) do {      \
    ne10_fft_cpx_float32_t tmp;             \
    NE10_CPX_MUL(tmp,Z,TW);                 \
    Z = tmp;                                \
} while (0);
#define NE10_CPX_MUL_INV_TW_F32(Z,TW) do {  \
    ne10_fft_cpx_float32_t tmp;             \
    NE10_CPX_CONJ_MUL(tmp,Z,TW);            \
    Z = tmp;                                \
} while (0);

// R2C FFT size==4
// In[4]    R[0],R[1],R[2],R[3]
// OUT[4]   R[0],R[1],I[1],R[2]
#define NE10_FFT_R2C_4R_RCR(OUT,IN) do {    \
    ne10_float32_t SCRATCH [2];             \
    SCRATCH[0] = IN[0] + IN[2];             \
    SCRATCH[1] = IN[1] + IN[3];             \
    OUT[0] = SCRATCH[0] + SCRATCH[1];       \
    OUT[1] = IN[0] - IN[2];                 \
    OUT[2] = IN[3] - IN[1];                 \
    OUT[3] = SCRATCH[0] - SCRATCH[1];       \
} while(0);

// C2R FFT size==4  - inversed of R2C FFT
// In[4]    R[0],R[1],I[1],R[2]
// OUT[4]   R[0],R[1],R[2],R[3]
#define NE10_FFT_C2R_RCR_4R(OUT,IN) do {    \
    ne10_float32_t SCRATCH [4];             \
    SCRATCH[0] =(IN[0] + IN[3]) ;    \
    SCRATCH[1] =(IN[0] - IN[3]) ;    \
    SCRATCH[2] = IN[1] + IN[1];              \
    SCRATCH[3] = IN[2] + IN[2];              \
    OUT[0] = SCRATCH[0] + SCRATCH[2];       \
    OUT[1] = SCRATCH[1] - SCRATCH[3];       \
    OUT[2] = SCRATCH[0] - SCRATCH[2];       \
    OUT[3] = SCRATCH[1] + SCRATCH[3];       \
} while(0);

// R2C FFT size==4
// In[4]    R[0],R[1],R[2],R[3]
// OUT[4]   R[0],I[0],R[1],I[1]
#define NE10_FFT_R2C_4R_CC(OUT,IN) do {    \
    ne10_float32_t SCRATCH [2];            \
    ne10_float32_t TMP [2];                \
    SCRATCH[0] = (IN[3] - IN[1]) * TW_81N;     \
    SCRATCH[1] = (IN[3] + IN[1]) * TW_81N;     \
    OUT[0] = IN[0] + SCRATCH[0];    \
    OUT[2] = IN[0] - SCRATCH[0];    \
    OUT[1] = SCRATCH[1] - IN[2];    \
    OUT[3] = SCRATCH[1] + IN[2];    \
} while(0);

// C2R FFT size==4  - inversed of R2C FFT
// In[4]    R[0],I[0],R[1],I[1]
// OUT[4]   R[0],R[1],R[2],R[3]
#define NE10_FFT_C2R_CC_4R(OUT,IN) do {    \
    ne10_float32_t SCRATCH [4];            \
    OUT[0] = ( IN[0] + IN[2]) ;    \
    OUT[2] = (-IN[1] + IN[3]) ;    \
    OUT[0] = OUT[0] + OUT[0] ;  \
    OUT[2] = OUT[2] + OUT[2] ;  \
    SCRATCH[0] = (IN[0] - IN[2]) ;     \
    SCRATCH[1] = (IN[1] + IN[3]) ;     \
    SCRATCH[2] = (SCRATCH[0] + SCRATCH[1]) ;     \
    SCRATCH[3] = (SCRATCH[0] - SCRATCH[1]) ;     \
    OUT[3] = SCRATCH[2] / TW_81N;    \
    OUT[1] = SCRATCH[3] / TW_81;    \
} while(0);

// R2C FFT size==4
// In[4]    R[0],I[0],R[1],I[1]
// OUT[4]   R[0],I[0],R[1],I[1]
#define NE10_FFT_R2C_CC_CC(OUT,IN) do { \
    ne10_fft_cpx_float32_t TMP[4];      \
    ne10_float32_t TMP_SWAP;            \
    TMP[0].r = IN[0].r + IN[2].r;       \
    TMP[0].i = IN[0].i + IN[2].i;       \
    TMP[1].r = IN[0].r - IN[2].r;       \
    TMP[1].i = IN[0].i - IN[2].i;       \
    TMP[2].r = IN[1].r + IN[3].r;       \
    TMP[2].i = IN[1].i + IN[3].i;       \
    TMP[3].r = IN[1].r - IN[3].r;       \
    TMP[3].i = IN[1].i - IN[3].i;       \
    TMP_SWAP = TMP[3].i;                \
    TMP[3].i = - TMP[3].r;              \
    TMP[3].r = TMP_SWAP;                \
    OUT[0].r = TMP[0].r + TMP[2].r;     \
    OUT[0].i = TMP[0].i + TMP[2].i;     \
    OUT[2].r =   TMP[0].r - TMP[2].r;   \
    OUT[2].i = -(TMP[0].i - TMP[2].i);  \
    OUT[1].r = TMP[1].r + TMP[3].r;     \
    OUT[1].i = TMP[1].i + TMP[3].i;     \
    OUT[3].r =   TMP[1].r - TMP[3].r;   \
    OUT[3].i = -(TMP[1].i - TMP[3].i);  \
} while(0);


// C2R FFT size==4  - inversed of R2C FFT
// In[4]    R[0],I[0],R[1],I[1]
// OUT[4]   R[0],I[0],R[1],I[1]
#define NE10_FFT_C2R_CC_CC(OUT,IN) do {             \
    ne10_fft_cpx_float32_t SCRATCH[4];              \
    SCRATCH[0].r = (IN[0].r + IN[1].r) ;      \
    SCRATCH[2].r = (IN[0].r - IN[1].r) ;      \
    SCRATCH[2].i = (IN[0].i + IN[1].i) ;      \
    SCRATCH[0].i = (IN[0].i - IN[1].i) ;      \
    SCRATCH[1].r = (IN[2].r + IN[3].r) ;      \
    SCRATCH[3].i = (IN[2].r - IN[3].r) ;      \
    SCRATCH[3].r = (IN[2].i + IN[3].i) * -1.0f;     \
    SCRATCH[1].i = (IN[2].i - IN[3].i) ;     \
    OUT[0].r = (SCRATCH[0].r + SCRATCH[1].r) ;\
    OUT[2].r = (SCRATCH[0].r - SCRATCH[1].r) ;\
    OUT[0].i = (SCRATCH[0].i + SCRATCH[1].i) ;\
    OUT[2].i = (SCRATCH[0].i - SCRATCH[1].i) ;\
    OUT[1].r = (SCRATCH[2].r + SCRATCH[3].r) ;\
    OUT[3].r = (SCRATCH[2].r - SCRATCH[3].r) ;\
    OUT[1].i = (SCRATCH[2].i + SCRATCH[3].i) ;\
    OUT[3].i = (SCRATCH[2].i - SCRATCH[3].i) ;\
} while(0);

#ifdef PERFORMANCE_TEST
  #undef NE10_VERBOSE
#endif

#ifdef NE10_VERBOSE
  #define PRINT_STAGE_INFO do {   \
      fprintf( stdout, "%s,%d\n\t:f,m,n=%d,%d,%d\n", __FUNCTION__, __LINE__, fstride, mstride, nfft );  \
  } while(0);
  #define PRINT_POINTERS_INFO(IN,OUT,BUF,TW) do {  \
      fprintf( stdout, "%s,%d\t:"              "IN:%p\t" "OUT:%p\t" "BUF:%p\t" "TW:%p\n",    \
                        __FUNCTION__, __LINE__, IN,       OUT,       BUF,       TW);  \
  } while(0);
  #define PRINT_BUTTERFLY_INFO ;
  #define PRINT_HIT do {    \
      fprintf( stderr, "HIT %s:%d\n", __FUNCTION__, __LINE__);  \
  } while(0);
  #define PRINT_VAR(X,FORM) do {   \
      fprintf( stderr, #X "=" FORM "\n", X );                  \
  } while(0);
  #define RFORM "\t%+8.4e\t"
  #define CFORM "(" RFORM "," RFORM ")\t"

  #define NE10_PRINT_Q_VECTOR(Q_VECTOR) do {            \
      fprintf(stderr,"inside %s\n", __FUNCTION__ );     \
      fprintf(stderr, #Q_VECTOR "\n");                  \
      fprintf(stderr, RFORM RFORM RFORM RFORM "\n", Q_VECTOR[0], Q_VECTOR[1], Q_VECTOR[2], Q_VECTOR[3] ); \
  } while(0);

  #define NE10_PRINT_Q2_VECTOR(Q2_VECTOR) do {                                                                    \
      fprintf(stderr,"inside %s\n", __FUNCTION__ );                                                               \
      fprintf(stderr, #Q2_VECTOR "\n");                                                                            \
      fprintf(stderr,"REAL:" RFORM RFORM RFORM RFORM "\n", Q2_VECTOR[0].r, Q2_VECTOR[1].r, Q2_VECTOR[2].r, Q2_VECTOR[3].r ); \
      fprintf(stderr,"IMAG:" RFORM RFORM RFORM RFORM "\n", Q2_VECTOR[0].i, Q2_VECTOR[1].i, Q2_VECTOR[2].i, Q2_VECTOR[3].i ); \
  } while(0);
#else
  #define PRINT_STAGE_INFO ;
  #define PRINT_BUTTERFLY_INFO ;
  #define PRINT_HIT ;
  #define PRINT_VAR(X,FORM) ;
  #define PRINT_POINTERS_INFO(IN,OUT,BUF,TW) ;
  #define NE10_PRINT_Q_VECTOR(Q_VECTOR) ;
  #define NE10_PRINT_Q2_VECTOR(Q2_VECTOR) ;
#endif

///////////////////////////
// common varibles
///////////////////////////
const static ne10_float32_t TW_81  =  0.70710678;
const static ne10_float32_t TW_81N = -0.70710678;

///////////////////////////
// function prototypes:
///////////////////////////

    /*common fft functions */
    extern ne10_int32_t ne10_factor (ne10_int32_t n, ne10_int32_t * facbuf);

    extern ne10_fft_cpx_float32_t* ne10_fft_generate_twiddles_float32 (ne10_fft_cpx_float32_t * twiddles,
        const ne10_int32_t * factors,
        const ne10_int32_t nfft );

    /*common functions for float fft */
    extern void ne10_mixed_radix_fft_forward_float32_neon (ne10_fft_cpx_float32_t * Fout,
            ne10_fft_cpx_float32_t * Fin,
            ne10_int32_t * factors,
            ne10_fft_cpx_float32_t * twiddles,
            ne10_fft_cpx_float32_t * buffer)
    asm ("ne10_mixed_radix_fft_forward_float32_neon");

    extern void ne10_mixed_radix_fft_backward_float32_neon (ne10_fft_cpx_float32_t * Fout,
            ne10_fft_cpx_float32_t * Fin,
            ne10_int32_t * factors,
            ne10_fft_cpx_float32_t * twiddles,
            ne10_fft_cpx_float32_t * buffer)
    asm ("ne10_mixed_radix_fft_backward_float32_neon");

    /* common functions for fixed point fft */
    /* butterfly for int 32 */
    extern void ne10_mixed_radix_fft_forward_int32_unscaled_neon (ne10_fft_cpx_int32_t * Fout,
            ne10_fft_cpx_int32_t *fin,
            ne10_int32_t * factors,
            ne10_fft_cpx_int32_t * twiddles,
            ne10_fft_cpx_int32_t * buffer)
    asm ("ne10_mixed_radix_fft_forward_int32_unscaled_neon");
    extern void ne10_mixed_radix_fft_backward_int32_unscaled_neon (ne10_fft_cpx_int32_t * Fout,
            ne10_fft_cpx_int32_t *fin,
            ne10_int32_t * factors,
            ne10_fft_cpx_int32_t * twiddles,
            ne10_fft_cpx_int32_t * buffer)
    asm ("ne10_mixed_radix_fft_backward_int32_unscaled_neon");
    extern void ne10_mixed_radix_fft_forward_int32_scaled_neon (ne10_fft_cpx_int32_t * Fout,
            ne10_fft_cpx_int32_t *fin,
            ne10_int32_t * factors,
            ne10_fft_cpx_int32_t * twiddles,
            ne10_fft_cpx_int32_t * buffer)
    asm ("ne10_mixed_radix_fft_forward_int32_scaled_neon");
    extern void ne10_mixed_radix_fft_backward_int32_scaled_neon (ne10_fft_cpx_int32_t * Fout,
            ne10_fft_cpx_int32_t *fin,
            ne10_int32_t * factors,
            ne10_fft_cpx_int32_t * twiddles,
            ne10_fft_cpx_int32_t * buffer)
    asm ("ne10_mixed_radix_fft_backward_int32_scaled_neon");

    /* butterfly for int 16 */
    extern void ne10_mixed_radix_fft_forward_int16_unscaled_neon (ne10_fft_cpx_int16_t * Fout,
            ne10_fft_cpx_int16_t *fin,
            ne10_int32_t * factors,
            ne10_fft_cpx_int16_t * twiddles,
            ne10_fft_cpx_int16_t * buffer)
    asm ("ne10_mixed_radix_fft_forward_int16_unscaled_neon");
    extern void ne10_mixed_radix_fft_backward_int16_unscaled_neon (ne10_fft_cpx_int16_t * Fout,
            ne10_fft_cpx_int16_t *fin,
            ne10_int32_t * factors,
            ne10_fft_cpx_int16_t * twiddles,
            ne10_fft_cpx_int16_t * buffer)
    asm ("ne10_mixed_radix_fft_backward_int16_unscaled_neon");
    extern void ne10_mixed_radix_fft_forward_int16_scaled_neon (ne10_fft_cpx_int16_t * Fout,
            ne10_fft_cpx_int16_t *fin,
            ne10_int32_t * factors,
            ne10_fft_cpx_int16_t * twiddles,
            ne10_fft_cpx_int16_t * buffer)
    asm ("ne10_mixed_radix_fft_forward_int16_scaled_neon");
    extern void ne10_mixed_radix_fft_backward_int16_scaled_neon (ne10_fft_cpx_int16_t * Fout,
            ne10_fft_cpx_int16_t *fin,
            ne10_int32_t * factors,
            ne10_fft_cpx_int16_t * twiddles,
            ne10_fft_cpx_int16_t * buffer)
    asm ("ne10_mixed_radix_fft_backward_int16_scaled_neon");


#ifdef __cplusplus
}
#endif

#endif
