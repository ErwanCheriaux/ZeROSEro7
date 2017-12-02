#include <stdbool.h>
#include <stdint.h>
#include "boards.h"
#include "rtt.h"
#include "hw.h"

int main(void)
{
    rtt_init();
    rtt_write_string("\n\n======== DEBUG INITIALIZED ========\n");

    bsp_board_leds_init();
    bsp_board_led_on(1);

    HW_RTC_Init();

    while(true) {
    }

    return 0;
}
