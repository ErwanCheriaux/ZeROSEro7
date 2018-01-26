#ifndef LORA_PROTOCOL_H
#define LORA_PROTOCOL_H

#include "sdk_config.h"

#define YELLOW_ADDRESS 0x04
#define BLUE_ADDRESS 0x05
#define PINK_ADDRESS 0x06

#define BROADCAST_ADDRESS 0x0F

#define LORA_PROTOCOL_MESSAGE_MAX_LENGTH 58
#define HEADER_LENGTH 3      // bytes
#define WKP_PACKET_LENGTH 2  // Must be unique. No other packet can have this size.
#define MAX_PAYLOAD_LENGTH (LORA_PROTOCOL_MESSAGE_MAX_LENGTH + HEADER_LENGTH)

/*
    LoRa custom protocol implementation

    Packets format is:
        8bit packet_type
        8bit receiver_addr
        8bit sender_addr

        dynamic, up to LORA_PROTOCOL_MESSAGE_MAX_LENGTH bytes

    Exception with the wakeup packet :
        8bit receiver_addr
        8bit sender_addr
    Because it needs to be very small for low power

    Encryption is done by the phone on the message. Addresses are in clear for low power

    Low power is done by putting the radio in sleep most of the time.
    A wakeup is sent containing only the address of the receiver (small packet)
    repeatedly during the sleep period. The receiver replies by a ready signal on
    reception and the transfer of the packet starts between the two peers only.
    At the end, receiver replies with an acknowledge.

    // TODO for broadcasts, receivers responses are ommitted because they would scramble.
    The wakeup signal is sent several times nonetheless to be sure to wakeup everybody.
    // TODO an advertising channel and switching to channel[addr] for less collisions
    // TODO Frequency hopping

    length is adapted for maximum 3s TX
*/

void lora_protocol_init(uint8_t address);
void lora_protocol_start();
void lora_protocol_handlers_init(
    void (*lora_protocol_receive_handler)(uint8_t sender_address, uint8_t* message, unsigned int length),
    void (*lora_protocol_ack_handler)(uint8_t receiver_address),
    void (*lora_protocol_tx_failed_handler)(uint8_t receiver_address));
void lora_protocol_send(uint8_t address, uint8_t* message, unsigned int length);

#endif
