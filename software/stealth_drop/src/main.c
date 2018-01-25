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
#include "led.h"
#include "pwm.h"
#include "timer.h"
#include "rtt.h"
#include "wifi.h"
#include "uart.h"
#include "sd.h"

static const SDCConfig sdccfg = {
    0,             // SD Card do not need it
    SDC_MODE_4BIT  // bus width (D0, D1, D2, ...)
};

extern char data_buff[MAX_BUFF_LEN + 1];

int main(void)
{
    halInit();
    chSysInit();

    timer_init();
    led_init();
    pwm_init();
    timer_init();
    wifi_init();
    rtt_init();

    led_on();

    //wifi_break_stream_mode();
    //wifi_configure();

    // Initializes the SDIO drivers.
    sdcStart(&SDCD1, &sdccfg);

    rtt_printf("\n======== INITIALIZATION SUCCESSFUL ========\n\n");

    char filename_test[] = "test.txt";
    strcpy(data_buff, "If you see this message, Read and Write functions are working !\n");
    sd_file_open(filename_test, FA_WRITE);
    sd_file_write(strlen(data_buff));
    sd_file_close();
    sd_file_open(filename_test, FA_READ);
    unsigned int bytes_read = 0;
    sd_file_read(&bytes_read);
    rtt_printf("data_read: %s\n", data_buff);
    sd_file_close();
    while(sd_get_next_filename() == 0)
        rtt_printf("filename: %s\n", data_buff);
    rtt_printf("If you see a file list including \"%s\", listing is working !\n", filename_test);
    sd_file_remove(filename_test);
    while(sd_get_next_filename() == 0)
        rtt_printf("filename: %s\n", data_buff);
    rtt_printf("If you do not see file called \"%s\", removing is working !\n", filename_test);

    rtt_printf("\n======== SD CARD TESTS FINISHED ========\n\n");

    char start_seq[] = START_SEQ;
    char buff;
    char filename[MAX_FILENAME_SIZE];
    while(1) {
        wifi_wait_for(start_seq);
        if(!uart_receive_timeout(&buff, 1, MS2ST(1000)))
            continue;
        timer_off();
        switch(buff) {
            case 'U':  // Upload a file
                rtt_printf("File upload: ");
                if(wifi_get_word(filename, MAX_FILENAME_SIZE, '\n'))
                    break;
                rtt_printf("%s\n", filename);
                wifi_save_file(filename);
                break;
            case 'D':  // Download a file
                rtt_printf("File download: ");
                if(wifi_get_word(filename, MAX_FILENAME_SIZE, '\n'))
                    break;
                rtt_printf("%s\n", filename);
                chThdSleep(MS2ST(500));
                wifi_send_file(filename);
                rtt_printf("Download ended\n");
                break;
            case 'R':  // Remove a file
                rtt_printf("Remove file: ");
                if(wifi_get_word(filename, MAX_FILENAME_SIZE, '\n'))
                    break;
                rtt_printf("%s\n", filename);
                sd_file_remove(filename);
                rtt_printf("Done\n");
                break;
            case 'L':  // Get file list
                rtt_printf("List asked\n");
                chThdSleep(MS2ST(500));
                while(sd_get_next_filename() == 0) {
                    rtt_printf("filename: %s\n", data_buff);
                    strcpy(filename, data_buff);
                    uart_send(filename, strlen(filename));
                    strcpy(filename, " ");
                    uart_send(filename, strlen(filename));
                }
                strcpy(filename, "\n");
                uart_send(filename, strlen(filename));
                rtt_printf("List sent\n");
                break;
            default: rtt_printf("[ERROR] Unkown command: %c\n", buff);
        }
        timer_on(SLEEP_DELAY, wifi_sleep_callback, 0);
    }

    chThdSleep(TIME_INFINITE);
    return 0;
}
