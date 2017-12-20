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
    palSetPad(GPIOC, GPIOC_LED);
    toggle = true;
}

void led_off(void)
{
    palClearPad(GPIOC, GPIOC_LED);
    toggle = false;
}

void led_toggle(void)
{
    rtt_printf(0, "LED toggle\n");
    if(toggle)
        led_off();
    else
        led_on();
}
