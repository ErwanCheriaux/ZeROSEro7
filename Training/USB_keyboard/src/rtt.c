//rtt.c

#include "rtt.h"

void rtt_printf(const char *f, ...)
{
    SEGGER_RTT_printf(0, f);
}
