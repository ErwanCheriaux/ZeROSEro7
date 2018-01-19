//flash.c

#include "flash.h"

#define FLASH_ACR (*(volatile uint32_t *)0x40023c00)
#define FLASH_KEYR (*(volatile uint32_t *)0x40023c04)
#define FLASH_OPT_KEYR (*(volatile uint32_t *)0x40023c08)
#define FLASH_SR (*(volatile uint32_t *)0x40023c0c)
#define FLASH_CR (*(volatile uint32_t *)0x40023c10)
#define FLASH_OPT_CR (*(volatile uint32_t *)0x40023c14)

void flash_erase(void)
{
}

void flash_program(uint8_t data)
{
    (void)data;
}
