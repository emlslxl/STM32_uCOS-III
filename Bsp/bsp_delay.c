#include "bsp_delay.h"


static u8  fac_us=0;//us��ʱ������			   
static u16 fac_ms=0;//ms��ʱ������,��ucos��,����ÿ�����ĵ�ms��


//��ʼ���ӳٺ���
//��ʹ��ucos��ʱ��,�˺������ʼ��ucos��ʱ�ӽ���
//SYSTICK��ʱ�ӹ̶�ΪHCLKʱ�ӵ�1/8
//SYSCLK:ϵͳʱ��
void delay_init(u8 SYSCLK)
{
#ifdef NVIC_PENDSVSET 	//���OS_CRITICAL_METHOD������,˵��ʹ��ucosII��.
	u32 reload;
#endif
 	SysTick->CTRL&=~(1<<2);	//SYSTICKʹ���ⲿʱ��Դ	 
	fac_us=SYSCLK/8;		//�����Ƿ�ʹ��ucos,fac_us����Ҫʹ��
	    
#ifdef NVIC_PENDSVSET 	//���OS_CRITICAL_METHOD������,˵��ʹ��ucosII��.
	reload=SYSCLK/8;		//ÿ���ӵļ������� ��λΪK	   
	reload*=1000000/OS_TICKS_PER_SEC;//����OS_TICKS_PER_SEC�趨���ʱ��
							//reloadΪ24λ�Ĵ���,���ֵ:16777216,��72M��,Լ��1.86s����	
	fac_ms=1000/OS_TICKS_PER_SEC;//����ucos������ʱ�����ٵ�λ	   
	SysTick->CTRL|=1<<1;   	//����SYSTICK�ж�
	SysTick->LOAD=reload; 	//ÿ1/OS_TICKS_PER_SEC���ж�һ��	
	SysTick->CTRL|=1<<0;   	//����SYSTICK    
#else
	fac_ms=(u16)fac_us*1000;//��ucos��,����ÿ��ms��Ҫ��systickʱ����   
#endif
}								    



#ifdef NVIC_PENDSVSET 	//���NVIC_PENDSVSET������,˵��ʹ��ucosIII��.
//��ʱnus
//nusΪҪ��ʱ��us��.		    								   
void delay_us(u32 nus)
{		
	OS_ERR err;
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;	//LOAD��ֵ	    	 
	ticks=nus*fac_us; 			//��Ҫ�Ľ�����	  		 
	tcnt=0;
	OSSchedLock(&err);				//��ֹucos���ȣ���ֹ���us��ʱ
	told=SysTick->VAL;        	//�ս���ʱ�ļ�����ֵ
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;//����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;//ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
		}  
	};
	OSSchedUnlock(&err);			//����ucos���� 									    
}


//��ʱnms
//nms:Ҫ��ʱ��ms��
void delay_ms(u16 nms)
{	
	OS_ERR err;
	if(OSRunning==OS_STATE_OS_RUNNING)//���os�Ѿ�������	    
	{		  
		if(nms>=fac_ms)//��ʱ��ʱ�����ucos������ʱ������ 
		{
   			OSTimeDly((OS_TICK )nms/fac_ms,//ucos��ʱ
                  (OS_OPT  )OS_OPT_TIME_DLY,
                  (OS_ERR *)&err);
		}
		nms%=fac_ms;			//ucos�Ѿ��޷��ṩ��ôС����ʱ��,������ͨ��ʽ��ʱ    
	}
	delay_us((u32)(nms*1000));	//��ͨ��ʽ��ʱ 
}
#else//����ucosʱ

//��ʱnus
//nusΪҪ��ʱ��us��.		    								   
void delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; //ʱ�����	  		 
	SysTick->VAL=0x00;        //��ռ�����
	SysTick->CTRL=0x01 ;      //��ʼ���� 	 
	do
	{
		temp=SysTick->CTRL;
	}
	while((temp&0x01)&&!(temp&(1<<16)));//�ȴ�ʱ�䵽��   
	SysTick->CTRL=0x00;       //�رռ�����
	SysTick->VAL =0X00;       //��ռ�����	 
}
//��ʱnms
//ע��nms�ķ�Χ
//SysTick->LOADΪ24λ�Ĵ���,����,�����ʱΪ:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK��λΪHz,nms��λΪms
//��72M������,nms<=1864 
void delay_ms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;//ʱ�����(SysTick->LOADΪ24bit)
	SysTick->VAL =0x00;           //��ռ�����
	SysTick->CTRL=0x01 ;          //��ʼ����  
	do
	{
		temp=SysTick->CTRL;
	}
	while((temp&0x01)&&!(temp&(1<<16)));//�ȴ�ʱ�䵽��   
	SysTick->CTRL=0x00;       //�رռ�����
	SysTick->VAL =0X00;       //��ռ�����	  	    
} 
#endif
			 

/* ���뼶��ȷ��ʱ */
__asm void
SysCtlDelay(unsigned long ulCount)
{
    subs    r0, #1;
    bne     SysCtlDelay;
    bx      lr;
}

void DelayUS(uint32_t delay)
{
  SysCtlDelay(delay*(SystemCoreClock/6000000));
}

void DelayMS(uint32_t delay) 
{
  SysCtlDelay(delay*(SystemCoreClock/6000));
}


void delayUs(uint32 us)
{
    uint32 k;
    k = 10*us;
    while(k--);
}


void delayMs(uint32 ms)
{
    uint32 k;
    k = 10000*ms;
    while(ms--);

}

















