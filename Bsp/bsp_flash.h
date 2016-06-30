#ifndef _BSP_FLASH_H_
#define _BSP_FLASH_H_

#include "global.h"



#define FLASH_PAGE_SIZE    0x800  /* 2kByte/page stm32f103ZE */



typedef struct
{
  u16 MsgID;
  u16 len;
  u32 addr;
}
S_WriteFlashReq;







#endif  /* _BSP_FLASH_H_ */

