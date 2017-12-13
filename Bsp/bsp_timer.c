#include "bsp_timer.h"



S_Timer_TCB gSoftTimer[TIMER_NUM];


/*
 * TIM_Period / Auto Reload Register(ARR) = 10000   TIM_Prescaler--71
 * �ж�����Ϊ = 1/(72MHZ /72) * 10000 = 10ms
 *
 * TIMxCLK/CK_PSC --> TIMxCNT --> TIM_Period(ARR) --> �ж� ��TIMxCNT����Ϊ0���¼���
 */
void TIM2_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    /* ����TIM2CLK Ϊ 72MHZ */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    //TIM_DeInit(TIM2);

    /* �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) */
    TIM_TimeBaseStructure.TIM_Period = 10000;

    /* �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж� */     /* ʱ��Ԥ��Ƶ��Ϊ72 */
    TIM_TimeBaseStructure.TIM_Prescaler = 71;

    /* ���ⲿʱ�ӽ��в�����ʱ�ӷ�Ƶ,����û���õ� */
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    TIM_ClearFlag(TIM2, TIM_FLAG_Update);

    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    TIM_Cmd(TIM2, ENABLE);

    /*�ȹرյȴ�ʹ��*/
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




