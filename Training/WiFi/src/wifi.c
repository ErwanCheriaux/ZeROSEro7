// wifi.c

#include "wifi.h"
#include <string.h>
#include "uart.h"
#include "rtt.h"

//#define DEBUG 1

void wifi_init(void)
{
    uart_init();
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
** timeout:  time to wait for WiFi chip response
** handler:  function called with data buffer in parameter
** display:  will data be printed?
** return:   value returned by the handler 
*/
static int get_data(int data_len, char format, int timeout, int display, int (*handler)(uint8_t*))
{
    uint8_t data_buffer[MAX_DATA_BUFFER_LEN + 1];
    int answer = 0;
    while(data_len > 0) {
        // set data buffer length
        int data_buff_len = data_len;
        if(data_len > MAX_DATA_BUFFER_LEN)
            data_buff_len = MAX_DATA_BUFFER_LEN;
        if(uart_receive_timeout(data_buffer, data_buff_len, timeout))
            break;
        data_buffer[data_buff_len] = '\0';
        data_len -= data_buff_len;
        if(data_len == 0){
            data_buffer[data_buff_len - 2] = '\0'; // last characters are "\r\n"
            if(format == 'R')
                data_buffer[data_buff_len - 4] = '\0'; // last characters are "\r\n> "
        }
        // print data
        if(display)
            rtt_printf(0, "%s", data_buffer);
        if(handler)
            answer = handler(data_buffer);
    }
    if(display)
        rtt_printf(0, "\n");
    return answer;
}

int wifi_command(void* buff, int timeout, int display)
{
    char format;
    int buff_len = strlen(buff);
    send_command(buff, display);
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
            get_data(data_len, format, timeout, display, 0);
        buff_len = 0;
    } while(data_len != 0); // Until there is a response of the WiFi chip
    return 0;
}

static int get_opened_stream(uint8_t* buffer)
{
    return(buffer[9] - '0');
}


/* Check if a file exists into wifi chip flash
** Also open the file if it exists
** filename: file to test
** return:   if the file exists, id of the stream opened, or -1
*/
static int file_exists(char* filename)
{
    char format;
    int answer = -1;
    int file_size = strlen(filename);
    int buff_len = file_size + 6;
    // Send command to open a file
    char cmd[buff_len + 1];
    cmd[0] = 'f';
    cmd[1] = 'o';
    cmd[2] = 'p';
    cmd[3] = ' ';
    for(int i = 0; i < file_size; i++)
        cmd[4 + i] = filename[i];
    cmd[file_size + 4] = '\r';
    cmd[file_size + 5] = '\n';
    cmd[file_size + 6] = '\0';
    send_command(cmd, 0);
    int data_len;
    do {
        data_len = get_header(buff_len, &format, MS2ST(500));
        if(data_len < 0) // An error occured
            return data_len;
        if(data_len) { // Data length could be null if there was a timeout
            if(data_len == 13) // File already exists because answer is "[Opened: X]"
                answer = get_data(data_len, format, MS2ST(500), 0, get_opened_stream);
            else
                get_data(data_len, format, MS2ST(500), 0, 0);
        }
        buff_len = 0;
    } while(data_len != 0); // Until there is a response of the WiFi chip
#ifdef DEBUG
    if(answer != -1)
        rtt_printf(0, "%s exists\n", filename);
    else
        rtt_printf(0, "%s doesn't exists\n", filename);
#endif
    return answer;
}

/* Add a number into a string
** in:     input string
** pos:    position where number must begin
** val:    value to insert
** return: return position after the last figure into the string
*/
static int int_into_str(char* in, int pos, int val)
{
    int val_cpy = val;
    while(val > 9) {
        pos++;
        val /= 10;
    }
    int last_pos = pos;
    while(val_cpy > 0) {
        in[pos--] = val_cpy % 10 + '0';
        val_cpy /= 10;
    }
    return last_pos + 1;
}

void wifi_save_file(char* filename)
{
    int file_size = strlen(filename);
    char file_block[file_size + MAX_FILENAME_EXT + 1];
    strcpy(file_block, filename);
    file_block[file_size] = '_';
    file_block[file_size + 1] = '0';
    file_block[file_size + 2] = '\0';
    int count = 0;
    int stream;
    char close_cmd[] = "close 0\r\n";
    char read_cmd[] = "read 0 XXXX\r\n";
    int_into_str(read_cmd, 7, MAX_DATA_LEN_HTTP);
    while((stream = file_exists(file_block)) != -1) {
        // read stream
        read_cmd[5] = stream + '0';
        wifi_command(read_cmd, 1000, 1);
        // close opened stream
        close_cmd[6] = stream + '0';
        wifi_command(close_cmd, 500, 0);
        // increment block filename
        int cur = int_into_str(file_block, file_size + 1, count++);
        file_block[cur] = '\0';
    }
}