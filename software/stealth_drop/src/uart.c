//uart.c

#include "uart.h"

static SerialConfig serialcfg = {
    921600,
    0,
    USART_CR2_STOP_1,
    USART_CR3_CTSE};

void uart_RTS_start(void)
{
    palClearPad(GPIOA, GPIOA_UART_UC2WIFI_TS);
}

void uart_RTS_stop(void)
{
    palSetPad(GPIOA, GPIOA_UART_UC2WIFI_TS);
}

void uart_init(void)
{
    sdStart(&SD1, &serialcfg);
    palSetPadMode(GPIOA, GPIOA_UART_UC2WIFI, PAL_MODE_ALTERNATE(7));        // TX
    palSetPadMode(GPIOA, GPIOA_UART_WIFI2UC, PAL_MODE_ALTERNATE(7));        // RX
    palSetPadMode(GPIOA, GPIOA_UART_WIFI2UC_TS, PAL_MODE_ALTERNATE(7));     // CTS
    palSetPadMode(GPIOA, GPIOA_UART_UC2WIFI_TS, PAL_MODE_OUTPUT_PUSHPULL);  // RTS

    // set RTS to enable transmition
    uart_RTS_start();
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
