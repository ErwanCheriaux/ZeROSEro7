// Using app_timer which is capable of sufficient precision (max 32768 Hz)
#include "app_timer.h"
#include "sdk_config.h"

#include "hw.h"
#include "utilities.h"

APP_TIMER_DEF(RTC_SX12);

uint32_t HW_RTC_ms2Tick( TimerTime_t timeMilliSec )
{
    return APP_TIMER_TICKS(timeMilliSec) ;
}

// Nordic doesn't provide this option so we compute the ratio with a
// convenient precision
#define MS_CONSTANT 256
TimerTime_t HW_RTC_Tick2ms( uint32_t tick )
{
    return tick * MS_CONSTANT / APP_TIMER_TICKS(MS_CONSTANT) ;
}


void HW_RTC_IrqHandler(void * p_context) {
    APP_ERROR_CHECK(0xDEADBEEF) ; // TODO Unimplemented
}

void HW_RTC_Init(void) {
    APP_ERROR_CHECK(app_timer_init()) ;
    APP_ERROR_CHECK(app_timer_create(&RTC_SX12, APP_TIMER_MODE_SINGLE_SHOT , HW_RTC_IrqHandler)) ;
}
