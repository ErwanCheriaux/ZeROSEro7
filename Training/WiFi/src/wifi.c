// wifi.c

#include "wifi.h"
#include <string.h>
#include "uart.h"
#include "rtt.h"

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
    strcpy(cmd + 4, filename);
    cmd[file_size + 4] = '\r';
    cmd[file_size + 5] = '\n';
    cmd[file_size + 6] = '\0';
    send_command(cmd, 0);
    int data_len;
    do {
        data_len = get_header(buff_len, &format, MS2ST(FILE_POLL_DELAY / 2));
        if(data_len < 0) // An error occured
            return data_len;
        if(data_len) { // Data length could be null if there was a timeout
            if(data_len == 13) // File already exists because answer is "[Opened: X]"
                answer = get_data(data_len, format, MS2ST(FILE_POLL_DELAY / 2), 0, get_opened_stream);
            else
                get_data(data_len, format, MS2ST(FILE_POLL_DELAY / 2), 0, 0);
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
    if(val == 0)
        in[pos] = '0';
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
    char read_cmd[] = "read 0 XXXX\r\n";
    char remove_cmd[file_size + MAX_FILENAME_EXT + 5];
    remove_cmd[0] = 'f';
    remove_cmd[1] = 'd';
    remove_cmd[2] = 'e';
    remove_cmd[3] = ' ';
    strcpy(remove_cmd + 4, filename);
    remove_cmd[file_size + 4] = '_';
    int_into_str(read_cmd, 7, MAX_DATA_LEN_HTTP);
    // Each FILE_POLL_DELAY ms, check if the file is present
    int timeout = 60;
    for(; ((stream = file_exists(file_block)) == -1) && (timeout > 0); timeout--);
    if(stream < 0)
        return;
    if(timeout == 0) {
        rtt_printf(0, "[ERROR] File cannot be received: Timeout\n");
        return;
    }
    do {
        // read stream
        // TODO: must save file into SD card
        read_cmd[5] = stream + '0';
        wifi_command(read_cmd, 200, 1);
        // remove file
        int cur = int_into_str(remove_cmd, file_size + 5, count);
        remove_cmd[cur++] = '\r';
        remove_cmd[cur++] = '\n';
        remove_cmd[cur] = '\0';
        wifi_command(remove_cmd, 200, 0);
        count++;
        // increment block filename
        cur = int_into_str(file_block, file_size + 1, count);
        file_block[cur] = '\0';
    } while((stream = file_exists(file_block)) != -1);
}

/* Return the number of digits into a number
** val:    input number
** return: number of digits
*/
int int_len(int val)
{
    int count = 1;
    while(val > 9) {
        count++;
        val /= 10;
    }
    return count;
}

void wifi_send_file(char* filename)
{
    int filename_len = strlen(filename);
    // Read file data
    // TODO: A file must be loaded from SD card
    char * data = "This is the message you should be able to read on your smartphone.\n\nPS: this stream do not comes from SD card :/";
    int file_size = strlen(data);
    int file_size_len = int_len(file_size);
    // create a nex file on the wifi chip flash
    char new_file_cmd[filename_len + file_size_len + 11];
    strcpy(new_file_cmd, "fcr -o ");
    strcpy(new_file_cmd + 7, filename);
    new_file_cmd[filename_len + 7] = ' ';
    int cur = int_into_str(new_file_cmd, filename_len + 8, file_size);
    new_file_cmd[cur++] = '\r';
    new_file_cmd[cur++] = '\n';
    new_file_cmd[cur] = '\0';
    rtt_printf(0, "Command to add a new file: %s\n", new_file_cmd);
    //wifi_command(new_file_cmd, 400, 1);
}