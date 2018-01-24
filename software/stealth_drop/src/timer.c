//timer.c

#include "timer.h"

static virtual_timer_t timerv;
static bool            run = false;
static void(*          func)(void);

static void timercb(void *arg)
{
    (void)arg;
    //re arm timer
    if(run) {
        func();
        chSysLockFromISR();
        chVTSetI(&timerv, MS2ST(500), timercb, NULL);
        chSysUnlockFromISR();
    }
}

void timer_init(void)
{
    chVTObjectInit(&timerv);
}

void timer_on(int delay, void (*f)(void))
{
    func = f;
    chVTSet(&timerv, MS2ST(delay), timercb, NULL);
    run = true;
}

void timer_off(void)
{
    run = false;
}