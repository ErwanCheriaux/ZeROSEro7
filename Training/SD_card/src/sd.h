//sd.h

#ifndef SD_H
#define SD_H

#include "ch.h"
#include "hal.h"

#define SDC_BURST_SIZE 16
/* Buffer for block read/write operations, note that extra bytes are
** allocated in order to support unaligned operations.*/
#define SD_BUF_SIZE    MMCSD_BLOCK_SIZE * SDC_BURST_SIZE + 4

/* Read the content at the given address
** addr:   address to read
** value:  returned content at the given address
** return: 0 for success, 1 for error
*/ 
int sd_read(int addr, int* value);

/* Read whole memory blocks
** block:  block number to start
** nb:     number of blocks to read
** return: 0 for success, 1 for error
*/
#define sd_read_blocks(block, nb) sdcRead(&SDCD1, block, buf, nb)

/* Write a content into a given address
** addr:   address to write the content
** value:  content to write
** return: 0 for success, 1 for error
*/
int sd_write(int addr, int value);


#endif // SD_H