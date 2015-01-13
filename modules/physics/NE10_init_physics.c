/*
 *  Copyright 2014 ARM Limited and Contributors.
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
 * NE10 Library : physics/NE10_physics.c
 */

#include <stdio.h>

#include "NE10_physics.h"

ne10_result_t ne10_init_physics (ne10_int32_t is_NEON_available)
{
    if (NE10_OK == is_NEON_available)
    {
#ifdef ENABLE_NE10_PHYSICS_COMPUTE_AABB_VEC2F_NEON
        ne10_physics_compute_aabb_vec2f = ne10_physics_compute_aabb_vec2f_neon;
#else
        ne10_physics_compute_aabb_vec2f = ne10_physics_compute_aabb_vec2f_c;
#endif // ENABLE_NE10_PHYSICS_COMPUTE_AABB_VEC2F_NEON

#ifdef ENABLE_NE10_PHYSICS_RELATIVE_V_VEC2F_NEON
        ne10_physics_relative_v_vec2f = ne10_physics_relative_v_vec2f_neon;
#else
        ne10_physics_relative_v_vec2f = ne10_physics_relative_v_vec2f_c;
#endif // ENABLE_NE10_PHYSICS_RELATIVE_V_VEC2F_NEON

#ifdef ENABLE_NE10_PHYSICS_APPLY_IMPULSE_VEC2F_NEON
        ne10_physics_apply_impulse_vec2f = ne10_physics_apply_impulse_vec2f_neon;
#else
        ne10_physics_apply_impulse_vec2f = ne10_physics_apply_impulse_vec2f_c;
#endif // ENABLE_NE10_PHYSICS_RELATIVE_V_VEC2F_NEON
    }
    else
    {
        ne10_physics_compute_aabb_vec2f = ne10_physics_compute_aabb_vec2f_c;
        ne10_physics_relative_v_vec2f = ne10_physics_relative_v_vec2f_c;
        ne10_physics_apply_impulse_vec2f = ne10_physics_apply_impulse_vec2f_c;
    }
    return NE10_OK;
}

// These are actual definitions of our function pointers that are declared in inc/NE10_physics.h
void (*ne10_physics_compute_aabb_vec2f) (ne10_mat2x2f_t *aabb,
        ne10_vec2f_t *vertices,
        ne10_mat2x2f_t *xf,
        ne10_vec2f_t *radius,
        ne10_uint32_t vertex_count);
void (*ne10_physics_relative_v_vec2f) (ne10_vec2f_t *dv,
                                       ne10_vec3f_t *v_wa,
                                       ne10_vec2f_t *ra,
                                       ne10_vec3f_t *v_wb,
                                       ne10_vec2f_t *rb,
                                       ne10_uint32_t count);
void (*ne10_physics_apply_impulse_vec2f) (ne10_vec3f_t *v_wa,
        ne10_vec3f_t *v_wb,
        ne10_vec2f_t *ra,
        ne10_vec2f_t *rb,
        ne10_vec2f_t *ima,
        ne10_vec2f_t *imb,
        ne10_vec2f_t *p,
        ne10_uint32_t count);
