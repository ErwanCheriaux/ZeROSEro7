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
#include <stdlib.h>

#include "ch.h"
#include "hal.h"

#include "chprintf.h"
#include "shell.h"

#include "rtt.h"
#include "sd.h"

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

extern uint8_t buf[SD_BUF_SIZE];

/*===========================================================================*/
/* Command line related.                                                     */
/*===========================================================================*/

#define SHELL_WA_SIZE THD_WORKING_AREA_SIZE(2048)

void cmd_sdc(BaseSequentialStream *chp, int argc, char *argv[])
{
    (void)*chp;

    if(argc < 1 || argc > 3)
        goto usage;

    if(strcmp(argv[0], "write") == 0) {
        if (argc == 3) // write a single value
            sd_write_byte(atoi(argv[1]), atoi(argv[2]));
        else
            goto usage;
    }
    else if(strcmp(argv[0], "read") == 0) {
        if(argc == 3) { // print a whole area
            int size = atoi(argv[2]);
            int addr = atoi(argv[1]);
            uint8_t buffer[size];
            sd_read(addr, size, buffer);
            // print
            int nb_bytes_per_row = 8;
            if(addr % nb_bytes_per_row != 0) {
                rtt_printf("0x%08X  ", ((int)addr/nb_bytes_per_row)*nb_bytes_per_row);
                for(int i = 0; i < addr % nb_bytes_per_row; i++)
                    rtt_printf("   ");
            }
            for (int i = 0; i < size; i++) {
                if((addr + i) % nb_bytes_per_row == 0)
                    rtt_printf("\n0x%08X  ", addr + i);
                rtt_printf("%02X ", buffer[i]);
            }
            rtt_printf("\n");
        }
        else if (argc == 2) { // print a single value
            uint8_t value;
            int addr = atoi(argv[1]);
            sd_read_byte(addr, &value);
            rtt_printf("0x%08X  %02X\n", addr, value);
        }
        else
            goto usage;
    }
    else if(strcmp(argv[0], "test") == 0) { // read and write test
        if(argc == 1) {
            int addr = FILE_AREA;
            uint8_t value = 0;
            sd_write_byte(addr, value);
            uint8_t result;
            sd_read_byte(addr, &result);
            if(result != value) {
                rtt_printf("[ERROR] SD: Value 0x%02X has been written but 0x%02X was read\n", value, result);
                return;
            }
            value = 123;
            sd_write_byte(addr, value);
            sd_read_byte(addr, &result);
            if(result != value) {
                rtt_printf("[ERROR] SD: Value 0x%02X has been written but 0x%02X was read\n", value, result);
                return;
            }
            int len = 0x200;
            uint8_t buffer[len];
            for(int i = 0; i < len; i++)
                buffer[i] = i;
            sd_write(FILE_AREA, len, buffer);
            sd_read(FILE_AREA, len, buffer);
            int res = 0;
            for(int i = 0; i < len; i++) {
                if(buffer[i] != i % 0x100) {
                    res = 1;
                    rtt_printf("[ERROR] SD: Value 0x%02X has been written at 0x%08X but 0x%02X was read\n", i % 0x100, i, buffer[i]);
                    break;
                }
            }
            if(res) {
                rtt_printf("[ERROR] SD: Area value is different from area written\n");
                return;
            }
            rtt_printf("[INFO] SD: Tests successfully passed\n");
        }
        else
            goto usage;
    }
    return;
usage:
    rtt_printf("Invalid parameters\n");
    rtt_printf("Usage: sdc read <addr> |\n" \
               "           read <addr> <size> |\n" \
               "           write <addr> <value>\n" \
               "(values in decimal)\n");
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
