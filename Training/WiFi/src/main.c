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

    rtt_printf(0, "\n======== INITIALIZATION SUCCED ========\n\n");
    led_on();

    wifi_command("setup stop\r\n", 1000);
    wifi_command("set so s \"We <3 M.Polti & M.Tardieu\"\r\n", 1000);
    wifi_command("set so p \"zerosero7\"\r\n", 1000);
    wifi_command("set so u \"zerosero7.com\"\r\n", 1000);
    wifi_command("tcp_server -i softap start 20\r\n", 2000);
    wifi_command("get so i\r\n", 1000);
    wifi_command("get so u\r\n", 1000);

    chThdSleep(TIME_INFINITE);
    return 0;
}
