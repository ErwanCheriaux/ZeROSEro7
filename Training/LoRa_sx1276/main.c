#include <stdbool.h>
#include <stdint.h>
#include "boards.h"
#include "rtt.h"
#include "hw.h"
#include "ble_central.h"
#include "radio.h"
#include "sx1276.h"

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

    static uint8_t addr = 0x1F ;    // non-0 SX12 register
    static uint8_t rx_buffer[10] ;
    while(true) {
        SX1276ReadBuffer(addr,rx_buffer,10) ;
        rtt_printf(0,"Received : 0x%#04X\n", rx_buffer[0]) ;
        HW_RTC_DelayMs(100) ;
    }

    return 0;
}
