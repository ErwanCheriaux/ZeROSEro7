//spi.h

#ifndef SPI_H
#define SPI_H

extern char *password;

void spi_init(void);
void spi_write(char *msg);
void SPI_Rx_IRQHandler(void);

#endif
