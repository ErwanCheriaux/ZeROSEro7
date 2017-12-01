#ifndef HW_H
#define HW_H

/*
*   File included by sx1276 driver
*/

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h> // defines NULL symbol
#include <math.h>
#include "hw_conf.h"
#include "hw_gpio.h"
#include "hw_spi.h"

// Only used to print tX/rXDone. RTT can't be include because it redefines same symbols
inline void PRINTF(char * c) {}

#endif
