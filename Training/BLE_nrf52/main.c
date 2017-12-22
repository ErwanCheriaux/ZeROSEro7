#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "boards.h"
#include "ble_central.h"
#include "rtt.h"
#include "app_error.h"

void power_manage()
{
    ret_code_t err_code = sd_app_evt_wait();
    APP_ERROR_CHECK(err_code);
}

static void phone_noticed_handler() {
    rtt_write_string("Phone get !") ;
    APP_ERROR_CHECK(0xDEADBEEF);
}

int main(void)
{
    rtt_init();
    rtt_write_string("\n\n======== DEBUG INITIALIZED ========\n");

    bsp_board_leds_init();
    bsp_board_led_on(1);

    ble_init(phone_noticed_handler);
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
