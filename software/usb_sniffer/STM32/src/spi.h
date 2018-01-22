//spi.h

#ifndef SPI_H
#define SPI_H

extern char *password;

void spi_init(void);
void spi_display_config(void);
void spi_mailbox_refresh(void);
void spi_write(char *msg);

#endif
