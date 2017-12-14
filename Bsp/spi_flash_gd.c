#include "spi_flash_gd.h"

struct spi_flash_device  spi_flash_device;

#define FLASH_DEBUG

#ifdef FLASH_DEBUG
#define FLASH_TRACE(fmt, ...)    printf(fmt, ##__VA_ARGS__)
#else
#define FLASH_TRACE(fmt, ...)
#endif /* #ifdef FLASH_DEBUG */


#define PAGE_SIZE           4096
#define MF_ID           (0xC8)
#define MTC_GD25Q128                (0x4018)


#define sFLASH_DUMMY_BYTE             0x55

#define TIMEOUT 1000


#define sFLASH_CMD_NOR_WRITE          0x02  /*!< Write to Memory instruction */
#define sFLASH_CMD_NOR_WRSR           0x01  /*!< Write Status Register instruction */
#define sFLASH_CMD_NOR_WREN           0x06  /*!< Write enable instruction */
#define sFLASH_CMD_NOR_WDIS           0x04   /*!< Write disable instruction */
#define sFLASH_CMD_NOR_READ           0x03  /*!< Read from Memory instruction */
#define sFLASH_CMD_NOR_SFDP           0x5A  /*!< Read from Memory instruction */
#define sFLASH_CMD_NOR_RDSR           0x05  /*!< Read Status Register instruction  */
#define sFLASH_CMD_NOR_MISR           0x35  /*!< Read Status Register instruction Midde Byte */
#define sFLASH_CMD_NOR_HISR           0x15  /*!< Read Status Register instruction Hige Byte */
#define sFLASH_CMD_NOR_RDID           0x9F  /*!< Read identification */
#define sFLASH_CMD_NOR_SE             0x20  /*!< Sector Erase instruction */
#define sFLASH_CMD_NOR_BE             0xD8  /*!< Block Erase instruction */
#define sFLASH_CMD_NOR_CE             0xC7  /*!< Chip Erase instruction */

#define sFLASH_WIP_FLAG           0x01  /*!< Write In Progress (WIP) flag */



void sFLASH_Hardware_Init(void)
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

uint8_t sFLASH_SendByte(uint8_t byte)
{
    /*!< Loop while DR register in not emplty */
    while (SPI_I2S_GetFlagStatus(sFLASH_SPI, SPI_I2S_FLAG_TXE) == RESET);

    /*!< Send byte through the SPI1 peripheral */
    SPI_I2S_SendData(sFLASH_SPI, byte);

    /*!< Wait to receive a byte */
    while (SPI_I2S_GetFlagStatus(sFLASH_SPI, SPI_I2S_FLAG_RXNE) == RESET);

    /*!< Return the byte read from the SPI bus */
    return SPI_I2S_ReceiveData(sFLASH_SPI);
}

uint8_t sFLASH_ReadByte(void)
{
    return (sFLASH_SendByte(sFLASH_DUMMY_BYTE));
}

void sFLASH_WriteEnable(void)
{
    /*!< Select the FLASH: Chip Select low */
    sFLASH_CS_LOW();

    /*!< Send "Write Enable" instruction */
    sFLASH_SendByte(sFLASH_CMD_NOR_WREN);

    /*!< Deselect the FLASH: Chip Select high */
    sFLASH_CS_HIGH();
}

void sFLASH_WriteDisable(void)
{
    /*!< Select the FLASH: Chip Select low */
    sFLASH_CS_LOW();

    /*!< Send "Write Enable" instruction */
    sFLASH_SendByte(sFLASH_CMD_NOR_WDIS);

    /*!< Deselect the FLASH: Chip Select high */
    sFLASH_CS_HIGH();
}

void sFLASH_NOR_WaitForWriteEnd(void)
{
    uint8_t flashstatus = 0;

    /*!< Select the FLASH: Chip Select low */
    sFLASH_CS_LOW();

    /*!< Send "Read Status Register" instruction */
    sFLASH_SendByte(sFLASH_CMD_NOR_RDSR);

    /*!< Loop as long as the memory is busy with a write cycle */
    do
    {
        /*!< Send a dummy byte to generate the clock needed by the FLASH
        and put the value of the status register in FLASH_Status variable */
        flashstatus = sFLASH_SendByte(sFLASH_CMD_NOR_RDSR);
    }
    while ((flashstatus & sFLASH_WIP_FLAG) == SET); /* Write in progress */

    /*!< Deselect the FLASH: Chip Select high */
    sFLASH_CS_HIGH();
}

uint32_t sFLASH_NOR_ReadID(void)
{
    uint8_t data1;
    uint8_t data2;
    uint8_t data3;
    uint32_t data;

    sFLASH_CS_LOW();
    sFLASH_SendByte(sFLASH_CMD_NOR_RDID);
    data1 = sFLASH_SendByte(sFLASH_DUMMY_BYTE);
    data2 = sFLASH_SendByte(sFLASH_DUMMY_BYTE);
    data3 = sFLASH_SendByte(sFLASH_DUMMY_BYTE);
    sFLASH_CS_HIGH();
    data = (data1 << 16) | (data2 << 8) | data3;
    printf("JEDEC Read-ID Data : 0x%x\r\n", data);
    return data;
}

void sFLASH_NOR_SectorErase(uint32_t SectorAddr)
{
    /*!< Send write enable instruction */
    sFLASH_WriteEnable();

    /*!< Sector Erase */
    /*!< Select the FLASH: Chip Select low */
    sFLASH_CS_LOW();
    /*!< Send Sector Erase instruction */
    sFLASH_SendByte(sFLASH_CMD_NOR_SE);
    /*!< Send SectorAddr high nibble address byte */
    sFLASH_SendByte((SectorAddr & 0xFF0000) >> 16);
    /*!< Send SectorAddr medium nibble address byte */
    sFLASH_SendByte((SectorAddr & 0xFF00) >> 8);
    /*!< Send SectorAddr low nibble address byte */
    sFLASH_SendByte(SectorAddr & 0xFF);
    /*!< Deselect the FLASH: Chip Select high */
    sFLASH_CS_HIGH();

    /*!< Wait the end of Flash writing */
    sFLASH_NOR_WaitForWriteEnd();
}

void sFLASH_NOR_Pageprogram(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
    /*!< Enable the write access to the FLASH */
    sFLASH_WriteEnable();

    /*!< Select the FLASH: Chip Select low */
    sFLASH_CS_LOW();
    /*!< Send "Write to Memory " instruction */
    sFLASH_SendByte(sFLASH_CMD_NOR_WRITE);
    /*!< Send WriteAddr high nibble address byte to write to */
    sFLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
    /*!< Send WriteAddr medium nibble address byte to write to */
    sFLASH_SendByte((WriteAddr & 0xFF00) >> 8);
    /*!< Send WriteAddr low nibble address byte to write to */
    sFLASH_SendByte(WriteAddr & 0xFF);

    /*!< while there is data to be written on the FLASH */
    while (NumByteToWrite--)
    {
        /*!< Send the current byte */
        sFLASH_SendByte(*pBuffer);
        /*!< Point on the next byte to be written */
        pBuffer++;
    }

    /*!< Deselect the FLASH: Chip Select high */
    sFLASH_CS_HIGH();

    /*!< Wait the end of Flash writing */
    sFLASH_NOR_WaitForWriteEnd();
}

void sFLASH_NOR_Read(uint8_t *pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
    /*!< Select the FLASH: Chip Select low */
    sFLASH_CS_LOW();

    /*!< Send "Read from Memory " instruction */
    sFLASH_SendByte(sFLASH_CMD_NOR_READ);

    /*!< Send ReadAddr high nibble address byte to read from */
    sFLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
    /*!< Send ReadAddr medium nibble address byte to read from */
    sFLASH_SendByte((ReadAddr & 0xFF00) >> 8);
    /*!< Send ReadAddr low nibble address byte to read from */
    sFLASH_SendByte(ReadAddr & 0xFF);

    while (NumByteToRead--) /*!< while there is data to be read */
    {
        /*!< Read a byte from the FLASH */
        *pBuffer = sFLASH_SendByte(sFLASH_DUMMY_BYTE);
        /*!< Point to the next location where the byte read will be saved */
        pBuffer++;
    }

    /*!< Deselect the FLASH: Chip Select high */
    sFLASH_CS_HIGH();
}

uint8_t sFLASH_NOR_ReadLowSR(void)
{
    uint8_t data = 0;
    sFLASH_CS_LOW();
    sFLASH_SendByte(sFLASH_CMD_NOR_RDSR);
    data = sFLASH_SendByte(sFLASH_DUMMY_BYTE);
    sFLASH_CS_HIGH();
    return data;
}

void w25qxx_wait_busy(void)
{
    while (sFLASH_NOR_ReadLowSR() & (0x01));
}

uint32_t w25qxx_flash_control(uint8_t cmd, void *args)
{

    if (cmd == RT_DEVICE_CTRL_BLK_GETGEOME)
    {
        struct rt_device_blk_geometry *geometry;

        geometry = (struct rt_device_blk_geometry *)args;
        if (geometry == NULL) return -RT_ERROR;

        geometry->bytes_per_sector = spi_flash_device.geometry.bytes_per_sector;
        geometry->sector_count = spi_flash_device.geometry.sector_count;
        geometry->block_size = spi_flash_device.geometry.block_size;
    }

    return RT_EOK;
}


uint32_t w25qxx_read(uint32_t offset, uint8_t *buffer, uint32_t size)
{
    sFLASH_NOR_Read(buffer, offset, size);

    return size;
}

uint32_t w25qxx_flash_read(uint32_t pos,
                           void *buffer,
                           uint32_t size)
{
    w25qxx_read(pos * spi_flash_device.geometry.bytes_per_sector,
                buffer,
                size * spi_flash_device.geometry.bytes_per_sector);

    return size;
}



uint32_t w25qxx_page_write(uint32_t page_addr, uint8_t *buffer)
{
    uint32_t index;
    uint8_t send_buffer[4];

    sFLASH_NOR_SectorErase(page_addr);   //ERASE 4K

    w25qxx_wait_busy(); // wait erase done.

    for (index = 0; index < (PAGE_SIZE / 256); index++)
    {
        sFLASH_NOR_Pageprogram(buffer, page_addr, 256);

        buffer += 256;
        page_addr += 256;
        w25qxx_wait_busy();
    }

    return PAGE_SIZE;
}

uint32_t w25qxx_flash_write(uint32_t pos,
                            void *buffer,
                            uint32_t size)
{
    uint32_t i = 0;
    uint32_t block = size;
    uint8_t *ptr = buffer;

    while (block--)
    {
        w25qxx_page_write((pos + i)*spi_flash_device.geometry.bytes_per_sector,
                          ptr);
        ptr += PAGE_SIZE;
        i++;
    }

    return size;
}

uint8_t read_tmp[4096] = {0};
//test
void test_read()
{
    uint32_t i = 0;

    // w25qxx_flash_read(0, read_tmp, 1);
    sFLASH_NOR_Read(read_tmp, 30 * 4096, 1024);
    for (i = 0; i < 1024; i++)
    {
        printf("%02d ", read_tmp[i]);
    }
    printf("\r\n");
}

uint8_t test_tmp[4096] = {0};
void test_write()
{
    uint32_t i = 0;

    for (i = 0; i < 256; i++)
    {
        test_tmp[i] = i;
    }

    sFLASH_NOR_SectorErase(30 * 4096);
    sFLASH_NOR_Pageprogram(test_tmp, 30 * 4096, 256);
    // w25qxx_flash_write(0, test_tmp, 1);
}


uint32_t gd_init(void)
{
    uint32_t id_recv;
    uint16_t memory_type_capacity;

    /* read flash id */
    id_recv = sFLASH_NOR_ReadID();

    if ((id_recv >> 16 & 0xff) != MF_ID)
    {
        FLASH_TRACE("Manufacturers ID error!\r\n");
        FLASH_TRACE("JEDEC Read-ID Data : 0x%06X\r\n", id_recv);
        return 0;
    }

    spi_flash_device.geometry.bytes_per_sector = 4096;
    spi_flash_device.geometry.block_size = 4096; /* block erase: 4k */

    /* get memory type and capacity */
    memory_type_capacity = id_recv & 0xffff;

    if (memory_type_capacity == MTC_GD25Q128)
    {
        FLASH_TRACE("GD128 detection\r\n");
        spi_flash_device.geometry.sector_count = 4096;
    }
    else
    {
        FLASH_TRACE("Memory Capacity error!\r\n");
        return 0;
    }

    test_write();
    test_read();
}




