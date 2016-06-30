#include "bsp_exti.h"

volatile uint32 gExtiTrigerFlag[16];
volatile uint32 gExtiFIFOInputCnt = 0;
volatile uint32 gExtiFIFOOutputCnt = 0;


S_ExtiFifo gExtiFIFO[32];


void EXTI_PA0_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;

	/* config the extiline(PA0) clock and AFIO clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO,ENABLE);
												

	/* EXTI line gpio config(PA0) */	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;       
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	 // 上拉输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* EXTI line(PA0) mode config */
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0); 
  
    if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) == SET)
    {
        EXTI_InitStructure.EXTI_Line = EXTI_Line0;
        EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿中断
        EXTI_InitStructure.EXTI_LineCmd = ENABLE;
        EXTI_Init(&EXTI_InitStructure); 
    }
    else
    {
        EXTI_InitStructure.EXTI_Line = EXTI_Line0;
        EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; 
        EXTI_InitStructure.EXTI_LineCmd = ENABLE;
        EXTI_Init(&EXTI_InitStructure); 
    }
  
}


void EXTI_PC13_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;

	/* config the extiline(PC13) clock and AFIO clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO,ENABLE);
												

	/* EXTI line gpio config(PC13) */	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;       
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	 // 上拉输入
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* EXTI line(PC13) mode config */
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource13); 
  
    if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13) == SET)
    {
        EXTI_InitStructure.EXTI_Line = EXTI_Line13;
        EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿中断
        EXTI_InitStructure.EXTI_LineCmd = ENABLE;
        EXTI_Init(&EXTI_InitStructure); 

    }
    else
    {
        EXTI_InitStructure.EXTI_Line = EXTI_Line13;
        EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; 
        EXTI_InitStructure.EXTI_LineCmd = ENABLE;
        EXTI_Init(&EXTI_InitStructure); 

    }

  
}


void EXTI_Cfg()
{
    EXTI_PA0_Config();
    EXTI_PC13_Config();
}


void EXTI_MaskIRQ(uint32 PinNum)
{
    EXTI->IMR &= (uint32)~(1 << PinNum);
}

void EXTI_UnMaskIRQ(uint32 PinNum)
{
    EXTI->IMR |= (uint32)(1 << PinNum);
}



void ExtiISR(uint32 PinNum, uint32 Trigger)
{
    OS_ERR err;
    gExtiFIFO[gExtiFIFOInputCnt].PinNum = PinNum;
    gExtiFIFO[gExtiFIFOInputCnt].TrigState = Trigger;
    gExtiFIFOInputCnt = (++gExtiFIFOInputCnt)&0x1f;

    OSTaskQPost(&EXTIHandler_TCB,NULL,0,NULL,&err);
}


void ExtiHisr()
{
    while(gExtiFIFOInputCnt != gExtiFIFOOutputCnt)
    {
        switch(gExtiFIFO[gExtiFIFOOutputCnt].PinNum)
        {
            case 0:
                if(gExtiFIFO[gExtiFIFOOutputCnt].TrigState == RISE_EDGE)
                {
                    RDA5807_ReadRSSI();
                }
                else
                {
                    
                }
                break;
                
                break;
            case 13:
                if(gExtiFIFO[gExtiFIFOOutputCnt].TrigState == RISE_EDGE)
                {
                    printf(" key_13 release\r\n");
                    //SoftTimerSet(CallBack,100);
                    RDA5807_FM_seek();

                }
                else
                {
                    printf(" key_13 press\r\n");

                }
                break;
            default:
                break;

        }
        gExtiFIFOOutputCnt = (++gExtiFIFOOutputCnt)&0x1f;

    }
}













