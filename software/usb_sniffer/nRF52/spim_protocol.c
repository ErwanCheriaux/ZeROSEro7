#include "spim_protocol.h"
#include "nrf_spim.h"
#include "sdk_config.h"
#include <string.h>
#include "rtt.h"

#define END_OF_FILE ((uint8_t)0x00)
#define START_MESSAGE ((uint8_t*)"go")
#define STOP_MESSAGE ((uint8_t*)"ha")
#define NEXT_MESSAGE ((uint8_t*)"nx")

#define PACKET_SIZE NRF_SDH_BLE_GATT_MAX_MTU_SIZE
#define TRANSFER_SIZE (NRF_SDH_BLE_GATT_MAX_MTU_SIZE+2)

static uint8_t rx_buffer[TRANSFER_SIZE];  // SPI MISO
static uint8_t tx_buffer[TRANSFER_SIZE];  // SPI MOSI

void spim_protocol_init() {
    spim_init();
}

static buffer_t strip_buffer = {
    rx_buffer+2,
    PACKET_SIZE
};

static buffer_t* detect_end_symbol() {
    // TODO actually look for END_OF_FILE
    return &strip_buffer;
}

buffer_t* spim_protocol_start() {
    memcpy(tx_buffer,START_MESSAGE,2);
    rtt_write_string("Sending via SPI :\n");
    rtt_write_buffer(0,tx_buffer,TRANSFER_SIZE);
    rtt_write_string("\n");
    spim_transfer(rx_buffer,tx_buffer,TRANSFER_SIZE);
    rtt_write_string("Received via SPI :\n");
    rtt_write_buffer(0,rx_buffer,TRANSFER_SIZE);
    rtt_write_string("\n");
    return detect_end_symbol();
}

buffer_t* spim_protocol_next() {
    memcpy(tx_buffer,NEXT_MESSAGE,2);
    spim_transfer(rx_buffer,tx_buffer,TRANSFER_SIZE);
    return detect_end_symbol();
}

void spim_protocol_stop() {
    memcpy(tx_buffer,STOP_MESSAGE,2);
    spim_transfer(rx_buffer,tx_buffer,TRANSFER_SIZE);
}
