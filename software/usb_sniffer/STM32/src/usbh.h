//usbh.h

#ifndef USBH_H
#define USBH_H

extern uint8_t led_status;

void usbh_init(void);
void usbh_power(bool onoff);

#endif
