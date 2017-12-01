//main.c

#include "ch.h"
#include "hal.h"

#include "led.h"
#include "pwm.h"
#include "timer.h"
#include "uart.h"
#include "rtt.h"

int main(void)
{
    halInit();
    chSysInit();

    led_init();
    pwm_init();
    timer_init();
    uart_init();
    rtt_init();

    rtt_printf(0, "\n======== DEBUG INITIALIZED ========\n");
    led_on();

    chThdSleep(TIME_INFINITE);
    return 0;
}
