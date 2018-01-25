//uart.c

#include "uart.h"

static SerialConfig serialcfg = {
    115200,
    0,
    USART_CR2_STOP_1,
    USART_CR3_RTSE | USART_CR3_CTSE  // Depend of STM
};

void uart_init(void)
{
    sdStart(&SD1, &serialcfg);
}

void uart_send(void* buff, int size)
{
    if(size == 0)
        return;
    sdWrite(&SD1, buff, size);
}

void uart_receive(void* buff, int size)
{
    if(size == 0)
        return;
    sdRead(&SD1, buff, size);
}

int uart_receive_timeout(void* buff, int size, int timeout)
{
    if(size == 0)
        return 0;
    return sdReadTimeout(&SD1, buff, size, timeout);
}

int uart_is_empty(void)
{
    return sdGetWouldBlock(&SD1);
}
