/*
    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include <string.h>

#include "ch.h"
#include "hal.h"

#include "chprintf.h"
#include "shell.h"

#include "rtt.h"
#include "SEGGER_RTT.h"

/*
 * Working area for driver.
 */
static uint8_t sd_scratchpad[512];

/*
 * SDIO configuration.
 */
static const SDCConfig sdccfg = {
    sd_scratchpad,
    SDC_MODE_4BIT};

/*
 * LED blinker thread, times are in milliseconds.
 */
static THD_WORKING_AREA(waThread1, 128);
static THD_FUNCTION(Thread1, arg)
{
    (void)arg;
    chRegSetThreadName("blinker");
    while(true) {
        palSetPad(GPIOC, GPIOC_LED);
        chThdSleepMilliseconds(500);
        palClearPad(GPIOC, GPIOC_LED);
        chThdSleepMilliseconds(500);
    }
}

/*===========================================================================*/
/* Command line related.                                                     */
/*===========================================================================*/

#define SHELL_WA_SIZE THD_WORKING_AREA_SIZE(2048)

#define SDC_BURST_SIZE 16

/* Buffer for block read/write operations, note that extra bytes are
   allocated in order to support unaligned operations.*/
static uint8_t buf[MMCSD_BLOCK_SIZE * SDC_BURST_SIZE + 4];

/* Additional buffer for sdcErase() test */
static uint8_t buf2[MMCSD_BLOCK_SIZE * SDC_BURST_SIZE];

void cmd_sdc(BaseSequentialStream *chp, int argc, char *argv[])
{
    (void)*chp;
    static const char *mode[] = {"SDV11", "SDV20", "MMC", NULL};
    systime_t          start, end;
    uint32_t           n, startblk;

    if(argc != 1) {
        SEGGER_RTT_printf(0, "Usage: sdiotest read|write|erase|all\r\n");
        return;
    }

    /* Card presence check.*/
    if(!blkIsInserted(&SDCD1)) {
        SEGGER_RTT_printf(0, "Card not inserted, aborting.\r\n");
        return;
    }

    /* Connection to the card.*/
    SEGGER_RTT_printf(0, "Connecting... ");
    if(sdcConnect(&SDCD1)) {
        SEGGER_RTT_printf(0, "failed\r\n");
        return;
    }

    SEGGER_RTT_printf(0, "OK\r\n\r\nCard Info\r\n");
    SEGGER_RTT_printf(0, "CSD      : %08X %8X %08X %08X \r\n",
             SDCD1.csd[3], SDCD1.csd[2], SDCD1.csd[1], SDCD1.csd[0]);
    SEGGER_RTT_printf(0, "CID      : %08X %8X %08X %08X \r\n",
             SDCD1.cid[3], SDCD1.cid[2], SDCD1.cid[1], SDCD1.cid[0]);
    SEGGER_RTT_printf(0, "Mode     : %s\r\n", mode[SDCD1.cardmode & 3U]);
    SEGGER_RTT_printf(0, "Capacity : %DMB\r\n", SDCD1.capacity / 2048);

    /* The test is performed in the middle of the flash area.*/
    startblk = (SDCD1.capacity / MMCSD_BLOCK_SIZE) / 2;

    if((strcmp(argv[0], "read") == 0) ||
       (strcmp(argv[0], "all") == 0)) {
        /* Single block read performance, aligned.*/
        SEGGER_RTT_printf(0, "Single block aligned read performance:           ");
        start = chVTGetSystemTime();
        end   = start + MS2ST(1000);
        n     = 0;
        do {
            if(blkRead(&SDCD1, startblk, buf, 1)) {
                SEGGER_RTT_printf(0, "failed\r\n");
                goto exittest;
            }
            n++;
        } while(chVTIsSystemTimeWithin(start, end));
        SEGGER_RTT_printf(0, "%D blocks/S, %D bytes/S\r\n", n, n * MMCSD_BLOCK_SIZE);

        /* Multiple sequential blocks read performance, aligned.*/
        SEGGER_RTT_printf(0, "16 sequential blocks aligned read performance:   ");
        start = chVTGetSystemTime();
        end   = start + MS2ST(1000);
        n     = 0;
        do {
            if(blkRead(&SDCD1, startblk, buf, SDC_BURST_SIZE)) {
                SEGGER_RTT_printf(0, "failed\r\n");
                goto exittest;
            }
            n += SDC_BURST_SIZE;
        } while(chVTIsSystemTimeWithin(start, end));
        SEGGER_RTT_printf(0, "%D blocks/S, %D bytes/S\r\n", n, n * MMCSD_BLOCK_SIZE);

#if STM32_SDC_SDIO_UNALIGNED_SUPPORT
        /* Single block read performance, unaligned.*/
        SEGGER_RTT_printf(0, "Single block unaligned read performance:         ");
        start = chVTGetSystemTime();
        end   = start + MS2ST(1000);
        n     = 0;
        do {
            if(blkRead(&SDCD1, startblk, buf + 1, 1)) {
                SEGGER_RTT_printf(0, "failed\r\n");
                goto exittest;
            }
            n++;
        } while(chVTIsSystemTimeWithin(start, end));
        SEGGER_RTT_printf(0, "%D blocks/S, %D bytes/S\r\n", n, n * MMCSD_BLOCK_SIZE);

        /* Multiple sequential blocks read performance, unaligned.*/
        SEGGER_RTT_printf(0, "16 sequential blocks unaligned read performance: ");
        start = chVTGetSystemTime();
        end   = start + MS2ST(1000);
        n     = 0;
        do {
            if(blkRead(&SDCD1, startblk, buf + 1, SDC_BURST_SIZE)) {
                SEGGER_RTT_printf(0, "failed\r\n");
                goto exittest;
            }
            n += SDC_BURST_SIZE;
        } while(chVTIsSystemTimeWithin(start, end));
        SEGGER_RTT_printf(0, "%D blocks/S, %D bytes/S\r\n", n, n * MMCSD_BLOCK_SIZE);
#endif /* STM32_SDC_SDIO_UNALIGNED_SUPPORT */
    }

    if((strcmp(argv[0], "write") == 0) ||
       (strcmp(argv[0], "all") == 0)) {
        unsigned i;

        memset(buf, 0xAA, MMCSD_BLOCK_SIZE * 2);
        SEGGER_RTT_printf(0, "Writing...");
        if(sdcWrite(&SDCD1, startblk, buf, 2)) {
            SEGGER_RTT_printf(0, "failed\r\n");
            goto exittest;
        }
        SEGGER_RTT_printf(0, "OK\r\n");

        memset(buf, 0x55, MMCSD_BLOCK_SIZE * 2);
        SEGGER_RTT_printf(0, "Reading...");
        if(blkRead(&SDCD1, startblk, buf, 1)) {
            SEGGER_RTT_printf(0, "failed\r\n");
            goto exittest;
        }
        SEGGER_RTT_printf(0, "OK\r\n");

        for(i      = 0; i < MMCSD_BLOCK_SIZE; i++)
            buf[i] = i + 8;
        SEGGER_RTT_printf(0, "Writing...");
        if(sdcWrite(&SDCD1, startblk, buf, 2)) {
            SEGGER_RTT_printf(0, "failed\r\n");
            goto exittest;
        }
        SEGGER_RTT_printf(0, "OK\r\n");

        memset(buf, 0, MMCSD_BLOCK_SIZE * 2);
        SEGGER_RTT_printf(0, "Reading...");
        if(blkRead(&SDCD1, startblk, buf, 1)) {
            SEGGER_RTT_printf(0, "failed\r\n");
            goto exittest;
        }
        SEGGER_RTT_printf(0, "OK\r\n");
    }

    if((strcmp(argv[0], "erase") == 0) ||
       (strcmp(argv[0], "all") == 0)) {
        /**
     * Test sdcErase()
     * Strategy:
     *   1. Fill two blocks with non-constant data
     *   2. Write two blocks starting at startblk
     *   3. Erase the second of the two blocks
     *      3.1. First block should be equal to the data written
     *      3.2. Second block should NOT be equal too the data written (i.e. erased).
     *   4. Erase both first and second block
     *      4.1 Both blocks should not be equal to the data initially written
     * Precondition: SDC_BURST_SIZE >= 2
     */
        memset(buf, 0, MMCSD_BLOCK_SIZE * 2);
        memset(buf2, 0, MMCSD_BLOCK_SIZE * 2);
        /* 1. */
        unsigned int i = 0;
        for(; i < MMCSD_BLOCK_SIZE * 2; ++i) {
            buf[i] = (i + 7) % 'T';  //Ensure block 1/2 are not equal
        }
        /* 2. */
        if(sdcWrite(&SDCD1, startblk, buf, 2)) {
            SEGGER_RTT_printf(0, "sdcErase() test write failed\r\n");
            goto exittest;
        }
        /* 3. (erase) */
        if(sdcErase(&SDCD1, startblk + 1, startblk + 2)) {
            SEGGER_RTT_printf(0, "sdcErase() failed\r\n");
            goto exittest;
        }
        sdcflags_t errflags = sdcGetAndClearErrors(&SDCD1);
        if(errflags) {
            SEGGER_RTT_printf(0, "sdcErase() yielded error flags: %d\r\n", errflags);
            goto exittest;
        }
        if(sdcRead(&SDCD1, startblk, buf2, 2)) {
            SEGGER_RTT_printf(0, "single-block sdcErase() failed\r\n");
            goto exittest;
        }
        /* 3.1. */
        if(memcmp(buf, buf2, MMCSD_BLOCK_SIZE) != 0) {
            SEGGER_RTT_printf(0, "sdcErase() non-erased block compare failed\r\n");
            goto exittest;
        }
        /* 3.2. */
        if(memcmp(buf + MMCSD_BLOCK_SIZE,
                  buf2 + MMCSD_BLOCK_SIZE, MMCSD_BLOCK_SIZE) == 0) {
            SEGGER_RTT_printf(0, "sdcErase() erased block compare failed\r\n");
            goto exittest;
        }
        /* 4. */
        if(sdcErase(&SDCD1, startblk, startblk + 2)) {
            SEGGER_RTT_printf(0, "multi-block sdcErase() failed\r\n");
            goto exittest;
        }
        if(sdcRead(&SDCD1, startblk, buf2, 2)) {
            SEGGER_RTT_printf(0, "single-block sdcErase() failed\r\n");
            goto exittest;
        }
        /* 4.1 */
        if(memcmp(buf, buf2, MMCSD_BLOCK_SIZE) == 0) {
            SEGGER_RTT_printf(0, "multi-block sdcErase() erased block compare failed\r\n");
            goto exittest;
        }
        if(memcmp(buf + MMCSD_BLOCK_SIZE,
                  buf2 + MMCSD_BLOCK_SIZE, MMCSD_BLOCK_SIZE) == 0) {
            SEGGER_RTT_printf(0, "multi-block sdcErase() erased block compare failed\r\n");
            goto exittest;
        }
        /* END of sdcErase() test */
    }

/* Card disconnect and command end.*/
exittest:
    sdcDisconnect(&SDCD1);
}

static const ShellCommand commands[] = {
    {"sdc", cmd_sdc},
    {NULL, NULL}};

static const ShellConfig shell_cfg1 = {
    (BaseSequentialStream *)&rtt_shell,
    commands};

/*
 * Application entry point.
 */
int main(void)
{
    /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
    halInit();
    chSysInit();

    /* Pinout config */
    //led_init
    palSetPadMode(GPIOC, GPIOC_LED, PAL_MODE_OUTPUT_PUSHPULL);
    palSetPad(GPIOC, GPIOC_LED);  //led off

    //sd_init
    palSetPadMode(GPIOC, GPIOC_SD_D0, PAL_MODE_ALTERNATE(12));
    palSetPadMode(GPIOC, GPIOC_SD_D1, PAL_MODE_ALTERNATE(12));
    palSetPadMode(GPIOC, GPIOC_SD_D2, PAL_MODE_ALTERNATE(12));
    palSetPadMode(GPIOC, GPIOC_SD_D3, PAL_MODE_ALTERNATE(12));
    palSetPadMode(GPIOC, GPIOC_SD_CLK, PAL_MODE_ALTERNATE(12));
    palSetPadMode(GPIOD, GPIOD_SD_CMD, PAL_MODE_ALTERNATE(12));

    /*
   * Shell manager initialization.
   */
    shellInit();

    /*
   * Activates the serial driver 6 using the driver default configuration.
   */
    sdStart(&SD6, NULL);

    /*
   * Initializes the SDIO drivers.
   */
    sdcStart(&SDCD1, &sdccfg);

    /*
   * Creates the blinker thread.
   */
    chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

    /*
   * Normal main() thread activity, spawning shells.
   */
    while(true) {
        thread_t *shelltp = chThdCreateFromHeap(NULL, SHELL_WA_SIZE,
                                                "shell", NORMALPRIO + 1,
                                                shellThread, (void *)&shell_cfg1);
        chThdWait(shelltp); /* Waiting termination.             */
        chThdSleepMilliseconds(1000);
    }
}
