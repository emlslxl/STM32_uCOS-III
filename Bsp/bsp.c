#include "bsp.h"

u32 ChipUniqueID[3];

/**
  * @brief  Get_ChipID
  * @param  ��
  * @retval ��
  */
void Get_ChipID(void)
{
	ChipUniqueID[0] = *(__IO u32 *)(0X1FFFF7F0); // ���ֽ�
	ChipUniqueID[1] = *(__IO u32 *)(0X1FFFF7EC); // 
	ChipUniqueID[2] = *(__IO u32 *)(0X1FFFF7E8); // ���ֽ�
}

/*
 * ��������BSP_Init
 * ����  ��ʱ�ӳ�ʼ����Ӳ����ʼ��
 * ����  ����
 * ���  ����
 */
void BSP_Init(void)
{
  SysTick_init();
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
  
  /* LED �˿ڳ�ʼ�� */
  LED_GPIO_Config();  
  //EXTI_Cfg();

  USART1_Config();
  ADC1_Init();
  //DAC_Config();
  //RTC_Configuration();
  
  //SoftTimerInit();

  //NVIC_Configuration();
  
}

/*
 * ��������SysTick_init
 * ����  ������SysTick��ʱ��
 * ����  ����
 * ���  ����
 */
void SysTick_init(void)
{
  /* ��ʼ����ʹ��SysTick��ʱ�� */
  SysTick_Config(SystemCoreClock/OS_CFG_TICK_RATE_HZ);
  
  /*  ����1ms �ж�һ�Σ���os��Ƶ��Ϊ1000hz */
	if (SysTick_Config(SystemCoreClock/OS_CFG_TICK_RATE_HZ))	
	{ 
		/* Capture error */ 
		while (1);
	}
}





/* --------------------------------------end of file--------------------------------------- */
