#ifndef BLE_UART_SERVICE_H
#define BLE_UART_SERVICE_H

#include <stdint.h>

#include "ble.h"
#include "ble_srv_common.h"
#include "nrf_sdh_ble.h"
#include "sdk_config.h"
#include "nrf_log.h"
#include "mem_manager.h"

void uart_service_init();
void phone_send_notification(uint8_t* buff, int length);  // Up to 512 bytes

extern void* ble_uart_characteristic_value;

typedef struct {
    uint8_t                  uuid_type;
    uint16_t                 service_handle;
    uint16_t                 conn_handle;
    ble_gatts_char_handles_t char_handles;
} ble_ms_t;

extern ble_ms_t uart_characteristic_config;

#endif
