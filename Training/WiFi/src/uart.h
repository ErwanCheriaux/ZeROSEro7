//uart.h

#ifndef UART_H
#define UART_H

#include "ch.h"
#include "hal.h"

void uart_init(void);

/* Send data using uart
** size: number of data frames to send
** buff: send buffer
*/
void uart_send(int size, void * buff);

/* Receive data using uart
** size: number of data frames to receive
** buff: receive buffer
*/
void uart_receive(int size, void * buff);

#endif
