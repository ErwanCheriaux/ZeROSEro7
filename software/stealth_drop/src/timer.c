//timer.c

#include "timer.h"

static virtual_timer_t timerv;
static virtual_timer_t timerled;
static int             rearm     = 0;
static int             led_state = 0;
static void (*func)(void);

static void timer_cb(void *arg)
{
    (void)arg;
    func();
    if(rearm) {
        //re arm timer
        chSysLockFromISR();
        chVTSetI(&timerv, MS2ST(500), timer_cb, NULL);
        chSysUnlockFromISR();
    }
}

void timer_init(void)
{
    chVTObjectInit(&timerv);
    chVTObjectInit(&timerled);
}

void timer_on(int delay, void (*f)(void), int loop)
{
    func = f;
    chVTSet(&timerv, MS2ST(delay), timer_cb, NULL);
    rearm = loop;
}

void timer_off(void)
{
    chVTReset(&timerv);
}

static void timerled_cb(void *arg)
{
    (void)arg;
    pwm_b(led_state);
    led_state = 1 - led_state;
    //re arm timer
    chSysLockFromISR();
    chVTSetI(&timerled, MS2ST(1000), timerled_cb, NULL);
    chSysUnlockFromISR();
}

void timer_led(void)
{
    chVTSet(&timerled, MS2ST(1000), timerled_cb, NULL);
}