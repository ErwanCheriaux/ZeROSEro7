#include "sd.h"
#include <string.h>
#include "rtt.h"

//#define DEBUG 1

uint8_t buf[SD_BUF_SIZE];

int sd_read_byte(int addr, uint8_t* value)
{
    if(sdcRead(&SDCD1, addr / MMCSD_BLOCK_SIZE, buf, 1))
        return 1;
    // extract correct value
    *value = buf[addr % MMCSD_BLOCK_SIZE];
    return 0;
}

int sd_read(int addr, unsigned int len, uint8_t* buffer)
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

int sd_write_byte(int addr, uint8_t value)
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

int sd_write(int addr, unsigned int len, uint8_t* buffer)
{
    int nb_blocks_to_write = (addr % MMCSD_BLOCK_SIZE + len) / MMCSD_BLOCK_SIZE + 1;
    if (len % MMCSD_BLOCK_SIZE == 0)
        nb_blocks_to_write--;
    if(nb_blocks_to_write > SDC_BURST_SIZE) {
        rtt_printf("[ERROR] sd_write: burst size too big: %d / %d\n", nb_blocks_to_write, SDC_BURST_SIZE);
        return 1;
    }
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