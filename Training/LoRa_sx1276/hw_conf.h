#ifndef HW_CONF_H
#define HW_CONF_H

/*
*   Used by SX1276 driver. Defines board relative values.
*/

#define RADIO_NSS_PORT  UNIMPLEMENTED // TODO



// Found in  CMSIS :
// "cmsis_gcc.h" // CMSIS compiler cortex m4 specifics assembly because the nRF52 also has one.

#define __STATIC_INLINE static __inline
#define __CLZ __builtin_clz // Count zeroes on the left

#endif
