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
    rtt_write_string("RTC Initialized") ;
    bsp_board_led_on(2);

    rtt_printf(0, "Tick in ms : %u", HW_RTC_Tick2ms(65536)) ;

    while(true) {
    }

    return 0;
}
