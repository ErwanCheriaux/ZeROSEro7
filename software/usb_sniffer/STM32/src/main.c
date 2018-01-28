//main.c

#include "ch.h"
#include "hal.h"

#include "led.h"
#include "rtt.h"
#include "spi.h"
#include "usb.h"
#include "usbh.h"
#include "timer.h"

mailbox_t umb;

int main(void)
{
    halInit();
    chSysInit();

    led_init();
    rtt_init();
    spi_init();
    usb_init();
    usbh_init();
    timer_init();

    timer_on();

    msg_t msg;

    while(1) {
        //mailbox check
        chMBFetch(&umb, &msg, TIME_INFINITE);
        rtt_printf("mailbox (umb) = %d", msg);
    }

    return 0;
}
