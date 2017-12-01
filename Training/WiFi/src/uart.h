//uart.h

#ifndef UART_H
#define UART_H

#include "ch.h"
#include "hal.h"

void uart_init(void);

/* Send data using uart
** buff: send buffer
** size: number of data frames to send
*/
void uart_send(void * buff, int size);

/* Receive data using uart
** buff: receive buffer
** size: number of data frames to receive
*/
void uart_receive(void * buff, int size);

#endif
