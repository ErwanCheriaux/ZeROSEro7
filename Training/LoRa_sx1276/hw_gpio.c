#include "nrf_drv_gpiote.h"
#include "app_error.h"

#include "hw.h"
#include "utilities.h"

static nrf_drv_gpiote_out_config_t reset_out_config = {
    NRF_GPIOTE_POLARITY_TOGGLE,
    NRF_GPIOTE_INITIAL_VALUE_LOW,
    false
} ;

void HW_GPIO_Init(uint16_t GPIO_Port, uint16_t GPIO_Pin, GPIO_InitTypeDef* initStruct) {
    switch(GPIO_Pin) {
        case RADIO_RESET_PIN :
            if(initStruct->Mode == GPIO_MODE_OUTPUT_PP) {
                nrf_drv_gpiote_out_init(RADIO_RESET_PIN,&reset_out_config) ;
            } else {
                // Disconnect (configure as input)
                nrf_drv_gpiote_out_uninit(RADIO_RESET_PIN) ;
            }
            break ;

        case RADIO_NSS_PIN :    // Handled by SPI driver
            return ;
    }
}

void HW_GPIO_Write(uint16_t GPIO_Port, uint16_t GPIO_Pin, uint32_t value) {
    switch(GPIO_Pin) {
        case RADIO_RESET_PIN :  // Handled in init config
            break ;
        case RADIO_NSS_PIN :    // Handled by SPI driver
            return ;
    }
}
