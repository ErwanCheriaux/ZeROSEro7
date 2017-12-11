#include "nrf_drv_gpiote.h"
#include "app_error.h"

#include "lora_ant_switch.h"
#include "lora.h"

void SX1276AntSwInit(void)
{
}

// TODO Inspired by sx1276_lrwan_ns1.c or the most similar board to our sx1276 montage

// No X0 pin on SX1276
void SX1276SetXO(uint8_t state)
{
}

uint32_t SX1276GetWakeTime()
{
    return 0;
}

// DIO pins unused
void SX1276IoIrqInit(DioIrqHandler **irqHandlers)
{
}

// TODO for low power, release GPIO channels, SPI, ...
void SX1276SetAntSwLowPower(bool status)
{
}

// No power amplification connected
static uint8_t paConfig;
void           SX1276SetRfTxPower(int8_t power)
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

LoRaBoardCallback_t lora_ant_switch_callbacks = {
    SX1276SetXO,
    SX1276GetWakeTime,
    SX1276IoIrqInit,
    SX1276SetRfTxPower,
    SX1276SetAntSwLowPower,
    SX1276SetAntSw};
