//timer.c

#include "timer.h"

static virtual_timer_t timerv;
static int             rearm = 0;
static void (*func)(void);

static void timercb(void *arg)
{
    (void)arg;
    func();
    if(rearm) {
        //re arm timer
        chSysLockFromISR();
        chVTSetI(&timerv, MS2ST(500), timercb, NULL);
        chSysUnlockFromISR();
    }
}

void timer_init(void)
{
    chVTObjectInit(&timerv);
}

void timer_on(int delay, void (*f)(void), int loop)
{
    func = f;
    chVTSet(&timerv, MS2ST(delay), timercb, NULL);
    rearm = loop;
}

void timer_off(void)
{
    chVTReset(&timerv);
}