//flash.h

#ifndef FLASH_H
#define FLASH_H

#include "hal.h"

void flash_erase(void);
void flash_program(uint8_t data);

#endif
