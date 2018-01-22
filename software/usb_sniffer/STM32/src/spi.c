//spi.c

#include "spi.h"
#include "rtt.h"
#include "vectors.h"

#define OVR (SPI3->SR & SPI_SR_OVR)
#define TXE (SPI3->SR & SPI_SR_TXE)
#define RXNE (SPI3->SR & SPI_SR_RXNE)

/*
 * Mail Box
 */
#define MB_SIZE 8

static msg_t mb_buffer[MB_SIZE];
static MAILBOX_DECL(mb, mb_buffer, MB_SIZE);

/*
 * SPI TX buffers.
 */
#define BF_SIZE 500

static uint16_t msg_size;
static uint16_t txbuf[BF_SIZE];

void spi_init(void)
{
    /*
     * Mail box
     */
    chMBObjectInit(&mb, mb_buffer, MB_SIZE);

    /*
     * SPI3 I/O pins setup.
     */
    palSetPadMode(GPIOC, GPIOC_SPI3_SCK, PAL_MODE_ALTERNATE(6));
    palSetPadMode(GPIOC, GPIOC_SPI3_MISO, PAL_MODE_ALTERNATE(6));
    palSetPadMode(GPIOC, GPIOC_SPI3_MOSI, PAL_MODE_ALTERNATE(6));
    palSetPadMode(GPIOA, GPIOA_SPI3_NSS, PAL_MODE_INPUT_PULLDOWN);

    /*
     * CONFIG SPI
     * 16bit
     * CPHA = 0, CPOL = 0
     * MSB-first
     * NSS Hardware
     * Motorola mode
     * Slave mode
     * Rx interupt on
     */

    RCC->APB1ENR |= RCC_APB1ENR_SPI3EN;     // Turn SPI3 clock on.
    SPI3->CR1 = 0;                          // SPI off while configuring it
    SPI3->CR2 = SPI_CR2_RXNEIE;             // RX interrupt if data present
    SPI3->CR1 = SPI_CR1_SPE | SPI_CR1_DFF;  // Enable SPI in slave mode.

    //enable interrupt
    NVIC->ISER[1] = (1 << 19);  // Position 51
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

void spi_mailbox_refresh(void)
{
    msg_t msg;
    for(int i = 0; i < chMBGetUsedCountI(&mb); i++) {
        chMBFetch(&mb, &msg, TIME_INFINITE);
        rtt_printf("Mai box[%d]: %04x", i, msg);
    }
}

void spi_write(uint16_t *msg, int n)
{
    //verif msg size
    if(n > BF_SIZE) {
        rtt_printf("Can't send msg too long...");
        rtt_printf("%d > %d", n, BF_SIZE);
        return;
    }

    msg_size = n;

    //push to send buffer
    for(uint8_t i = 0; i < msg_size; i++)
        txbuf[i]  = (uint16_t)msg[i];

    //turn on Tx interrupt
    SPI3->CR2 |= SPI_CR2_TXEIE;  //TXEIE = 1
}

void SPI_IRQHandler(void)
{
    //Overrun flag
    if(OVR) {
        rtt_printf("Overrun !!!");
        chSysLockFromISR();
        chMBPostI(&mb, SPI3->DR);
        chSysUnlockFromISR();
    }
    //Receive buffer not empty
    else if(RXNE) {
        chSysLockFromISR();
        chMBPostI(&mb, SPI3->DR);
        chSysUnlockFromISR();
    }

    //Transfer buffer empty
    else if(TXE) {
        static uint32_t index = 0;

        if(index >= msg_size) {
            //turn off Tx interrupt
            SPI3->CR2 &= ~SPI_CR2_TXEIE;  //TXEIE = 0
            index = 0;
        } else {
            //write data in DR buffer
            SPI3->DR = txbuf[index];
            index++;
        }
    }
}
