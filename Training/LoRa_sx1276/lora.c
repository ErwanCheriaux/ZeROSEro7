#include "rtt.h"

#include "hw.h"
#include "radio.h"
#include "sx1276.h"

#include "lora_ant_switch.h"

#define RF_FREQUENCY                868000000 // Hz
#define RX_TIMEOUT_VALUE            10000     // ms
#define LORA_BANDWIDTH              0         // [0: 125 kHz,
                                              //  1: 250 kHz,
                                              //  2: 500 kHz,
                                              //  3: Reserved]
#define LORA_SPREADING_FACTOR       7         // [SF7..SF12]
#define LORA_CODINGRATE             1         // [1: 4/5,
                                              //  2: 4/6,
                                              //  3: 4/7,
                                              //  4: 4/8]
#define LORA_PREAMBLE_LENGTH        8         // Same for Tx and Rx
#define LORA_SYMBOL_TIMEOUT         5         // Symbols
#define LORA_FIX_LENGTH_PAYLOAD_ON  false
#define LORA_FIX_LENGTH_PAYLOAD     0
#define LORA_IQ_INVERSION_ON        false
#define LORA_FREQUENCY_HOPPING_ON   0
#define LORA_HOPPING_PERIOD         0         // Symbols
#define LORA_CRC_ON                 true

#define LORA_TX_TIMEOUT             10000     // ms
#define LORA_TX_POWER               14        // dBm

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
    rtt_write_string("Received a LoRa payload :)\n") ;
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

    Radio.SetRxConfig( MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
                                     LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
                                     LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
                                     LORA_FIX_LENGTH_PAYLOAD, LORA_CRC_ON, LORA_FREQUENCY_HOPPING_ON,
                                     LORA_HOPPING_PERIOD, LORA_IQ_INVERSION_ON, false
                     );

     Radio.SetTxConfig( MODEM_LORA, LORA_TX_POWER, 0,
                                      LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
                                      LORA_CODINGRATE, LORA_PREAMBLE_LENGTH,
                                      LORA_FIX_LENGTH_PAYLOAD_ON, LORA_CRC_ON, LORA_FREQUENCY_HOPPING_ON,
                                      LORA_HOPPING_PERIOD, LORA_IQ_INVERSION_ON, LORA_TX_TIMEOUT
                      );
}

void lora_observe() {
    Radio.Rx(RX_TIMEOUT_VALUE);
}

void lora_send(uint8_t * buffer, unsigned int size) {
    Radio.Send(buffer, size);
}

bool SX1276CheckRfFrequency(uint32_t frequency) {
    if(frequency == RF_FREQUENCY) {
        return true ;
    }
    return false ;
}
