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
 * NE10 Library : physics/NE10_physics.neon.s
 */

#ifdef ENABLE_NE10_PHYSICS_COMPUTE_AABB_VEC2F_NEON
        .text
        .syntax   unified

        .align   4
        .global   ne10_physics_compute_aabb_vertex4_vec2f_neon
        .thumb
        .thumb_func

ne10_physics_compute_aabb_vertex4_vec2f_neon:
        /**
        *@
        *@ compute AABB for ploygon
        *@ vertex_count is the multiple of 4
        *@ to improve performance, 4 vertices are processed in one loop
        *@ when vertex_count < 4*n, the lacking of vertices should be filled with 0
        *@
        *@ void ne10_physics_compute_aabb_vertex4_vec2f_neon(ne10_mat2x2f_t *aabb,
        *@                 ne10_vec2f_t *vertices,
        *@                 ne10_mat2x2f_t *xf,
        *@                 ne10_vec2f_t *radius,
        *@                 ne10_uint32_t vertex_count);
        *@
        *@  r0: *aabb, return axis aligned box
        *@  r1: *vertices, a convex polygon
        *@  r2: *xf, the position and orientation of rigid
        *@  r3: *radius, the aligned bounding
        *@  sp: vertex_count, vertices count of convex ploygen
        */

        push              {r4, r5}
        ldr               r4, [sp, #8]        @ r4 = vertex_count


        vld1.f32          {d30}, [r3]  @load radius to d30
        vld1.f32          {d4, d5}, [r2]  @load xf to d4,d5
        vdup.f32          q0, d4[0]
        vdup.f32          q1, d4[1]

        @vertices[0~3]
        vld2.f32          {q4, q5}, [r1]!  @load vertices
        vmla.f32          q0, q4, d5[1]
        vmul.f32          q6, q5, d5[1]
        vmla.f32          q1, q4, d5[0]
        vmul.f32          q7, q5, d5[0]
        vsub.f32          q7, q0, q7
        vadd.f32          q6, q1, q6
        vswp.f32          d12, d15
        subs              r4, r4, #4

        vmin.f32          q8, q7, q6
        vpmin.f32         d24, d16, d17
        vmax.f32          q9, q7, q6
        vpmax.f32         d25, d18, d19

        ble               aabb_store_result

aabb_main_loop:
        @vertices
        vld2.f32          {q4, q5}, [r1]!  @load vertices
        vdup.f32          q0, d4[0]
        vdup.f32          q1, d4[1]
        vmla.f32          q0, q4, d5[1]
        vmul.f32          q6, q5, d5[1]
        vmla.f32          q1, q4, d5[0]
        vmul.f32          q7, q5, d5[0]
        vsub.f32          q7, q0, q7
        vadd.f32          q6, q1, q6
        vswp.f32          d12, d15

        vmin.f32          q8, q7, q6
        vpmin.f32         d26, d16, d17
        vmax.f32          q9, q7, q6
        vpmax.f32         d27, d18, d19
        subs              r4, r4, #4

        vmin.f32          d24, d24, d26
        vmax.f32          d25, d25, d27
        bgt               aabb_main_loop

aabb_store_result:
        vsub.f32          d24, d24, d30
        vadd.f32          d25, d25, d30
        vst1.f32          {d24, d25}, [r0]

aabb_end:
        @ return
        pop               {r4, r5}
        bx                lr
#endif // ENABLE_NE10_PHYSICS_COMPUTE_AABB_VEC2F_NEON

#ifdef ENABLE_NE10_PHYSICS_RELATIVE_V_VEC2F_NEON
        .align   4
        .global   ne10_physics_relative_v_vec2f_neon
        .thumb
        .thumb_func

ne10_physics_relative_v_vec2f_neon:
        /**
         *@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
         *@
         *@ calculate relative velocity at contact
         *@
         *@
         *@ ne10_result_t ne10_physics_relative_v_vec2f_neon(ne10_vec2f_t *dv,
         *@              ne10_vec3f_t *v_wa,
         *@              ne10_vec2f_t *ra,
         *@              ne10_vec3f_t *v_wb,
         *@              ne10_vec2f_t *rb,
         *@              ne10_uint32_t count)
         *@
         *@  r0: *dv, return relative velocity
         *@  r1: *v_wa, velocity and angular velocity of body a
         *@  r2: *ra, distance vector from center of mass of body a to contact point
         *@  r3: *v_wb, velocity and angular velocity of body b
         *@  sp: *rb, distance vector from center of mass of body b to contact point
         *@  sp+4: count, the number of items
         *@
         *@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        */

        push              {r4, r5, r6, r7}
        ldr               r4, [sp, #16]         @ r4 = *rb
        ldr               r5, [sp, #20]         @ r5 = count
        and               r6, r5, #1            @ r6 = count&1
        sub               r5, r5, r6

        cmp               r5, #0
        beq               check_relative_v_left


relative_v_main_loop:
        vld3.f32          {d0, d1, d2}, [r1]!  @load v_wa [va->x, va->y, wa]
        vld3.f32          {d4, d5, d6}, [r3]!  @load v_wb [vb->x, vb->y, wb]
        vld2.f32          {d7, d8}, [r2]!  @load ra
        vld2.f32          {d9, d10}, [r4]!  @load rb

        vmls.f32          d0, d2, d8
        vmla.f32          d1, d2, d7

        vmls.f32          d4, d6, d10
        vmla.f32          d5, d6, d9

        subs              r5, r5, #2
        vsub.f32          q10, q2, q0
        vst2.f32          {d20, d21}, [r0]!

        bgt               relative_v_main_loop

check_relative_v_left:
        cmp               r6, #0
        beq               relative_v_end

relative_v_left:
        vld3.f32          {d0[0], d1[0], d2[0]}, [r1]!  @load v_wa [va->x, va->y, wa]
        vld3.f32          {d4[0], d5[0], d6[0]}, [r3]!  @load v_wb [vb->x, vb->y, wb]
        vld1.f32          {d7}, [r2]!  @load ra
        vld1.f32          {d8}, [r4]!  @load rb

        vmls.f32          d0, d2, d7[1]
        vmla.f32          d1, d2, d7[0]

        vmls.f32          d4, d6, d8[1]
        vmla.f32          d5, d6, d8[0]

        vsub.f32          q10, q2, q0
        vst2.f32          {d20[0], d21[0]}, [r0]!

relative_v_end:
        @ return
        pop               {r4, r5, r6, r7}
        bx                lr
#endif // ENABLE_NE10_PHYSICS_RELATIVE_V_VEC2F_NEON

#ifdef ENABLE_NE10_PHYSICS_APPLY_IMPULSE_VEC2F_NEON
        .align   4
        .global   ne10_physics_apply_impulse_vec2f_neon
        .thumb
        .thumb_func

ne10_physics_apply_impulse_vec2f_neon:
        /**
         *@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
         *@
         *@ apply contact impulse
         *@
         *@ ne10_result_t ne10_physics_apply_impulse_vec2f_neon(ne10_vec3f_t *v_wa,
         *@              ne10_vec3f_t *v_wb,
         *@              ne10_vec2f_t *ra,
         *@              ne10_vec2f_t *rb,
         *@              ne10_vec2f_t *ima,
         *@              ne10_vec2f_t *imb,
         *@              ne10_vec2f_t *p,
         *@              ne10_uint32_t count)
         *@
         *@  r0: *v_wa, return velocity and angular velocity of body a
         *@  r1: *v_wb, return velocity and angular velocity of body b
         *@  r2: *ra, distance vector from center of mass of body a to contact point
         *@  r3: *rb, distance vector from center of mass of body b to contact point
         *@  sp: *ima, constant of body a
         *@  sp+4: *imb, constant of body b
         *@  sp+8: *p, constant
         *@  sp+12: count, the number of items
         *@
         *@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
         **/

        push              {r4, r5, r6, r7}
        ldr               r4, [sp, #16]         @ r4 = *ima
        ldr               r5, [sp, #20]         @ r5 = *imb
        ldr               r6, [sp, #24]         @ r6 = *p
        ldr               r7, [sp, #28]         @ r7 = count
        @push              {r8}

        and               r12, r7, #1            @ r12 = count&1
        sub               r7, r7, r12

        cmp               r7, #0
        beq               check_apply_impulse_left

apply_impulse_main_loop:
        vld2.f32          {d0, d1}, [r2]!  @load ra
        vld2.f32          {d2, d3}, [r3]!  @load rb
        vld2.f32          {d20, d21}, [r4]!  @load ima
        vld2.f32          {d22, d23}, [r5]!  @load imb
        vld2.f32          {d6, d7}, [r6]!  @load p
        vld3.f32          {d8, d9, d10}, [r0]  @load v_wa
        vld3.f32          {d12, d13, d14}, [r1]  @load v_wb

        vmls.f32          d8, d6, d20
        vmls.f32          d9, d7, d20

        vmul.f32          d16, d0, d7
        vmls.f32          d16, d1, d6
        vmls.f32          d10, d16, d21

        vmla.f32          d12, d6, d22
        vmla.f32          d13, d7, d22

        vmul.f32          d16, d2, d7
        vmls.f32          d16, d3, d6
        vmla.f32          d14, d16, d23

        subs              r7, r7, #2
        vst3.f32          {d8, d9, d10}, [r0]!
        vst3.f32          {d12, d13, d14}, [r1]!

        bgt               apply_impulse_main_loop

check_apply_impulse_left:
        cmp               r12, #0
        beq               apply_impulse_end

apply_impulse_left:
        vld2.f32          {d0[0], d1[0]}, [r2]!  @load ra
        vld2.f32          {d2[0], d3[0]}, [r3]!  @load rb
        vld1.f32          {d4}, [r4]!  @load ima
        vld1.f32          {d5}, [r5]!  @load imb
        vld2.f32          {d6[0], d7[0]}, [r6]!  @load p
        vld3.f32          {d8[0], d9[0], d10[0]}, [r0]  @load v_wa
        vld3.f32          {d12[0], d13[0], d14[0]}, [r1]  @load v_wb

        vmls.f32          d8, d6, d4[0]
        vmls.f32          d9, d7, d4[0]

        vmul.f32          d16, d0, d7
        vmls.f32          d16, d1, d6
        vmls.f32          d10, d16, d4[1]

        vmla.f32          d12, d6, d5[0]
        vmla.f32          d13, d7, d5[0]

        vmul.f32          d16, d2, d7
        vmls.f32          d16, d3, d6
        vmla.f32          d14, d16, d5[1]

        vst3.f32          {d8[0], d9[0], d10[0]}, [r0]!
        vst3.f32          {d12[0], d13[0], d14[0]}, [r1]!

apply_impulse_end:
        @ return
        @pop               {r8}
        pop               {r4, r5, r6, r7}
        bx                lr

#endif // ENABLE_NE10_PHYSICS_APPLY_IMPULSE_VEC2F_NEON
