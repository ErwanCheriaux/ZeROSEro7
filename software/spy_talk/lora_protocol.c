#include "rtt.h"
#include "nrf_log.h"
#include "app_timer.h"
#include "nrf_delay.h"

#include "lora.h"
#include "sx1276Regs-LoRa.h"
#include "lora_protocol.h"
#include "board.h"
#include <stdint.h>

static void protocol_main_callback();

/*
    Typedefs
*/

typedef enum {
    PROTOCOL_RECEIVER_SLEEP,        // 0
    PROTOCOL_RECEIVER_WAKEUP,       // 1
    PROTOCOL_RECEIVER_READY,        // 2
    PROTOCOL_RECEIVER_DATA,         // 3
    PROTOCOL_RECEIVER_ACKNOWLEDGE,  // 4
    PROTOCOL_SENDER_WAKEUP,         // 5
    PROTOCOL_SENDER_READY,          // 6
    PROTOCOL_SENDER_DATA,           // 7
    PROTOCOL_SENDER_ACKNOWLEDGE     // 8
} protocol_state_t;

typedef enum {
    PROTOCOL_TX_DONE,    // 0
    PROTOCOL_RX_FAILED,  // 1
    PROTOCOL_RX_DONE,    // 2
    PROTOCOL_SLEEP_DONE  // 3
} protocol_event_type_t;

// Given the event type, not all fields hold meaningfull data
typedef struct {
    protocol_event_type_t event_type;
    uint8_t               packet_type;
    uint8_t               sender_address;
    uint8_t               receiver_address;
    uint8_t               message[MAX_PAYLOAD_LENGTH];
    unsigned int          length;
} protocol_evt_t;

static protocol_evt_t event_build;

/*
    Application provided
*/

static uint8_t local_address;  // Our address

// Outgoing transfer data
// Only one send at a time. The application doesn't allow multiple writes as it waits for ack or timeout.
static uint8_t outgoing_buffer[MAX_PAYLOAD_LENGTH];  // Buffer to store the data to send
static uint8_t outgoing_address;
static uint8_t outgoing_length;

// Buffer used to store incomming data. Required to delay tx_success after ack is send. Caused problem for broadcasts and parrot.
static uint8_t incomming_buffer[MAX_PAYLOAD_LENGTH];
static uint8_t incomming_address;
static uint8_t incomming_length;

// Handlers provided by main.c
static void (*lora_on_receive)(uint8_t sender_address, uint8_t* message, unsigned int length);
static void (*lora_on_ack)(uint8_t receiver_address);
static void (*lora_on_tx_fail)(uint8_t receiver_address);

/*
    Packet formatting and Radio control
*/

#define PACKET_WKP 0x0
#define PACKET_RDY 0x1
#define PACKET_DAT 0x2
#define PACKET_ACK 0x3

static uint8_t      tx_buffer[MAX_PAYLOAD_LENGTH];
static uint8_t      tx_length;
static uint8_t      tx_type;
static unsigned int tx_addr;
static void radio_send(uint8_t address, uint8_t type, uint8_t* msg, unsigned int length)
{
    led_on(2);

    tx_addr = address;
    tx_type = type;

    if(type == PACKET_WKP) {
        tx_length    = 0;
        tx_buffer[0] = address;
        tx_buffer[1] = local_address;
        Radio.Send(tx_buffer, WKP_PACKET_LENGTH);

    } else {
        nrf_delay_ms(5);  // Wait for the other end to go in RX.
        length    = MIN(length, LORA_PROTOCOL_MESSAGE_MAX_LENGTH);
        tx_length = length;

        tx_buffer[0] = type;
        tx_buffer[1] = address;
        tx_buffer[2] = local_address;
        memcpy(tx_buffer + HEADER_LENGTH, msg, length);

        Radio.Send(tx_buffer, tx_length + HEADER_LENGTH);
    }
}

static protocol_evt_t parse_packet(uint8_t* payload, uint16_t size)
{
    event_build.event_type = PROTOCOL_RX_DONE;

    if(size == WKP_PACKET_LENGTH) {
        event_build.packet_type      = PACKET_WKP;
        event_build.receiver_address = payload[0];
        event_build.sender_address   = payload[1];
        event_build.length           = 0;
    } else {
        event_build.packet_type      = payload[0];
        event_build.receiver_address = payload[1];
        event_build.sender_address   = payload[2];

        memcpy(event_build.message, payload + HEADER_LENGTH, size - HEADER_LENGTH);
        event_build.length = size - HEADER_LENGTH;
    }

    return event_build;
}

static void radio_receive(uint32_t timeout_ms)
{
    Radio.Rx(timeout_ms);
    led_on(3);
}

APP_TIMER_DEF(RTC_LORA_SLEEP);

void end_of_nap_handler()
{
    event_build.event_type = PROTOCOL_SLEEP_DONE;
    protocol_main_callback(event_build);
}

static void sleep_timer_init()
{
    APP_ERROR_CHECK(app_timer_init());
    APP_ERROR_CHECK(app_timer_create(&RTC_LORA_SLEEP, APP_TIMER_MODE_SINGLE_SHOT, end_of_nap_handler));
}

static void radio_sleep(uint32_t time_ms)
{
    APP_ERROR_CHECK(app_timer_start(RTC_LORA_SLEEP, APP_TIMER_TICKS(time_ms), NULL));

    Radio.Standby();  // Sleep and Stby have very close consumption. Sleep is intended for longer periods of inactivity
}

static void sleep_stop()
{
    APP_ERROR_CHECK(app_timer_stop(RTC_LORA_SLEEP));
}

/*
    Internal state machine
*/

#define WKP_RETRY_ATTEMPTS 10

#define MAX_DATA_RX_FRAME 8000       // ms
#define SMALL_PACKETS_RX_FRAME 1000  // ms  // RDY, DAT, ACK
#define WAKEUP_PACKET_RX_FRAME 2250  // ms // TODO Reduce  // WKP  // Must be at least double the WKP packet time to ensure first wkup is catched
#define SLEEP_FRAME 3000             // ms // Approximative 5s worse case latency

static volatile protocol_state_t state = PROTOCOL_RECEIVER_SLEEP;  // REVIEW volatile ?

static void lora_tx_done_handler()
{
    led_off(2);

    event_build.event_type       = PROTOCOL_TX_DONE;
    event_build.sender_address   = local_address;
    event_build.receiver_address = tx_addr;
    event_build.packet_type      = tx_type;
    if(tx_type == PACKET_DAT && tx_length) {
        memcpy(event_build.message, tx_buffer + HEADER_LENGTH, tx_length);
    }
    event_build.length = tx_length;

    protocol_main_callback(event_build);
}

static void lora_tx_timeout_handler()
{
    // Should never happen. Yet happens due to an error in SDK. See their note in sx1276.c
    // Has no effet; The tx is done anyway. Error would be caught in reception error eventually.
    rtt_write_string("TX timeout\n");
    lora_tx_done_handler();
}

static void lora_rx_timeout_handler()
{
    event_build.event_type = PROTOCOL_RX_FAILED;
    led_off(3);

    protocol_main_callback(event_build);
}

static void lora_rx_error_handler()
{
    led_off(3);
    lora_rx_timeout_handler();
}

static void lora_rx_done_handler(uint8_t* payload, uint16_t size, int16_t rssi, int8_t snr)
{
    led_off(3);
    event_build = parse_packet(payload, size);

    if(event_build.receiver_address == local_address || event_build.receiver_address == BROADCAST_ADDRESS) {
        event_build.event_type = PROTOCOL_RX_DONE;
    } else {
        event_build.event_type = PROTOCOL_RX_FAILED;  // Packet is not for us
        rtt_write_string("Packet isn't for us.\n");
    }
    protocol_main_callback(event_build);
}

static unsigned int number_of_wkp_tries;
static void protocol_main_callback(protocol_evt_t event)
{
    rtt_printf(0, "LoRa state : %u LoRa event : %u\n", state, event.event_type);

    switch(state) {
        case PROTOCOL_RECEIVER_SLEEP:
            // Try to receive a wakeup
            radio_receive(WAKEUP_PACKET_RX_FRAME);
            state = PROTOCOL_RECEIVER_WAKEUP;
            break;

        case PROTOCOL_RECEIVER_WAKEUP:
            if(event.event_type == PROTOCOL_RX_DONE && event.packet_type == PACKET_WKP) {
                state = PROTOCOL_RECEIVER_READY;
                radio_send(event.sender_address, PACKET_RDY, tx_buffer, 0);
            } else {
                radio_sleep(SLEEP_FRAME);
                state = PROTOCOL_RECEIVER_SLEEP;
            }
            break;

        case PROTOCOL_RECEIVER_READY:
            switch(event.event_type) {
                case PROTOCOL_TX_DONE:
                    state = PROTOCOL_RECEIVER_DATA;
                    radio_receive(MAX_DATA_RX_FRAME);
                    break;
                default:
                    radio_sleep(SLEEP_FRAME);
                    state = PROTOCOL_RECEIVER_SLEEP;
                    break;
            }
            break;

        case PROTOCOL_RECEIVER_DATA:
            if(event.event_type == PROTOCOL_RX_DONE && event.packet_type == PACKET_DAT) {
                state = PROTOCOL_RECEIVER_ACKNOWLEDGE;
                radio_send(event.sender_address, PACKET_ACK, tx_buffer, 0);
                incomming_length  = event.length;
                incomming_address = event.sender_address;
                memcpy(incomming_buffer, event.message, event.length);
            } else {
                radio_sleep(SLEEP_FRAME);
                state = PROTOCOL_RECEIVER_SLEEP;
            }
            break;

        case PROTOCOL_RECEIVER_ACKNOWLEDGE:
            lora_on_receive(incomming_address, incomming_buffer, incomming_length);
#ifndef PARROT
            radio_sleep(SLEEP_FRAME);
            state = PROTOCOL_RECEIVER_SLEEP;
#endif  // PARROT
            break;

        case PROTOCOL_SENDER_WAKEUP:
            switch(event.event_type) {
                case PROTOCOL_TX_DONE:
                    state = PROTOCOL_SENDER_READY;
                    radio_receive(SMALL_PACKETS_RX_FRAME);
                    break;
                default:
                    radio_sleep(SLEEP_FRAME);
                    state           = PROTOCOL_RECEIVER_SLEEP;
                    outgoing_length = 0;
                    lora_on_tx_fail(event.receiver_address);
                    break;
            }
            break;

        case PROTOCOL_SENDER_READY:
            if(event.event_type == PROTOCOL_RX_DONE && event.sender_address == outgoing_address) {
                state = PROTOCOL_SENDER_DATA;
                radio_send(outgoing_address, PACKET_DAT, outgoing_buffer, outgoing_length);
            } else {
                if(number_of_wkp_tries < WKP_RETRY_ATTEMPTS) {
                    number_of_wkp_tries++;
                    state = PROTOCOL_SENDER_WAKEUP;
                    radio_send(outgoing_address, PACKET_WKP, outgoing_buffer, 0);
                } else {
                    radio_sleep(SLEEP_FRAME);
                    state           = PROTOCOL_RECEIVER_SLEEP;
                    outgoing_length = 0;
                    lora_on_tx_fail(event.receiver_address);
                }
            }
            break;

        case PROTOCOL_SENDER_DATA:
            switch(event.event_type) {
                case PROTOCOL_TX_DONE:
                    state = PROTOCOL_SENDER_ACKNOWLEDGE;
                    radio_receive(SMALL_PACKETS_RX_FRAME);
                    break;
                default:
                    radio_sleep(SLEEP_FRAME);
                    state           = PROTOCOL_RECEIVER_SLEEP;
                    outgoing_length = 0;
                    lora_on_tx_fail(event.receiver_address);
                    break;
            }
            break;

        case PROTOCOL_SENDER_ACKNOWLEDGE:
            if(event.event_type == PROTOCOL_RX_DONE && event.sender_address == outgoing_address && event.packet_type == PACKET_ACK) {
                lora_on_ack(event.sender_address);
            } else {
                lora_on_tx_fail(event.receiver_address);
            }
            radio_sleep(SLEEP_FRAME);
            state           = PROTOCOL_RECEIVER_SLEEP;
            outgoing_length = 0;
            break;
    }
}

/*
    Exported functions
*/

void lora_protocol_send(uint8_t address, uint8_t* message, unsigned int length)
{
    if(outgoing_length || length == 0 || state == PROTOCOL_RECEIVER_READY || state == PROTOCOL_RECEIVER_DATA || state == PROTOCOL_RECEIVER_ACKNOWLEDGE) {
        rtt_write_string("Multiple LoRa operation unsupported; Message discarded.\n");  // Cannot be reached when using the spy talk app.
        lora_on_tx_fail(address);
    } else {
        sleep_stop();  // FIXME Simultaneous sleep_done could happen. At worse, the outgoing procedure is discarded so tx_failed handler will not be called. App could handle it with a timeout.
        number_of_wkp_tries = 0;
        outgoing_length     = length;
        memcpy(outgoing_buffer, message, length);
        outgoing_address = address;

        radio_send(address, PACKET_WKP, tx_buffer, 0);
        state = PROTOCOL_SENDER_WAKEUP;
    }
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

void lora_protocol_handlers_init(
    void (*lora_protocol_receive_handler)(uint8_t sender_address, uint8_t* message, unsigned int length),
    void (*lora_protocol_ack_handler)(uint8_t receiver_address),
    void (*lora_protocol_tx_failed_handler)(uint8_t receiver_address))
{
    lora_on_receive = lora_protocol_receive_handler;
    lora_on_ack     = lora_protocol_ack_handler;
    lora_on_tx_fail = lora_protocol_tx_failed_handler;
}

void lora_protocol_init(uint8_t address)
{
    local_address = address;
    lora_init(&RadioEvents);

    sleep_timer_init();
}

void lora_protocol_start()
{
    state                  = PROTOCOL_RECEIVER_SLEEP;
    event_build.event_type = PROTOCOL_SLEEP_DONE;
    protocol_main_callback(event_build);
}
