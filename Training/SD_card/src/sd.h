//sd.h

#ifndef SD_H
#define SD_H

#include "ch.h"
#include "hal.h"

#define SDC_BURST_SIZE 16
/* Buffer for block read/write operations, note that extra bytes are
** allocated in order to support unaligned operations.*/
#define SD_BUF_SIZE    MMCSD_BLOCK_SIZE * SDC_BURST_SIZE

/* Read the content at the given address
** addr:   address to read
** value:  returned content at the given address
** return: 0 for success, 1 for error
*/
int sd_read_byte(int addr, int* value);

/* Read whole memory area
** addr:   address to read
** len:    number of bytes to read
** buffer: returned content 
** return: 0 for success, 1 for error
*/
int sd_read(int addr, unsigned int len, uint8_t* buffer);

/* Write a byte into a given address
** addr:   address to write the byte
** value:  content to write
** return: 0 for success, 1 for error
*/
int sd_write_byte(int addr, int value);


#endif // SD_H