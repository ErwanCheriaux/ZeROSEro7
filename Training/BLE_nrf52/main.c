#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "boards.h"
#include "ble_central.h"
#include "ble_peripheral_gap.h"
#include "ble_peripheral_gatt.h"
#include "rtt.h"
#include "app_error.h"

// Goes in low power mode. Not const lat, not OFF mode
void power_manage()
{
    APP_ERROR_CHECK(sd_app_evt_wait());
}

static void phone_noticed_handler()
{
    rtt_write_string("!!! Phone get, negociating connection !!!\n");
    bsp_board_led_on(2);
    // We advertise to switch role in the GAP connection for lower consumption
    ble_stop_observing();
    ble_peripheral_start_advertising();
}

static void phone_connected_handler()
{
    rtt_write_string("!!! Phone connected !!!\n");
    bsp_board_led_on(3);
}

int main(void)
{
    rtt_init();
    rtt_write_string("\n\n======== DEBUG INITIALIZED ========\n");

    bsp_board_leds_init();

    ble_handler_init(phone_noticed_handler);
    ble_advertising_handler_init(phone_connected_handler);
    ble_stack_init();
    ble_gap_init();
    ble_gatt_init();
    ble_advertise_init();
    ble_services_init();
    //ble_conn_negociation_init();
    // ble_peer_init(); TODO
    rtt_write_string("BLE initialized\n");

    ble_start_observing();
    rtt_write_string("Now observing BLE\n");
    bsp_board_led_on(1);

    while(true) {
        power_manage();
    }

    return 0;
}
