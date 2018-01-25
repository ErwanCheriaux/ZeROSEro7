#include "rtt.h"
#include "nrf_log.h"

#include "lora.h"
#include "sx1276Regs-LoRa.h"
#include "lora_protocol.h"
#include "board.h"

#define LORA_RX_FRAME 8000  // ms
#define HEADER_LENGTH 2     // bytes
#define PAYLOAD_LENGTH (LORA_PROTOCOL_MESSAGE_LENGTH + HEADER_LENGTH)

static lora_protocol_address_t local_address;
static uint8_t                 tx_buffer[PAYLOAD_LENGTH];
static uint8_t                 tx_length;  // Only one send at a time. The application doesn't allow multiple writes.
static void (*lora_on_receive)(uint8_t sender_address, uint8_t* message, unsigned int length);

void lora_protocol_handlers_init(
    void (*lora_protocol_receive_handler)(uint8_t sender_address, uint8_t* message, unsigned int length)  // Message ends with 0x00
    )
{
    lora_on_receive = lora_protocol_receive_handler;
}

static void lora_tx_timeout_handler()
{
    // Should never happen given the size of packets
    rtt_write_string("TX timeout\n");
}

static void lora_callback();

static void lora_rx_error_handler()
{
    // Unhandled
}

static void lora_tx_done_handler()
{
    tx_length = 0;
    lora_callback();
}

static void lora_rx_timeout_handler()
{
    lora_callback();
}

static void lora_rx_done_handler(uint8_t* payload, uint16_t size, int16_t rssi, int8_t snr)
{
    led_toggle(2);
    if(payload[0] == local_address || payload[0] == BROADCAST_ADDRESS) {
        lora_on_receive(payload[1], payload + HEADER_LENGTH, size - HEADER_LENGTH);
#ifdef PARROT
        lora_protocol_send(payload[1], payload + HEADER_LENGTH, size - HEADER_LENGTH);
#endif
    }
#ifndef PARROT
    lora_callback();
#endif
}

static void lora_callback()
{
    Radio.Rx(LORA_RX_FRAME);
    rtt_write_string("LoRa Receiving\n");
    led_off(0);
}

void lora_protocol_send(lora_protocol_address_t address, uint8_t* message, unsigned int length)
{
    if(tx_length) {
        rtt_write_string("Multiple LoRa send unsupported; Message ignored\n");  // Cannot be reached when using the spy talk app.
    }

    tx_length    = MIN(length + HEADER_LENGTH, PAYLOAD_LENGTH);
    tx_buffer[0] = address;
    tx_buffer[1] = local_address;
    memcpy(HEADER_LENGTH + tx_buffer, message, tx_length - HEADER_LENGTH);
    Radio.Send(tx_buffer, tx_length);
    rtt_write_string("LoRa Sending");
    led_on(0);
}

static RadioEvents_t RadioEvents = {
    lora_tx_done_handler,
    lora_tx_timeout_handler,
    lora_rx_done_handler,
    lora_rx_timeout_handler,
    lora_rx_error_handler,
    NULL,  // Frequency Hopping handler
    NULL   // CAD done handler
};

void lora_protocol_init(lora_protocol_address_t address)
{
    local_address = address;
    lora_init(&RadioEvents);
}

void lora_protocol_start()
{
    Radio.Rx(LORA_RX_FRAME);
}
