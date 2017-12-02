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

void wifi_send(void * buff)
{
    int header_len = 9 + strlen(buff);
    uint8_t header_buffer[header_len];
    rtt_printf(0, ">%s", buff);
    uart_send(buff);
    // get header
#ifdef DEBUG
    rtt_printf(0, "Header len: %d\n", header_len);
#endif
    uart_receive(header_buffer, header_len);
    // extract data length from header
    int data_len = 0;
    for(int i = header_len - 8; i <= header_len - 3; i++)
        data_len = data_len * 10 + header_buffer[i] - '0';
#ifdef DEBUG
    rtt_printf(0, "Data len: %d\n", data_len);
#endif
    // get data
    data_len += 2; // data finish by "\n>" but are not counted in the header
    uint8_t data_buffer[MAX_DATA_BUFFER_LEN];
    while(data_len > 0) {
        int data_buff_len = data_len;
        if(data_len > MAX_DATA_BUFFER_LEN)
            data_buff_len = MAX_DATA_BUFFER_LEN;
        uart_receive(data_buffer, data_buff_len);
        data_buffer[data_buff_len - 3] = '\0'; // two last charaters are "\n>"
        // print data
        rtt_printf(0, "%s", data_buffer);
        data_len -= data_buff_len;
    }
    rtt_printf(0, "\n\n", data_buffer);
}