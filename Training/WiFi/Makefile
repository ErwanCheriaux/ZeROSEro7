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
PROJECT = usb_dev_kit

# Imported source files and paths
CHIBIOS = ../../SDK/ChibiOS
# Startup files.
include $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC/mk/startup_stm32f4xx.mk
# HAL-OSAL files (optional).
include $(CHIBIOS)/os/hal/hal.mk
include $(CHIBIOS)/os/hal/ports/STM32/STM32F4xx/platform.mk
include ./board_E407/board.mk
include $(CHIBIOS)/os/hal/osal/rt/osal.mk
# RTOS files (optional).
include $(CHIBIOS)/os/rt/rt.mk
include $(CHIBIOS)/os/common/ports/ARMCMx/compilers/GCC/mk/port_v7m.mk

# Define linker script file here
LDSCRIPT= $(STARTUPLD)/STM32F407xG.ld

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
	   		src/main.c \
			src/led.c \
			src/timer.c \
			src/pwm.c \
			src/uart.c \
			src/SEGGER_RTT.c \
			src/SEGGER_RTT_printf.c

# List ASM source files here
ASMXSRC = $(STARTUPASM) $(PORTASM) $(OSALASM)

INCDIR = $(CHIBIOS)/os/license \
         $(STARTUPINC) $(KERNINC) $(PORTINC) $(OSALINC) \
         $(HALINC) $(PLATFORMINC) $(BOARDINC) $(TESTINC) \
         $(STREAMSINC) $(SHELLINC) $(LWINC) $(FATFSINC) \
         $(CHIBIOS)/os/various

#
# Project, sources and paths
##############################################################################

##############################################################################
# Compiler settings
#

MCU  = cortex-m4
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
CWARN = -Wall -Wextra -Werror -Wundef -Wstrict-prototypes -Wno-implicit-fallthrough

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
	@echo		format         - format code
	@echo		clean          - remove build

start:
	    @pidof JLinkGDBServer > /dev/null || JLinkGDBServer -if swd -speed auto -device STM32F405ZG

stop:
	    @pidof JLinkGDBServer > /dev/null && killall JLinkGDBServer || true

db: $(ELF)
	    $(GDB) $<

format: src/*.c src/*.h
	clang-format -i $^

telnet:
	telnet localhost 19021

.PHONY: help startgdbserver stopgdbserver debug format

#
# Démon JLink
##############################################################################
