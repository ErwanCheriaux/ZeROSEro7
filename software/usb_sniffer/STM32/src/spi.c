//spi.c

#include "spi.h"
#include "rtt.h"
#include "hal.h"
#include "vectors.h"

#define RCC_AHB1_RSTR (*(volatile uint32_t *)0x40023810)
#define RCC_APB1_RSTR (*(volatile uint32_t *)0x40023820)
#define RCC_AHB1_ENR (*(volatile uint32_t *)0x40023830)
#define RCC_APB1_ENR (*(volatile uint32_t *)0x40023840)
#define RCC_AHB1_LPENR (*(volatile uint32_t *)0x40023850)
#define RCC_APB1_LPENR (*(volatile uint32_t *)0x40023860)

#define SPI3_CR1 (*(volatile uint32_t *)0x40003c00)
#define SPI3_CR2 (*(volatile uint32_t *)0x40003c04)
#define SPI3_SR (*(volatile uint32_t *)0x40003c08)
#define SPI3_DR (*(volatile uint32_t *)0x40003c0c)
#define SPI3_CRCPR (*(volatile uint32_t *)0x40003c10)
#define SPI3_RXCRCR (*(volatile uint32_t *)0x40003c14)
#define SPI3_TXCRCR (*(volatile uint32_t *)0x40003c18)
#define SPI3_I2SCFGR (*(volatile uint32_t *)0x40003c1c)
#define SPI3_I2SPR (*(volatile uint32_t *)0x40003c20)

#define TXE (SPI3_SR & (1 << 1))
#define RXNE (SPI3_SR & (1 << 0))

char *password;

/*
 * SPI TX and RX buffers.
 */
static uint32_t size_buffer;
static uint8_t  txbuf[512];
static uint8_t  rxbuf[512];

void spi_init(void)
{
    /*
   * SPI3 I/O pins setup.
   */
    palSetPadMode(GPIOC, GPIOC_SPI3_SCK, PAL_MODE_ALTERNATE(6));
    palSetPadMode(GPIOC, GPIOC_SPI3_MISO, PAL_MODE_ALTERNATE(6));
    palSetPadMode(GPIOC, GPIOC_SPI3_MOSI, PAL_MODE_ALTERNATE(6));
    palSetPadMode(GPIOA, GPIOA_SPI3_NSS, PAL_MODE_INPUT_PULLDOWN);

    /*
     * Clock
     */
    //GPIOA
    RCC_AHB1_RSTR |= (1 << 0);   //GPIOARST = 1
    RCC_AHB1_ENR |= (1 << 0);    //GPIOAEN = 1
    RCC_AHB1_LPENR |= (1 << 0);  //GPIOALPEN = 1
    //SPI3
    RCC_APB1_RSTR |= (1 << 15);   //SPI3RST = 1
    RCC_APB1_ENR |= (1 << 15);    //SPI3EN = 1
    RCC_APB1_LPENR |= (1 << 15);  //SPI3LPEN = 1

    SPI3_CR1 &= ~(1 << 6);  //SPE = 0  : Turn off SPI before config

    /* 1. Set the DFF bit to define 8- or 16-bit data frame format */
    SPI3_CR1 &= ~(1 << 11);  //DFF = 0 : 8bit

    /* 2. Select the CPOL and CPHA bits to define one of the four relationships between the
    data transfer and the serial clock (see Figure 248). For correct data transfer, the CPOL
    and CPHA bits must be configured in the same way in the slave device and the master
    device. This step is not required when the TI mode is selected through the FRF bit in
    the SPI_CR2 register. */
    SPI3_CR1 &= ~(1 << 0);  //CPHA = 0
    SPI3_CR1 &= ~(1 << 1);  //CPOL = 0

    /*3. The frame format (MSB-first or LSB-first depending on the value of the LSBFIRST bit in
    the SPI_CR1 register) must be the same as the master device. This step is not
    required when TI mode is selected. */
    SPI3_CR1 &= ~(1 << 7);  //LSBFIRST = 0 : MSB-first

    /* 4. In Hardware mode (refer to Slave select (NSS) pin management on page 880), the
    NSS pin must be connected to a low level signal during the complete byte transmit
    sequence. In NSS software mode, set the SSM bit and clear the SSI bit in the SPI_CR1
    register. This step is not required when TI mode is selected. */
    //SPI3_CR1 |= (1 << 9);   //SSM = 1
    //SPI3_CR1 &= ~(1 << 8);  //SSI = 0

    SPI3_CR1 &= ~(1 << 9);  //SSM = 0 : NSS Hardware

    /* 5. Set the FRF bit in the SPI_CR2 register to select the TI mode protocol for serial
    communications. */
    SPI3_CR2 &= ~(1 << 4);  //FRF = 0 : Motorola mode

    //turn on Rx interrupt
    SPI3_CR2 |= (1 << 6);  //RXNEIE = 1

    /* 6. Clear the MSTR bit and set the SPE bit (both in the SPI_CR1 register) to assign the
    pins to alternate functions. */
    SPI3_CR1 &= ~(1 << 2);  //MSTR = 0 : Slave mode
    SPI3_CR1 |= (1 << 6);   //SPE = 1  : Turn on SPI

    //debug
    spi_display_config();
}

void spi_display_config(void)
{
    rtt_printf("===== SPI config =====");
    rtt_printf("RCC_AHB1_RSTR  = %08x", RCC_AHB1_RSTR);
    rtt_printf("RCC_APB1_RSTR  = %08x", RCC_APB1_RSTR);
    rtt_printf("RCC_AHB1_ENR   = %08x", RCC_AHB1_ENR);
    rtt_printf("RCC_APB1_ENR   = %08x", RCC_APB1_ENR);
    rtt_printf("RCC_AHB1_LPENR = %08x", RCC_AHB1_LPENR);
    rtt_printf("RCC_APB1_LPENR = %08x", RCC_APB1_LPENR);
    rtt_printf("");
    rtt_printf("SPI3_CR1       = %08x", SPI3_CR1);
    rtt_printf("SPI3_CR2       = %08x", SPI3_CR2);
    rtt_printf("SPI3_SR        = %08x", SPI3_SR);
    rtt_printf("SPI3_DR        = %08x", SPI3_DR);
    rtt_printf("SPI3_CRCPR     = %08x", SPI3_CRCPR);
    rtt_printf("SPI3_RXCRCR    = %08x", SPI3_RXCRCR);
    rtt_printf("SPI3_TXCRCR    = %08x", SPI3_TXCRCR);
    rtt_printf("SPI3_I2SCFGR   = %08x", SPI3_I2SCFGR);
    rtt_printf("SPI3_I2SPR     = %08x", SPI3_I2SPR);
    rtt_printf("");
}

void spi_write(char *msg)
{
    size_buffer = sizeof msg;
    if(size_buffer > 512)
        size_buffer = 512;

    //push to send buffer
    for(uint8_t i = 0; i < size_buffer; i++)
        txbuf[i]  = (uint8_t)msg[i];

    //turn on Tx interrupt
    SPI3_CR2 |= (1 << 7);  //TXEIE = 1
}

void SPI_IRQHandler(void)
{
    //Receive buffer not empty
    if(RXNE) {
        rxbuf[1] = SPI3_DR;
        rtt_printf("SPI receive: %08x", rxbuf[1]);
    }

    //Transfer buffer empty
    else if(TXE) {
        static uint32_t index = 0;

        if(index >= size_buffer) {
            //turn off Tx interrupt
            SPI3_CR2 &= ~(1 << 7);  //TXEIE = 0
            index = 0;
        } else {
            //write data in DR buffer
            SPI3_DR = txbuf[index];
            index++;
        }
    }
}
