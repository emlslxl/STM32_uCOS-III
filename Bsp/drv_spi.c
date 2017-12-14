#include "drv_spi.h"

void bsp_spi2_init(void)
{
    SPI_InitTypeDef  SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    /*!< sFLASH_SPI_CS_GPIO, sFLASH_SPI_MOSI_GPIO, sFLASH_SPI_MISO_GPIO
        and sFLASH_SPI_SCK_GPIO Periph clock enable */
    RCC_APB2PeriphClockCmd(sFLASH_CS_GPIO_CLK | sFLASH_SPI_MOSI_GPIO_CLK | sFLASH_SPI_MISO_GPIO_CLK |
                           sFLASH_SPI_SCK_GPIO_CLK, ENABLE);

    /*!< sFLASH_SPI Periph clock enable */
    RCC_APB1PeriphClockCmd(sFLASH_SPI_CLK, ENABLE);

    /*!< Configure sFLASH_SPI pins: SCK */
    GPIO_InitStructure.GPIO_Pin = sFLASH_SPI_SCK_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(sFLASH_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);

    /*!< Configure sFLASH_SPI pins: MOSI */
    GPIO_InitStructure.GPIO_Pin = sFLASH_SPI_MOSI_PIN;
    GPIO_Init(sFLASH_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);

    /*!< Configure sFLASH_SPI pins: MISO */
    GPIO_InitStructure.GPIO_Pin = sFLASH_SPI_MISO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(sFLASH_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);

    /*!< Configure sFLASH_CS_PIN pin: sFLASH Card CS pin */
    GPIO_InitStructure.GPIO_Pin = sFLASH_CS_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(sFLASH_CS_GPIO_PORT, &GPIO_InitStructure);

    /*!< Deselect the FLASH: Chip Select high */
    sFLASH_CS_HIGH();

    /*!< SPI configuration */
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;

    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(sFLASH_SPI, &SPI_InitStructure);

    /*!< Enable the sFLASH_SPI  */
    SPI_Cmd(sFLASH_SPI, ENABLE);
}



