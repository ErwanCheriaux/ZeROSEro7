#ifndef HW_GPIO_H
#define HW_GPIO_H

#include "sdk_config.h"

/*
*   GPIO (light) Module to control resetting the SX1276 in the driver
*   Simplified clone from stm32l4xx_hal_gpio.h from HAL_DRIVER (since nrf52 also has a cortex m4)
*   Usage in sx1276.c:1226
*/

/**
  * @brief  GPIO Bit SET and Bit RESET enumeration
  */
/*typedef enum
{
  GPIO_PIN_RESET = 0,
  GPIO_PIN_SET
} GPIO_PinState;*/

// TODO not sure about these definitions used only Once in sx1276.c:1248. Used to put into sleep mode and other opModes.
#define SET     1
#define RESET   0



/** @defgroup GPIO_Exported_Types GPIO Exported Types
  * @{
  */
/**
  * @brief   GPIO Init structure definition
  */
typedef struct
{
  uint32_t Pin;        /*!< Specifies the GPIO pins to be configured.
                           This parameter can be any value of @ref GPIO_pins */

  uint32_t Mode;       /*!< Specifies the operating mode for the selected pins.
                           This parameter can be a value of @ref GPIO_mode */

  uint32_t Pull;       /*!< Specifies the Pull-up or Pull-Down activation for the selected pins.
                           This parameter can be a value of @ref GPIO_pull */

  uint32_t Speed;      /*!< Specifies the speed for the selected pins.
                           This parameter can be a value of @ref GPIO_speed */

  uint32_t Alternate;  /*!< Peripheral to be connected to the selected pins
                            This parameter can be a value of @ref GPIOEx_Alternate_function_selection */
} GPIO_InitTypeDef;

#define  GPIO_MODE_OUTPUT_PP    ((uint32_t)0x00000001)   /*!< Output Push Pull Mode                 */
#define  GPIO_NOPULL            ((uint32_t)0x00000000)   /*!< No Pull-up or Pull-down activation  */
#define  GPIO_SPEED_HIGH        ((uint32_t)0x00000002)   /*!< range 25 MHz to 50 MHz, please refer to the product datasheet */


// Can be any value. Only passed to our HW_GPIO_INIT and WRITE functions
// No need to handle, SPI driver controls it already.
#define RADIO_NSS_PORT  SPI_SS_PIN
#define RADIO_NSS_PIN   SPI_SS_PIN

#define RADIO_RESET_PORT    0xBEEF // Unused
#define RADIO_RESET_PIN     31

#define SX1276_DIO0_PIN 0xDEADBEEF  // Unused, Indicates FIFO levels in packet mode
#define SX1276_DIO1_PIN 0xDEADBEEF
#define SX1276_DIO2_PIN 0xDEADBEEF
#define SX1276_DIO3_PIN 0xDEADBEEF
#define SX1276_DIO4_PIN 0xDEADBEEF
#define SX1276_DIO5_PIN 0xDEADBEEF


/*!
 * @brief Initializes the given GPIO object
 *
 * @param  GPIOx: where x can be (A..E and H)
 * @param  GPIO_Pin: specifies the port bit to be written.
 *                   This parameter can be one of GPIO_PIN_x where x can be (0..15).
 *                   All port bits are not necessarily available on all GPIOs.
 * @param [IN] initStruct  GPIO_InitTypeDef intit structure
 * @retval none
 */
 // TODO mimic the reset in sx1276.c:1226.
 // Warning ! The second call gives a Pull type as Mode to say "GPIO_MODE_INPUT" which represent mask 0x00000000
void HW_GPIO_Init(uint16_t GPIO_Port, uint16_t GPIO_Pin, GPIO_InitTypeDef* initStruct);

/*!
 * @brief Writes the given value to the GPIO output
 *
 * @param  GPIO_Pin: specifies the port bit to be written.
 *                   This parameter can be one of GPIO_PIN_x where x can be (0..15).
 *                   All port bits are not necessarily available on all GPIOs.
 * @param [IN] value New GPIO output value
 * @retval none
 */
void HW_GPIO_Write(uint16_t GPIO_Port, uint16_t GPIO_Pin, uint32_t value);

/*!
 * \brief Initializes the io
 */
void SX1276IoInit() ;

/*!
 * \brief Deinitializes the io
 */
void SX1276IoDeInit() ;

#endif
