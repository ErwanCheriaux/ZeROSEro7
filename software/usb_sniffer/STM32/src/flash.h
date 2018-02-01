//flash.h

#ifndef FLASH_H
#define FLASH_H

#include "hal.h"

extern volatile char _keyboard_storage_start;
extern volatile char _keyboard_storage_end;

void flash_init(void);
void flash_lock(void);
void flash_unlock(void);
void flash_erase(void);
void flash_program(uint16_t *data, int size);
void flash_display(void);

#endif
