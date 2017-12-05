//wifi.h

#ifndef WIFI_H
#define WIFI_H

#include "ch.h"
#include "hal.h"

/* This value musn't be between 66 and 72
** Default is 256
*/
#define MAX_DATA_BUFFER_LEN 256

/* Allow Network SSID (SHA3-256)
** Only these ID will be allowed to connect to this device
*/
extern const uint8_t white_list[2][65];

void wifi_init(void);

/* Send data using uart and print response
** buff:   message to send buffer
** return: error code (0 means success)
*/
int wifi_command(void* buff);

/* Find devices using white list
** return: error code (0 means success)
*/
int find_devices(void);

#endif
