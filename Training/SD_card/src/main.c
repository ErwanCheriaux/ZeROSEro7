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

static const SDCConfig sdccfg = {
    0,            // SD Card do not need it
    SDC_MODE_4BIT // bus width (D0, D1, D2, ...)
};

extern char data_buff[MAX_BUFF_LEN + 1];

int main(void)
{
    halInit();
    chSysInit();

    //led_init
    palSetPadMode(GPIOC, GPIOC_LED, PAL_MODE_OUTPUT_PUSHPULL);
    palSetPad(GPIOC, GPIOC_LED);  //led off
    // Activates the serial driver 6 using the driver default configuration.
    sdStart(&SD6, NULL);
    // Initializes the SDIO drivers.
    sdcStart(&SDCD1, &sdccfg);

    rtt_printf("================ INITIALIZATION SUCCESSFUL ================\n\n");

    char filename[] = "test.txt";
    strcpy(data_buff, "If you see this message, Read and Write functions are working !\n");
    sd_file_open(filename, FA_WRITE);
    sd_file_write();
    sd_file_close();
    sd_file_open(filename, FA_READ);
    unsigned int bytes_read = 0;
    sd_file_read(&bytes_read);
    rtt_printf("data_read: %s\n", data_buff);
    sd_file_close();
    while(sd_get_next_filename() == 0)
        rtt_printf("filename: %s\n", data_buff);
    rtt_printf("If you see a file list including \"%s\", listing is working !\n", filename);
    sd_file_remove(filename);
    while(sd_get_next_filename() == 0)
        rtt_printf("filename: %s\n", data_buff);
    rtt_printf("If you do not see file called \"%s\", removing is working !\n", filename);

    chThdSleep(TIME_INFINITE);
    return 0;
}

