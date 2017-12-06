//main.c

#include "ch.h"
#include "hal.h"

#include "fibo.h"

#include "led.h"
#include "pwm.h"
#include "rtt.h"
#include "timer.h"
#include "SEGGER_RTT.h"
#include "SEGGER_RTT_Conf.h"

int main(void)
{
    halInit();
    chSysInit();

    led_init();
    pwm_init();
    rtt_init();
    timer_init();

    timer_on();
    chThdSleep(TIME_INFINITE);
    return 0;
}
