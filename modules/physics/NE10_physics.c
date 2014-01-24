/*
 *  Copyright 2014 ARM Limited
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
 * NE10 Library : physics/NE10_physics.c
 */

#include "NE10_types.h"

/**
 * @ingroup groupPhysics
 */
/**
 * @defgroup COLLISION_DETECT Collision Detection
 *
 * \par
 * Collision detection typically refers to the computational problem of detecting the intersection of two or more objects.
 * \par
 * This set of functions are used for collision detection algorithm for 32-bit float data types. Currently compute AABB,
 * caculate relative velocity and apply contact impulse were implemented.
 *
 */
static inline ne10_vec2f_t ne10_mul_matvec_float (ne10_mat2x2f_t T, ne10_vec2f_t v)
{
    ne10_vec2f_t tmp;
    ne10_float32_t x = (T.c2.r2 * v.x - T.c2.r1 * v.y) + T.c1.r1;
    ne10_float32_t y = (T.c2.r1 * v.x + T.c2.r2 * v.y) + T.c1.r2;
    tmp.x = x;
    tmp.y = y;
    return tmp;
}

static inline ne10_float32_t min (float a, ne10_float32_t b)
{
    return a < b ? a : b;
}

static inline ne10_vec2f_t min_2f (ne10_vec2f_t a, ne10_vec2f_t b)
{
    ne10_vec2f_t tmp = {min (a.x, b.x), min (a.y, b.y) };
    return tmp;
}

static inline ne10_float32_t max (float a, ne10_float32_t b)
{
    return a > b ? a : b;
}

static inline ne10_vec2f_t max_2f (ne10_vec2f_t a, ne10_vec2f_t b)
{
    ne10_vec2f_t tmp = {max (a.x, b.x), max (a.y, b.y) };
    return tmp;
}

/**
 * @addtogroup COLLISION_DETECT
 * @{
 */

/**
 * @brief compute AABB for ploygon.
 * @param[out] *aabb               return axis aligned box
 * @param[in]  *vertices           a convex polygon
 * @param[in]  *xf                 the position and orientation of rigid
 * @param[in]  radius              the aligned bounding
 * @param[in]  vertex_count        vertices count of convex ploygen
 * @return none.
 * The function is to compute AABB for ploygon.
 * vertex_count > 0.
 */
void ne10_physics_compute_aabb_vec2f_c (ne10_mat2x2f_t *aabb,
                                        ne10_vec2f_t *vertices,
                                        ne10_mat2x2f_t *xf,
                                        ne10_vec2f_t *radius,
                                        ne10_uint32_t vertex_count)
{
    ne10_vec2f_t lower = ne10_mul_matvec_float (*xf, vertices[0]);
    ne10_vec2f_t upper = lower;
    ne10_vec2f_t v;
    ne10_int32_t i;

    for (i = 1; i < vertex_count; ++i)
    {
        v = ne10_mul_matvec_float (*xf, vertices[i]);
        lower = min_2f (lower, v);
        upper = max_2f (upper, v);
    }

    aabb->c1.r1 = lower.x - radius->x;
    aabb->c1.r2 = lower.y - radius->y;
    aabb->c2.r1 = upper.x + radius->x;
    aabb->c2.r2 = upper.y + radius->y;

}

/**
 * @brief calculate relative velocity at contact.
 * @param[out] *dv               return relative velocity
 * @param[in]  *v_wa             velocity and angular velocity of body a
 * @param[in]  *ra               distance vector from center of mass of body a to contact point
 * @param[in]  *v_wb             velocity and angular velocity of body b
 * @param[in]  *rb               distance vector from center of mass of body b to contact point
 * @param[in]  count             the number of items
 * @return none.
 *
 *  To improve performance, 2 items are processed in one loop
 */
void ne10_physics_relative_v_vec2f_c (ne10_vec2f_t *dv,
                                      ne10_vec3f_t *v_wa,
                                      ne10_vec2f_t *ra,
                                      ne10_vec3f_t *v_wb,
                                      ne10_vec2f_t *rb,
                                      ne10_uint32_t count)
{
    ne10_int32_t i;
    ne10_vec2f_t va;
    ne10_vec2f_t vb;

    for (i = 0; i < count; i++)
    {
        va.x = v_wa->x - v_wa->z * ra->y;
        va.y = v_wa->y + v_wa->z * ra->x;
        vb.x = v_wb->x - v_wb->z * rb->y;
        vb.y = v_wb->y + v_wb->z * rb->x;

        dv->x = vb.x - va.x;
        dv->y = vb.y - va.y;

        v_wa++;
        v_wb++;
        ra++;
        rb++;
        dv++;

    }
}

/**
 * @brief apply contact impulse.
 * @param[in/out] *v_wa          return velocity and angular velocity of body a
 * @param[in/out] *v_wb          return velocity and angular velocity of body b
 * @param[in]  *ra               distance vector from center of mass of body a to contact point
 * @param[in]  *rb               distance vector from center of mass of body b to contact point
 * @param[in]  *ima              constant of body a
 * @param[in]  *imb              constant of body b
 * @param[in]  *p                constant
 * @param[in]  count             the number of items
 * @return none.
 *
 *  To improve performance, 2 items are processed in one loop
 */
void ne10_physics_apply_impulse_vec2f_c (ne10_vec3f_t *v_wa,
        ne10_vec3f_t *v_wb,
        ne10_vec2f_t *ra,
        ne10_vec2f_t *rb,
        ne10_vec2f_t *ima,
        ne10_vec2f_t *imb,
        ne10_vec2f_t *p,
        ne10_uint32_t count)
{
    ne10_int32_t i;
    ne10_vec2f_t va;
    ne10_vec2f_t vb;

    for (i = 0; i < count; i++)
    {
        v_wa->x -= ima->x * p->x;
        v_wa->y -= ima->x * p->y;
        v_wa->z -= ima->y * (ra->x * p->y - ra->y * p->x);

        v_wb->x += imb->x * p->x;
        v_wb->y += imb->x * p->y;
        v_wb->z += imb->y * (rb->x * p->y - rb->y * p->x);

        v_wa++;
        v_wb++;
        ra++;
        rb++;
        ima++;
        imb++;
        p++;
    }
}
/**
 * @} end of COLLISION_DETECT group
 */
