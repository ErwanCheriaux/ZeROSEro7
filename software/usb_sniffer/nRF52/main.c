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

#include "nrf_spim.h"

static uint8_t rx_buffer[4];                         // SPI MISO
static uint8_t tx_buffer[4] = {1, 2, 3, 4};          // SPI MOSI

static void log_init(void)
{
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
}

#define LED_PIN 22

static void sniffer_led_init() {
    static nrf_drv_gpiote_out_config_t led_config = {
        NRF_GPIOTE_POLARITY_TOGGLE,
        NRF_GPIOTE_INITIAL_VALUE_LOW,
        false};
    nrf_drv_gpiote_init();
    APP_ERROR_CHECK(nrf_drv_gpiote_out_init(LED_PIN, &led_config));
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

    ble_stack_init();

    sniffer_led_init();

    spim_init();
    rtt_write_string("SPI initialized\n");

    while(true) {
        spim_transfer(rx_buffer,tx_buffer,4);
        rtt_write_buffer_hexa(rx_buffer,4);
        rtt_write_string("\n");
        if(rx_buffer[0] == 5) {
            tx_buffer[0]++;
            rtt_printf(0,"tx[0] = %u", tx_buffer[0]);
        }
        nrf_drv_gpiote_out_toggle(LED_PIN);

        nrf_delay_ms(100);  // TODO Not tested yet
    }

    return 0;
}
