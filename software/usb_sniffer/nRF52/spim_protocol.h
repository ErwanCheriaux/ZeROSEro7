#ifndef SPIM_PROTOCOL_H
#define SPIM_PROTOCOL_H

#include <stdint.h>

/*
*   SPI Master control for handling our custom protocol.
*   We exchange 251 bytes packets of data via SPI between the nRF52 and STM32
*   The master starts the transmission with the "go" message and the slave responds with 251 bytes
*   The master then sends the "nx" message to receive the following packets.
*   The master can end the download with the "ha" message.
*   The slave can say it is the end of data with 0x00.
*
*   We chose 251 bytes because it is the max MTU for BLE on the nRF52
*/

// We use bloqcking spi transfers without much loss of performance
// Because of the length of BLE transaction

typedef struct {
    uint8_t* data;
    int length;
} buffer_t;

void spim_protocol_init();
buffer_t* spim_protocol_start();
buffer_t* spim_protocol_next();
void spim_protocol_stop();

#endif
