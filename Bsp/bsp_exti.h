#ifndef _BSP_EXTI_H_
#define _BSP_EXTI_H_

#include "global.h"

extern volatile uint32 gExtiTrigerFlag[16];

#define RISE_EDGE       1
#define FALLING_EDGE    0



typedef struct 
{
    uint32 PinNum;
    uint32 TrigState;
}S_ExtiFifo;















#endif

