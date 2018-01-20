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
#include "ff.h"

static const SDCConfig sdccfg = {
    0,            // SD Card do not need it
    SDC_MODE_4BIT // bus width (D0, D1, D2, ...)
};
uint8_t working_buff[4096] __attribute__ ((aligned (4))) ; // Working buffer
FATFS fatfs;

static int f_print_error(int error)
{
    if(error == FR_OK)
        return error;
    switch(error) {
        case FR_DISK_ERR:            rtt_printf("[ERROR] FR_DISK_ERR\n"); break;
        case FR_INT_ERR:             rtt_printf("[ERROR] FR_INT_ERR\n"); break;
        case FR_NOT_READY:           rtt_printf("[ERROR] FR_NOT_READY\n"); break;
        case FR_NO_FILE:             rtt_printf("[ERROR] FR_NO_FILE\n"); break;
        case FR_NO_PATH:             rtt_printf("[ERROR] FR_NO_PATH\n"); break;
        case FR_INVALID_NAME:        rtt_printf("[ERROR] FR_INVALID_NAME\n"); break;
        case FR_DENIED:              rtt_printf("[ERROR] FR_DENIED\n"); break;
        case FR_EXIST:               rtt_printf("[ERROR] FR_EXIST\n"); break;
        case FR_INVALID_OBJECT:      rtt_printf("[ERROR] FR_INVALID_OBJECT\n"); break;
        case FR_WRITE_PROTECTED:     rtt_printf("[ERROR] FR_WRITE_PROTECTED\n"); break;
        case FR_INVALID_DRIVE:       rtt_printf("[ERROR] FR_INVALID_DRIVE\n"); break;
        case FR_NOT_ENABLED:         rtt_printf("[ERROR] FR_NOT_ENABLED\n"); break;
        case FR_NO_FILESYSTEM:       rtt_printf("[ERROR] FR_NO_FILESYSTEM\n"); break;
        case FR_MKFS_ABORTED:        rtt_printf("[ERROR] FR_MKFS_ABORTED\n"); break;
        case FR_TIMEOUT:             rtt_printf("[ERROR] FR_TIMEOUT\n"); break;
        case FR_LOCKED:              rtt_printf("[ERROR] FR_LOCKED\n"); break;
        case FR_NOT_ENOUGH_CORE:     rtt_printf("[ERROR] FR_NOT_ENOUGH_CORE\n"); break;
        case FR_TOO_MANY_OPEN_FILES: rtt_printf("[ERROR] FR_TOO_MANY_OPEN_FILES\n"); break;
        case FR_INVALID_PARAMETER:   rtt_printf("[ERROR] FR_INVALID_PARAMETER\n"); break;
        default:                     rtt_printf("[ERROR] UNEXPECTED ERROR\n");
    }
    return error;
}

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

    // connect
    if(sdcConnect(&SDCD1)) {
        rtt_printf("[ERROR] sdcConnect ERROR\n");
        goto sleep;
    }
    rtt_printf("sdcConnect SUCCESS\n");

    // read at 0x0000
    uint8_t value = 0;
    int addr = 0;
    if(sd_read_byte(addr, &value)) {
        rtt_printf("[ERROR] sd_read_byte ERROR\n");
        goto disconnect;
    }
    rtt_printf("sd_read_byte SUCCESS 0x%08X: 0x%02X\n", addr, value);

    // read at 0x0001
    addr = 1;
    if(sd_read_byte(addr, &value)) {
        rtt_printf("[ERROR] sd_read_byte ERROR\n");
        goto disconnect;
    }
    rtt_printf("sd_read_byte SUCCESS 0x%08X: 0x%02X\n", addr, value);
    
    // write 0x12 at 0x0000
    value = 0x12;
    addr = 0;
    if(sd_write_byte(addr, value)) {
        rtt_printf("[ERROR] sd_write_byte ERROR\n");
        goto disconnect;
    }
    rtt_printf("sd_write_byte SUCCESS 0x%08X: 0x%02X\n", addr, value);

    // read at 0x0000
    addr = 0;
    if(sd_read_byte(addr, &value)) {
        rtt_printf("[ERROR] sd_read_byte ERROR\n");
        goto disconnect;
    }
    rtt_printf("sd_read_byte SUCCESS 0x%08X: 0x%02X\n", addr, value);
    
    // write 0x34 at 0x0001
    value = 0x34;
    addr = 1;
    if(sd_write_byte(addr, value)) {
        rtt_printf("[ERROR] sd_write_byte ERROR\n");
        goto disconnect;
    }
    rtt_printf("sd_write_byte SUCCESS 0x%08X: 0x%02X\n", addr, value);

    // read at 0x0001
    addr = 1;
    if(sd_read_byte(addr, &value)) {
        rtt_printf("[ERROR] sd_read_byte ERROR\n");
        goto disconnect;
    }
    rtt_printf("sd_read_byte SUCCESS 0x%08X: 0x%02X\n", addr, value);

    // write 0x00 at 0x0000
    value = 0x00;
    addr = 0;
    if(sd_write_byte(addr, value)) {
        rtt_printf("[ERROR] sd_write_byte ERROR\n");
        goto disconnect;
    }
    rtt_printf("sd_write_byte SUCCESS 0x%08X: 0x%02X\n", addr, value);

    // write 0x00 at 0x0001
    value = 0x00;
    addr = 1;
    if(sd_write_byte(addr, value)) {
        rtt_printf("[ERROR] sd_write_byte ERROR\n");
        goto disconnect;
    }
    rtt_printf("sd_write_byte SUCCESS 0x%08X: 0x%02X\n", addr, value);

    // mount when it will be needed
    if(f_print_error(f_mount(&fatfs, "/", 1)))
        goto disconnect;
    rtt_printf("f_mount SUCCESS\n");

    /*// create file system to the logical drive
    if(f_print_error(f_mkfs("", FM_ANY | FM_FAT32, 0, working_buff, sizeof working_buff)))
        goto disconnect;
    rtt_printf("f_mount SUCCESS\n");*/

disconnect:
    // disconnect
    if(sdcDisconnect(&SDCD1)) {
        rtt_printf("[ERROR] sdcDisconnect ERROR\n");
        goto sleep;
    }
    rtt_printf("sdcDisconnect SUCCESS\n");

sleep:
    chThdSleep(TIME_INFINITE);
    return 0;
}

