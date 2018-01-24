#include "spim_protocol.h"
#include "nrf_spim.h"
#include <string.h>
#include "rtt.h"
#include "nrf_delay.h"

#define END_OF_FILE ((uint8_t)0x00)
#define START_MESSAGE ((uint8_t*)"go")
#define REPEAT_MESSAGE ((uint8_t*)"ha")
#define NEXT_MESSAGE ((uint8_t*)"nx")

#define TRANSFER_SIZE (SPIM_PROTOCOL_PACKET_SIZE + 1)  // last byte for parity because STM reads by 16 bits

static uint8_t rx_buffer[TRANSFER_SIZE];  // SPI MISO
static uint8_t tx_buffer[TRANSFER_SIZE];  // SPI MOSI

void spim_protocol_init()
{
    spim_init();
}

static buffer_t message_buffer = {
    rx_buffer,
    SPIM_PROTOCOL_PACKET_SIZE};

static uint8_t*  ptr_end;
static buffer_t* detect_end_symbol()
{
    message_buffer.data = rx_buffer;
    ptr_end             = rx_buffer;
    while(*ptr_end != END_OF_FILE) {
        if(ptr_end == rx_buffer + SPIM_PROTOCOL_PACKET_SIZE) {
            break;
        }
        ptr_end++;
    }
    message_buffer.length = (uint8_t)(ptr_end - (rx_buffer));
    return &message_buffer;
}

static void prepare_tx_buffer(uint8_t* command)
{
    memcpy(tx_buffer + TRANSFER_SIZE - 2, command, 2);
}

void spim_protocol_start()
{
    prepare_tx_buffer(START_MESSAGE);
    spim_transfer(rx_buffer, tx_buffer, TRANSFER_SIZE);
    nrf_delay_ms(10);  // Workaround on Nordic SDK erroneous transfer size.

    // Workaround for erroneous STM32 behavior FIXME
    //nrf_delay_ms(50);
    //spim_protocol_next();
    //nrf_delay_ms(50);
}

buffer_t* spim_protocol_next()
{
    prepare_tx_buffer(NEXT_MESSAGE);
    spim_transfer(rx_buffer, tx_buffer, TRANSFER_SIZE);
    rtt_write_string("SPI NEXT :\n");
    rtt_write_buffer_hexa(rx_buffer, 40);
    rtt_write_string("\n");
    return detect_end_symbol();
}

void spim_protocol_repeat()
{
    prepare_tx_buffer(REPEAT_MESSAGE);
    spim_transfer(rx_buffer, tx_buffer, TRANSFER_SIZE);
}
