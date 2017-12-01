#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "boards.h"
#include "ble_central.h"
#include "rtt.h"
#include "app_error.h"


int main(void)
{
    rtt_init();
    rtt_write_string("\n\n======== DEBUG INITIALIZED ========\n");

    bsp_board_leds_init();
    bsp_board_led_on(1);

    while(true) {
    }

    return 0;
}
