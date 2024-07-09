#include "can.h"

uint16_t adIOPin[] = {GPIO_Pin_0,GPIO_Pin_1,GPIO_Pin_2,GPIO_Pin_3,GPIO_Pin_4,GPIO_Pin_5,GPIO_Pin_6,GPIO_Pin_7};
uint8_t can_filter[8] = {0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff};
uint8_t msg[10] = {0x00,0xE8,0x01,0x03,0x04,0x05,0x06,0x07,0x08,0x09};
void adio_out_change(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void adio_in_change(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void PA0_7_init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = 0XFF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void sja1000_IO_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_15; //PA12 SJA1000_CS PA15 SAJ1000_ALE
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA12
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5; //PB3 SJA1000_WR PB4 SAJ1000_RD PB5 SJA1000_RESET
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	
}


void sja1000_write_cmd(uint8_t addr, uint8_t cmd) //can 写寄存器数据
{
	uint8_t i;
	
	adio_out_change(); //切换数据io为输出
 
	CAN_ALE_H;  //写命令地址
	for(i=0; i<8; i++)
	{
		if(addr & 0x80)
			GPIO_SetBits(CAN_AD_Port, adIOPin[i]);
		else
			GPIO_ResetBits(CAN_AD_Port, adIOPin[i]);
		
		addr <<= 1;
	}
	CAN_ALE_L;
 
	CAN_CS_L; //写命令
	CAN_WR_L;
	for(i=0; i<8; i++)
	{
		if(cmd & 0x80)
			GPIO_SetBits(CAN_AD_Port, adIOPin[i]);
		else
			GPIO_ResetBits(CAN_AD_Port, adIOPin[i]);
		
		cmd <<= 1;
	}
	CAN_WR_H;
	CAN_CS_H;
}

uint8_t sja1000_read_byte(uint8_t addr) //can 读寄存器数据
{
	uint8_t i,data=0;
	
	adio_out_change(); //切换数据io为输出
 
	CAN_ALE_H; //写命令地址
	for(i=0; i<8; i++)
	{
		if(addr & 0x80)
			GPIO_SetBits(CAN_AD_Port, adIOPin[i]);
		else
			GPIO_ResetBits(CAN_AD_Port, adIOPin[i]);
		
		addr <<= 1;
	}
	CAN_ALE_L;
		
	adio_in_change();	//切换数据io为输入
	
	CAN_CS_L; //读8位数据
	CAN_RD_L;
	for(i=0; i<8; i++)
	{
		data <<= 1;
		
		if(GPIO_ReadInputDataBit(CAN_AD_Port,adIOPin[i]))
			data++;	
	}
	CAN_RD_H;
	CAN_CS_H;
	
	return data;
}


void sja1000_reg_Init(void) 
{

	sja1000_IO_init();
	CAN_RESET_L; 
	CAN_ALE_L;
	CAN_WR_H;
	CAN_RD_H;
	CAN_CS_H;
	Delay_ms(1);
	CAN_RESET_H;  
	
	sja1000_write_cmd(0x00, 0x01); // 复位模式
	sja1000_write_cmd(0x06, 0x00); // 配置总线定时器 0
	sja1000_write_cmd(0x07, 0x1C); // 配置总线定时器 1
	sja1000_write_cmd(0x08, 0xFA); // 配置输出管脚为 TX0 与 RX0，推挽输出
	sja1000_write_cmd(0x04, 0x00); // 验收
	sja1000_write_cmd(0x05, 0xFF); // 屏蔽
	sja1000_write_cmd(0x00, 0x02); // 工作模式+接收中断
	printf("sja1000_Inited\r\n");
}

/*	CMR 命令 addr 0x01
		CMR.4 自接收 1/0 
*/

void sja1000_send_frame(void)
{
	uint8_t i;
	msg[2]++;
	for(i=0;i<10;i++)
	{
		sja1000_write_cmd(0x0A+i,msg[i]); //发送缓冲区
	}
	sja1000_write_cmd(0x01,0x01);  //发送请求
}
