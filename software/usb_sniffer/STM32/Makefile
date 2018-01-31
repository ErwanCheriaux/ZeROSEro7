##############################################################################
# Build global options
# NOTE: Can be overridden externally.
#

# Compiler options here.
ifeq ($(USE_OPT),)
  USE_OPT = -O2 -ggdb -std=gnu99
endif

#
# Build global options
##############################################################################


##############################################################################
# Project, sources and paths
#

# Define project name here
PROJECT = usb_sniffer

# Imported source files and paths
CHIBIOS = ../../../SDK/ChibiOS
CHIBIOS_CONTRIB = $(CHIBIOS)/community
# Startup files.
include $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC/mk/startup_stm32f2xx.mk
# HAL-OSAL files (optional).
include ./board/board.mk
include $(CHIBIOS)/os/hal/hal.mk
include $(CHIBIOS)/os/hal/osal/rt/osal.mk
include $(CHIBIOS)/os/hal/ports/STM32/STM32F4xx/platform.mk
include $(CHIBIOS_CONTRIB)/os/hal/hal.mk
include $(CHIBIOS_CONTRIB)/os/hal/ports/STM32/STM32F4xx/platform.mk
# RTOS files (optional).
include $(CHIBIOS)/os/rt/rt.mk
include $(CHIBIOS)/os/common/ports/ARMCMx/compilers/GCC/mk/port_v7m.mk
# Other files (optional).
include $(CHIBIOS)/test/rt/test.mk
include $(CHIBIOS)/os/hal/lib/streams/streams.mk
include $(CHIBIOS)/os/various/shell/shell.mk

# Define linker script file here
LDSCRIPT= $(STARTUPLD)/STM32F407xG.ld

PROJ = .
SRC = src

# C sources that can be compiled in ARM or THUMB mode depending on the global
# setting.
CSRC = $(STARTUPSRC) \
       $(KERNSRC) \
       $(PORTSRC) \
       $(OSALSRC) \
       $(HALSRC) \
       $(PLATFORMSRC) \
       $(BOARDSRC) \
       $(TESTSRC) \
       $(LWSRC) \
       $(FATFSSRC) \
       $(STREAMSSRC) \
       $(SHELLSRC) \
       $(CHIBIOS)/os/various/evtimer.c \
       $(SRC)/main.c \
       $(SRC)/led.c \
       $(SRC)/usb.c \
       $(SRC)/usbh.c \
       $(SRC)/spi.c \
       $(SRC)/timer.c \
       $(SRC)/SEGGER_RTT.c \
       $(SRC)/usb_hid_keys.c \
       $(SRC)/SEGGER_RTT_printf.c \
       $(SRC)/usbh_custom_class_example.c

# List ASM source files here
ASMXSRC = $(STARTUPASM) $(PORTASM) $(OSALASM)

INCDIR = $(CHIBIOS)/os/license \
         $(STARTUPINC) $(KERNINC) $(PORTINC) $(OSALINC) \
         $(HALINC) $(PLATFORMINC) $(BOARDINC) $(TESTINC) \
         $(STREAMSINC) $(SHELLINC) $(LWINC) $(FATFSINC) \
         $(CHIBIOS)/os/various \
		 $(TEST_FIBO) \
		 $(SRC) \
		 $(PROJ)

#
# Project, sources and paths
##############################################################################

##############################################################################
# Compiler settings
#

MCU  = cortex-m3
ELF  = build/$(PROJECT).elf

TRGT = arm-none-eabi-
CC   = $(TRGT)gcc
GDB  = $(TRGT)gdb
LD   = $(TRGT)gcc
CP   = $(TRGT)objcopy
AS   = $(TRGT)gcc -x assembler-with-cpp
AR   = $(TRGT)ar
OD   = $(TRGT)objdump
SZ   = $(TRGT)size
HEX  = $(CP) -O ihex
BIN  = $(CP) -O binary

# THUMB-specific options here
TOPT = -mthumb -DTHUMB

# Define C warning options here
CWARN = -Wall -Wextra -Werror -Wundef -Wstrict-prototypes -Wno-unused-function -Wno-implicit-fallthrough -Wno-unused-but-set-variable

#
# Compiler settings
##############################################################################

##############################################################################
# Démon JLink
#
#

RULESPATH = $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC
include $(RULESPATH)/rules.mk

help:
	@echo		startgdbserver - open a gdb connection with the device
	@echo		startgdbserver - close a gdb connection with the device
	@echo		debug          - after startgdbserver, load and debug a prog on the device
	@echo		telnet         - open rtt communication
	@echo		format         - format code
	@echo		clean          - remove build

startgdbserver:
	@pidof JLinkGDBServer > /dev/null || JLinkGDBServer -if swd -speed auto -device STM32F205RG

stopgdbserver:
	@pidof JLinkGDBServer > /dev/null && killall JLinkGDBServer || true

debug: $(ELF)
	$(GDB) $<

format: $(SRC)/*.c $(SRC)/*.h
	clang-format -i $^

telnet:
	telnet localhost 19021

.PHONY: help startgdbserver stopgdbserver debug format telnet

#
# Démon JLink
##############################################################################
