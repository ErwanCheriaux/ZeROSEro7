#include "nrf_drv_gpiote.h"
#include "app_error.h"

#include "hw.h"
#include "utilities.h"

void HW_GPIO_Init(uint16_t GPIO_Port, uint16_t GPIO_Pin, GPIO_InitTypeDef* initStruct) {}

void HW_GPIO_Write(uint16_t GPIO_Port, uint16_t GPIO_Pin, uint32_t value) {
    switch(GPIO_Pin) {
        case RADIO_RESET_PIN :
            APP_ERROR_CHECK(0xDEADBEEF) ; // Unsupported
            break ;
        case RADIO_NSS_PIN :
            return ;
    }
}
