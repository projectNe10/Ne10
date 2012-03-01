/*
 *  Copyright 2011-12 ARM Limited
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/*
 * NE10 Library : source/NE10_div.neon.c
 */

#include "NE10.h"
#include "../headers/macros.h"

#include <assert.h>
#include <arm_neon.h>

 #include <stdio.h>
 #include <stdlib.h>

arm_result_t div_float_neon(arm_float_t * dst, arm_float_t * src1, arm_float_t * src2, unsigned int count)
{
    NE10_X_OPERATION_FLOAT_NEON
    (
      /* a single division operation */
      float32x4_t rec = vrecpeq_f32( n_src2 );
      rec = vmulq_f32(vrecpsq_f32(n_src2, rec), rec);
      rec = vmulq_f32(vrecpsq_f32(n_src2, rec), rec);
      n_dst = vmulq_f32( n_src , rec );
      ,
      /* a single division operation */
      float32x2_t rec = vrecpe_f32( n_tmp_src2 );
      rec = vmul_f32(vrecps_f32(n_tmp_src2, rec), rec);
      rec = vmul_f32(vrecps_f32(n_tmp_src2, rec), rec);
      n_tmp_src = vmul_f32( n_tmp_src, rec );
    );
}
