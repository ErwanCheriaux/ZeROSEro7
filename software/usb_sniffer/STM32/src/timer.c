//timer.c

#include "timer.h"

static virtual_timer_t timerv;
static bool            run = false;

static void timercb(void *arg)
{
    (void)arg;
    led_toggle();

    //re arm timer
    if(run) {
        chSysLockFromISR();
        chVTSetI(&timerv, MS2ST(500), timercb, NULL);
        chSysUnlockFromISR();
    }
}

void timer_init(void)
{
    chVTObjectInit(&timerv);
}

void timer_on(void)
{
    chVTSet(&timerv, MS2ST(500), timercb, NULL);
    run = true;
}

void timer_off(void)
{
    run = false;
}
