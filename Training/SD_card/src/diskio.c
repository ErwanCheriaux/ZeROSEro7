#include "diskio.h"
#include "sd.h"

DSTATUS disk_initialize (unsigned char pdrv)
{
    (void) pdrv;
    return RES_OK;
}

DSTATUS disk_status (unsigned char pdrv)
{
    (void) pdrv;
    return RES_OK;
}

DRESULT disk_read (unsigned char pdrv, unsigned char* buff, unsigned long sector, unsigned int count)
{
    (void) pdrv;
    return sd_read(sector * MMCSD_BLOCK_SIZE, count * MMCSD_BLOCK_SIZE, buff);
}

DRESULT disk_write (unsigned char pdrv, const unsigned char* buff, unsigned long sector, unsigned int count)
{
    (void) pdrv;
    return sd_write(sector * MMCSD_BLOCK_SIZE, count * MMCSD_BLOCK_SIZE, (uint8_t*) buff);
}

DRESULT disk_ioctl (unsigned char pdrv, unsigned char cmd, void* buff)
{
    (void) pdrv;
    (void) cmd;
    (void) buff;
    return RES_OK;
}
