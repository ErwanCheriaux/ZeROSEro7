//wifi.h

#ifndef WIFI_H
#define WIFI_H

#include "ch.h"
#include "hal.h"

/* This value musn't be between 66 and 72
** Default is 256
*/
#define MAX_DATA_BUFFER_LEN 256
#define WHITE_LIST_SIZE     3

/* Allow Network SSID (SHA3-256)
** Only these ID will be allowed to connect to this device
*/
extern const unsigned char white_list[WHITE_LIST_SIZE][65];

void wifi_init(void);

/* Send data using uart and print response
** buff:    message to send buffer
** timeout: WiFi chip waiting time in ms
** return:  error code (0 means success)
*/
int wifi_command(void* buff, int timeout);

#endif
