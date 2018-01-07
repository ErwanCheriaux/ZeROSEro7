//wifi.h

#ifndef WIFI_H
#define WIFI_H

#include "ch.h"
#include "hal.h"

// This value musn't be between 66 and 72
// Default is 256
#define MAX_DATA_BUFFER_LEN 256
#define WHITE_LIST_SIZE     3
// Number of byte of each part of a file stored into wifi chip flash 
// /!\ This value must be same as variable defined into DataTransfert.java
#define MAX_DATA_LEN_HTTP 2560
// NB_FILES = pow(MAX_FILENAME_EXT - 1, 10) - 1 files can be read from wifi chip flash
// its means that a file with a maximum size of MAX_DATA_LEN_HTTP * NB_FILES could be exchanged 
#define MAX_FILENAME_EXT 5
// Delay between two poll (does file exists)
// Warning, if this dekay is too short, some errors will appear
// 3000 ms is a good compromise
#define FILE_POLL_DELAY  3000

/* Allow Network SSID (SHA3-256)
** Only these ID will be allowed to connect to this device
*/
extern const unsigned char white_list[WHITE_LIST_SIZE][65];

void wifi_init(void);

/* Send data using uart and print response
** buff:    message to send buffer
** timeout: WiFi chip waiting time in ms
** display: will command and answer be displayed? 
** return:  error code (0 means success)
*/
int wifi_command(void* buff, int timeout, int display);

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

#endif
