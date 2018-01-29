//uart.c

#include "uart.h"

static SerialConfig serialcfg = {
    921600,
    0,
    USART_CR2_STOP_1,
    USART_CR3_CTSE
};

void uart_RTS_start(void)
{
    palClearPad(GPIOD, 12);
}

void uart_RTS_stop(void)
{
    palSetPad(GPIOD, 12);
}

void uart_init(void)
{
    sdStart(&SD3, &serialcfg);
    // set RX, TX, RTS, CTS pins
    palSetPadMode(GPIOD, 8, PAL_MODE_ALTERNATE(7)); // TX
    palSetPadMode(GPIOD, 9, PAL_MODE_ALTERNATE(7)); // RX
    palSetPadMode(GPIOD, 11, PAL_MODE_ALTERNATE(7)); // CTS
    palSetPadMode(GPIOD, 12, PAL_MODE_OUTPUT_PUSHPULL); // RTS
    // set RTS to enable transmition
    uart_RTS_start();
}

void uart_send(void* buff, int size)
{
    if(size == 0)
        return;
    sdWrite(&SD3, buff, size);
}

void uart_receive(void* buff, int size)
{
    if(size == 0)
        return;
    sdRead(&SD3, buff, size);
}

int uart_receive_timeout(void* buff, int size, int timeout)
{
    if(size == 0)
        return 0;
    return sdReadTimeout(&SD3, buff, size, timeout);
}

int uart_is_empty(void)
{
    return sdGetWouldBlock(&SD3);
}
