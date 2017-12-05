//wifi.h

#ifndef WIFI_H
#define WIFI_H

#include "ch.h"
#include "hal.h"

#define MAX_DATA_BUFFER_LEN 256

/* Allow Network SSID (SHA3-256)
** Only these ID will be allowed to connect to this device
*/
extern const char white_list[2][65];

void wifi_init(void);

/* Send data using uart
** buff:   message to send buffer
** return: error code (0 means success)
*/
int wifi_send(void* buff);

#endif
