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
 * NE10 Library : source/NE10_div_test.c
 */

//Make sure the following are defined before including "unit_test.h"

// length of the data arrays
#define ARRLEN TEST_ARRLEN
// number of the operations in a given unit
#define OP_COUNT 1
// number of the different implementations of each of the functions (C, ASM, NEON, ...)
#define IMPL_COUNT 3


#include "../headers/unit_test_x_operation_x.h"

void init_ftbl()
{
   // manually initialize the global function table with
   //  those functions that do have an actual implementation.
   ftbl[ 0] = (arm_func_4args_t) div_float_c;
   ftbl[ 1] = (arm_func_4args_t) div_float_asm;
   ftbl[ 2] = (arm_func_4args_t) div_float_neon;
}

arm_result_t main( int argc, char **argv )
{
   return run_test( argc, argv ); // defined in "unit_test.h"
}
