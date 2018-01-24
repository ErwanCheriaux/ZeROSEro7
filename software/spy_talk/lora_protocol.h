#ifndef LORA_PROTOCOL_H
#define LORA_PROTOCOL_H

#define YELLOW_ADDRESS 0x04
#define BLUE_ADDRESS 0x05
#define PINK_ADDRESS 0x06

#define LORA_PROTOCOL_MESSAGE_LENGTH 255

typedef lora_protocol_address_t uint8t_t;

void lora_protocol_init(lora_protocol_address_t address);
void lora_protocol_start();
void lora_protocol_handlers_init(
    void (*lora_protocol_receive_handler)(uint8_t sender_address, char * message); // Message ends with 0x00
);
void lora_protocol_send(lora_protocol_address_t address, char * message); // Message must end with 0x00

#endif
