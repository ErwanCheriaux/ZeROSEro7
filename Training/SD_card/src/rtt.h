//rtt.h

#ifndef RTT_H
#define RTT_H

#include "ch.h"
#include "hal.h"

#include "SEGGER_RTT.h"

extern BaseSequentialStream rtt_shell;

#define rtt_printf(f, ...)                      \
    do {                                        \
        SEGGER_RTT_printf(0, f, ##__VA_ARGS__); \
    } while(0)

#endif
