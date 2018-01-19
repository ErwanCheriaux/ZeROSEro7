//flash.c

#include "flash.h"
#include "rtt.h"

#define FLASH_ACR (*(volatile uint32_t *)0x40023c00)
#define FLASH_KEYR (*(volatile uint32_t *)0x40023c04)
#define FLASH_OPT_KEYR (*(volatile uint32_t *)0x40023c08)
#define FLASH_SR (*(volatile uint32_t *)0x40023c0c)
#define FLASH_CR (*(volatile uint32_t *)0x40023c10)
#define FLASH_OPT_CR (*(volatile uint32_t *)0x40023c14)

#define BSY (FLASH_SR & (1 << 16))
#define SIZE_BLOCK_MEMORIE 4

extern volatile uint32_t  _keyboard_storage_start;
extern volatile uint32_t  _keyboard_storage_end;
static volatile uint32_t *flash = &_keyboard_storage_start;

void flash_init(void)
{
    //size x32
    FLASH_CR |= (2 << 8);   //PSIZE
    FLASH_CR &= ~(2 << 8);  //PSIZE
}

void flash_lock(void)
{
    //Wait BSY is cleared
    while(BSY) continue;
    FLASH_CR |= (1 << 31);
}

void flash_unlock(void)
{
    FLASH_KEYR = 0x45670123;
    FLASH_KEYR = 0xcdef89ab;
}

void flash_erase(void)
{
    flash_unlock();

    //Check BSY
    while(BSY) continue;
    //Select sector
    FLASH_CR |= (1 << 1);    //SER
    FLASH_CR |= (11 << 3);   //SNB
    FLASH_CR &= ~(11 << 3);  //SNB
    //Start erase
    FLASH_CR |= (1 << 16);
    rtt_printf("Flash erased !");
    //Wait BSY is cleared
    while(BSY) continue;

    flash_lock();
}

void flash_program(uint32_t data)
{
    flash_unlock();

    //Check BSY
    while(BSY) continue;
    //Set PG
    FLASH_CR |= (1 << 0);
    //write
    *flash = data;
    rtt_printf("Write %04x at address %08x [%04x]", data, flash, *flash);
    //loop memorie
    flash = flash + SIZE_BLOCK_MEMORIE;
    if(flash >= &_keyboard_storage_end)
        flash = &_keyboard_storage_start;
    //Wait BSY is cleared
    while(BSY) continue;

    flash_lock();
}

void flash_display(void)
{
    int                i = 0;
    volatile uint32_t *dst;

    rtt_printf("num\tvalue\taddress");

    for(dst = &_keyboard_storage_start; dst < &_keyboard_storage_end; dst = dst + SIZE_BLOCK_MEMORIE) {
        rtt_printf("%d\t%04x\t%08x", i++, *dst, dst);
        if(i > 10)
            break;
    }
}
