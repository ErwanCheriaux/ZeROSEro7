#ifndef HW_SPI_H
#define HW_SPI_H

/*!
 * @brief Sends outData and receives inData
 *
 * @param [IN] outData Byte to be sent
 * @retval inData      Received byte.
 */
uint16_t HW_SPI_InOut(uint16_t outData);

#endif
