//led.c

#include "led.h"

static bool toggle = false;

void led_init(void)
{
    palSetPadMode(GPIOC, GPIOC_LED, PAL_MODE_OUTPUT_PUSHPULL);
    palSetPad(GPIOC, GPIOC_LED);  //led off
}

void led_on(void)
{
    pwm_on();
    toggle = true;
}

void led_off(void)
{
    pwm_off();
    toggle = false;
}

void led_toggle(void)
{
    if(toggle)
        led_off();
    else
        led_on();
}
