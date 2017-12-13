#include "app_iap.h"

struct iap_info iap;



void iap_init(void)
{
    iap.app1_addr_begin = ADDR_APP_RUN;
    iap.app2_addr_begin = ADDR_APP_BKP;
    iap.total_revc_byte = 0;
}

void ENTER_CRITICAL_SECTION(void)//进入超临界 关总中断
{
    __set_PRIMASK(1);
}

void EXIT_CRITICAL_SECTION(void)//退出超临界 开总中断
{
    __set_PRIMASK(0);
}

/*******************************************************************************
  * @函数名称   FLASH_PagesMask
  * @函数说明   计算Falsh页
  * @输入参数   Size:文件长度
  * @输出参数   无
  * @返回参数   页的数量
*******************************************************************************/
uint32_t FLASH_PagesMask(uint32_t Size)
{
    uint32_t pagenumber = 0x0;
    uint32_t size = Size;

    if ((size % PAGE_SIZE) != 0)
    {
        pagenumber = (size / PAGE_SIZE) + 1;
    }
    else
    {
        pagenumber = size / PAGE_SIZE;
    }
    return pagenumber;

}

FLASH_Status  EraseSectors(uint32_t destination_address, uint32_t flashsize)
{
    FLASH_Status FLASHStatus  = FLASH_COMPLETE;
    uint32_t NbrOfPage = 0;
    uint32_t EraseCounter = 0x0;
    uint16_t PageSize = PAGE_SIZE;

    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
    NbrOfPage = FLASH_PagesMask(flashsize); //计算需要擦除Flash的页

    printf("- Erase app sectors.\r\n");
    printf("  Start address: %08X, End address: %08X\r\n", destination_address, destination_address + flashsize);
    printf("  Flash size:% 3d KB\r\n", flashsize >> 10);
    printf("  Erase sectors: % 3d pages\r\n", NbrOfPage);

    //擦除Flash
    FLASH_Unlock();
    for (EraseCounter = 0; (EraseCounter < NbrOfPage) && (FLASHStatus == FLASH_COMPLETE); EraseCounter++)
    {
        FLASHStatus = FLASH_ErasePage(destination_address + (PageSize * EraseCounter));
        if (FLASHStatus != FLASH_COMPLETE)
        {
            printf("FLASH_ErasePage is failed, error_num = %d\r\n", FLASHStatus);
        }
    }
    FLASH_Lock();

    return FLASHStatus;
}

uint32_t WriteUpdateFlag(uint32_t updateFlag)
{
    uint32_t writeData = 0;
    uint32_t readData  = 0;


    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);

    writeData = updateFlag;
    writeData = (writeData << 8) & 0xFF00;

    ENTER_CRITICAL_SECTION();
    FLASH_Unlock();
    FLASH_ErasePage(ADDR_DATA_FILED);//擦出对应地址的1页(2K)
    FLASH_ProgramWord(ADDR_DATA_FILED, writeData);
    FLASH_Lock();
    EXIT_CRITICAL_SECTION();

    readData = (*(u32 *)(ADDR_DATA_FILED));

    if (readData == writeData)
        return 1;
    else
        return 0;
}

uint32_t ReadUpdateFlag(void)
{
    uint32_t updateFlag = 0;
    updateFlag = (*(u32 *)(ADDR_DATA_FILED));
    updateFlag = (updateFlag >> 8) & 0xFF;
    return updateFlag;
}



uint32_t iap_write_data(uint32_t write_address, uint8_t *data, uint32_t length)
{
    uint16_t i, index;
    uint32_t data_len;
    uint32_t data_addr;
    uint32_t write_addr;

    data_len = length;
    data_addr = (uint32_t)data;
    write_addr = write_address;

    ENTER_CRITICAL_SECTION();

    FLASH_Unlock();
    for (i = 0; i < data_len; i += 4)
    {
        //把接收到的数据编写到Flash中
        index = FLASH_ProgramWord(write_addr, *(uint32_t *)data_addr);

        if (index != FLASH_COMPLETE)
            printf("flash write failed,error_num is %d\r\n", index);

        if (*(uint32_t *)write_addr != *(uint32_t *)data_addr)
        {
            printf("flash write data not same\r\n");
            return -2;
        }
        write_addr += 4;
        data_addr += 4;
    }
    FLASH_Lock();

    EXIT_CRITICAL_SECTION();

    return 0;
}

void iap_revc_data_packet(uint8_t *data, uint32_t length)
{
    iap_write_data(iap.app2_addr_begin + iap.total_revc_byte, data, length);
    iap.total_revc_byte += length;
    printf("flash write data total %d byte\r\n", iap.total_revc_byte);
}

uint8_t copy_flash_to_flash(uint32_t des, uint32_t src)
{
    uint32_t datalen;
    uint32_t addr_src;
    uint32_t addr_des;

    addr_src = src;
    addr_des = des;

    FLASH_Unlock();

    for (datalen = 0; datalen < APP_DATA_SIZE; datalen += 4)
    {
        FLASH_ProgramWord(addr_des, *(uint32_t *)addr_src);

        if (*(uint32_t *)addr_src != *(uint32_t *)addr_des)
        {
            return -2;
        }
        addr_src += 4;
        addr_des += 4;
    }

    FLASH_Lock();

    printf("* Program flash size: < %d KB>\r\n\r\n", (datalen >> 10));

    return 0;
}

uint8_t copy_app2_to_app1(void)
{
    printf("* copy app2 to app1\r\n");
    return copy_flash_to_flash(ADDR_APP_RUN, ADDR_APP_BKP);
}

uint8_t copy_app1_to_app2(void)
{
    printf("* copy app1 to app2\r\n");
    return copy_flash_to_flash(ADDR_APP_BKP, ADDR_APP_RUN);
}


void iap_write_flag_R(void)
{
    WriteUpdateFlag('R');
}

void iap_write_flag_N(void)
{
    WriteUpdateFlag('N');
}

void iap_erase_app1(void)
{
    EraseSectors(ADDR_APP_RUN, APP_DATA_SIZE);
}

void iap_erase_app2(void)
{
    EraseSectors(ADDR_APP_BKP, APP_DATA_SIZE);
}


typedef  void (*pFunction)(void);

pFunction Jump_To_Application;

void jump_to_addr(uint32_t jump_address)
{
    uint32_t tmp;

    /* Test if user code is programmed starting from address "ApplicationAddress" */
    if (((*(__IO uint32_t *)jump_address) & 0x2FFE0000) == 0x20000000)
    {
        /* Jump to user application */
        tmp = *(__IO uint32_t *)(jump_address + 4);

        Jump_To_Application = (pFunction) tmp;

        /* Initialize user application's Stack Pointer */
        __set_MSP(*(__IO uint32_t *) jump_address);

        Jump_To_Application();
    }
}

void boot_jump_to_app1(void)
{
    jump_to_addr(ADDR_APP_RUN);
}



