//main.c

#include "ch.h"
#include "hal.h"

#include "led.h"
#include "pwm.h"
#include "timer.h"
#include "wifi.h"
#include "rtt.h"

int main(void)
{
    halInit();
    chSysInit();

    led_init();
    pwm_init();
    timer_init();
    wifi_init();
    rtt_init();

    led_on();
    rtt_printf(0, "\n======== DEBUG INITIALIZED ========\n\n");

    wifi_send("ver\r\n");
    chThdSleep(MS2ST(1000));
    wifi_send("help\r\n");
    
    chThdSleep(TIME_INFINITE);
    return 0;
}
