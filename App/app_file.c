#include "app_file.h"

FATFS fs[1];

void fatfs_init(void)
{
    FRESULT res = 0;
    FIL fd;

    res = f_mount(&fs[0], "0:/", 0);
    if (res != FR_OK)
        printf("f_mount failed res = %d\r\n", res);
    else
        printf("f_mount success\r\n");

    res = f_open(&fd, "0:/log.txt", FA_OPEN_EXISTING);
    if (res != FR_OK)
        printf("f_open failed res = %d\r\n", res);
    else
        printf("f_open success\r\n");
}

