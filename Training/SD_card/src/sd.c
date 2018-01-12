#include "sd.h"

#define SDC_BURST_SIZE 16

int sd_read(int addr, int* value, uint8_t* buf)
{
    if(sdcRead(&SDCD1, addr / MMCSD_BLOCK_SIZE, buf, 1))
        return 1;
    *value = buf[addr % MMCSD_BLOCK_SIZE];
    return 0;
}

int sd_write(int addr, int value, uint8_t* buf)
{
    // clean buffer
    for(unsigned int i = 0; i < MMCSD_BLOCK_SIZE * SDC_BURST_SIZE + 4; i++)
        buf[i] = 0;
    buf[addr % MMCSD_BLOCK_SIZE] = value;
    if(sdcWrite(&SDCD1, addr / MMCSD_BLOCK_SIZE, buf, 1))
        return 1;
    return 0;
}