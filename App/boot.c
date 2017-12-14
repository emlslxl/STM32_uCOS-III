#include "boot.h"
#include "app_iap.h"
#include "ff.h"
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

void Task_BootHandler(void *p_arg)
{
    uint8_t flag = 0;
    OS_ERR err;
    (void)p_arg;

    flag = ReadUpdateFlag();
    printf("\r\rupdate flag = %c\r\n", flag);

    fatfs_init();

    if (flag == 'R')
    {
        iap_erase_app1();

        if (copy_app2_to_app1() != 0)
        {
            printf("copy_app2_to_app1 failed\r\n");
        }
        else
        {
            printf("copy_app2_to_app1 success\r\n");
            iap_write_flag_N();
        }
    }

    printf("jump to app1\r\n");
    printf("\r\n");
    printf("\r\n");
    boot_jump_to_app1();

    while (1)
    {
        rt_led_blue_on();
        OSTimeDly(100, OS_OPT_TIME_DLY, &err);
        rt_led_blue_off();
        OSTimeDly(100, OS_OPT_TIME_DLY, &err);
    }
}

/* -------------------------------------end of file------------------------------------ */
