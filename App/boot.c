#include "boot.h"
#include "app_iap.h"
#include "app_file.h"


void Task_BootHandler(void *p_arg)
{
    OS_ERR err;
    (void)p_arg;

    read_upgrade_info();

    if (iap_flash.upgrade_flag == 'R')
    {
        iap_init();

#ifdef IAP_USE_APP2
        //app1 app2 升级方案
        {
            fota_app2_handle();
        }
#else
        //文件系统升级方案
        {
            if (fatfs_init() == FR_OK)
            {
                fota_file_handle();
            }
        }
#endif
    }

    printf("jump to app1\r\n");
    printf("\r\n");
    printf("\r\n");
    ENTER_CRITICAL_SECTION();
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
