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
 /* Promblem Number: 1     Author:lxl,   Date:2015/7/26 
    Description    : haahh */
    /* BEGIN: Added by lxl, 2015/7/26   PN:1 */
    /* END:   Added by lxl, 2015/7/26 */
    /* BEGIN: Added by lxl, 2015/7/26   PN:1 */
    /* BEGIN: Deleted by lxl, 2015/7/26   PN:1 */
    /* END:   Added by lxl, 2015/7/26   PN:1 */
    /* BEGIN: Deleted by lxl, 2015/7/26   PN:1 */
    
    /* END:   Deleted by lxl, 2015/7/26   PN:1 */
    /* BEGIN: Modified by lxl, 2015/7/26   PN:1 */
    /* BEGIN: Modified by lxl, 2015/7/26   PN:1 */
    
    /* END:   Modified by lxl, 2015/7/26   PN:1 */
    
void Task_EXTIHandler(void *p_arg)
{
  OS_ERR err;
  
  (void)p_arg;                	

  while (1)
  {
    OSTaskQPend(0,OS_OPT_PEND_BLOCKING,0,NULL,&err);
    ExtiHisr();

  }
}


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
    OSTaskQPend(0,OS_OPT_PEND_BLOCKING,0,NULL,&err);
    SoftTimerHisr();
    //GPIO_ResetBits(GPIOC, GPIO_Pin_3);
    //OSTimeDlyHMSM(0, 0,1,0,OS_OPT_TIME_HMSM_STRICT,&err);
    
    //GPIO_SetBits(GPIOC, GPIO_Pin_3);
    //OSTimeDlyHMSM(0, 0,1,0,OS_OPT_TIME_HMSM_STRICT,&err);

  }


}


void Task_MsgHandler(void *p_arg)
{
  u16 len,MsgId;
  OS_ERR err;
	u8* memblk;
  (void)p_arg;    
  

  while (1)
  {
    memblk = (u8*)OSTaskQPend(0,OS_OPT_PEND_BLOCKING,&len,0,&err);

    MsgId = memblk[0];

    switch(MsgId)
    {
      case Write_Flash_REQ:
        TestWriteFlash();
        printf("ok\r\n");
        break;
      case Write_DAC_REQ:
        DAC_WriteData(memblk);
        break;
      case Read_ADC_REQ:
        ADC1_SingleConvert(memblk);
        break;
      default:
        break;
    }

    FreeMem(memblk);
    OSTimeDlyHMSM(0, 0,0,200,OS_OPT_TIME_HMSM_STRICT,&err);
  }
}





void CallBack()
{
    printf(" CallBack\r\n");

}


/* -------------------------------------end of file------------------------------------ */
