/*
 *  Copyright 2011-15 ARM Limited and Contributors.
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
 * NE10 Library : common/factor.h
 */

// Typebuilding macros (slight difference between toolchain versions on intrinsics)
#define FLOAT32_2x3(x1,y1,x2,y2,x3,y3) \
    {{ \
        {x1, y1}, {x2,y2}, {x3,y3} \
    }}

// There are several categories of functions that share common code. Different groups of
// functions take different number of inputs.
//
// Group 1 = Functions that take a dst, a src, and a cst ("DstSrcCst" for short)
// Group 2 = Those that take a dst, an acc, a src, and a cst ("DstAccSrcCst" for short)
// Group 3 = The ones that take a dst, and a cst only ("DstCst" for short)
//
// Group 4 = These take a dst, and two src inputs, src2 and scr2 ("DstSrc1Src2")
// Group 5 = These take a dst, an acc, and two src inputs ("DstAccSrc1Src2")

// A few macros to check pointers and their address range to make sure there's
// no unwanted overlap between any two of them
#define NE10_CHECKPOINTER_DstSrcCst \
   if ( (void *)dst < (void *)src ) \
    { assert ( (void *)dst + count <= (void *)src ); } \
   else if ( (void *)dst > (void *)src ) \
    { assert ( (void *)src + count <= (void *)dst ); }

#define NE10_CHECKPOINTER_DstSrc NE10_CHECKPOINTER_DstSrcCst

#define NE10_CHECKPOINTER_3POINTER(arg1, arg2, arg3) \
   if ( (void *)arg1 < (void *)arg2 ) \
    { assert ( (void *)arg1 + count <= (void *)arg2 ); } \
   else if ( (void *)arg1 > (void *)arg2 ) \
    { assert ( (void *)arg2 + count <= (void *)arg1 ); } \
   if ( (void *)arg1 < (void *)arg3 ) \
    { assert ( (void *)arg1 + count <= (void *)arg3 ); } \
   else if ( (void *)arg1 > (void *)arg3 ) \
    { assert ( (void *)arg3 + count <= (void *)arg1 ); } \
   if ( (void *)arg3 < (void *)arg2 ) \
    { assert ( (void *)arg3 + count <= (void *)arg2 ); } \
   else if ( (void *)arg3 > (void *)arg2 ) \
    { assert ( (void *)arg2 + count <= (void *)arg3 ); }

#define NE10_CHECKPOINTER_4POINTER(arg1, arg2, arg3, arg4) \
   NE10_CHECKPOINTER_3POINTER(arg1, arg2, arg3) \
   if ( (void *)arg1 < (void *)arg4 ) \
    { assert ( (void *)arg1 + count <= (void *)arg4 ); } \
   else if ( (void *)arg1 > (void *)arg4 ) \
    { assert ( (void *)arg4 + count <= (void *)arg1 ); } \
   if ( (void *)arg2 < (void *)arg4 ) \
    { assert ( (void *)arg2 + count <= (void *)arg4 ); } \
   else if ( (void *)arg2 > (void *)arg4 ) \
    { assert ( (void *)arg4 + count <= (void *)arg2 ); } \
   if ( (void *)arg4 < (void *)arg3 ) \
    { assert ( (void *)arg4 + count <= (void *)arg3 ); } \
   else if ( (void *)arg4 > (void *)arg3 ) \
    { assert ( (void *)arg3 + count <= (void *)arg4 ); }



#define NE10_CHECKPOINTER_DstAccSrcCst { \
   NE10_CHECKPOINTER_3POINTER(dst, acc, src); }

#define NE10_CHECKPOINTER_DstCst  {}

#define NE10_CHECKPOINTER_DstSrc1Src2 { \
   NE10_CHECKPOINTER_3POINTER(dst, src1, src2); }

#define NE10_CHECKPOINTER_DstAccSrc1Src2 { \
   NE10_CHECKPOINTER_4POINTER(dst, acc, src1, src2); }


// Main Loop = The loop where the number of items to be processed is exactly the
//              number that we can process in a single iteration.
//
// Secondary Loop = The loop that follows a Main Loop to fill in the entries that
//                   did not fit into the Main Loop. This is needed when the number of
//                   input items is not a multiple of the number of items that we
//                   process in every iteration of the Main Loop.


/****************************************************
 *                                                  *
 *  The "DstSrcCst" group of functions              *
 *                                                  *
 ****************************************************/

///// - FLOAT - /////

#define NE10_DstSrcCst_MAINLOOP_FLOAT_NEON(loopCode) { \
     /* load 4 values  */ \
     n_src = vld1q_f32( (float32_t*)src ); \
     src += 4; /* move to the next 4 float items; 4*float */ \
     loopCode; /* the actual operation is placed here... */ /* The main loop iterates through four float values each time */ \
     vst1q_f32 ( (float32_t*)dst , n_dst ); /* store the results back */ \
     dst += 4; /* move to the next items; 4*float */ \
    }

#define NE10_DstSrcCst_SECONDLOOP_FLOAT_NEON(loopCode) { \
      float32x2_t n_rest = { 0.0f , 0.0f }; /* temporary storage to be used with NEON load/store intrinsics */ \
      float32x2_t n_rest_cst = { cst, cst }; /* temporary constant value for use in the main NEON operation */ \
      n_rest = vld1_lane_f32 ( (float32_t*)src, n_rest, 0); /* load into the first lane of d0 */ \
      loopCode; /* the actual operation is placed here ... */ /* exceptional cases where the count is not a multiple of 4 */ \
      vst1_lane_f32( (float32_t*)dst, n_rest, 0); /* store the lane back into the memory */ \
      /* move to the next item in the stream */ \
      src++; \
      dst++; \
     }

#define NE10_DstSrcCst_OPERATION_FLOAT_NEON(loopCode1, loopCode2) { \
   ne10_result_t res = NE10_OK; \
   float32x4_t n_src; \
   float32x4_t n_dst; \
   int dif = 0; \
   dif = count % 4; /* either 0 or one of 1,2,3; in the latter cases the second path is taken */ \
   for (; count > dif; count -= 4) { \
     loopCode1; \
    } \
   if ( 0 != dif ) { \
    unsigned int idx; \
    for ( idx = 0 ; idx < dif; idx++ ) { \
      loopCode2; \
     } \
    } \
   return res; \
  }

///// - VEC2F - /////

#define NE10_DstSrcCst_MAINLOOP_VEC2F_NEON(loopCode) { \
     n_src = vld1q_f32( (float32_t*)src ); /* load two vectors */ \
     src += 2; /* move to the next two vectors */ \
     loopCode; /* actual operation */ /* The main loop iterates through two 2D vectors each time */ \
     vst1q_f32 ( (float32_t*)dst , n_dst ); /* store back */ \
     dst += 2; /* move to the next 2 vectors */ \
    }

#define NE10_DstSrcCst_SECONDLOOP_VEC2F_NEON(loopCode) { \
     float32x2_t n_rest; \
     float32x2_t n_rest_cst = { cst->x, cst->y }; \
     n_rest = vld1_f32( (float32_t*)src  ); \
     loopCode; /* exceptional cases where the count isn't a multiple of 2 */ \
     vst1_f32( (float32_t*)dst, n_rest); \
    }

#define NE10_DstSrcCst_OPERATION_VEC2F_NEON(loopCode1, loopCode2) { \
   ne10_result_t res = NE10_OK; \
   float32x4_t n_cst = { cst->x, cst->y, cst->x, cst->y }; \
   float32x4_t n_src; \
   float32x4_t n_dst; \
   int dif = count % 2; \
   for (; count > dif; count -= 2) { \
    loopCode1; \
   } \
   if ( 0 != dif ) { \
    loopCode2; \
   } \
   return res; \
  }

///// - VEC3F - /////

#define NE10_DstSrcCst_MAINLOOP_VEC3F_NEON(loopCode) { \
     n_src1 = vld1q_f32( (float32_t*)src ); \
     src = ((void*)src)+(4*sizeof(ne10_float32_t)); \
     n_src2 = vld1q_f32( (float32_t*)src ); \
     src = ((void*)src)+(4*sizeof(ne10_float32_t)); \
     n_src3 = vld1q_f32( (float32_t*)src ); \
     src = ((void*)src)+(4*sizeof(ne10_float32_t)); \
     loopCode; /* The main loop iterates through three 3D vectors each time */ \
     vst1q_f32 ( (float32_t*)dst , n_dst1 ); \
     dst = ((void*)dst)+(4*sizeof(ne10_float32_t)); \
     vst1q_f32 ( (float32_t*)dst , n_dst2 ); \
     dst = ((void*)dst)+(4*sizeof(ne10_float32_t)); \
     vst1q_f32 ( (float32_t*)dst , n_dst3 ); \
     dst = ((void*)dst)+(4*sizeof(ne10_float32_t)); \
  }

#define NE10_DstSrcCst_SECONDLOOP_VEC3F_NEON(loopCode) { \
      float32x2x3_t n_rest = FLOAT32_2x3( \
        0.0f, 0.0f, 0.0f , 0.0f, 0.0f , 0.0f); \
      float32x2x3_t n_rest_cst = { (const float32x2_t){cst->x, 0}, \
             (const float32x2_t){cst->y, 0}, (const float32x2_t){cst->z, 0} }; \
      n_rest = vld3_lane_f32 ( (float32_t*)src, n_rest, 0); \
      loopCode; /* exceptional cases where the count isn't a multiple of 3 */ \
      vst3_lane_f32( (float32_t*)dst, n_rest, 0); \
      src++; \
      dst++; \
     }

#define NE10_DstSrcCst_OPERATION_VEC3F_NEON(loopCode1, loopCode2) { \
   ne10_result_t res = NE10_OK; \
   float32x4_t n_cst1 = { cst->x, cst->y, cst->z, cst->x }; \
   float32x4_t n_cst2 = { cst->y, cst->z, cst->x, cst->y }; \
   float32x4_t n_cst3 = { cst->z, cst->x, cst->y, cst->z }; \
    float32x4_t n_src1, n_src2, n_src3; \
   float32x4_t n_dst1, n_dst2, n_dst3; \
   int dif = count % 4;  \
   for (; count > dif; count -= 4) { \
    loopCode1; \
  } \
  if ( 0 != dif ) { \
    unsigned int idx; \
    for ( idx = 0 ; idx < dif; idx++ ) { \
      loopCode2; \
     } \
    } \
   return res; \
  }

///// - VEC4F - /////

/* Note that for the VEC4* types, we do not need a second loop as the number
    of input items is always a multiple of four. */

#define NE10_DstSrcCst_MAINLOOP_VEC4F_NEON(loopCode) { \
     n_src = vld1q_f32( (float32_t*)src ); \
     src ++; \
     loopCode; \
     vst1q_f32 ( (float32_t*)dst , n_dst );  /* The main loop iterates through one 4D vector each time */ \
     dst ++; \
   }

#define NE10_DstSrcCst_OPERATION_VEC4F_NEON(loopCode) { \
   ne10_result_t res = NE10_OK; \
   float32x4_t n_cst = { cst->x, cst->y, cst->z, cst->w }; \
   float32x4_t n_src; \
   float32x4_t n_dst; \
   for (; count != 0; count --) { \
     loopCode; \
    } \
   return res; \
  }

/****************************************************
 *                                                  *
 *  The "DstAccSrcCst" group of functions           *
 *                                                  *
 ****************************************************/

///// - FLOAT - /////

#define NE10_DstAccSrcCst_MAINLOOP_FLOAT_NEON(loopCode) { \
     /* load 4 values  */ \
     n_acc = vld1q_f32( (float32_t*)acc ); \
     n_src = vld1q_f32( (float32_t*)src ); \
     acc += 4; /* move to the next 4 float items; 4*float */ \
     src += 4; \
     loopCode; /* the actual operation is placed here... */ /* The main loop iterates through four float values each time */ \
     vst1q_f32 ( (float32_t*)dst , n_dst ); /* store theresults back */ \
     dst += 4; /* move to the next items; 4*float */ \
    }

#define NE10_DstAccSrcCst_SECONDLOOP_FLOAT_NEON(loopCode) { \
      float32x2_t n_rest_acc = { 0.0f , 0.0f }; /* temporary storage to be used with NEON load/store intrinsics */ \
      float32x2_t n_rest = { 0.0f , 0.0f }; /* temporary storage to be used with NEON load/store intrinsics */ \
      float32x2_t n_rest_cst = { cst, cst }; /* temporary constant value for use in the main NEON operation */ \
      n_rest_acc = vld1_lane_f32 ( (float32_t*)acc, n_rest_acc, 0); /* load into the first lane of d0 */ \
      n_rest = vld1_lane_f32 ( (float32_t*)src, n_rest, 0); /* load into the first lane of d1 */ \
      loopCode; /* the actual operation is palced here ... */ /* exceptional cases where the count is not a multiple of 4 */ \
      vst1_lane_f32( (float32_t*)dst, n_rest, 0); /* store the lane back into the memory */ \
      /* move to the next item in the stream */ \
      acc++; \
      src++; \
      dst++; \
     }

#define NE10_DstAccSrcCst_OPERATION_FLOAT_NEON    NE10_DstSrcCst_OPERATION_FLOAT_NEON

///// - VEC2F - /////

#define NE10_DstAccSrcCst_MAINLOOP_VEC2F_NEON(loopCode) { \
     n_acc = vld1q_f32( (float32_t*)acc ); /* load two vectors */ \
     n_src = vld1q_f32( (float32_t*)src ); /* load two vectors */ \
     acc += 2; /* move to the next two vectors */ \
     src += 2; \
     loopCode; /* actual operation */ /* The main loop iterates through two 2D vectors each time */ \
     vst1q_f32 ( (float32_t*)dst , n_dst ); /* store back */ \
     dst += 2; /* move to the next 2 vectors */ \
    }

#define NE10_DstAccSrcCst_SECONDLOOP_VEC2F_NEON(loopCode) { \
     float32x2_t n_rest_acc; \
     float32x2_t n_rest; \
     float32x2_t n_rest_cst = { cst->x, cst->y }; \
     n_rest_acc = vld1_f32( (float32_t*)acc  ); \
     n_rest = vld1_f32( (float32_t*)src  ); \
     loopCode; /* exceptional cases where the count isn't a multiple of 2 */ \
     vst1_f32( (float32_t*)dst, n_rest); \
    }

#define NE10_DstAccSrcCst_OPERATION_VEC2F_NEON    NE10_DstSrcCst_OPERATION_VEC2F_NEON

///// - VEC3F - /////

#define NE10_DstAccSrcCst_MAINLOOP_VEC3F_NEON(loopCode) { \
     n_acc1 = vld1q_f32( (float32_t*)acc ); /* Load accumulator values */ \
     acc = ((void*)acc)+(4*sizeof(ne10_float32_t)); \
     n_acc2 = vld1q_f32( (float32_t*)acc ); \
     acc = ((void*)acc)+(4*sizeof(ne10_float32_t)); \
     n_acc3 = vld1q_f32( (float32_t*)acc ); \
     acc = ((void*)acc)+(4*sizeof(ne10_float32_t)); \
     n_src1 = vld1q_f32( (float32_t*)src ); /* Load source values */ \
     src = ((void*)src)+(4*sizeof(ne10_float32_t)); \
     n_src2 = vld1q_f32( (float32_t*)src ); \
     src = ((void*)src)+(4*sizeof(ne10_float32_t)); \
     n_src3 = vld1q_f32( (float32_t*)src ); \
     src = ((void*)src)+(4*sizeof(ne10_float32_t)); \
     loopCode; /* The main loop iterates through three 3D vectors each time */ \
     vst1q_f32 ( (float32_t*)dst , n_dst1 ); /* Store the results back into the memory */ \
     dst = ((void*)dst)+(4*sizeof(ne10_float32_t)); \
     vst1q_f32 ( (float32_t*)dst , n_dst2 ); \
     dst = ((void*)dst)+(4*sizeof(ne10_float32_t)); \
     vst1q_f32 ( (float32_t*)dst , n_dst3 ); \
     dst = ((void*)dst)+(4*sizeof(ne10_float32_t)); \
  }

#define NE10_DstAccSrcCst_SECONDLOOP_VEC3F_NEON(loopCode) { \
      float32x2x3_t n_rest_acc = FLOAT32_2x3( \
         0.0f, 0.0f, \
         0.0f, 0.0f, \
         0.0f, 0.0f  \
      ); \
      float32x2x3_t n_rest = FLOAT32_2x3( \
        0.0f, 0.0f, \
        0.0f, 0.0f, \
        0.0f, 0.0f  \
      ); \
      float32x2x3_t n_rest_cst = { (const float32x2_t){cst->x, 0}, \
                                  (const float32x2_t){cst->y, 0}, \
                                  (const float32x2_t){cst->z, 0} };     \
      n_rest_acc = vld3_lane_f32 ( (float32_t*)acc, n_rest_acc, 0);       \
      n_rest = vld3_lane_f32 ( (float32_t*)src, n_rest, 0);       \
      loopCode; /* exceptional cases where the count isn't a multiple of 3 */ \
      vst3_lane_f32( (float32_t*)dst, n_rest, 0); \
      acc++; \
      src++; \
      dst++; \
  }

#define NE10_DstAccSrcCst_OPERATION_VEC3F_NEON    NE10_DstSrcCst_OPERATION_VEC3F_NEON

///// - VEC4F - /////

#define NE10_DstAccSrcCst_MAINLOOP_VEC4F_NEON(loopCode) { \
     n_acc = vld1q_f32( (float32_t*)acc ); \
     n_src = vld1q_f32( (float32_t*)src ); \
     acc ++; \
     src ++; \
     loopCode; \
     vst1q_f32 ( (float32_t*)dst , n_dst );  /* The main loop iterates through one 4D vector each time */ \
     dst ++; \
  }

#define NE10_DstAccSrcCst_OPERATION_VEC4F_NEON    NE10_DstSrcCst_OPERATION_VEC4F_NEON

/****************************************************
 *                                                  *
 *  The "DstCst" group of functions                 *
 *                                                  *
 ****************************************************/

///// - FLOAT - /////

#define NE10_DstCst_MAINLOOP_FLOAT_NEON(loopCode) { \
     /* load 4 values  */ \
     loopCode; /* the actual operation is placed here... */ /* The main loop iterates through four float values each time */ \
     vst1q_f32 ( (float32_t*)dst , n_cst ); /* store theresults back */ \
     dst += 4; /* move to the next items; 4*float */ \
    }

#define NE10_DstCst_SECONDLOOP_FLOAT_NEON(loopCode) { \
      float32x2_t n_rest_cst = { cst, cst }; /* temporary constant value for use in the main NEON operation */ \
      loopCode; /* the actual operation is palced here ... */ /* exceptional cases where the count is not a multiple of 4 */ \
      vst1_lane_f32( (float32_t*)dst, n_rest_cst, 0); /* store the lane back into the memory */ \
      /* move to the next item in the stream */ \
      dst++; \
     }

#define NE10_DstCst_OPERATION_FLOAT_NEON(loopCode1, loopCode2) { \
   ne10_result_t res = NE10_OK; \
   int dif = 0; \
   dif = count % 4; /* either 0 or one of 1,2,3; in the latter cases the second path is taken */ \
   for (; count > dif; count -= 4) { \
     loopCode1; \
    } \
   if ( 0 != dif ) { \
    unsigned int idx; \
    for ( idx = 0 ; idx < dif; idx++ ) { \
      loopCode2; \
     } \
    } \
   return res; \
  }

///// - VEC2F - /////


#define NE10_DstCst_MAINLOOP_VEC2F_NEON(loopCode) { \
     loopCode; /* actual operation */ /* The main loop iterates through two 2D vectors each time */ \
     vst1q_f32 ( (float32_t*)dst , n_cst ); /* store back */ \
     dst += 2; /* move to the next 2 vectors */ \
    }

#define NE10_DstCst_SECONDLOOP_VEC2F_NEON(loopCode) { \
     float32x2_t n_rest_cst = { cst->x, cst->y }; \
     loopCode; /* exceptional cases where the count isn't a multiple of 2 */ \
     vst1_f32( (float32_t*)dst, n_rest_cst); \
    }

#define NE10_DstCst_OPERATION_VEC2F_NEON(loopCode1, loopCode2) { \
   ne10_result_t res = NE10_OK; \
   float32x4_t n_cst = { cst->x, cst->y, cst->x, cst->y }; \
   int dif = count % 2; \
   for (; count > dif; count -= 2) { \
    loopCode1; \
   } \
   if ( 0 != dif ) { \
    loopCode2; \
   } \
   return res; \
  }

///// - VEC3F - /////

#define NE10_DstCst_MAINLOOP_VEC3F_NEON(loopCode) { \
     loopCode; /* The main loop iterates through three 3D vectors each time */ \
     vst1q_f32 ( (float32_t*)dst , n_cst1 ); \
     dst = ((void*)dst)+(4*sizeof(ne10_float32_t)); \
     vst1q_f32 ( (float32_t*)dst , n_cst2 ); \
     dst = ((void*)dst)+(4*sizeof(ne10_float32_t)); \
     vst1q_f32 ( (float32_t*)dst , n_cst3 ); \
     dst = ((void*)dst)+(4*sizeof(ne10_float32_t)); \
  }

#define NE10_DstCst_SECONDLOOP_VEC3F_NEON(loopCode) { \
      float32x2x3_t n_rest_cst = { (const float32x2_t){cst->x, 0}, \
      (const float32x2_t){cst->y, 0}, (const float32x2_t){cst->z, 0} }; \
      loopCode; /* exceptional cases where the count isn't a multiple of 3 */ \
      vst3_lane_f32( (float32_t*)dst, n_rest_cst, 0); \
      dst++; \
     }

#define NE10_DstCst_OPERATION_VEC3F_NEON(loopCode1, loopCode2) { \
   ne10_result_t res = NE10_OK; \
   float32x4_t n_cst1 = { cst->x, cst->y, cst->z, cst->x }; \
   float32x4_t n_cst2 = { cst->y, cst->z, cst->x, cst->y }; \
   float32x4_t n_cst3 = { cst->z, cst->x, cst->y, cst->z }; \
   int dif = count % 4;  \
   for (; count > dif; count -= 4) { \
    loopCode1; \
  } \
  if ( 0 != dif ) { \
    unsigned int idx; \
    for ( idx = 0 ; idx < dif; idx++ ) { \
      loopCode2; \
     } \
    } \
   return res; \
  }

///// - VEC4F - /////

#define NE10_DstCst_MAINLOOP_VEC4F_NEON(loopCode) { \
     loopCode; \
     vst1q_f32 ( (float32_t*)dst , n_cst );  /* The main loop iterates through one 4D vector each time */ \
     dst ++; \
   }

#define NE10_DstCst_OPERATION_VEC4F_NEON(loopCode) { \
   ne10_result_t res = NE10_OK; \
   float32x4_t n_cst = { cst->x, cst->y, cst->z, cst->w }; \
   for (; count != 0; count --) { \
     loopCode; \
    } \
   return res; \
  }

/****************************************************
 *                                                  *
 *  The "DstSrc1Src2" group of functions            *
 *                                                  *
 ****************************************************/

///// - FLOAT - /////

#define NE10_DstSrc1Src2_MAINLOOP_FLOAT_NEON(loopCode) { \
     /* load 4 values  */ \
     n_src = vld1q_f32( (float32_t*)src1 ); \
     src1 += 4; /* move to the next 4 float items; 4*float */ \
     n_src2 = vld1q_f32( (float32_t*)src2 ); \
     src2 += 4; /* move to the next 4 float items; 4*float */ \
     loopCode; /* the actual operation is placed here... */ /* The main loop iterates through four float values each time */ \
     vst1q_f32 ( (float32_t*)dst , n_dst ); /* store the results back */ \
     dst += 4; /* move to the next items; 4*float */ \
    }

#define NE10_DstSrc1Src2_SECONDLOOP_FLOAT_NEON(loopCode) { \
      float32x2_t n_rest = { 0.0f , 0.0f }; /* temporary storage to be used with NEON load/store intrinsics */ \
      float32x2_t n_rest2 = { 0.0f , 0.0f }; \
      n_rest = vld1_lane_f32 ( (float32_t*)src1, n_rest, 0); /* load into the first lane of d0 */ \
      n_rest2 = vld1_lane_f32 ( (float32_t*)src2, n_rest, 0); \
      loopCode; /* the actual operation is placed here ... */ /* exceptional cases where the count is not a multiple of 4 */ \
      vst1_lane_f32( (float32_t*)dst, n_rest, 0); /* store the lane back into the memory */ \
      /* move to the next item in the stream */ \
      src1++; \
      src2++; \
      dst++; \
     }

#define NE10_DstSrc1Src2_OPERATION_FLOAT_NEON NE10_DstSrcCst_OPERATION_FLOAT_NEON

/****************************************************
 *                                                  *
 *  The "DstAccSrc1Src2" group of functions         *
 *                                                  *
 ****************************************************/

///// - FLOAT - /////

#define NE10_DstAccSrc1Src2_MAINLOOP_FLOAT_NEON(loopCode) { \
     /* load 4 values  */ \
     n_acc = vld1q_f32( (float32_t*)acc ); \
     n_src = vld1q_f32( (float32_t*)src1 ); \
     n_src2 = vld1q_f32( (float32_t*)src2 ); \
     acc += 4; /* move to the next 4 float items; 4*float */ \
     src1 += 4; \
     src2 += 4; \
     loopCode; /* the actual operation is placed here... */ /* The main loop iterates through four float values each time */ \
     vst1q_f32 ( (float32_t*)dst , n_dst ); /* store theresults back */ \
     dst += 4; /* move to the next items; 4*float */ \
    }

#define NE10_DstAccSrc1Src2_SECONDLOOP_FLOAT_NEON(loopCode) { \
      float32x2_t n_rest_acc = { 0.0f , 0.0f }; /* temporary storage to be used with NEON load/store intrinsics */ \
      float32x2_t n_rest = { 0.0f , 0.0f }; \
      float32x2_t n_rest2 = { 0.0f, 0.0f }; \
      n_rest_acc = vld1_lane_f32 ( (float32_t*)acc, n_rest_acc, 0); /* load into the first lane of d0 */ \
      n_rest = vld1_lane_f32 ( (float32_t*)src1, n_rest, 0); /* load into the first lane of d1 */ \
      n_rest2 = vld1_lane_f32 ( (float32_t*)src2, n_rest2, 0); /* load into the first lane of d2 */ \
      loopCode; /* the actual operation is palced here ... */ /* exceptional cases where the count is not a multiple of 4 */ \
      vst1_lane_f32( (float32_t*)dst, n_rest, 0); /* store the lane back into the memory */ \
      /* move to the next item in the stream */ \
      acc++; \
      src1++; \
      src2++; \
      dst++; \
     }

#define NE10_DstAccSrc1Src2_OPERATION_FLOAT_NEON NE10_DstAccSrcCst_OPERATION_FLOAT_NEON
