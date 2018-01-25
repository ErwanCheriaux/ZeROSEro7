//led.c

#include "led.h"

void led_init(void)
{
    palSetPadMode(GPIOC, GPIOC_RGB_R, PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOA, GPIOA_RGB_G, PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOC, GPIOC_RGB_B, PAL_MODE_OUTPUT_PUSHPULL);
    palSetPad(GPIOC, GPIOC_RGB_R);  //led off
    palSetPad(GPIOA, GPIOA_RGB_G);  //led off
    palSetPad(GPIOC, GPIOC_RGB_B);  //led off
}

void led_rgb(bool r, bool g, bool b)
{
    if(r)
        palClearPad(GPIOC, GPIOC_RGB_R);
    else
        palSetPad(GPIOC, GPIOC_RGB_R);
    if(g)
        palClearPad(GPIOA, GPIOA_RGB_G);
    else
        palSetPad(GPIOA, GPIOA_RGB_G);
    if(b)
        palClearPad(GPIOC, GPIOC_RGB_B);
    else
        palSetPad(GPIOC, GPIOC_RGB_B);
}
