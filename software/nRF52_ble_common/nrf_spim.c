#include "nrf_drv_spi.h"
#include "app_error.h"

#if SPI_ENABLED == 1

#define SPI_INSTANCE_0 0
static const nrf_drv_spi_t spi_instance = NRF_DRV_SPI_INSTANCE(SPI_INSTANCE_0);

void spim_init(void)
{
    static nrf_drv_spi_config_t spi_config = NRF_DRV_SPI_DEFAULT_CONFIG;
    spi_config.ss_pin                      = SPI_SS_PIN;
    spi_config.miso_pin                    = SPI_MISO_PIN;
    spi_config.mosi_pin                    = SPI_MOSI_PIN;
    spi_config.sck_pin                     = SPI_SCK_PIN;
    APP_ERROR_CHECK(nrf_drv_spi_init(&spi_instance, &spi_config, NULL, NULL));  // No Handler so that transfer is blocking
}

// Nordic driver pauses every two symbols by forcing clock to 0
// but the SX12 is okay with this.
void spim_transfer(uint8_t* rx_buffer, uint8_t* tx_buffer, uint16_t size)
{
    nrf_drv_spi_transfer(&spi_instance, tx_buffer, size, rx_buffer, size);
}

#endif  // SPI_ENABLED = 1
