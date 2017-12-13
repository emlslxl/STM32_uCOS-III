
#include "global.h"


int main(void)
{
    OS_ERR err;

    BSP_Init();

    /* 初始化"uC/OS-III"内核 */
    OSInit(&err);
    printf("OS Start!!\r\n");

    RTOS_CreateTask();

    /* 启动多任务系统，控制权交给uC/OS-II */
    OSStart(&err);
}

/*********************************************END OF FILE**********************/

