#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "boards.h"
#include "ble_central.h"
#include "ble_peripheral_gap.h"
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
    // We advertise to switch role in the GAP negociation for lower consumption
    ble_stop_observing();
    ble_peripheral_start_advertising();
}

static void phone_connected_handler()
{
    rtt_write_string("!!! Phone connected !!!\n");
}

int main(void)
{
    rtt_init();
    rtt_write_string("\n\n======== DEBUG INITIALIZED ========\n");

    bsp_board_leds_init();
    bsp_board_led_on(1);

    ble_init(phone_noticed_handler);
    ble_peripheral_advertising_init(phone_connected_handler);
    rtt_write_string("BLE initialized\n");
    bsp_board_led_on(2);

    ble_start_observing();
    rtt_write_string("Now observing BLE\n");
    bsp_board_led_on(3);

    while(true) {
        power_manage();
    }

    return 0;
}
