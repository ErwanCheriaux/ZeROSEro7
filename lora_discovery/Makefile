# To understand where does all this come from, see CubeExpansion/Projects/.../PingPong/SW4STM32/B-L072Z

TRGT = arm-none-eabi-
CC = $(TRGT)gcc
ARCH = -mcpu=cortex-m0plus -mthumb -DTHUMB

CUBEEXPANSION = ../icube_lrwan/CubeExpansion
DRIVERS = $(CUBEEXPANSION)/Drivers
BSP = $(DRIVERS)/BSP
CMSIS = $(DRIVERS)/CMSIS
HAL_DRIVER = $(DRIVERS)/STM32L0xx_HAL_Driver
MIDDLEWARES = $(CUBEEXPANSION)/Middlewares/Third_Party/Lora
INCDIR = -I. \
	-I$(BSP)/B-L072Z-LRWAN1 \
	-I$(BSP)/MLM32L07X01 \
	-I$(HAL_DRIVER)/Inc \
	-I$(CMSIS)/Device/ST/STM32L0xx/Include \
	-I$(CMSIS)/Include \
	-I$(MIDDLEWARES)/Crypto \
	-I$(MIDDLEWARES)/Mac \
	-I$(MIDDLEWARES)/Phy \
	-I$(MIDDLEWARES)/Utilities \
	-I$(BSP)/X_NUCLEO_IKS01A1 \
	-I$(BSP)/Components/Common \
	-I$(BSP)/Components/hts221 \
	-I$(BSP)/Components/lps25hb \
	-I$(BSP)/Components/sx1276 \
	-I$(MIDDLEWARES)/Core \

DEFINES = -D STM32L072xx \
	-D USE_B_L072Z_LRWAN1 \
	-D USE_HAL_DRIVER \
	-D USE_BAND_868 \
	-D USE_MODEM_LORA \
	-D _DEBUG

LDSCRIPT = STM32L072CZYx_FLASH.ld
CWARN = -Wall -Wextra -Werror -Wno-unused-parameter -Wno-sign-compare
CFLAGS = $(ARCH) $(CWARN) $(DEFINES) -Og -ggdb -std=gnu11
LDFLAGS = $(INCFILE) $(INCDIR) -T $(LDSCRIPT)

ELF = main

USER_SRC = startup_stm32l072xx.s main.c debug.c hw_gpio.c hw_rtc.c hw_spi.c \
	stm32l0xx_hal_msp.c stm32l0xx_hw.c stm32l0xx_it.c vcom.c
 # REVIEW How to only compile .o that are used ??? like ChibiOS
BL072Z_SRC = $(BSP)/B-L072Z-LRWAN1/b-l072z-lrwan1.c \
	$(CMSIS)/Device/ST/STM32L0xx/Source/Templates/system_stm32l0xx.c \
	$(HAL_DRIVER)/Src/stm32l0xx_hal.c \
	$(HAL_DRIVER)/Src/stm32l0xx_hal_adc.c \
	$(HAL_DRIVER)/Src/stm32l0xx_hal_adc_ex.c \
	$(HAL_DRIVER)/Src/stm32l0xx_hal_cortex.c \
	$(HAL_DRIVER)/Src/stm32l0xx_hal_dma.c \
	$(HAL_DRIVER)/Src/stm32l0xx_hal_gpio.c \
	$(HAL_DRIVER)/Src/stm32l0xx_hal_i2c.c \
	$(HAL_DRIVER)/Src/stm32l0xx_hal_pwr.c \
	$(HAL_DRIVER)/Src/stm32l0xx_hal_pwr_ex.c \
	$(HAL_DRIVER)/Src/stm32l0xx_hal_rcc.c \
	$(HAL_DRIVER)/Src/stm32l0xx_hal_rcc_ex.c \
	$(HAL_DRIVER)/Src/stm32l0xx_hal_rtc.c \
	$(HAL_DRIVER)/Src/stm32l0xx_hal_rtc_ex.c \
	$(HAL_DRIVER)/Src/stm32l0xx_hal_spi.c \
	$(HAL_DRIVER)/Src/stm32l0xx_hal_tim.c \
	$(HAL_DRIVER)/Src/stm32l0xx_hal_tim_ex.c \
	$(HAL_DRIVER)/Src/stm32l0xx_hal_uart.c \
	$(HAL_DRIVER)/Src/stm32l0xx_hal_uart_ex.c \
	$(BSP)/B-L072Z-LRWAN1/b-l072z-lrwan1.c \
	$(BSP)/Components/sx1276/sx1276.c \
	$(BSP)/MLM32L07X01/mlm32l07x01.c \
	$(MIDDLEWARES)/Utilities/delay.c \
	$(MIDDLEWARES)/Utilities/low_power.c \
	$(MIDDLEWARES)/Utilities/timeServer.c \
	$(MIDDLEWARES)/Utilities/utilities.c
CSRC = $(USER_SRC) $(BL072Z_SRC)

$(ELF) : $(CSRC)
