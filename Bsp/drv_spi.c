#include "drv_spi.h"

void rt_hw_spi_init(SPI_TypeDef *SPIx)
{
    SPI_InitTypeDef SPI_InitStructure;

    if (SPIx == SPI1)
    {
        /* Enable GPIO clock */
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
        rt_gpio_set_mode(GPIOA, GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7, GPIO_Mode_AF_PP, GPIO_Speed_50MHz);

        rt_gpio_set_mode(GPIOA, GPIO_Pin_4, GPIO_Mode_Out_PP, GPIO_Speed_10MHz);
        GPIO_SetBits(GPIOA, GPIO_Pin_4);

        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
    }
    else if (SPIx == SPI2)
    {
        /* Enable GPIO clock */
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
        rt_gpio_set_mode(GPIOB, GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15, GPIO_Mode_AF_PP, GPIO_Speed_50MHz);

        rt_gpio_set_mode(GPIOB, GPIO_Pin_12, GPIO_Mode_Out_PP, GPIO_Speed_10MHz);
        GPIO_SetBits(GPIOB, GPIO_Pin_12);

        RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
    }
    else
    {
        printf("no SPI selected\r\n");
    }

    SPI_StructInit(&SPI_InitStructure);

    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_NSS  = SPI_NSS_Soft;

    /* init SPI */
    SPI_I2S_DeInit(SPIx);
    SPI_Init(SPIx, &SPI_InitStructure);
    /* Enable SPI_MASTER */
    SPI_Cmd(SPIx, ENABLE);
}

uint32_t spi_xfer(SPI_TypeDef *SPIx, struct rt_spi_message *message)
{
    const uint8_t *send_ptr = message->send_buf;
    uint8_t *recv_ptr = message->recv_buf;
    uint32_t size = message->length;
    uint8_t data = 0xFF;

    /* take CS */
    if (SPIx == SPI1)
    {
        GPIO_ResetBits(GPIOA, GPIO_Pin_4);
    }
    else if (SPIx == SPI2)
    {
        GPIO_ResetBits(GPIOB, GPIO_Pin_12);
    }

    // if (config->data_width <= 8)
    {
        while (size--)
        {
            if (send_ptr != NULL)
            {
                data = *send_ptr++;
            }

            //Wait until the transmit buffer is empty
            while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET);
            // Send the byte
            SPI_I2S_SendData(SPIx, data);

            //Wait until a data is received
            while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET);
            // Get the received data
            data = SPI_I2S_ReceiveData(SPIx);

            if (recv_ptr != NULL)
            {
                *recv_ptr++ = data;
            }
        }
    }

    /* release CS */
    if (SPIx == SPI1)
    {
        GPIO_SetBits(GPIOA, GPIO_Pin_4);
    }
    else if (SPIx == SPI2)
    {
        GPIO_SetBits(GPIOB, GPIO_Pin_12);
    }

    return message->length;
}

uint32_t rt_spi_transfer(SPI_TypeDef          *SPIx,
                         const void           *send_buf,
                         void                 *recv_buf,
                         uint32_t             length)
{
    uint32_t result;
    struct rt_spi_message message;

    {
        /* initial message */
        message.send_buf   = send_buf;
        message.recv_buf   = recv_buf;
        message.length     = length;
        message.cs_take    = 1;
        message.cs_release = 1;
        message.next       = NULL;

        /* transfer message */
        result = spi_xfer(SPIx, &message);
        if (result == 0)
        {
        }
    }

    return result;
}

uint32_t rt_spi_send_then_recv(SPI_TypeDef          *SPIx,
                               const void           *send_buf,
                               uint32_t             send_length,
                               void                 *recv_buf,
                               uint32_t             recv_length)
{
    uint32_t result;
    struct rt_spi_message message;

    /* send data */
    message.send_buf   = send_buf;
    message.recv_buf   = NULL;
    message.length     = send_length;
    message.cs_take    = 1;
    message.cs_release = 0;
    message.next       = NULL;

    result = spi_xfer(SPIx, &message);
    if (result == 0)
    {
        // result = -RT_EIO;
    }

    /* recv data */
    message.send_buf   = NULL;
    message.recv_buf   = recv_buf;
    message.length     = recv_length;
    message.cs_take    = 0;
    message.cs_release = 1;
    message.next       = NULL;

    result = spi_xfer(SPIx, &message);;
    if (result == 0)
    {
        // result = -RT_EIO;
    }

    result = RT_EOK;

    return result;
}

uint32_t rt_spi_send_then_send(SPI_TypeDef          *SPIx,
                               const void           *send_buf1,
                               uint32_t             send_length1,
                               const void           *send_buf2,
                               uint32_t             send_length2)
{
    uint32_t result;
    struct rt_spi_message message;

    /* send data1 */
    message.send_buf   = send_buf1;
    message.recv_buf   = NULL;
    message.length     = send_length1;
    message.cs_take    = 1;
    message.cs_release = 0;
    message.next       = NULL;

    result = spi_xfer(SPIx, &message);
    if (result == 0)
    {
        result = -RT_EIO;
    }

    /* send data2 */
    message.send_buf   = send_buf2;
    message.recv_buf   = NULL;
    message.length     = send_length2;
    message.cs_take    = 0;
    message.cs_release = 1;
    message.next       = NULL;

    result = spi_xfer(SPIx, &message);
    if (result == 0)
    {
        result = -RT_EIO;
    }

    result = RT_EOK;

    return result;
}

uint8_t rt_spi_sendrecv8(SPI_TypeDef        *SPIx,
                         uint8_t            data)
{
    uint8_t value;

    rt_spi_send_then_recv(SPIx, &data, 1, &value, 1);

    return value;
}

uint32_t rt_spi_recv(SPI_TypeDef        *SPIx,
                     void                 *recv_buf,
                     uint32_t             length)
{
    return rt_spi_transfer(SPIx, NULL, recv_buf, length);
}

uint32_t rt_spi_send(SPI_TypeDef        *SPIx,
                     const void           *send_buf,
                     uint32_t             length)
{
    return rt_spi_transfer(SPIx, send_buf, NULL, length);
}

