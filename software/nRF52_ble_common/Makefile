PROJECT_NAME   		 := nRF52_ble_common
PROJ_DIR       		 := .
nRF52_ble_common_DIR := .
SDK_COMMON_DIR		 := ../../SDK

USER_SRC += \
	$(PROJ_DIR)/main.c \

CFLAGS += -DUSE_COMMON_BOARD

# No backslash before this line or it will be ignored !
include $(nRF52_ble_common_DIR)/nRF52_ble_common.mk
