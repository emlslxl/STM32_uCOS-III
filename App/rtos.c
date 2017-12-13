#include "rtos.h"

/******************for task***********************/
OS_TCB  TIM2Hisr_TCB;
static  CPU_STK TIM2Hisr_Stk[TASK_TIM2Hisr_STK_SIZE];

OS_TCB  BootHandler_TCB;
static  CPU_STK BootHandler_Stk[TASK_BootHandler_STK_SIZE];


/******************for memroy***********************/
uint8 mem_part1[MemBlkNum_1][MemBlkSize_1];
uint8 mem_part2[MemBlkNum_2][MemBlkSize_2];
uint8 mem_part3[MemBlkNum_3][MemBlkSize_3];

OS_MEM MEM1_MCB;
OS_MEM MEM2_MCB;
OS_MEM MEM3_MCB;


void ErrorNum(char *file, u32 line, u32 err)
{
    printf("error[%d]: file[%s],line[%d]\r\n", err, file, line);

}


void *AllocMem(uint32 size)
{
    OS_ERR err;
    void *p;

    if (size < MemBlkSize_1)
    {
        p = OSMemGet(&MEM1_MCB, &err);
        if (err != OS_ERR_NONE)
        {
            ErrorNum(__FILE__, __LINE__, err);
        }
        return p;
    }
    else if ((size >= MemBlkSize_1) && (size < MemBlkSize_2))
    {
        p = OSMemGet(&MEM2_MCB, &err);
        if (err != OS_ERR_NONE)
        {
            ErrorNum(__FILE__, __LINE__, err);
        }
        return p;
    }
    else if ((size >= MemBlkSize_2) && (size < MemBlkSize_3))
    {
        p = OSMemGet(&MEM3_MCB, &err);
        if (err != OS_ERR_NONE)
        {
            ErrorNum(__FILE__, __LINE__, err);
        }
        return p;
    }
    if (size > MemBlkSize_3)
    {
        err = OS_ERR_MEM_INVALID_SIZE;
        ErrorNum(__FILE__, __LINE__, err);
        return (void *)0;
    }

}

void FreeMem(void *pAddr)
{
    OS_ERR err;
    uint8 *memPart1Start, *memPart2Start, *memPart3Start;
    uint8 *memPart1End, *memPart2End, *memPart3End;
    uint8 *p;

    p = (uint8 *)pAddr;

    memPart1Start = (uint8 *)&mem_part1[0][0];
    memPart1End = memPart1Start + MemBlkNum_1 * MemBlkSize_1;

    memPart2Start = (uint8 *)&mem_part2[0][0];
    memPart2End = memPart2Start + MemBlkNum_2 * MemBlkSize_2;

    memPart3Start = (uint8 *)&mem_part3[0][0];
    memPart3End = memPart3Start + MemBlkNum_3 * MemBlkSize_3;

    if ((p < memPart1End) && (p >= memPart1Start))
    {
        OSMemPut(&MEM1_MCB, p, &err);
        if (err != OS_ERR_NONE)
        {
            ErrorNum(__FILE__, __LINE__, err);
        }
    }
    else if ((p < memPart2End) && (p >= memPart2Start))
    {
        OSMemPut(&MEM2_MCB, p, &err);
        if (err != OS_ERR_NONE)
        {
            ErrorNum(__FILE__, __LINE__, err);
        }
    }
    else if ((p < memPart3End) && (p >= memPart3Start))
    {
        OSMemPut(&MEM3_MCB, p, &err);
        if (err != OS_ERR_NONE)
        {
            ErrorNum(__FILE__, __LINE__, err);
        }
    }
    else
    {
        err = OS_ERR_MEM_INVALID_P_MEM;
        if (err != OS_ERR_NONE)
        {
            ErrorNum(__FILE__, __LINE__, err);
        }
    }

}




void RTOS_CreateMem()
{
    OS_ERR err;
    OSMemCreate(&MEM1_MCB, "mem_part1", mem_part1, MemBlkNum_1, MemBlkSize_1, &err);
    if (err != OS_ERR_NONE)
    {
        ErrorNum(__FILE__, __LINE__, err);
    }
    OSMemCreate(&MEM2_MCB, "mem_part2", mem_part2, MemBlkNum_2, MemBlkSize_2, &err);
    OSMemCreate(&MEM3_MCB, "mem_part3", mem_part3, MemBlkNum_3, MemBlkSize_3, &err);
}


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


    OSTaskCreate((OS_TCB *)&TIM2Hisr_TCB,                                   // 任务控制块指针
                 (CPU_CHAR *)"TIM2Hisr_TCB",                                      // 任务名称
                 (OS_TASK_PTR)Task_SoftTimerHisr,                              // 任务代码指针
                 (void *)0,                                               // 传递给任务的参数parg
                 (OS_PRIO)TASK_TIM2Hisr_PRIO,                             // 任务优先级
                 (CPU_STK *)&TIM2Hisr_Stk[0],                           // 任务堆栈基地址
                 (CPU_STK_SIZE)TASK_TIM2Hisr_STK_SIZE / 10,         // 堆栈剩余警戒线
                 (CPU_STK_SIZE)TASK_TIM2Hisr_STK_SIZE,                  // 堆栈大小
                 (OS_MSG_QTY)10,                                              // 可接收的最大消息队列数
                 (OS_TICK)0,                                              // 时间片轮转时间
                 (void *)0,                                               // 任务控制块扩展信息
                 (OS_OPT)(OS_OPT_TASK_STK_CHK |
                          OS_OPT_TASK_STK_CLR),         // 任务选项
                 (OS_ERR *)&err);                                       // 返回值


}




















