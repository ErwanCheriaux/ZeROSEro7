#include "sd.h"
#include <string.h>
#include "rtt.h"

//#define DEBUG 1

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
    int nb_blocks_to_read = (addr % MMCSD_BLOCK_SIZE + len) / MMCSD_BLOCK_SIZE + 1;
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
    memcpy(buffer, buf + addr % MMCSD_BLOCK_SIZE, len);
    return 0;
}

static int write_byte(int addr, int value)
{
    // check if file informations are not override
    if(addr < FILE_AREA) {
        rtt_printf("[ERROR] sd_write_byte: try to override into file area (0 to 0x%08X): 0x%08X\n", FILE_AREA, addr);
        return 1;
    }
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

static int write(int addr, unsigned int len, uint8_t* buffer)
{
    // check if file informations are not override
    if(addr < FILE_AREA) {
        rtt_printf("[ERROR] sd_write: try to override into file area (0 to 0x%08X): 0x%08X\n", FILE_AREA, addr);
        return 1;
    }
    int nb_blocks_to_write = (addr % MMCSD_BLOCK_SIZE + len) / MMCSD_BLOCK_SIZE + 1;
    if (len % MMCSD_BLOCK_SIZE == 0)
        nb_blocks_to_write--;
    if(sdcRead(&SDCD1, addr / MMCSD_BLOCK_SIZE, buf, nb_blocks_to_write)) {
        rtt_printf("[ERROR] sd_write: error: addr:%d, nb_blocks_to_write:%d\n", addr, nb_blocks_to_write);
        return 1;
    }
    memcpy(buf + addr % MMCSD_BLOCK_SIZE, buffer, len);
    if(sdcWrite(&SDCD1, addr / MMCSD_BLOCK_SIZE, buf, nb_blocks_to_write)) {
        rtt_printf("[ERROR] sd_write: error: addr:%d, nb_blocks_to_write:%d\n", addr, nb_blocks_to_write);
        return 1;
    }
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
            rtt_printf("[ERROR] SD: Reading failed\n");
            goto error;
        }
#ifdef DEBUG
        rtt_printf("[INFO] SD: Read 0x%02X at 0x%08X\n", *buffer, addr);
#endif
    }
    else if(cmd == 2) { // write a single value
        if(write_byte(addr, param)) {
            rtt_printf("[ERROR] SD: Writing failed\n");
            goto error;
        }
#ifdef DEBUG
        rtt_printf("[INFO] SD: Write 0x%02X at 0x%08X\n", param, addr);
#endif
    }
    else if(cmd == 3) { // print a whole area
        if(param > (int)SD_BUF_SIZE) {
            rtt_printf("[ERROR] sd_read: too large len: %d / %d\n", param, SD_BUF_SIZE);
            goto error;
        }
        if(read(addr, param, buffer)) {
            rtt_printf("[ERROR] SD: Reading failed\n");
            goto error;
        }
#ifdef DEBUG
        rtt_printf("[INFO] SD: Read 0x%08X bytes from 0x%08X\n", param, addr);
#endif
    }
    else if(cmd == 4) { // write a whole area
        if(param > (int)SD_BUF_SIZE) {
            rtt_printf("[ERROR] sd_write: too large len: %d / %d\n", param, SD_BUF_SIZE);
            goto error;
        }
        if(write(addr, param, buffer)) {
            rtt_printf("[ERROR] SD: Writing failed\n");
            goto error;
        }
#ifdef DEBUG
        rtt_printf("[INFO] SD: Write 0x%08X bytes from 0x%08X\n", param, addr);
#endif
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

int sd_write(int addr, unsigned int len, uint8_t* buffer)
{
    return sdc(4, addr, len, buffer);
}