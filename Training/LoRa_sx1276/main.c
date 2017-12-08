#include <stdbool.h>
#include <stdint.h>
#include "boards.h"
#include "rtt.h"
#include "hw.h"
#include "ble_central.h"
#include "lora.h"
#include "sx1276Regs-LoRa.h"
#include "delay.h"

static uint8_t addr ;    // non-0 SX12 register
static uint8_t rx_buffer[4] ;
static uint8_t tx_buffer[4] = {1,2,3,4} ;
static uint8_t lora_send_buffer[4] = {'a','b','c','d'} ;

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

    lora_init() ;
    rtt_write_string("LoRa initialized\n") ;
    addr = REG_LR_MODEMCONFIG1 ;
    Radio.ReadBuffer(addr,rx_buffer,2) ;    // should print 0x90,0x40,0x40,0x00 at RESET
    rtt_printf(0,"LoRa config regs : 0x%#02X, 0x%#02X\n", rx_buffer[0], rx_buffer[1]) ;

    rtt_printf(0, "1500 tick in ms : %u\n", HW_RTC_Tick2ms(1500)) ;
    rtt_printf(0, "1000 ms in ticks : %u\n", HW_RTC_ms2Tick(1000)) ;


    addr = REG_LR_FIFORXBYTEADDR ;
    Radio.ReadBuffer(addr,rx_buffer,1) ;
    rtt_printf(0,"Fifo rx addr : 0x%#02X\n", rx_buffer[0]) ;

    addr = REG_LR_OPMODE ;
    Radio.ReadBuffer(addr,rx_buffer,1) ;
    rtt_printf(0,"Op mode : 0x%#02X\n", rx_buffer[0]) ; // TODO says LF mode registers ...

    addr = REG_LR_PACONFIG ;
    Radio.ReadBuffer(addr,rx_buffer,1) ;
    rtt_printf(0,"Amplification config : 0x%#02X\n", rx_buffer[0]) ;

    //DelayMs(3000) ;

    while(true) {
        addr = REG_LR_FIFORXBYTEADDR ;
        Radio.ReadBuffer(addr,rx_buffer,1) ;
        rtt_printf(0,"Fifo rx addr : 0x%#02X\n", rx_buffer[0]) ;

        lora_observe() ;
        rtt_write_string("LoRa receiving\n") ;
        DelayMs(10050) ;

        lora_send(lora_send_buffer, 4) ;
        rtt_write_string("LoRa sending\n") ;
        DelayMs(10050) ;
    }

    return 0;
}
