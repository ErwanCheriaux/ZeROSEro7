//usbh.h

#ifndef USBH_H
#define USBH_H

extern uint8_t led_status;
extern uint16_t password[200];
extern uint16_t password_size;

void usbh_init(void);

#endif
