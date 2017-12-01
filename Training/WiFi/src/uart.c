//uart.c

#include "uart.h"
#include "rtt.h"

/*
 * This callback is invoked on a receive error, the errors mask is passed
 * as parameter.
 */
static void rxerr(UARTDriver *uartp, uartflags_t e)
{
    (void)uartp;
    (void)e;
    rtt_printf(0, "[ERROR]: uart_error\n");
}

/*
 * This callback is invoked when a character is received but the application
 * was not ready to receive it, the character is passed as parameter.
 */
static void rxchar(UARTDriver *uartp, uint16_t c)
{
    (void)uartp;
    rtt_printf(0, "uart_rx: %c\n", c);
}

/*
 * This callback is invoked when a receive buffer has been completely written.
 */
static void rxend(UARTDriver *uartp)
{
    (void)uartp;
}

/*
 * This callback is invoked when a transmission buffer has been completely
 * read by the driver.
 */
static void txend1(UARTDriver *uartp)
{
    (void)uartp;
}

/*
 * This callback is invoked when a transmission has physically completed.
 */
static void txend2(UARTDriver *uartp)
{
    (void)uartp;
}

static UARTConfig uartcfg = {
    txend1,
    txend2,
    rxend,
    rxchar,
    rxerr,
    115200,
    0,
    USART_CR2_LINEN,
    0
};

void uart_init(void)
{
    uartStart(&UARTD6, &uartcfg);
}
