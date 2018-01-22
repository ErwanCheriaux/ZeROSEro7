#	Common ble targets for nRF52

#	Required definitions :
# PROJ_DIR          := .
# PROJECT_NAME      := nRF52_ble_common
# SDK_COMMON_DIR	:= ../../SDK
# nRF52_ble_common_DIR := .
# CFLAGS += -DUSE_COMMON_BOARD  custom board.h must be included in this board.h

TARGETS          := nrf52832_xxaa
OUTPUT_DIRECTORY := _build

NORDIC_DIR	 := $(SDK_COMMON_DIR)/BLE_nordic
SDK_ROOT     := $(NORDIC_DIR)/nRF5_SDK
COMMAND_ROOT := $(NORDIC_DIR)/nRF5x-Command-Line-Tools

GNU_PREFIX    := arm-none-eabi-
LINKER_SCRIPT := $(nRF52_ble_common_DIR)/$(TARGETS).ld

GDB := $(GNU_PREFIX)gdb
ELF := $(OUTPUT_DIRECTORY)/$(TARGETS).out
HEX := $(OUTPUT_DIRECTORY)/$(TARGETS).hex

USER_SRC += \
	$(nRF52_ble_common_DIR)/rtt.c \
	$(nRF52_ble_common_DIR)/ble_central.c \
	$(nRF52_ble_common_DIR)/ble_peripheral_gap.c \
	$(nRF52_ble_common_DIR)/ble_peripheral_gatt.c \
	$(nRF52_ble_common_DIR)/ble_uart_service.c \

USER_INC += $(PROJ_DIR) \
	$(nRF52_ble_common_DIR) \

# Source files common to all targets
SRC_FILES += $(USER_SRC) \
	$(SDK_ROOT)/components/libraries/experimental_log/src/nrf_log_backend_rtt.c \
	$(SDK_ROOT)/components/libraries/experimental_log/src/nrf_log_backend_serial.c \
	$(SDK_ROOT)/components/libraries/experimental_log/src/nrf_log_backend_uart.c \
	$(SDK_ROOT)/components/libraries/experimental_log/src/nrf_log_default_backends.c \
	$(SDK_ROOT)/components/libraries/experimental_log/src/nrf_log_frontend.c \
	$(SDK_ROOT)/components/libraries/experimental_log/src/nrf_log_str_formatter.c \
	$(SDK_ROOT)/components/libraries/button/app_button.c \
	$(SDK_ROOT)/components/libraries/util/app_error.c \
	$(SDK_ROOT)/components/libraries/util/app_error_weak.c \
	$(SDK_ROOT)/components/libraries/scheduler/app_scheduler.c \
	$(SDK_ROOT)/components/libraries/timer/app_timer.c \
	$(SDK_ROOT)/components/libraries/util/app_util_platform.c \
	$(SDK_ROOT)/components/libraries/crc16/crc16.c \
	$(SDK_ROOT)/components/libraries/fds/fds.c \
	$(SDK_ROOT)/components/libraries/hardfault/hardfault_implementation.c \
	$(SDK_ROOT)/components/libraries/util/nrf_assert.c \
	$(SDK_ROOT)/components/libraries/atomic_fifo/nrf_atfifo.c \
	$(SDK_ROOT)/components/libraries/balloc/nrf_balloc.c \
	$(SDK_ROOT)/external/fprintf/nrf_fprintf.c \
	$(SDK_ROOT)/external/fprintf/nrf_fprintf_format.c \
	$(SDK_ROOT)/components/libraries/fstorage/nrf_fstorage.c \
	$(SDK_ROOT)/components/libraries/fstorage/nrf_fstorage_sd.c \
	$(SDK_ROOT)/components/libraries/experimental_memobj/nrf_memobj.c \
	$(SDK_ROOT)/components/libraries/pwr_mgmt/nrf_pwr_mgmt.c \
	$(SDK_ROOT)/components/libraries/experimental_section_vars/nrf_section_iter.c \
	$(SDK_ROOT)/components/libraries/strerror/nrf_strerror.c \
	$(SDK_ROOT)/components/libraries/util/sdk_mapped_flags.c \
	$(SDK_ROOT)/components/boards/boards.c \
	$(SDK_ROOT)/components/drivers_nrf/clock/nrf_drv_clock.c \
	$(SDK_ROOT)/components/drivers_nrf/common/nrf_drv_common.c \
	$(SDK_ROOT)/components/drivers_nrf/gpiote/nrf_drv_gpiote.c \
	$(SDK_ROOT)/components/drivers_nrf/uart/nrf_drv_uart.c \
	$(SDK_ROOT)/components/libraries/bsp/bsp.c \
	$(SDK_ROOT)/components/libraries/bsp/bsp_btn_ble.c \
	$(SDK_ROOT)/components/libraries/bsp/bsp_nfc.c \
	$(SDK_ROOT)/external/segger_rtt/SEGGER_RTT.c \
	$(SDK_ROOT)/external/segger_rtt/SEGGER_RTT_Syscalls_GCC.c \
	$(SDK_ROOT)/external/segger_rtt/SEGGER_RTT_printf.c \
	$(SDK_ROOT)/components/ble/common/ble_advdata.c \
	$(SDK_ROOT)/components/ble/ble_advertising/ble_advertising.c \
	$(SDK_ROOT)/components/ble/common/ble_conn_params.c \
	$(SDK_ROOT)/components/ble/common/ble_conn_state.c \
	$(SDK_ROOT)/components/ble/ble_db_discovery/ble_db_discovery.c \
	$(SDK_ROOT)/components/ble/common/ble_srv_common.c \
	$(SDK_ROOT)/components/ble/peer_manager/gatt_cache_manager.c \
	$(SDK_ROOT)/components/ble/peer_manager/gatts_cache_manager.c \
	$(SDK_ROOT)/components/ble/peer_manager/id_manager.c \
	$(SDK_ROOT)/components/ble/nrf_ble_gatt/nrf_ble_gatt.c \
	$(SDK_ROOT)/components/ble/peer_manager/peer_data_storage.c \
	$(SDK_ROOT)/components/ble/peer_manager/peer_database.c \
	$(SDK_ROOT)/components/ble/peer_manager/peer_id.c \
	$(SDK_ROOT)/components/ble/peer_manager/peer_manager.c \
	$(SDK_ROOT)/components/ble/peer_manager/pm_buffer.c \
	$(SDK_ROOT)/components/ble/peer_manager/pm_mutex.c \
	$(SDK_ROOT)/components/ble/peer_manager/security_dispatcher.c \
	$(SDK_ROOT)/components/ble/peer_manager/security_manager.c \
    $(SDK_ROOT)/components/ble/ble_services/ble_dis/ble_dis.c \
	$(SDK_ROOT)/components/toolchain/gcc/gcc_startup_nrf52.S \
	$(SDK_ROOT)/components/toolchain/system_nrf52.c \
	$(SDK_ROOT)/components/ble/ble_services/ble_ans_c/ble_ans_c.c \
	$(SDK_ROOT)/components/softdevice/common/nrf_sdh.c \
	$(SDK_ROOT)/components/softdevice/common/nrf_sdh_ble.c \
	$(SDK_ROOT)/components/softdevice/common/nrf_sdh_soc.c \
    $(SDK_ROOT)/components/drivers_nrf/spi_master/nrf_drv_spi.c \
	$(SDK_ROOT)/components/libraries/mem_manager/mem_manager.c \

# Include folders common to all targets
INC_FOLDERS += $(USER_INC) \
	$(SDK_ROOT)/components/drivers_nrf/comp \
	$(SDK_ROOT)/components/drivers_nrf/twi_master \
	$(SDK_ROOT)/components/ble/ble_services/ble_ancs_c \
	$(SDK_ROOT)/components/ble/ble_services/ble_ias_c \
	$(SDK_ROOT)/components/libraries/pwm \
	$(SDK_ROOT)/components/softdevice/s132/headers/nrf52 \
	$(SDK_ROOT)/components/libraries/usbd/class/cdc/acm \
	$(SDK_ROOT)/components/libraries/usbd/class/hid/generic \
	$(SDK_ROOT)/components/libraries/usbd/class/msc \
	$(SDK_ROOT)/components/libraries/usbd/class/hid \
	$(SDK_ROOT)/components/ble/ble_services/ble_lbs \
	$(SDK_ROOT)/components/ble/ble_services/ble_gls \
	$(SDK_ROOT)/components/libraries/fstorage \
	$(SDK_ROOT)/components/drivers_nrf/i2s \
	$(SDK_ROOT)/components/libraries/mutex \
	$(SDK_ROOT)/components/libraries/gpiote \
	$(SDK_ROOT)/components/libraries/experimental_log/src \
	$(SDK_ROOT)/components/drivers_nrf/gpiote \
	$(SDK_ROOT)/components/boards \
	$(SDK_ROOT)/components/libraries/experimental_memobj \
	$(SDK_ROOT)/components/drivers_nrf/common \
	$(SDK_ROOT)/components/ble/ble_advertising \
	$(SDK_ROOT)/components/ble/ble_services/ble_bas_c \
	$(SDK_ROOT)/components/ble/ble_services/ble_hrs_c \
	$(SDK_ROOT)/components/libraries/queue \
	$(SDK_ROOT)/components/libraries/pwr_mgmt \
	$(SDK_ROOT)/components/ble/ble_dtm \
	$(SDK_ROOT)/components/toolchain/cmsis/include \
	$(SDK_ROOT)/components/ble/ble_services/ble_rscs_c \
	$(SDK_ROOT)/components/drivers_nrf/uart \
	$(SDK_ROOT)/components/ble/common \
	$(SDK_ROOT)/components/ble/ble_services/ble_lls \
	$(SDK_ROOT)/components/drivers_nrf/wdt \
	$(SDK_ROOT)/components/libraries/bsp \
	$(SDK_ROOT)/components/ble/ble_db_discovery \
	$(SDK_ROOT)/components/ble/ble_services/ble_bas \
	$(SDK_ROOT)/components/libraries/experimental_section_vars \
	$(SDK_ROOT)/components/softdevice/s132/headers \
	$(SDK_ROOT)/components/ble/ble_services/ble_ans_c \
	$(SDK_ROOT)/components/libraries/slip \
	$(SDK_ROOT)/components/libraries/mem_manager \
	$(SDK_ROOT)/external/segger_rtt \
	$(SDK_ROOT)/components/libraries/usbd/class/cdc \
	$(SDK_ROOT)/components/drivers_nrf/hal \
	$(SDK_ROOT)/components/ble/ble_services/ble_nus_c \
	$(SDK_ROOT)/components/drivers_nrf/rtc \
	$(SDK_ROOT)/components/softdevice/common \
	$(SDK_ROOT)/components/ble/ble_services/ble_ias \
	$(SDK_ROOT)/components/libraries/usbd/class/hid/mouse \
	$(SDK_ROOT)/components/libraries/ecc \
	$(SDK_ROOT)/components/drivers_nrf/ppi \
	$(SDK_ROOT)/components/ble/ble_services/ble_dfu \
	$(SDK_ROOT)/external/fprintf \
	$(SDK_ROOT)/components/drivers_nrf/twis_slave \
	$(SDK_ROOT)/components/libraries/atomic \
	$(SDK_ROOT)/components \
	$(SDK_ROOT)/components/libraries/scheduler \
	$(SDK_ROOT)/components/libraries/cli \
	$(SDK_ROOT)/components/libraries/experimental_log \
	$(SDK_ROOT)/components/ble/ble_services/ble_hts \
	$(SDK_ROOT)/components/drivers_nrf/delay \
	$(SDK_ROOT)/components/libraries/crc16 \
	$(SDK_ROOT)/components/drivers_nrf/timer \
	$(SDK_ROOT)/components/libraries/util \
	$(SDK_ROOT)/components/drivers_nrf/pwm \
	$(SDK_ROOT)/components/libraries/csense_drv \
	$(SDK_ROOT)/components/libraries/csense \
	$(SDK_ROOT)/components/libraries/balloc \
	$(SDK_ROOT)/components/libraries/low_power_pwm \
	$(SDK_ROOT)/components/libraries/hardfault \
	$(SDK_ROOT)/components/ble/ble_services/ble_cscs \
	$(SDK_ROOT)/components/libraries/uart \
	$(SDK_ROOT)/components/libraries/hci \
	$(SDK_ROOT)/components/libraries/usbd/class/hid/kbd \
	$(SDK_ROOT)/components/drivers_nrf/spi_slave \
	$(SDK_ROOT)/components/drivers_nrf/lpcomp \
	$(SDK_ROOT)/components/libraries/timer \
	$(SDK_ROOT)/components/drivers_nrf/rng \
	$(SDK_ROOT)/components/drivers_nrf/power \
	$(SDK_ROOT)/components/libraries/usbd/config \
	$(SDK_ROOT)/components/toolchain \
	$(SDK_ROOT)/components/libraries/led_softblink \
	$(SDK_ROOT)/components/drivers_nrf/qdec \
	$(SDK_ROOT)/components/ble/ble_services/ble_cts_c \
	$(SDK_ROOT)/components/drivers_nrf/spi_master \
	$(SDK_ROOT)/components/ble/ble_services/ble_nus \
	$(SDK_ROOT)/components/libraries/twi_mngr \
	$(SDK_ROOT)/components/ble/ble_services/ble_hids \
	$(SDK_ROOT)/components/libraries/strerror \
	$(SDK_ROOT)/components/drivers_nrf/pdm \
	$(SDK_ROOT)/components/libraries/crc32 \
	$(SDK_ROOT)/components/libraries/usbd/class/audio \
	$(SDK_ROOT)/components/ble/peer_manager \
	$(SDK_ROOT)/components/drivers_nrf/swi \
	$(SDK_ROOT)/components/ble/ble_services/ble_tps \
	$(SDK_ROOT)/components/ble/ble_services/ble_dis \
	$(SDK_ROOT)/components/device \
	$(SDK_ROOT)/components/ble/nrf_ble_gatt \
	$(SDK_ROOT)/components/ble/nrf_ble_qwr \
	$(SDK_ROOT)/components/libraries/button \
	$(SDK_ROOT)/components/libraries/usbd \
	$(SDK_ROOT)/components/drivers_nrf/saadc \
	$(SDK_ROOT)/components/libraries/atomic_fifo \
	$(SDK_ROOT)/components/ble/ble_services/ble_lbs_c \
	$(SDK_ROOT)/components/ble/ble_racp \
	$(SDK_ROOT)/components/toolchain/gcc \
	$(SDK_ROOT)/components/libraries/fds \
	$(SDK_ROOT)/components/libraries/twi \
	$(SDK_ROOT)/components/drivers_nrf/clock \
	$(SDK_ROOT)/components/ble/ble_services/ble_rscs \
	$(SDK_ROOT)/components/drivers_nrf/usbd \
	$(SDK_ROOT)/components/ble/ble_services/ble_hrs \

# Libraries common to all targets
LIB_FILES += \

# Optimization flags
OPT = -Og -g3 -std=gnu11
# Uncomment the line below to enable link time optimization
#OPT += -flto

# C flags common to all targets
CFLAGS += $(OPT)
CFLAGS += -DBOARD_PCA10040
CFLAGS += -DCONFIG_GPIO_AS_PINRESET
CFLAGS += -DNRF52
CFLAGS += -DNRF52832_XXAA
CFLAGS += -DNRF52_PAN_74
CFLAGS += -DNRF_SD_BLE_API_VERSION=5
CFLAGS += -DS132
CFLAGS += -DSOFTDEVICE_PRESENT
CFLAGS += -DSWI_DISABLE0
CFLAGS += -mcpu=cortex-m4
CFLAGS += -mthumb -mabi=aapcs
CFLAGS +=  -Wall -Werror -Wno-unused-function -Wno-unused-variable -Wno-expansion-to-defined
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
# keep every function in a separate section, this allows linker to discard unused ones
CFLAGS += -ffunction-sections -fdata-sections -fno-strict-aliasing
CFLAGS += -fno-builtin -fshort-enums

# C++ flags common to all targets
CXXFLAGS += $(OPT)

# Assembler flags common to all targets
ASMFLAGS += -g3
ASMFLAGS += -mcpu=cortex-m4
ASMFLAGS += -mthumb -mabi=aapcs
ASMFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
ASMFLAGS += -DBOARD_PCA10040
ASMFLAGS += -DCONFIG_GPIO_AS_PINRESET
ASMFLAGS += -DNRF52
ASMFLAGS += -DNRF52832_XXAA
ASMFLAGS += -DNRF52_PAN_74
ASMFLAGS += -DNRF_SD_BLE_API_VERSION=5
ASMFLAGS += -DS132
ASMFLAGS += -DSOFTDEVICE_PRESENT
ASMFLAGS += -DSWI_DISABLE0

# Linker flags
LDFLAGS += $(OPT)
LDFLAGS += -mthumb -mabi=aapcs -L $(TEMPLATE_PATH) -T$(LINKER_SCRIPT)
LDFLAGS += -mcpu=cortex-m4
LDFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
# let linker dump unused sections
LDFLAGS += -Wl,--gc-sections
# use newlib in nano version
LDFLAGS += --specs=nano.specs


# Add standard libraries at the very end of the linker input, after all objects
# that may need symbols provided by these libraries.
LIB_FILES += -lc -lnosys -lm


.PHONY: default help

# Default target - first one defined
default: nrf52832_xxaa

# Print all targets that can be built
help:
	@echo following targets are available:
	@echo		nrf52832_xxaa
	@echo		flash          - flashing binary
	@echo		startgdbserver - open a gdb connection with the device
	@echo		debug          - after startgdbserver, load and debug a prog on the device
	@echo		clean          - remove _build

TEMPLATE_PATH := $(SDK_ROOT)/components/toolchain/gcc

include $(TEMPLATE_PATH)/Makefile.common

$(foreach target, $(TARGETS), $(call define_target, $(target)))

.PHONY: flash startgdbserver debug rtttelnet

# Flash the program
flash: $(HEX)
	@echo Flashing: $<
	$(COMMAND_ROOT)/nrfjprog/nrfjprog -f nrf52 --program $< --sectorerase
	$(COMMAND_ROOT)/nrfjprog/nrfjprog -f nrf52 --reset

NRFJPROG = $(SDK_ROOT)/../nRF5x-Command-Line-Tools/nrfjprog/nrfjprog

# Flash SoftDevice. Must be done once for the board.
# Could be linked, but takes time to load
# Allocated flash area : (0x0 - 0x22fff)
# Allocated ram area : (0x20000000 - 0x200020df)
flash_softdevice:
	@echo Flashing: s132_nrf52_5.0.0_softdevice.hex
	$(NRFJPROG) -f nrf52 --program $(SDK_ROOT)/components/softdevice/s132/hex/s132_nrf52_5.0.0_softdevice.hex --sectorerase
	$(NRFJPROG) -f nrf52 --reset

recover:
	$(NRFJPROG) --recover

startgdbserver:
	JLinkGDBServer -device nrf52832_xxAA -if swd -speed 1000 -port 2331

debug: $(ELF)
	$(GDB) $<

rtttelnet:
	telnet localhost 19021

format: *.c *.h
	clang-format -i $^
