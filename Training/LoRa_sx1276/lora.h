#ifndef LORA_H
#define LORA_H

#include "radio.h"

extern const struct Radio_s Radio ;

void lora_init() ;
void lora_observe() ;

/*!
 * \brief Checks if the given RF frequency is supported by the hardware
 *
 * \param [IN] frequency RF frequency to be checked
 * \retval isSupported [true: supported, false: unsupported]
 */
bool SX1276CheckRfFrequency(uint32_t frequency) ;

#endif
