#ifndef __APP_IAP_H__
#define __APP_IAP_H__

#include "board.h"


#define IAP_USE_FILE    {IAP_USE_APP2,IAP_USE_FILE}

// 是否更新标志
// 'Y' --- 进行更新；
// 'N' --- 不进行更新
#define BOOTLOADER_SIZE     (1024*46)         // 46K bytes
#define DATA_FILED_SIZE     (1024*10)         // 10K bytes
#define APP_DATA_SIZE       (1024*228)        // 228K bytes

#define ADDR_BOOTLOADER     0x08000000
#define ADDR_DATA_FILED     (ADDR_BOOTLOADER + BOOTLOADER_SIZE)
#define ADDR_APP_RUN        (ADDR_DATA_FILED + DATA_FILED_SIZE)
#define ADDR_APP_BKP        (ADDR_APP_RUN    + APP_DATA_SIZE)

#define ADDR_DATA_RECV_BYTE (ADDR_DATA_FILED + 4)   //数据域 占用4字节 表示升级文件size

struct iap_info
{
    uint32_t app1_addr_begin;
    uint32_t app1_addr_end;
    uint32_t app2_addr_begin;
    uint32_t app2_addr_end;
    uint32_t total_revc_byte;
};

struct iap_flash_info
{
    uint32_t upgrade_flag;
    uint32_t upgrade_file_size;
};

extern struct iap_flash_info iap_flash;

#if defined (STM32F10X_MD) || defined (STM32F10X_MD_VL)
    #define PAGE_SIZE                         (0x400)    /* 1 Kbyte */
    #define FLASH_SIZE                        (0x20000)  /* 128 KBytes */
#elif defined STM32F10X_CL
    #define PAGE_SIZE                         (0x800)    /* 2 Kbytes */
    #define FLASH_SIZE                        (0x40000)  /* 256 KBytes */
#elif defined STM32F10X_HD || defined (STM32F10X_HD_VL)
    #define PAGE_SIZE                         (0x800)    /* 2 Kbytes */
    #define FLASH_SIZE                        (0x80000)  /* 512 KBytes */
#elif defined STM32F10X_XL
    #define PAGE_SIZE                         (0x800)    /* 2 Kbytes */
    #define FLASH_SIZE                        (0x100000) /* 1 MByte */
#else
    #error "Please select first the STM32 device to be used (in stm32f10x.h)"
#endif

extern void read_upgrade_info(void);
extern void iap_write_flag_R(void);
extern void iap_write_flag_N(void);
extern void iap_erase_app1(void);
extern void iap_erase_app2(void);
extern void boot_jump_to_app1(void);
extern void iap_init(void);
extern void fota_app2_handle(void);
extern void write_data_to_app1(uint8_t *data, uint32_t length);

#endif /* end of include guard: __APP_IAP_H__ */
