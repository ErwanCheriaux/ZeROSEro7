//main.c

#include "ch.h"
#include "hal.h"

#include "led.h"
#include "pwm.h"
#include "timer.h"
#include "wifi.h"
#include "rtt.h"
#include "sha256.h"

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

    wifi_command("ver\r\n", 1000);
    wifi_command("reboot\r\n", 6000);
    wifi_command("ver\r\n", 1000);
    wifi_command("hge https://www.google.com\r\n", 4000);
    wifi_command("ver\r\n", 1000);
    wifi_command("tcp_server -i wlan start 666\r\n", 4000);
    wifi_command("ver\r\n", 1000);

    chThdSleep(TIME_INFINITE);
    return 0;
}
