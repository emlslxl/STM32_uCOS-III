#ifndef _RTOS_H_
#define _RTOS_H_


#include "global.h"



/********************控制块外部声明*******************/
extern OS_TCB   BootHandler_TCB;



/*******************设置任务优先级*******************/
#define TASK_BootHandler_PRIO               4


/************设置栈大小（单位为 OS_STK ）************/
#define TASK_BootHandler_STK_SIZE         512










/**************** 用户任务声明 *******************/
extern void Task_BootHandler(void *p_arg);
extern void Task_SoftTimerHisr(void *p_arg);




extern void *AllocMem(uint32 size);
extern void FreeMem(void *pAddr);

extern void RTOS_CreateMem(void);
extern void RTOS_CreateTask(void);















#endif

