#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "boards.h"
#include "app_error.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "app_timer.h"

#include "ble_uart_service.h"
#include "ble_central.h"
#include "ble_peripheral_gap.h"
#include "ble_peripheral_gatt.h"
#include "rtt.h"

// Goes in low power mode. Not const lat, not OFF mode
void power_manage()
{
    APP_ERROR_CHECK(sd_app_evt_wait());
}

static void phone_noticed_handler()
{
    rtt_write_string("Phone get, negociating connection\n");
    bsp_board_led_on(2);
    // We advertise to switch role in the GAP connection for lower consumption.
    // REVIEW Maybe use whitelisted scan requests instead
    ble_stop_observing();
    ble_peripheral_start_advertising();
}

static void phone_connected_handler()
{
    rtt_write_string("Phone connected\n");
    ble_peripheral_stop_advertising();
    bsp_board_led_on(3);
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
    phone_send_notification(buff, length);
}

static void log_init(void)
{
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
}

#include "nrf_drv_gpiote.h"
static nrf_drv_gpiote_out_config_t led_config = {
    NRF_GPIOTE_POLARITY_TOGGLE,
    NRF_GPIOTE_INITIAL_VALUE_HIGH,
    false};

int main(void)
{
    rtt_init();
    log_init();
    NRF_LOG_INFO("\n\n======== DEBUG INITIALIZED ========\n");

    bsp_board_leds_init();

    ble_handler_init(phone_noticed_handler, phone_connected_handler, phone_disconnected_handler, phone_write_handler);
    ble_stack_init();
    ble_gap_init();
    ble_gatt_init();
    ble_advertise_init();
    ble_services_init();
    ble_conn_negociation_init();
    // ble_peer_init(); REVIEW Bonus, bond with device and security
    rtt_write_string("BLE initialized\n");

    ble_start_observing();
    ble_peripheral_start_advertising(); // TODO remove. Convenient for debugging purpose
    rtt_write_string("Now observing BLE\n");
    bsp_board_led_on(1);

    while(true) {
        power_manage();
    }

    return 0;
}
