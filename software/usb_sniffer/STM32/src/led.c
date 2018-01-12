//led.c

#include "led.h"

static bool toggle = false;

void led_init(void)
{
    palSetPadMode(GPIOC, GPIOC_RGB_G, PAL_MODE_OUTPUT_PUSHPULL);
    palSetPad(GPIOC, GPIOC_RGB_G);  //led off
}

void led_on(void)
{
    palSetPad(GPIOC, GPIOC_RGB_G);
    toggle = true;
}

void led_off(void)
{
    palClearPad(GPIOC, GPIOC_RGB_G);
    toggle = false;
}

void led_toggle(void)
{
    if(toggle)
        led_off();
    else
        led_on();
}
