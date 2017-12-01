//uart.c

#include "uart.h"
#include "rtt.h"

static SerialConfig serialcfg = {
    115200,
    0,
    USART_CR2_STOP_1,
    0
};

void uart_init(void)
{
    sdStart(&SD6, &serialcfg);
    palSetPadMode(GPIOC, 6, PAL_MODE_ALTERNATE(8));
    palSetPadMode(GPIOC, 7, PAL_MODE_ALTERNATE(8));
}

void uart_send(void * buff, int size)
{
    sdWrite(&SD6, buff, size);
}

void uart_receive(void * buff, int size)
{
    sdRead(&SD6, buff, size);
}
