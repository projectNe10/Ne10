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
 * NE10 Library : inc/NE10_macros.h
 */

/** NE10 defines a number of macros for use in its function signatures.
 *  The macros are defined within this header file.
 */

#ifndef NE10_MACROS_H
#define NE10_MACROS_H

#ifdef __cplusplus
extern "C" {
#endif

/////////////////////////////////////////////////////////
// constant values that are used across the library
/////////////////////////////////////////////////////////

#define NE10_PI (ne10_float32_t)(3.1415926535897932384626433832795)

/////////////////////////////////////////////////////////
// some external macro definitions to be exposed to the users
/////////////////////////////////////////////////////////

#define NE10_MALLOC malloc
#define NE10_FREE(p) \
    do { \
        free(p); \
        p = NULL; \
    }while(0)

#define NE10_MIN(a,b) ((a)>(b)?(b):(a))
#define NE10_MAX(a,b) ((a)<(b)?(b):(a))

#define NE10_BYTE_ALIGNMENT(address, alignment) \
    do { \
        (address) = (((address) + ((alignment) - 1)) & ~ ((alignment) - 1)); \
    }while (0)

/////////////////////////////////////////////////////////
// macro definitions for float to fixed point
/////////////////////////////////////////////////////////
#define NE10_F2I16_MAX         32767
#define NE10_F2I16_SHIFT       15
#define NE10_F2I16_SAMPPROD    ne10_int32_t
#define NE10_F2I16_OP(x)       (ne10_int16_t)((x)*NE10_F2I16_MAX + 0.5f)
#define NE10_F2I16_SROUND(x)   (ne10_int16_t)((((x)<<1)+(1<<NE10_F2I16_SHIFT))>>16)
#define NE10_F2I16_SMUL(a,b)   ((NE10_F2I16_SAMPPROD)(a)*(b))
#define NE10_F2I16_FIXDIV(c,div) \
    do {    ((c).r) = ( ( ((c).r)/div) );  \
        ((c).i) = ( ( ((c).i)/div) ); }while (0)

#define NE10_F2I32_MAX         2147483647
#define NE10_F2I32_SHIFT       31
#define NE10_F2I32_SAMPPROD    ne10_int64_t
#define NE10_F2I32_OP(x)       (ne10_int32_t)((x)*NE10_F2I32_MAX + 0.5f)
#define NE10_F2I32_SROUND(x)   (ne10_int32_t) ((x)>>NE10_F2I32_SHIFT)
#define NE10_F2I32_SMUL(a,b)    ((NE10_F2I32_SAMPPROD)(a)*(b))
#define NE10_F2I32_FIXDIV(c,div) \
    do {    ((c).r) = ( ( ((c).r)/div) );  \
        ((c).i) = ( ( ((c).i)/div) ); }while (0)

#ifdef __cplusplus
}
#endif

#endif
