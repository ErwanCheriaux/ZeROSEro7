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
#include <stdio.h>

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
static FATFS fatfs;
static FIL file;
static DIR directory;
static FILINFO fileinfos;

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

static void getTime(char* str, int time)
{
    sprintf(str, "%02d", (time >> 11) & 0b11111);
    str[2] = ':';
    sprintf(str+3, "%02d", (time >> 5) & 0b111111);
    str[5] = ':';
    sprintf(str+6,   "%02d", time & 0b11111);
    str[8] = '\0';
}

static void getDate(char* str, int date)
{
    sprintf(str,   "%02d", date & 0b11111);
    str[2] = '/';
    sprintf(str+3, "%02d", (date >> 5) & 0b1111);
    str[5] = '/';
    sprintf(str+6, "%04d", ((date >> 9) & 0b1111111) + 1980);
    str[10] = '\0';
}

static int read_folder(char* path)
{
    int res = 0;
    char date_str[11];
    char time_str[9];
    // open folder
    if(f_print_error(f_opendir(&directory, path)))
        return 1;
    // print each elements
    while(1) {
        if(f_print_error(f_readdir(&directory, &fileinfos))) {
            res = 1;
            break;
        }
        if(fileinfos.fname[0] == 0) {
            rtt_printf("end of folder\n");
            break;
        }
        rtt_printf("fsize: %d\n", fileinfos.fsize);
        getDate(date_str, fileinfos.fdate);
        rtt_printf("fdate: %d [%s]\n", fileinfos.fdate, date_str);
        getTime(time_str, fileinfos.ftime);
        rtt_printf("ftime: %d [%s]\n", fileinfos.ftime, time_str);
        rtt_printf("fattrib: %d [ ", fileinfos.fattrib);
        if(fileinfos.fattrib & AM_RDO)
            rtt_printf("READONLY ");
        if(fileinfos.fattrib & AM_HID)
            rtt_printf("HIDDEN ");
        if(fileinfos.fattrib & AM_SYS)
            rtt_printf("SYSTEM ");
        if(fileinfos.fattrib & AM_DIR)
            rtt_printf("DIRECTORY ");
        if(fileinfos.fattrib & AM_ARC)
            rtt_printf("ARCHVE ");
        rtt_printf("]\n");
        rtt_printf("fname: %s\n\n", fileinfos.fname);
    }
    // close folder
    if(f_print_error(f_closedir(&directory)))
        return 1;
    return res;
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
    rtt_printf("> sdcConnect SUCCESS\n");

    // mount when it will be needed
    if(f_print_error(f_mount(&fatfs, "/", 1)))
        goto disconnect;
    rtt_printf("> f_mount SUCCESS\n");

    // read root dir
    char path[] = "/";
    if(read_folder(path))
        goto disconnect;
    rtt_printf("> read_folder SUCCESS\n");

    // create folder if it doesn-t already exists
    char dir_name[] = "FOLDER";
    int res = f_mkdir(dir_name);
    if(res != FR_EXIST && res != FR_OK) {
        f_print_error(res);
        goto disconnect;
    }
    rtt_printf("> f_mkdir SUCCESS\n");

    // create file and erase if it already exists
    char file_path[] = "FOLDER/FILENAME.TXT";
    if(f_print_error(f_open(&file, file_path, FA_CREATE_ALWAYS | FA_WRITE | FA_READ)))
        goto disconnect;
    rtt_printf("> f_open SUCCESS\n");

    // read previous folder
    if(read_folder(dir_name))
        goto disconnect;
    rtt_printf("> read_folder SUCCESS\n");

    // close opened file
    if(f_print_error(f_close(&file)))
        goto disconnect;
    rtt_printf("> f_close SUCCESS\n");

    // remove previous file
    if(f_print_error(f_unlink(file_path)))
        goto disconnect;
    rtt_printf("> f_unlink SUCCESS\n");

    // read previous folder
    if(read_folder(dir_name))
        goto disconnect;
    rtt_printf("> read_folder SUCCESS\n");

    // remove previous folder
    if(f_print_error(f_unlink(dir_name)))
        goto disconnect;
    rtt_printf("> f_unlink SUCCESS\n");

    // read previous folder
    if(read_folder(path))
        goto disconnect;
    rtt_printf("> read_folder SUCCESS\n");

disconnect:
    // disconnect
    if(sdcDisconnect(&SDCD1)) {
        rtt_printf("[ERROR] sdcDisconnect ERROR\n");
        goto sleep;
    }
    rtt_printf("> sdcDisconnect SUCCESS\n");

sleep:
    chThdSleep(TIME_INFINITE);
    return 0;
}

