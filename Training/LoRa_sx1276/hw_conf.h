#ifndef HW_CONF_H
#define HW_CONF_H

/*
*   Used by SX1276 driver. Defines board relative compiler Macros.
*/

#define __ASM            __asm

#ifndef __STATIC_INLINE
#define __STATIC_INLINE static __inline
#endif

// The one from Nordic devkit (which is the same as for icube)
#include "cmsis_gcc.h"


#endif
