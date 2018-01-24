//uart.c

#include "uart.h"
#include <string.h>

static SerialConfig serialcfg = {
    115200,
    0,
    USART_CR2_STOP_1,
    USART_CR3_RTSE | USART_CR3_CTSE // Depend of STM
};

void uart_init(void)
{
    sdStart(&SD3, &serialcfg);
    palSetPadMode(GPIOD, 8, PAL_MODE_ALTERNATE(7));
    palSetPadMode(GPIOD, 9, PAL_MODE_ALTERNATE(7));
    palSetPadMode(GPIOD, 11, PAL_MODE_ALTERNATE(7));
    palSetPadMode(GPIOD, 12, PAL_MODE_ALTERNATE(7));
}

void uart_send(void* buff)
{
    sdWrite(&SD3, buff, strlen(buff));
}

void uart_receive(void* buff, int size)
{
    sdRead(&SD3, buff, size);
}

int uart_receive_timeout(void* buff, int size, int timeout)
{
    return sdReadTimeout(&SD3, buff, size, timeout);
}

int uart_is_empty(void)
{
    return sdGetWouldBlock(&SD3);
}
