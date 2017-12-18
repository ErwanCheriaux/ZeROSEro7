#include "nrf_power.h"
#include "rtt.h"

void low_power_standby() {
    // TODO Disable clocks manually ?
    APP_ERROR_CHECK(sd_app_evt_wait()) ;   // Clears pending IRQs
    APP_ERROR_CHECK(sd_app_evt_wait()) ;   // Stops MCU, waiting for IRQ
}
