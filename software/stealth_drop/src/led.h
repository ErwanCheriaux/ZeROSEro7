//led.h

#ifndef LED_H
#define LED_H

#include "ch.h"
#include "hal.h"

#include "pwm.h"

void led_init(int r, int g, int b, int intensity);

void led_on(void);

void led_off(void);

void led_toggle(void);

#define led_rgb(r, g, b) pwm_rgb(r, g, b)

#define led_r(r) pwm_r(r)

#define led_g(g) pwm_g(g)

#define led_b(b) pwm_b(b)

#endif
