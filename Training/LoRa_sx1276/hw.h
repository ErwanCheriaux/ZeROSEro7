#ifndef _HW_H__
#define _HW_H__

/*
*   File included by sx1276 driver
*/

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h> // defines NULL symbol
#include <math.h>
#include "hw_conf.h"
#include "hw_gpio.h"

// Only used to print tX/rXDone. RTT can't be include because it redefines same symbols
inline void PRINTF(char * c) {}

#endif
