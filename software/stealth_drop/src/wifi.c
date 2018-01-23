// wifi.c

#include "wifi.h"
#include <string.h>
#include "uart.h"
#include "rtt.h"
#include "sd.h"

//#define DEBUG 1

extern char data_buff[MAX_BUFF_LEN + 1];

void wifi_break_stream_mode(void)
{
    char buff_break[] = "$$$\r\n";
    uart_send(buff_break);
    chThdSleep(MS2ST(1000));
}

void wifi_configure(void)
{
    rtt_printf("Wifi configuration...\n");
    wifi_command("set softap.auto_start true\r\n", 500);
    wifi_command("set softap.dhcp_server.enabled true\r\n", 500);
    wifi_command("set softap.ssid ZeROSEro7\r\n", 500);
    wifi_command("set tcp.server.auto_interface softap\r\n", 500);
    wifi_command("set tcp.server.auto_start true\r\n", 500);
    wifi_command("set tcp.server.idle_timeout 300\r\n", 500);
    wifi_command("set bus.log_bus uart1\r\n", 500);
    wifi_command("set bus.data_bus uart0\r\n", 500);
    wifi_command("set bus.mode stream\r\n", 500);
    wifi_command("save\r\n", 1000);
    wifi_command("reboot\r\n", 1000);
    rtt_printf("Success\n");
}

/* Reset wifi chip
*/
static void wifi_reset_chip(void)
{
    palSetPadMode(GPIOB, GPIOA_WIFI_NRST, PAL_MODE_OUTPUT_PUSHPULL);
    palClearPad(GPIOB, GPIOA_WIFI_NRST);  // rst on
    chThdSleep(MS2ST(100));
    palSetPad(GPIOB, GPIOA_WIFI_NRST);  // rst off
    chThdSleep(MS2ST(1000));
}

void wifi_init(void)
{
    rtt_printf("\n======== WIFI INITIALIZATION ========\n\n");
#ifdef DEBUG
    rtt_printf("Error codes:\n"\
        "  0: Command failed\n"\
        "  1: Unknown command\n"\
        "  2: Too few args\n"\
        "  3: Too many args\n"\
        "  4: Unknown variable or option\n"\
        "  5: Invalid argument\n"\
        "  6: Serial command line buffer overflow\n"\
        "  7: Bounds error, command specific, bounds of the command were exceeded\n\n");
#endif
    wifi_reset_chip();

    uart_init();
}

/* Send acommand to wifi chip using uart
** buff:    message to send buffer
*/
static void send_command(void* buff)
{
    uart_send(buff);
}

int wifi_command(void* buff, int timeout)
{
    send_command(buff);
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
    int nb_char_received;
    sd_file_open(filename, FA_WRITE);
    while((nb_char_received = uart_receive_timeout(data_buff, MAX_BUFF_LEN, MS2ST(TIMEOUT)))) {
        data_buff[nb_char_received] = '\0';
#ifdef DEBUG
        rtt_printf("%s", data_buff);
#endif
        sd_file_write();
    }
    sd_file_close();
    rtt_printf("\n");
    char response[] = "Success\n";
    uart_send(response);
}

void wifi_send_file(char* filename)
{
    if(sd_file_open(filename, FA_READ))
        return;
    char response[] = "Success\n";
    uart_send(response);
#ifdef DEBUG
    rtt_printf("Response sent: %s", response);
#endif
    unsigned int bytes_read = 0;
    do {
        sd_file_read(&bytes_read);
        uart_send(data_buff);
#ifdef DEBUG
        rtt_printf("data_read: %s\n", data_buff);
#endif
    } while(bytes_read != 0);
    rtt_printf("Download finished\n");
}

void wifi_wait_for(char* msg)
{
    rtt_printf("Waiting for: %s... ", msg);
    unsigned int char_received = 0;
    data_buff[1] = '\0';
    while(char_received != strlen(msg)) {
        if(uart_receive_timeout(data_buff, 1, MS2ST(1000))) {
            rtt_printf("%c", data_buff[0]);
            if(data_buff[0] == msg[char_received])
                char_received++;
            else
                char_received = 0;
        }
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