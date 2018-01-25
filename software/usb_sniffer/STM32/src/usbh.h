//usbh.h

#ifndef USBH_H
#define USBH_H

#define NB_INPUT 200
#define PASSWORD_BUFFER_SIZE 1024

extern uint8_t led_status;
extern uint8_t password[PASSWORD_BUFFER_SIZE];
extern uint8_t password_hid[PASSWORD_BUFFER_SIZE];

extern uint8_t input_hid[NB_INPUT];
extern int     input_index;

void usbh_init(void);

#endif
