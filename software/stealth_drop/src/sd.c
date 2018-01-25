#include "sd.h"
#include <string.h>
#include "ch.h"
#include "hal.h"
#include "rtt.h"

#define PATH_LEN           (MAX_FILENAME_SIZE + sizeof(DATA_FOLDER) + 1)
#define MAX_FILENAME_SIZE  255

//#define DEBUG 1

char data_buff[MAX_BUFF_LEN + 1];

// Is SD Card mounted
static int mounted = 0;
// Is folder opened
static int dir_open = 0;
static char path[PATH_LEN];

static FATFS fatfs;
static FIL file;

static DIR directory;
static FILINFO fileinfos;

static int f_print_error(int error)
{
    if(error == FR_OK)
        return error;
    switch(error) {
        case FR_DISK_ERR:            rtt_printf("[ERROR] FR_DISK_ERR\n"); break;
        case FR_INT_ERR:             rtt_printf("[ERROR] FR_INT_ERR\n"); break;
        case FR_NOT_READY:           rtt_printf("[ERROR] FR_NOT_READY\n"); break;
        case FR_NO_FILE:             rtt_printf("[ERROR] FR_NO_FILE\n"); break;
        case FR_NO_PATH:             rtt_printf("[ERROR] FR_NO_PATH\n"); break;
        case FR_INVALID_NAME:        rtt_printf("[ERROR] FR_INVALID_NAME\n"); break;
        case FR_DENIED:              rtt_printf("[ERROR] FR_DENIED\n"); break;
        case FR_EXIST:               rtt_printf("[ERROR] FR_EXIST\n"); break;
        case FR_INVALID_OBJECT:      rtt_printf("[ERROR] FR_INVALID_OBJECT\n"); break;
        case FR_WRITE_PROTECTED:     rtt_printf("[ERROR] FR_WRITE_PROTECTED\n"); break;
        case FR_INVALID_DRIVE:       rtt_printf("[ERROR] FR_INVALID_DRIVE\n"); break;
        case FR_NOT_ENABLED:         rtt_printf("[ERROR] FR_NOT_ENABLED\n"); break;
        case FR_NO_FILESYSTEM:       rtt_printf("[ERROR] FR_NO_FILESYSTEM\n"); break;
        case FR_MKFS_ABORTED:        rtt_printf("[ERROR] FR_MKFS_ABORTED\n"); break;
        case FR_TIMEOUT:             rtt_printf("[ERROR] FR_TIMEOUT\n"); break;
        case FR_LOCKED:              rtt_printf("[ERROR] FR_LOCKED\n"); break;
        case FR_NOT_ENOUGH_CORE:     rtt_printf("[ERROR] FR_NOT_ENOUGH_CORE\n"); break;
        case FR_TOO_MANY_OPEN_FILES: rtt_printf("[ERROR] FR_TOO_MANY_OPEN_FILES\n"); break;
        case FR_INVALID_PARAMETER:   rtt_printf("[ERROR] FR_INVALID_PARAMETER\n"); break;
        default:                     rtt_printf("[ERROR] UNEXPECTED ERROR\n");
    }
    return error;
}

static int mount(void)
{
    // connect
    if(sdcConnect(&SDCD1)) {
        rtt_printf("[ERROR] sdcConnect ERROR\n");
        return 1;
    }
    // mount when it will be needed
    if(f_print_error(f_mount(&fatfs, "/", 1)))
        return 1;
    rtt_printf("> Mount SUCCESS\n");
    mounted = 1;
    // pre-set path by beginning with "DATA_FOLDER/"
    strcpy(path, DATA_FOLDER);
    path[sizeof(DATA_FOLDER) - 1] = '/';
    return 0;
}

static int unmount(void)
{
    if(sdcDisconnect(&SDCD1)) {
        rtt_printf("[ERROR] sdcDisconnect ERROR\n");
        return 1;
    }
    rtt_printf("> Unmount SUCCESS\n");
    mounted = 0;
    return 0;
}

int sd_file_open(char* filename, int action)
{
    // mount
    if (mounted) {
        rtt_printf("[ERROR] SD Card should not be mounted\n");
        unmount();
        return 1;
    }
    if(mount())
        return 1;
    // check filename length
    int filename_len = strlen(filename);
    if(filename_len > MAX_FILENAME_SIZE) {
        rtt_printf("[ERROR] Opening file: Too long filename (%d > %d)\n", filename_len, MAX_FILENAME_SIZE);
        unmount();
        return 1;
    }
    // set filename
    strcpy(path + sizeof(DATA_FOLDER), filename);
#ifdef DEBUG
    rtt_printf("Open path: %s\n", path);
#endif
    // open file
    if(action == FA_WRITE)
        action |= FA_CREATE_ALWAYS;
    if(f_print_error(f_open(&file, path, action))) {
        unmount();
        return 1;
    }
    rtt_printf("> File open SUCCESS\n");
    return 0;
}

int sd_file_write(unsigned int size)
{
    unsigned int data_written = 0;
    if(!mounted) {
        rtt_printf("[ERROR] SD Card is not mounted\n");
        return 1;
    }
    if(f_print_error(f_write(&file, data_buff, size, &data_written))) {
        unmount();
        return 1;
    }
    if(data_written != size) {
        rtt_printf("[ERROR] %d bytes were written but %d must be writted\n", data_written, size);
        unmount();
        return 1;
    }
#ifdef DEBUG
    rtt_printf("> File write SUCCESS\n");
#endif
    return 0;
}


int sd_file_close(void)
{
    if(!mounted) {
        rtt_printf("[ERROR] SD Card is not mounted\n");
        return 1;
    }
    if(!f_print_error(f_close(&file)))
        rtt_printf("> File close SUCCESS\n");
    return unmount();
}

int sd_file_remove(char* filename)
{
    // mount
    if (mounted) {
        rtt_printf("[ERROR] SD Card should not be mounted\n");
        unmount();
        return 1;
    }
    if(mount())
        return 1;
    // set filename
    strcpy(path + sizeof(DATA_FOLDER), filename);
    // remove file
    if(f_print_error(f_unlink(path))) {
        unmount();
        return 1;
    }
    rtt_printf("> File removing SUCCESS\n");
    return unmount();
}

int sd_file_read(unsigned int* bytes_read)
{
    if(!mounted) {
        rtt_printf("[ERROR] SD Card is not mounted\n");
        return 1;
    }
    if(f_print_error(f_read(&file, data_buff, MAX_BUFF_LEN, bytes_read))) {
        unmount();
        return 1;
    }
    data_buff[*bytes_read] = '\0';
    return 0;
}

int sd_get_next_filename(void)
{
    if(!dir_open) {
        // mount
        if (mounted) {
            rtt_printf("[ERROR] SD Card should not be mounted\n");
            unmount();
            return 1;
        }
        if(mount())
            return 1;
        // open folder
        if(f_print_error(f_opendir(&directory, DATA_FOLDER))) {
            unmount();
            return 1;
        }
        dir_open = 1;
    }
    while(1) {
        if(f_print_error(f_readdir(&directory, &fileinfos)))
            break;
        if(fileinfos.fname[0] == 0) { // end of files
            dir_open = 0;
            if(!f_print_error(f_closedir(&directory)))
                rtt_printf("> File list SUCCESS\n");
            unmount();
            return 2;
        }
        if(fileinfos.fattrib & AM_DIR) // it's a folder
            continue;
        // copy filename into data_buff
        strcpy(data_buff, fileinfos.fname);
        return 0;
    }
    // close folder
    dir_open = 0;
    f_print_error(f_closedir(&directory));
    unmount();
    return 1;
}