#include "spim_protocol.h"
#include "nrf_spim.h"
#include <string.h>
#include "rtt.h"

#define END_OF_FILE ((uint8_t)0x00)
#define START_MESSAGE ((uint8_t*)"go")
#define STOP_MESSAGE ((uint8_t*)"ha")
#define NEXT_MESSAGE ((uint8_t*)"nx")

#define TRANSFER_SIZE (NRF_SDH_BLE_GATT_MAX_MTU_SIZE+2+1) // last byte for parity because STM reads by 16 bits

static uint8_t rx_buffer[TRANSFER_SIZE];  // SPI MISO
static uint8_t tx_buffer[TRANSFER_SIZE];  // SPI MOSI

void spim_protocol_init() {
    spim_init();
}

static buffer_t strip_buffer = {
    rx_buffer+2,
    SPIM_PROTOCOL_PACKET_SIZE
};

static uint8_t* ptr_end;
static buffer_t* detect_end_symbol() {
    strip_buffer.data = rx_buffer+2;
    ptr_end = rx_buffer+2;
    while(*ptr_end != END_OF_FILE || ptr_end == rx_buffer + 2 + SPIM_PROTOCOL_PACKET_SIZE) {
        ptr_end ++;
    }
    strip_buffer.length =(uint8_t)(ptr_end - (rx_buffer+2));
    return &strip_buffer;
}

buffer_t* spim_protocol_start() {
    memcpy(tx_buffer,START_MESSAGE,2);
    spim_transfer(rx_buffer,tx_buffer,TRANSFER_SIZE);
    rtt_write_string("Received via SPI :\n");
    rtt_write_buffer_hexa(rx_buffer,20);
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
