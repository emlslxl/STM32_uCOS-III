#include "rtos.h"

/******************for task***********************/
OS_TCB	TIM2Hisr_TCB;		   				
static	CPU_STK	TIM2Hisr_Stk[TASK_TIM2Hisr_STK_SIZE];	 

OS_TCB	EXTIHandler_TCB;		   				
static	CPU_STK	EXTIHandler_Stk[TASK_EXTIHandler_STK_SIZE];	   

OS_TCB	MsgHandler_TCB;		   				
static	CPU_STK	MsgHandler_Stk[TASK_MsgHandler_STK_SIZE];	   	   


/******************for memroy***********************/
uint8 mem_part1[MemBlkNum_1][MemBlkSize_1];
uint8 mem_part2[MemBlkNum_2][MemBlkSize_2];
uint8 mem_part3[MemBlkNum_3][MemBlkSize_3];

OS_MEM MEM1_MCB;
OS_MEM MEM2_MCB;
OS_MEM MEM3_MCB;


void ErrorNum(char* file, u32 line, u32 err)
{
  printf("error[%d]: file[%s],line[%d]\r\n",err,file,line);

}


void* AllocMem(uint32 size)
{
  OS_ERR err;	
  void *p;
  
  if(size < MemBlkSize_1)
  {
    p = OSMemGet(&MEM1_MCB,&err);
    if(err != OS_ERR_NONE)
    {
      ErrorNum(__FILE__,__LINE__,err);
    }
    return p;
  }
  else if((size >= MemBlkSize_1)&&(size < MemBlkSize_2))
  {
    p = OSMemGet(&MEM2_MCB,&err);
    if(err != OS_ERR_NONE)
    {
      ErrorNum(__FILE__,__LINE__,err);
    }
    return p;
  }
  else if((size >= MemBlkSize_2)&&(size < MemBlkSize_3))
  {
    p = OSMemGet(&MEM3_MCB,&err);
    if(err != OS_ERR_NONE)
    {
      ErrorNum(__FILE__,__LINE__,err);
    }
    return p;
  }
  if(size > MemBlkSize_3)
  {
    err = OS_ERR_MEM_INVALID_SIZE;
    ErrorNum(__FILE__,__LINE__,err);
    return (void*)0;
  }

}

void FreeMem(void* pAddr)
{
	OS_ERR err;
  uint8 *memPart1Start,*memPart2Start,*memPart3Start;
  uint8 *memPart1End,*memPart2End,*memPart3End;
  uint8 *p;

  p = (uint8*)pAddr;
  
  memPart1Start = (uint8*)&mem_part1[0][0];
  memPart1End = memPart1Start + MemBlkNum_1*MemBlkSize_1;

  memPart2Start = (uint8*)&mem_part2[0][0];
  memPart2End = memPart2Start + MemBlkNum_2*MemBlkSize_2;

  memPart3Start = (uint8*)&mem_part3[0][0];
  memPart3End = memPart3Start + MemBlkNum_3*MemBlkSize_3;
  
  if((p < memPart1End)&&(p >= memPart1Start))
  {
    OSMemPut(&MEM1_MCB,p,&err);
	if(err != OS_ERR_NONE)
    {
      ErrorNum(__FILE__,__LINE__,err);
    }
  }
  else if((p < memPart2End)&&(p >= memPart2Start))
  {
    OSMemPut(&MEM2_MCB,p,&err);
	if(err != OS_ERR_NONE)
    {
      ErrorNum(__FILE__,__LINE__,err);
    }
  }
  else if((p < memPart3End)&&(p >= memPart3Start))
  {
    OSMemPut(&MEM3_MCB,p,&err);
	if(err != OS_ERR_NONE)
    {
      ErrorNum(__FILE__,__LINE__,err);
    }
  }
  else
  {
    err = OS_ERR_MEM_INVALID_P_MEM;
	if(err != OS_ERR_NONE)
    {
      ErrorNum(__FILE__,__LINE__,err);
    }
  }
  
}




void RTOS_CreateMem()
{
	OS_ERR err;
    OSMemCreate(&MEM1_MCB,"mem_part1",mem_part1,MemBlkNum_1,MemBlkSize_1,&err);
    if(err != OS_ERR_NONE)
    {
      ErrorNum(__FILE__,__LINE__,err);
    }
    OSMemCreate(&MEM2_MCB,"mem_part2",mem_part2,MemBlkNum_2,MemBlkSize_2,&err);
    OSMemCreate(&MEM3_MCB,"mem_part3",mem_part3,MemBlkNum_3,MemBlkSize_3,&err);
}


void RTOS_CreateTask()
{
    OS_ERR err;
    /*�����ⲿ�жϴ�������  */
    OSTaskCreate((OS_TCB     *)&EXTIHandler_TCB,                               /* ������ƿ�ָ�� */
             (CPU_CHAR   *)"EXTIHandler",                                    /* ��������  */
             (OS_TASK_PTR )Task_EXTIHandler,                             /* �������ָ��  */
             (void       *)0,                                         // ���ݸ�����Ĳ���parg
             (OS_PRIO     )TASK_EXTIHandler_PRIO,                        // �������ȼ�
             (CPU_STK    *)&EXTIHandler_Stk[0],                        // �����ջ����ַ
             (CPU_STK_SIZE)TASK_EXTIHandler_STK_SIZE/10,           // ��ջʣ�ྯ����
             (CPU_STK_SIZE)TASK_EXTIHandler_STK_SIZE,                  // ��ջ��С
             (OS_MSG_QTY  )10,                                            // �ɽ��յ������Ϣ������
             (OS_TICK     )0,                                         // ʱ��Ƭ��תʱ��
             (void       *)0,                                         // ������ƿ���չ��Ϣ
             (OS_OPT      )(OS_OPT_TASK_STK_CHK | 
                            OS_OPT_TASK_STK_CLR),         // ����ѡ��
             (OS_ERR     *)&err);                                   // ����ֵ

    //������Ϣ��������  
    OSTaskCreate((OS_TCB     *)&MsgHandler_TCB,                      
             (CPU_CHAR   *)"MsgHandler",                      
             (OS_TASK_PTR )Task_MsgHandler,                   
             (void       *)0,                         
             (OS_PRIO     )TASK_MsgHandler_PRIO,              
             (CPU_STK    *)&MsgHandler_Stk[0],                
             (CPU_STK_SIZE)TASK_MsgHandler_STK_SIZE/10,       
             (CPU_STK_SIZE)TASK_MsgHandler_STK_SIZE,          
             (OS_MSG_QTY  )10,                            
             (OS_TICK     )0,                         
             (void       *)0,                         
             (OS_OPT      )(OS_OPT_TASK_STK_CHK | 
                            OS_OPT_TASK_STK_CLR),  
             (OS_ERR     *)&err);      

    
    
        OSTaskCreate((OS_TCB     *)&TIM2Hisr_TCB,                               // ������ƿ�ָ��          
                 (CPU_CHAR   *)"TIM2Hisr_TCB",                                    // ��������
                 (OS_TASK_PTR )Task_SoftTimerHisr,                             // �������ָ��
                 (void       *)0,                                         // ���ݸ�����Ĳ���parg
                 (OS_PRIO     )TASK_TIM2Hisr_PRIO,                        // �������ȼ�
                 (CPU_STK    *)&TIM2Hisr_Stk[0],                        // �����ջ����ַ
                 (CPU_STK_SIZE)TASK_TIM2Hisr_STK_SIZE/10,           // ��ջʣ�ྯ����
                 (CPU_STK_SIZE)TASK_TIM2Hisr_STK_SIZE,                  // ��ջ��С
                 (OS_MSG_QTY  )10,                                            // �ɽ��յ������Ϣ������
                 (OS_TICK     )0,                                         // ʱ��Ƭ��תʱ��
                 (void       *)0,                                         // ������ƿ���չ��Ϣ
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | 
                                OS_OPT_TASK_STK_CLR),         // ����ѡ��
                 (OS_ERR     *)&err);                                   // ����ֵ


}




















