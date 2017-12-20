//led.h

#ifndef LED_H
#define LED_H

#include "ch.h"
#include "hal.h"

#include "pwm.h"

void led_init(void);
void led_on(void);
void led_off(void);
void led_toggle(void);

#endif
