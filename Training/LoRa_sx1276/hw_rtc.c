// Using app_timer which is capable of sufficient precision (max 32768 Hz)
#include "app_timer.h"
#include "sdk_config.h"
#include "boards.h"
#include "rtt.h"

#include "hw.h"
#include "utilities.h"
#include "timeServer.h"

APP_TIMER_DEF(RTC_SX12);

// Simple context lasting only a few days. Copy icube if a greater one is needed.
static volatile uint32_t context;
static bool              timer_running = false;  // Indicates if Alarm is On (multiple starts not supported!)
static bool              timer_started = false;  // Indicates if timer has been started once

uint32_t HW_RTC_ms2Tick(TimerTime_t timeMilliSec)
{
    return APP_TIMER_TICKS(timeMilliSec);
}

// Nordic doesn't provide this option so we compute the ratio with a
// convenient precision
#define MS_CONSTANT 256
TimerTime_t HW_RTC_Tick2ms(uint32_t tick)
{
    return tick * MS_CONSTANT / APP_TIMER_TICKS(MS_CONSTANT);
}

// Given by app_timer
uint32_t HW_RTC_GetMinimumTimeout(void)
{
    return 5;  // Ticks
}

uint32_t HW_RTC_GetTimerValue(void)
{
    return app_timer_cnt_get();
}

uint32_t HW_RTC_SetTimerContext(void)
{
    context = HW_RTC_GetTimerValue();
    return context;
}

uint32_t HW_RTC_GetTimerContext(void)
{
    return context;
}

uint32_t HW_RTC_GetTimerElapsedTime(void)
{
    return HW_RTC_GetTimerValue() - HW_RTC_GetTimerContext();
}

void HW_RTC_DelayMs(uint32_t delay)
{
    rtt_printf(0, "Delay requested of : %ums\n", delay);
    TimerTime_t delayValue = 0;
    TimerTime_t timeout    = 0;

    delayValue = HW_RTC_ms2Tick(delay);

    if(!timer_started) {  // RTC must be running to poll it
        APP_ERROR_CHECK(app_timer_start(RTC_SX12, HW_RTC_GetMinimumTimeout(), NULL));
    }

    /* Wait delay ms */
    timeout = HW_RTC_GetTimerValue();
    while(((HW_RTC_GetTimerValue() - timeout)) < delayValue) {
        __NOP();
    }
}

void HW_RTC_StopAlarm(void)
{
    APP_ERROR_CHECK(app_timer_stop(RTC_SX12));
}

void HW_RTC_SetAlarm(uint32_t timeout)
{
    rtt_printf(0, "Alarm requested in : %ums\n", HW_RTC_Tick2ms(timeout));
    if(timer_running) {
        APP_ERROR_CHECK(0xDEADBEEF);  // Unsupported. Cannot only start one timer at a time.
    }
    // TODO If going to sleep mode during the alarm, substract wake-up time if noticeable
    APP_ERROR_CHECK(app_timer_start(RTC_SX12, timeout, NULL));
    timer_running = true;
    ;
    timer_started = true;
}

void HW_RTC_IrqHandler(void* p_context)
{
    bsp_board_led_invert(1);
    rtt_printf(0, "Alarm ended\n");
    timer_running = false;
    TimerIrqHandler();
}

void HW_RTC_Init(void)
{
    APP_ERROR_CHECK(app_timer_init());
    APP_ERROR_CHECK(app_timer_create(&RTC_SX12, APP_TIMER_MODE_SINGLE_SHOT, HW_RTC_IrqHandler));
}
