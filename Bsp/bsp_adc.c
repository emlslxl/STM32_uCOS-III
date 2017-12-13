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
    GPIO_Init(GPIOC, &GPIO_InitStructure);              // PC1,输入时不用设置速率
}

/**
  * @brief  配置ADC1的工作模式为MDA模式
  * @param  无
  * @retval 无
  */
static void ADC1_DMA_Mode_Config(void)
{
    DMA_InitTypeDef DMA_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;

    /* DMA channel1 configuration */
    DMA_DeInit(DMA1_Channel1);

    DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;             //ADC地址
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&gADC_ConvertedValue;   //内存地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = 16; /* 代表接受通道的内存 */
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;    //外设地址固定
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //半字
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; /* 循环模式下，DMA传输计数才会自动加载 */
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    /* Enable DMA  */
    DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);
    /* Enable DMA channel1 */
    DMA_Cmd(DMA1_Channel1, ENABLE);

    /* ADC1 configuration */
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;          //独立ADC模式
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;               //扫描模式，扫描模式用于多通道采集
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;          //连续转换模式，即不停地进行ADC转换
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //外部触发转换
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;  //采集数据右对齐
    ADC_InitStructure.ADC_NbrOfChannel = 1;                                 //要转换的通道数目2
    ADC_Init(ADC1, &ADC_InitStructure);


    /*配置ADC时钟，为PCLK2的8分频，即9MHz*/
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);

    /*配置ADC1的通道11为55.   5个采样周期，序列为1 */
    ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_55Cycles5);

    /* Enable ADC1 */
    ADC_Cmd(ADC1, ENABLE);
    /* Enable ADC1 DMA */
    ADC_DMACmd(ADC1, ENABLE);


    /*复位校准寄存器 */
    ADC_ResetCalibration(ADC1);
    /*等待校准寄存器复位完成 */
    while (ADC_GetResetCalibrationStatus(ADC1));

    /* ADC校准 */
    ADC_StartCalibration(ADC1);
    /* 等待校准完成*/
    while (ADC_GetCalibrationStatus(ADC1));

    ADC_TempSensorVrefintCmd(ENABLE);
    /* 由于没有采用外部触发，所以使用软件触发ADC转换 */
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);

}

/*************************************************************
  * @brief  配置ADC1的工作模式为单次转换模式
  * @param  无
  * @retval 无
************************************************************/
static void ADC1_Single_Mode_Config(void)
{
    DMA_InitTypeDef DMA_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;

    /* ADC1 configuration */
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;          //独立ADC模式
    ADC_InitStructure.ADC_ScanConvMode = DISABLE ;                  //扫描模式，扫描模式用于多通道采集
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;         //连续转换模式，即不停地进行ADC转换
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //外部触发转换
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;  //采集数据右对齐
    ADC_InitStructure.ADC_NbrOfChannel = 1;                                 //要转换的通道数目2
    ADC_Init(ADC1, &ADC_InitStructure);


    /*配置ADC时钟，为PCLK2的8分频，即9MHz*/
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);


    /* Enable ADC1 */
    ADC_Cmd(ADC1, ENABLE);
    /* Enable ADC1 DMA */
    //ADC_DMACmd(ADC1, ENABLE);


    /*复位校准寄存器 */
    ADC_ResetCalibration(ADC1);
    /*等待校准寄存器复位完成 */
    while (ADC_GetResetCalibrationStatus(ADC1));

    /* ADC校准 */
    ADC_StartCalibration(ADC1);
    /* 等待校准完成*/
    while (ADC_GetCalibrationStatus(ADC1));

    /* Enable ADC1 */
    ADC_Cmd(ADC1, DISABLE);

}


/*
************************************************************************************************************************
*
* Description: 初始化ADC1
*
* Arguments  : none
*
* Returns    : none
*
* Note(s)    : ADC可配成单次转换和连续转换2种模式
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
* Description: 连续转换模式下启动转换
*
* Arguments  : none
*
* Returns    : none
*
* Note(s)    : 不需要连续转换的时，在DMA中断函数中将ADC关闭了，所以需要再次转换时，
                需要调用该函数启动ADC
************************************************************************************************************************
*/

void ADC1_MultiChanConvert(void)
{
    /* Enable ADC1 */
    ADC_Cmd(ADC1, ENABLE);

    /* 由于没有采用外部触发，所以使用软件触发ADC转换 */
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);

}

/*
************************************************************************************************************************
*
* Description: 启动单次转换
*
* Arguments  : channel -> 需要启动转换的通道
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
        /*配置ADC1的通道11为55. 5个采样周期，序列为1 */
        ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_55Cycles5);
        break;
    case 1:
        ADC_TempSensorVrefintCmd(ENABLE);
        /*配置ADC1的通道11为55. 5个采样周期，序列为2 */
        ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1, ADC_SampleTime_239Cycles5);
        break;
    default:
        break;
    }

    /* Enable ADC1 */
    ADC_Cmd(ADC1, ENABLE);

    /* 由于没有采用外部触发，所以使用软件触发ADC转换 */
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
    u32 sum = 0;/*使用前初始化*/

    for (i = 0; i < 16; i++)
    {
        sum += gADC_ConvertedValue[i];

    }
    return (sum >> 4);
}













