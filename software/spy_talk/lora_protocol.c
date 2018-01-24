#include "lora.h"
#include "sx1276Regs-LoRa.h"
#include "lora_protocol.h"

#define LORA_RX_TIMEOUT 8000  // ms

static lora_protocol_address_t local_address;

static void (*lora_on_receive)(uint8_t sender_address, char * message);

void lora_protocol_handlers_init(
    void (*lora_protocol_receive_handler)(uint8_t sender_address, char * message); // Message ends with 0x00
) {
    lora_on_receive = lora_protocol_receive_handler;
}

static void lora_tx_done_handler()
{

}

static void lora_tx_timeout_handler()
{

}

static void lora_rx_timeout_handler()
{

}

static void lora_rx_error_handler()
{

}

static void lora_rx_done_handler(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr)
{

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

void lora_protocol_init(lora_protocol_address_t address) {
    local_address = address;
    lora_init(&RadioEvents);
}

void lora_protocol_start() {
    // TODO
}
