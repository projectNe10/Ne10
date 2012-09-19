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

#include <stdio.h>
#include <string.h>

#define CPUINFO_BUFFER_SIZE  (1024*4)

// This local variable indicates whether or not the running platform supports ARM NEON
ne10_result_t is_NEON_available = NE10_ERR;

ne10_result_t NE10_HasNEON()
{
    return is_NEON_available;
}

ne10_result_t NE10_init()
{
    FILE*   infofile = NULL;               // To open the file /proc/cpuinfo
    ne10_int8_t    cpuinfo[CPUINFO_BUFFER_SIZE];  // The buffer to read in the string
    ne10_uint32_t  bytes = 0;                     // Numbers of bytes read from the file
    ne10_int32_t     i = 0;                         // Temporary loop counter
    ne10_result_t status = NE10_ERR;

    memset (cpuinfo, 0, CPUINFO_BUFFER_SIZE);
    infofile = fopen ("/proc/cpuinfo", "r");
    bytes    = fread (cpuinfo, 1, sizeof (cpuinfo), infofile);
    fclose (infofile);

    if (0 == bytes || CPUINFO_BUFFER_SIZE == bytes)
    {
        fprintf (stderr, "ERROR: Couldn't read the file \"/proc/cpuinfo\". NE10_init() failed.\n");
        return NE10_ERR;
    }

    while ('\0' != cpuinfo[i]) cpuinfo[i++] = (ne10_int8_t) tolower (cpuinfo[i]);

    if (0 != strstr (cpuinfo, "neon"))
    {
        is_NEON_available = NE10_OK;
    }

#if defined (NE10_ENABLE_MATH)
    status = NE10_init_math (is_NEON_available);
    if (status != NE10_OK)
    {
        fprintf(stderr, "ERROR: init math failed\n");
        return NE10_ERR;
    }
#endif
    return NE10_OK;
}
