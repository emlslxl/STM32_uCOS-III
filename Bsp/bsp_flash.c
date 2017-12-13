#include "bsp_flash.h"


u16 Flash_Write_Without_check(u32 iAddress, u8 *buf, u16 iNumByteToWrite)
{
    u16 i;
    volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;
    i = 0;

    //    FLASH_UnlockBank1();
    while (i < iNumByteToWrite)
    {
        FLASHStatus = FLASH_ProgramHalfWord(iAddress, *(u16 *)buf);
        if (FLASHStatus != FLASH_COMPLETE)
            return FLASHStatus;
        i = i + 2;
        iAddress = iAddress + 2;
        buf = buf + 2;
    }

    return FLASH_COMPLETE;
}


u16 Flash_Write(u32 iAddress, u8 *buf, u32 iNbrToWrite)
{
    /* Unlock the Flash Bank1 Program Erase controller */
    u16 NumOfPage, NumOfByte, addr, temp;
    u16 PageRemain;
    u16 i = 0;
    u16 res;

    FLASH_UnlockBank1();
    addr = iAddress % FLASH_PAGE_SIZE; //扇区地址
    NumOfPage = iNbrToWrite / FLASH_PAGE_SIZE; //写入数据占用几个页
    NumOfByte = iNbrToWrite % FLASH_PAGE_SIZE; //不够一页的数据字节数
    PageRemain = FLASH_PAGE_SIZE - addr; //扇区剩余空间大小

    if (addr == 0) /* 页对齐 */
    {
        if (NumOfPage == 0) /* 只有1页 */
        {
            res = FLASH_ErasePage(iAddress); //擦除这个扇区
            res = Flash_Write_Without_check(iAddress, buf, iNbrToWrite);
        }
        else  /* 大于1页 */
        {
            while (NumOfPage--)
            {
                res = FLASH_ErasePage(iAddress); //擦除这个扇区
                res = Flash_Write_Without_check(iAddress, buf, FLASH_PAGE_SIZE);
                iAddress += FLASH_PAGE_SIZE;
                buf += FLASH_PAGE_SIZE;
            }
            res = FLASH_ErasePage(iAddress); //擦除这个扇区
            res = Flash_Write_Without_check(iAddress, buf, NumOfByte);

        }

    }
    else  /* no 页对齐  */
    {
        if (NumOfPage == 0) /* 数据量小于一页 */
        {
            if (NumOfByte > PageRemain) /* 数据存储跨页了 */
            {
                res = FLASH_ErasePage(iAddress); //擦除这个扇区
                res = Flash_Write_Without_check(iAddress, buf, PageRemain);
                iAddress += PageRemain;
                buf += PageRemain;
                temp = NumOfByte - PageRemain;
                res = FLASH_ErasePage(iAddress); //擦除这个扇区
                res = Flash_Write_Without_check(iAddress, buf, temp);
            }
            else
            {
                res = FLASH_ErasePage(iAddress); //擦除这个扇区
                res = Flash_Write_Without_check(iAddress, buf, iNbrToWrite);
            }
        }
        else  /* 数据量大于一页 */
        {
            iNbrToWrite -= PageRemain;
            NumOfPage = iNbrToWrite / FLASH_PAGE_SIZE;
            NumOfByte = iNbrToWrite % FLASH_PAGE_SIZE;

            res = FLASH_ErasePage(iAddress); //擦除这个扇区
            res = Flash_Write_Without_check(iAddress, buf, PageRemain);
            iAddress += FLASH_PAGE_SIZE;
            buf += FLASH_PAGE_SIZE;

            while (NumOfPage--)
            {
                res = FLASH_ErasePage(iAddress); //擦除这个扇区
                res = Flash_Write_Without_check(iAddress, buf, FLASH_PAGE_SIZE);
                iAddress += FLASH_PAGE_SIZE;
                buf += FLASH_PAGE_SIZE;
            }

            if (NumOfByte != 0)
            {
                res = FLASH_ErasePage(iAddress); //擦除这个扇区
                res = Flash_Write_Without_check(iAddress, buf, NumOfByte);
            }
        }
    }

    FLASH_LockBank1();

    return FLASH_COMPLETE;
}


u16 Flash_Read(u32 iAddress, u8 *buf, u32 iNbrToRead)
{
    int i = 0;
    while (i < iNbrToRead)
    {
        *(buf + i) = *(__IO u8 *)iAddress++;
        i++;
    }
    return i;
}


void function_test()
{
    u8 watchBuff[10];
    Flash_Read(0x8020000, watchBuff, 10);
}



void TestWriteFlash(void *pMsgBody, u16 len)
{
    u32 AddrToWrite;
    u16 LenToWrite;

    u8 buff[10] = {2, 3, 4, 5, 6, 7, 8, 9, 1, 1};

    LenToWrite = 10;
    AddrToWrite = 0x8020000;

    Flash_Write(AddrToWrite, buff, LenToWrite);
}



















