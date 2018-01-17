#ifndef LORA_H
#define LORA_H

#include "radio.h"

/*
*   SX1276 high level control. Check out radio.h for a list of implemented functions.
*/

void lora_init(RadioEvents_t* RadioEvents);

extern const struct Radio_s Radio;

/*!
 * \brief Checks if the given RF frequency is supported by the hardware
 *
 * \param [IN] frequency RF frequency to be checked
 * \retval isSupported [true: supported, false: unsupported]
 */
bool SX1276CheckRfFrequency(uint32_t frequency);

#endif
