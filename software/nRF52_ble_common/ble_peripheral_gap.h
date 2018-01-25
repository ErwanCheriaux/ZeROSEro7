#ifndef BLE_PERIPHERAL_GAP_H
#define BLE_PERIPHERAL_GAP_H

/*
*   BLE Peripheral role. Handles advertising and connection negociation with the smartphone.
*   The device is programmed to listen for a specific code before bonding with a single phone.
*/

// Init from ble_central is required before this init
void ble_gap_init();
void ble_advertise_init(uint8_t app_id);  // app_id expected in Manufacturer data on both sides of the advertisement to distinguish devices and apps
void ble_conn_negociation_init();

// Advertises for 120 seconds
void ble_peripheral_start_advertising();
// No need to specificaly stop advertising. It will shut down itself after timeout
void ble_peripheral_stop_advertising();

#endif
