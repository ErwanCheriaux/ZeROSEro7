#include <stdbool.h>
#include <stdint.h>
#include "boards.h"
#include "rtt.h"
#include "hw.h"
#include "ble_central.h"

int main(void)
{
    rtt_init();
    rtt_write_string("\n\n======== DEBUG INITIALIZED ========\n");

    bsp_board_leds_init();
    rtt_write_string("LEDs initialized\n");

    // Like if we were using BLE, has the side effect of starting LFCLK needed by RTC
    ble_init();
    rtt_write_string("BLE initialized\n");

    HW_RTC_Init();
    rtt_write_string("RTC Initialized\n") ;
    bsp_board_led_on(0);

    HW_SPI_Init() ;
    rtt_write_string("SPI Initialized\n") ;


    rtt_printf(0, "1500 tick in ms : %u\n", HW_RTC_Tick2ms(1500)) ;
    rtt_printf(0, "1000 ms in ticks : %u\n", HW_RTC_ms2Tick(1000)) ;

    HW_RTC_SetAlarm(HW_RTC_ms2Tick(1000)) ;
    rtt_write_string("Started RTC\n") ;


    while(true) {
        rtt_printf(0,"It is %u o'clock\n", HW_RTC_GetTimerValue()) ;
        HW_RTC_DelayMs(3000) ;
    }

    return 0;
}
