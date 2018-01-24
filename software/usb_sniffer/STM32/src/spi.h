//spi.h

#ifndef SPI_H
#define SPI_H

#include "hal.h"

void spi_init(void);
void spi_write(uint8_t *msg, int begin);

void spiMainLoop(void);

#endif
