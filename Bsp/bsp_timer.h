#ifndef _BSP_TIMER_H_
#define _BSP_TIMER_H_

#include "board.h"

#define IDLE    0
#define BUSY    1

#define TIMER_NUM   16  //
#define SOFT_TIMER_TICK 10 //10ms



typedef struct
{
    void (*pCallBack)();
    uint32 CounterLeft;
    uint32 IsActive;
} S_Timer_TCB;












#endif

