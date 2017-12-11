#ifndef HW_SPI_H
#define HW_SPI_H

/*!
 * @brief Initializes the SPI object and MCU peripheral
 *
 * @param [IN] none
 */
void HW_SPI_Init(void);

/*
*   Custom. The nRF SPI driver won't make successive transfers in one window,
*   so I had to override the way sx1276 asks for InOut
*/
void HW_SPI_transfer(uint8_t* rx_buffer, uint8_t* tx_buffer, uint16_t size);

#endif
