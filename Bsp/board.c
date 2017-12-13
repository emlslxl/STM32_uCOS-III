#include "board.h"

void rt_nvic_init(IRQn_Type irq, uint8_t PreemptionPriority, uint8_t SubPriority)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = irq;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PreemptionPriority;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = SubPriority;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void rt_exti_init(uint32_t line, EXTIMode_TypeDef mode, EXTITrigger_TypeDef trigger, uint8_t flag)
{
    EXTI_InitTypeDef EXTI_InitStructure;

    EXTI_InitStructure.EXTI_Line = line;
    EXTI_InitStructure.EXTI_Mode = mode;
    EXTI_InitStructure.EXTI_Trigger = trigger;
    EXTI_InitStructure.EXTI_LineCmd = flag;
    EXTI_Init(&EXTI_InitStructure);
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


/* --------------------------------------end of file--------------------------------------- */
