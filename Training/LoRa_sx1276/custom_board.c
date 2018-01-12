#ifndef TEST_BOARD

#include "custom_board.h"
#include "nrf_drv_gpiote.h"

static nrf_drv_gpiote_out_config_t led_config = {
    NRF_GPIOTE_POLARITY_TOGGLE,
    NRF_GPIOTE_INITIAL_VALUE_HIGH,
    false
};

void leds_init() {
    nrf_drv_gpiote_init();

    APP_ERROR_CHECK(nrf_drv_gpiote_out_init(LEDB_PIN, &led_config));
    APP_ERROR_CHECK(nrf_drv_gpiote_out_init(RGBR_PIN, &led_config));
    APP_ERROR_CHECK(nrf_drv_gpiote_out_init(RGBG_PIN, &led_config));
    APP_ERROR_CHECK(nrf_drv_gpiote_out_init(RGBB_PIN, &led_config));
}

void led_on(int i) {
    // on at init, no toggle for now
}

#endif
