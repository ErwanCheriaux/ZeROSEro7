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
#define BOARD_OLIMEX_STM32_E407_REV_D
#define BOARD_NAME                  "Olimex STM32-E407 (rev.D)"

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
#define STM32_HSECLK                12000000U
#endif

/*
 * Board voltages.
 * Required for performance limits calculation.
 */
#define STM32_VDD                   330U

/*
 * MCU type as defined in the ST header.
 */
#define STM32F407xx

/*
 * IO pins assignments.
 */
#define GPIOA_BUTTON_WKUP           0U
#define GPIOA_ETH_RMII_REF_CLK      1U
#define GPIOA_ETH_RMII_MDIO         2U
#define GPIOA_ETH_RMII_MDINT        3U
#define GPIOA_PIN4                  4U
#define GPIOA_PIN5                  5U
#define GPIOA_PIN6                  6U
#define GPIOA_ETH_RMII_CRS_DV       7U
#define GPIOA_USB_HS_BUSON          8U
#define GPIOA_OTG_FS_VBUS           9U
#define GPIOA_OTG_FS_ID             10U
#define GPIOA_OTG_FS_DM             11U
#define GPIOA_OTG_FS_DP             12U
#define GPIOA_JTAG_TMS              13U
#define GPIOA_JTAG_TCK              14U
#define GPIOA_JTAG_TDI              15U

#define GPIOB_USB_FS_BUSON          0U
#define GPIOB_USB_FS_FAULT          1U
#define GPIOB_BOOT1                 2U
#define GPIOB_JTAG_TDO              3U
#define GPIOB_JTAG_TRST             4U
#define GPIOB_PIN5                  5U
#define GPIOB_PIN6                  6U
#define GPIOB_PIN7                  7U
#define GPIOB_I2C1_SCL              8U
#define GPIOB_I2C1_SDA              9U
#define GPIOB_SPI2_SCK              10U
#define GPIOB_PIN11                 11U
#define GPIOB_OTG_HS_ID             12U
#define GPIOB_OTG_HS_VBUS           13U
#define GPIOB_OTG_HS_DM             14U
#define GPIOB_OTG_HS_DP             15U

#define GPIOC_PIN0                  0U
#define GPIOC_ETH_RMII_MDC          1U
#define GPIOC_SPI2_MISO             2U
#define GPIOC_SPI2_MOSI             3U
#define GPIOC_ETH_RMII_RXD0         4U
#define GPIOC_ETH_RMII_RXD1         5U
#define GPIOC_USART6_TX             6U
#define GPIOC_USART6_RX             7U
#define GPIOC_SD_D0                 8U
#define GPIOC_SD_D1                 9U
#define GPIOC_SD_D2                 10U
#define GPIOC_SD_D3                 11U
#define GPIOC_SD_CLK                12U
#define GPIOC_LED                   13U
#define GPIOC_OSC32_IN              14U
#define GPIOC_OSC32_OUT             15U

#define GPIOD_PIN0                  0U
#define GPIOD_PIN1                  1U
#define GPIOD_SD_CMD                2U
#define GPIOD_PIN3                  3U
#define GPIOD_PIN4                  4U
#define GPIOD_PIN5                  5U
#define GPIOD_PIN6                  6U
#define GPIOD_PIN7                  7U
#define GPIOD_PIN8                  8U
#define GPIOD_PIN9                  9U
#define GPIOD_PIN10                 10U
#define GPIOD_PIN11                 11U
#define GPIOD_PIN12                 12U
#define GPIOD_PIN13                 13U
#define GPIOD_PIN14                 14U
#define GPIOD_PIN15                 15U

#define GPIOE_PIN0                  0U
#define GPIOE_PIN1                  1U
#define GPIOE_PIN2                  2U
#define GPIOE_PIN3                  3U
#define GPIOE_PIN4                  4U
#define GPIOE_PIN5                  5U
#define GPIOE_PIN6                  6U
#define GPIOE_PIN7                  7U
#define GPIOE_PIN8                  8U
#define GPIOE_PIN9                  9U
#define GPIOE_PIN10                 10U
#define GPIOE_PIN11                 11U
#define GPIOE_PIN12                 12U
#define GPIOE_PIN13                 13U
#define GPIOE_PIN14                 14U
#define GPIOE_PIN15                 15U

#define GPIOF_PIN0                  0U
#define GPIOF_PIN1                  1U
#define GPIOF_PIN2                  2U
#define GPIOF_PIN3                  3U
#define GPIOF_PIN4                  4U
#define GPIOF_PIN5                  5U
#define GPIOF_PIN6                  6U
#define GPIOF_PIN7                  7U
#define GPIOF_PIN8                  8U
#define GPIOF_PIN9                  9U
#define GPIOF_PIN10                 10U
#define GPIOF_USB_HS_FAULT          11U
#define GPIOF_PIN12                 12U
#define GPIOF_PIN13                 13U
#define GPIOF_PIN14                 14U
#define GPIOF_PIN15                 15U

#define GPIOG_PIN0                  0U
#define GPIOG_PIN1                  1U
#define GPIOG_PIN2                  2U
#define GPIOG_PIN3                  3U
#define GPIOG_PIN4                  4U
#define GPIOG_PIN5                  5U
#define GPIOG_PIN6                  6U
#define GPIOG_PIN7                  7U
#define GPIOG_PIN8                  8U
#define GPIOG_PIN9                  9U
#define GPIOG_SPI2_CS               10U
#define GPIOG_ETH_RMII_TXEN         11U
#define GPIOG_PIN12                 12U
#define GPIOG_ETH_RMII_TXD0         13U
#define GPIOG_ETH_RMII_TXD1         14U
#define GPIOG_PIN15                 15U

#define GPIOH_OSC_IN                0U
#define GPIOH_OSC_OUT               1U
#define GPIOH_PIN2                  2U
#define GPIOH_PIN3                  3U
#define GPIOH_PIN4                  4U
#define GPIOH_PIN5                  5U
#define GPIOH_PIN6                  6U
#define GPIOH_PIN7                  7U
#define GPIOH_PIN8                  8U
#define GPIOH_PIN9                  9U
#define GPIOH_PIN10                 10U
#define GPIOH_PIN11                 11U
#define GPIOH_PIN12                 12U
#define GPIOH_PIN13                 13U
#define GPIOH_PIN14                 14U
#define GPIOH_PIN15                 15U

#define GPIOI_PIN0                  0U
#define GPIOI_PIN1                  1U
#define GPIOI_PIN2                  2U
#define GPIOI_PIN3                  3U
#define GPIOI_PIN4                  4U
#define GPIOI_PIN5                  5U
#define GPIOI_PIN6                  6U
#define GPIOI_PIN7                  7U
#define GPIOI_PIN8                  8U
#define GPIOI_PIN9                  9U
#define GPIOI_PIN10                 10U
#define GPIOI_PIN11                 11U
#define GPIOI_PIN12                 12U
#define GPIOI_PIN13                 13U
#define GPIOI_PIN14                 14U
#define GPIOI_PIN15                 15U

/*
 * IO lines assignments.
 */
#define LINE_BUTTON_WKUP            PAL_LINE(GPIOA, 0U)
#define LINE_ETH_RMII_REF_CLK       PAL_LINE(GPIOA, 1U)
#define LINE_ETH_RMII_MDIO          PAL_LINE(GPIOA, 2U)
#define LINE_ETH_RMII_MDINT         PAL_LINE(GPIOA, 3U)
#define LINE_ETH_RMII_CRS_DV        PAL_LINE(GPIOA, 7U)
#define LINE_USB_HS_BUSON           PAL_LINE(GPIOA, 8U)
#define LINE_OTG_FS_VBUS            PAL_LINE(GPIOA, 9U)
#define LINE_OTG_FS_ID              PAL_LINE(GPIOA, 10U)
#define LINE_OTG_FS_DM              PAL_LINE(GPIOA, 11U)
#define LINE_OTG_FS_DP              PAL_LINE(GPIOA, 12U)
#define LINE_JTAG_TMS               PAL_LINE(GPIOA, 13U)
#define LINE_JTAG_TCK               PAL_LINE(GPIOA, 14U)
#define LINE_JTAG_TDI               PAL_LINE(GPIOA, 15U)

#define LINE_USB_FS_BUSON           PAL_LINE(GPIOB, 0U)
#define LINE_USB_FS_FAULT           PAL_LINE(GPIOB, 1U)
#define LINE_BOOT1                  PAL_LINE(GPIOB, 2U)
#define LINE_JTAG_TDO               PAL_LINE(GPIOB, 3U)
#define LINE_JTAG_TRST              PAL_LINE(GPIOB, 4U)
#define LINE_I2C1_SCL               PAL_LINE(GPIOB, 8U)
#define LINE_I2C1_SDA               PAL_LINE(GPIOB, 9U)
#define LINE_SPI2_SCK               PAL_LINE(GPIOB, 10U)
#define LINE_OTG_HS_ID              PAL_LINE(GPIOB, 12U)
#define LINE_OTG_HS_VBUS            PAL_LINE(GPIOB, 13U)
#define LINE_OTG_HS_DM              PAL_LINE(GPIOB, 14U)
#define LINE_OTG_HS_DP              PAL_LINE(GPIOB, 15U)

#define LINE_ETH_RMII_MDC           PAL_LINE(GPIOC, 1U)
#define LINE_SPI2_MISO              PAL_LINE(GPIOC, 2U)
#define LINE_SPI2_MOSI              PAL_LINE(GPIOC, 3U)
#define LINE_ETH_RMII_RXD0          PAL_LINE(GPIOC, 4U)
#define LINE_ETH_RMII_RXD1          PAL_LINE(GPIOC, 5U)
#define LINE_USART6_TX              PAL_LINE(GPIOC, 6U)
#define LINE_USART6_RX              PAL_LINE(GPIOC, 7U)
#define LINE_SD_D0                  PAL_LINE(GPIOC, 8U)
#define LINE_SD_D1                  PAL_LINE(GPIOC, 9U)
#define LINE_SD_D2                  PAL_LINE(GPIOC, 10U)
#define LINE_SD_D3                  PAL_LINE(GPIOC, 11U)
#define LINE_SD_CLK                 PAL_LINE(GPIOC, 12U)
#define LINE_LED                    PAL_LINE(GPIOC, 13U)
#define LINE_OSC32_IN               PAL_LINE(GPIOC, 14U)
#define LINE_OSC32_OUT              PAL_LINE(GPIOC, 15U)

#define LINE_SD_CMD                 PAL_LINE(GPIOD, 2U)


#define LINE_USB_HS_FAULT           PAL_LINE(GPIOF, 11U)

#define LINE_SPI2_CS                PAL_LINE(GPIOG, 10U)
#define LINE_ETH_RMII_TXEN          PAL_LINE(GPIOG, 11U)
#define LINE_ETH_RMII_TXD0          PAL_LINE(GPIOG, 13U)
#define LINE_ETH_RMII_TXD1          PAL_LINE(GPIOG, 14U)

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
#define VAL_GPIOA_MODER             (PIN_MODE_INPUT(GPIOA_BUTTON_WKUP)      |\
                                     PIN_MODE_ALTERNATE(GPIOA_ETH_RMII_REF_CLK) |\
                                     PIN_MODE_ALTERNATE(GPIOA_ETH_RMII_MDIO)    |\
                                     PIN_MODE_INPUT(GPIOA_ETH_RMII_MDINT)   |\
                                     PIN_MODE_INPUT(GPIOA_PIN4)             |\
                                     PIN_MODE_INPUT(GPIOA_PIN5)             |\
                                     PIN_MODE_INPUT(GPIOA_PIN6)             |\
                                     PIN_MODE_ALTERNATE(GPIOA_ETH_RMII_CRS_DV)  |\
                                     PIN_MODE_INPUT(GPIOA_USB_HS_BUSON)     |\
                                     PIN_MODE_INPUT(GPIOA_OTG_FS_VBUS)      |\
                                     PIN_MODE_INPUT(GPIOA_OTG_FS_ID)        |\
                                     PIN_MODE_INPUT(GPIOA_OTG_FS_DM)        |\
                                     PIN_MODE_INPUT(GPIOA_OTG_FS_DP)        |\
                                     PIN_MODE_ALTERNATE(GPIOA_JTAG_TMS)     |\
                                     PIN_MODE_ALTERNATE(GPIOA_JTAG_TCK)     |\
                                     PIN_MODE_ALTERNATE(GPIOA_JTAG_TDI))
#define VAL_GPIOA_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOA_BUTTON_WKUP)      |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_ETH_RMII_REF_CLK) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_ETH_RMII_MDIO)    |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_ETH_RMII_MDINT)   |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN4)             |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN5)             |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN6)             |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_ETH_RMII_CRS_DV)  |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_USB_HS_BUSON)     |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_OTG_FS_VBUS)      |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_OTG_FS_ID)        |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_OTG_FS_DM)        |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_OTG_FS_DP)        |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_JTAG_TMS)         |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_JTAG_TCK)         |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_JTAG_TDI))
#define VAL_GPIOA_OSPEEDR           (PIN_OSPEED_VERYLOW(GPIOA_BUTTON_WKUP)      |\
                                     PIN_OSPEED_HIGH(GPIOA_ETH_RMII_REF_CLK)    |\
                                     PIN_OSPEED_HIGH(GPIOA_ETH_RMII_MDIO)       |\
                                     PIN_OSPEED_HIGH(GPIOA_ETH_RMII_MDINT)      |\
                                     PIN_OSPEED_VERYLOW(GPIOA_PIN4)             |\
                                     PIN_OSPEED_VERYLOW(GPIOA_PIN5)             |\
                                     PIN_OSPEED_VERYLOW(GPIOA_PIN6)             |\
                                     PIN_OSPEED_HIGH(GPIOA_ETH_RMII_CRS_DV)     |\
                                     PIN_OSPEED_HIGH(GPIOA_USB_HS_BUSON)        |\
                                     PIN_OSPEED_HIGH(GPIOA_OTG_FS_VBUS)         |\
                                     PIN_OSPEED_HIGH(GPIOA_OTG_FS_ID)           |\
                                     PIN_OSPEED_HIGH(GPIOA_OTG_FS_DM)           |\
                                     PIN_OSPEED_HIGH(GPIOA_OTG_FS_DP)           |\
                                     PIN_OSPEED_VERYLOW(GPIOA_JTAG_TMS)         |\
                                     PIN_OSPEED_VERYLOW(GPIOA_JTAG_TCK)         |\
                                     PIN_OSPEED_VERYLOW(GPIOA_JTAG_TDI))
#define VAL_GPIOA_PUPDR             (PIN_PUPDR_FLOATING(GPIOA_BUTTON_WKUP)      |\
                                     PIN_PUPDR_FLOATING(GPIOA_ETH_RMII_REF_CLK) |\
                                     PIN_PUPDR_FLOATING(GPIOA_ETH_RMII_MDIO)    |\
                                     PIN_PUPDR_FLOATING(GPIOA_ETH_RMII_MDINT)   |\
                                     PIN_PUPDR_FLOATING(GPIOA_PIN4)             |\
                                     PIN_PUPDR_FLOATING(GPIOA_PIN5)             |\
                                     PIN_PUPDR_FLOATING(GPIOA_PIN6)             |\
                                     PIN_PUPDR_FLOATING(GPIOA_ETH_RMII_CRS_DV)  |\
                                     PIN_PUPDR_FLOATING(GPIOA_USB_HS_BUSON)     |\
                                     PIN_PUPDR_FLOATING(GPIOA_OTG_FS_VBUS)      |\
                                     PIN_PUPDR_FLOATING(GPIOA_OTG_FS_ID)        |\
                                     PIN_PUPDR_FLOATING(GPIOA_OTG_FS_DM)        |\
                                     PIN_PUPDR_FLOATING(GPIOA_OTG_FS_DP)        |\
                                     PIN_PUPDR_PULLUP(GPIOA_JTAG_TMS)           |\
                                     PIN_PUPDR_PULLDOWN(GPIOA_JTAG_TCK)         |\
                                     PIN_PUPDR_PULLUP(GPIOA_JTAG_TDI))
#define VAL_GPIOA_ODR               (PIN_ODR_LOW(GPIOA_BUTTON_WKUP)      |\
                                     PIN_ODR_HIGH(GPIOA_ETH_RMII_REF_CLK) |\
                                     PIN_ODR_HIGH(GPIOA_ETH_RMII_MDIO)    |\
                                     PIN_ODR_HIGH(GPIOA_ETH_RMII_MDINT)   |\
                                     PIN_ODR_LOW(GPIOA_PIN4)             |\
                                     PIN_ODR_LOW(GPIOA_PIN5)             |\
                                     PIN_ODR_LOW(GPIOA_PIN6)             |\
                                     PIN_ODR_HIGH(GPIOA_ETH_RMII_CRS_DV)  |\
                                     PIN_ODR_LOW(GPIOA_USB_HS_BUSON)     |\
                                     PIN_ODR_LOW(GPIOA_OTG_FS_VBUS)      |\
                                     PIN_ODR_LOW(GPIOA_OTG_FS_ID)        |\
                                     PIN_ODR_LOW(GPIOA_OTG_FS_DM)        |\
                                     PIN_ODR_LOW(GPIOA_OTG_FS_DP)        |\
                                     PIN_ODR_LOW(GPIOA_JTAG_TMS)         |\
                                     PIN_ODR_LOW(GPIOA_JTAG_TCK)         |\
                                     PIN_ODR_LOW(GPIOA_JTAG_TDI))
#define VAL_GPIOA_AFRL              (PIN_AFIO_AF(GPIOA_BUTTON_WKUP,      0U) |\
                                     PIN_AFIO_AF(GPIOA_ETH_RMII_REF_CLK, 11U) |\
                                     PIN_AFIO_AF(GPIOA_ETH_RMII_MDIO,    11U) |\
                                     PIN_AFIO_AF(GPIOA_ETH_RMII_MDINT,   0U) |\
                                     PIN_AFIO_AF(GPIOA_PIN4,             0U) |\
                                     PIN_AFIO_AF(GPIOA_PIN5,             0U) |\
                                     PIN_AFIO_AF(GPIOA_PIN6,             0U) |\
                                     PIN_AFIO_AF(GPIOA_ETH_RMII_CRS_DV,  11U))
#define VAL_GPIOA_AFRH              (PIN_AFIO_AF(GPIOA_USB_HS_BUSON, 0U) |\
                                     PIN_AFIO_AF(GPIOA_OTG_FS_VBUS,  0U) |\
                                     PIN_AFIO_AF(GPIOA_OTG_FS_ID,    0U) |\
                                     PIN_AFIO_AF(GPIOA_OTG_FS_DM,    0U) |\
                                     PIN_AFIO_AF(GPIOA_OTG_FS_DP,    0U) |\
                                     PIN_AFIO_AF(GPIOA_JTAG_TMS,     0U) |\
                                     PIN_AFIO_AF(GPIOA_JTAG_TCK,     0U) |\
                                     PIN_AFIO_AF(GPIOA_JTAG_TDI,     0U))

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
#define VAL_GPIOB_MODER             (PIN_MODE_INPUT(GPIOB_USB_FS_BUSON)  |\
                                     PIN_MODE_INPUT(GPIOB_USB_FS_FAULT)  |\
                                     PIN_MODE_INPUT(GPIOB_BOOT1)         |\
                                     PIN_MODE_ALTERNATE(GPIOB_JTAG_TDO)  |\
                                     PIN_MODE_ALTERNATE(GPIOB_JTAG_TRST) |\
                                     PIN_MODE_INPUT(GPIOB_PIN5)          |\
                                     PIN_MODE_INPUT(GPIOB_PIN6)          |\
                                     PIN_MODE_INPUT(GPIOB_PIN7)          |\
                                     PIN_MODE_INPUT(GPIOB_I2C1_SCL)      |\
                                     PIN_MODE_INPUT(GPIOB_I2C1_SDA)      |\
                                     PIN_MODE_INPUT(GPIOB_SPI2_SCK)      |\
                                     PIN_MODE_INPUT(GPIOB_PIN11)         |\
                                     PIN_MODE_ALTERNATE(GPIOB_OTG_HS_ID)     |\
                                     PIN_MODE_INPUT(GPIOB_OTG_HS_VBUS)   |\
                                     PIN_MODE_ALTERNATE(GPIOB_OTG_HS_DM)     |\
                                     PIN_MODE_ALTERNATE(GPIOB_OTG_HS_DP))
#define VAL_GPIOB_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOB_USB_FS_BUSON) |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_USB_FS_FAULT) |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_BOOT1)        |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_JTAG_TDO)     |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_JTAG_TRST)    |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN5)         |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN6)         |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN7)         |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_I2C1_SCL)     |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_I2C1_SDA)     |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_SPI2_SCK)     |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN11)        |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_OTG_HS_ID)    |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_OTG_HS_VBUS)  |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_OTG_HS_DM)    |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_OTG_HS_DP))
#define VAL_GPIOB_OSPEEDR           (PIN_OSPEED_HIGH(GPIOB_USB_FS_BUSON)    |\
                                     PIN_OSPEED_HIGH(GPIOB_USB_FS_FAULT)    |\
                                     PIN_OSPEED_VERYLOW(GPIOB_BOOT1)        |\
                                     PIN_OSPEED_HIGH(GPIOB_JTAG_TDO)        |\
                                     PIN_OSPEED_VERYLOW(GPIOB_JTAG_TRST)    |\
                                     PIN_OSPEED_VERYLOW(GPIOB_PIN5)         |\
                                     PIN_OSPEED_VERYLOW(GPIOB_PIN6)         |\
                                     PIN_OSPEED_VERYLOW(GPIOB_PIN7)         |\
                                     PIN_OSPEED_VERYLOW(GPIOB_I2C1_SCL)     |\
                                     PIN_OSPEED_VERYLOW(GPIOB_I2C1_SDA)     |\
                                     PIN_OSPEED_VERYLOW(GPIOB_SPI2_SCK)     |\
                                     PIN_OSPEED_VERYLOW(GPIOB_PIN11)        |\
                                     PIN_OSPEED_HIGH(GPIOB_OTG_HS_ID)    |\
                                     PIN_OSPEED_HIGH(GPIOB_OTG_HS_VBUS)  |\
                                     PIN_OSPEED_HIGH(GPIOB_OTG_HS_DM)    |\
                                     PIN_OSPEED_HIGH(GPIOB_OTG_HS_DP))
#define VAL_GPIOB_PUPDR             (PIN_PUPDR_FLOATING(GPIOB_USB_FS_BUSON) |\
                                     PIN_PUPDR_FLOATING(GPIOB_USB_FS_FAULT) |\
                                     PIN_PUPDR_FLOATING(GPIOB_BOOT1)        |\
                                     PIN_PUPDR_FLOATING(GPIOB_JTAG_TDO)     |\
                                     PIN_PUPDR_PULLUP(GPIOB_JTAG_TRST)      |\
                                     PIN_PUPDR_FLOATING(GPIOB_PIN5)         |\
                                     PIN_PUPDR_FLOATING(GPIOB_PIN6)         |\
                                     PIN_PUPDR_FLOATING(GPIOB_PIN7)         |\
                                     PIN_PUPDR_FLOATING(GPIOB_I2C1_SCL)     |\
                                     PIN_PUPDR_FLOATING(GPIOB_I2C1_SDA)     |\
                                     PIN_PUPDR_FLOATING(GPIOB_SPI2_SCK)     |\
                                     PIN_PUPDR_FLOATING(GPIOB_PIN11)        |\
                                     PIN_PUPDR_FLOATING(GPIOB_OTG_HS_ID)    |\
                                     PIN_PUPDR_FLOATING(GPIOB_OTG_HS_VBUS)  |\
                                     PIN_PUPDR_FLOATING(GPIOB_OTG_HS_DM)    |\
                                     PIN_PUPDR_FLOATING(GPIOB_OTG_HS_DP))
#define VAL_GPIOB_ODR               (PIN_ODR_LOW(GPIOB_USB_FS_BUSON) |\
                                     PIN_ODR_LOW(GPIOB_USB_FS_FAULT) |\
                                     PIN_ODR_LOW(GPIOB_BOOT1)        |\
                                     PIN_ODR_LOW(GPIOB_JTAG_TDO)     |\
                                     PIN_ODR_LOW(GPIOB_JTAG_TRST)    |\
                                     PIN_ODR_LOW(GPIOB_PIN5)         |\
                                     PIN_ODR_LOW(GPIOB_PIN6)         |\
                                     PIN_ODR_LOW(GPIOB_PIN7)         |\
                                     PIN_ODR_LOW(GPIOB_I2C1_SCL)     |\
                                     PIN_ODR_LOW(GPIOB_I2C1_SDA)     |\
                                     PIN_ODR_LOW(GPIOB_SPI2_SCK)     |\
                                     PIN_ODR_LOW(GPIOB_PIN11)        |\
                                     PIN_ODR_HIGH(GPIOB_OTG_HS_ID)    |\
                                     PIN_ODR_HIGH(GPIOB_OTG_HS_VBUS)  |\
                                     PIN_ODR_HIGH(GPIOB_OTG_HS_DM)    |\
                                     PIN_ODR_HIGH(GPIOB_OTG_HS_DP))
#define VAL_GPIOB_AFRL              (PIN_AFIO_AF(GPIOB_USB_FS_BUSON, 0U) |\
                                     PIN_AFIO_AF(GPIOB_USB_FS_FAULT, 0U) |\
                                     PIN_AFIO_AF(GPIOB_BOOT1,        0U) |\
                                     PIN_AFIO_AF(GPIOB_JTAG_TDO,     0U) |\
                                     PIN_AFIO_AF(GPIOB_JTAG_TRST,    0U) |\
                                     PIN_AFIO_AF(GPIOB_PIN5,         0U) |\
                                     PIN_AFIO_AF(GPIOB_PIN6,         0U) |\
                                     PIN_AFIO_AF(GPIOB_PIN7,         0U))
#define VAL_GPIOB_AFRH              (PIN_AFIO_AF(GPIOB_I2C1_SCL,    0U) |\
                                     PIN_AFIO_AF(GPIOB_I2C1_SDA,    0U) |\
                                     PIN_AFIO_AF(GPIOB_SPI2_SCK,    0U) |\
                                     PIN_AFIO_AF(GPIOB_PIN11,       0U) |\
                                     PIN_AFIO_AF(GPIOB_OTG_HS_ID,  10U) |\
                                     PIN_AFIO_AF(GPIOB_OTG_HS_VBUS, 0U) |\
                                     PIN_AFIO_AF(GPIOB_OTG_HS_DM,  10U) |\
                                     PIN_AFIO_AF(GPIOB_OTG_HS_DP,  10U))

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
#define VAL_GPIOC_MODER             (PIN_MODE_INPUT(GPIOC_PIN0)          |\
                                     PIN_MODE_ALTERNATE(GPIOC_ETH_RMII_MDC)  |\
                                     PIN_MODE_INPUT(GPIOC_SPI2_MISO)     |\
                                     PIN_MODE_INPUT(GPIOC_SPI2_MOSI)     |\
                                     PIN_MODE_ALTERNATE(GPIOC_ETH_RMII_RXD0) |\
                                     PIN_MODE_ALTERNATE(GPIOC_ETH_RMII_RXD1) |\
                                     PIN_MODE_INPUT(GPIOC_USART6_TX)     |\
                                     PIN_MODE_INPUT(GPIOC_USART6_RX)     |\
                                     PIN_MODE_INPUT(GPIOC_SD_D0)         |\
                                     PIN_MODE_INPUT(GPIOC_SD_D1)         |\
                                     PIN_MODE_INPUT(GPIOC_SD_D2)         |\
                                     PIN_MODE_INPUT(GPIOC_SD_D3)         |\
                                     PIN_MODE_INPUT(GPIOC_SD_CLK)        |\
                                     PIN_MODE_INPUT(GPIOC_LED)           |\
                                     PIN_MODE_INPUT(GPIOC_OSC32_IN)      |\
                                     PIN_MODE_INPUT(GPIOC_OSC32_OUT))
#define VAL_GPIOC_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOC_PIN0)          |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_ETH_RMII_MDC)  |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_SPI2_MISO)     |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_SPI2_MOSI)     |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_ETH_RMII_RXD0) |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_ETH_RMII_RXD1) |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_USART6_TX)     |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_USART6_RX)     |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_SD_D0)         |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_SD_D1)         |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_SD_D2)         |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_SD_D3)         |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_SD_CLK)        |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_LED)           |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_OSC32_IN)      |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_OSC32_OUT))
#define VAL_GPIOC_OSPEEDR           (PIN_OSPEED_VERYLOW(GPIOC_PIN0)          |\
                                     PIN_OSPEED_HIGH(GPIOC_ETH_RMII_MDC)     |\
                                     PIN_OSPEED_VERYLOW(GPIOC_SPI2_MISO)     |\
                                     PIN_OSPEED_VERYLOW(GPIOC_SPI2_MOSI)     |\
                                     PIN_OSPEED_HIGH(GPIOC_ETH_RMII_RXD0)    |\
                                     PIN_OSPEED_HIGH(GPIOC_ETH_RMII_RXD1)    |\
                                     PIN_OSPEED_VERYLOW(GPIOC_USART6_TX)     |\
                                     PIN_OSPEED_VERYLOW(GPIOC_USART6_RX)     |\
                                     PIN_OSPEED_VERYLOW(GPIOC_SD_D0)         |\
                                     PIN_OSPEED_VERYLOW(GPIOC_SD_D1)         |\
                                     PIN_OSPEED_VERYLOW(GPIOC_SD_D2)         |\
                                     PIN_OSPEED_VERYLOW(GPIOC_SD_D3)         |\
                                     PIN_OSPEED_VERYLOW(GPIOC_SD_CLK)        |\
                                     PIN_OSPEED_VERYLOW(GPIOC_LED)           |\
                                     PIN_OSPEED_VERYLOW(GPIOC_OSC32_IN)      |\
                                     PIN_OSPEED_VERYLOW(GPIOC_OSC32_OUT))
#define VAL_GPIOC_PUPDR             (PIN_PUPDR_FLOATING(GPIOC_PIN0)          |\
                                     PIN_PUPDR_FLOATING(GPIOC_ETH_RMII_MDC)  |\
                                     PIN_PUPDR_FLOATING(GPIOC_SPI2_MISO)     |\
                                     PIN_PUPDR_FLOATING(GPIOC_SPI2_MOSI)     |\
                                     PIN_PUPDR_FLOATING(GPIOC_ETH_RMII_RXD0) |\
                                     PIN_PUPDR_FLOATING(GPIOC_ETH_RMII_RXD1) |\
                                     PIN_PUPDR_FLOATING(GPIOC_USART6_TX)     |\
                                     PIN_PUPDR_FLOATING(GPIOC_USART6_RX)     |\
                                     PIN_PUPDR_FLOATING(GPIOC_SD_D0)         |\
                                     PIN_PUPDR_FLOATING(GPIOC_SD_D1)         |\
                                     PIN_PUPDR_FLOATING(GPIOC_SD_D2)         |\
                                     PIN_PUPDR_FLOATING(GPIOC_SD_D3)         |\
                                     PIN_PUPDR_FLOATING(GPIOC_SD_CLK)        |\
                                     PIN_PUPDR_FLOATING(GPIOC_LED)           |\
                                     PIN_PUPDR_FLOATING(GPIOC_OSC32_IN)      |\
                                     PIN_PUPDR_FLOATING(GPIOC_OSC32_OUT))
#define VAL_GPIOC_ODR               (PIN_ODR_LOW(GPIOC_PIN0)          |\
                                     PIN_ODR_HIGH(GPIOC_ETH_RMII_MDC)  |\
                                     PIN_ODR_LOW(GPIOC_SPI2_MISO)     |\
                                     PIN_ODR_LOW(GPIOC_SPI2_MOSI)     |\
                                     PIN_ODR_HIGH(GPIOC_ETH_RMII_RXD0) |\
                                     PIN_ODR_HIGH(GPIOC_ETH_RMII_RXD1) |\
                                     PIN_ODR_LOW(GPIOC_USART6_TX)     |\
                                     PIN_ODR_LOW(GPIOC_USART6_RX)     |\
                                     PIN_ODR_LOW(GPIOC_SD_D0)         |\
                                     PIN_ODR_LOW(GPIOC_SD_D1)         |\
                                     PIN_ODR_LOW(GPIOC_SD_D2)         |\
                                     PIN_ODR_LOW(GPIOC_SD_D3)         |\
                                     PIN_ODR_LOW(GPIOC_SD_CLK)        |\
                                     PIN_ODR_LOW(GPIOC_LED)           |\
                                     PIN_ODR_LOW(GPIOC_OSC32_IN)      |\
                                     PIN_ODR_LOW(GPIOC_OSC32_OUT))
#define VAL_GPIOC_AFRL              (PIN_AFIO_AF(GPIOC_PIN0,          0U) |\
                                     PIN_AFIO_AF(GPIOC_ETH_RMII_MDC, 11U) |\
                                     PIN_AFIO_AF(GPIOC_SPI2_MISO,     0U) |\
                                     PIN_AFIO_AF(GPIOC_SPI2_MOSI,     0U) |\
                                     PIN_AFIO_AF(GPIOC_ETH_RMII_RXD0, 11U) |\
                                     PIN_AFIO_AF(GPIOC_ETH_RMII_RXD1, 11U) |\
                                     PIN_AFIO_AF(GPIOC_USART6_TX,     0U) |\
                                     PIN_AFIO_AF(GPIOC_USART6_RX,     0U))
#define VAL_GPIOC_AFRH              (PIN_AFIO_AF(GPIOC_SD_D0,     0U) |\
                                     PIN_AFIO_AF(GPIOC_SD_D1,     0U) |\
                                     PIN_AFIO_AF(GPIOC_SD_D2,     0U) |\
                                     PIN_AFIO_AF(GPIOC_SD_D3,     0U) |\
                                     PIN_AFIO_AF(GPIOC_SD_CLK,    0U) |\
                                     PIN_AFIO_AF(GPIOC_LED,       0U) |\
                                     PIN_AFIO_AF(GPIOC_OSC32_IN,  0U) |\
                                     PIN_AFIO_AF(GPIOC_OSC32_OUT, 0U))

/*
 * GPIOD setup:
 *
 * PD0  - PIN0
 * PD1  - PIN1
 * PD2  - SD_CMD
 * PD3  - PIN3
 * PD4  - PIN4
 * PD5  - PIN5
 * PD6  - PIN6
 * PD7  - PIN7
 * PD8  - PIN8
 * PD9  - PIN9
 * PD10 - PIN10
 * PD11 - PIN11
 * PD12 - PIN12
 * PD13 - PIN13
 * PD14 - PIN14
 * PD15 - PIN15
 */
#define VAL_GPIOD_MODER             (PIN_MODE_INPUT(GPIOD_PIN0)   |\
                                     PIN_MODE_INPUT(GPIOD_PIN1)   |\
                                     PIN_MODE_INPUT(GPIOD_SD_CMD) |\
                                     PIN_MODE_INPUT(GPIOD_PIN3)   |\
                                     PIN_MODE_INPUT(GPIOD_PIN4)   |\
                                     PIN_MODE_INPUT(GPIOD_PIN5)   |\
                                     PIN_MODE_INPUT(GPIOD_PIN6)   |\
                                     PIN_MODE_INPUT(GPIOD_PIN7)   |\
                                     PIN_MODE_INPUT(GPIOD_PIN8)   |\
                                     PIN_MODE_INPUT(GPIOD_PIN9)   |\
                                     PIN_MODE_INPUT(GPIOD_PIN10)  |\
                                     PIN_MODE_INPUT(GPIOD_PIN11)  |\
                                     PIN_MODE_INPUT(GPIOD_PIN12)  |\
                                     PIN_MODE_INPUT(GPIOD_PIN13)  |\
                                     PIN_MODE_INPUT(GPIOD_PIN14)  |\
                                     PIN_MODE_INPUT(GPIOD_PIN15))
#define VAL_GPIOD_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOD_PIN0)   |\
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN1)   |\
                                     PIN_OTYPE_PUSHPULL(GPIOD_SD_CMD) |\
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN3)   |\
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN4)   |\
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN5)   |\
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN6)   |\
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN7)   |\
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN8)   |\
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN9)   |\
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN10)  |\
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN11)  |\
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN12)  |\
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN13)  |\
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN14)  |\
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN15))
#define VAL_GPIOD_OSPEEDR           (PIN_OSPEED_VERYLOW(GPIOD_PIN0)   |\
                                     PIN_OSPEED_VERYLOW(GPIOD_PIN1)   |\
                                     PIN_OSPEED_VERYLOW(GPIOD_SD_CMD) |\
                                     PIN_OSPEED_VERYLOW(GPIOD_PIN3)   |\
                                     PIN_OSPEED_VERYLOW(GPIOD_PIN4)   |\
                                     PIN_OSPEED_VERYLOW(GPIOD_PIN5)   |\
                                     PIN_OSPEED_VERYLOW(GPIOD_PIN6)   |\
                                     PIN_OSPEED_VERYLOW(GPIOD_PIN7)   |\
                                     PIN_OSPEED_VERYLOW(GPIOD_PIN8)   |\
                                     PIN_OSPEED_VERYLOW(GPIOD_PIN9)   |\
                                     PIN_OSPEED_VERYLOW(GPIOD_PIN10)  |\
                                     PIN_OSPEED_VERYLOW(GPIOD_PIN11)  |\
                                     PIN_OSPEED_VERYLOW(GPIOD_PIN12)  |\
                                     PIN_OSPEED_VERYLOW(GPIOD_PIN13)  |\
                                     PIN_OSPEED_VERYLOW(GPIOD_PIN14)  |\
                                     PIN_OSPEED_VERYLOW(GPIOD_PIN15))
#define VAL_GPIOD_PUPDR             (PIN_PUPDR_FLOATING(GPIOD_PIN0)   |\
                                     PIN_PUPDR_FLOATING(GPIOD_PIN1)   |\
                                     PIN_PUPDR_FLOATING(GPIOD_SD_CMD) |\
                                     PIN_PUPDR_FLOATING(GPIOD_PIN3)   |\
                                     PIN_PUPDR_FLOATING(GPIOD_PIN4)   |\
                                     PIN_PUPDR_FLOATING(GPIOD_PIN5)   |\
                                     PIN_PUPDR_FLOATING(GPIOD_PIN6)   |\
                                     PIN_PUPDR_FLOATING(GPIOD_PIN7)   |\
                                     PIN_PUPDR_FLOATING(GPIOD_PIN8)   |\
                                     PIN_PUPDR_FLOATING(GPIOD_PIN9)   |\
                                     PIN_PUPDR_FLOATING(GPIOD_PIN10)  |\
                                     PIN_PUPDR_FLOATING(GPIOD_PIN11)  |\
                                     PIN_PUPDR_FLOATING(GPIOD_PIN12)  |\
                                     PIN_PUPDR_FLOATING(GPIOD_PIN13)  |\
                                     PIN_PUPDR_FLOATING(GPIOD_PIN14)  |\
                                     PIN_PUPDR_FLOATING(GPIOD_PIN15))
#define VAL_GPIOD_ODR               (PIN_ODR_LOW(GPIOD_PIN0)   |\
                                     PIN_ODR_LOW(GPIOD_PIN1)   |\
                                     PIN_ODR_LOW(GPIOD_SD_CMD) |\
                                     PIN_ODR_LOW(GPIOD_PIN3)   |\
                                     PIN_ODR_LOW(GPIOD_PIN4)   |\
                                     PIN_ODR_LOW(GPIOD_PIN5)   |\
                                     PIN_ODR_LOW(GPIOD_PIN6)   |\
                                     PIN_ODR_LOW(GPIOD_PIN7)   |\
                                     PIN_ODR_LOW(GPIOD_PIN8)   |\
                                     PIN_ODR_LOW(GPIOD_PIN9)   |\
                                     PIN_ODR_LOW(GPIOD_PIN10)  |\
                                     PIN_ODR_LOW(GPIOD_PIN11)  |\
                                     PIN_ODR_LOW(GPIOD_PIN12)  |\
                                     PIN_ODR_LOW(GPIOD_PIN13)  |\
                                     PIN_ODR_LOW(GPIOD_PIN14)  |\
                                     PIN_ODR_LOW(GPIOD_PIN15))
#define VAL_GPIOD_AFRL              (PIN_AFIO_AF(GPIOD_PIN0,   0U) |\
                                     PIN_AFIO_AF(GPIOD_PIN1,   0U) |\
                                     PIN_AFIO_AF(GPIOD_SD_CMD, 0U) |\
                                     PIN_AFIO_AF(GPIOD_PIN3,   0U) |\
                                     PIN_AFIO_AF(GPIOD_PIN4,   0U) |\
                                     PIN_AFIO_AF(GPIOD_PIN5,   0U) |\
                                     PIN_AFIO_AF(GPIOD_PIN6,   0U) |\
                                     PIN_AFIO_AF(GPIOD_PIN7,   0U))
#define VAL_GPIOD_AFRH              (PIN_AFIO_AF(GPIOD_PIN8,  0U) |\
                                     PIN_AFIO_AF(GPIOD_PIN9,  0U) |\
                                     PIN_AFIO_AF(GPIOD_PIN10, 0U) |\
                                     PIN_AFIO_AF(GPIOD_PIN11, 0U) |\
                                     PIN_AFIO_AF(GPIOD_PIN12, 0U) |\
                                     PIN_AFIO_AF(GPIOD_PIN13, 0U) |\
                                     PIN_AFIO_AF(GPIOD_PIN14, 0U) |\
                                     PIN_AFIO_AF(GPIOD_PIN15, 0U))

/*
 * GPIOE setup:
 *
 * PE0  - PIN0
 * PE1  - PIN1
 * PE2  - PIN2
 * PE3  - PIN3
 * PE4  - PIN4
 * PE5  - PIN5
 * PE6  - PIN6
 * PE7  - PIN7
 * PE8  - PIN8
 * PE9  - PIN9
 * PE10 - PIN10
 * PE11 - PIN11
 * PE12 - PIN12
 * PE13 - PIN13
 * PE14 - PIN14
 * PE15 - PIN15
 */
#define VAL_GPIOE_MODER             (PIN_MODE_INPUT(GPIOE_PIN0)  |\
                                     PIN_MODE_INPUT(GPIOE_PIN1)  |\
                                     PIN_MODE_INPUT(GPIOE_PIN2)  |\
                                     PIN_MODE_INPUT(GPIOE_PIN3)  |\
                                     PIN_MODE_INPUT(GPIOE_PIN4)  |\
                                     PIN_MODE_INPUT(GPIOE_PIN5)  |\
                                     PIN_MODE_INPUT(GPIOE_PIN6)  |\
                                     PIN_MODE_INPUT(GPIOE_PIN7)  |\
                                     PIN_MODE_INPUT(GPIOE_PIN8)  |\
                                     PIN_MODE_INPUT(GPIOE_PIN9)  |\
                                     PIN_MODE_INPUT(GPIOE_PIN10) |\
                                     PIN_MODE_INPUT(GPIOE_PIN11) |\
                                     PIN_MODE_INPUT(GPIOE_PIN12) |\
                                     PIN_MODE_INPUT(GPIOE_PIN13) |\
                                     PIN_MODE_INPUT(GPIOE_PIN14) |\
                                     PIN_MODE_INPUT(GPIOE_PIN15))
#define VAL_GPIOE_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOE_PIN0)  |\
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN1)  |\
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN2)  |\
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN3)  |\
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN4)  |\
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN5)  |\
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN6)  |\
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN7)  |\
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN8)  |\
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN9)  |\
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN10) |\
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN11) |\
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN12) |\
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN13) |\
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN14) |\
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN15))
#define VAL_GPIOE_OSPEEDR           (PIN_OSPEED_VERYLOW(GPIOE_PIN0)  |\
                                     PIN_OSPEED_VERYLOW(GPIOE_PIN1)  |\
                                     PIN_OSPEED_VERYLOW(GPIOE_PIN2)  |\
                                     PIN_OSPEED_VERYLOW(GPIOE_PIN3)  |\
                                     PIN_OSPEED_VERYLOW(GPIOE_PIN4)  |\
                                     PIN_OSPEED_VERYLOW(GPIOE_PIN5)  |\
                                     PIN_OSPEED_VERYLOW(GPIOE_PIN6)  |\
                                     PIN_OSPEED_VERYLOW(GPIOE_PIN7)  |\
                                     PIN_OSPEED_VERYLOW(GPIOE_PIN8)  |\
                                     PIN_OSPEED_VERYLOW(GPIOE_PIN9)  |\
                                     PIN_OSPEED_VERYLOW(GPIOE_PIN10) |\
                                     PIN_OSPEED_VERYLOW(GPIOE_PIN11) |\
                                     PIN_OSPEED_VERYLOW(GPIOE_PIN12) |\
                                     PIN_OSPEED_VERYLOW(GPIOE_PIN13) |\
                                     PIN_OSPEED_VERYLOW(GPIOE_PIN14) |\
                                     PIN_OSPEED_VERYLOW(GPIOE_PIN15))
#define VAL_GPIOE_PUPDR             (PIN_PUPDR_FLOATING(GPIOE_PIN0)  |\
                                     PIN_PUPDR_FLOATING(GPIOE_PIN1)  |\
                                     PIN_PUPDR_FLOATING(GPIOE_PIN2)  |\
                                     PIN_PUPDR_FLOATING(GPIOE_PIN3)  |\
                                     PIN_PUPDR_FLOATING(GPIOE_PIN4)  |\
                                     PIN_PUPDR_FLOATING(GPIOE_PIN5)  |\
                                     PIN_PUPDR_FLOATING(GPIOE_PIN6)  |\
                                     PIN_PUPDR_FLOATING(GPIOE_PIN7)  |\
                                     PIN_PUPDR_FLOATING(GPIOE_PIN8)  |\
                                     PIN_PUPDR_FLOATING(GPIOE_PIN9)  |\
                                     PIN_PUPDR_FLOATING(GPIOE_PIN10) |\
                                     PIN_PUPDR_FLOATING(GPIOE_PIN11) |\
                                     PIN_PUPDR_FLOATING(GPIOE_PIN12) |\
                                     PIN_PUPDR_FLOATING(GPIOE_PIN13) |\
                                     PIN_PUPDR_FLOATING(GPIOE_PIN14) |\
                                     PIN_PUPDR_FLOATING(GPIOE_PIN15))
#define VAL_GPIOE_ODR               (PIN_ODR_LOW(GPIOE_PIN0)  |\
                                     PIN_ODR_LOW(GPIOE_PIN1)  |\
                                     PIN_ODR_LOW(GPIOE_PIN2)  |\
                                     PIN_ODR_LOW(GPIOE_PIN3)  |\
                                     PIN_ODR_LOW(GPIOE_PIN4)  |\
                                     PIN_ODR_LOW(GPIOE_PIN5)  |\
                                     PIN_ODR_LOW(GPIOE_PIN6)  |\
                                     PIN_ODR_LOW(GPIOE_PIN7)  |\
                                     PIN_ODR_LOW(GPIOE_PIN8)  |\
                                     PIN_ODR_LOW(GPIOE_PIN9)  |\
                                     PIN_ODR_LOW(GPIOE_PIN10) |\
                                     PIN_ODR_LOW(GPIOE_PIN11) |\
                                     PIN_ODR_LOW(GPIOE_PIN12) |\
                                     PIN_ODR_LOW(GPIOE_PIN13) |\
                                     PIN_ODR_LOW(GPIOE_PIN14) |\
                                     PIN_ODR_LOW(GPIOE_PIN15))
#define VAL_GPIOE_AFRL              (PIN_AFIO_AF(GPIOE_PIN0, 0U)  |\
                                     PIN_AFIO_AF(GPIOE_PIN1, 0U)  |\
                                     PIN_AFIO_AF(GPIOE_PIN2, 0U)  |\
                                     PIN_AFIO_AF(GPIOE_PIN3, 0U)  |\
                                     PIN_AFIO_AF(GPIOE_PIN4, 0U)  |\
                                     PIN_AFIO_AF(GPIOE_PIN5, 0U)  |\
                                     PIN_AFIO_AF(GPIOE_PIN6, 0U)  |\
                                     PIN_AFIO_AF(GPIOE_PIN7, 0U))
#define VAL_GPIOE_AFRH              (PIN_AFIO_AF(GPIOE_PIN8,  0U) |\
                                     PIN_AFIO_AF(GPIOE_PIN9,  0U) |\
                                     PIN_AFIO_AF(GPIOE_PIN10, 0U) |\
                                     PIN_AFIO_AF(GPIOE_PIN11, 0U) |\
                                     PIN_AFIO_AF(GPIOE_PIN12, 0U) |\
                                     PIN_AFIO_AF(GPIOE_PIN13, 0U) |\
                                     PIN_AFIO_AF(GPIOE_PIN14, 0U) |\
                                     PIN_AFIO_AF(GPIOE_PIN15, 0U))

/*
 * GPIOF setup:
 *
 * PF0  - PIN0
 * PF1  - PIN1
 * PF2  - PIN2
 * PF3  - PIN3
 * PF4  - PIN4
 * PF5  - PIN5
 * PF6  - PIN6
 * PF7  - PIN7
 * PF8  - PIN8
 * PF9  - PIN9
 * PF10 - PIN10
 * PF11 - USB_HS_FAULT
 * PF12 - PIN12
 * PF13 - PIN13
 * PF14 - PIN14
 * PF15 - PIN15
 */
#define VAL_GPIOF_MODER             (PIN_MODE_INPUT(GPIOF_PIN0)         |\
                                     PIN_MODE_INPUT(GPIOF_PIN1)         |\
                                     PIN_MODE_INPUT(GPIOF_PIN2)         |\
                                     PIN_MODE_INPUT(GPIOF_PIN3)         |\
                                     PIN_MODE_INPUT(GPIOF_PIN4)         |\
                                     PIN_MODE_INPUT(GPIOF_PIN5)         |\
                                     PIN_MODE_INPUT(GPIOF_PIN6)         |\
                                     PIN_MODE_INPUT(GPIOF_PIN7)         |\
                                     PIN_MODE_INPUT(GPIOF_PIN8)         |\
                                     PIN_MODE_INPUT(GPIOF_PIN9)         |\
                                     PIN_MODE_INPUT(GPIOF_PIN10)        |\
                                     PIN_MODE_INPUT(GPIOF_USB_HS_FAULT) |\
                                     PIN_MODE_INPUT(GPIOF_PIN12)        |\
                                     PIN_MODE_INPUT(GPIOF_PIN13)        |\
                                     PIN_MODE_INPUT(GPIOF_PIN14)        |\
                                     PIN_MODE_INPUT(GPIOF_PIN15))
#define VAL_GPIOF_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOF_PIN0)         |\
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN1)         |\
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN2)         |\
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN3)         |\
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN4)         |\
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN5)         |\
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN6)         |\
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN7)         |\
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN8)         |\
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN9)         |\
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN10)        |\
                                     PIN_OTYPE_PUSHPULL(GPIOF_USB_HS_FAULT) |\
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN12)        |\
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN13)        |\
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN14)        |\
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN15))
#define VAL_GPIOF_OSPEEDR           (PIN_OSPEED_VERYLOW(GPIOF_PIN0)         |\
                                     PIN_OSPEED_VERYLOW(GPIOF_PIN1)         |\
                                     PIN_OSPEED_VERYLOW(GPIOF_PIN2)         |\
                                     PIN_OSPEED_VERYLOW(GPIOF_PIN3)         |\
                                     PIN_OSPEED_VERYLOW(GPIOF_PIN4)         |\
                                     PIN_OSPEED_VERYLOW(GPIOF_PIN5)         |\
                                     PIN_OSPEED_VERYLOW(GPIOF_PIN6)         |\
                                     PIN_OSPEED_VERYLOW(GPIOF_PIN7)         |\
                                     PIN_OSPEED_VERYLOW(GPIOF_PIN8)         |\
                                     PIN_OSPEED_VERYLOW(GPIOF_PIN9)         |\
                                     PIN_OSPEED_VERYLOW(GPIOF_PIN10)        |\
                                     PIN_OSPEED_HIGH(GPIOF_USB_HS_FAULT)    |\
                                     PIN_OSPEED_VERYLOW(GPIOF_PIN12)        |\
                                     PIN_OSPEED_VERYLOW(GPIOF_PIN13)        |\
                                     PIN_OSPEED_VERYLOW(GPIOF_PIN14)        |\
                                     PIN_OSPEED_VERYLOW(GPIOF_PIN15))
#define VAL_GPIOF_PUPDR             (PIN_PUPDR_FLOATING(GPIOF_PIN0)         |\
                                     PIN_PUPDR_FLOATING(GPIOF_PIN1)         |\
                                     PIN_PUPDR_FLOATING(GPIOF_PIN2)         |\
                                     PIN_PUPDR_FLOATING(GPIOF_PIN3)         |\
                                     PIN_PUPDR_FLOATING(GPIOF_PIN4)         |\
                                     PIN_PUPDR_FLOATING(GPIOF_PIN5)         |\
                                     PIN_PUPDR_FLOATING(GPIOF_PIN6)         |\
                                     PIN_PUPDR_FLOATING(GPIOF_PIN7)         |\
                                     PIN_PUPDR_FLOATING(GPIOF_PIN8)         |\
                                     PIN_PUPDR_FLOATING(GPIOF_PIN9)         |\
                                     PIN_PUPDR_FLOATING(GPIOF_PIN10)        |\
                                     PIN_PUPDR_FLOATING(GPIOF_USB_HS_FAULT) |\
                                     PIN_PUPDR_FLOATING(GPIOF_PIN12)        |\
                                     PIN_PUPDR_FLOATING(GPIOF_PIN13)        |\
                                     PIN_PUPDR_FLOATING(GPIOF_PIN14)        |\
                                     PIN_PUPDR_FLOATING(GPIOF_PIN15))
#define VAL_GPIOF_ODR               (PIN_ODR_LOW(GPIOF_PIN0)         |\
                                     PIN_ODR_LOW(GPIOF_PIN1)         |\
                                     PIN_ODR_LOW(GPIOF_PIN2)         |\
                                     PIN_ODR_LOW(GPIOF_PIN3)         |\
                                     PIN_ODR_LOW(GPIOF_PIN4)         |\
                                     PIN_ODR_LOW(GPIOF_PIN5)         |\
                                     PIN_ODR_LOW(GPIOF_PIN6)         |\
                                     PIN_ODR_LOW(GPIOF_PIN7)         |\
                                     PIN_ODR_LOW(GPIOF_PIN8)         |\
                                     PIN_ODR_LOW(GPIOF_PIN9)         |\
                                     PIN_ODR_LOW(GPIOF_PIN10)        |\
                                     PIN_ODR_LOW(GPIOF_USB_HS_FAULT) |\
                                     PIN_ODR_LOW(GPIOF_PIN12)        |\
                                     PIN_ODR_LOW(GPIOF_PIN13)        |\
                                     PIN_ODR_LOW(GPIOF_PIN14)        |\
                                     PIN_ODR_LOW(GPIOF_PIN15))
#define VAL_GPIOF_AFRL              (PIN_AFIO_AF(GPIOF_PIN0, 0U) |\
                                     PIN_AFIO_AF(GPIOF_PIN1, 0U) |\
                                     PIN_AFIO_AF(GPIOF_PIN2, 0U) |\
                                     PIN_AFIO_AF(GPIOF_PIN3, 0U) |\
                                     PIN_AFIO_AF(GPIOF_PIN4, 0U) |\
                                     PIN_AFIO_AF(GPIOF_PIN5, 0U) |\
                                     PIN_AFIO_AF(GPIOF_PIN6, 0U) |\
                                     PIN_AFIO_AF(GPIOF_PIN7, 0U))
#define VAL_GPIOF_AFRH              (PIN_AFIO_AF(GPIOF_PIN8, 0U)         |\
                                     PIN_AFIO_AF(GPIOF_PIN9, 0U)         |\
                                     PIN_AFIO_AF(GPIOF_PIN10, 0U)        |\
                                     PIN_AFIO_AF(GPIOF_USB_HS_FAULT, 0U) |\
                                     PIN_AFIO_AF(GPIOF_PIN12, 0U)        |\
                                     PIN_AFIO_AF(GPIOF_PIN13, 0U)        |\
                                     PIN_AFIO_AF(GPIOF_PIN14, 0U)        |\
                                     PIN_AFIO_AF(GPIOF_PIN15, 0U))

/*
 * GPIOG setup:
 *
 * PG0  - PIN0
 * PG1  - PIN1
 * PG2  - PIN2
 * PG3  - PIN3
 * PG4  - PIN4
 * PG5  - PIN5
 * PG6  - PIN6
 * PG7  - PIN7
 * PG8  - PIN8
 * PG9  - PIN9
 * PG10 - SPI2_CS
 * PG11 - ETH_RMII_TXEN
 * PG12 - PIN12
 * PG13 - ETH_RMII_TXD0
 * PG14 - ETH_RMII_TXD1
 * PG15 - PIN15
 */
#define VAL_GPIOG_MODER             (PIN_MODE_INPUT(GPIOG_PIN0)          |\
                                     PIN_MODE_INPUT(GPIOG_PIN1)          |\
                                     PIN_MODE_INPUT(GPIOG_PIN2)          |\
                                     PIN_MODE_INPUT(GPIOG_PIN3)          |\
                                     PIN_MODE_INPUT(GPIOG_PIN4)          |\
                                     PIN_MODE_INPUT(GPIOG_PIN5)          |\
                                     PIN_MODE_INPUT(GPIOG_PIN6)          |\
                                     PIN_MODE_INPUT(GPIOG_PIN7)          |\
                                     PIN_MODE_INPUT(GPIOG_PIN8)          |\
                                     PIN_MODE_INPUT(GPIOG_PIN9)          |\
                                     PIN_MODE_INPUT(GPIOG_SPI2_CS)       |\
                                     PIN_MODE_ALTERNATE(GPIOG_ETH_RMII_TXEN) |\
                                     PIN_MODE_INPUT(GPIOG_PIN12)         |\
                                     PIN_MODE_ALTERNATE(GPIOG_ETH_RMII_TXD0) |\
                                     PIN_MODE_ALTERNATE(GPIOG_ETH_RMII_TXD1) |\
                                     PIN_MODE_INPUT(GPIOG_PIN15))
#define VAL_GPIOG_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOG_PIN0)          |\
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN1)          |\
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN2)          |\
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN3)          |\
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN4)          |\
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN5)          |\
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN6)          |\
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN7)          |\
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN8)          |\
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN9)          |\
                                     PIN_OTYPE_PUSHPULL(GPIOG_SPI2_CS)       |\
                                     PIN_OTYPE_PUSHPULL(GPIOG_ETH_RMII_TXEN) |\
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN12)         |\
                                     PIN_OTYPE_PUSHPULL(GPIOG_ETH_RMII_TXD0) |\
                                     PIN_OTYPE_PUSHPULL(GPIOG_ETH_RMII_TXD1) |\
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN15))
#define VAL_GPIOG_OSPEEDR           (PIN_OSPEED_VERYLOW(GPIOG_PIN0)          |\
                                     PIN_OSPEED_VERYLOW(GPIOG_PIN1)          |\
                                     PIN_OSPEED_VERYLOW(GPIOG_PIN2)          |\
                                     PIN_OSPEED_VERYLOW(GPIOG_PIN3)          |\
                                     PIN_OSPEED_VERYLOW(GPIOG_PIN4)          |\
                                     PIN_OSPEED_VERYLOW(GPIOG_PIN5)          |\
                                     PIN_OSPEED_HIGH(GPIOG_PIN6)             |\
                                     PIN_OSPEED_VERYLOW(GPIOG_PIN7)          |\
                                     PIN_OSPEED_VERYLOW(GPIOG_PIN8)          |\
                                     PIN_OSPEED_VERYLOW(GPIOG_PIN9)          |\
                                     PIN_OSPEED_VERYLOW(GPIOG_SPI2_CS)       |\
                                     PIN_OSPEED_HIGH(GPIOG_ETH_RMII_TXEN)    |\
                                     PIN_OSPEED_VERYLOW(GPIOG_PIN12)         |\
                                     PIN_OSPEED_HIGH(GPIOG_ETH_RMII_TXD0)    |\
                                     PIN_OSPEED_HIGH(GPIOG_ETH_RMII_TXD1)    |\
                                     PIN_OSPEED_VERYLOW(GPIOG_PIN15))
#define VAL_GPIOG_PUPDR             (PIN_PUPDR_FLOATING(GPIOG_PIN0)          |\
                                     PIN_PUPDR_FLOATING(GPIOG_PIN1)          |\
                                     PIN_PUPDR_FLOATING(GPIOG_PIN2)          |\
                                     PIN_PUPDR_FLOATING(GPIOG_PIN3)          |\
                                     PIN_PUPDR_FLOATING(GPIOG_PIN4)          |\
                                     PIN_PUPDR_FLOATING(GPIOG_PIN5)          |\
                                     PIN_PUPDR_FLOATING(GPIOG_PIN6)          |\
                                     PIN_PUPDR_FLOATING(GPIOG_PIN7)          |\
                                     PIN_PUPDR_FLOATING(GPIOG_PIN8)          |\
                                     PIN_PUPDR_FLOATING(GPIOG_PIN9)          |\
                                     PIN_PUPDR_FLOATING(GPIOG_SPI2_CS)       |\
                                     PIN_PUPDR_FLOATING(GPIOG_ETH_RMII_TXEN) |\
                                     PIN_PUPDR_FLOATING(GPIOG_PIN12)         |\
                                     PIN_PUPDR_FLOATING(GPIOG_ETH_RMII_TXD0) |\
                                     PIN_PUPDR_FLOATING(GPIOG_ETH_RMII_TXD1) |\
                                     PIN_PUPDR_FLOATING(GPIOG_PIN15))
#define VAL_GPIOG_ODR               (PIN_ODR_LOW(GPIOG_PIN0)          |\
                                     PIN_ODR_LOW(GPIOG_PIN1)          |\
                                     PIN_ODR_LOW(GPIOG_PIN2)          |\
                                     PIN_ODR_LOW(GPIOG_PIN3)          |\
                                     PIN_ODR_LOW(GPIOG_PIN4)          |\
                                     PIN_ODR_LOW(GPIOG_PIN5)          |\
                                     PIN_ODR_LOW(GPIOG_PIN6)          |\
                                     PIN_ODR_LOW(GPIOG_PIN7)          |\
                                     PIN_ODR_LOW(GPIOG_PIN8)          |\
                                     PIN_ODR_LOW(GPIOG_PIN9)          |\
                                     PIN_ODR_LOW(GPIOG_SPI2_CS)       |\
                                     PIN_ODR_HIGH(GPIOG_ETH_RMII_TXEN) |\
                                     PIN_ODR_LOW(GPIOG_PIN12)         |\
                                     PIN_ODR_HIGH(GPIOG_ETH_RMII_TXD0) |\
                                     PIN_ODR_HIGH(GPIOG_ETH_RMII_TXD1) |\
                                     PIN_ODR_LOW(GPIOG_PIN15))
#define VAL_GPIOG_AFRL              (PIN_AFIO_AF(GPIOG_PIN0, 0U) |\
                                     PIN_AFIO_AF(GPIOG_PIN1, 0U) |\
                                     PIN_AFIO_AF(GPIOG_PIN2, 0U) |\
                                     PIN_AFIO_AF(GPIOG_PIN3, 0U) |\
                                     PIN_AFIO_AF(GPIOG_PIN4, 0U) |\
                                     PIN_AFIO_AF(GPIOG_PIN5, 0U) |\
                                     PIN_AFIO_AF(GPIOG_PIN6, 0U) |\
                                     PIN_AFIO_AF(GPIOG_PIN7, 0U))
#define VAL_GPIOG_AFRH              (PIN_AFIO_AF(GPIOG_PIN8,          0U) |\
                                     PIN_AFIO_AF(GPIOG_PIN9,          0U) |\
                                     PIN_AFIO_AF(GPIOG_SPI2_CS,       0U) |\
                                     PIN_AFIO_AF(GPIOG_ETH_RMII_TXEN, 11U) |\
                                     PIN_AFIO_AF(GPIOG_PIN12,         0U) |\
                                     PIN_AFIO_AF(GPIOG_ETH_RMII_TXD0, 11U) |\
                                     PIN_AFIO_AF(GPIOG_ETH_RMII_TXD1, 11U) |\
                                     PIN_AFIO_AF(GPIOG_PIN15,         0U))

/*
 * GPIOH setup:
 *
 * PH0  - OSC_IN
 * PH1  - OSC_OUT
 * PH2  - PIN2
 * PH3  - PIN3
 * PH4  - PIN4
 * PH5  - PIN5
 * PH6  - PIN6
 * PH7  - PIN7
 * PH8  - PIN8
 * PH9  - PIN9
 * PH10 - PIN10
 * PH11 - PIN11
 * PH12 - PIN12
 * PH13 - PIN13
 * PH14 - PIN14
 * PH15 - PIN15
 */
#define VAL_GPIOH_MODER             (PIN_MODE_INPUT(GPIOH_OSC_IN)  |\
                                     PIN_MODE_INPUT(GPIOH_OSC_OUT) |\
                                     PIN_MODE_INPUT(GPIOH_PIN2)    |\
                                     PIN_MODE_INPUT(GPIOH_PIN3)    |\
                                     PIN_MODE_INPUT(GPIOH_PIN4)    |\
                                     PIN_MODE_INPUT(GPIOH_PIN5)    |\
                                     PIN_MODE_INPUT(GPIOH_PIN6)    |\
                                     PIN_MODE_INPUT(GPIOH_PIN7)    |\
                                     PIN_MODE_INPUT(GPIOH_PIN8)    |\
                                     PIN_MODE_INPUT(GPIOH_PIN9)    |\
                                     PIN_MODE_INPUT(GPIOH_PIN10)   |\
                                     PIN_MODE_INPUT(GPIOH_PIN11)   |\
                                     PIN_MODE_INPUT(GPIOH_PIN12)   |\
                                     PIN_MODE_INPUT(GPIOH_PIN13)   |\
                                     PIN_MODE_INPUT(GPIOH_PIN14)   |\
                                     PIN_MODE_INPUT(GPIOH_PIN15))
#define VAL_GPIOH_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOH_OSC_IN)  |\
                                     PIN_OTYPE_PUSHPULL(GPIOH_OSC_OUT) |\
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN2)    |\
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN3)    |\
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN4)    |\
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN5)    |\
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN6)    |\
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN7)    |\
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN8)    |\
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN9)    |\
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN10)   |\
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN11)   |\
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN12)   |\
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN13)   |\
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN14)   |\
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN15))
#define VAL_GPIOH_OSPEEDR           (PIN_OSPEED_VERYLOW(GPIOH_OSC_IN)  |\
                                     PIN_OSPEED_VERYLOW(GPIOH_OSC_OUT) |\
                                     PIN_OSPEED_VERYLOW(GPIOH_PIN2)    |\
                                     PIN_OSPEED_VERYLOW(GPIOH_PIN3)    |\
                                     PIN_OSPEED_VERYLOW(GPIOH_PIN4)    |\
                                     PIN_OSPEED_VERYLOW(GPIOH_PIN5)    |\
                                     PIN_OSPEED_VERYLOW(GPIOH_PIN6)    |\
                                     PIN_OSPEED_VERYLOW(GPIOH_PIN7)    |\
                                     PIN_OSPEED_VERYLOW(GPIOH_PIN8)    |\
                                     PIN_OSPEED_VERYLOW(GPIOH_PIN9)    |\
                                     PIN_OSPEED_VERYLOW(GPIOH_PIN10)   |\
                                     PIN_OSPEED_VERYLOW(GPIOH_PIN11)   |\
                                     PIN_OSPEED_VERYLOW(GPIOH_PIN12)   |\
                                     PIN_OSPEED_VERYLOW(GPIOH_PIN13)   |\
                                     PIN_OSPEED_VERYLOW(GPIOH_PIN14)   |\
                                     PIN_OSPEED_VERYLOW(GPIOH_PIN15))
#define VAL_GPIOH_PUPDR             (PIN_PUPDR_FLOATING(GPIOH_OSC_IN)  |\
                                     PIN_PUPDR_FLOATING(GPIOH_OSC_OUT) |\
                                     PIN_PUPDR_FLOATING(GPIOH_PIN2)    |\
                                     PIN_PUPDR_FLOATING(GPIOH_PIN3)    |\
                                     PIN_PUPDR_FLOATING(GPIOH_PIN4)    |\
                                     PIN_PUPDR_FLOATING(GPIOH_PIN5)    |\
                                     PIN_PUPDR_FLOATING(GPIOH_PIN6)    |\
                                     PIN_PUPDR_FLOATING(GPIOH_PIN7)    |\
                                     PIN_PUPDR_FLOATING(GPIOH_PIN8)    |\
                                     PIN_PUPDR_FLOATING(GPIOH_PIN9)    |\
                                     PIN_PUPDR_FLOATING(GPIOH_PIN10)   |\
                                     PIN_PUPDR_FLOATING(GPIOH_PIN11)   |\
                                     PIN_PUPDR_FLOATING(GPIOH_PIN12)   |\
                                     PIN_PUPDR_FLOATING(GPIOH_PIN13)   |\
                                     PIN_PUPDR_FLOATING(GPIOH_PIN14)   |\
                                     PIN_PUPDR_FLOATING(GPIOH_PIN15))
#define VAL_GPIOH_ODR               (PIN_ODR_LOW(GPIOH_OSC_IN)  |\
                                     PIN_ODR_LOW(GPIOH_OSC_OUT) |\
                                     PIN_ODR_LOW(GPIOH_PIN2)    |\
                                     PIN_ODR_LOW(GPIOH_PIN3)    |\
                                     PIN_ODR_LOW(GPIOH_PIN4)    |\
                                     PIN_ODR_LOW(GPIOH_PIN5)    |\
                                     PIN_ODR_LOW(GPIOH_PIN6)    |\
                                     PIN_ODR_LOW(GPIOH_PIN7)    |\
                                     PIN_ODR_LOW(GPIOH_PIN8)    |\
                                     PIN_ODR_LOW(GPIOH_PIN9)    |\
                                     PIN_ODR_LOW(GPIOH_PIN10)   |\
                                     PIN_ODR_LOW(GPIOH_PIN11)   |\
                                     PIN_ODR_LOW(GPIOH_PIN12)   |\
                                     PIN_ODR_LOW(GPIOH_PIN13)   |\
                                     PIN_ODR_LOW(GPIOH_PIN14)   |\
                                     PIN_ODR_LOW(GPIOH_PIN15))
#define VAL_GPIOH_AFRL              (PIN_AFIO_AF(GPIOH_OSC_IN,  0U) |\
                                     PIN_AFIO_AF(GPIOH_OSC_OUT, 0U) |\
                                     PIN_AFIO_AF(GPIOH_PIN2,    0U) |\
                                     PIN_AFIO_AF(GPIOH_PIN3,    0U) |\
                                     PIN_AFIO_AF(GPIOH_PIN4,    0U) |\
                                     PIN_AFIO_AF(GPIOH_PIN5,    0U) |\
                                     PIN_AFIO_AF(GPIOH_PIN6,    0U) |\
                                     PIN_AFIO_AF(GPIOH_PIN7,    0U))
#define VAL_GPIOH_AFRH              (PIN_AFIO_AF(GPIOH_PIN8,  0U) |\
                                     PIN_AFIO_AF(GPIOH_PIN9,  0U) |\
                                     PIN_AFIO_AF(GPIOH_PIN10, 0U) |\
                                     PIN_AFIO_AF(GPIOH_PIN11, 0U) |\
                                     PIN_AFIO_AF(GPIOH_PIN12, 0U) |\
                                     PIN_AFIO_AF(GPIOH_PIN13, 0U) |\
                                     PIN_AFIO_AF(GPIOH_PIN14, 0U) |\
                                     PIN_AFIO_AF(GPIOH_PIN15, 0U))

/*
 * GPIOI setup:
 *
 * PI0  - PIN0
 * PI1  - PIN1
 * PI2  - PIN2
 * PI3  - PIN3
 * PI4  - PIN4
 * PI5  - PIN5
 * PI6  - PIN6
 * PI7  - PIN7
 * PI8  - PIN8
 * PI9  - PIN9
 * PI10 - PIN10
 * PI11 - PIN11
 * PI12 - PIN12
 * PI13 - PIN13
 * PI14 - PIN14
 * PI15 - PIN15
 */
#define VAL_GPIOI_MODER             (PIN_MODE_INPUT(GPIOI_PIN0)  |\
                                     PIN_MODE_INPUT(GPIOI_PIN1)  |\
                                     PIN_MODE_INPUT(GPIOI_PIN2)  |\
                                     PIN_MODE_INPUT(GPIOI_PIN3)  |\
                                     PIN_MODE_INPUT(GPIOI_PIN4)  |\
                                     PIN_MODE_INPUT(GPIOI_PIN5)  |\
                                     PIN_MODE_INPUT(GPIOI_PIN6)  |\
                                     PIN_MODE_INPUT(GPIOI_PIN7)  |\
                                     PIN_MODE_INPUT(GPIOI_PIN8)  |\
                                     PIN_MODE_INPUT(GPIOI_PIN9)  |\
                                     PIN_MODE_INPUT(GPIOI_PIN10) |\
                                     PIN_MODE_INPUT(GPIOI_PIN11) |\
                                     PIN_MODE_INPUT(GPIOI_PIN12) |\
                                     PIN_MODE_INPUT(GPIOI_PIN13) |\
                                     PIN_MODE_INPUT(GPIOI_PIN14) |\
                                     PIN_MODE_INPUT(GPIOI_PIN15))
#define VAL_GPIOI_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOI_PIN0)  |\
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN1)  |\
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN2)  |\
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN3)  |\
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN4)  |\
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN5)  |\
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN6)  |\
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN7)  |\
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN8)  |\
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN9)  |\
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN10) |\
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN11) |\
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN12) |\
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN13) |\
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN14) |\
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN15))
#define VAL_GPIOI_OSPEEDR           (PIN_OSPEED_VERYLOW(GPIOI_PIN0)  |\
                                     PIN_OSPEED_VERYLOW(GPIOI_PIN1)  |\
                                     PIN_OSPEED_VERYLOW(GPIOI_PIN2)  |\
                                     PIN_OSPEED_VERYLOW(GPIOI_PIN3)  |\
                                     PIN_OSPEED_VERYLOW(GPIOI_PIN4)  |\
                                     PIN_OSPEED_VERYLOW(GPIOI_PIN5)  |\
                                     PIN_OSPEED_VERYLOW(GPIOI_PIN6)  |\
                                     PIN_OSPEED_VERYLOW(GPIOI_PIN7)  |\
                                     PIN_OSPEED_VERYLOW(GPIOI_PIN8)  |\
                                     PIN_OSPEED_VERYLOW(GPIOI_PIN9)  |\
                                     PIN_OSPEED_VERYLOW(GPIOI_PIN10) |\
                                     PIN_OSPEED_VERYLOW(GPIOI_PIN11) |\
                                     PIN_OSPEED_VERYLOW(GPIOI_PIN12) |\
                                     PIN_OSPEED_VERYLOW(GPIOI_PIN13) |\
                                     PIN_OSPEED_VERYLOW(GPIOI_PIN14) |\
                                     PIN_OSPEED_VERYLOW(GPIOI_PIN15))
#define VAL_GPIOI_PUPDR             (PIN_PUPDR_FLOATING(GPIOI_PIN0)  |\
                                     PIN_PUPDR_FLOATING(GPIOI_PIN1)  |\
                                     PIN_PUPDR_FLOATING(GPIOI_PIN2)  |\
                                     PIN_PUPDR_FLOATING(GPIOI_PIN3)  |\
                                     PIN_PUPDR_FLOATING(GPIOI_PIN4)  |\
                                     PIN_PUPDR_FLOATING(GPIOI_PIN5)  |\
                                     PIN_PUPDR_FLOATING(GPIOI_PIN6)  |\
                                     PIN_PUPDR_FLOATING(GPIOI_PIN7)  |\
                                     PIN_PUPDR_FLOATING(GPIOI_PIN8)  |\
                                     PIN_PUPDR_FLOATING(GPIOI_PIN9)  |\
                                     PIN_PUPDR_FLOATING(GPIOI_PIN10) |\
                                     PIN_PUPDR_FLOATING(GPIOI_PIN11) |\
                                     PIN_PUPDR_FLOATING(GPIOI_PIN12) |\
                                     PIN_PUPDR_FLOATING(GPIOI_PIN13) |\
                                     PIN_PUPDR_FLOATING(GPIOI_PIN14) |\
                                     PIN_PUPDR_FLOATING(GPIOI_PIN15))
#define VAL_GPIOI_ODR               (PIN_ODR_LOW(GPIOI_PIN0)  |\
                                     PIN_ODR_LOW(GPIOI_PIN1)  |\
                                     PIN_ODR_LOW(GPIOI_PIN2)  |\
                                     PIN_ODR_LOW(GPIOI_PIN3)  |\
                                     PIN_ODR_LOW(GPIOI_PIN4)  |\
                                     PIN_ODR_LOW(GPIOI_PIN5)  |\
                                     PIN_ODR_LOW(GPIOI_PIN6)  |\
                                     PIN_ODR_LOW(GPIOI_PIN7)  |\
                                     PIN_ODR_LOW(GPIOI_PIN8)  |\
                                     PIN_ODR_LOW(GPIOI_PIN9)  |\
                                     PIN_ODR_LOW(GPIOI_PIN10) |\
                                     PIN_ODR_LOW(GPIOI_PIN11) |\
                                     PIN_ODR_LOW(GPIOI_PIN12) |\
                                     PIN_ODR_LOW(GPIOI_PIN13) |\
                                     PIN_ODR_LOW(GPIOI_PIN14) |\
                                     PIN_ODR_LOW(GPIOI_PIN15))
#define VAL_GPIOI_AFRL              (PIN_AFIO_AF(GPIOI_PIN0, 0U) |\
                                     PIN_AFIO_AF(GPIOI_PIN1, 0U) |\
                                     PIN_AFIO_AF(GPIOI_PIN2, 0U) |\
                                     PIN_AFIO_AF(GPIOI_PIN3, 0U) |\
                                     PIN_AFIO_AF(GPIOI_PIN4, 0U) |\
                                     PIN_AFIO_AF(GPIOI_PIN5, 0U) |\
                                     PIN_AFIO_AF(GPIOI_PIN6, 0U) |\
                                     PIN_AFIO_AF(GPIOI_PIN7, 0U))
#define VAL_GPIOI_AFRH              (PIN_AFIO_AF(GPIOI_PIN8, 0U)  |\
                                     PIN_AFIO_AF(GPIOI_PIN9, 0U)  |\
                                     PIN_AFIO_AF(GPIOI_PIN10, 0U) |\
                                     PIN_AFIO_AF(GPIOI_PIN11, 0U) |\
                                     PIN_AFIO_AF(GPIOI_PIN12, 0U) |\
                                     PIN_AFIO_AF(GPIOI_PIN13, 0U) |\
                                     PIN_AFIO_AF(GPIOI_PIN14, 0U) |\
                                     PIN_AFIO_AF(GPIOI_PIN15, 0U))
#define VAL_GPIOI_AFRH              (PIN_AFIO_AF(GPIOI_PIN8,  0U) |\
                                     PIN_AFIO_AF(GPIOI_PIN9,  0U) |\
                                     PIN_AFIO_AF(GPIOI_PIN10, 0U) |\
                                     PIN_AFIO_AF(GPIOI_PIN11, 0U) |\
                                     PIN_AFIO_AF(GPIOI_PIN12, 0U) |\
                                     PIN_AFIO_AF(GPIOI_PIN13, 0U) |\
                                     PIN_AFIO_AF(GPIOI_PIN14, 0U) |\
                                     PIN_AFIO_AF(GPIOI_PIN15, 0U))

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
