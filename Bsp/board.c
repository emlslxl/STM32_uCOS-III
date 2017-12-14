#include "board.h"


/*********************************************
*                延时函数
*********************************************/
/* 毫秒级精确延时 */
__asm void
SysCtlDelay(unsigned long ulCount)
{
    subs    r0, #1;
    bne     SysCtlDelay;
    bx      lr;
}

void delay_us(uint32_t delay)
{
    SysCtlDelay(delay * (SystemCoreClock / 6000000));
}

void delay_ms(uint32_t delay)
{
    SysCtlDelay(delay * (SystemCoreClock / 6000));
}


void rt_nvic_init(IRQn_Type irq, uint8_t PreemptionPriority, uint8_t SubPriority)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = irq;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PreemptionPriority;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = SubPriority;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void NVIC_Configuration(void)
{
#ifdef  VECT_TAB_RAM
    /* Set the Vector Table base location at 0x20000000 */
    NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else  /* VECT_TAB_FLASH  */
    /* Set the Vector Table base location at 0x08000000 */
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
#endif
}


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


/*
 * 函数名：BSP_Init
 * 描述  ：时钟初始化、硬件初始化
 * 输入  ：无
 * 输出  ：无
 */
void BSP_Init(void)
{
    NVIC_Configuration();

    SysTick_init();

    bsp_gpio_init();
    bsp_uart_init();
    bsp_spi2_init();
    gd_init();

    switch (RCC_GetSYSCLKSource())
    {
    case 0x00 :
        printf("\r\nHSI used as system clock.\r\n");
        break;
    case 0x04 :
        printf("\r\nHSE used as system clock.\r\n");
        break;
    case 0x08 :
        printf("\r\nPLL used as system clock.\r\n");
        break;

    default:
        printf("\r\n * unkonwn clock soure. *");
        break;
    }
}


/* --------------------------------------end of file--------------------------------------- */
