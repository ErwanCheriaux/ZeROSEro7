// Minimum RTC tick is 0.9765625ms according to Ping-Pong Demo (see hw_rtc.c, tick to ms)
// Hence using app_timer which is capable of sufficient precision (max 32768 Hz)
#include "app_timer.h"

#include "hw.h"
#include "utilities.h"

APP_TIMER_DEF(RTC_SX12);

void HW_RTC_IrqHandler(void * p_context) {
    APP_ERROR_CHECK(0xDEADBEEF) ; // TODO Unimplemented
}

void HW_RTC_Init(void) {
    APP_ERROR_CHECK(app_timer_init()) ;
    APP_ERROR_CHECK(app_timer_create(&RTC_SX12, APP_TIMER_MODE_REPEATED , HW_RTC_IrqHandler)) ;
}
