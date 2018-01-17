#ifndef LOW_POWER_H
#define LOW_POWER_H

/*
*   Defines low power mode for spy talk.
*   At the cost of 1µA, system is kept in ON mode to save 30 ms wakeup time.
*   Spy Talk should allow low latency.
*   Constant latency mode isn't required because it only saves a few µs.
*/

// Enters low power ON mode until next interrupt. Low latency.
// Each peripheral can be set to idle mode for lower consumption
void low_power_standby();

// Goes to power OFF mode. High latency, lower consumption.
// Equivalent to a RESET. Exited only when DETECT or RESET.
// TODO Device must not be in debug mode. It is only emulated in debug.
void low_power_deep_sleep();

#endif
