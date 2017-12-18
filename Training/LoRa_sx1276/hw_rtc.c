// Using app_timer which is capable of sufficient precision (max 32768 Hz)
#include "app_timer.h"
#include "sdk_config.h"
#include "boards.h"
#include "rtt.h"
#include "low_power.h"

#include "hw.h"
#include "utilities.h"
#include "timeServer.h"

APP_TIMER_DEF(RTC_SX12);    // Timer for Alarms
APP_TIMER_DEF(RTC_DELAY);   // Timer for Delays and Context

// Simple context lasting only a few days. Copy icube if a greater one is needed.
static volatile uint32_t context;
static volatile bool              timer_running = false;  // Indicates if Alarm is On (multiple starts not supported!)
static volatile uint32_t wakeup_time_ticks = 0;

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
    return APP_TIMER_MIN_TIMEOUT_TICKS; // 5 Ticks
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
    TimerTime_t delayValue = 0;
    TimerTime_t timeout    = 0;

    delayValue = HW_RTC_ms2Tick(delay);

    /* Wait delay ms */
    timeout = HW_RTC_GetTimerValue();
    while(((HW_RTC_GetTimerValue() - timeout)) < delayValue) {
    }
}

void HW_RTC_StopAlarm(void)
{
    timer_running     = false;
    APP_ERROR_CHECK(app_timer_stop(RTC_SX12));
}

void HW_RTC_SetAlarm(uint32_t timeout)
{
    if(timer_running) {
        APP_ERROR_CHECK(0xDEADBEEF);  // Unsupported. Can only start one alarm at a time.
    }
    // We always go to sleep after setting an alarm. (only used for rx/tx timeouts)
    timeout = timeout - wakeup_time_ticks;
    APP_ERROR_CHECK(app_timer_start(RTC_SX12, timeout, NULL));
    timer_running     = true;
}

void HW_RTC_IrqHandler(void* p_context)
{
    bsp_board_led_invert(1);
    timer_running = false;
    TimerIrqHandler();
}

/*
*   Measure wakeup time for more accurate timing.
*/
#define MEASUREMENT_TIME_TO_SLEEP 1000
static void compute_wakeup_handler(void* p_context) {
    if(wakeup_time_ticks == 0) {
        wakeup_time_ticks = HW_RTC_GetTimerElapsedTime() - MEASUREMENT_TIME_TO_SLEEP;
        app_timer_stop(RTC_DELAY);
        rtt_printf(0,"Wakeup ticks : %u\n", wakeup_time_ticks);
    }
}

static void compute_wakeup_time() {
    HW_RTC_SetTimerContext() ;
    APP_ERROR_CHECK(app_timer_start(RTC_DELAY, MEASUREMENT_TIME_TO_SLEEP, compute_wakeup_handler));
    while(wakeup_time_ticks == 0) {
        low_power_standby() ;
    }
}

void HW_RTC_Init(void)
{
    APP_ERROR_CHECK(app_timer_init());
    APP_ERROR_CHECK(app_timer_create(&RTC_SX12, APP_TIMER_MODE_SINGLE_SHOT, HW_RTC_IrqHandler));
    APP_ERROR_CHECK(app_timer_create(&RTC_DELAY, APP_TIMER_MODE_REPEATED, compute_wakeup_handler));

    compute_wakeup_time() ;

    APP_ERROR_CHECK(app_timer_start(RTC_DELAY, UINT32_MAX, NULL));  // Runs in the background so that RTC keeps runnning
}
