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

static uint8_t rx_buffer[4];                 // SPI MISO
static uint8_t tx_buffer[4] = {1, 2, 3, 4};  // SPI MOSI

#define STEALTH_DROP_APP_ID 0x03

static void log_init(void)
{
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
}

#define LED_PIN 19
#define STM_WKUP_PIN 8

static void wakeup_stm_init()
{
    static nrf_drv_gpiote_out_config_t stm_wkup_config = {
        NRF_GPIOTE_POLARITY_TOGGLE,
        NRF_GPIOTE_INITIAL_VALUE_LOW,
        false};
    APP_ERROR_CHECK(nrf_drv_gpiote_out_init(STM_WKUP_PIN, &stm_wkup_config));
}

static void wakup_stm_toggle()
{
    nrf_drv_gpiote_out_toggle(STM_WKUP_PIN);
}

static void stealth_drop_led_init()
{
    static nrf_drv_gpiote_out_config_t led_config = {
        NRF_GPIOTE_POLARITY_TOGGLE,
        NRF_GPIOTE_INITIAL_VALUE_HIGH,
        false};
    APP_ERROR_CHECK(nrf_drv_gpiote_out_init(LED_PIN, &led_config));
}

static void do_nothing()
{
}

static void phone_noticed_handler()
{
    rtt_write_string("Phone detected, waking up the STM\n");
    wakup_stm_toggle();
}

static void phone_write_handler(uint8_t* buff, int length)
{
}

int main(void)
{
    rtt_init();
    rtt_write_string("RTT initialized\n");

    log_init();
    NRF_LOG_INFO("\n\n======== DEBUG INITIALIZED ========\n");

    nrf_drv_gpiote_init();
    stealth_drop_led_init();
    wakeup_stm_init();

    ble_handler_init(phone_noticed_handler, do_nothing, do_nothing, phone_write_handler, do_nothing);
    ble_stack_init(STEALTH_DROP_APP_ID);
    ble_gap_init();
    ble_gatt_init();
    ble_advertise_init(STEALTH_DROP_APP_ID);
    ble_services_init();
    ble_conn_negociation_init();
    ble_start_observing();
    rtt_write_string("BLE observing\n");

    while(true) {
        sd_app_evt_wait();
    }

    return 0;
}
