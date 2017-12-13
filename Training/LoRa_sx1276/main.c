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

static uint8_t fifo_rx_start, fifo_rx_end;

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

    while(true) {

        lora_send(lora_send_buffer, 4);
        rtt_write_string("\nLoRa sending\n");

        while(true) {}
        /*send = !send;
        DelayMs(1);
        *rx_buffer = Radio.Read(REG_LR_OPMODE);
        rtt_printf(0, "Op mode : 0x%#02X\n", rx_buffer[0]);
        DelayMs(1050);

        fifo_rx_end = Radio.Read(REG_LR_FIFORXBYTEADDR);
        rtt_printf(0, "Fifo rx addr : 0x%#02X\n", fifo_rx_end);
        fifo_rx_start = Radio.Read(REG_LR_FIFORXCURRENTADDR);
        rtt_printf(0, "Fifo rx current addr : 0x%#02X\n", fifo_rx_start);

        if(fifo_rx_end != 0x00) {
            Radio.ReadBuffer(fifo_rx_start, rx_buffer, 4);
            rtt_printf(0, "Data received ! : %c, %c, %c, %c\n", rx_buffer[0], rx_buffer[1], rx_buffer[2], rx_buffer[3]);
            APP_ERROR_CHECK(0xDEADBEEF);
        }

        *rx_buffer = Radio.Read(REG_LR_IRQFLAGS);
        rtt_printf(0, "Irq : 0x%#02X\n", rx_buffer[0]);
        *rx_buffer = Radio.Read(REG_LR_RXNBBYTES);
        rtt_printf(0, "Received bytes count : 0x%#02X\n", rx_buffer[0]);
        *rx_buffer = Radio.Read(REG_LR_MODEMSTAT);
        rtt_printf(0, "Modem status : 0x%#02X\n", rx_buffer[0]);
        *rx_buffer = Radio.Read(REG_LR_HOPCHANNEL);
        rtt_printf(0, "Hopping status : 0x%#02X\n", rx_buffer[0]);*/
    }

    return 0;
}
