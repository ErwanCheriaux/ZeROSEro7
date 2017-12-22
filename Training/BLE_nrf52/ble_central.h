#ifndef BLE_H
#define BLE_H

/*
*   BLE Central role functions. More consumming than peripheral
*   but required to scan.
*/

void ble_init(void (*phone_noticed_handler)());
void ble_start_observing();
void ble_stop_observing();

#endif
