// wifi.c

#include "wifi.h"
#include <string.h>
#include "uart.h"
#include "rtt.h"

void break_stream_mode(void)
{
    char buff_break[] = "$$$";
    uart_send(buff_break);
}

void configure(void)
{
    wifi_command("set softap.auto_start true\r\n", 1000, 1);
    wifi_command("set softap.dhcp_server.enabled true\r\n", 1000, 1);
    wifi_command("set softap.ssid ZeROSEro7\r\n", 1000, 1);
    wifi_command("set tcp.server.auto_interface softap\r\n", 1000, 1);
    wifi_command("set tcp.server.auto_start true\r\n", 1000, 1);
    wifi_command("set tcp.server.idle_timeout 300\r\n", 1000, 1);
    wifi_command("set bus.mode stream\r\n", 1000, 1);
    wifi_command("save\r\n", 1000, 1);
    wifi_command("reboot\r\n", 5000, 1);
}

void wifi_init(void)
{
    rtt_printf(0, "\n======== WIFI INITIALIZATION ========\n\n");

    // rst wifi chip
    palSetPadMode(GPIOB, GPIOB_I2C1_SCL, PAL_MODE_OUTPUT_PUSHPULL);
    palClearPad(GPIOB, GPIOB_I2C1_SCL);  // rst on
    chThdSleep(MS2ST(100));
    palSetPad(GPIOB, GPIOB_I2C1_SCL);  // rst off
    chThdSleep(MS2ST(2000));

    uart_init();

    // display infos
    rtt_printf(0, "Error codes:\n");
    rtt_printf(0, "  Command failed\n");
    rtt_printf(0, "  Parse error\n");
    rtt_printf(0, "  Unknown command\n");
    rtt_printf(0, "  Too few args\n");
    rtt_printf(0, "  Too many args\n");
    rtt_printf(0, "  Unknown variable or option\n");
    rtt_printf(0, "  Invalid argument\n");
    rtt_printf(0, "  Serial command line buffer overflow\n");
    rtt_printf(0, "  Bounds error, command specific, bounds of the command were exceeded\n\n");
    rtt_printf(0, "\n======== INITIALIZATION SUCCED ========\n\n");
}

/* Send acommand to wifi chip using uart
** buff:    message to send buffer
** display: will command be displayed?
*/
static void send_command(void* buff, int display)
{
    if(display)
        rtt_printf(0, ">%s", buff);
    uart_send(buff);
}

int wifi_command(void* buff, int timeout, int display)
{
    send_command(buff, display);
    char uart_buff[2];
    uart_buff[1] = '\0';
    while(uart_receive_timeout(uart_buff, 1, MS2ST(timeout)))
        rtt_printf(0, uart_buff);
    return 0;
}

void wifi_save_file(char* filename)
{
    (void)filename;
}

void wifi_send_file(char* filename)
{
    (void)filename;
}

void wifi_wait_for(char* msg)
{
    rtt_printf(0, "Waiting for: %s... ", msg);
    unsigned int char_received = 0;
    char uart_buff[2];
    uart_buff[1] = '\0';
    while(char_received != strlen(msg)) {
        if(uart_receive_timeout(uart_buff, 1, MS2ST(1000))) {
            rtt_printf(0, "%s", uart_buff);
            if(uart_buff[0] == msg[char_received])
                char_received++;
            else
                char_received = 0;
        }
    }
    rtt_printf(0, "received !\n");
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
        rtt_printf(0, "[ERROR] Word too long: %s\n", buffer);
        return 1;
    }
    buffer[idx] = '\0';
    return 0;
}