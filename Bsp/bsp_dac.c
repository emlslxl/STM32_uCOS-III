#include "bsp_dac.h"



void DAC_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    DAC_InitTypeDef  DAC_InitStructure;

    /* 使能GPIOA时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    /* 使能DAC时钟 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

    /* DAC的GPIO配置，模拟输入 */
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);



    /* 配置DAC 通道1 */
    DAC_InitStructure.DAC_Trigger = DAC_Trigger_Software;
    DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;   //不使用波形发生器
    DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;    //不使用DAC输出缓冲
    DAC_Init(DAC_Channel_1, &DAC_InitStructure);

    /* 配置DAC 通道2 */
    DAC_Init(DAC_Channel_2, &DAC_InitStructure);

    /* 使能通道1 由PA4输出 */
    DAC_Cmd(DAC_Channel_1, ENABLE);
    /* 使能通道2 由PA5输出 */
    DAC_Cmd(DAC_Channel_2, ENABLE);

    /* 使能DAC的DMA请求 */
    //DAC_DMACmd(DAC_Channel_2, ENABLE);
}



void DAC_WriteData(void *pMsgbody)
{
    u8 *pMsg;
    u16 data, channel;

    pMsg = (u8 *)pMsgbody;
    channel = *(u16 *)(pMsg + 2);
    data = *(u16 *)(pMsg + 4);

    if (channel == 1)
    {
        DAC_SetChannel2Data(DAC_Align_12b_R, data);
        DAC_SoftwareTriggerCmd(DAC_Channel_2, ENABLE);
    }
    else if (channel == 2)
    {
        DAC_SetChannel1Data(DAC_Align_12b_R, data);
        DAC_SoftwareTriggerCmd(DAC_Channel_1, ENABLE);
    }
}
















