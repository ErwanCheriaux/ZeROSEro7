//rtt.c

#include "rtt.h"

/* Channel put method, blocking.*/
static msg_t put(void *instance, uint8_t b)
{
    return SEGGER_RTT_PutChar((unsigned)instance, b);
}

/* Stream write buffer method.*/
static size_t write(void *instance, const uint8_t *bp, size_t n)
{
    return SEGGER_RTT_Write((unsigned)instance, bp, n);
}

/* Channel get method, blocking.*/
static msg_t get(void *instance)
{
    (void)instance;
    int r;

    do {
        r = SEGGER_RTT_GetKey();
        chThdSleepMilliseconds(10);
    } while(r < 0);
    return r;
}

/* Stream read buffer method.*/
static size_t read(void *instance, uint8_t *bp, size_t n)
{
    for(unsigned int i=0; i<n; i++)
        bp[i] = get(instance);

    return n;
}

static const struct BaseSequentialStreamVMT rtt_bss_vmt = {
    write,
    read,
    put,
    get
};

BaseSequentialStream rtt_bss = {
   &rtt_bss_vmt
};
