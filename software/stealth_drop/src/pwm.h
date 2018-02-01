//pwm.h

#ifndef PWM_H
#define PWM_H

#include "ch.h"
#include "hal.h"
#include "led.h"

/* Initialize pwm for leds
** w: see pwm_width
*/
void pwm_init(uint16_t w);

/* Power on leds
*/
void pwm_on(void);

/* Power off leds
*/
void pwm_off(void);

/* Set intensity power
 * [       1      -       99      ]
 * [low intensity - high intensity]
 */
void pwm_width(uint16_t w);

/* set r g b intensity (0 or 1)
*/
void pwm_rgb(int r, int g, int b);

void pwm_r(int r);

void pwm_g(int g);

void pwm_b(int b);

#endif
