// wifi.c

#include "wifi.h"
#include <string.h>
#include "uart.h"
#include "rtt.h"
#include "sd.h"

//#define DEBUG

void wifi_break_stream_mode(void)
{
    char buff_break[] = "$$$\r\n";
    uart_send(buff_break);
    chThdSleep(MS2ST(1000));
}

void wifi_configure(void)
{
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
}

/* Reset wifi chip
*/
static void wifi_reset_chip(void)
{
    palSetPadMode(GPIOB, GPIOB_I2C1_SCL, PAL_MODE_OUTPUT_PUSHPULL);
    palClearPad(GPIOB, GPIOB_I2C1_SCL);  // rst on
    chThdSleep(MS2ST(100));
    palSetPad(GPIOB, GPIOB_I2C1_SCL);  // rst off
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
    char uart_buff[2];
    uart_buff[1] = '\0';
    while(uart_receive_timeout(uart_buff, 1, MS2ST(timeout)))
#ifdef DEBUG
        rtt_printf(uart_buff)
#endif
        ;
    return 0;
}

void wifi_save_file(char* filename)
{
    (void)filename;
    char uart_buff[BUFF_LEN + 1];
    int nb_char_received;
    while((nb_char_received = uart_receive_timeout(uart_buff, BUFF_LEN, MS2ST(TIMEOUT)))) {
        uart_buff[nb_char_received] = '\0';
#ifdef DEBUG
        rtt_printf("%s", uart_buff);
#endif
    }
    rtt_printf("\n");
    char response[] = "Success\n";
    uart_send(response);
}

void wifi_send_file(char* filename)
{
    if(strcmp(filename, "bleed_it_out.txt") != 0) {
        rtt_printf("[ERROR] File do not exists: %s\n", filename);
        char response[] = "No file\n";
        uart_send(response);
        rtt_printf("Response sent: %s", response);
        return;
    }
    char response[] = "Success\n";
    uart_send(response);
    rtt_printf("Response sent: %s", response);
    char file[] = "This is a file content.\nIf you can read this message, download was successful zefhvbeuvbhekvrjbhnskvrunhlkuvnhqkuvr grjk abjrkdvbhcukbg rajkbh kzjbhgvuvskbhgcjbejvnsklnhviudnbvjerhgcuzjhgvsukdhvusjhgvscdhgvnkjhbvnsfhgjchnfgjkchdjkvhjkvbhgnjkccngcjdhvbkjvdhnvcqjkhdvnjdfhvb jkhvnjkdqhvnjdhvndfjh jfvsjdfvnjdfvhnfjvnfjhvgnv jkhgnvdjfhvngjkvfnhvgnudkrjvgniuej vghiujbh vbhjrbgvjkdbhgniujcbhniugvjbhdrngjkvhsdrngjkvbhnsjkghb jrgh iurnkjh bhjfbhjfb uhj gbqhj hqkjhg qjgkvq nkjg hjk fdj ndf  !!\n";
    uart_send(file);
}

void wifi_wait_for(char* msg)
{
    rtt_printf("Waiting for: %s... ", msg);
    unsigned int char_received = 0;
    char uart_buff[2];
    uart_buff[1] = '\0';
    while(char_received != strlen(msg)) {
        if(uart_receive_timeout(uart_buff, 1, MS2ST(1000))) {
            rtt_printf("%c", uart_buff[0]);
            if(uart_buff[0] == msg[char_received])
                char_received++;
            else
                char_received = 0;
        }
    }
    rtt_printf("received !\n");
}

int wifi_get_word(char* buffer, int max_len, char separator)
{
    char uart_buff;
    int idx = 0;
    while(uart_receive_timeout(&uart_buff, 1, MS2ST(1000)) && idx < max_len) {
        if(uart_buff == separator)
            break;
        buffer[idx++] = uart_buff;
    }
    if(idx == max_len) {
        buffer[idx - 1] = '\0';
        rtt_printf("[ERROR] Word too long: %s\n", buffer);
        return 1;
    }
    buffer[idx] = '\0';
    return 0;
}