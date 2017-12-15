#include <stdbool.h>
#include <stdint.h>
#include "boards.h"
#include "rtt.h"
#include "hw.h"
#include "ble_central.h"
#include "lora.h"
#include "sx1276Regs-LoRa.h"
#include "delay.h"

static uint8_t addr;                                        // SPI address
static uint8_t rx_buffer[4];                                // SPI MISO
static uint8_t tx_buffer[4]        = {1, 2, 3, 4};          // SPI MOSI
static uint8_t lora_send_buffer[4] = {'a', 'b', 'c', 'd'};  // LORA FIFO TX

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
    rtt_write_string("RTC Initialized\n");
    bsp_board_led_on(0);

    HW_SPI_Init();
    rtt_write_string("SPI Initialized\n");

    lora_init();
    rtt_write_string("LoRa initialized\n");
    addr = REG_LR_MODEMCONFIG1;
    Radio.ReadBuffer(addr, rx_buffer, 2);
    rtt_printf(0, "LoRa config regs : 0x%#02X, 0x%#02X\n", rx_buffer[0], rx_buffer[1]);

    rtt_printf(0, "1500 tick in ms : %u\n", HW_RTC_Tick2ms(1500));
    rtt_printf(0, "1000 ms in ticks : %u\n", HW_RTC_ms2Tick(1000));

    static bool send = true;

    while(true) {

        if(send) {
            rtt_write_string("\nLoRa sending\n");
            lora_send(lora_send_buffer, 4);
        } else {
            rtt_write_string("\nReceiving\n");
            lora_observe();
        }
        send = !send;

        while(Radio.GetStatus() != RF_IDLE)
            ;
    }

    return 0;
}
