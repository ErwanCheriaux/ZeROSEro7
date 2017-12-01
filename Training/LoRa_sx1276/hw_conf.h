#ifndef _HW_CONF_H
#define _HW_CONF_H

/*
*   Used by SX1276 driver. Defines board relative values.
*/

// Found in  CMSIS :
// "cmsis_gcc.h" // CMSIS compiler cortex m4 specifics assembly because the nRF52 also has one.

#define __STATIC_INLINE static __inline
#define __CLZ __builtin_clz

#endif
