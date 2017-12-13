#include "bsp_timer.h"



S_Timer_TCB gSoftTimer[TIMER_NUM];


/*
 * TIM_Period / Auto Reload Register(ARR) = 10000   TIM_Prescaler--71
 * 中断周期为 = 1/(72MHZ /72) * 10000 = 10ms
 *
 * TIMxCLK/CK_PSC --> TIMxCNT --> TIM_Period(ARR) --> 中断 且TIMxCNT重置为0重新计数
 */
void TIM2_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    /* 设置TIM2CLK 为 72MHZ */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    //TIM_DeInit(TIM2);

    /* 自动重装载寄存器周期的值(计数值) */
    TIM_TimeBaseStructure.TIM_Period = 10000;

    /* 累计 TIM_Period个频率后产生一个更新或者中断 */     /* 时钟预分频数为72 */
    TIM_TimeBaseStructure.TIM_Prescaler = 71;

    /* 对外部时钟进行采样的时钟分频,这里没有用到 */
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    TIM_ClearFlag(TIM2, TIM_FLAG_Update);

    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    TIM_Cmd(TIM2, ENABLE);

    /*先关闭等待使用*/
    //RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , DISABLE);
}



void SoftTimerInit()
{
    uint32 i;
    for (i = 0; i < TIMER_NUM; i++)
    {
        gSoftTimer[i].pCallBack = (void *)0;
        gSoftTimer[i].CounterLeft = 0;
        gSoftTimer[i].IsActive = IDLE;
    }

    TIM2_Configuration();
}


int SoftTimerSet(void (*pCallBack)(), uint32 TimeMs)
{
    int i;
    for (i = 0; i < TIMER_NUM; i++)
    {
        if (gSoftTimer[i].IsActive == IDLE)
        {
            gSoftTimer[i].IsActive = BUSY;
            gSoftTimer[i].pCallBack = pCallBack;
            gSoftTimer[i].CounterLeft = TimeMs;

            return i;

        }
    }

    return -1;
}


void SoftTimerHisr()
{
    uint32 i;
    for (i = 0; i < TIMER_NUM; i++)
    {
        if (gSoftTimer[i].IsActive == BUSY)
        {
            if (gSoftTimer[i].CounterLeft >= SOFT_TIMER_TICK)
            {
                gSoftTimer[i].CounterLeft -= SOFT_TIMER_TICK;
            }
            else
            {
                gSoftTimer[i].CounterLeft = 0;
                gSoftTimer[i].pCallBack();

                gSoftTimer[i].pCallBack = NULL;
                gSoftTimer[i].IsActive = IDLE;
            }

        }

    }
}




