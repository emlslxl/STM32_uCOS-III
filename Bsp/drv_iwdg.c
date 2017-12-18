#include "drv_iwdg.h"


void rt_hw_iwdg_init(void)
{
    //允许写IWDG
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
    //设置256分频 中断1次6.4ms
    IWDG_SetPrescaler(IWDG_Prescaler_256);
    //设置Reload 最大4095
    IWDG_SetReload(0xfff);
    //重载值
    IWDG_ReloadCounter();
    //使能IDWG
    IWDG_Enable();
}

void rt_iwdg_feed(void)
{
    IWDG_ReloadCounter();//重载值
}


