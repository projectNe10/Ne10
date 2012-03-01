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
 * NE10 Library : inc/NE10_types.h
 */

/** NE10 defines a number of types for use in its function signatures.
 *  The types are defined within this header file.
 */

#ifndef NE10_TYPES_H
#define NE10_TYPES_H

/////////////////////////////////////////////////////////
// constant values that are used across the library
/////////////////////////////////////////////////////////
#define NE10_OK 0
#define NE10_ERR -1

/////////////////////////////////////////////////////////
// some external definitions to be exposed to the users
/////////////////////////////////////////////////////////
typedef float arm_float_t;      // a single float value
typedef int   arm_result_t;     // resulting [error-]code

typedef struct
{
        float x;
        float y;
} arm_vec2f_t; // a 2-tuple of float values

typedef struct
{
        float x;
        float y;
        float z;
} arm_vec3f_t; // a 3-tuple of float values

typedef struct
{
        float x;
        float y;
        float z;
        float w;
} arm_vec4f_t; // a 4-tuple of float values


typedef struct { float r1; float r2; } arm_mat_row2f;

typedef struct
{
        arm_mat_row2f c1;
        arm_mat_row2f c2;

} arm_mat2x2f_t;     // a 2x2 matrix


typedef struct { float r1; float r2; float r3; }  arm_mat_row3f;

typedef struct
{
        arm_mat_row3f c1;
        arm_mat_row3f c2;
        arm_mat_row3f c3;

} arm_mat3x3f_t;     // a 3x3 matrix


typedef struct { float r1; float r2; float r3; float r4; } arm_mat_row4f;

typedef struct
{
        arm_mat_row4f c1;
        arm_mat_row4f c2;
        arm_mat_row4f c3;
        arm_mat_row4f c4;

} arm_mat4x4f_t;     // a 4x4 matrix

#endif
