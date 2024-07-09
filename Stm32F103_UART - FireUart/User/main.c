#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "led.h"
#include "uart1.h"
#include "uart2.h"
#include "uart3.h"


int main(void)
{
	LED_Init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	uart1_init(256000);
	//uart2_init(9600);
	//uart3_init(9600);
	
	while (1)
	{
		//包头
		Serial_SendByte(0x53);
		Serial_SendByte(0x5A);
		Serial_SendByte(0x48);
		Serial_SendByte(0x59);
		//设备地址
		Serial_SendByte(0x00);
		//包长度
		Serial_SendByte(0x17);
		Serial_SendByte(0x00);
		Serial_SendByte(0x00);
		Serial_SendByte(0x00);
		//指令
		Serial_SendByte(0x01);
		//图像格式
		Serial_SendByte(0x04);
		//宽
		Serial_SendByte(0x40);
		Serial_SendByte(0x01);
		//高
		Serial_SendByte(0xF0);
		Serial_SendByte(0x00);
		//校验 CRC-16不校验 可以任意值
		Serial_SendByte(0xFF);
		Serial_SendByte(0xFF);
		if(USART_RX_STA)
		{
			Delay_ms(500);
			printf("%s",USART_RX_BUF);
			if(strcmp(USART_RX_BUF,"LED_OFF\r\n") == 0)//对比数组
			{
				GPIO_SetBits(GPIOC, GPIO_Pin_13);//关灯
				printf("OFF\r\n");
				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//清空数组内存
			}
				
			
			if(strcmp(USART_RX_BUF,"LED_ON\r\n") == 0)//对比数组
			{
				GPIO_ResetBits(GPIOC, GPIO_Pin_13);//开灯
				printf("ON\r\n");
				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//清空数组内存
			}
			
			if(strcmp(USART_RX_BUF,"Send\r\n") == 0)//对比数组
			{
				GPIO_ResetBits(GPIOC, GPIO_Pin_13);//开灯

				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//清空数组内存
			}
			
			memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//清空数组内存
			
			USART_RX_STA = 0;
		}
	}
}
