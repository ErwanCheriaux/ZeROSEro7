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

void cmd_sdc(BaseSequentialStream *chp, int argc, char *argv[])
{
    (void)*chp;

    if(argc != 1) {
        rtt_printf("Usage: sdiotest read|write|all");
        return;
    }

    /* Card presence check.*/
    if(!blkIsInserted(&SDCD1)) {
        rtt_printf("Card not inserted, aborting.");
        return;
    }

    /* Connection to the card.*/
    rtt_printf("Connecting... ");
    if(sdcConnect(&SDCD1)) {
        rtt_printf("failed");
        return;
    }

    /* The test is performed in the middle of the flash area.*/
    if((strcmp(argv[0], "read") == 0) ||
       (strcmp(argv[0], "all") == 0)) {
        rtt_printf("Read the first block:");
        int nb_blocks_read = 1;
        if(blkRead(&SDCD1, 0, buf, nb_blocks_read)) {
            rtt_printf("Reading failed");
            goto exittest;
        }
        for(unsigned int i = 0; i < MMCSD_BLOCK_SIZE * nb_blocks_read / 4; i++)
            rtt_printf("%02X%02X %02X%02X", buf[4*i], buf[4*i+1], buf[4*i+2], buf[4*i+3]);
    }

    uint32_t startblk = (SDCD1.capacity / MMCSD_BLOCK_SIZE) / 2;
    if((strcmp(argv[0], "write") == 0) ||
       (strcmp(argv[0], "all") == 0)) {
        unsigned i;

        memset(buf, 0x55, MMCSD_BLOCK_SIZE * 2);
        rtt_printf("Reading...");
        if(blkRead(&SDCD1, startblk, buf, 1)) {
            rtt_printf("failed");
            goto exittest;
        }
        rtt_printf("OK");

        for(i      = 0; i < MMCSD_BLOCK_SIZE; i++)
            buf[i] = i + 8;
        rtt_printf("Writing...");
        if(sdcWrite(&SDCD1, startblk, buf, 2)) {
            rtt_printf("failed");
            goto exittest;
        }
        rtt_printf("OK");

        memset(buf, 0, MMCSD_BLOCK_SIZE * 2);
        rtt_printf("Reading...");
        if(blkRead(&SDCD1, startblk, buf, 1)) {
            rtt_printf("failed");
            goto exittest;
        }
        rtt_printf("OK");

        rtt_printf("Write '0' at position 0");
        // clean buffer
        for(unsigned int i = 0; i < sizeof(buf); i++)
            buf[i] = 0;
        buf[0] = '0';
        if(sdcWrite(&SDCD1, 0, buf, 1)) {
            rtt_printf("Writing failed");
            goto exittest;
        }
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
    //palSetPadMode(GPIOC, GPIOC_LED, PAL_MODE_OUTPUT_PUSHPULL);
    palSetPad(GPIOC, GPIOC_LED);  //led off
    
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
