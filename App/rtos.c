#include "rtos.h"

OS_TCB  BootHandler_TCB;
static  CPU_STK BootHandler_Stk[TASK_BootHandler_STK_SIZE];


void RTOS_CreateTask()
{
    OS_ERR err;
    /*创建外部中断处理任务  */
    OSTaskCreate((OS_TCB *)&BootHandler_TCB,                                   /* 任务控制块指针 */
                 (CPU_CHAR *)"BootHandler",                                      /* 任务名称  */
                 (OS_TASK_PTR)Task_BootHandler,                              /* 任务代码指针  */
                 (void *)0,                                               // 传递给任务的参数parg
                 (OS_PRIO)TASK_BootHandler_PRIO,                             // 任务优先级
                 (CPU_STK *)&BootHandler_Stk[0],                           // 任务堆栈基地址
                 (CPU_STK_SIZE)TASK_BootHandler_STK_SIZE / 10,         // 堆栈剩余警戒线
                 (CPU_STK_SIZE)TASK_BootHandler_STK_SIZE,                  // 堆栈大小
                 (OS_MSG_QTY)10,                                              // 可接收的最大消息队列数
                 (OS_TICK)0,                                              // 时间片轮转时间
                 (void *)0,                                               // 任务控制块扩展信息
                 (OS_OPT)(OS_OPT_TASK_STK_CHK |
                          OS_OPT_TASK_STK_CLR),         // 任务选项
                 (OS_ERR *)&err);                                       // 返回值
}




















