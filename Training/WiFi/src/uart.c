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
    sdStart(&SD6, &serialcfg);
    palSetPadMode(GPIOC, 6, PAL_MODE_ALTERNATE(8));
    palSetPadMode(GPIOC, 7, PAL_MODE_ALTERNATE(8));
}

void uart_send(void* buff)
{
    sdWrite(&SD6, buff, strlen(buff));
}

void uart_receive(void* buff, int size)
{
    sdRead(&SD6, buff, size);
}

int uart_receive_timeout(void* buff, int size, int timeout)
{
    if((int)sdReadTimeout(&SD6, buff, size, timeout) != size)
        return 1;
    return 0;
}

int uart_is_empty(void)
{
    return sdGetWouldBlock(&SD6);
}
