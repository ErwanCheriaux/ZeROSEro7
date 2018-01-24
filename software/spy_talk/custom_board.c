#ifndef TEST_BOARD

#include "custom_board.h"
#include "nrf_drv_gpiote.h"

static nrf_drv_gpiote_out_config_t led_config = {
    NRF_GPIOTE_POLARITY_TOGGLE,
    NRF_GPIOTE_INITIAL_VALUE_HIGH,
    false};

void leds_init()
{
    nrf_drv_gpiote_init();

    APP_ERROR_CHECK(nrf_drv_gpiote_out_init(LEDB_PIN, &led_config));
    APP_ERROR_CHECK(nrf_drv_gpiote_out_init(RGBR_PIN, &led_config));
    APP_ERROR_CHECK(nrf_drv_gpiote_out_init(RGBG_PIN, &led_config));
    APP_ERROR_CHECK(nrf_drv_gpiote_out_init(RGBB_PIN, &led_config));
}

void led_on(int i)
{
    switch(i) {
        case 0:
            nrf_drv_gpiote_out_clear(LEDB_PIN);
            break;
        case 1:
            nrf_drv_gpiote_out_clear(RGBR_PIN);
            break;
        case 2:
            nrf_drv_gpiote_out_clear(RGBG_PIN);
            break;
        case 3:
            nrf_drv_gpiote_out_clear(RGBB_PIN);
            break;
    }
}

void led_off(int i)
{
    switch(i) {
        case 0:
            nrf_drv_gpiote_out_set(LEDB_PIN);
            break;
        case 1:
            nrf_drv_gpiote_out_set(RGBR_PIN);
            break;
        case 2:
            nrf_drv_gpiote_out_set(RGBG_PIN);
            break;
        case 3:
            nrf_drv_gpiote_out_set(RGBB_PIN);
            break;
    }
}

#endif
