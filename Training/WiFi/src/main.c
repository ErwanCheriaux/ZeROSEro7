//main.c

#include "ch.h"
#include "hal.h"

#include "led.h"
#include "pwm.h"
#include "timer.h"
#include "wifi.h"
#include "rtt.h"
#include "uart.h"

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
    
    wifi_command("set bus.mode command\r\n", 1000, 1);
    wifi_command("set softap.auto_start true\r\n", 1000, 1);
    wifi_command("set softap.dhcp_server.enabled true\r\n", 1000, 1);
    wifi_command("set softap.ssid ZeROSEro7\r\n", 1000, 1);
    wifi_command("get softap.info\r\n", 1000, 1);
    wifi_command("set tcp.server.auto_interface softap\r\n", 1000, 1);
    wifi_command("set tcp.server.auto_start true\r\n", 1000, 1);
    wifi_command("set tcp.server.idle_timeout 300\r\n", 1000, 1);
    wifi_command("set tcp.server.connected_gpio 22\r\n", 1000, 1);
    wifi_command("set tcp.server.data_gpio 21\r\n", 1000, 1);
    wifi_command("set bus.mode stream\r\n", 1000, 1);
    wifi_command("save\r\n", 1000, 1);
    wifi_command("reboot\r\n", 5000, 1);
    char buff[] = "ok";
    while(1) {
        uart_send(buff);
        chThdSleep(MS2ST(1000));
    }
    
    chThdSleep(TIME_INFINITE);
    return 0;
}
