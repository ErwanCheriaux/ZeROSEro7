//uart.c

#include "uart.h"
#include <string.h>

static SerialConfig serialcfg = {
    115200,
    0,
    USART_CR2_STOP_1,
    0};

void uart_init(void)
{
    sdStart(&SD1, &serialcfg);
}

void uart_send(void* buff)
{
    sdWrite(&SD1, buff, strlen(buff));
}

void uart_receive(void* buff, int size)
{
    sdRead(&SD1, buff, size);
}

int uart_receive_timeout(void* buff, int size, int timeout)
{
    return sdReadTimeout(&SD1, buff, size, timeout);
}

int uart_is_empty(void)
{
    return sdGetWouldBlock(&SD1);
}
