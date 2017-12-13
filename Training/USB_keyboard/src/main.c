//main.c

#include "ch.h"
#include "hal.h"
#include "usbh/debug.h" /* for usbDbgPuts/usbDbgPrintf */

#include "fibo.h"

#include "led.h"
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
    rtt_init();
    usb_init();
    timer_init();

    timer_on();

    /*USBH OTG*/
    palSetPadMode(GPIOB, GPIOB_OTG_HS_VBUS, PAL_MODE_INPUT_PULLDOWN);
    palSetPadMode(GPIOB, GPIOB_OTG_HS_ID, PAL_MODE_ALTERNATE(10));
    palSetPadMode(GPIOB, GPIOB_OTG_HS_DM, PAL_MODE_ALTERNATE(10));
    palSetPadMode(GPIOB, GPIOB_OTG_HS_DP, PAL_MODE_ALTERNATE(10));
    palSetPadMode(GPIOA, GPIOA_USB_HS_BUSON, PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOF, GPIOF_USB_HS_FAULT, PAL_MODE_INPUT);

    //turn on USB power
    palSetPad(GPIOA, GPIOA_USB_HS_BUSON);
    chThdSleepMilliseconds(100);

    usbhStart(&USBHD2);

    while(1) {
        usbhMainLoop(&USBHD2);
        chThdSleepMilliseconds(100);
//      usbhDevicePrintInfo(&USBHD2);
    }

    chThdSleep(TIME_INFINITE);
    return 0;
}
