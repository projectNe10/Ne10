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

#include "NE10.h"
#include "NE10_init.h"

// This test code shows you how you can statically embed NE10 in your code

void main()
{
    ne10_result_t status;
    printf ("Going to initialze NE10...\n");

    status = NE10_init();
    if (status != NE10_OK)
        printf ("NE10 init failed.\n");

    printf ("NE10 has been initialized.\n");
}

