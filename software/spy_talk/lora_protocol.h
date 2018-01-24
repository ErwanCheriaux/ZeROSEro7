#ifndef LORA_PROTOCOL_H
#define LORA_PROTOCOL_H

#include "sdk_config.h"

#define YELLOW_ADDRESS 0x04
#define BLUE_ADDRESS 0x05
#define PINK_ADDRESS 0x06

#define LORA_PROTOCOL_MESSAGE_LENGTH NRF_SDH_BLE_GATT_MAX_MTU_SIZE

/*
    LoRa protocol implementation

    Packets format is:
        8bit receiver_addr
        8bit sender_addr

        50bit message
*/

typedef uint8_t lora_protocol_address_t;

void lora_protocol_init(lora_protocol_address_t address);
void lora_protocol_start();
void lora_protocol_handlers_init(
    void (*lora_protocol_receive_handler)(uint8_t sender_address, uint8_t* message, unsigned int length);  // Message ends with 0x00
    );
void lora_protocol_send(lora_protocol_address_t address, uint8_t* message, unsigned int length);  // Message must end with 0x00

#endif
