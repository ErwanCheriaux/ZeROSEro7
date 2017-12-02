// Minimum RTC tick is 0.9765625ms according to Ping-Pong Demo (see hw_rtc.c, tick to ms)
// Hence using app_timer which is capable of sufficient precision (max 32768 Hz)
#include "app_timer.h"
#include "sdk_config.h"

#include "hw.h"
#include "utilities.h"

APP_TIMER_DEF(RTC_SX12);

//uint32_t HW_RTC_ms2Tick(TimerTime_t timeMilliSec) {
//
//}

TimerTime_t HW_RTC_Tick2ms(uint32_t tick) {
    switch(APP_TIMER_CONFIG_RTC_FREQUENCY) {
        case 0 :
            return tick >> 15 ;
        default :
            APP_ERROR_CHECK(NRF_ERROR_INVALID_STATE) ;
            return 0 ;
    }
}

void HW_RTC_IrqHandler(void * p_context) {
    APP_ERROR_CHECK(0xDEADBEEF) ; // TODO Unimplemented
}

void HW_RTC_Init(void) {
    APP_ERROR_CHECK(app_timer_init()) ;
    APP_ERROR_CHECK(app_timer_create(&RTC_SX12, APP_TIMER_MODE_REPEATED , HW_RTC_IrqHandler)) ;
}
