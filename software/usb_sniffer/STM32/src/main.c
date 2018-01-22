//main.c

#include "ch.h"
#include "hal.h"

#include "led.h"
#include "rtt.h"
#include "timer.h"

#include "usb.h"
#include "usbh.h"

int main(void)
{
    halInit();
    chSysInit();

    led_init();
    rtt_init();
    usb_init();
    usbh_init();
    timer_init();

    timer_on();

    while(1) {
        usbhMainLoop(&USBHD1);
        chThdSleepMilliseconds(1000);
    }

    chThdSleep(TIME_INFINITE);
    return 0;
}
