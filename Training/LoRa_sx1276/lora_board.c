#include "nrf_drv_gpiote.h"
#include "app_error.h"

#include "lora_board.h"
#include "lora.h"
#include "hw.h"

// Inspired by sx1276_lrwan_ns1.c, the most similar board to our sx1276 schematic

void SX1276IoInit()
{
    if(!nrf_drv_gpiote_is_init())
        APP_ERROR_CHECK(nrf_drv_gpiote_init());
}

void SX1276IoDeInit()
{
    APP_ERROR_CHECK(0xDEADBEEF);  // Unsupported. TODO free io channels, rtc to sleep and spi maybe
}

void SX1276AntSwInit(void)
{
}

// No X0 pin on SX1276
void SX1276SetXO(uint8_t state)
{
}

uint32_t SX1276GetWakeTime()
{
    return 0;  //TODO
}

static const nrf_drv_gpiote_in_config_t dio_in_config = {
    true,   // High accuracy
    false,  // Tracker mode
    NRF_GPIO_PIN_PULLDOWN,
    NRF_GPIOTE_POLARITY_LOTOHI};

void SX1276IoIrqInit(DioIrqHandler **irqHandlers)
{
    nrf_drv_gpiote_in_init(SX1276_DIO0_PIN, &dio_in_config, (nrf_drv_gpiote_evt_handler_t)irqHandlers[0]);
    nrf_drv_gpiote_in_init(SX1276_DIO1_PIN, &dio_in_config, (nrf_drv_gpiote_evt_handler_t)irqHandlers[1]);
    nrf_drv_gpiote_in_init(SX1276_DIO2_PIN, &dio_in_config, (nrf_drv_gpiote_evt_handler_t)irqHandlers[2]);
    nrf_drv_gpiote_in_init(SX1276_DIO3_PIN, &dio_in_config, (nrf_drv_gpiote_evt_handler_t)irqHandlers[3]);
    nrf_drv_gpiote_in_init(SX1276_DIO4_PIN, &dio_in_config, (nrf_drv_gpiote_evt_handler_t)irqHandlers[4]);
    nrf_drv_gpiote_in_init(SX1276_DIO5_PIN, &dio_in_config, (nrf_drv_gpiote_evt_handler_t)irqHandlers[5]);

    nrf_drv_gpiote_in_event_enable(SX1276_DIO0_PIN, true);
    nrf_drv_gpiote_in_event_enable(SX1276_DIO1_PIN, true);
    nrf_drv_gpiote_in_event_enable(SX1276_DIO2_PIN, true);
    nrf_drv_gpiote_in_event_enable(SX1276_DIO3_PIN, true);
    nrf_drv_gpiote_in_event_enable(SX1276_DIO4_PIN, true);
    nrf_drv_gpiote_in_event_enable(SX1276_DIO5_PIN, true);
}

void SX1276SetAntSwLowPower(bool status)
{
}

// No power amplification connected
static uint8_t paConfig;
void SX1276SetRfTxPower(int8_t power)
{
    paConfig = SX1276Read(REG_PACONFIG);
    if((paConfig & RF_PACONFIG_PASELECT_PABOOST) == RF_PACONFIG_PASELECT_PABOOST) {
        APP_ERROR_CHECK(0xDEADBEEF);  // Unsupported Power Amplification
    }
}

// No external RXTX_EXT control
void SX1276SetAntSw(uint8_t opMode)
{
}

LoRaBoardCallback_t lora_board_callbacks = {
    SX1276SetXO,
    SX1276GetWakeTime,
    SX1276IoIrqInit,
    SX1276SetRfTxPower,
    SX1276SetAntSwLowPower,
    SX1276SetAntSw};
