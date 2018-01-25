#include <stdbool.h>
#include <stdint.h>
#include "ble_uart_service.h"
#include "ble_central.h"
#include "ble_peripheral_gap.h"
#include "ble_peripheral_gatt.h"
#include "rtt.h"
#include "nrf_drv_gpiote.h"
#include "nrf_delay.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "spim_protocol.h"

#define USB_SNIFFER_APP_ID 0x02

static buffer_t* spim_received_buffer;
static bool      spim_transfer_ongoing;

static void log_init(void)
{
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
}

#define LED_PIN 22

static void sniffer_led_init()
{
    static nrf_drv_gpiote_out_config_t led_config = {
        NRF_GPIOTE_POLARITY_TOGGLE,
        NRF_GPIOTE_INITIAL_VALUE_LOW,
        false};
    nrf_drv_gpiote_init();
    APP_ERROR_CHECK(nrf_drv_gpiote_out_init(LED_PIN, &led_config));
}

/*
    BLE Handlers
*/

static void phone_noticed_handler()
{
    rtt_write_string("Phone get, negociating connection\n");
    // We advertise to switch role in the GAP connection for lower consumption.
    ble_stop_observing();
    ble_peripheral_start_advertising();
}

static void phone_connected_handler()
{
    rtt_write_string("Phone connected\n");
    ble_peripheral_stop_advertising();
}

static void phone_disconnected_handler()
{
    rtt_write_string("Phone disconnected\n");
    ble_peripheral_stop_advertising();
    ble_start_observing();
}

static void phone_write_handler(uint8_t* buff, int length)
{
    rtt_write_string("Received data from phone :\n");
    rtt_write_buffer(0, buff, length);
    rtt_write_string("\n");

    spim_protocol_start();
    spim_transfer_ongoing = true;
    spim_received_buffer  = spim_protocol_next();
    if(spim_received_buffer->length < SPIM_PROTOCOL_PACKET_SIZE) {
        spim_transfer_ongoing = false;
    }
    rtt_write_string("Sending to phone :\n");
    rtt_write_buffer(0,spim_received_buffer->data, MIN(40,spim_received_buffer->length));
    rtt_write_string("\n");
    phone_send_notification(spim_received_buffer->data, spim_received_buffer->length);
}

static void phone_notification_complete_handler()
{
    if(spim_transfer_ongoing) {
        spim_received_buffer = spim_protocol_next();
        if(spim_received_buffer->length < SPIM_PROTOCOL_PACKET_SIZE) {
            spim_transfer_ongoing = false;
        }
        phone_send_notification(spim_received_buffer->data, spim_received_buffer->length);
    }
}

/*
    Spi master test code for communicating with the STM.

    SPI Parameters are :
    CPOL = 0 and CPHA = 0 in Motorola/Freescale nomenclature.
    4 MHz Frequency

    SPI_SCK_PIN     P6
    SPI_MISO_PIN    P7
    SPI_MOSI_PIN    P8
    SPI_SS_PIN      P5

    Every 100ms, it transfers 1,2,3,4. SPI_SS is held high when not transmitting.
    If 5,X,X,X is received during the transfer,
    the data emitted is incremented.
*/
int main(void)
{
    rtt_init();
    rtt_write_string("RTT initialized\n");

    log_init();
    NRF_LOG_INFO("\n\n======== DEBUG INITIALIZED ========\n");

    ble_handler_init(phone_noticed_handler, phone_connected_handler, phone_disconnected_handler, phone_write_handler, phone_notification_complete_handler);
    ble_stack_init(USB_SNIFFER_APP_ID);
    ble_gap_init();
    ble_gatt_init();
    ble_advertise_init(USB_SNIFFER_APP_ID);
    ble_services_init();
    ble_conn_negociation_init();
    rtt_write_string("BLE initialized\n");
    ble_start_observing();
    ble_peripheral_start_advertising();  // TODO remove. Convenient for debugging purpose
    rtt_write_string("Now observing BLE\n");

    sniffer_led_init();

    spim_protocol_init();
    rtt_write_string("SPI initialized\n");

    static buffer_t* spim_buff;
    while(true) {
        sd_app_evt_wait();
        /*
        nrf_drv_gpiote_out_toggle(LED_PIN);
        spim_protocol_start();
        rtt_write_string("SPI START\n");
        do {
            spim_buff = spim_protocol_next();
            rtt_printf(0, "SPI response length :%u\n", spim_buff->length);
            nrf_delay_ms(300);
        } while(spim_buff->length == SPIM_PROTOCOL_PACKET_SIZE);*/
    }

    return 0;
}
