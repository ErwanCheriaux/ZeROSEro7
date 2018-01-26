//usbh.h

#ifndef USBH_H
#define USBH_H

#define USBMB_SIZE 200
#define PASSWORD_BUFFER_SIZE 1024

extern msg_t usbmb_buffer[USBMB_SIZE];
extern mailbox_t usbmb;
extern uint8_t  led_status;
extern uint16_t password[PASSWORD_BUFFER_SIZE];

void usbh_init(void);

#endif
