/******************************************************************************

                  版权所有 (C), 2001-2011, 神州数码网络有限公司

 ******************************************************************************
  文 件 名   : bsp_pulsecnt.c
  版 本 号   : 初稿
  作    者   : lxl
  生成日期   : 2015年4月8日
  最近修改   :
  功能描述   : 脉冲频率检测，使用定时器的输入捕获功能
  函数列表   :
  修改历史   :
  1.日    期   : 2015年4月8日
    作    者   : lxl
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/
S_TIM_CAP gTIM3;

/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/

 
 /*****************************************************************************
  函 数 名	: TIM3_Cap_Init
  功能描述	: TIM3输入捕获功能初始化
  输入参数	: u16 arr  
			  u16 psc  
  输出参数	: 无
  返 回 值	: void
  调用函数	: 
  被调函数	: 
  
  修改历史		:
   1.日    期	: 2015年4月3日
	 作    者	: lxl
	 修改内容	: 新生成函数
 
 *****************************************************************************/
  void TIM3_Cap_Init(u16 arr,u16 psc)
  { 	   
	  RCC->APB1ENR|=1<<1;	  //TIM3 时钟使能 
	  RCC->APB2ENR|=1<<3;	  //使能PORTB时钟  
	   
	  GPIOB->CRL&=0XFFFFFFF0; //PB0 清除之前设置  
	  GPIOB->CRL|=0X00000008; //PB0 输入   
	  GPIOB->ODR|=0<<0; 	  //PB0 下拉
		
	  TIM3->ARR=arr;		  //设定计数器自动重装值   
	  TIM3->PSC=psc;		  //预分频器 
  
	  TIM3->CCMR2|=1<<0;	  //CC3S=01   选择输入端 IC3映射到TI3上
	  TIM3->CCMR2|=0x4<<4;	  //IC3F=0000 配置输入滤波器 不滤波
	  TIM3->CCMR2|=0<<2;	 //IC3PS=00  配置输入分频,不分频 
  
	  TIM3->CCER|=0<<9; 	  //CC3P=0	  上升沿捕获
	  TIM3->CCER|=1<<8; 	  //CC3E=1	  允许捕获计数器的值到捕获寄存器中
  
	  TIM3->DIER|=1<<3; 	  //CC3IE允许捕获中断			  
	  TIM3->DIER|=1<<0; 	  //允许更新中断  
	  TIM3->CR1|=0x01;		  //使能定时器
  }
 
 
  
 
/*****************************************************************************
  函 数 名	: TIM3_IRQHandler
  功能描述	: TIM3 的3通道进行频率捕获，捕获时间长度为两个上升沿之间，即一个
			  完整周期的长度
  输入参数	: void	
  输出参数	: 无
  返 回 值	: void
  调用函数	: 
  被调函数	: 
  
  修改历史		:
   1.日    期	: 2015年4月3日
	 作    者	: lxl
	 修改内容	: 新生成函数
 
*****************************************************************************/
void TIM3_IRQHandler(void)
{ 		  
  u16 tsr;
  
  tsr=TIM3->SR;
  if((gTIM3.CapState[CHANNEL_3]&0X80)==0)//还未成功捕获 
  {
	  if(tsr&0X01)//溢出
	  { 	  
		  if(gTIM3.CapState[CHANNEL_3]&0X40)//捕获已经开始
		  {
			  if((gTIM3.CapState[CHANNEL_3]&0X3F)==0X3F)//高电平太长了
			  {
				  gTIM3.CapState[CHANNEL_3]|=0X80;//标记成功捕获了一次
				  gTIM3.CapValue[CHANNEL_3]=0XFFFF;
			  }else 
			  {
				 gTIM3.CapState[CHANNEL_3]++;
			  }
		  }    
	  }
	  if(tsr&0x08)//捕获3发生捕获事件
	  {   
		  if(gTIM3.CapState[CHANNEL_3]&0X40) //捕获到第二个上升沿，捕获结束   
		  { 			  
			 gTIM3.CapState[CHANNEL_3]&=~0X40;	 //等待开启下次上升沿捕获
			  gTIM3.CapState[CHANNEL_3]|=0X80;	  //标记成功捕获到一个脉冲周期
			  gTIM3.CapValue[CHANNEL_3]=TIM3->CCR3; //获取当前的捕获值.
		  }else  //第一次捕获上升沿
		  {
			  gTIM3.CapState[CHANNEL_3]=0;		  //清空
			  gTIM3.CapValue[CHANNEL_3]=0;
			  gTIM3.CapState[CHANNEL_3]|=0X40;	  //标记捕获捕获开始
			  //TIM3->CNT = 0;
			  gTIM3.FirstCapValue[CHANNEL_3]=TIM3->CCR3;
		  } 		  
	  } 											 
  }

  TIM3->SR=0;//清除中断标志位		  
}
 
 
u16 TIM3_GetFreqValue(u16 Channel)
{
	 u16 freqValue;
	 u32 temp,cnt;

	 //开启下一次捕获
	 gTIM3.CapState[2]=0;	 
	 gTIM3.CapValue[2] = 0;
	 gTIM3.FirstCapValue[2] = 0;

	 while(!(gTIM3.CapState[2]&0X80));

	 if(gTIM3.CapState[CHANNEL_3]&0X3F)
	 {
		 temp = 65536 - gTIM3.FirstCapValue[CHANNEL_3] + gTIM3.CapValue[CHANNEL_3];
		 cnt=(gTIM3.CapState[CHANNEL_3]&0X3F) - 1;
	 }
	 else
	 {
		 cnt = 0;
		 temp = gTIM3.CapValue[CHANNEL_3] - gTIM3.FirstCapValue[CHANNEL_3];
	 }

	 //溢出时间总和
	 cnt*=65536;				 
	 //得到总的采样平时间
	 temp+=cnt; 	 
	 //printf(" 脉冲宽度 = %d\r\n",temp);
	 //printf(" 脉冲频率 = %d\r\n",1000000/(temp-10));
	 freqValue = 1000000/(temp-10);
	 
	 return freqValue;
}


void TIM3_CAP_TEST()
{
 u32 cnt = 0;
 u32 temp;
 
 printf(" 脉冲频率 = %d\r\n",TIM3_GetFreqValue(CHANNEL_3));
#if 0	
 if(gTIM3.CapState[CHANNEL_3]&0X80)//成功捕获了一次
 {
	 if(gTIM3.CapState[CHANNEL_3]&0X3F)
	 {
		 temp = 65536 - gTIM3.FirstCapValue[CHANNEL_3] + gTIM3.CapValue[CHANNEL_3];
		 cnt=(gTIM3.CapState[CHANNEL_3]&0X3F) - 1;
	 }
	 else
	 {
		 cnt = 0;
		 temp = gTIM3.CapValue[CHANNEL_3] - gTIM3.FirstCapValue[CHANNEL_3];
	 }

	 //溢出时间总和
	 cnt*=65536;				 
	 //得到总的采样平时间
	 temp+=cnt; 	 
	 printf(" 脉冲宽度 = %d\r\n",temp);
	 printf(" 脉冲频率 = %d\r\n",1000000/(temp-10));

	 //开启下一次捕获
	 gTIM3.CapState[CHANNEL_3]=0;			 
		 
 }
#endif

}


 

