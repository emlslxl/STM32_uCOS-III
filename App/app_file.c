#include "app_file.h"

FATFS fs[1];

void fatfs_init(void)
{
    FRESULT res = 0;
    FIL fd;
    uint8 read_buff[200] = {0};
    UINT cnt = 0;

    res = f_mount(&fs[0], "0:/", 0);
    if (res != FR_OK)
        printf("f_mount failed res = %d\r\n", res);
    else
        printf("f_mount success\r\n");

    res = f_open(&fd, "0:/log.txt", FA_OPEN_EXISTING | FA_READ);
    if (res != FR_OK)
        printf("f_open log.txt failed res = %d\r\n", res);
    else
    {
        printf("f_open log.txt success\r\n");
        f_read(&fd, read_buff, 100, &cnt);
        printf("read byte = %d\r\n", cnt);
        printf("%s\r\n", read_buff);

        res = f_close(&fd);
        if (res != FR_OK)
            printf("f_close failed res = %d\r\n", res);
        else
            printf("f_close success\r\n");
    }

    res = f_open(&fd, "0:/111.txt", FA_CREATE_ALWAYS | FA_READ | FA_WRITE | FA_OPEN_APPEND);
    if (res != FR_OK)
        printf("f_open 111.txt failed res = %d\r\n", res);
    else
    {
        printf("f_open 111.txt success\r\n");
        f_write(&fd, read_buff, 100, &cnt);
        printf("write byte = %d\r\n", cnt);
        res = f_close(&fd);
        if (res != FR_OK)
            printf("f_close failed res = %d\r\n", res);
        else
            printf("f_close success\r\n");
    }

}

