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

    led_on();
    rtt_printf(0, "\n======== DEBUG INITIALIZED ========\n\n");

    uint8_t message[] = "help\r\n";
    uint8_t buffer[] = "000000000000000";
    while(1)
    {
        rtt_printf(0, "Start uart sending\n");
        uart_send(6, message);
        rtt_printf(0, "Start uart reception\n");
        uart_receive(16, buffer);
        buffer[15] = '\0';
        rtt_printf(0, "End uart reception: %s\n", buffer);
        chThdSleep(500);
    }

    chThdSleep(TIME_INFINITE);
    return 0;
}
