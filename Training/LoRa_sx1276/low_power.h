#ifndef LOW_POWER_H
#define LOW_POWER_H

/*
*   Defines low power mode for spy talk as constant latency.
*   The cost in power is justified by the need of real-time communication.
*/

// Enables constant latency mode
void low_power_init();

// Enters low power ON mode with constant latency untill next interrupt
// Each peripheral can be set to idle mode for lowre consumption
void low_power_standby();

#endif
