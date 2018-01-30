#ifndef SPIM_PROTOCOL_H
#define SPIM_PROTOCOL_H

#include <stdint.h>
#include "sdk_config.h"

/*
*   SPI Master control for handling our custom protocol.
*   We exchange 250 bytes packets of data via SPI between the nRF52 and STM32
*   The master starts the transmission with the "go" message and the slave responds with 250 bytes
*   The master then sends the "nx" message to receive the following packets.
*   The master can repeat a data with the "ha" message.
*   The slave can say it is the end of data with 0x00.
*
*   We chose 250 bytes because 251 is the max MTU for BLE on the nRF52.
*   Repeat functionnality is here in case the phone is disconnected during the transfer.
*/

// We use blocking spi transfers without much loss of performance
// Because of the length of BLE transaction

#define SPIM_PROTOCOL_PACKET_SIZE (NRF_SDH_BLE_GATT_MAX_MTU_SIZE - 1)  // 250

typedef struct {
    uint8_t* data;
    int      length;
} buffer_t;

void      spim_protocol_init();
void      spim_protocol_start();
buffer_t* spim_protocol_next();
void      spim_protocol_repeat();

#endif
