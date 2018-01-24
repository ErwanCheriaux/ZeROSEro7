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
#define MB_SIZE 253

static msg_t rmb_buffer[MB_SIZE];
static MAILBOX_DECL(rmb, rmb_buffer, MB_SIZE);

static msg_t wmb_buffer[MB_SIZE];
static MAILBOX_DECL(wmb, wmb_buffer, MB_SIZE);

/*
 * SPI TX buffers.
 */
#define MSG_SIZE 251  //nb byte useful
#define BUF_SIZE 126  //nb trame

static int     password_idx = 1000;
static uint8_t test[1000];

void spi_init(void)
{
    for(int i = 0; i < 1000; i++) {
        test[i] = 0xaa;
    }
    test[0]   = 0x01;
    test[251] = 0x02;
    test[502] = 0x03;
    test[753] = 0x04;
    test[999] = 0x00;

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
    while(chMBFetch(&rmb, &msg, TIME_IMMEDIATE) == MSG_OK) {
        //start
        if(msg == 0x676f) {
            spi_write(test, 0);
            password_ptr = 1;
        }

        //next
        else if(msg == 0x6e78) {
            spi_write(test, MSG_SIZE * password_ptr);
            password_ptr++;
        }
    }
}

void spi_write(uint8_t *msg, int begin)
{
    for(int i = 0; i < BUF_SIZE; i += 2)
        chMBPost(&wmb,
                 begin + i < password_idx ? ((uint16_t)msg[begin + i] << 8) | (uint16_t)msg[begin + i + 1] : 0,
                 TIME_INFINITE);

    // Turn on Tx interrupt
    SPI3->CR2 |= SPI_CR2_TXEIE;  //TXEIE = 1
}

void SPI_IRQHandler(void)
{
    //Overrun flag
    if(OVR) {
        rtt_printf("Overrun !!!");
        chSysHalt("Overrun !!!!");
    }
    //Receive buffer not empty
    else if(RXNE) {
        chSysLockFromISR();
        chMBPostI(&rmb, SPI3->DR);
        chSysUnlockFromISR();
    }

    //Transfer buffer empty
    else if(TXE) {
        chSysLockFromISR();
        msg_t msg;
        if(chMBFetchI(&wmb, &msg) == MSG_OK)
            SPI3->DR = msg;
        else
            SPI3->CR2 &= ~SPI_CR2_TXEIE;  // Nothing else to transmit
        chSysUnlockFromISR();
    }
}
