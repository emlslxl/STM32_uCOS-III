#include "bsp.h"

u32 ChipUniqueID[3];

/**
  * @brief  Get_ChipID
  * @param  无
  * @retval 无
  */
void Get_ChipID(void)
{
    ChipUniqueID[0] = *(__IO u32 *)(0X1FFFF7F0); // 高字节
    ChipUniqueID[1] = *(__IO u32 *)(0X1FFFF7EC); //
    ChipUniqueID[2] = *(__IO u32 *)(0X1FFFF7E8); // 低字节
}

/*
 * 函数名：BSP_Init
 * 描述  ：时钟初始化、硬件初始化
 * 输入  ：无
 * 输出  ：无
 */
void BSP_Init(void)
{
    SysTick_init();

    USART1_Config();

    switch (RCC_GetSYSCLKSource())
    {
    case 0x00 :
        printf("\r\n HSI used as system clock.\r\n");
        break;
    case 0x04 :
        printf("\r\n HSE used as system clock.\r\n");
        break;
    case 0x08 :
        printf("\r\n PLL used as system clock.\r\n");
        break;

    default:
        printf("\r\n * unkonwn clock soure. *");
        break;
    }
}

/*
 * 函数名：SysTick_init
 * 描述  ：配置SysTick定时器
 * 输入  ：无
 * 输出  ：无
 */
void SysTick_init(void)
{
    /* 初始化并使能SysTick定时器 */
    SysTick_Config(SystemCoreClock / OS_CFG_TICK_RATE_HZ);

    /*  配置1ms 中断一次，即os的频率为1000hz */
    if (SysTick_Config(SystemCoreClock / OS_CFG_TICK_RATE_HZ))
    {
        /* Capture error */
        while (1);
    }
}





/* --------------------------------------end of file--------------------------------------- */
