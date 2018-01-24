//spi.c

#include <string.h>

#include "spi.h"
#include "rtt.h"
#include "vectors.h"

#define OVR (SPI3->SR & SPI_SR_OVR)
#define TXE (SPI3->SR & SPI_SR_TXE)
#define RXNE (SPI3->SR & SPI_SR_RXNE)

/*
 * Mail Box
 */
#define MB_SIZE 253

static msg_t mb_buffer[MB_SIZE];
static MAILBOX_DECL(mb, mb_buffer, MB_SIZE);

/*
 * SPI TX buffers.
 */
#define MSG_SIZE 251 //nb byte useful
#define BUF_SIZE 126 //nb trame

static uint16_t txbuf[BUF_SIZE];

static uint8_t test[1000];

void spi_init(void)
{
    for(int i   = 0; i < 1000; i++)
        test[i] = 0xaa;
    test[999]   = 0x00;

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
    static int password_ptr = 0;
    msg_t      msg;
    for(int i = 0; i < chMBGetUsedCountI(&mb); i++) {
        chMBFetch(&mb, &msg, TIME_INFINITE);

        //start
        if(msg == 0x676f) {
            rtt_printf("Receive start");
            spi_write(test, 0);
            password_ptr = 1;
        }

        //next
        else if(msg == 0x6e78) {
            rtt_printf("Receive next");
            spi_write(test, MSG_SIZE * password_ptr);
            password_ptr++;
        }
    }
}

void spi_write(uint8_t *msg, int begin)
{
    int index_left, index_right;

    //push to send buffer
    for(int i    = 0; i < BUF_SIZE; i++) {
        index_left  = 2*i+begin;
        index_right = 2*i+1+begin;

        if(msg[index_left] == 0) {
            txbuf[i] = 0x0000;
            break;
        }

        txbuf[i] = ((uint16_t)msg[index_left] << 8) + (uint16_t)msg[index_right];

        if(msg[index_right] == 0)
            break;
    }

    //write first data to be read for the first posedge clock
    SPI3->DR = txbuf[0];

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
        // Byte 0 is sent during first TX
        static int index = 1;

        if(index >= BUF_SIZE) {
            //turn off Tx interrupt
            SPI3->CR2 &= ~SPI_CR2_TXEIE;  //TXEIE = 0
            SPI3->DR = 0x0000;
            index    = 1;
        } else {
            //write data in DR buffer
            SPI3->DR = txbuf[index];
            index++;
        }
    }
}
