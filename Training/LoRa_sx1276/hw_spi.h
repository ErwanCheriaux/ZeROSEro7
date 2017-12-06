#ifndef HW_SPI_H
#define HW_SPI_H

/*!
 * @brief Sends outData and receives inData
 *
 * @param [IN] outData Byte to be sent
 * @retval inData      Received byte.
 */ // Only uses 8 LSB
uint16_t HW_SPI_InOut(uint16_t outData);

/*!
 * @brief Initializes the SPI object and MCU peripheral
 *
 * @param [IN] none
 */
void HW_SPI_Init( void );

#endif
