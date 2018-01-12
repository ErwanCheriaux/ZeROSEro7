//sd.h

#ifndef SD_H
#define SD_H

#include "ch.h"
#include "hal.h"

/* Read the content at the given address
** addr:   address to read
** value:  returned content at the given address
** return: 0 for success, 1 for error
*/ 
int sd_read(int addr, int* value, uint8_t* buf);

/* Write a content into a given address
** addr:   address to write the content
** value:  content to write
** return: 0 for success, 1 for error
*/
int sd_write(int addr, int value, uint8_t* buf);

#endif // SD_H