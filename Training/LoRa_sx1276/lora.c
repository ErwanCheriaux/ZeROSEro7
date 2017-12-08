#include "rtt.h"

#include "hw.h"
#include "radio.h"
#include "sx1276.h"

#include "lora_ant_switch.h"

#define RF_FREQUENCY 868000000 // Hz

static bool SX1276CheckRfFrequency(uint32_t frequency) ;

const struct Radio_s Radio =
{
    SX1276IoInit,
    SX1276IoDeInit,
    SX1276Init,
    SX1276GetStatus,
    SX1276SetModem,
    SX1276SetChannel,
    SX1276IsChannelFree,
    SX1276Random,
    SX1276SetRxConfig,
    SX1276SetTxConfig,
    SX1276CheckRfFrequency,
    SX1276GetTimeOnAir,
    SX1276Send,
    SX1276SetSleep,
    SX1276SetStby,
    SX1276SetRx,
    SX1276StartCad,
    SX1276SetTxContinuousWave,
    SX1276ReadRssi,
    SX1276Write,
    SX1276Read,
    SX1276WriteBuffer,
    SX1276ReadBuffer,
    SX1276SetMaxPayloadLength,
    SX1276SetPublicNetwork,
    SX1276GetRadioWakeUpTime
};

static void handler_print() {
    rtt_write_string("Unhandled Radio event\n") ;
}

static void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr ) {
    rtt_write_string("Unhandled Radio event\n") ;
}

static RadioEvents_t RadioEvents;
void lora_init() {
    Radio.IoInit() ;
    SX1276AntSwInit() ;
    SX1276BoardInit(&lora_ant_switch_callbacks) ;

    RadioEvents.TxDone = handler_print;
    RadioEvents.RxDone = OnRxDone;
    RadioEvents.TxTimeout = handler_print;
    RadioEvents.RxTimeout = handler_print;
    RadioEvents.RxError = handler_print;

    Radio.Init(&RadioEvents) ;
    Radio.SetChannel( RF_FREQUENCY ) ;
}

/*!
 * \brief Checks if the given RF frequency is supported by the hardware
 *
 * \param [IN] frequency RF frequency to be checked
 * \retval isSupported [true: supported, false: unsupported]
 */
static bool SX1276CheckRfFrequency(uint32_t frequency) {
    APP_ERROR_CHECK(0xDEADBEEF) ; // Unsupported
    return false ;
}
