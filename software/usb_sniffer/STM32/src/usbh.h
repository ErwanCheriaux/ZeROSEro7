//usbh.h

#ifndef USBH_H
#define USBH_H

#include "usb_hid_keys.h"

extern mailbox_t umb;
extern uint8_t   led_status;

void usbh_init(void);
void usbh_add_input(uint16_t input);
void usbh_print_input(void);

#endif
