#include "nrf_drv_spi.h"
#include "app_error.h"
#include "nrf_spim.h"
#include "hw.h"

void HW_SPI_Init(void)
{
    spim_init();
}

// Nordic driver pauses every two symbols by forcing clock to 0
// but the SX12 is okay with this.
void HW_SPI_transfer(uint8_t* rx_buffer, uint8_t* tx_buffer, uint16_t size)
{
    spim_transfer(rx_buffer, tx_buffer, size);
}
