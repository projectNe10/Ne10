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
 * NE10 Library : inc/NE10_physics.h
 */


#include "NE10_types.h"

#ifndef NE10_PHYSICS_H
#define NE10_PHYSICS_H

#ifdef __cplusplus
extern "C" {
#endif
    ne10_result_t ne10_init_physics (ne10_int32_t is_NEON_available);

    /**
     * @ingroup COLLISION_DETECT
     * @brief Compute the AABB for a polygon.
     *
     * @param[out] *aabb               return axis aligned box
     * @param[in]  *vertices           a convex polygon
     * @param[in]  *xf                 the position and orientation of rigid
     * @param[in]  radius              the aligned bounding
     * @param[in]  vertex_count        vertices count of convex ploygen
     *
     * The function computes the AABB for a polygon. Points to @ref ne10_physics_compute_aabb_vec2f_c
     * or @ref ne10_physics_compute_aabb_vec2f_neon, the latter of which requires that vertex_count
     * is a multiple of 4.
     */
    extern void (*ne10_physics_compute_aabb_vec2f) (ne10_mat2x2f_t *aabb,
            ne10_vec2f_t *vertices,
            ne10_mat2x2f_t *xf,
            ne10_vec2f_t *radius,
            ne10_uint32_t vertex_count);
    extern void ne10_physics_compute_aabb_vec2f_c (ne10_mat2x2f_t *aabb,
            ne10_vec2f_t *vertices,
            ne10_mat2x2f_t *xf,
            ne10_vec2f_t *radius,
            ne10_uint32_t vertex_count);
#ifdef ENABLE_NE10_PHYSICS_COMPUTE_AABB_VEC2F_NEON
    extern void ne10_physics_compute_aabb_vec2f_neon (ne10_mat2x2f_t *aabb,
            ne10_vec2f_t *vertices,
            ne10_mat2x2f_t *xf,
            ne10_vec2f_t *radius,
            ne10_uint32_t vertex_count);
#endif // ENABLE_NE10_PHYSICS_COMPUTE_AABB_VEC2F_NEON

    /**
     * @ingroup COLLISION_DETECT
     * @brief Calculate relative velocity at contact.
     *
     * @param[out] *dv               return relative velocity
     * @param[in]  *v_wa             velocity and angular velocity of body a
     * @param[in]  *ra               distance vector from center of mass of body a to contact point
     * @param[in]  *v_wb             velocity and angular velocity of body b
     * @param[in]  *rb               distance vector from center of mass of body b to contact point
     * @param[in]  count             the number of items
     *
     * To improve performance, two items are processed in one loop.
     * Points to @ref ne10_physics_relative_v_vec2f_c or @ref ne10_physics_relative_v_vec2f_neon.
     */
    extern void (*ne10_physics_relative_v_vec2f) (ne10_vec2f_t *dv,
            ne10_vec3f_t *v_wa,
            ne10_vec2f_t *ra,
            ne10_vec3f_t *v_wb,
            ne10_vec2f_t *rb,
            ne10_uint32_t count);
    extern void ne10_physics_relative_v_vec2f_c (ne10_vec2f_t *dv,
            ne10_vec3f_t *v_wa,
            ne10_vec2f_t *ra,
            ne10_vec3f_t *v_wb,
            ne10_vec2f_t *rb,
            ne10_uint32_t count);
#ifdef ENABLE_NE10_PHYSICS_RELATIVE_V_VEC2F_NEON
    /**
     * @ingroup COLLISION_DETECT
     * Specific implementation of @ref ne10_physics_relative_v_vec2f using NEON SIMD capabilities.
     */
    extern void ne10_physics_relative_v_vec2f_neon (ne10_vec2f_t *dv,
            ne10_vec3f_t *v_wa,
            ne10_vec2f_t *ra,
            ne10_vec3f_t *v_wb,
            ne10_vec2f_t *rb,
            ne10_uint32_t count)
    asm ("ne10_physics_relative_v_vec2f_neon");
#endif // ENABLE_NE10_PHYSICS_RELATIVE_V_VEC2F_NEON

    /**
     * @ingroup COLLISION_DETECT
     * @brief Apply contact impulse.
     *
     * @param[in,out] *v_wa          return velocity and angular velocity of body a
     * @param[in,out] *v_wb          return velocity and angular velocity of body b
     * @param[in]  *ra               distance vector from center of mass of body a to contact point
     * @param[in]  *rb               distance vector from center of mass of body b to contact point
     * @param[in]  *ima              constant of body a
     * @param[in]  *imb              constant of body b
     * @param[in]  *p                constant
     * @param[in]  count             the number of items
     *
     * To improve performance, two items are processed in one loop.
     * Points to @ref ne10_physics_apply_impulse_vec2f_c or @ref ne10_physics_apply_impulse_vec2f_neon.
     */
    extern void (*ne10_physics_apply_impulse_vec2f) (ne10_vec3f_t *v_wa,
            ne10_vec3f_t *v_wb,
            ne10_vec2f_t *ra,
            ne10_vec2f_t *rb,
            ne10_vec2f_t *ima,
            ne10_vec2f_t *imb,
            ne10_vec2f_t *p,
            ne10_uint32_t count);
    extern void ne10_physics_apply_impulse_vec2f_c (ne10_vec3f_t *v_wa,
            ne10_vec3f_t *v_wb,
            ne10_vec2f_t *ra,
            ne10_vec2f_t *rb,
            ne10_vec2f_t *ima,
            ne10_vec2f_t *imb,
            ne10_vec2f_t *p,
            ne10_uint32_t count);
#ifdef ENABLE_NE10_PHYSICS_APPLY_IMPULSE_VEC2F_NEON
    /**
     * @ingroup COLLISION_DETECT
     * Specific implementation of @ref ne10_physics_apply_impulse_vec2f using NEON SIMD capabilities.
     */
    extern void ne10_physics_apply_impulse_vec2f_neon (ne10_vec3f_t *v_wa,
            ne10_vec3f_t *v_wb,
            ne10_vec2f_t *ra,
            ne10_vec2f_t *rb,
            ne10_vec2f_t *ima,
            ne10_vec2f_t *imb,
            ne10_vec2f_t *p,
            ne10_uint32_t count)
    asm ("ne10_physics_apply_impulse_vec2f_neon");
#endif // ENABLE_NE10_PHYSICS_APPLY_IMPULSE_VEC2F_NEON

#ifdef __cplusplus
}
#endif

#endif
