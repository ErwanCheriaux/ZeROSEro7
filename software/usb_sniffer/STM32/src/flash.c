//flash.c

#include "flash.h"
#include "rtt.h"

#define FLASH_KEY1 ((uint32_t)0x45670123U)
#define FLASH_KEY2 ((uint32_t)0xCDEF89ABU)

#define BSY (FLASH->SR & (1 << 16))
#define SIZE_BLOCK_MEMORIE 4
#define PSIZE 0

extern volatile char  _keyboard_storage_start;
extern volatile char  _keyboard_storage_end;
static volatile char *flash = &_keyboard_storage_start;

void flash_init(void)
{
    FLASH->CR = 0;  //PSIZE = 0 : x8
}

void flash_lock(void)
{
    //Wait BSY is cleared
    while(BSY)
        continue;
    FLASH->CR |= FLASH_CR_LOCK;
}

void flash_unlock(void)
{
    FLASH->KEYR = FLASH_KEY1;
    FLASH->KEYR = FLASH_KEY2;
}

void flash_erase(void)
{
    flash_unlock();

    //Check BSY
    while(BSY)
        continue;
    //Select sector and start erase
    rtt_printf("Flash erased !");
    FLASH->CR = FLASH_CR_SER | FLASH_CR_SNB_0 | FLASH_CR_SNB_1 | FLASH_CR_SNB_3 | FLASH_CR_STRT;  //SNB = 11
    //Wait BSY is cleared
    while(BSY)
        continue;

    flash_lock();
}

void flash_program(uint32_t data)
{
    flash_unlock();

    //Check BSY
    while(BSY)
        continue;
    //Set PG
    FLASH->CR |= FLASH_CR_PG;
    //write
    *flash = data;
    rtt_printf("Write %04x at address %08x [%04x]", data, flash, *flash);
    //loop memorie
    flash = flash + SIZE_BLOCK_MEMORIE;
    if(flash >= &_keyboard_storage_end)
        flash = &_keyboard_storage_start;
    //Wait BSY is cleared
    while(BSY)
        continue;

    flash_lock();
}

void flash_display(void)
{
    int            i = 0;
    volatile char *dst;

    rtt_printf("num\tvalue\taddress");

    for(dst = &_keyboard_storage_start; dst < &_keyboard_storage_end; dst = dst + SIZE_BLOCK_MEMORIE) {
        rtt_printf("%d\t%04x\t%08x", i++, *dst, dst);
        if(i > 10)
            break;
    }
}
