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
 * Setup for Stealth Drop
 */

/*
 * Board identifier.
 */
#define BOARD_STEALTH_DROP
#define BOARD_NAME                  "Stealth Drop"

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
#define STM32F205xx

/*
 * IO pins assignments.
 */
#define GPIOA_VBAT_2                0U
#define GPIOA_PIN1                  1U
#define GPIOA_PIN2                  2U
#define GPIOA_PIN3                  3U
#define GPIOA_SPI_NSS               4U
#define GPIOA_SPI_SCK               5U
#define GPIOA_SPI_MISO              6U
#define GPIOA_SPI_MOSI              7U
#define GPIOA_WIFI_NRST             8U
#define GPIOA_UART_UC2WIF           9U
#define GPIOA_UART_WIFI2UC          10U
#define GPIOA_UART_WIFI2UC_TS       11U
#define GPIOA_UART_UC2WIFI_TS       12U
#define GPIOA_SWDIO                 13U
#define GPIOA_SWCLK                 14U
#define GPIOA_WIFI_WAKE             15U

#define GPIOB_PIN0                  0U
#define GPIOB_PIN1                  1U
#define GPIOB_PIN2                  2U
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

#define GPIOC_RGB_R                 0U
#define GPIOC_RGB_G                 1U
#define GPIOC_RGB_B                 2U
#define GPIOC_PIN3                  3U
#define GPIOC_PIN4                  4U
#define GPIOC_PIN5                  5U
#define GPIOC_PIN6                  6U
#define GPIOC_PIN7                  7U
#define GPIOC_SD_D0                 8U
#define GPIOC_SD_D1                 9U
#define GPIOC_SD_D2                 10U
#define GPIOC_SD_D3                 11U
#define GPIOC_SD_CLK                12U
#define GPIOC_PIN13                 13U
#define GPIOC_PIN14                 14U
#define GPIOC_PIN15                 15U

#define GPIOD_SD_CMD                2U

#define GPIOH_OSC_IN                0U
#define GPIOH_OSC_OUT               1U

/*
 * IO lines assignments.
 */
#define LINE_VBAT_2                 PAL_LINE(GPIOA, 0U)
#define LINE_SPI_NSS                PAL_LINE(GPIOA, 4U)
#define LINE_SPI_SCK                PAL_LINE(GPIOA, 5U)
#define LINE_SPI_MISO               PAL_LINE(GPIOA, 6U)
#define LINE_SPI_MOSI               PAL_LINE(GPIOA, 7U)
#define LINE_WIFI_NRST              PAL_LINE(GPIOA, 8U)
#define LINE_UART_UC2WIF            PAL_LINE(GPIOA, 9U)
#define LINE_UART_WIFI2UC           PAL_LINE(GPIOA, 10U)
#define LINE_UART_WIFI2UC_TS        PAL_LINE(GPIOA, 11U)
#define LINE_UART_UC2WIFI_TS        PAL_LINE(GPIOA, 12U)
#define LINE_SWDIO                  PAL_LINE(GPIOA, 13U)
#define LINE_SWCLK                  PAL_LINE(GPIOA, 14U)
#define LINE_WIFI_WAKE              PAL_LINE(GPIOA, 15U)

#define LINE_OTG_HS_VBUS            PAL_LINE(GPIOB, 13U)
#define LINE_OTG_HS_DM              PAL_LINE(GPIOB, 14U)
#define LINE_OTG_HS_DP              PAL_LINE(GPIOB, 15U)

#define LINE_RGB_R                  PAL_LINE(GPIOC, 0U)
#define LINE_RGB_G                  PAL_LINE(GPIOC, 1U)
#define LINE_RGB_B                  PAL_LINE(GPIOC, 2U)
#define LINE_SD_D0                  PAL_LINE(GPIOC, 8U)
#define LINE_SD_D1                  PAL_LINE(GPIOC, 9U)
#define LINE_SD_D2                  PAL_LINE(GPIOC, 10U)
#define LINE_SD_D3                  PAL_LINE(GPIOC, 11U)
#define LINE_SD_CLK                 PAL_LINE(GPIOC, 12U)

#define LINE_SD_CMD                 PAL_LINE(GPIOD, 2U)

#define LINE_OSC_IN                 PAL_LINE(GPIOH, 0U)
#define LINE_OSC_OUT                PAL_LINE(GPIOH, 1U)


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
#define PIN_ODR_LOW(n)             (1U << (n))
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
 * PA0  - VBAT_2
 * PA1  - PIN1
 * PA2  - PIN2
 * PA3  - PIN3
 * PA4  - SPI_NSS
 * PA5  - SPI_SCK
 * PA6  - SPI_MISO
 * PA7  - SPI_MOSI
 * PA8  - WIFI_NRST
 * PA9  - UART_UC2WIF
 * PA10 - UART_WIFI2UC
 * PA11 - UART_WIFI2UC_TS
 * PA12 - UART_UC2WIFI_TS
 * PA13 - SWDIO
 * PA14 - SWCLK
 * PA15 - WIFI_WAKE
 */
#define VAL_GPIOA_MODER             (PIN_MODE_INPUT(GPIOA_VBAT_2) |\
                                     PIN_MODE_INPUT(GPIOA_PIN1) |\
                                     PIN_MODE_INPUT(GPIOA_PIN2) |\
                                     PIN_MODE_INPUT(GPIOA_PIN3) |\
                                     PIN_MODE_INPUT(GPIOA_SPI_NSS) |\
                                     PIN_MODE_INPUT(GPIOA_SPI_SCK) |\
                                     PIN_MODE_INPUT(GPIOA_SPI_MISO) |\
                                     PIN_MODE_INPUT(GPIOA_SPI_MOSI) |\
                                     PIN_MODE_INPUT(GPIOA_WIFI_NRST) |\
                                     PIN_MODE_INPUT(GPIOA_UART_UC2WIF) |\
                                     PIN_MODE_INPUT(GPIOA_UART_WIFI2UC) |\
                                     PIN_MODE_INPUT(GPIOA_UART_WIFI2UC_TS) |\
                                     PIN_MODE_INPUT(GPIOA_UART_UC2WIFI_TS) |\
                                     PIN_MODE_ALTERNATE(GPIOA_SWDIO) |\
                                     PIN_MODE_ALTERNATE(GPIOA_SWCLK) |\
                                     PIN_MODE_INPUT(GPIOA_WIFI_WAKE))
#define VAL_GPIOA_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOA_VBAT_2) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN1) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN2) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN3) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_SPI_NSS) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_SPI_SCK) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_SPI_MISO) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_SPI_MOSI) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_WIFI_NRST) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_UART_UC2WIF) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_UART_WIFI2UC) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_UART_WIFI2UC_TS) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_UART_UC2WIFI_TS) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_SWDIO) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_SWCLK) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_WIFI_WAKE))
#define VAL_GPIOA_OSPEEDR           (PIN_OSPEED_VERYLOW(GPIOA_VBAT_2) |\
                                     PIN_OSPEED_VERYLOW(GPIOA_PIN1) |\
                                     PIN_OSPEED_VERYLOW(GPIOA_PIN2) |\
                                     PIN_OSPEED_VERYLOW(GPIOA_PIN3) |\
                                     PIN_OSPEED_VERYLOW(GPIOA_SPI_NSS) |\
                                     PIN_OSPEED_VERYLOW(GPIOA_SPI_SCK) |\
                                     PIN_OSPEED_VERYLOW(GPIOA_SPI_MISO) |\
                                     PIN_OSPEED_HIGH(GPIOA_SPI_MOSI) |\
                                     PIN_OSPEED_VERYLOW(GPIOA_WIFI_NRST) |\
                                     PIN_OSPEED_HIGH(GPIOA_UART_UC2WIF) |\
                                     PIN_OSPEED_HIGH(GPIOA_UART_WIFI2UC) |\
                                     PIN_OSPEED_HIGH(GPIOA_UART_WIFI2UC_TS) |\
                                     PIN_OSPEED_HIGH(GPIOA_UART_UC2WIFI_TS) |\
                                     PIN_OSPEED_VERYLOW(GPIOA_SWDIO) |\
                                     PIN_OSPEED_VERYLOW(GPIOA_SWCLK) |\
                                     PIN_OSPEED_VERYLOW(GPIOA_WIFI_WAKE))
#define VAL_GPIOA_PUPDR             (PIN_PUPDR_FLOATING(GPIOA_VBAT_2) |\
                                     PIN_PUPDR_FLOATING(GPIOA_PIN1) |\
                                     PIN_PUPDR_FLOATING(GPIOA_PIN2) |\
                                     PIN_PUPDR_FLOATING(GPIOA_PIN3) |\
                                     PIN_PUPDR_FLOATING(GPIOA_SPI_NSS) |\
                                     PIN_PUPDR_FLOATING(GPIOA_SPI_SCK) |\
                                     PIN_PUPDR_FLOATING(GPIOA_SPI_MISO) |\
                                     PIN_PUPDR_FLOATING(GPIOA_SPI_MOSI) |\
                                     PIN_PUPDR_FLOATING(GPIOA_WIFI_NRST) |\
                                     PIN_PUPDR_FLOATING(GPIOA_UART_UC2WIF) |\
                                     PIN_PUPDR_FLOATING(GPIOA_UART_WIFI2UC) |\
                                     PIN_PUPDR_FLOATING(GPIOA_UART_WIFI2UC_TS) |\
                                     PIN_PUPDR_FLOATING(GPIOA_UART_UC2WIFI_TS) |\
                                     PIN_PUPDR_PULLUP(GPIOA_SWDIO) |\
                                     PIN_PUPDR_PULLDOWN(GPIOA_SWCLK) |\
                                     PIN_PUPDR_FLOATING(GPIOA_WIFI_WAKE))
#define VAL_GPIOA_ODR               (PIN_ODR_LOW(GPIOA_VBAT_2) |\
                                     PIN_ODR_LOW(GPIOA_PIN1) |\
                                     PIN_ODR_LOW(GPIOA_PIN2) |\
                                     PIN_ODR_LOW(GPIOA_PIN3) |\
                                     PIN_ODR_LOW(GPIOA_SPI_NSS) |\
                                     PIN_ODR_LOW(GPIOA_SPI_SCK) |\
                                     PIN_ODR_LOW(GPIOA_SPI_MISO) |\
                                     PIN_ODR_LOW(GPIOA_SPI_MOSI) |\
                                     PIN_ODR_LOW(GPIOA_WIFI_NRST) |\
                                     PIN_ODR_LOW(GPIOA_UART_UC2WIF) |\
                                     PIN_ODR_LOW(GPIOA_UART_WIFI2UC) |\
                                     PIN_ODR_LOW(GPIOA_UART_WIFI2UC_TS) |\
                                     PIN_ODR_LOW(GPIOA_UART_UC2WIFI_TS) |\
                                     PIN_ODR_LOW(GPIOA_SWDIO) |\
                                     PIN_ODR_LOW(GPIOA_SWCLK) |\
                                     PIN_ODR_LOW(GPIOA_WIFI_WAKE))
#define VAL_GPIOA_AFRL              (PIN_AFIO_AF(GPIOA_VBAT_2, 0U) |\
                                     PIN_AFIO_AF(GPIOA_PIN1, 0U) |\
                                     PIN_AFIO_AF(GPIOA_PIN2, 0U) |\
                                     PIN_AFIO_AF(GPIOA_PIN3, 0U) |\
                                     PIN_AFIO_AF(GPIOA_SPI_NSS, 5U) |\
                                     PIN_AFIO_AF(GPIOA_SPI_SCK, 5U) |\
                                     PIN_AFIO_AF(GPIOA_SPI_MISO, 5U) |\
                                     PIN_AFIO_AF(GPIOA_SPI_MOSI, 5U))
#define VAL_GPIOA_AFRH              (PIN_AFIO_AF(GPIOA_WIFI_NRST, 0U) |\
                                     PIN_AFIO_AF(GPIOA_UART_UC2WIF, 7U) |\
                                     PIN_AFIO_AF(GPIOA_UART_WIFI2UC, 7U) |\
                                     PIN_AFIO_AF(GPIOA_UART_WIFI2UC_TS, 7U) |\
                                     PIN_AFIO_AF(GPIOA_UART_UC2WIFI_TS, 7U) |\
                                     PIN_AFIO_AF(GPIOA_SWDIO, 0U) |\
                                     PIN_AFIO_AF(GPIOA_SWCLK, 0U) |\
                                     PIN_AFIO_AF(GPIOA_WIFI_WAKE, 0U))

/*
 * GPIOB setup:
 *
 * PB0  - PIN0
 * PB1  - PIN1
 * PB2  - PIN2
 * PB3  - PIN3
 * PB4  - PIN4
 * PB5  - PIN5
 * PB6  - PIN6
 * PB7  - PIN7
 * PB8  - PIN8
 * PB9  - PIN9
 * PB10 - PIN10
 * PB11 - PIN11
 * PB12 - PIN12
 * PB13 - OTG_HS_VBUS
 * PB14 - OTG_HS_DM
 * PB15 - OTG_HS_DP
 */
#define VAL_GPIOB_MODER             (PIN_MODE_INPUT(GPIOB_PIN0) |\
                                     PIN_MODE_INPUT(GPIOB_PIN1) |\
                                     PIN_MODE_INPUT(GPIOB_PIN2) |\
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
#define VAL_GPIOB_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOB_PIN0) |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN1) |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN2) |\
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
#define VAL_GPIOB_OSPEEDR           (PIN_OSPEED_VERYLOW(GPIOB_PIN0) |\
                                     PIN_OSPEED_VERYLOW(GPIOB_PIN1) |\
                                     PIN_OSPEED_VERYLOW(GPIOB_PIN2) |\
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
#define VAL_GPIOB_PUPDR             (PIN_PUPDR_FLOATING(GPIOB_PIN0) |\
                                     PIN_PUPDR_FLOATING(GPIOB_PIN1) |\
                                     PIN_PUPDR_FLOATING(GPIOB_PIN2) |\
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
#define VAL_GPIOB_ODR               (PIN_ODR_LOW(GPIOB_PIN0) |\
                                     PIN_ODR_LOW(GPIOB_PIN1) |\
                                     PIN_ODR_LOW(GPIOB_PIN2) |\
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
#define VAL_GPIOB_AFRL              (PIN_AFIO_AF(GPIOB_PIN0, 0U) |\
                                     PIN_AFIO_AF(GPIOB_PIN1, 0U) |\
                                     PIN_AFIO_AF(GPIOB_PIN2, 0U) |\
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
 * PC0  - RGB_R
 * PC1  - RGB_G
 * PC2  - RGB_B
 * PC3  - PIN3
 * PC4  - PIN4
 * PC5  - PIN5
 * PC6  - PIN6
 * PC7  - PIN7
 * PC8  - SD_D0
 * PC9  - SD_D1
 * PC10 - SD_D2
 * PC11 - SD_D3
 * PC12 - SD_CLK
 * PC13 - PIN13
 * PC14 - PIN14
 * PC15 - PIN15
 */
#define VAL_GPIOC_MODER             (PIN_MODE_INPUT(GPIOC_RGB_R) |\
                                     PIN_MODE_INPUT(GPIOC_RGB_G) |\
                                     PIN_MODE_INPUT(GPIOC_RGB_B) |\
                                     PIN_MODE_INPUT(GPIOC_PIN3) |\
                                     PIN_MODE_INPUT(GPIOC_PIN4) |\
                                     PIN_MODE_INPUT(GPIOC_PIN5) |\
                                     PIN_MODE_INPUT(GPIOC_PIN6) |\
                                     PIN_MODE_INPUT(GPIOC_PIN7) |\
                                     PIN_MODE_ALTERNATE(GPIOC_SD_D0) |\
                                     PIN_MODE_ALTERNATE(GPIOC_SD_D1) |\
                                     PIN_MODE_ALTERNATE(GPIOC_SD_D2) |\
                                     PIN_MODE_ALTERNATE(GPIOC_SD_D3) |\
                                     PIN_MODE_ALTERNATE(GPIOC_SD_CLK) |\
                                     PIN_MODE_INPUT(GPIOC_PIN13) |\
                                     PIN_MODE_INPUT(GPIOC_PIN14) |\
                                     PIN_MODE_INPUT(GPIOC_PIN15))
#define VAL_GPIOC_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOC_RGB_R) |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_RGB_G) |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_RGB_B) |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN3) |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN4) |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN5) |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN6) |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN7) |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_SD_D0) |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_SD_D1) |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_SD_D2) |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_SD_D3) |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_SD_CLK) |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN13) |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN14) |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN15))
#define VAL_GPIOC_OSPEEDR           (PIN_OSPEED_VERYLOW(GPIOC_RGB_R) |\
                                     PIN_OSPEED_VERYLOW(GPIOC_RGB_G) |\
                                     PIN_OSPEED_VERYLOW(GPIOC_RGB_B) |\
                                     PIN_OSPEED_VERYLOW(GPIOC_PIN3) |\
                                     PIN_OSPEED_VERYLOW(GPIOC_PIN4) |\
                                     PIN_OSPEED_VERYLOW(GPIOC_PIN5) |\
                                     PIN_OSPEED_VERYLOW(GPIOC_PIN6) |\
                                     PIN_OSPEED_VERYLOW(GPIOC_PIN7) |\
                                     PIN_OSPEED_HIGH(GPIOC_SD_D0) |\
                                     PIN_OSPEED_HIGH(GPIOC_SD_D1) |\
                                     PIN_OSPEED_HIGH(GPIOC_SD_D2) |\
                                     PIN_OSPEED_HIGH(GPIOC_SD_D3) |\
                                     PIN_OSPEED_HIGH(GPIOC_SD_CLK) |\
                                     PIN_OSPEED_VERYLOW(GPIOC_PIN13) |\
                                     PIN_OSPEED_VERYLOW(GPIOC_PIN14) |\
                                     PIN_OSPEED_VERYLOW(GPIOC_PIN15))
#define VAL_GPIOC_PUPDR             (PIN_PUPDR_FLOATING(GPIOC_RGB_R) |\
                                     PIN_PUPDR_FLOATING(GPIOC_RGB_G) |\
                                     PIN_PUPDR_FLOATING(GPIOC_RGB_B) |\
                                     PIN_PUPDR_FLOATING(GPIOC_PIN3) |\
                                     PIN_PUPDR_FLOATING(GPIOC_PIN4) |\
                                     PIN_PUPDR_FLOATING(GPIOC_PIN5) |\
                                     PIN_PUPDR_FLOATING(GPIOC_PIN6) |\
                                     PIN_PUPDR_FLOATING(GPIOC_PIN7) |\
                                     PIN_PUPDR_FLOATING(GPIOC_SD_D0) |\
                                     PIN_PUPDR_FLOATING(GPIOC_SD_D1) |\
                                     PIN_PUPDR_FLOATING(GPIOC_SD_D2) |\
                                     PIN_PUPDR_FLOATING(GPIOC_SD_D3) |\
                                     PIN_PUPDR_FLOATING(GPIOC_SD_CLK) |\
                                     PIN_PUPDR_FLOATING(GPIOC_PIN13) |\
                                     PIN_PUPDR_FLOATING(GPIOC_PIN14) |\
                                     PIN_PUPDR_FLOATING(GPIOC_PIN15))
#define VAL_GPIOC_ODR               (PIN_ODR_LOW(GPIOC_RGB_R) |\
                                     PIN_ODR_LOW(GPIOC_RGB_G) |\
                                     PIN_ODR_LOW(GPIOC_RGB_B) |\
                                     PIN_ODR_LOW(GPIOC_PIN3) |\
                                     PIN_ODR_LOW(GPIOC_PIN4) |\
                                     PIN_ODR_LOW(GPIOC_PIN5) |\
                                     PIN_ODR_LOW(GPIOC_PIN6) |\
                                     PIN_ODR_LOW(GPIOC_PIN7) |\
                                     PIN_ODR_LOW(GPIOC_SD_D0) |\
                                     PIN_ODR_LOW(GPIOC_SD_D1) |\
                                     PIN_ODR_LOW(GPIOC_SD_D2) |\
                                     PIN_ODR_LOW(GPIOC_SD_D3) |\
                                     PIN_ODR_LOW(GPIOC_SD_CLK) |\
                                     PIN_ODR_LOW(GPIOC_PIN13) |\
                                     PIN_ODR_LOW(GPIOC_PIN14) |\
                                     PIN_ODR_LOW(GPIOC_PIN15))
#define VAL_GPIOC_AFRL              (PIN_AFIO_AF(GPIOC_RGB_R, 0U) |\
                                     PIN_AFIO_AF(GPIOC_RGB_G, 0U) |\
                                     PIN_AFIO_AF(GPIOC_RGB_B, 0U) |\
                                     PIN_AFIO_AF(GPIOC_PIN3, 0U) |\
                                     PIN_AFIO_AF(GPIOC_PIN4, 0U) |\
                                     PIN_AFIO_AF(GPIOC_PIN5, 0U) |\
                                     PIN_AFIO_AF(GPIOC_PIN6, 0U) |\
                                     PIN_AFIO_AF(GPIOC_PIN7, 0U))
#define VAL_GPIOC_AFRH              (PIN_AFIO_AF(GPIOC_SD_D0, 12U) |\
                                     PIN_AFIO_AF(GPIOC_SD_D1, 12U) |\
                                     PIN_AFIO_AF(GPIOC_SD_D2, 12U) |\
                                     PIN_AFIO_AF(GPIOC_SD_D3, 12U) |\
                                     PIN_AFIO_AF(GPIOC_SD_CLK, 12U) |\
                                     PIN_AFIO_AF(GPIOC_PIN13, 0U) |\
                                     PIN_AFIO_AF(GPIOC_PIN14, 0U) |\
                                     PIN_AFIO_AF(GPIOC_PIN15, 0U))

/*
 * GPIOD setup:
 *
 * PD2  - SD_CMD
 */
#define VAL_GPIOD_MODER             (PIN_MODE_ALTERNATE(GPIOD_SD_CMD))
#define VAL_GPIOD_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOD_SD_CMD))
#define VAL_GPIOD_OSPEEDR           (PIN_OSPEED_HIGH(GPIOD_SD_CMD))
#define VAL_GPIOD_PUPDR             (PIN_PUPDR_FLOATING(GPIOD_SD_CMD))
#define VAL_GPIOD_ODR               (PIN_ODR_LOW(GPIOD_SD_CMD))
#define VAL_GPIOD_AFRL              (PIN_AFIO_AF(GPIOD_SD_CMD, 12U))
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
#define VAL_GPIOH_AFRL              (PIN_AFIO_AF(GPIOH_OSC_IN, 0U) |\
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
