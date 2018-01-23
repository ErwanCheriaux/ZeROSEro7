PROJECT_NAME 		 := spy_talk
PROJ_DIR  			 := .
nRF52_ble_common_DIR := ../nRF52_ble_common
SDK_COMMON_DIR		 := ../../SDK

USER_SRC += \
	$(PROJ_DIR)/main.c \
	$(PROJ_DIR)/hw_rtc.c \
	$(PROJ_DIR)/hw_spi.c \
	$(PROJ_DIR)/hw_gpio.c \
	$(PROJ_DIR)/lora.c \
	$(PROJ_DIR)/lora_board.c \
	$(PROJ_DIR)/low_power.c \
	$(PROJ_DIR)/test_board.c \
	$(PROJ_DIR)/custom_board.c

CFLAGS += -D USE_SPY_TALK_BOARD

# ***** LoRa Semtec Specific *****

CUBEEXPANSION = $(SDK_COMMON_DIR)/LoRa_icube/CubeExpansion
DRIVERS = $(CUBEEXPANSION)/Drivers
BSP = $(DRIVERS)/BSP
CMSIS = $(DRIVERS)/CMSIS
MIDDLEWARES = $(CUBEEXPANSION)/Middlewares/Third_Party/Lora

SRC_FILES += \
	$(BSP)/Components/sx1276/sx1276.c \
	$(MIDDLEWARES)/Utilities/utilities.c \
	$(MIDDLEWARES)/Utilities/delay.c \
	$(MIDDLEWARES)/Utilities/timeServer.c \
#	$(MIDDLEWARES)/Core/lora.c \	for LoRaMAC

INC_FOLDERS += \
	$(MIDDLEWARES)/Phy \
	$(BSP)/Components/sx1276 \
	$(MIDDLEWARES)/Utilities \
#	$(MIDDLEWARES)/Core \
#	$(CMSIS)/Include \
#	$(MIDDLEWARES)/Crypto \
#	$(MIDDLEWARES)/Mac

CFLAGS += \
	-D USE_BAND_868 \
	-D USE_MODEM_LORA \
	-D _DEBUG \
	-D DEBUG \
	-D DIO_PINS_CONNECTED

include $(nRF52_ble_common_DIR)/nRF52_ble_common.mk

test_board: $(ELF)
# Otherwise, custom_board.h is selected instead of test_board.h
test_board: CFLAGS += -D TEST_BOARD -D PINK_BOARD

# The boards are distinguished by their manufacturer data in BLE advertisements.
yellow_board: $(ELF)
yellow_board: CFLAGS += -D YELLOW_BOARD

blue_board: $(ELF)
blue_board: CFLAGS += -D BLUE_BOARD

# Pink is default
pink_board: $(ELF)
pink_board: CFLAGS += -D PINK_BOARD
