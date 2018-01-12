#include "sd.h"
#include <string.h>
#include "rtt.h"

uint8_t buf[SD_BUF_SIZE];

int sd_read_byte(int addr, int* value)
{
    if(sdcRead(&SDCD1, addr / MMCSD_BLOCK_SIZE, buf, 1))
        return 1;
    // extract correct value
    *value = buf[addr % MMCSD_BLOCK_SIZE];
    return 0;
}

int sd_read(int addr, int len, uint8_t* buffer)
{
    rtt_printf("sd_read(addr=%d, len=%d, buffer)\n", addr, len);
    int total_nb_blocks_to_read = len / MMCSD_BLOCK_SIZE + 1;
    if (len % MMCSD_BLOCK_SIZE == 0)
        total_nb_blocks_to_read--;
    int nb_blocks_to_read = total_nb_blocks_to_read;
    if(nb_blocks_to_read > SDC_BURST_SIZE)
        nb_blocks_to_read = SDC_BURST_SIZE;
    if(sdcRead(&SDCD1, addr / MMCSD_BLOCK_SIZE, buf, nb_blocks_to_read))
        return 1;
    int bytes_read = nb_blocks_to_read * MMCSD_BLOCK_SIZE - addr % MMCSD_BLOCK_SIZE;
    if(bytes_read > len)
        bytes_read = len;
    rtt_printf("bytes_read: %d\n", bytes_read);
    rtt_printf("memcpy from %d for %d\n", buf + addr % MMCSD_BLOCK_SIZE, bytes_read);
    memcpy(buffer, buf + addr % MMCSD_BLOCK_SIZE, bytes_read);
    len -= bytes_read;
    while(len > 0) {
        rtt_printf("len: %d\n", len);
        buffer += bytes_read;
        total_nb_blocks_to_read -= nb_blocks_to_read;
        if(total_nb_blocks_to_read < SDC_BURST_SIZE)
            nb_blocks_to_read = total_nb_blocks_to_read;
        if(sdcRead(&SDCD1, addr / MMCSD_BLOCK_SIZE, buf, nb_blocks_to_read))
            return 1;
        bytes_read = nb_blocks_to_read * MMCSD_BLOCK_SIZE;
        if(total_nb_blocks_to_read <= SDC_BURST_SIZE) // last turn
            bytes_read -= len % MMCSD_BLOCK_SIZE;
        memcpy(buffer, buf + addr % MMCSD_BLOCK_SIZE, bytes_read);
        len -= bytes_read;
    }
    return 0;
}

int sd_write_byte(int addr, int value)
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