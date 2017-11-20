//pwm.h

#ifndef PWM_H
#define PWM_H

#include "ch.h"
#include "hal.h"
#include "led.h"

void pwm_init(void);
void pwm_on(void);
void pwm_off(void);
void pwm_width(uint16_t w);

#endif
