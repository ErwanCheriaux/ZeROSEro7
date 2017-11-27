//timer.h

#ifndef TIMER_H
#define TIMER_H

#include "ch.h"
#include "hal.h"
#include "led.h"
#include "pwm.h"

void timer_init(void);
void timer_on(void);
void timer_off(void);

#endif
