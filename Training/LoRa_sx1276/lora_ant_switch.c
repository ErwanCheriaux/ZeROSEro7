#include "nrf_drv_gpiote.h"

#include "lora_ant_switch.h"

void SX1276AntSwInit( void ) {
    if(!nrf_drv_gpiote_is_init())
        APP_ERROR_CHECK(nrf_drv_gpiote_init()) ;
}

static void SX1276SetXO( uint8_t state );
static uint32_t SX1276GetWakeTime( void );
static void SX1276IoIrqInit( DioIrqHandler **irqHandlers );
static void SX1276SetAntSwLowPower( bool status );
static void SX1276SetRfTxPower( int8_t power );
static void SX1276SetAntSw( uint8_t opMode );

LoRaBoardCallback_t lora_ant_switch_callbacks = {
  SX1276SetXO,
  SX1276GetWakeTime,
  SX1276IoIrqInit,
  SX1276SetRfTxPower,
  SX1276SetAntSwLowPower,
  SX1276SetAntSw
};
