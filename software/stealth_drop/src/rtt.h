#ifndef RTT_H
#define RTT_H

#include "SEGGER_RTT.h"

#define rtt_init SEGGER_RTT_Init
#define rtt_write_string(a) SEGGER_RTT_WriteString(0, a)

/* Printf signature : int SEGGER_RTT_printf(unsigned BufferIndex, const char * sFormat, ...);
Put 0 as first argument for default channel 0 */
#define rtt_printf(f, ...)                      \
    do {                                        \
        SEGGER_RTT_printf(0, f, ##__VA_ARGS__); \
    } while(0)

#define rtt_write_buffer SEGGER_RTT_Write

#endif
