//main.c

#include "ch.h"
#include "hal.h"

#include "led.h"
#include "pwm.h"
#include "timer.h"
#include "uart.h"

int main(void)
{
    halInit();
    chSysInit();

    led_init();
    pwm_init();
    timer_init();
    uart_init();

    chThdSleep(TIME_INFINITE);
    return 0;
}
