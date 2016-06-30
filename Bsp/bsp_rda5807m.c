#include "bsp_rda5807m.h"



//#define delayms(ms) Delay_us(ms)

#define RDA5807M_SLAVE_ADDRESS 0x20
uint8_t temp; // 音量


// RDA5807 寄存器  写
uint8_t RDA_reg_data[8] =
{
   0xd0,0x00,  // 02H
   0x00,0x00,  // 03H
   0x00,0x40,  // 04H
   0x90,0x88,  // 05H
};

/**********************************************************

 连续写寄存器子函数

**********************************************************/
void RDA5807_write_reg(void)
{
	uint8_t i;
	uint8_t ucAck;
	
	i2c_Start();
	
	/* 发送设备地址+读写控制bit（0 = w， 1 = r) bit7 先传 */
	i2c_SendByte(RDA5807M_SLAVE_ADDRESS | I2C_WR);
	
	/* 检测ACK */
    ucAck = i2c_WaitAck();
	if (ucAck == 1)
	{
		goto err_quit;
	}
	
	// 寄存器连续写操作
	for(i=0; i<8; i++)
	{
		i2c_SendByte(RDA_reg_data[i]);
		
		/* 检测ACK */
		ucAck = i2c_WaitAck();
		if (ucAck == 1)
		{
			goto err_quit;
		}
	}
	
err_quit:
	i2c_Stop();
}

/**********************************************************

 连续读寄存器子函数

**********************************************************/
void RDA5807_read_reg(uint8_t *reg_buf)
{
	uint8_t i;
	uint8_t ucAck;

	i2c_Start();

	/* 发送设备地址+读写控制bit（0 = w， 1 = r) bit7 先传 */
	i2c_SendByte(RDA5807M_SLAVE_ADDRESS | I2C_RD);

	/* 检测ACK */
	ucAck = i2c_WaitAck();
	if (ucAck == 1)
	{
		goto err_quit;
	}

	for (i = 0; i < 4; i++)
	{
		reg_buf[i] = i2c_ReadByte();
		if (i == 3)
		{
			i2c_NAck();
		}
		else
		{                        
			i2c_Ack();
		}
	}

err_quit:
	i2c_Stop();
}

/**********************************************************

 模块上电初始化子函数

**********************************************************/
void RDA5807_power(void)
{
	delayMs(50);
	
	    // 发送软件复位指令
   RDA_reg_data[0] = 0x00;
   RDA_reg_data[1] = 0x02;
   RDA5807_write_reg();

   delayMs(10);

    // 收音模块默认参数
   RDA_reg_data[0] = 0xd0;
   RDA_reg_data[1] = 0x01;
   RDA5807_write_reg();
}

/**********************************************************

 功能描述：收音模块自动寻台模式

**********************************************************/
void RDA5807_FM_seek(void)
{
   uint16_t chan;
   uint8_t  reg_data[4] = {0x00, 0x00, 0x00, 0x00};

   RDA_reg_data[3] &= ~(1 << 4);      //调谐禁用

   // 内部自动寻台使能
   RDA_reg_data[0] |=  (1 << 0);      //SEEK位置1
   RDA5807_write_reg();

    // 等待STC 标志置位
   while(0 == (reg_data[0] & 0x40))
   {
     delayMs(40);
      // 读取内部状态
     RDA5807_read_reg(reg_data);
	 
   }
    // 获取当前工作频点
   chan = reg_data[0] & 0x03;
   chan = reg_data[1] | (chan << 8);
   
   printf("\r\n 寻台成功 \r\n");
   printf("当前工作频点为：%d" , chan);

    // 保存当前工作频点
   RDA_reg_data[2] = (chan >> 8) & 0xff;
   RDA_reg_data[3] = (chan & 0xff);
}


void RDA5807_SetFreq(uint32 freq)
{
    uint8_t  reg_data[4] = {0x00, 0x00, 0x00, 0x00};
    uint16_t chan;

    chan = freq - 870;
    RDA_reg_data[2] = (chan >> 2);
    RDA_reg_data[3] &= ~(0x3 << 6);
    RDA_reg_data[3] |= ((chan & 0x3) << 6);
    RDA_reg_data[3] |= (1 << 4);     
	
   RDA5807_write_reg();

   while(0 == (reg_data[0] & 0x40))
   {
     delayMs(40);
     RDA5807_read_reg(reg_data);
   }

   chan = reg_data[0] & 0x03;
   chan = reg_data[1] | (chan << 8);
   
   printf("当前频点为 : %d\r\n" , chan);

}

/**********************************************************

 功能描述：收音模块测试

**********************************************************/
void RDA5807_FM_Test(void)
{

	printf("\r\n rda5807m 测试实验 \r\n");
	
	if ( i2c_CheckDevice(RDA5807M_SLAVE_ADDRESS) == 0 )
	{
		printf("\r\n rda5807m 检测成功 \r\n");
	}
	else
	{
		printf("\r\n rda5807m 检测不成功 \r\n");
	}
	
	RDA5807_power();
	
	printf("\r\n 上电成功 \r\n");
	
	RDA_reg_data[7] = 131;	 // 05H寄存器低四bit 改变音量
    RDA5807_write_reg();
	
	temp = RDA_reg_data[7] & 0xff; //取音量值	
	printf("当前音量为%d" , temp);
	
	RDA_reg_data[0] |= (1 << 1); 	//SEEK UP	
	//RDA5807_FM_seek();              //自动寻台

	RDA5807_SetFreq(876);
}

uint16 RDA5807_ReadRSSI()
{
    uint8 read_buff[4] = {0};
    RDA5807_read_reg(&read_buff[0]);

    //return (read_buff[2] >> 1);

    printf("RSSI = %d\r\n",(read_buff[2] >> 1));
    printf("reg[0] = 0x%x\r\n",read_buff[0]);
    printf("reg[1] = 0x%x\r\n",read_buff[1]);
    printf("reg[2] = 0x%x\r\n",read_buff[2]);
    printf("reg[3] = 0x%x\r\n",read_buff[3]);
    
}



/*********************************************END OF FILE**********************/

