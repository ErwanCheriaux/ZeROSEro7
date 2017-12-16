// wifi.c

#include "wifi.h"
#include <string.h>
#include "uart.h"
#include "rtt.h"
#include "sha256.h"

#define DEBUG 1

// init white list
const unsigned char white_list[WHITE_LIST_SIZE][65] = {
    {0x48,0x4f,0x95,0xf3,0x56,0x35,0xe2,0x82,0xca,0x6b,0xca,0x99,0x6b,0x6c,0xd8,0x34,0xdf,0xe8,0x1e,0xfe,0x04,0xe6,0x2e,0x7c,0x1f,0x85,0xa5,0xbb,0x1c,0xf1,0x37,0x88}, // ZeROSEro7
    {0xde,0xbd,0x23,0xd6,0xf2,0xa5,0x53,0xf2,0xa0,0x03,0xd3,0x42,0x3d,0x57,0x37,0xe3,0x5f,0x6b,0xb8,0x97,0xdd,0xad,0x48,0xfa,0xc3,0x0a,0x3b,0x0b,0x5d,0x21,0x05,0x68}, // zerosero7
    {0x2d,0xc0,0xf1,0x77,0x81,0xf0,0xf8,0xd9,0x62,0x09,0xd1,0x3f,0xf9,0x32,0xc9,0xfd,0x3b,0x1e,0x3b,0xaf,0x90,0x75,0xec,0x6b,0x52,0x50,0x17,0x88,0xc8,0x26,0xa3,0xaa} // My ASUS
};

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
*/
static void print_data(int data_len, char format)
{
    uint8_t data_buffer[MAX_DATA_BUFFER_LEN];
    while(data_len > 0) {
        data_buffer[0] = '\0';
        // set data buffer length
        int data_buff_len = data_len;
        if(data_len > MAX_DATA_BUFFER_LEN)
            data_buff_len = MAX_DATA_BUFFER_LEN;
        uart_receive(data_buffer, data_buff_len);
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

int wifi_command(void* buff)
{
    char format;
    send_command(buff);
    int count = 0;
    int timeout = 0;
    int data_len = 0;
    do {
        data_len = get_header(strlen(buff), &format, timeout);
        if(data_len < 0)
            return data_len;
        if(data_len)
            print_data(data_len, format);
        count++;
        timeout = 100;
    } while(data_len != 0);
    return 0;
}

int find_devices(void)
{
    // scan command
    send_command("scan\r\n");
    int data_len = get_header(6, NULL, 0);
    // handle get_header error
    if(data_len < 0)
        return data_len;
    // get data
    uint8_t data_buffer[MAX_DATA_BUFFER_LEN];
    int stored_data_buff_len = 0;
    while(data_len > 0) {
        // set data buffer length
        int data_buff_len = data_len + stored_data_buff_len;
        if(data_len >= MAX_DATA_BUFFER_LEN)
            data_buff_len = MAX_DATA_BUFFER_LEN - 1;
        data_buffer[data_buff_len] = '\0';
        uart_receive(data_buffer + stored_data_buff_len, data_buff_len - stored_data_buff_len);
        data_len -= data_buff_len - stored_data_buff_len;
        if(data_len == 0) // only for the end of reception
            data_buffer[data_buff_len - 4] = '\0'; // last characters are "\r\n> "
        uint8_t * new_lf_pos = data_buffer;
        while(1) {
            // get first occurence of a network new line
            uint8_t * lf_pos = (uint8_t *)strstr((char *)new_lf_pos,  "\n#");
            if(lf_pos == NULL)
                break;
            // get next occurence of a network new line
            new_lf_pos = (uint8_t *)strstr((char *)lf_pos + 2,  "\n#");
            if(new_lf_pos == NULL && data_len != 0) {
                stored_data_buff_len = data_buff_len - (lf_pos - data_buffer);
                strcpy((char *)data_buffer, (char *)lf_pos);
                break;
            }
            // check if there is no offset in the line
            if(*(lf_pos + 31) == ' ') {
                uint8_t * ssid = lf_pos + 32;
                // end ssid string
                *(new_lf_pos - 1) = '\0';
                // calculate ssid hash
                unsigned char hash[SHA256_LEN];
                sha256(ssid, hash);
                int equal = 1;
                for(int i = 0; i < WHITE_LIST_SIZE; i++){
                    equal = 1;
                    for(int j = 0; j < SHA256_LEN; j++){
                        if(hash[j] != white_list[i][j]){
                            equal = 0;
                            break;
                        }
                    }
                    if(equal == 1)
                        break;
                }
                if(equal)
                    rtt_printf(0, "\nFOUND !: %s\n", ssid);
                rtt_printf(0, "\nSSID: %s", ssid);
                // TODO: ssid must be handled

                lf_pos = new_lf_pos;
            }
#ifdef DEBUG
            else
                rtt_printf(0, "[ERROR] Offset error: '%c'(%d) != ' '\n", *(lf_pos + 31), *(lf_pos + 31));
            if(data_len == 0) // only for the end of reception
                    break;
#endif
        }
    }
    rtt_printf(0, "\n\n");

    return 0;
}