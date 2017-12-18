#include "nrf_power.h"
#include "rtt.h"

void low_power_init() {
    nrf_power_task_trigger(NRF_POWER_TASK_CONSTLAT) ; // Constant 30 ms wakeup time at the cost of power
}

void low_power_standby() {
    sd_app_evt_wait() ;   // Stops MCU, waiting for IRQ
}
