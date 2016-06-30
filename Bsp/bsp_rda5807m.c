#include "bsp_rda5807m.h"



//#define delayms(ms) Delay_us(ms)

#define RDA5807M_SLAVE_ADDRESS 0x20
uint8_t temp; // ����


// RDA5807 �Ĵ���  д
uint8_t RDA_reg_data[8] =
{
   0xd0,0x00,  // 02H
   0x00,0x00,  // 03H
   0x00,0x40,  // 04H
   0x90,0x88,  // 05H
};

/**********************************************************

 ����д�Ĵ����Ӻ���

**********************************************************/
void RDA5807_write_reg(void)
{
	uint8_t i;
	uint8_t ucAck;
	
	i2c_Start();
	
	/* �����豸��ַ+��д����bit��0 = w�� 1 = r) bit7 �ȴ� */
	i2c_SendByte(RDA5807M_SLAVE_ADDRESS | I2C_WR);
	
	/* ���ACK */
    ucAck = i2c_WaitAck();
	if (ucAck == 1)
	{
		goto err_quit;
	}
	
	// �Ĵ�������д����
	for(i=0; i<8; i++)
	{
		i2c_SendByte(RDA_reg_data[i]);
		
		/* ���ACK */
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

 �������Ĵ����Ӻ���

**********************************************************/
void RDA5807_read_reg(uint8_t *reg_buf)
{
	uint8_t i;
	uint8_t ucAck;

	i2c_Start();

	/* �����豸��ַ+��д����bit��0 = w�� 1 = r) bit7 �ȴ� */
	i2c_SendByte(RDA5807M_SLAVE_ADDRESS | I2C_RD);

	/* ���ACK */
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

 ģ���ϵ��ʼ���Ӻ���

**********************************************************/
void RDA5807_power(void)
{
	delayMs(50);
	
	    // ���������λָ��
   RDA_reg_data[0] = 0x00;
   RDA_reg_data[1] = 0x02;
   RDA5807_write_reg();

   delayMs(10);

    // ����ģ��Ĭ�ϲ���
   RDA_reg_data[0] = 0xd0;
   RDA_reg_data[1] = 0x01;
   RDA5807_write_reg();
}

/**********************************************************

 ��������������ģ���Զ�Ѱ̨ģʽ

**********************************************************/
void RDA5807_FM_seek(void)
{
   uint16_t chan;
   uint8_t  reg_data[4] = {0x00, 0x00, 0x00, 0x00};

   RDA_reg_data[3] &= ~(1 << 4);      //��г����

   // �ڲ��Զ�Ѱ̨ʹ��
   RDA_reg_data[0] |=  (1 << 0);      //SEEKλ��1
   RDA5807_write_reg();

    // �ȴ�STC ��־��λ
   while(0 == (reg_data[0] & 0x40))
   {
     delayMs(40);
      // ��ȡ�ڲ�״̬
     RDA5807_read_reg(reg_data);
	 
   }
    // ��ȡ��ǰ����Ƶ��
   chan = reg_data[0] & 0x03;
   chan = reg_data[1] | (chan << 8);
   
   printf("\r\n Ѱ̨�ɹ� \r\n");
   printf("��ǰ����Ƶ��Ϊ��%d" , chan);

    // ���浱ǰ����Ƶ��
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
   
   printf("��ǰƵ��Ϊ : %d\r\n" , chan);

}

/**********************************************************

 ��������������ģ�����

**********************************************************/
void RDA5807_FM_Test(void)
{

	printf("\r\n rda5807m ����ʵ�� \r\n");
	
	if ( i2c_CheckDevice(RDA5807M_SLAVE_ADDRESS) == 0 )
	{
		printf("\r\n rda5807m ���ɹ� \r\n");
	}
	else
	{
		printf("\r\n rda5807m ��ⲻ�ɹ� \r\n");
	}
	
	RDA5807_power();
	
	printf("\r\n �ϵ�ɹ� \r\n");
	
	RDA_reg_data[7] = 131;	 // 05H�Ĵ�������bit �ı�����
    RDA5807_write_reg();
	
	temp = RDA_reg_data[7] & 0xff; //ȡ����ֵ	
	printf("��ǰ����Ϊ%d" , temp);
	
	RDA_reg_data[0] |= (1 << 1); 	//SEEK UP	
	//RDA5807_FM_seek();              //�Զ�Ѱ̨

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

