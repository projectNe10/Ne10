/*
 *  Copyright 2014-15 ARM Limited
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
 * NE10 Library : dsp/NE10_fft_debug_macro.h
 */

#include "NE10_types.h"

#ifndef NE10_FFT_DEBUG_MACRO_H
#define NE10_FFT_DEBUG_MACRO_H

#ifdef PERFORMANCE_TEST
#undef NE10_VERBOSE
#endif

#ifdef NE10_VERBOSE

#define PRINT_STAGE_INFO \
    do { \
        fprintf( stdout, "%s,%d \n \t:f,m,n=%d,%d,%d \n", __FUNCTION__, __LINE__, fstride, mstride, nfft ); \
    } while (0)

#define PRINT_POINTERS_INFO(IN,OUT,BUF,TW) \
    do { \
        fprintf( stdout, "%s,%d \t:" "IN:%p \t" "OUT:%p \t" "BUF:%p \t" "TW:%p \n", \
                  __FUNCTION__, __LINE__, IN, OUT, BUF, TW); \
    } while (0)

#define PRINT_BUTTERFLY_INFO ;

#define PRINT_HIT \
    do { \
        fprintf( stderr, "HIT %s:%d \n", __FUNCTION__, __LINE__); \
    } while (0)

#define PRINT_VAR(X,FORM) \
    do { \
        fprintf( stderr, #X "=" FORM " \n", X ); \
    } while (0)

#define RFORM " \t%+8.4e \t"

#define CFORM "(" RFORM "," RFORM ") \t"

#define NE10_PRINT_Q_VECTOR(Q_VECTOR) \
    do { \
        fprintf(stderr,"inside %s \n", __FUNCTION__ ); \
        fprintf(stderr, #Q_VECTOR " \n"); \
        fprintf(stderr, RFORM RFORM RFORM RFORM " \n", \
                Q_VECTOR[0], Q_VECTOR[1], Q_VECTOR[2], Q_VECTOR[3] ); \
    } while (0)

#define NE10_PRINT_Q2_VECTOR(Q2_VECTOR) \
    do { \
        fprintf(stderr,"inside %s \n", __FUNCTION__ ); \
        fprintf(stderr, #Q2_VECTOR " \n"); \
        fprintf(stderr,"REAL:" RFORM RFORM RFORM RFORM " \n", \
                Q2_VECTOR[0].r, Q2_VECTOR[1].r, Q2_VECTOR[2].r, Q2_VECTOR[3].r ); \
        fprintf(stderr,"IMAG:" RFORM RFORM RFORM RFORM " \n", \
                Q2_VECTOR[0].i, Q2_VECTOR[1].i, Q2_VECTOR[2].i, Q2_VECTOR[3].i ); \
    } while (0)
#else // NE10_VERBOSE

#define PRINT_STAGE_INFO ;
#define PRINT_BUTTERFLY_INFO ;
#define PRINT_HIT ;
#define PRINT_VAR(X,FORM) ;
#define PRINT_POINTERS_INFO(IN,OUT,BUF,TW) ;
#define NE10_PRINT_Q_VECTOR(Q_VECTOR) ;
#define NE10_PRINT_Q2_VECTOR(Q2_VECTOR) ;

#endif // NE10_VERBOSE

#endif // NE10_FFT_DEBUG_MACRO_H
