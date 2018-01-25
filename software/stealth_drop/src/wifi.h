//wifi.h

#ifndef WIFI_H
#define WIFI_H

#include "ch.h"
#include "hal.h"

#define START_SEQ "[007]"
#define MAX_FILENAME_SIZE 255
#define SLEEP_DELAY 10000

void wifi_init(void);

/* Send data using uart and print response
** buff:    message to send buffer
** timeout: WiFi chip waiting time in ms
** return:  error code (0 means success)
*/
int wifi_command(void* buff, int timeout);

/* Move a file from wifi chip flash to stm32 fash
** file must be composed of one or several files named <filename>_<i>,
** with i from 0 to NB_FILES (see MAX_FILENAME_EXT).
** filename: filename to load
*/
void wifi_save_file(char* filename);

/* Send a file on the wifi chip flash
** filename: filename to send
*/
void wifi_send_file(char* filename);

/* If stream mode is activated, this function
** breaks stream mode and swtich to command mode
*/
void wifi_break_stream_mode(void);

/* Send commands to configure tcp server,
** swtich to stream mode and reboot the chip
** This function must be called in command mode
*/
void wifi_configure(void);

/* Wait that a message is received
** msg: message to wait for
*/
void wifi_wait_for(char* msg);

/* Wait for a line ('\n') and save it
** buffer:    buffer where line is saved
** max_len:   max line length
** separator: character at the end of the word
** reutrn:    0 if there is no error, 1 else
*/
int wifi_get_word(char* buffer, int max_len, char separator);

/* Switch wifi chip to sleep mode (indirect)
*/
void wifi_sleep_callback(void);

#endif
