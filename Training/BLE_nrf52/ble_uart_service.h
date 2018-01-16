#ifndef BLE_UART_SERVICE_H
#define BLE_UART_SERVICE_H

void uart_service_init();
void phone_send_notification(uint8_t *buff, int length); // Up to 512 bytes

extern void*  ble_uart_characteristic_value ;

#endif
