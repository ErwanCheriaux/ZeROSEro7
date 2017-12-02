// Using app_timer which is capable of sufficient precision (max 32768 Hz)
#include "app_timer.h"
#include "sdk_config.h"
#include "boards.h"

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

// Given by app_timer
uint32_t HW_RTC_GetMinimumTimeout(void) {
    return 5 ;  // Ticks
}

uint32_t HW_RTC_GetTimerValue(void) {
    return app_timer_cnt_get() ;
}

void HW_RTC_DelayMs(uint32_t delay) {
    TimerTime_t delayValue = 0;
    TimerTime_t timeout = 0;

    delayValue = HW_RTC_ms2Tick( delay );

    /* Wait delay ms */
    timeout = HW_RTC_GetTimerValue( );
    while( ( ( HW_RTC_GetTimerValue( ) - timeout ) ) < delayValue )
    {
        __NOP( );
    }
}

void HW_RTC_StopAlarm(void) {
    APP_ERROR_CHECK(app_timer_stop(RTC_SX12)) ;
}

void HW_RTC_SetAlarm(uint32_t timeout) {
    APP_ERROR_CHECK(app_timer_start(RTC_SX12, timeout, NULL)) ;
}

void HW_RTC_IrqHandler(void * p_context) {
    bsp_board_led_invert(1) ;
    HW_RTC_SetAlarm(HW_RTC_ms2Tick(1000)) ;
}

void HW_RTC_Init(void) {
    APP_ERROR_CHECK(app_timer_init()) ;
    APP_ERROR_CHECK(app_timer_create(&RTC_SX12, APP_TIMER_MODE_SINGLE_SHOT , HW_RTC_IrqHandler)) ;
}
