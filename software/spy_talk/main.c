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
#include "lora_protocol.h"
#include "delay.h"

#ifdef BOARD_YELLOW
#define SPY_TALK_APP_ID YELLOW_ADDRESS
#else  // BOARD_YELLOW
#ifdef BOARD_BLUE
#define SPY_TALK_APP_ID BLUE_ADDRESS
#else  // BOARD_BLUE
#define SPY_TALK_APP_ID PINK_ADDRESS
#endif  // BOARD_BLUE
#endif  // BOARD_YELLOW

static void log_init(void)
{
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
}

static void phone_noticed_handler()
{
    rtt_write_string("Phone get, negociating connection\n");
    // We advertise to switch role in the GAP connection for lower consumption.
    // REVIEW Maybe use whitelisted scan requests instead
    ble_stop_observing();
    ble_peripheral_start_advertising();
}

static void phone_connected_handler()
{
    rtt_write_string("Phone connected\n");
    ble_stop_observing();  // If reconnecting, sometimes the phone hasn't got the time to advertise
    ble_peripheral_stop_advertising();
    led_on(1);
}

static void phone_disconnected_handler()
{
    rtt_write_string("Phone disconnected\n");
    ble_peripheral_stop_advertising();
    ble_start_observing();
    led_off(1);
}

static void phone_write_handler(uint8_t* buff, int length)
{
    rtt_write_string("Received data from phone :\n");
    rtt_write_buffer(0, buff, length);
    rtt_write_string("\n");
    lora_protocol_send(*buff, buff + 1, length - 1);
}

static void phone_notification_complete_handler()
{
}

static uint8_t notif_to_send[LORA_PROTOCOL_MESSAGE_LENGTH + 1];
void lora_on_receive(uint8_t sender_address, uint8_t* message, unsigned int length)
{
    notif_to_send[0] = sender_address;
    memcpy(notif_to_send + 1, message, length);
    phone_send_notification(notif_to_send, length + 1);
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

    ble_handler_init(phone_noticed_handler, phone_connected_handler, phone_disconnected_handler, phone_write_handler, phone_notification_complete_handler);
    ble_stack_init(SPY_TALK_APP_ID);
    ble_gap_init();
    ble_gatt_init();
    ble_advertise_init(SPY_TALK_APP_ID);
    ble_services_init();
    ble_conn_negociation_init();
    rtt_write_string("BLE initialized\n");

    ble_start_observing();
    ble_peripheral_start_advertising();
    rtt_write_string("BLE online\n");

    ble_peripheral_start_advertising();

    HW_RTC_Init();
    rtt_write_string("RTC initialized\n");

    HW_SPI_Init();
    rtt_write_string("SPI initialized\n");

    lora_protocol_handlers_init(lora_on_receive);
    lora_protocol_init(SPY_TALK_APP_ID);
    rtt_write_string("LoRa initialized\n");

    lora_protocol_start();
    rtt_write_string("\nLoRa online\n");

    while(true) {
        low_power_standby();
    }

    return 0;
}
