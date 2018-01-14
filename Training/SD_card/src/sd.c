#include "sd.h"
#include <string.h>
#include "rtt.h"

uint8_t buf[SD_BUF_SIZE];

#define SD_INIT() \
    if (!blkIsInserted(&SDCD1)) {\
        rtt_printf("[ERROR] SD: Card not inserted, aborting.\n");\
        return 1;\
    }\
    if (sdcConnect(&SDCD1)) {\
        rtt_printf("[ERROR] SD: Connection failed, aborting.\n");\
        return 1;\
    }

static int read_byte(int addr, uint8_t* value)
{
    if(sdcRead(&SDCD1, addr / MMCSD_BLOCK_SIZE, buf, 1))
        return 1;
    // extract correct value
    *value = buf[addr % MMCSD_BLOCK_SIZE];
    return 0;
}

static int read(int addr, unsigned int len, uint8_t* buffer)
{
    int nb_blocks_to_read = len / MMCSD_BLOCK_SIZE + 1;
    if (len % MMCSD_BLOCK_SIZE == 0)
        nb_blocks_to_read--;
    if(nb_blocks_to_read > SDC_BURST_SIZE) {
        rtt_printf("[ERROR] sd_read: burst size too big: %d / %d\n", nb_blocks_to_read, SDC_BURST_SIZE);
        return 1;
    }
    if(sdcRead(&SDCD1, addr / MMCSD_BLOCK_SIZE, buf, nb_blocks_to_read)) {
        rtt_printf("[ERROR] sd_read: error: addr:%d, nb_blocks_to_read:%d\n", addr, nb_blocks_to_read);
        return 1;
    }
    unsigned int bytes_read = nb_blocks_to_read * MMCSD_BLOCK_SIZE - addr % MMCSD_BLOCK_SIZE;
    if(bytes_read > len)
        bytes_read = len;
    memcpy(buffer, buf + addr % MMCSD_BLOCK_SIZE, bytes_read);
    return 0;
}

static int write_byte(int addr, int value)
{
    // copy memory block
    if(sdcRead(&SDCD1, addr / MMCSD_BLOCK_SIZE, buf, 1))
        return 1;
    // change a single byte
    buf[addr % MMCSD_BLOCK_SIZE] = value;
    // write block in memory
    if(sdcWrite(&SDCD1, addr / MMCSD_BLOCK_SIZE, buf, 1))
        return 1;
    return 0;
}

/* General function to deal with SD card
** sdc (   cmd,      addr, param, buffer)
** cmd:    1: read,  addr, ,      buffer
**         2: write, addr, value
**         3: read,  addr, size,  buffer
** return: 0 for success, 1 for error
*/
static int sdc(int cmd, int addr, int param, uint8_t* buffer)
{
    SD_INIT()
    int result = 1;
    if(cmd == 1){ // print a single value
        if(read_byte(addr, buffer)) {
            rtt_printf("Reading failed\n");
            goto error;
        }
    }
    else if(cmd == 2) { // write a single value
        rtt_printf("Write %d at position %d\n", param, addr);
        if(write_byte(addr, param)) {
            rtt_printf("Writing failed\n");
            goto error;
        }
    }
    else if(cmd == 3) { // print a whole area
        rtt_printf("Read a big memory area:\n");
        if(param > (int)SD_BUF_SIZE) {
            rtt_printf("[ERROR] sd_read: too large len: %d / %d\n", param, SD_BUF_SIZE);
            goto error;
        }
        if(read(addr, param, buffer)) {
            rtt_printf("Reading failed\n");
            goto error;
        }
    }
    result = 0;
error:
    sdcDisconnect(&SDCD1);
    return result;
}

int sd_read_byte(int addr, uint8_t* value)
{
    return sdc(1, addr, 0, value);
}

int sd_write_byte(int addr, uint8_t value)
{
    return sdc(2, addr, value, 0);
}

int sd_read(int addr, unsigned int len, uint8_t* buffer)
{
    return sdc(3, addr, len, buffer);
}