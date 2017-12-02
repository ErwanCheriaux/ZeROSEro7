//wifi.h

#ifndef WIFI_H
#define WIFI_H

#include "ch.h"
#include "hal.h"

#define MAX_DATA_BUFFER_LEN 256

void wifi_init(void);

/* Send data using uart
** buff: message to send buffer
*/
void wifi_send(void* buff);

#endif
