#include <stdbool.h>
#include <stdint.h>
#include "boards.h"
#include "rtt.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "hw.h"
#include "ble_central.h"
#include "lora.h"
#include "sx1276Regs-LoRa.h"
#include "delay.h"

static uint8_t addr;                                        // SPI address
static uint8_t rx_buffer[4];                                // SPI MISO
static uint8_t tx_buffer[4]        = {1, 2, 3, 4};          // SPI MOSI
static uint8_t lora_send_buffer[4] = {'a', 'b', 'c', 'd'};  // LORA FIFO TX

#define LORA_RX_TIMEOUT 8000    // ms

static void lora_tx_done_handler()
{
    rtt_write_string(" -> Tx done\n");
}

static void lora_tx_timeout_handler()
{
    rtt_write_string(" -> Tx timeout\n");
}

static void lora_rx_timeout_handler()
{
    rtt_write_string(" -> Rx timeout\n");
    Radio.Standby();
}

static void lora_rx_error_handler()
{
    rtt_write_string(" -> Rx error\n");
}

static void lora_rx_done_handler(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr)
{
    rtt_write_string(" -> Rx done\nData : ");
    rtt_write_buffer(0, payload, size);
    rtt_printf(0, "\nRssiValue=%d dBm, SnrValue=%d\n", rssi, snr);
    DelayMs(LORA_RX_TIMEOUT-1000);
    Radio.Standby();
}

static RadioEvents_t RadioEvents ={
    lora_tx_done_handler,
    lora_tx_timeout_handler,
    lora_rx_done_handler,
    lora_rx_timeout_handler,
    lora_rx_error_handler,
    NULL,   // Frequency Hopping handler
    NULL    // CAD done handler
};

static void log_init(void)
{
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
}

int main(void)
{
    rtt_init();
    rtt_write_string("\n\n======== DEBUG INITIALIZED ========\n");

    log_init();
    NRF_LOG_INFO("Log initialized\n");

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

    lora_init(&RadioEvents);
    rtt_write_string("LoRa initialized\n");
    addr = REG_LR_MODEMCONFIG1;
    Radio.ReadBuffer(addr, rx_buffer, 2);
    rtt_printf(0, "LoRa config regs : 0x%#02X, 0x%#02X\n", rx_buffer[0], rx_buffer[1]);

    rtt_printf(0, "1500 tick in ms : %u\n", HW_RTC_Tick2ms(1500));
    rtt_printf(0, "1000 ms in ticks : %u\n", HW_RTC_ms2Tick(1000));

    static bool send = true;

    while(true) {
         NRF_LOG_PROCESS() ;

        if(send) {
            rtt_write_string("\nLoRa sending\n");
            Radio.Send(lora_send_buffer,4);
        } else {
            rtt_write_string("\nReceiving\n");
            Radio.Rx(LORA_RX_TIMEOUT);
        }
        send = !send;

        while(Radio.GetStatus() != RF_IDLE)
            ;
    }

    return 0;
}
