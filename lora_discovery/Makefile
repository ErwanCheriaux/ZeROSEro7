TRGT = arm-none-eabi-
CC = $(TRGT)gcc

CUBEEXPANSION = ../icube_lrwan/CubeExpansion
DRIVERS = $(CUBEEXPANSION)/Drivers
BSP = $(DRIVERS)/BSP
CMSIS = $(DRIVERS)/CMSIS
HAL_DRIVER = $(DRIVERS)/STM32L0xx_HAL_Driver
MIDDLEWARES = $(CUBEEXPANSION)/Middlewares/Third_Party/Lora
INCDIR = -I. -I$(HAL_DRIVER)/Inc -I$(BSP)/B-L072Z-LRWAN1 -I$(BSP)/Components/sx1276 \
	-I$(BSP)/MLM32L07X01 -I$(CMSIS)/Include -I$(CMSIS)/Device/ST/STM32L0xx/Include \
	-I$(MIDDLEWARES)/Utilities -I$(MIDDLEWARES)/Phy -I$(MIDDLEWARES)/Mac \
	-I$(MIDDLEWARES)/Core -I$(MIDDLEWARES)/Conf/Inc -I$(MIDDLEWARES)/Crypto
INCFILE = -include $(CMSIS)/Device/ST/STM32L0xx/Include/stm32l072xx.h \
	-include $(HAL_DRIVER)/Inc/stm32l0xx_hal_gpio.h \
	-include $(HAL_DRIVER)/Inc/stm32l0xx_hal.h # TODO Add all files in this folder

DEFINES = -D _DEBUG -D USE_MODEM_LORA -D USE_BAND_868 -D STM32L072xx

LDSCRIPT = STM32L072CZYx_FLASH.ld
CWARN = -Wall -Wextra -Werror
CFLAGS = $(CWARN) $(DEFINES) -Og -ggdb -std=gnu11
LDFLAGS = $(INCFILE) $(INCDIR) -T $(LDSCRIPT)

ELF = main

USERSRC = main.c
# CSRC = $(USERSRC) # TODO Add .c Files listed in .project from Project/.../PingPong/SW4STM32

$(ELF) : $(CSRC)
