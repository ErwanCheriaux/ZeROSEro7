//spi.h

#ifndef SPI_H
#define SPI_H

#include "hal.h"

void spi_init(void);
void spi_write(uint16_t *msg, int n);

void spiMainLoop(void);

#endif
