#ifndef BLE_PERIPHERAL_GAP_H
#define BLE_PERIPHERAL_GAP_H

/*
*   BLE Peripheral role. Handles advertising and connection negociation with the smartphone.
*   The device is programmed to listen for a specific code before bonding with a single phone.
*/

void ble_advertising_handler_init(void (*phone_connected_handler)());

// Init from ble_central is required before this init
void ble_gap_init();
void ble_advertise_init();
void ble_conn_negociation_init();

void ble_peripheral_start_advertising();
void ble_peripheral_stop_advertising();

#endif