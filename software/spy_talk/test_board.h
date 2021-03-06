#ifndef TEST_BOARD_H
#define TEST_BOARD_H

/*
*   PINS and clocks defined according to dev kit nRF and SX12 montage.
*/

// <o> CLOCK_CONFIG_LF_SRC  - LF Clock Source

// <0=> RC
// <1=> XTAL
// <2=> Synth

#ifndef CLOCK_CONFIG_LF_SRC
#define CLOCK_CONFIG_LF_SRC 1
#endif

// <o> NRF_SDH_CLOCK_LF_SRC  - SoftDevice clock source.

// <0=> NRF_CLOCK_LF_SRC_RC
// <1=> NRF_CLOCK_LF_SRC_XTAL
// <2=> NRF_CLOCK_LF_SRC_SYNTH

#ifndef NRF_SDH_CLOCK_LF_SRC
#define NRF_SDH_CLOCK_LF_SRC 1
#endif

/*
*   Board specific control of the SX1276 including PA, ANTSW and DIO
*/

#define SX1276_DIO0_PIN 6
#define SX1276_DIO1_PIN 7
#define SX1276_DIO2_PIN 8
#define SX1276_DIO3_PIN 5
#define SX1276_DIO4_PIN 1
#define SX1276_DIO5_PIN 0

/*
*   SX1276 pins
*/

// <0=> 0 (P0.0)
// <1=> 1 (P0.1)
// <2=> 2 (P0.2)
// <3=> 3 (P0.3)
// <4=> 4 (P0.4)
// <5=> 5 (P0.5)
// <6=> 6 (P0.6)
// <7=> 7 (P0.7)
// <8=> 8 (P0.8)
// <9=> 9 (P0.9)
// <10=> 10 (P0.10)
// <11=> 11 (P0.11)
// <12=> 12 (P0.12)
// <13=> 13 (P0.13)
// <14=> 14 (P0.14)
// <15=> 15 (P0.15)
// <16=> 16 (P0.16)
// <17=> 17 (P0.17)
// <18=> 18 (P0.18)
// <19=> 19 (P0.19)
// <20=> 20 (P0.20)
// <21=> 21 (P0.21)
// <22=> 22 (P0.22)
// <23=> 23 (P0.23)
// <24=> 24 (P0.24)
// <25=> 25 (P0.25)
// <26=> 26 (P0.26)
// <27=> 27 (P0.27)
// <28=> 28 (P0.28)
// <29=> 29 (P0.29)
// <30=> 30 (P0.30)
// <31=> 31 (P0.31)
// <4294967295=> Not connected

#ifndef SPI_ENABLED
#define SPI_ENABLED 1
#endif

// <o> SPI_DEFAULT_FREQUENCY  - SPI frequency

// <33554432=> 125 kHz
// <67108864=> 250 kHz
// <134217728=> 500 kHz
// <268435456=> 1 MHz
// <536870912=> 2 MHz
// <1073741824=> 4 MHz
// <2147483648=> 8 MHz

#ifndef SPI_DEFAULT_FREQUENCY
#define SPI_DEFAULT_FREQUENCY 1073741824
#endif

#ifndef SPI_SCK_PIN
#define SPI_SCK_PIN 3
#endif

// <o> SPI_MISO_PIN  - Pin number

#ifndef SPI_MISO_PIN
#define SPI_MISO_PIN 28
#endif

// <o> SPI_MOSI_PIN  - Pin number

#ifndef SPI_MOSI_PIN
#define SPI_MOSI_PIN 4
#endif

// <o> SPI_SS_PIN  - Pin number

#ifndef SPI_SS_PIN
#define SPI_SS_PIN 29
#endif

// LEDs

void leds_init();
void led_on(int i);
void led_off(int i);
void led_toggle(int i);

#endif
