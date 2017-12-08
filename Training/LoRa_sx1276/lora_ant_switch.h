#ifndef LORA_ANT_SWITCH_H
#define LORA_ANT_SWITCH_H value

#include "hw.h"
#include "radio.h"
#include "sx1276.h"

/*!
 * \brief Controls the antena switch if necessary.
 *
 * \remark see errata note
 *
 * \param [IN] opMode Current radio operating mode
 */
extern LoRaBoardCallback_t lora_ant_switch_callbacks;

void SX1276AntSwInit() ;

/*!
 * \brief Set XO state on the board
 */
void SX1276BoardSetXO(uint8_t state );
/*!
 * \brief Get Board Wake Up time
 */
void SX1276BoardGetWakeTime( void );
/*!
 * \brief Initializes the radio I/Os Irq
 */
void ( *SX1276BoardIoIrqInit )( DioIrqHandler **irqHandlers );
/*!
 * \brief Sets the radio output power.
 *
 * \param [IN] power Sets the RF output power
 */
void SX1276BoardSetRfTxPower( int8_t power );
/*!
 * \brief Set the RF Switch I/Os pins in Low Power mode
 *
 * \param [IN] status enable or disable
 */
void SX1276BoardSetAntSwLowPower( bool status );
/*!
 * \brief Controls the antena switch if necessary.
 *
 * \remark see errata note
 *
 * \param [IN] opMode Current radio operating mode
 */
void SX1276BoardSetAntSw( uint8_t opMode );

#endif
