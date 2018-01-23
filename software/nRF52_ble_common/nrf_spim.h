#ifndef NRF_SPIM_H
#define NRF_SPIM_H

void spim_init();

void spim_transfer(uint8_t* rx_buffer, uint8_t* tx_buffer, uint16_t size);

#endif
