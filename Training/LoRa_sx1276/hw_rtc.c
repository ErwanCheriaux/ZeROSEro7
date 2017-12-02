// Using app_timer which is capable of sufficient precision (max 32768 Hz)
#include "app_timer.h"
#include "sdk_config.h"

#include "hw.h"
#include "utilities.h"

/* Calculates ceiling(X/N) */
#define DIVC(X,N)   ( ( (X) + (N) -1 ) / (N) )

// Still not sure how ticks are used. Seem different from RTC precision.
// tick is 0.9765625ms according to Ping-Pong Demo (see hw_rtc.c, tick to ms)
#define TICK_NUMER 9765625
#define TICK_DENOM 10000000


APP_TIMER_DEF(RTC_SX12);

uint32_t HW_RTC_ms2Tick(TimerTime_t timeMilliSec) {
  return (uint32_t) (( ((uint64_t)timeMilliSec) * TICK_DENOM) / TICK_NUMER);
}

TimerTime_t HW_RTC_Tick2ms(uint32_t tick) {
    return  ( ((uint64_t)(tick) * TICK_NUMER) / TICK_DENOM);
}

void HW_RTC_IrqHandler(void * p_context) {
    APP_ERROR_CHECK(0xDEADBEEF) ; // TODO Unimplemented
}

void HW_RTC_Init(void) {
    APP_ERROR_CHECK(app_timer_init()) ;
    APP_ERROR_CHECK(app_timer_create(&RTC_SX12, APP_TIMER_MODE_SINGLE_SHOT , HW_RTC_IrqHandler)) ;
}
