// wifi.c

#include "wifi.h"
#include <string.h>
#include "uart.h"
#include "rtt.h"

// init white list
const char white_list[2][65] = {
    "c26d99fa3977e96955f8678cfb6c611414bcb4b6d9395166dacd9acf6c2d873e",
    "debd23d6f2a553f2a003d3423d5737e35f6bb897ddad48fac30a3b0b5d210568"
};

void wifi_init(void)
{
    uart_init();
}

int wifi_send(void* buff)
{
    int     header_len = 9 + strlen(buff);
    uint8_t header_buffer[header_len];
    rtt_printf(0, ">%s", buff);
    uart_send(buff);
    // get header
#ifdef DEBUG
    rtt_printf(0, "Header len: %d\n", header_len);
#endif
    uart_receive(header_buffer, header_len);
    // check error code
    int err_code = header_buffer[header_len - 9] - '0';
    if(err_code != 0) {
        rtt_printf(0, "[ERROR] An error occured after sending this command: \"%s\":\n", buff);
        rtt_printf(0, "[ERROR] Error code:%d (", err_code);
        switch(err_code) {
            case 1: rtt_printf(0, "Command failed)\n");
            case 2: rtt_printf(0, "Parse error)\n");
            case 3: rtt_printf(0, "Unknown command)\n");
            case 4: rtt_printf(0, "Too few args)\n");
            case 5: rtt_printf(0, "Too many args)\n");
            case 6: rtt_printf(0, "Unknown variable or option)\n");
            case 7: rtt_printf(0, "Invalid argument)\n");
            case 8: rtt_printf(0, "Serial command line buffer overflow)\n");
            case 9: rtt_printf(0, "Bounds error, command specific, bounds of the command were exceeded)\n");
        }
        return err_code;
    }
    // extract data length from header
    int data_len = 0;
    for(int i = header_len - 8; i <= header_len - 3; i++)
        data_len = data_len * 10 + header_buffer[i] - '0';
#ifdef DEBUG
    rtt_printf(0, "Data len: %d\n", data_len);
#endif
    // get data
    data_len += 2;  // data finish by "\n>" but are not counted in the header
    uint8_t data_buffer[MAX_DATA_BUFFER_LEN];
    while(data_len > 0) {
        int data_buff_len = data_len;
        if(data_len > MAX_DATA_BUFFER_LEN)
            data_buff_len = MAX_DATA_BUFFER_LEN;
        uart_receive(data_buffer, data_buff_len);
        data_buffer[data_buff_len - 3] = '\0';  // two last charaters are "\n>"
        // print data
        rtt_printf(0, "%s", data_buffer);
        data_len -= data_buff_len;
    }
    rtt_printf(0, "\n\n", data_buffer);
    return 0;
}