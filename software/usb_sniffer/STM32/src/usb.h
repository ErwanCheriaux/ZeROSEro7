/*
    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#ifndef USB_H
#define USB_H

#include "hal_usb_hid.h"

extern const USBConfig    usbcfg;
extern const USBHIDConfig usbhidcfg;
extern USBHIDDriver       UHD2;
extern uint8_t            led_status;

void usb_init(void);
void usbh_init(void);
void usb_send_key(USBHIDDriver *uhdp, uint8_t key);
void usb_report(USBHIDDriver *uhdp, uint8_t *bp);

#endif /* USB_H */

/** @} */
