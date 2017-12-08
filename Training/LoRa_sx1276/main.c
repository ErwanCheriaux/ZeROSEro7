#include <stdbool.h>
#include <stdint.h>
#include "boards.h"
#include "rtt.h"
#include "hw.h"
#include "ble_central.h"
#include "lora.h"


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
    HW_RTC_SetAlarm(HW_RTC_ms2Tick(1000)) ;
    rtt_write_string("Started RTC\n") ;

    HW_SPI_Init() ;
    rtt_write_string("SPI Initialized\n") ;

    lora_init() ;
    rtt_write_string("LoRa initialized\n") ;


    rtt_printf(0, "1500 tick in ms : %u\n", HW_RTC_Tick2ms(1500)) ;
    rtt_printf(0, "1000 ms in ticks : %u\n", HW_RTC_ms2Tick(1000)) ;

    static uint8_t addr = 0x30 ;    // non-0 SX12 register
    static uint8_t rx_buffer[4] ;
    static uint8_t tx_buffer[4] = {1,2,3,4} ;
    while(true) {
        Radio.ReadBuffer(addr,rx_buffer,4) ;    // should print 0x90,0x40,0x40,0x00 at RESET
        rtt_printf(0,"Read : 0x%#04X, 0x%#04X, 0x%#04X, 0x%#04X\n", rx_buffer[0], rx_buffer[1], rx_buffer[2], rx_buffer[3]) ;
        HW_RTC_DelayMs(3000) ;
    }

    return 0;
}
