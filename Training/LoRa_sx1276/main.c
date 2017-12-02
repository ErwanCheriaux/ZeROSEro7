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
    rtt_write_string("RTC Initialized\n") ;
    bsp_board_led_on(2);

    rtt_printf(0, "15 tick in ms : %u\n", HW_RTC_Tick2ms(15)) ;
    rtt_printf(0, "15 ms in ticks : %u\n", HW_RTC_ms2Tick(15)) ;

    while(true) {
    }

    return 0;
}
