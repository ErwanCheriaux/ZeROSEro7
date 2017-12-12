//main.c

#include "ch.h"
#include "hal.h"
#include "usbh/debug.h" /* for usbDbgPuts/usbDbgPrintf */

#include "fibo.h"

#include "led.h"
#include "pwm.h"
#include "rtt.h"
#include "usb.h"
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
    usb_init();
    timer_init();

    timer_on();

    /*USBH OTG*/

    //turn on USB power
    palClearPad(GPIOA, GPIOA_USB_HS_BUSON);
    chThdSleepMilliseconds(100);

    usbhStart(&USBHD2);

    while(1) {
        usbhMainLoop(&USBHD2);
        chThdSleepMilliseconds(100);
    }

    chThdSleep(TIME_INFINITE);
    return 0;
}
