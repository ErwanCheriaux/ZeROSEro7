//usbh.h

#ifndef USBH_H
#define USBH_H

#define NB_INPUT             200
#define PASSWORD_BUFFER_SIZE 1024
#define PASSWORD_MAX_SIZE    30 // must be < NB_INPUT

extern uint8_t  led_status;

void usbh_init(void);
void usbh_email_detector(uint8_t input);

#endif
