
#include "global.h"


int main(void)
{
    OS_ERR err;
    u16 value;

    /* 板级初始化 */
    BSP_Init();

    //function_test();

    /* 初始化"uC/OS-III"内核 */
    OSInit(&err);
    printf("OS Start!!\r\n");

    RTOS_CreateMem();

    RTOS_CreateTask();


    //RDA5807_FM_Test();



    /* 启动多任务系统，控制权交给uC/OS-II */
    OSStart(&err);

}

/*********************************************END OF FILE**********************/

