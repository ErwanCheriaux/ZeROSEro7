//uart.h

#ifndef UART_H
#define UART_H

#include "ch.h"
#include "hal.h"

void uart_init(void);

/* Send data using uart
** buff: send buffer
*/
void uart_send(void* buff);

/* Receive data using uart
** buff: receive buffer
** size: number of data frames to receive
*/
void uart_receive(void* buff, int size);

/* Receive data using uart and return 1 if a timeout occured
** buff:    receive buffer
** size:    number of data frames to receive
** timeout: time to wait for input in system ticks
*/
int uart_receive_timeout(void* buff, int size, int timeout);

/* Check if uart fifo is empty
** return 0 if fifo is empty, else 1
*/
int uart_is_empty(void);

#endif
