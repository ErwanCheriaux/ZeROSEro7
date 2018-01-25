//usbh.h

#ifndef USBH_H
#define USBH_H

#define PASSWORD_BUFFER_SIZE 100

extern uint8_t led_status;
extern uint8_t password[PASSWORD_BUFFER_SIZE];

void usbh_init(void);

#endif
