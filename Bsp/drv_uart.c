/**
  ******************************************************************************
  * @file    bsp_usart1.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   usart应用bsp
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 iSO STM32 开发板
  * 论坛    :http://www.chuxue123.com
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */

#include "drv_uart.h"


static int stm32_putc(USART_TypeDef *USARTx, char c)
{
    USARTx->SR;// 防止第一次发送时的乱码
    USARTx->DR = c;
    while (!(USARTx->SR & USART_FLAG_TC));

    return 1;
}

static int stm32_getc(USART_TypeDef *USARTx)
{
    int ch;

    ch = -1;
    if (USARTx->SR & USART_FLAG_RXNE)
    {
        ch = USARTx->DR & 0xff;
    }

    return ch;
}

/**
 * @desc  : 重映射printf功能到调试串口
 * @param : ch 待发送的字符
 * @return: none
 */
int fputc(int ch, FILE *f)
{
    stm32_putc(USART3, ch);

    return ch;
}


static void RCC_Configuration(void)
{
#if defined(RT_USING_UART1)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
#endif /* RT_USING_UART1 */

#if defined(RT_USING_UART2)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
#endif /* RT_USING_UART2 */

#if defined(RT_USING_UART3)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOD, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
#ifdef UART3_REMAP
    GPIO_PinRemapConfig(GPIO_FullRemap_USART3, ENABLE);
#endif
#endif

#if defined(RT_USING_UART4)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);//UART4
#endif

#if defined(RT_USING_UART5)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);//UART5
#endif
}

static void GPIO_Configuration(void)
{
#if defined(RT_USING_UART1)
    rt_gpio_set_mode(GPIOA, GPIO_Pin_10, GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz);
    rt_gpio_set_mode(GPIOA, GPIO_Pin_9, GPIO_Mode_AF_PP, GPIO_Speed_50MHz);
#endif /* RT_USING_UART1 */

#if defined(RT_USING_UART2)
    rt_gpio_set_mode(GPIOA, GPIO_Pin_3, GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz);
    rt_gpio_set_mode(GPIOA, GPIO_Pin_2, GPIO_Mode_AF_PP, GPIO_Speed_50MHz);
#endif /* RT_USING_UART2 */

#if defined(RT_USING_UART3)
#ifdef UART3_REMAP
    rt_gpio_set_mode(GPIOD, GPIO_Pin_9, GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz);
    rt_gpio_set_mode(GPIOD, GPIO_Pin_8, GPIO_Mode_AF_PP, GPIO_Speed_50MHz);
#else
    rt_gpio_set_mode(GPIOB, GPIO_Pin_11, GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz);
    rt_gpio_set_mode(GPIOB, GPIO_Pin_10, GPIO_Mode_AF_PP, GPIO_Speed_50MHz);
#endif /* UART3_REMAP */
#endif /* RT_USING_UART3 */

#if defined(RT_USING_UART4)
    rt_gpio_set_mode(GPIOC, GPIO_Pin_11, GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz);
    rt_gpio_set_mode(GPIOC, GPIO_Pin_10, GPIO_Mode_AF_PP, GPIO_Speed_50MHz);
#endif /* RT_USING_UART4 */

#if defined(RT_USING_UART5)
    rt_gpio_set_mode(GPIOD, GPIO_Pin_5, GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz);
    rt_gpio_set_mode(GPIOC, GPIO_Pin_12, GPIO_Mode_AF_PP, GPIO_Speed_50MHz);
#endif /* RT_USING_UART5 */
}

void bsp_uart_init(void)
{
    USART_InitTypeDef USART_InitStructure;

    RCC_Configuration();
    GPIO_Configuration();

#if defined(RT_USING_UART3)
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART3, &USART_InitStructure);
    USART_Cmd(USART3, ENABLE);
#endif /* RT_USING_UART3 */
}

/*********************************************END OF FILE**********************/
