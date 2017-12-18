#include "app_file.h"

FATFS fs[1];

FRESULT fatfs_init(void)
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

    return res;

    //test read function
    // res = f_open(&fd, "0:/log.txt", FA_OPEN_EXISTING | FA_READ);
    // if (res != FR_OK)
    //     printf("f_open log.txt failed res = %d\r\n", res);
    // else
    // {
    //     printf("f_open log.txt success\r\n");
    //     f_read(&fd, read_buff, 100, &cnt);
    //     printf("read byte = %d\r\n", cnt);
    //     printf("%s\r\n", read_buff);

    //     res = f_close(&fd);
    //     if (res != FR_OK)
    //         printf("f_close failed res = %d\r\n", res);
    //     else
    //         printf("f_close success\r\n");
    // }

    //test write function
    // res = f_open(&fd, "0:/111.txt", FA_CREATE_ALWAYS | FA_READ | FA_WRITE | FA_OPEN_APPEND);
    // if (res != FR_OK)
    //     printf("f_open 111.txt failed res = %d\r\n", res);
    // else
    // {
    //     printf("f_open 111.txt success\r\n");
    //     f_write(&fd, read_buff, 100, &cnt);
    //     printf("write byte = %d\r\n", cnt);
    //     res = f_close(&fd);
    //     if (res != FR_OK)
    //         printf("f_close failed res = %d\r\n", res);
    //     else
    //         printf("f_close success\r\n");
    // }

}

FRESULT fota_file_handle(void)
{
    FIL fd;
    FRESULT res = 0;
    UINT cnt = 0;
    uint32 i = 0;
    uint32 total_read_byte = 0;
    uint32 left_read_byte = 0;
    uint8 read_buff[1024] = {0};

    res = f_open(&fd, "0:/fota.txt", FA_OPEN_EXISTING | FA_READ);
    if (res != FR_OK)
    {
        printf("f_open fota.txt failed res = %d\r\n", res);
        iap_write_flag_N();
        return FR_DISK_ERR;
    }
    else
    {
        printf("f_open fota.txt success\r\n");

        //校验数据是否正确
        for (left_read_byte = 119612, total_read_byte = 0; left_read_byte != 0;)
        {
            rt_iwdg_feed();

            f_lseek(&fd, total_read_byte);

            if (left_read_byte >= 1024)
            {
                f_read(&fd, read_buff, 1024, &cnt);

                //计算校验值
                //to do
            }
            else
            {
                f_read(&fd, read_buff, left_read_byte, &cnt);

                //计算校验值
                //to do
            }

            total_read_byte += cnt;
            left_read_byte -= cnt;

            // printf("total_read_byte = %d, left_read_byte = %d\r\n",
            //        total_read_byte, left_read_byte);
        }

        //如果升级数据正确 擦除APP1
        if (total_read_byte == 119612)
        {
            iap_erase_app1();
        }
        else
        {
            iap_write_flag_N();
            return FR_DISK_ERR;
        }

        //写数据到APP1
        for (left_read_byte = 119612, total_read_byte = 0; left_read_byte != 0;)
        {
            rt_iwdg_feed();

            f_lseek(&fd, total_read_byte);

            if (left_read_byte >= 1024)
            {
                f_read(&fd, read_buff, 1024, &cnt);

                //写入flash
                write_data_to_app1(read_buff, 1024);
            }
            else
            {
                f_read(&fd, read_buff, left_read_byte, &cnt);

                //写入flash
                write_data_to_app1(read_buff, left_read_byte);
            }

            //打印读取到的数据
            // for (i = 0; i < cnt; i++)
            //     printf("%02x ", read_buff[i]);
            // printf("\r\n");

            total_read_byte += cnt;
            left_read_byte -= cnt;

            printf("total_read_byte = %d, left_read_byte = %d\r\n",
                   total_read_byte, left_read_byte);
        }

        f_close(&fd);

        if (total_read_byte == 119612)  //成功条件
        {
            iap_write_flag_N();
            return FR_OK;
        }
        else
        {
            iap_write_flag_R();
            return FR_DISK_ERR;
        }
    }
}

