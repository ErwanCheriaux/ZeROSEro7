#ifndef BOARD_H
#define BOARD_H

#ifdef USE_SPY_TALK_BOARD

#include "board_spy_talk.h"

#else
#ifdef USE_COMMON_BOARD

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

#else

#error("Please select a board.h")

#endif // USE_COMMON_BOARD
#endif // USE_SPY_TALK_BOARD

#endif // BOARD_H
