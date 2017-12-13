#ifndef _BSP_DAC_H_
#define _BSP_DAC_H_


#include "global.h"










typedef struct
{
    u16 MsgID;
    u16 channel;
    u16 value;
}
S_WriteDACReq;





#endif  /*_BSP_DAC_H_*/

