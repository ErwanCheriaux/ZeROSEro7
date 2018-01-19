//flash.h

#ifndef FLASH_H
#define FLASH_H

#include "hal.h"

void flash_init(void);
void flash_lock(void);
void flash_unlock(void);
void flash_erase(void);
void flash_program(uint8_t data);
void flash_display(void);

#endif
