// COPYRIGHT NOTICE TBD NOT FOR RELEASE

#include <NE10.h>

#ifndef NE10_init_H
#define NE10_init_H

/*!
    This routine returns NE10_OK if the running platform supports NEON, otherwise it returns NE10_ERR
 */
extern arm_result_t NE10_HasNEON();

/*!
    This routine initializes all the function pointers defined in "NE10.h" with pointers to ARM NEON or ARM VFP implementations.
 */
extern arm_result_t NE10_init();

#endif
