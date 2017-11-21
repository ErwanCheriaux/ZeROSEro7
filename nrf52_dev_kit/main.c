#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "boards.h"
#include "ble_peripheral.h"
#include "rtt.h"
#include "app_error.h"

void power_manage() {
    ret_code_t err_code = sd_app_evt_wait() ;
    APP_ERROR_CHECK(err_code) ;
}

int main(void)
{
    rtt_init() ;
    rtt_write_string("\n\n======== DEBUG INITIALIZED ========\n") ;

    bsp_board_leds_init();
    bsp_board_led_on(1);

    ble_init() ;
    rtt_write_string("BLE initialized\n") ;
    bsp_board_led_on(2);

    ble_start_advertising() ;
    rtt_write_string("Now advertizing BLE\n") ;
    bsp_board_led_on(3);

    while(true) {
        power_manage();
    }

    return 0;
}
