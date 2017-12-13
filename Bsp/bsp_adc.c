#include "bsp_adc.h"


#define ADC1_DR_Address    ((u32)0x40012400+0x4c)

u16 gADC_ConvertedValue[16];

static void ADC1_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Enable DMA clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    /* Enable ADC1 and GPIOC clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOC, ENABLE);

    /* Configure PC.01  as analog input */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOC, &GPIO_InitStructure);              // PC1,����ʱ������������
}

/**
  * @brief  ����ADC1�Ĺ���ģʽΪMDAģʽ
  * @param  ��
  * @retval ��
  */
static void ADC1_DMA_Mode_Config(void)
{
    DMA_InitTypeDef DMA_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;

    /* DMA channel1 configuration */
    DMA_DeInit(DMA1_Channel1);

    DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;             //ADC��ַ
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&gADC_ConvertedValue;   //�ڴ��ַ
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = 16; /* �������ͨ�����ڴ� */
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;    //�����ַ�̶�
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //����
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; /* ѭ��ģʽ�£�DMA��������Ż��Զ����� */
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    /* Enable DMA  */
    DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);
    /* Enable DMA channel1 */
    DMA_Cmd(DMA1_Channel1, ENABLE);

    /* ADC1 configuration */
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;          //����ADCģʽ
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;               //ɨ��ģʽ��ɨ��ģʽ���ڶ�ͨ���ɼ�
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;          //����ת��ģʽ������ͣ�ؽ���ADCת��
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //�ⲿ����ת��
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;  //�ɼ������Ҷ���
    ADC_InitStructure.ADC_NbrOfChannel = 1;                                 //Ҫת����ͨ����Ŀ2
    ADC_Init(ADC1, &ADC_InitStructure);


    /*����ADCʱ�ӣ�ΪPCLK2��8��Ƶ����9MHz*/
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);

    /*����ADC1��ͨ��11Ϊ55.   5���������ڣ�����Ϊ1 */
    ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_55Cycles5);

    /* Enable ADC1 */
    ADC_Cmd(ADC1, ENABLE);
    /* Enable ADC1 DMA */
    ADC_DMACmd(ADC1, ENABLE);


    /*��λУ׼�Ĵ��� */
    ADC_ResetCalibration(ADC1);
    /*�ȴ�У׼�Ĵ�����λ��� */
    while (ADC_GetResetCalibrationStatus(ADC1));

    /* ADCУ׼ */
    ADC_StartCalibration(ADC1);
    /* �ȴ�У׼���*/
    while (ADC_GetCalibrationStatus(ADC1));

    ADC_TempSensorVrefintCmd(ENABLE);
    /* ����û�в����ⲿ����������ʹ���������ADCת�� */
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);

}

/*************************************************************
  * @brief  ����ADC1�Ĺ���ģʽΪ����ת��ģʽ
  * @param  ��
  * @retval ��
************************************************************/
static void ADC1_Single_Mode_Config(void)
{
    DMA_InitTypeDef DMA_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;

    /* ADC1 configuration */
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;          //����ADCģʽ
    ADC_InitStructure.ADC_ScanConvMode = DISABLE ;                  //ɨ��ģʽ��ɨ��ģʽ���ڶ�ͨ���ɼ�
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;         //����ת��ģʽ������ͣ�ؽ���ADCת��
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //�ⲿ����ת��
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;  //�ɼ������Ҷ���
    ADC_InitStructure.ADC_NbrOfChannel = 1;                                 //Ҫת����ͨ����Ŀ2
    ADC_Init(ADC1, &ADC_InitStructure);


    /*����ADCʱ�ӣ�ΪPCLK2��8��Ƶ����9MHz*/
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);


    /* Enable ADC1 */
    ADC_Cmd(ADC1, ENABLE);
    /* Enable ADC1 DMA */
    //ADC_DMACmd(ADC1, ENABLE);


    /*��λУ׼�Ĵ��� */
    ADC_ResetCalibration(ADC1);
    /*�ȴ�У׼�Ĵ�����λ��� */
    while (ADC_GetResetCalibrationStatus(ADC1));

    /* ADCУ׼ */
    ADC_StartCalibration(ADC1);
    /* �ȴ�У׼���*/
    while (ADC_GetCalibrationStatus(ADC1));

    /* Enable ADC1 */
    ADC_Cmd(ADC1, DISABLE);

}


/*
************************************************************************************************************************
*
* Description: ��ʼ��ADC1
*
* Arguments  : none
*
* Returns    : none
*
* Note(s)    : ADC����ɵ���ת��������ת��2��ģʽ
************************************************************************************************************************
*/

void ADC1_Init(void)
{
    ADC1_GPIO_Config();
    ADC1_DMA_Mode_Config();
    //ADC1_Single_Mode_Config();
}


/*
************************************************************************************************************************
*
* Description: ����ת��ģʽ������ת��
*
* Arguments  : none
*
* Returns    : none
*
* Note(s)    : ����Ҫ����ת����ʱ����DMA�жϺ����н�ADC�ر��ˣ�������Ҫ�ٴ�ת��ʱ��
                ��Ҫ���øú�������ADC
************************************************************************************************************************
*/

void ADC1_MultiChanConvert(void)
{
    /* Enable ADC1 */
    ADC_Cmd(ADC1, ENABLE);

    /* ����û�в����ⲿ����������ʹ���������ADCת�� */
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);

}

/*
************************************************************************************************************************
*
* Description: ��������ת��
*
* Arguments  : channel -> ��Ҫ����ת����ͨ��
*
* Returns    : none
*
* Note(s)    :
************************************************************************************************************************
*/

u16 ADC1_SingleConvert(u16 channel)
{
    u16 Temperature;
    u16 data;


    switch (channel)
    {
    case 0:
        /*����ADC1��ͨ��11Ϊ55. 5���������ڣ�����Ϊ1 */
        ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_55Cycles5);
        break;
    case 1:
        ADC_TempSensorVrefintCmd(ENABLE);
        /*����ADC1��ͨ��11Ϊ55. 5���������ڣ�����Ϊ2 */
        ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1, ADC_SampleTime_239Cycles5);
        break;
    default:
        break;
    }

    /* Enable ADC1 */
    ADC_Cmd(ADC1, ENABLE);

    /* ����û�в����ⲿ����������ʹ���������ADCת�� */
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);


    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);

    data = ADC_GetConversionValue(ADC1);


    if (channel == 0)
    {
        //Temperature = Temperature*3.3/4096;
        // printf("ADC_ConversionValue is %d\r\n",data);
    }
    else if (channel == 1)
    {
        //Temperature = (1.42 - Temperature*3.3/4096)*1000/4.35 + 25;
        // printf("Temperature is %d 'C\r\n",data);
    }

    return data;

}


u32 ADC1_Filter()
{
    u32 i;
    u32 sum = 0;/*ʹ��ǰ��ʼ��*/

    for (i = 0; i < 16; i++)
    {
        sum += gADC_ConvertedValue[i];

    }
    return (sum >> 4);
}













