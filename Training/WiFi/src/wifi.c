// wifi.c

#include "wifi.h"
#include <string.h>
#include "uart.h"
#include "rtt.h"

#define DEBUG 1

void wifi_init(void)
{
    uart_init();
}

/* Send acommand to wifi chip using uart
** buff: message to send buffer
*/
static void send_command(void* buff)
{
    rtt_printf(0, ">%s", buff);
    uart_send(buff);
}

/* Get header response from command
** bufflen: message to send length
** format:  message format, set to 'L' or 'R' (output)
** timeout: time to wait for WiFi chip response
** return:  data length or error (if < 0)
*/
static int get_header(int bufflen, char* format, int timeout)
{
    int     header_len = 9 + bufflen;
    uint8_t header_buffer[header_len];
    char    format_tmp;
    if(timeout == 0)
        timeout = TIME_INFINITE;
    // get header
    if(uart_receive_timeout(header_buffer, header_len, timeout))
        return 0;
    header_buffer[header_len - 2] = '\0';  // two last charaters are "\r\n"
#ifdef DEBUG
    rtt_printf(0, "Header: %s\n", header_buffer + header_len - 9);
#endif
    // set format
    format_tmp = header_buffer[header_len - 9];
    if(format != NULL)
        *format = format_tmp;
    // check error code
    int err_code = header_buffer[header_len - 7] - '0';
    if(err_code != 0) {
        rtt_printf(0, "[ERROR] An error occured. Error code:%d (", err_code);
        switch(err_code) {
            case 1: rtt_printf(0, "Command failed)\n"); break;
            case 2: rtt_printf(0, "Parse error)\n"); break;
            case 3: rtt_printf(0, "Unknown command)\n"); break;
            case 4: rtt_printf(0, "Too few args)\n"); break;
            case 5: rtt_printf(0, "Too many args)\n"); break;
            case 6: rtt_printf(0, "Unknown variable or option)\n"); break;
            case 7: rtt_printf(0, "Invalid argument)\n"); break;
            case 8: rtt_printf(0, "Serial command line buffer overflow)\n"); break;
            case 9: rtt_printf(0, "Bounds error, command specific, bounds of the command were exceeded)\n"); break;
            default: rtt_printf(0, "Enexpected error)\n");
        }
        return -err_code;
    }
    // extract data length from header
    int data_len = 0;
    for(int i = header_len - 7; i <= header_len - 3; i++)
        data_len = data_len * 10 + header_buffer[i] - '0';
#ifdef DEBUG
    rtt_printf(0, "Data len: %d\n", data_len);
#endif
    if(format_tmp == 'R')
        data_len += 2;  // data finish by "> " but are not counted in the header
    return data_len;
}

/* Get all data and display them
** data_len: size of the data (in bytes)
** format:   data format (Response or Log)
** timeout: time to wait for WiFi chip response
*/
static void print_data(int data_len, char format, int timeout)
{
    uint8_t data_buffer[MAX_DATA_BUFFER_LEN];
    while(data_len > 0) {
        data_buffer[0] = '\0';
        // set data buffer length
        int data_buff_len = data_len;
        if(data_len > MAX_DATA_BUFFER_LEN)
            data_buff_len = MAX_DATA_BUFFER_LEN;
        if(uart_receive_timeout(data_buffer, data_buff_len, timeout))
            break;
        data_len -= data_buff_len;
        if(data_len == 0){
            data_buffer[data_buff_len - 2] = '\0'; // last characters are "\r\n"
            if(format == 'R')
                data_buffer[data_buff_len - 4] = '\0'; // last characters are "\r\n> "
        }
        // print data
        rtt_printf(0, "%s", data_buffer);
    }
    rtt_printf(0, "\n\n");
}

int wifi_command(void* buff, int timeout)
{
    char format;
    int buff_len = strlen(buff);
    send_command(buff);
    int data_len = 0;
    if(timeout)
        timeout = MS2ST(timeout);
    else
        timeout = TIME_INFINITE;
    do {
        data_len = get_header(buff_len, &format, timeout);
        if(data_len < 0) // An error occured
            return data_len;
        if(data_len) // Data length could be null if there was a timeout
            print_data(data_len, format, timeout);
        buff_len = 0;
    } while(data_len != 0); // Until there is a response of the WiFi chip
    return 0;
}