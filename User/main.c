
#include "global.h"


int main(void)
{
    OS_ERR err;
    u16 value;

    /* �弶��ʼ�� */
    BSP_Init();

    //function_test();

    /* ��ʼ��"uC/OS-III"�ں� */
    OSInit(&err);
    printf("OS Start!!\r\n");

    RTOS_CreateMem();

    RTOS_CreateTask();


    //RDA5807_FM_Test();



    /* ����������ϵͳ������Ȩ����uC/OS-II */
    OSStart(&err);

}

/*********************************************END OF FILE**********************/

