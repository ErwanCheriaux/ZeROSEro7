#include "rtt.h"

#include "hw.h"
#include "radio.h"
#include "sx1276.h"
#include "delay.h"

#include "lora_board.h"

#define RF_FREQUENCY 868300000   // Hz
#define RX_TIMEOUT_VALUE 8000    // ms
#define LORA_BANDWIDTH 0         /* [0: 125 kHz, \
                                     1: 250 kHz, \
                                     2: 500 kHz, \
                                     3: Reserved] */
#define LORA_SPREADING_FACTOR 12 // [SF7..SF12]
#define LORA_CODINGRATE 4        /* [1: 4/5, \
                                     2: 4/6, \
                                     3: 4/7, \
                                     4: 4/8] */
#define LORA_PREAMBLE_LENGTH 8   // Same for Tx and Rx
#define LORA_SYMBOL_TIMEOUT 5    // Symbols
#define LORA_FIX_LENGTH_PAYLOAD_ON false
#define LORA_FIX_LENGTH_PAYLOAD 0
#define LORA_IQ_INVERSION_ON false
#define LORA_FREQUENCY_HOPPING_ON 0
#define LORA_HOPPING_PERIOD 0  // Symbols
#define LORA_CRC_ON true
#define LORA_RX_CONTINUOUS_ON true  // Repeat after symbol timeout

#define LORA_TX_TIMEOUT 30000 // ms
#define LORA_TX_POWER 14      // dBm. max 14 without PA BOOST

bool SX1276CheckRfFrequency(uint32_t frequency);

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
        SX1276GetRadioWakeUpTime};

static void onTxDone()
{
    rtt_write_string(" -> Tx done\n");
}

static void onTxTimeout()
{
    rtt_write_string(" -> Tx timeout\n");
}

static void onRxTimeout()
{
    rtt_write_string(" -> Rx timeout\n");
    Radio.Standby();
}

static void onRxError()
{
    rtt_write_string(" -> Rx error\n");
}

static void OnRxDone(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr)
{
    rtt_write_string(" -> Rx done\nData : ");
    rtt_write_buffer(0, payload, size);
    rtt_printf(0, "\nRssiValue=%d dBm, SnrValue=%d\n", rssi, snr);
    DelayMs(RX_TIMEOUT_VALUE-1000);
    Radio.Standby();
}

static RadioEvents_t RadioEvents;
void                 lora_init()
{
    Radio.IoInit();
    SX1276AntSwInit();
    SX1276BoardInit(&lora_board_callbacks);

    RadioEvents.TxDone    = onTxDone;
    RadioEvents.RxDone    = OnRxDone;
    RadioEvents.TxTimeout = onTxTimeout;
    RadioEvents.RxTimeout = onRxTimeout;
    RadioEvents.RxError   = onRxError;

    Radio.Init(&RadioEvents);
    Radio.SetChannel(RF_FREQUENCY);

    Radio.SetRxConfig(MODEM_LORA, LORA_BANDWIDTH,
                      LORA_SPREADING_FACTOR, LORA_CODINGRATE,
                      0, LORA_PREAMBLE_LENGTH,
                      LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
                      LORA_FIX_LENGTH_PAYLOAD,
                      LORA_CRC_ON, LORA_FREQUENCY_HOPPING_ON, LORA_HOPPING_PERIOD,
                      LORA_IQ_INVERSION_ON, LORA_RX_CONTINUOUS_ON);

    Radio.SetTxConfig(MODEM_LORA, LORA_TX_POWER, 0,
                      LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
                      LORA_CODINGRATE, LORA_PREAMBLE_LENGTH,
                      LORA_FIX_LENGTH_PAYLOAD_ON, LORA_CRC_ON, LORA_FREQUENCY_HOPPING_ON,
                      LORA_HOPPING_PERIOD, LORA_IQ_INVERSION_ON, LORA_TX_TIMEOUT);
}

void lora_observe()
{
    Radio.Rx(RX_TIMEOUT_VALUE);
}

void lora_send(uint8_t *buffer, unsigned int size)
{
    Radio.Send(buffer, size);
}

void lora_clear_irq()
{
    Radio.Write(REG_LR_IRQFLAGS, 0xff);
}

bool SX1276CheckRfFrequency(uint32_t frequency)
{
    return true;
}
