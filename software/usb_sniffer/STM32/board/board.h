/*
    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#ifndef BOARD_H
#define BOARD_H

/*
 * Setup for Olimex STM32-E407 (rev.D) board.
 */

/*
 * Board identifier.
 */
#define BOARD_OLIMEX_STM32_F215
#define BOARD_NAME                  "USB Sniffer STM32-F215"

/*
 * Ethernet PHY type.
 */
#define BOARD_PHY_ID                MII_LAN8710A_ID
#define BOARD_PHY_RMII

/*
 * Board oscillators-related settings.
 */
#if !defined(STM32_LSECLK)
#define STM32_LSECLK                32768U
#endif

#if !defined(STM32_HSECLK)
#define STM32_HSECLK                16000000U
#endif

/*
 * Board voltages.
 * Required for performance limits calculation.
 */
#define STM32_VDD                   330U

/*
 * MCU type as defined in the ST header.
 */
#define STM32F215xx

/*
 * IO pins assignments.
 */
#define GPIOA_BUTTON_WKUP           0U
#define GPIOA_PIN1                  1U
#define GPIOA_PIN2                  2U
#define GPIOA_PIN3                  3U
#define GPIOA_PIN4                  4U
#define GPIOA_PIN5                  5U
#define GPIOA_PIN6                  6U
#define GPIOA_PIN7                  7U
#define GPIOC_RGB_G                 8U
#define GPIOA_OTG_FS_VBUS           9U
#define GPIOA_PIN10                 10U
#define GPIOA_OTG_FS_DM             11U
#define GPIOA_OTG_FS_DP             12U
#define GPIOA_JTAG_TMS              13U
#define GPIOA_JTAG_TCK              14U
#define GPIOA_SPI3_NSS              15U

#define GPIOA_PIN0                  0U
#define GPIOB_PIN1                  1U
#define GPIOB_BOOT1                 2U
#define GPIOB_PIN3                  3U
#define GPIOB_PIN4                  4U
#define GPIOB_PIN5                  5U
#define GPIOB_PIN6                  6U
#define GPIOB_PIN7                  7U
#define GPIOB_PIN8                  8U
#define GPIOB_PIN9                  9U
#define GPIOB_PIN10                 10U
#define GPIOB_PIN11                 11U
#define GPIOB_PIN12                 12U
#define GPIOB_OTG_HS_VBUS           13U
#define GPIOB_OTG_HS_DM             14U
#define GPIOB_OTG_HS_DP             15U

#define GPIOC_PIN0                  0U
#define GPIOC_PIN1                  1U
#define GPIOC_PIN2                  2U
#define GPIOC_PIN3                  3U
#define GPIOC_PIN4                  4U
#define GPIOC_PIN5                  5U
#define GPIOC_PIN6                  6U
#define GPIOC_PIN7                  7U
#define GPIOC_RGB_B                 8U
#define GPIOC_RGB_R                 9U
#define GPIOC_SPI3_SCK              10U
#define GPIOC_SPI3_MISO             11U
#define GPIOC_SPI3_MOSI             12U
#define GPIOC_PIN13                 13U
#define GPIOC_OSC32_IN              14U
#define GPIOC_OSC32_OUT             15U

#define GPIOD_PIN2                  2U

#define GPIOH_OSC_IN                0U
#define GPIOH_OSC_OUT               1U

/*
 * IO lines assignments.
 */
#define LINE_BUTTON_WKUP          PAL_LINE(GPIOA, 0U)
#define LINE_RGB_G                PAL_LINE(GPIOA, 8U)
#define LINE_OTG_FS_VBUS          PAL_LINE(GPIOA, 9U)
#define LINE_OTG_FS_DM            PAL_LINE(GPIOA, 11U)
#define LINE_OTG_FS_DP            PAL_LINE(GPIOA, 12U)
#define LINE_JTAG_TMS             PAL_LINE(GPIOA, 13U)
#define LINE_JTAG_TCK             PAL_LINE(GPIOA, 14U)
#define LINE_SPI3_NSS             PAL_LINE(GPIOA, 15U)

#define LINE_BOOT1                PAL_LINE(GPIOB, 2U)
#define LINE_OTG_HS_VBUS          PAL_LINE(GPIOB, 13U)
#define LINE_OTG_HS_DM            PAL_LINE(GPIOB, 14U)
#define LINE_OTG_HS_DP            PAL_LINE(GPIOB, 15U)

#define LINE_RGB_B                PAL_LINE(GPIOC, 8U)
#define LINE_RGB_R                PAL_LINE(GPIOC, 9U)
#define LINE_SPI3_SCK             PAL_LINE(GPIOC, 10U)
#define LINE_SPI3_MISO            PAL_LINE(GPIOC, 11U)
#define LINE_SPI3_MOSI            PAL_LINE(GPIOC, 12U)
#define LINE_OSC32_IN             PAL_LINE(GPIOC, 14U)
#define LINE_OSC32_OUT            PAL_LINE(GPIOC, 15U)

#define LINE_OSC_IN               PAL_LINE(GPIOH, 0U)
#define LINE_OSC_OUT              PAL_LINE(GPIOH, 1U)

/*
 * I/O ports initial setup, this configuration is established soon after reset
 * in the initialization code.
 * Please refer to the STM32 Reference Manual for details.
 */
#define PIN_MODE_INPUT(n)           (0U << ((n) * 2U))
#define PIN_MODE_OUTPUT(n)          (1U << ((n) * 2U))
#define PIN_MODE_ALTERNATE(n)       (2U << ((n) * 2U))
#define PIN_MODE_ANALOG(n)          (3U << ((n) * 2U))
#define PIN_ODR_LOW(n)              (0U << (n))
#define PIN_ODR_HIGH(n)             (1U << (n))
#define PIN_OTYPE_PUSHPULL(n)       (0U << (n))
#define PIN_OTYPE_OPENDRAIN(n)      (1U << (n))
#define PIN_OSPEED_VERYLOW(n)       (0U << ((n) * 2U))
#define PIN_OSPEED_LOW(n)           (1U << ((n) * 2U))
#define PIN_OSPEED_MEDIUM(n)        (2U << ((n) * 2U))
#define PIN_OSPEED_HIGH(n)          (3U << ((n) * 2U))
#define PIN_PUPDR_FLOATING(n)       (0U << ((n) * 2U))
#define PIN_PUPDR_PULLUP(n)         (1U << ((n) * 2U))
#define PIN_PUPDR_PULLDOWN(n)       (2U << ((n) * 2U))
#define PIN_AFIO_AF(n, v)           ((v) << (((n) % 8U) * 4U))

/*
 * GPIOA setup:
 *
 * PA0  - BUTTON_WKUP
 * PA1  - ETH_RMII_REF_CLK
 * PA2  - ETH_RMII_MDIO
 * PA3  - ETH_RMII_MDINT
 * PA4  - PIN4
 * PA5  - PIN5
 * PA6  - PIN6
 * PA7  - ETH_RMII_CRS_DV
 * PA8  - USB_HS_BUSON
 * PA9  - OTG_FS_VBUS
 * PA10 - OTG_FS_ID
 * PA11 - OTG_FS_DM
 * PA12 - OTG_FS_DP
 * PA13 - JTAG_TMS
 * PA14 - JTAG_TCK
 * PA15 - JTAG_TDI
 */
#define VAL_GPIOA_MODER             (PIN_MODE_INPUT(GPIOA_BUTTON_WKUP) |\
                                     PIN_MODE_INPUT(GPIOA_PIN1) |\
                                     PIN_MODE_INPUT(GPIOA_PIN2) |\
                                     PIN_MODE_INPUT(GPIOA_PIN3) |\
                                     PIN_MODE_INPUT(GPIOA_PIN4) |\
                                     PIN_MODE_INPUT(GPIOA_PIN5) |\
                                     PIN_MODE_INPUT(GPIOA_PIN6) |\
                                     PIN_MODE_INPUT(GPIOA_PIN7) |\
                                     PIN_MODE_INPUT(GPIOC_RGB_G) |\
                                     PIN_MODE_INPUT(GPIOA_OTG_FS_VBUS) |\
                                     PIN_MODE_INPUT(GPIOA_PIN10) |\
                                     PIN_MODE_INPUT(GPIOA_OTG_FS_DM) |\
                                     PIN_MODE_INPUT(GPIOA_OTG_FS_DP) |\
                                     PIN_MODE_ALTERNATE(GPIOA_JTAG_TMS) |\
                                     PIN_MODE_ALTERNATE(GPIOA_JTAG_TCK) |\
                                     PIN_MODE_INPUT(GPIOA_SPI3_NSS))
#define VAL_GPIOA_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOA_BUTTON_WKUP) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN1) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN2) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN3) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN4) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN5) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN6) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN7) |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_RGB_G) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_OTG_FS_VBUS) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN10) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_OTG_FS_DM) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_OTG_FS_DP) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_JTAG_TMS) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_JTAG_TCK) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_SPI3_NSS))
#define VAL_GPIOA_OSPEEDR           (PIN_OSPEED_VERYLOW(GPIOA_BUTTON_WKUP) |\
                                     PIN_OSPEED_VERYLOW(GPIOA_PIN1) |\
                                     PIN_OSPEED_VERYLOW(GPIOA_PIN2) |\
                                     PIN_OSPEED_VERYLOW(GPIOA_PIN3) |\
                                     PIN_OSPEED_VERYLOW(GPIOA_PIN4) |\
                                     PIN_OSPEED_VERYLOW(GPIOA_PIN5) |\
                                     PIN_OSPEED_VERYLOW(GPIOA_PIN6) |\
                                     PIN_OSPEED_VERYLOW(GPIOA_PIN7) |\
                                     PIN_OSPEED_VERYLOW(GPIOC_RGB_G) |\
                                     PIN_OSPEED_HIGH(GPIOA_OTG_FS_VBUS) |\
                                     PIN_OSPEED_VERYLOW(GPIOA_PIN10) |\
                                     PIN_OSPEED_HIGH(GPIOA_OTG_FS_DM) |\
                                     PIN_OSPEED_HIGH(GPIOA_OTG_FS_DP) |\
                                     PIN_OSPEED_VERYLOW(GPIOA_JTAG_TMS) |\
                                     PIN_OSPEED_VERYLOW(GPIOA_JTAG_TCK) |\
                                     PIN_OSPEED_HIGH(GPIOA_SPI3_NSS))
#define VAL_GPIOA_PUPDR             (PIN_PUPDR_FLOATING(GPIOA_BUTTON_WKUP) |\
                                     PIN_PUPDR_FLOATING(GPIOA_PIN1) |\
                                     PIN_PUPDR_FLOATING(GPIOA_PIN2) |\
                                     PIN_PUPDR_FLOATING(GPIOA_PIN3) |\
                                     PIN_PUPDR_FLOATING(GPIOA_PIN4) |\
                                     PIN_PUPDR_FLOATING(GPIOA_PIN5) |\
                                     PIN_PUPDR_FLOATING(GPIOA_PIN6) |\
                                     PIN_PUPDR_FLOATING(GPIOA_PIN7) |\
                                     PIN_PUPDR_FLOATING(GPIOC_RGB_G) |\
                                     PIN_PUPDR_FLOATING(GPIOA_OTG_FS_VBUS) |\
                                     PIN_PUPDR_FLOATING(GPIOA_PIN10) |\
                                     PIN_PUPDR_FLOATING(GPIOA_OTG_FS_DM) |\
                                     PIN_PUPDR_FLOATING(GPIOA_OTG_FS_DP) |\
                                     PIN_PUPDR_PULLUP(GPIOA_JTAG_TMS) |\
                                     PIN_PUPDR_PULLDOWN(GPIOA_JTAG_TCK) |\
                                     PIN_PUPDR_FLOATING(GPIOA_SPI3_NSS))
#define VAL_GPIOA_ODR               (PIN_ODR_LOW(GPIOA_BUTTON_WKUP) |\
                                     PIN_ODR_LOW(GPIOA_PIN1) |\
                                     PIN_ODR_LOW(GPIOA_PIN2) |\
                                     PIN_ODR_LOW(GPIOA_PIN3) |\
                                     PIN_ODR_LOW(GPIOA_PIN4) |\
                                     PIN_ODR_LOW(GPIOA_PIN5) |\
                                     PIN_ODR_LOW(GPIOA_PIN6) |\
                                     PIN_ODR_LOW(GPIOA_PIN7) |\
                                     PIN_ODR_LOW(GPIOC_RGB_G) |\
                                     PIN_ODR_LOW(GPIOA_OTG_FS_VBUS) |\
                                     PIN_ODR_LOW(GPIOA_PIN10) |\
                                     PIN_ODR_LOW(GPIOA_OTG_FS_DM) |\
                                     PIN_ODR_LOW(GPIOA_OTG_FS_DP) |\
                                     PIN_ODR_LOW(GPIOA_JTAG_TMS) |\
                                     PIN_ODR_LOW(GPIOA_JTAG_TCK) |\
                                     PIN_ODR_LOW(GPIOA_SPI3_NSS))
#define VAL_GPIOA_AFRL              (PIN_AFIO_AF(GPIOA_BUTTON_WKUP, 0U) |\
                                     PIN_AFIO_AF(GPIOA_PIN1, 0U) |\
                                     PIN_AFIO_AF(GPIOA_PIN2, 0U) |\
                                     PIN_AFIO_AF(GPIOA_PIN3, 0U) |\
                                     PIN_AFIO_AF(GPIOA_PIN4, 0U) |\
                                     PIN_AFIO_AF(GPIOA_PIN5, 0U) |\
                                     PIN_AFIO_AF(GPIOA_PIN6, 0U) |\
                                     PIN_AFIO_AF(GPIOA_PIN7, 0U))
#define VAL_GPIOA_AFRH              (PIN_AFIO_AF(GPIOC_RGB_G, 0U) |\
                                     PIN_AFIO_AF(GPIOA_OTG_FS_VBUS, 0U) |\
                                     PIN_AFIO_AF(GPIOA_PIN10, 0U) |\
                                     PIN_AFIO_AF(GPIOA_OTG_FS_DM, 0U) |\
                                     PIN_AFIO_AF(GPIOA_OTG_FS_DP, 0U) |\
                                     PIN_AFIO_AF(GPIOA_JTAG_TMS, 0U) |\
                                     PIN_AFIO_AF(GPIOA_JTAG_TCK, 0U) |\
                                     PIN_AFIO_AF(GPIOA_SPI3_NSS, 0U))

/*
 * GPIOB setup:
 *
 * PB0  - USB_FS_BUSON
 * PB1  - USB_FS_FAULT
 * PB2  - BOOT1
 * PB3  - JTAG_TDO
 * PB4  - JTAG_TRST
 * PB5  - PIN5
 * PB6  - PIN6
 * PB7  - PIN7
 * PB8  - I2C1_SCL
 * PB9  - I2C1_SDA
 * PB10 - SPI2_SCK
 * PB11 - PIN11
 * PB12 - OTG_HS_ID
 * PB13 - OTG_HS_VBUS
 * PB14 - OTG_HS_DM
 * PB15 - OTG_HS_DP
 */
#define VAL_GPIOB_MODER             (PIN_MODE_INPUT(GPIOA_PIN0) |\
                                     PIN_MODE_INPUT(GPIOB_PIN1) |\
                                     PIN_MODE_INPUT(GPIOB_BOOT1) |\
                                     PIN_MODE_INPUT(GPIOB_PIN3) |\
                                     PIN_MODE_INPUT(GPIOB_PIN4) |\
                                     PIN_MODE_INPUT(GPIOB_PIN5) |\
                                     PIN_MODE_INPUT(GPIOB_PIN6) |\
                                     PIN_MODE_INPUT(GPIOB_PIN7) |\
                                     PIN_MODE_INPUT(GPIOB_PIN8) |\
                                     PIN_MODE_INPUT(GPIOB_PIN9) |\
                                     PIN_MODE_INPUT(GPIOB_PIN10) |\
                                     PIN_MODE_INPUT(GPIOB_PIN11) |\
                                     PIN_MODE_INPUT(GPIOB_PIN12) |\
                                     PIN_MODE_INPUT(GPIOB_OTG_HS_VBUS) |\
                                     PIN_MODE_INPUT(GPIOB_OTG_HS_DM) |\
                                     PIN_MODE_INPUT(GPIOB_OTG_HS_DP))
#define VAL_GPIOB_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOA_PIN0) |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN1) |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_BOOT1) |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN3) |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN4) |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN5) |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN6) |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN7) |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN8) |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN9) |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN10) |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN11) |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN12) |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_OTG_HS_VBUS) |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_OTG_HS_DM) |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_OTG_HS_DP))
#define VAL_GPIOB_OSPEEDR           (PIN_OSPEED_VERYLOW(GPIOA_PIN0) |\
                                     PIN_OSPEED_VERYLOW(GPIOB_PIN1) |\
                                     PIN_OSPEED_VERYLOW(GPIOB_BOOT1) |\
                                     PIN_OSPEED_VERYLOW(GPIOB_PIN3) |\
                                     PIN_OSPEED_VERYLOW(GPIOB_PIN4) |\
                                     PIN_OSPEED_VERYLOW(GPIOB_PIN5) |\
                                     PIN_OSPEED_VERYLOW(GPIOB_PIN6) |\
                                     PIN_OSPEED_VERYLOW(GPIOB_PIN7) |\
                                     PIN_OSPEED_VERYLOW(GPIOB_PIN8) |\
                                     PIN_OSPEED_VERYLOW(GPIOB_PIN9) |\
                                     PIN_OSPEED_VERYLOW(GPIOB_PIN10) |\
                                     PIN_OSPEED_VERYLOW(GPIOB_PIN11) |\
                                     PIN_OSPEED_VERYLOW(GPIOB_PIN12) |\
                                     PIN_OSPEED_HIGH(GPIOB_OTG_HS_VBUS) |\
                                     PIN_OSPEED_HIGH(GPIOB_OTG_HS_DM) |\
                                     PIN_OSPEED_HIGH(GPIOB_OTG_HS_DP))
#define VAL_GPIOB_PUPDR             (PIN_PUPDR_FLOATING(GPIOA_PIN0) |\
                                     PIN_PUPDR_FLOATING(GPIOB_PIN1) |\
                                     PIN_PUPDR_FLOATING(GPIOB_BOOT1) |\
                                     PIN_PUPDR_FLOATING(GPIOB_PIN3) |\
                                     PIN_PUPDR_FLOATING(GPIOB_PIN4) |\
                                     PIN_PUPDR_FLOATING(GPIOB_PIN5) |\
                                     PIN_PUPDR_FLOATING(GPIOB_PIN6) |\
                                     PIN_PUPDR_FLOATING(GPIOB_PIN7) |\
                                     PIN_PUPDR_FLOATING(GPIOB_PIN8) |\
                                     PIN_PUPDR_FLOATING(GPIOB_PIN9) |\
                                     PIN_PUPDR_FLOATING(GPIOB_PIN10) |\
                                     PIN_PUPDR_FLOATING(GPIOB_PIN11) |\
                                     PIN_PUPDR_FLOATING(GPIOB_PIN12) |\
                                     PIN_PUPDR_FLOATING(GPIOB_OTG_HS_VBUS) |\
                                     PIN_PUPDR_FLOATING(GPIOB_OTG_HS_DM) |\
                                     PIN_PUPDR_FLOATING(GPIOB_OTG_HS_DP))
#define VAL_GPIOB_ODR               (PIN_ODR_LOW(GPIOA_PIN0) |\
                                     PIN_ODR_LOW(GPIOB_PIN1) |\
                                     PIN_ODR_LOW(GPIOB_BOOT1) |\
                                     PIN_ODR_LOW(GPIOB_PIN3) |\
                                     PIN_ODR_LOW(GPIOB_PIN4) |\
                                     PIN_ODR_LOW(GPIOB_PIN5) |\
                                     PIN_ODR_LOW(GPIOB_PIN6) |\
                                     PIN_ODR_LOW(GPIOB_PIN7) |\
                                     PIN_ODR_LOW(GPIOB_PIN8) |\
                                     PIN_ODR_LOW(GPIOB_PIN9) |\
                                     PIN_ODR_LOW(GPIOB_PIN10) |\
                                     PIN_ODR_LOW(GPIOB_PIN11) |\
                                     PIN_ODR_LOW(GPIOB_PIN12) |\
                                     PIN_ODR_HIGH(GPIOB_OTG_HS_VBUS) |\
                                     PIN_ODR_HIGH(GPIOB_OTG_HS_DM) |\
                                     PIN_ODR_HIGH(GPIOB_OTG_HS_DP))
#define VAL_GPIOB_AFRL              (PIN_AFIO_AF(GPIOA_PIN0, 0U) |\
                                     PIN_AFIO_AF(GPIOB_PIN1, 0U) |\
                                     PIN_AFIO_AF(GPIOB_BOOT1, 0U) |\
                                     PIN_AFIO_AF(GPIOB_PIN3, 0U) |\
                                     PIN_AFIO_AF(GPIOB_PIN4, 0U) |\
                                     PIN_AFIO_AF(GPIOB_PIN5, 0U) |\
                                     PIN_AFIO_AF(GPIOB_PIN6, 0U) |\
                                     PIN_AFIO_AF(GPIOB_PIN7, 0U))
#define VAL_GPIOB_AFRH              (PIN_AFIO_AF(GPIOB_PIN8, 0U) |\
                                     PIN_AFIO_AF(GPIOB_PIN9, 0U) |\
                                     PIN_AFIO_AF(GPIOB_PIN10, 0U) |\
                                     PIN_AFIO_AF(GPIOB_PIN11, 0U) |\
                                     PIN_AFIO_AF(GPIOB_PIN12, 0U) |\
                                     PIN_AFIO_AF(GPIOB_OTG_HS_VBUS, 0U) |\
                                     PIN_AFIO_AF(GPIOB_OTG_HS_DM, 0U) |\
                                     PIN_AFIO_AF(GPIOB_OTG_HS_DP, 0U))

/*
 * GPIOC setup:
 *
 * PC0  - PIN0
 * PC1  - ETH_RMII_MDC
 * PC2  - SPI2_MISO
 * PC3  - SPI2_MOSI
 * PC4  - ETH_RMII_RXD0
 * PC5  - ETH_RMII_RXD1
 * PC6  - USART6_TX
 * PC7  - USART6_RX
 * PC8  - SD_D0
 * PC9  - SD_D1
 * PC10 - SD_D2
 * PC11 - SD_D3
 * PC12 - SD_CLK
 * PC13 - LED
 * PC14 - OSC32_IN
 * PC15 - OSC32_OUT
 */
#define VAL_GPIOC_MODER             (PIN_MODE_INPUT(GPIOC_PIN0) |\
                                     PIN_MODE_INPUT(GPIOC_PIN1) |\
                                     PIN_MODE_INPUT(GPIOC_PIN2) |\
                                     PIN_MODE_INPUT(GPIOC_PIN3) |\
                                     PIN_MODE_INPUT(GPIOC_PIN4) |\
                                     PIN_MODE_INPUT(GPIOC_PIN5) |\
                                     PIN_MODE_INPUT(GPIOC_PIN6) |\
                                     PIN_MODE_INPUT(GPIOC_PIN7) |\
                                     PIN_MODE_INPUT(GPIOC_RGB_B) |\
                                     PIN_MODE_INPUT(GPIOC_RGB_R) |\
                                     PIN_MODE_INPUT(GPIOC_SPI3_SCK) |\
                                     PIN_MODE_INPUT(GPIOC_SPI3_MISO) |\
                                     PIN_MODE_INPUT(GPIOC_SPI3_MOSI) |\
                                     PIN_MODE_INPUT(GPIOC_PIN13) |\
                                     PIN_MODE_INPUT(GPIOC_OSC32_IN) |\
                                     PIN_MODE_INPUT(GPIOC_OSC32_OUT))
#define VAL_GPIOC_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOC_PIN0) |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN1) |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN2) |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN3) |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN4) |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN5) |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN6) |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN7) |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_RGB_B) |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_RGB_R) |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_SPI3_SCK) |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_SPI3_MISO) |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_SPI3_MOSI) |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN13) |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_OSC32_IN) |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_OSC32_OUT))
#define VAL_GPIOC_OSPEEDR           (PIN_OSPEED_VERYLOW(GPIOC_PIN0) |\
                                     PIN_OSPEED_VERYLOW(GPIOC_PIN1) |\
                                     PIN_OSPEED_VERYLOW(GPIOC_PIN2) |\
                                     PIN_OSPEED_VERYLOW(GPIOC_PIN3) |\
                                     PIN_OSPEED_VERYLOW(GPIOC_PIN4) |\
                                     PIN_OSPEED_VERYLOW(GPIOC_PIN5) |\
                                     PIN_OSPEED_VERYLOW(GPIOC_PIN6) |\
                                     PIN_OSPEED_VERYLOW(GPIOC_PIN7) |\
                                     PIN_OSPEED_VERYLOW(GPIOC_RGB_B) |\
                                     PIN_OSPEED_VERYLOW(GPIOC_RGB_R) |\
                                     PIN_OSPEED_HIGH(GPIOC_SPI3_SCK) |\
                                     PIN_OSPEED_HIGH(GPIOC_SPI3_MISO) |\
                                     PIN_OSPEED_HIGH(GPIOC_SPI3_MOSI) |\
                                     PIN_OSPEED_VERYLOW(GPIOC_PIN13) |\
                                     PIN_OSPEED_VERYLOW(GPIOC_OSC32_IN) |\
                                     PIN_OSPEED_VERYLOW(GPIOC_OSC32_OUT))
#define VAL_GPIOC_PUPDR             (PIN_PUPDR_FLOATING(GPIOC_PIN0) |\
                                     PIN_PUPDR_FLOATING(GPIOC_PIN1) |\
                                     PIN_PUPDR_FLOATING(GPIOC_PIN2) |\
                                     PIN_PUPDR_FLOATING(GPIOC_PIN3) |\
                                     PIN_PUPDR_FLOATING(GPIOC_PIN4) |\
                                     PIN_PUPDR_FLOATING(GPIOC_PIN5) |\
                                     PIN_PUPDR_FLOATING(GPIOC_PIN6) |\
                                     PIN_PUPDR_FLOATING(GPIOC_PIN7) |\
                                     PIN_PUPDR_FLOATING(GPIOC_RGB_B) |\
                                     PIN_PUPDR_FLOATING(GPIOC_RGB_R) |\
                                     PIN_PUPDR_FLOATING(GPIOC_SPI3_SCK) |\
                                     PIN_PUPDR_FLOATING(GPIOC_SPI3_MISO) |\
                                     PIN_PUPDR_FLOATING(GPIOC_SPI3_MOSI) |\
                                     PIN_PUPDR_FLOATING(GPIOC_PIN13) |\
                                     PIN_PUPDR_FLOATING(GPIOC_OSC32_IN) |\
                                     PIN_PUPDR_FLOATING(GPIOC_OSC32_OUT))
#define VAL_GPIOC_ODR               (PIN_ODR_LOW(GPIOC_PIN0) |\
                                     PIN_ODR_LOW(GPIOC_PIN1) |\
                                     PIN_ODR_LOW(GPIOC_PIN2) |\
                                     PIN_ODR_LOW(GPIOC_PIN3) |\
                                     PIN_ODR_LOW(GPIOC_PIN4) |\
                                     PIN_ODR_LOW(GPIOC_PIN5) |\
                                     PIN_ODR_LOW(GPIOC_PIN6) |\
                                     PIN_ODR_LOW(GPIOC_PIN7) |\
                                     PIN_ODR_LOW(GPIOC_RGB_B) |\
                                     PIN_ODR_LOW(GPIOC_RGB_R) |\
                                     PIN_ODR_LOW(GPIOC_SPI3_SCK) |\
                                     PIN_ODR_LOW(GPIOC_SPI3_MISO) |\
                                     PIN_ODR_LOW(GPIOC_SPI3_MOSI) |\
                                     PIN_ODR_LOW(GPIOC_PIN13) |\
                                     PIN_ODR_LOW(GPIOC_OSC32_IN) |\
                                     PIN_ODR_LOW(GPIOC_OSC32_OUT))
#define VAL_GPIOC_AFRL              (PIN_AFIO_AF(GPIOC_PIN0, 0U) |\
                                     PIN_AFIO_AF(GPIOC_PIN1, 0U) |\
                                     PIN_AFIO_AF(GPIOC_PIN2, 0U) |\
                                     PIN_AFIO_AF(GPIOC_PIN3, 0U) |\
                                     PIN_AFIO_AF(GPIOC_PIN4, 0U) |\
                                     PIN_AFIO_AF(GPIOC_PIN5, 0U) |\
                                     PIN_AFIO_AF(GPIOC_PIN6, 0U) |\
                                     PIN_AFIO_AF(GPIOC_PIN7, 0U))
#define VAL_GPIOC_AFRH              (PIN_AFIO_AF(GPIOC_RGB_B, 0U) |\
                                     PIN_AFIO_AF(GPIOC_RGB_R, 0U) |\
                                     PIN_AFIO_AF(GPIOC_SPI3_SCK, 0U) |\
                                     PIN_AFIO_AF(GPIOC_SPI3_MISO, 0U) |\
                                     PIN_AFIO_AF(GPIOC_SPI3_MOSI, 0U) |\
                                     PIN_AFIO_AF(GPIOC_PIN13, 0U) |\
                                     PIN_AFIO_AF(GPIOC_OSC32_IN, 0U) |\
                                     PIN_AFIO_AF(GPIOC_OSC32_OUT, 0U))

/*
 * GPIOD setup:
 *
 * PD2  - SD_CMD
 */
#define VAL_GPIOD_MODER             (PIN_MODE_INPUT(GPIOD_PIN2))
#define VAL_GPIOD_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOD_PIN2))
#define VAL_GPIOD_OSPEEDR           (PIN_OSPEED_VERYLOW(GPIOD_PIN2))
#define VAL_GPIOD_PUPDR             (PIN_PUPDR_FLOATING(GPIOD_PIN2))
#define VAL_GPIOD_ODR               (PIN_ODR_LOW(GPIOD_PIN2))
#define VAL_GPIOD_AFRL              (PIN_AFIO_AF(GPIOD_PIN2, 0U))
#define VAL_GPIOD_AFRH              (0U)

/*
 * GPIOH setup:
 *
 * PH0  - OSC_IN
 * PH1  - OSC_OUT
 */
#define VAL_GPIOH_MODER             (PIN_MODE_INPUT(GPIOH_OSC_IN) |\
                                     PIN_MODE_INPUT(GPIOH_OSC_OUT))
#define VAL_GPIOH_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOH_OSC_IN) |\
                                     PIN_OTYPE_PUSHPULL(GPIOH_OSC_OUT))
#define VAL_GPIOH_OSPEEDR           (PIN_OSPEED_VERYLOW(GPIOH_OSC_IN) |\
                                     PIN_OSPEED_VERYLOW(GPIOH_OSC_OUT))
#define VAL_GPIOH_PUPDR             (PIN_PUPDR_FLOATING(GPIOH_OSC_IN) |\
                                     PIN_PUPDR_FLOATING(GPIOH_OSC_OUT))
#define VAL_GPIOH_ODR               (PIN_ODR_LOW(GPIOH_OSC_IN) |\
                                     PIN_ODR_LOW(GPIOH_OSC_OUT))
#define VAL_GPIOH_AFRL              (PIN_AFIO_AF(GPIOH_OSC_IN,  0U) |\
                                     PIN_AFIO_AF(GPIOH_OSC_OUT, 0U))
#define VAL_GPIOH_AFRH              (0U)

#if !defined(_FROM_ASM_)
#ifdef __cplusplus
extern "C" {
#endif
  void boardInit(void);
#ifdef __cplusplus
}
#endif
#endif /* _FROM_ASM_ */

#endif /* BOARD_H */
