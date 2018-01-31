#ifdef TEST_BOARD

#include "boards.h"

void leds_init()
{
    bsp_board_leds_init();
}

void led_on(int i)
{
    bsp_board_led_on(i);
}

void led_off(int i)
{
    bsp_board_led_off(i);
}

void led_toggle(int i)
{
    bsp_board_led_invert(i);
}

#endif
