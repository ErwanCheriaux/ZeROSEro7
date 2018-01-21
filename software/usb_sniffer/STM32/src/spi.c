//spi.c

#include "spi.h"
#include "rtt.h"
#include "hal.h"
#include "vectors.h"

#define TXE (SPI3->SR & SPI_SR_TXE)
#define RXNE (SPI3->SR & SPI_SR_RXNE)

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
     * CONFIG SPI
     * 8bit
     * CPHA = 0, CPOL = 0
     * MSB-first
     * NSS Hardware
     * Motorola mode
     * Slave mode
     * Rx interupt on
     */

    RCC->APB1ENR |= RCC_APB1ENR_SPI3EN;  // Turn SPI3 clock on.
    SPI3->CR1 = 0;                       // SPI off while configuring it
    SPI3->CR2 = SPI_CR2_RXNEIE;          // RX interrupt if data present
    SPI3->CR1 = SPI_CR1_SPE;             // Enable SPI in slave mode.

    //debug
    spi_display_config();
}

void spi_display_config(void)
{
    rtt_printf("===== SPI config =====");
    rtt_printf("SPI3_CR1       = %08x", SPI3->CR1);
    rtt_printf("SPI3_CR2       = %08x", SPI3->CR2);
    rtt_printf("SPI3_SR        = %08x", SPI3->SR);
    rtt_printf("SPI3_DR        = %08x", SPI3->DR);
    rtt_printf("SPI3_CRCPR     = %08x", SPI3->CRCPR);
    rtt_printf("SPI3_RXCRCR    = %08x", SPI3->RXCRCR);
    rtt_printf("SPI3_TXCRCR    = %08x", SPI3->TXCRCR);
    rtt_printf("SPI3_I2SCFGR   = %08x", SPI3->I2SCFGR);
    rtt_printf("SPI3_I2SPR     = %08x", SPI3->I2SPR);
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
    SPI3->CR2 |= SPI_CR2_TXEIE;  //TXEIE = 1
}

void SPI_IRQHandler(void)
{
    rtt_printf("SPI_IRQHandler");
    //Receive buffer not empty
    if(RXNE) {
        rxbuf[1] = SPI3->DR;
        rtt_printf("SPI receive: %08x", rxbuf[1]);
    }

    //Transfer buffer empty
    else if(TXE) {
        static uint32_t index = 0;

        if(index >= size_buffer) {
            //turn off Tx interrupt
            SPI3->CR2 &= SPI_CR2_TXEIE;  //TXEIE = 0
            index = 0;
        } else {
            //write data in DR buffer
            SPI3->DR = txbuf[index];
            index++;
        }
    }
}
