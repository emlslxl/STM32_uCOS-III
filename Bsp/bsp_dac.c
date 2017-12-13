#include "bsp_dac.h"



void DAC_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    DAC_InitTypeDef  DAC_InitStructure;

    /* ʹ��GPIOAʱ�� */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    /* ʹ��DACʱ�� */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

    /* DAC��GPIO���ã�ģ������ */
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);



    /* ����DAC ͨ��1 */
    DAC_InitStructure.DAC_Trigger = DAC_Trigger_Software;
    DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;   //��ʹ�ò��η�����
    DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;    //��ʹ��DAC�������
    DAC_Init(DAC_Channel_1, &DAC_InitStructure);

    /* ����DAC ͨ��2 */
    DAC_Init(DAC_Channel_2, &DAC_InitStructure);

    /* ʹ��ͨ��1 ��PA4��� */
    DAC_Cmd(DAC_Channel_1, ENABLE);
    /* ʹ��ͨ��2 ��PA5��� */
    DAC_Cmd(DAC_Channel_2, ENABLE);

    /* ʹ��DAC��DMA���� */
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
















