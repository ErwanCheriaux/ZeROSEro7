#include "nrf.h"
#include "nrf_drv_timer.h"  // Not app_timer, which is for RTC

#include "hw.h"
#include "utilities.h"

static const nrf_drv_timer_t TIMER_SX12 = NRF_DRV_TIMER_INSTANCE(0) ;

// Parameters from sdk_config.h
static const nrf_drv_timer_config_t timer_default_cfg = NRF_DRV_TIMER_DEFAULT_CONFIG ;

void HW_RTC_Init(void) {

}

uint32_t HW_RTC_ms2Tick(TimerTime_t timeMilliSec) {
    return nrf_drv_timer_ms_to_ticks(&TIMER_SX12, timeMilliSec) ;
}
