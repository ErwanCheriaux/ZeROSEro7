//main.c

#include <string.h>
#include "ch.h"
#include "hal.h"

#include "led.h"
#include "pwm.h"
#include "timer.h"
#include "wifi.h"
#include "rtt.h"
#include "uart.h"

int main(void)
{
    halInit();
    chSysInit();

    led_init();
    pwm_init();
    timer_init();
    wifi_init();
    rtt_init();

    led_on();

    break_stream_mode();
    configure();

    char start_seq[] = START_SEQ;
    char buff;
    char filename[MAX_FILENAME_SIZE];
    while(1) {
        wifi_wait_for(start_seq);
        if(!uart_receive_timeout(&buff, 1, MS2ST(1000)))
            continue;
        switch(buff) {
            case 'U': // Upload a file
                rtt_printf("File upload: ");
                if(wifi_get_word(filename, MAX_FILENAME_SIZE, '\n'))
                    break;
                rtt_printf("%s\n", filename);
                wifi_save_file(filename);
                break;
            case 'D': // Download a file
                rtt_printf("File download: ");
                if(wifi_get_word(filename, MAX_FILENAME_SIZE, '\n'))
                    break;
                rtt_printf("%s\n", filename);
                chThdSleep(MS2ST(500));
                wifi_send_file(filename);
                rtt_printf("Download ended\n");
                break;
            case 'R': // Remove a file
                rtt_printf("Remove file: ");
                if(wifi_get_word(filename, MAX_FILENAME_SIZE, '\n'))
                    break;
                rtt_printf("%s\n", filename);
                // TODO: Remove file
                rtt_printf("Done\n");
                break;
            case 'L': // Get file list
                rtt_printf("List asked\n");
                chThdSleep(MS2ST(500));
                strcpy(filename, "First_file_name.txt\0");
                uart_send(filename);
                strcpy(filename, " \0");
                uart_send(filename);
                strcpy(filename, "bleed_it_out.txt\0");
                uart_send(filename);
                strcpy(filename, " \0");
                uart_send(filename);
                strcpy(filename, "Second_file_name.txt\0");
                uart_send(filename);
                strcpy(filename, "\n\0");
                uart_send(filename);
                rtt_printf("List sent\n");
                break;
            default: rtt_printf("[ERROR] Unkown command: %c\n", buff);
        }
    }
    
    chThdSleep(TIME_INFINITE);
    return 0;
}
