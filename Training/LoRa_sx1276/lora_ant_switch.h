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

#endif
