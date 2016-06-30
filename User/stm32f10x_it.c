/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "includes.h"
#include "global.h"



u8 gUsart1RxState,gUsart1RxStep,gUsartRxOver;
u16 gUsart1RxLength;
u8* gUsart1RxPoint;

#define USART1_RX_IDLE  0
#define USART1_RX_BUSY  1

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
#if 0
void PendSV_Handler(void)
{
}
#endif

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	OSIntEnter(); 	 //用于统计中断的嵌套层数，对嵌套层数+1
	OSTimeTick();	   //统计时间，遍历任务，对延时任务计时减1
	OSIntExit();	   //对嵌套层数减1，在退出中断前启动任务调度
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

void ADC1_2_IRQHandler(void)
{
    if(ADC_GetITStatus(ADC1,ADC_IT_EOC) == SET)
    {

        ADC_ClearITPendingBit(ADC1,ADC_IT_EOC);
        

    }
}

/***********************************************
*ADC1 convert finish
***********************************************/
void DMA1_Channel1_IRQHandler()
{
    if(DMA_GetITStatus(DMA1_IT_TC1) == SET)
    {
        
        ADC_Cmd(ADC1, DISABLE);
        DMA_ClearITPendingBit(DMA1_IT_TC1);
        printf("adc_value is %d\r\n",ADC1_Filter());
        
    }

    
}

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
void USART1_IRQHandler(void)
{
    OS_ERR err;
  u8 *memblk;
  u8 getChar;
  u32 i,index;
  
  if(USART_GetITStatus(USART1,USART_IT_RXNE) == SET)
  {
    USART_ClearITPendingBit(USART1,USART_IT_RXNE);
    getChar = (u8)USART_ReceiveData(USART1);
    switch(gUsart1RxState)
    {
      case USART1_RX_IDLE:
        switch(gUsart1RxStep)
        {
          case 0:
            if(getChar == 0x5F)
              gUsart1RxStep++;
            else
              gUsart1RxStep = 0;
            break;
          case 1:
            if(getChar == 0x5F)
              gUsart1RxStep++;
            else
              gUsart1RxStep = 0;
            break;
          case 2:
            gUsart1RxStep++;
            gUsart1RxLength = getChar;
            break;
          case 3:
            gUsart1RxStep++;
            gUsart1RxState = USART1_RX_BUSY;
            gUsart1RxPoint = gUsart1RxBuff;
            gUsart1RxLength = (u16)(getChar << 8)|(gUsart1RxLength);
            break;
          default:
            gUsartRxOver = 0;
            gUsart1RxStep = 0;
            gUsart1RxPoint = NULL;
            gUsart1RxState = USART1_RX_IDLE;
            break;
        }
        break;
      case USART1_RX_BUSY:
        index = gUsart1RxStep - 4;
        if((getChar == 0xAA)&&(gUsart1RxStep-4 == gUsart1RxLength))
        {
          gUsartRxOver = 1;
        }
        else if((getChar == 0x55)&&(gUsartRxOver == 1))
        {
          memblk = (u8*)AllocMem(gUsart1RxLength);
          if(memblk != NULL)
          {
            memset(memblk,0,gUsart1RxLength);
            for(i=0;i<gUsart1RxLength;i++)
            {
              memblk[i] = gUsart1RxBuff[i];
            }

            gUsartRxOver = 0;
            gUsart1RxStep = 0;
            gUsart1RxPoint = NULL;
            gUsart1RxState = USART1_RX_IDLE;
            
            OSTaskQPost(&MsgHandler_TCB,NULL,0,OS_OPT_POST_FIFO,&err);           
          }
          
        }
        else
        {
          if((gUsart1RxStep-4) > gUsart1RxLength)
          {
            gUsartRxOver = 0;
            gUsart1RxStep = 0;
            gUsart1RxLength = 0;
            gUsart1RxPoint = NULL;
            gUsart1RxState = USART1_RX_IDLE;
          }
          gUsart1RxStep++;
          gUsart1RxPoint[index] = getChar;
        }
        break;
      default:
        gUsartRxOver = 0;
        gUsart1RxStep = 0;
        gUsart1RxLength = 0;
        gUsart1RxPoint = NULL;
        gUsart1RxState = USART1_RX_IDLE;
        break;
    }  
  }
}


void EXTI0_IRQHandler()
{
    uint32 bit;
	OSIntEnter(); 
    if(EXTI_GetITStatus(EXTI_Line0) == SET)
    {
        EXTI_MaskIRQ(0);
        delayMs(20);
        if(gExtiTrigerFlag[0] == RISE_EDGE)
        {
            gExtiTrigerFlag[0] = FALLING_EDGE;
            EXTI->RTSR &= (uint32)~(1 << 0);
            EXTI->FTSR |= (uint32)(1 << 0);
            ExtiISR(0,RISE_EDGE);
        }
        else
        {
            gExtiTrigerFlag[0] = RISE_EDGE;
            EXTI->RTSR |= (uint32)(1 << 0);
            EXTI->FTSR &= (uint32)~(1 << 0);
            ExtiISR(0,FALLING_EDGE);

        }
        EXTI_ClearITPendingBit(EXTI_Line0);
        EXTI_UnMaskIRQ(0);

    }

    OSIntExit();
}



void EXTI15_10_IRQHandler()
{
    uint32 bit;
	OSIntEnter(); 	 
    if(EXTI_GetITStatus(EXTI_Line10) == SET)
    {
        if(gExtiTrigerFlag[10] == RISE_EDGE)
        {
            gExtiTrigerFlag[10] = FALLING_EDGE;
            EXTI->RTSR &= (uint32)~(1 << 10);
            EXTI->FTSR |= (uint32)(1 << 10);
            ExtiISR(10,RISE_EDGE);
        }
        else
        {
            gExtiTrigerFlag[10] = RISE_EDGE;
            EXTI->RTSR |= (uint32)(1 << 10);
            EXTI->FTSR &= (uint32)~(1 << 10);
            ExtiISR(10,FALLING_EDGE);

        }
        
        EXTI_ClearITPendingBit(EXTI_Line10);

    }
	if(EXTI_GetITStatus(EXTI_Line11) == SET)
    {
        EXTI_ClearITPendingBit(EXTI_Line11);

    }
    if(EXTI_GetITStatus(EXTI_Line12) == SET)
    {
        EXTI_ClearITPendingBit(EXTI_Line12);

    }
    if(EXTI_GetITStatus(EXTI_Line13) == SET)
    {
        EXTI_MaskIRQ(13);
        delayMs(20);
        if(gExtiTrigerFlag[13] == RISE_EDGE)
        {
            gExtiTrigerFlag[13] = FALLING_EDGE;
            EXTI->RTSR &= (uint32)~(1 << 13);
            EXTI->FTSR |= (uint32)(1 << 13);
            ExtiISR(13,RISE_EDGE);
        }
        else
        {
            gExtiTrigerFlag[13] = RISE_EDGE;
            EXTI->RTSR |= (uint32)(1 << 13);
            EXTI->FTSR &= (uint32)~(1 << 13);
            ExtiISR(13,FALLING_EDGE);

        }
        EXTI_ClearITPendingBit(EXTI_Line13);
        EXTI_UnMaskIRQ(13);

    }
    if(EXTI_GetITStatus(EXTI_Line14) == SET)
    {
        EXTI_ClearITPendingBit(EXTI_Line14);

    }
    if(EXTI_GetITStatus(EXTI_Line15) == SET)
    {
        EXTI_ClearITPendingBit(EXTI_Line15);

    }
    
	OSIntExit();

}



void TIM2_IRQHandler()
{
    OS_ERR err;
    if(TIM_GetITStatus(TIM2,TIM_IT_Update) == SET)
    {
        OSIntEnter(); 	
    	OSTaskQPost(&TIM2Hisr_TCB,NULL,0,NULL,&err);
        TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);
    	OSIntExit();
    }
    
}

void RTC_IRQHandler(void)
{
    if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
    {
    /* Clear the RTC Second interrupt */
    RTC_ClearITPendingBit(RTC_IT_SEC);

    /* Enable time update */

    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
    Time_Display();
    }
    
    if (RTC_GetITStatus(RTC_IT_ALR) != RESET)
    {
    /* Clear the RTC Second interrupt */
    RTC_ClearITPendingBit(RTC_IT_ALR);

    /* Enable time update */

    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
    printf(" RTC WAKEUP\r\n");
    }
}


void RTCAlarm_IRQHandler()
{

}


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
