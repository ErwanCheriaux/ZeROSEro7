PROJECT_NAME   		 := stealth_drop
PROJ_DIR       		 := .
nRF52_ble_common_DIR := ../../nRF52_ble_common
SDK_COMMON_DIR		 := ../../../SDK

USER_SRC += \
	$(PROJ_DIR)/main.c \

CFLAGS += -DUSE_STEALTH_DROP_BOARD

# No backslash before this line or it will be ignored!
include $(nRF52_ble_common_DIR)/nRF52_ble_common.mk
