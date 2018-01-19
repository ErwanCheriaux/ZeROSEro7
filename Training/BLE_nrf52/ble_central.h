#ifndef BLE_H
#define BLE_H

#include <stdint.h>

/*
*   BLE Central role functions. More consumming than peripheral
*   but required to scan.
*/

void ble_handler_init(
    void (*phone_noticed_handler)(),
    void (*phone_connected_handler)(),
    void (*phone_disconnected_handler)(),
    void (*phone_write_handler)(uint8_t *buff, int length));
void ble_stack_init();
void ble_start_observing();
void ble_stop_observing();

// Handle to latest connection, or BLE_CONN_HANDLE_INVALID if not connected.
// TODO ensure only one connection at a time
extern volatile uint16_t ble_central_latest_conn;

#endif
