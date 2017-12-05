// wifi.c

#include "wifi.h"
#include <string.h>
#include "uart.h"
#include "rtt.h"

#define DEBUG 1

// init white list
const uint8_t white_list[2][65] = {
    "c26d99fa3977e96955f8678cfb6c611414bcb4b6d9395166dacd9acf6c2d873e",
    "debd23d6f2a553f2a003d3423d5737e35f6bb897ddad48fac30a3b0b5d210568"
};

void wifi_init(void)
{
    uart_init();
}

/* Get header response from command
** buff:   message to send buffer
** result: data length or error (if < 0)
*/
static int get_header(void* buff)
{
    int     header_len = 9 + strlen(buff);
    uint8_t header_buffer[header_len];
    rtt_printf(0, ">%s", buff);
    uart_send(buff);
    // get header
    uart_receive(header_buffer, header_len);
    header_buffer[header_len - 2] = '\0';  // two last charaters are "\r\n"
#ifdef DEBUG
    rtt_printf(0, "Header: %s\n", header_buffer + header_len - 9);
#endif
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
    data_len += 2;  // data finish by "> " but are not counted in the header
    return data_len;
}

int wifi_command(void* buff)
{
    int data_len = get_header(buff);
    // handle get_header error
    if(data_len < 0)
        return data_len;
    // get data
    uint8_t data_buffer[MAX_DATA_BUFFER_LEN];
    while(data_len > 0) {
        // set data buffer length
        int data_buff_len = data_len;
        if(data_len > MAX_DATA_BUFFER_LEN)
            data_buff_len = MAX_DATA_BUFFER_LEN;
        uart_receive(data_buffer, data_buff_len);
        data_len -= data_buff_len;
        if(data_len == 0)
            data_buffer[data_buff_len - 4] = '\0'; // last characters are "\r\n> "
        rtt_printf(0, ""); // fix strange telnet error
        // print data
        rtt_printf(0, "%s", data_buffer);
    }
    rtt_printf(0, "\n\n");
    return 0;
}

int find_devices(void)
{
    // scan command
    int data_len = get_header("scan\r\n");
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
                //uint8_t * ssid_hash = sha3(ssid);
                rtt_printf(0, "SSID: %s\n", ssid);
                // TODO: check ssid
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

    return 0;
}