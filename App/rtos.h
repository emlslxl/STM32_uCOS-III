#ifndef _RTOS_H_
#define _RTOS_H_


#include "global.h"



/********************控制块外部声明*******************/
extern OS_TCB   MsgHandler_TCB;
extern OS_TCB   EXTIHandler_TCB;
extern OS_TCB   TIM2Hisr_TCB;



extern OS_MEM MEM1_MCB;
extern OS_MEM MEM2_MCB;
extern OS_MEM MEM3_MCB;


/**************************内存块定义***************************/

#define MemBlkNum_1  10
#define MemBlkSize_1 20

#define MemBlkNum_2  10
#define MemBlkSize_2 100

#define MemBlkNum_3  6
#define MemBlkSize_3 500



/*******************设置任务优先级*******************/
#define TASK_EXTIHandler_PRIO               4
#define TASK_MsgHandler_PRIO                5
#define TASK_TIM2Hisr_PRIO              7


/************设置栈大小（单位为 OS_STK ）************/
#define TASK_EXTIHandler_STK_SIZE         80
#define TASK_MsgHandler_STK_SIZE          80
#define TASK_TIM2Hisr_STK_SIZE              80










/**************** 用户任务声明 *******************/
extern void Task_EXTIHandler(void *p_arg);
extern void Task_MsgHandler(void *p_arg);
extern void Task_SoftTimerHisr(void *p_arg);




extern void *AllocMem(uint32 size);
extern void FreeMem(void *pAddr);

extern void RTOS_CreateMem(void);
extern void RTOS_CreateTask(void);















#endif

