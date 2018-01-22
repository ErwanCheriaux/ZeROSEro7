#include <stdbool.h>
#include <stdint.h>
#include "ble_uart_service.h"
#include "ble_central.h"
#include "ble_peripheral_gap.h"
#include "ble_peripheral_gatt.h"
#include "rtt.h"
#include "low_power.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "hw.h"
#include "ble_central.h"
#include "ble_peripheral_gap.h"
#include "lora.h"
#include "sx1276Regs-LoRa.h"
#include "delay.h"

static uint8_t addr;                                        // SPI address
static uint8_t rx_buffer[4];                                // SPI MISO
static uint8_t tx_buffer[4]        = {1, 2, 3, 4};          // SPI MOSI
static uint8_t lora_send_buffer[4] = {'a', 'b', 'c', 'd'};  // LORA FIFO TX

#define LORA_RX_TIMEOUT 8000  // ms

static void lora_callback();

static void lora_tx_done_handler()
{
    rtt_write_string(" -> Tx done\n");
    lora_callback();
}

static void lora_tx_timeout_handler()
{
    rtt_write_string(" -> Tx timeout\n");
    lora_callback();
}

static void lora_rx_timeout_handler()
{
    rtt_write_string(" -> Rx timeout\n");
    Radio.Standby();
    lora_callback();
}

static void lora_rx_error_handler()
{
    rtt_write_string(" -> Rx error\n");
    lora_callback();
}

static void lora_rx_done_handler(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr)
{
    rtt_write_string(" -> Rx done\nData : ");
    rtt_write_buffer(0, payload, size);
    rtt_printf(0, "\nRssiValue=%d dBm, SnrValue=%d\n", rssi, snr);
    DelayMs(LORA_RX_TIMEOUT - 1000);
    Radio.Standby();
    lora_callback();
}

static RadioEvents_t RadioEvents = {
    lora_tx_done_handler,
    lora_tx_timeout_handler,
    lora_rx_done_handler,
    lora_rx_timeout_handler,
    lora_rx_error_handler,
    NULL,  // Frequency Hopping handler
    NULL   // CAD done handler
};

static void log_init(void)
{
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
}

static bool send = true;

static void lora_callback()
{
    if(send) {
        rtt_write_string("\nLoRa sending\n");
        Radio.Send(lora_send_buffer, 4);
    } else {
        rtt_write_string("\nReceiving\n");
        Radio.Rx(LORA_RX_TIMEOUT);
    }
    send = !send;
}


static void phone_noticed_handler()
{
    rtt_write_string("Phone get, negociating connection\n");
    led_on(2);
    // We advertise to switch role in the GAP connection for lower consumption.
    // REVIEW Maybe use whitelisted scan requests instead
    ble_stop_observing();
    ble_peripheral_start_advertising();
}

static void phone_connected_handler()
{
    rtt_write_string("Phone connected\n");
    ble_peripheral_stop_advertising();
    led_on(3);
}

static void phone_disconnected_handler()
{
    rtt_write_string("Phone disconnected\n");
    ble_peripheral_stop_advertising();
    ble_start_observing();
}

static void phone_write_handler(uint8_t *buff, int length)
{
    rtt_write_string("Received data from phone :\n");
    rtt_write_buffer(0, buff, length);
    rtt_write_string("\n");
    rtt_write_string("Sending data to phone :\n");
    rtt_write_buffer(0, buff, length);
    rtt_write_string("\n");
    phone_send_notification((uint8_t*)"abcdefghijklmnopqrstuvwxyz abcdefghijklmnopqrstuvwxyz", 50);
}


// TODO Measure Reset time for deep sleep
int main(void)
{
    rtt_init();
    rtt_write_string("RTT initialized\n");

    log_init();
    NRF_LOG_INFO("\n\n======== DEBUG INITIALIZED ========\n");

    leds_init();
    rtt_write_string("LEDs initialized\n");

    ble_handler_init(phone_noticed_handler, phone_connected_handler, phone_disconnected_handler, phone_write_handler);
    ble_stack_init();
    ble_gap_init();
    ble_gatt_init();
    ble_advertise_init();
    ble_services_init();
    ble_conn_negociation_init();
    rtt_write_string("BLE initialized\n");

    ble_start_observing();
    ble_peripheral_start_advertising(); // TODO remove. Convenient for debugging purpose
    rtt_write_string("Now observing BLE\n");
    led_on(1);

    ble_peripheral_start_advertising();

    HW_RTC_Init();
    rtt_write_string("RTC initialized\n");
    led_on(0);

    HW_SPI_Init();
    rtt_write_string("SPI initialized\n");

    lora_init(&RadioEvents);
    rtt_write_string("LoRa initialized\n");
    addr = REG_LR_MODEMCONFIG1;
    Radio.ReadBuffer(addr, rx_buffer, 2);
    rtt_printf(0, "LoRa config regs : 0x%#02X, 0x%#02X\n", rx_buffer[0], rx_buffer[1]);

    rtt_printf(0, "1500 tick in ms : %u\n", HW_RTC_Tick2ms(1500));
    rtt_printf(0, "1000 ms in ticks : %u\n", HW_RTC_ms2Tick(1000));

    rtt_write_string("\nLoRa sending\n");
    Radio.Send(lora_send_buffer, 4);
    send = !send;

    while(true) {
        low_power_standby();
    }

    return 0;
}
