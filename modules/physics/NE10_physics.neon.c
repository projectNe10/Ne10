/*
 *  Copyright 2014-15 ARM Limited and Contributors.
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

#include "NE10_types.h"

#ifdef ENABLE_NE10_PHYSICS_COMPUTE_AABB_VEC2F_NEON
extern void ne10_physics_compute_aabb_vertex4_vec2f_neon (ne10_mat2x2f_t *aabb,
        ne10_vec2f_t *vertices,
        ne10_mat2x2f_t *xf,
        ne10_vec2f_t *radius,
        ne10_uint32_t vertex_count)
asm ("ne10_physics_compute_aabb_vertex4_vec2f_neon");
#endif // ENABLE_NE10_PHYSICS_COMPUTE_AABB_VEC2F_NEON

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
 * @brief compute AABB for ploygon.
 * @param[out] *aabb               return axis aligned box
 * @param[in]  *vertices           a convex polygon
 * @param[in]  *xf                 the position and orientation of rigid
 * @param[in]  radius              the aligned bounding
 * @param[in]  vertex_count        vertices count of convex ploygen
 * @return none.
 * The function is to compute AABB for ploygon.
 * vertex_count is the multiple of 4. To improve performance, 4 vertices are processed in one loop
 */
#ifdef ENABLE_NE10_PHYSICS_COMPUTE_AABB_VEC2F_NEON
void ne10_physics_compute_aabb_vec2f_neon (ne10_mat2x2f_t *aabb,
        ne10_vec2f_t *vertices,
        ne10_mat2x2f_t *xf,
        ne10_vec2f_t *radius,
        ne10_uint32_t vertex_count)
{
    ne10_int32_t residual_loops = (vertex_count & 0x3);
    ne10_int32_t main_loops = vertex_count - residual_loops;

    if (main_loops > 0)
    {
        ne10_physics_compute_aabb_vertex4_vec2f_neon (aabb, vertices, xf, radius, main_loops);
    }

    if (residual_loops > 0)
    {
        ne10_vec2f_t lower;
        ne10_vec2f_t upper;
        ne10_vec2f_t lower2;
        ne10_vec2f_t upper2;
        ne10_vec2f_t v;
        ne10_int32_t i;

       if (main_loops == 0)
       {
            lower = ne10_mul_matvec_float (*xf, vertices[main_loops]);
            upper = lower;
       }
       else
       {
            lower2.x = aabb->c1.r1 + radius->x;
            lower2.y = aabb->c1.r2 + radius->y;
            upper2.x = aabb->c2.r1 - radius->x;
            upper2.y = aabb->c2.r2 - radius->y;
            lower = ne10_mul_matvec_float (*xf, vertices[main_loops]);
            upper = lower;
            lower = min_2f (lower, lower2);
            upper = max_2f (upper, upper2);
       }

        for (i = main_loops + 1; i < vertex_count; ++i)
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
}
#endif // ENABLE_NE10_PHYSICS_COMPUTE_AABB_VEC2F_NEON
