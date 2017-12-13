/******************************************************************************

  Copyright (C), 2001-2011, DCN Co., Ltd.

 ******************************************************************************
  File Name     : app.c
  Version       : Initial Draft
  Author        : lxl
  Created       : 2015/7/26
  Last Modified :
  Description   : lxl
  Function List :
              CallBack
              Task_EXTIHandler
              Task_MsgHandler
              Task_SoftTimerHisr
  History       :
  1.Date        : 2015/7/26
    Author      : lxl
    Modification: Created file

  2.Date         : 2015/7/26
    Author       : lxl
    Modification : second

******************************************************************************/
#include "app.h"
/*----------------------------------------------*
 * external variables                           *
 *----------------------------------------------*/

/*----------------------------------------------*
 * external routine prototypes                  *
 *----------------------------------------------*/

/*----------------------------------------------*
 * internal routine prototypes                  *
 *----------------------------------------------*/

/*----------------------------------------------*
 * project-wide global variables                *
 *----------------------------------------------*/

/*----------------------------------------------*
 * module-wide global variables                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * constants                                    *
 *----------------------------------------------*/

/*----------------------------------------------*
 * macros                                       *
 *----------------------------------------------*/

/*----------------------------------------------*
 * routines' implementations                    *
 *----------------------------------------------*/


/*****************************************************************************
 Prototype    : Task_SoftTimerHisr
 Description  : sotftimer
 Input        : void *p_arg
 Output       : None
 Return Value :
 Calls        :
 Called By    :

  History        :
  1.Date         : 2015/7/26
    Author       : lxl
    Modification : Created function

*****************************************************************************/
void Task_SoftTimerHisr(void *p_arg)
{
    OS_ERR err;
    (void)p_arg;

    while (1)
    {
        OSTaskQPend(0, OS_OPT_PEND_BLOCKING, 0, NULL, &err);
        SoftTimerHisr();
    }
}


void Task_BootHandler(void *p_arg)
{
    OS_ERR err;
    (void)p_arg;

    while (1)
    {
        rt_led_blue_on();
        OSTimeDly(100, OS_OPT_TIME_DLY, &err);
        rt_led_blue_off();
        OSTimeDly(100, OS_OPT_TIME_DLY, &err);
    }
}

void CallBack()
{
    printf(" CallBack\r\n");

}

/* -------------------------------------end of file------------------------------------ */
