#include "nrf_drv_spi.h"
#include "app_error.h"

#define SPI_SX12_INSTANCE  0
static const nrf_drv_spi_t spi_sx12 = NRF_DRV_SPI_INSTANCE(SPI_SX12_INSTANCE);

// SX1276 only uses 8 bit out of 16 for transfers
#define BUFFER_LENGTH 1
static uint16_t rx_buffer;

// No need of complicated buffer management, length is fixed to 16 bits by SX12 driver
uint16_t HW_SPI_InOut(uint16_t outData) {
    rx_buffer = 0 ;
    APP_ERROR_CHECK(nrf_drv_spi_transfer(&spi_sx12, (uint8_t *) &outData, BUFFER_LENGTH, (uint8_t *) &rx_buffer, BUFFER_LENGTH));
    return rx_buffer ;
}

void HW_SPI_Init( void ) {
    static nrf_drv_spi_config_t spi_config = NRF_DRV_SPI_DEFAULT_CONFIG;
    spi_config.ss_pin   = SPI_SS_PIN;
    spi_config.miso_pin = SPI_MISO_PIN;
    spi_config.mosi_pin = SPI_MOSI_PIN;
    spi_config.sck_pin  = SPI_SCK_PIN;
    APP_ERROR_CHECK(nrf_drv_spi_init(&spi_sx12, &spi_config, NULL, NULL));  // No Handler so that transfer is blocking
}
