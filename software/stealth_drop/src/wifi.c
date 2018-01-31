// wifi.c

#include "wifi.h"
#include <string.h>
#include "uart.h"
#include "rtt.h"
#include "sd.h"
#include "timer.h"

//#define DEBUG 1
#define TIMEOUT 2000

extern char data_buff[MAX_BUFF_LEN + 1];

static int sleep_mode = 0;

void wifi_break_stream_mode(void)
{
    char buff_break[] = "$$$\r\n";
    uart_send(buff_break, strlen(buff_break));
    chThdSleep(MS2ST(500));
}

void wifi_configure(void)
{
    rtt_printf("Wifi configuration...\n");
    wifi_command("set softap.auto_start true\r\n", 500);
    wifi_command("set softap.dhcp_server.enabled true\r\n", 500);
    wifi_command("set softap.ssid ZeROSEro7\r\n", 500);
    wifi_command("set softap.passkey zerosero7\r\n", 500);
    wifi_command("set tcp.server.auto_interface softap\r\n", 500);
    wifi_command("set tcp.server.auto_start true\r\n", 500);
    wifi_command("set tcp.server.idle_timeout 300\r\n", 500);
    wifi_command("set bus.log_bus uart1\r\n", 500);
    wifi_command("set bus.data_bus uart0\r\n", 500);
    wifi_command("set uart.baud 1 921600\r\n", 500);
    wifi_command("set uart.baud 0 921600\r\n", 500);
    wifi_command("set uart.flow 0 on\r\n", 500);
    wifi_command("set system.wakeup.events gpio22\r\n", 500);
    wifi_command("set bus.mode stream\r\n", 500);
    wifi_command("save\r\n", 1000);
    wifi_command("reboot\r\n", 1000);
    rtt_printf("Success\n");
}

/* Reset wifi chip
*/
static void wifi_reset_chip(void)
{
    palClearPad(GPIOB, GPIOB_I2C1_SCL);  // rst on
    chThdSleep(MS2ST(100));
    palSetPad(GPIOB, GPIOB_I2C1_SCL);  // rst off
    chThdSleep(MS2ST(1000));
}

static void wifi_sleep(void)
{
#ifdef DEBUG
    rtt_printf("Wifi chip in sleep mode\n");
#endif
    wifi_reset_chip();
    wifi_break_stream_mode();
    wifi_command("sleep\r\n", 500);
    sleep_mode = 0;
}

void wifi_init(void)
{
    rtt_printf("\n======== WIFI INITIALIZATION ========\n\n");
#ifdef DEBUG
    rtt_printf(
        "Error codes:\n"
        "  0: Command failed\n"
        "  1: Unknown command\n"
        "  2: Too few args\n"
        "  3: Too many args\n"
        "  4: Unknown variable or option\n"
        "  5: Invalid argument\n"
        "  6: Serial command line buffer overflow\n"
        "  7: Bounds error, command specific, bounds of the command were exceeded\n\n");
#endif
    palSetPadMode(GPIOB, GPIOB_I2C1_SCL, PAL_MODE_OUTPUT_PUSHPULL);
    wifi_reset_chip();

    uart_init();
}

int wifi_command(void* buff, int timeout)
{
    uart_send(buff, strlen(buff));
    data_buff[1] = '\0';
    while(uart_receive_timeout(data_buff, 1, MS2ST(timeout)))
#ifdef DEBUG
        rtt_printf(data_buff)
#endif
            ;
    return 0;
}

void wifi_save_file(char* filename)
{
    // test to measure SD write speed
    (void)filename;
    int size = 5000;
    for(int i = 0; i < MAX_BUFF_LEN; i++)
        data_buff[i] = i;
    uint32_t     time = chVTGetSystemTime();
    sd_file_open("test_write_speed.txt", FA_WRITE); 
    for(int i = 0; i < size; i++)
        sd_file_write(MAX_BUFF_LEN);
    size *= 1024;
    // end of test
    sd_file_close();
    time = ST2MS(chVTGetSystemTime() - time);  // time used for upload in ms
    size /= 1024;                              // data size uploaded in KB
    rtt_printf("Upload %dKB in %dms (%dKB/s)\n", size, time, (size * 1000) / time);
    chThdSleep(MS2ST(10000));
    char response[] = "Success\n";
    uart_send(response, strlen(response));
}

void wifi_send_file(char* filename)
{
    if(sd_file_open(filename, FA_READ))
        return;
    unsigned int bytes_read = 0;
    uint32_t     time       = chVTGetSystemTime();
    int          size       = 0;
    do {
        sd_file_read(&bytes_read);
#ifdef DEBUG
        rtt_printf("(%d)\n", bytes_read);
#endif
        uart_send(data_buff, bytes_read);
        size += bytes_read;
    } while(bytes_read != 0);
    sd_file_close();
    time = ST2MS(chVTGetSystemTime() - time);  // time used for upload in ms
    size /= 1024;                              // data size uploaded in KB
    rtt_printf("Download %dKB in %dms (%dKB/s)\n", size, time, (size * 1000) / time);
    rtt_printf("Download finished\n");
}

void wifi_wait_for(char* msg)
{
    rtt_printf("Waiting for: %s... ", msg);
    unsigned int char_received = 0;
    data_buff[1]               = '\0';
    while(char_received != strlen(msg)) {
        if(uart_receive_timeout(data_buff, 1, MS2ST(1000))) {
            rtt_printf("%c", data_buff[0]);
            if(data_buff[0] == msg[char_received])
                char_received++;
            else
                char_received = 0;
        } else if(sleep_mode)
            wifi_sleep();
    }
    rtt_printf("received !\n");
}

int wifi_get_word(char* buffer, int max_len, char separator)
{
    int idx = 0;
    while(uart_receive_timeout(data_buff, 1, MS2ST(1000)) && idx < max_len) {
        if(data_buff[0] == separator)
            break;
        buffer[idx++] = data_buff[0];
    }
    if(idx == max_len) {
        buffer[idx - 1] = '\0';
        rtt_printf("[ERROR] Word too long: %s\n", buffer);
        return 1;
    }
    buffer[idx] = '\0';
    return 0;
}

void wifi_sleep_callback(void)
{
    sleep_mode = 1;
}