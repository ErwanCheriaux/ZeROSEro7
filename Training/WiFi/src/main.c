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

    //break_stream_mode();
    //chThdSleep(MS2ST(500));
    //configure();

    // Receive
    
    int size = 2;
    char buff[size + 1];
    buff[size] = '\0';
    while(1) {
        if(uart_receive_timeout(buff, size, MS2ST(2000)))
            rtt_printf(0, "Timeout\n");
        else
            rtt_printf(0, "%s\n", buff);
    }
    
    chThdSleep(TIME_INFINITE);
    return 0;
}
