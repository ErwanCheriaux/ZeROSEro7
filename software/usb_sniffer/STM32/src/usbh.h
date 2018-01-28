//usbh.h

#ifndef USBH_H
#define USBH_H

#include "usb_hid_keys.h"

#define PASSWORD_BUFFER_SIZE 1024

extern mailbox_t umb;
extern uint8_t   led_status;
extern uint16_t  password[PASSWORD_BUFFER_SIZE];

void usbh_init(void);

#endif
