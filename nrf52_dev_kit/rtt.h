#ifndef RTT_H
#define RTT_H

#include "SEGGER_RTT.h"

#define rtt_init SEGGER_RTT_Init
#define rtt_write_string(a) SEGGER_RTT_WriteString(0,a)
#define rtt_printf SEGGER_RTT_printf
// Printf signature : int SEGGER_RTT_printf(unsigned BufferIndex, const char * sFormat, ...);
// Put 0 as first argument for default channel 0

#endif
