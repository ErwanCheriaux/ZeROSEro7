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

extern char           _keyboard_storage_start;
extern char           _keyboard_storage_end;
static volatile char *flash = &_keyboard_storage_start;

void flash_init(void)
{
    //size x32
    FLASH_CR |= (2 << 8);   //PSIZE
    FLASH_CR &= ~(2 << 8);  //PSIZE

    flash_unlock();
}

void flash_lock(void)
{
    FLASH_CR |= (1 << 31);
}

void flash_unlock(void)
{
    FLASH_KEYR = 0x45670123;
    FLASH_KEYR = 0xcdef89ab;
}

void flash_erase(void)
{
    //Check BSY
    while(BSY)
        ;

    //Select sector
    FLASH_CR |= (1 << 1);   //SER
    FLASH_CR |= (4 << 3);   //SNB
    FLASH_CR &= ~(4 << 3);  //SNB

    //Start erase
    FLASH_CR |= (1 << 16);
    rtt_printf("Flash erase !");

    //Wait BSY is cleared
    while(BSY)
        ;
}

void flash_program(uint8_t data)
{
    //Check BSY
    while(BSY)
        ;

    //Set PG
    FLASH_CR |= (1 << 0);

    //write
    *flash = data;

    rtt_printf("Write %08x at address %08x [%08x]", data, flash, *flash);

    if(flash < &_keyboard_storage_end)
        flash++;

    //Wait BSY is cleared
    while(BSY)
        ;
}

void flash_display(void)
{
    int   i = 0;
    char *dst;

    rtt_printf("num\t\tvalue\t\t\taddress");

    for(dst = &_keyboard_storage_start; dst < &_keyboard_storage_end; dst++)
        rtt_printf("%d\t%032x\t%032x", i++, *dst, dst);
}
