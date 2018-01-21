//sd.h

#ifndef SD_H
#define SD_H

#include "ff.h"

#define DATA_FOLDER        "ZEROSERO"
// MAX_BUFF_LEN must be > MAX_FILENAME_SIZE (see sd.c)
#define MAX_BUFF_LEN       1024

/* Create a file into SD card
** if action = FA_WRITE and if the file already exists, it will be overridden
** filename: file name
** action:   read or write (FA_READ or FA_WRITE)
** return:   0 for success, 1 for error
*/
int sd_file_open(char* filename, int action);

/* Write into the previous opened file
** data_buff will be written
** return: 0 for success, 1 for error
*/
int sd_file_write(void);

/* Close current file
** return: 0 for success, 1 for error
*/
int sd_file_close(void);

/* Remove file from SD card
** filename: file name to remove
** return:   0 for success, 1 for error
*/
int sd_file_remove(char* filename);

/* Read opened file
** file will be closed at EOF
** data will be written into data_buff
** bytes_read: number of bytes read
** return:     0 for success, 1 for error
*/
int sd_file_read(unsigned int* bytes_read);

/* Get all filenames
** filenames pointers will be written into filenames
** return:    0 for success, 1 for error
*/
int sd_get_next_filename(void);

#endif // SD_H