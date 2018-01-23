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
#define Rx_BF_SIZE 10
#define Tx_BF_SIZE 200

static uint16_t msg_size;
static uint16_t rxbuf_index = 0;
static uint16_t rxbuf[Rx_BF_SIZE];
static uint16_t txbuf[Tx_BF_SIZE];

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

void spiMainLoop(void)
{
    msg_t msg;
    for(int i = 0; i < chMBGetUsedCountI(&mb); i++) {
        chMBFetch(&mb, &msg, TIME_INFINITE);
        rxbuf[rxbuf_index] = msg;

        // loop buffer
        if(rxbuf_index++ >= Rx_BF_SIZE)
            rxbuf_index = 0;
    }
}

void spi_write(uint16_t *msg, int n)
{
    //verif msg size
    if(n > Tx_BF_SIZE) {
        rtt_printf("Can't send msg too long...");
        rtt_printf("%d > %d", n, Tx_BF_SIZE);
        return;
    }

    msg_size = n;

    //push to send buffer
    for(uint8_t i = 0; i < msg_size; i++)
        txbuf[i]  = (uint16_t)msg[i];

    //write first data to be read for the first posedge clock
    SPI3->DR = txbuf[0];

    //turn on Tx interrupt
    SPI3->CR2 |= SPI_CR2_TXEIE;  //TXEIE = 1
}

void spi_display_buffer(uint16_t n)
{
    int index = rxbuf_index;
    while(n > 0) {
        rtt_printf("rxbuf[%d] = %04x", index, rxbuf[index]);
        if(index == 0)
            index = Rx_BF_SIZE;
        else
            index--;
        n--;
    }
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
        static uint32_t index = 1;

        if(index >= msg_size) {
            //turn off Tx interrupt
            SPI3->CR2 &= ~SPI_CR2_TXEIE;  //TXEIE = 0
            SPI3->DR = 0x0000;            //if not the last data is sent in loop
            index    = 0;
        } else {
            //write data in DR buffer
            SPI3->DR = txbuf[index];
            index++;
        }
    }
}
