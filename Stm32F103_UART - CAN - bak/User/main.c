#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "led.h"
#include "uart1.h"
#include "can.h"
#include "stdio.h"	
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

char* msg;
u8* reviced;

int main(void)
{
	volatile u8 res,len;
	LED_Init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	uart1_init(9600);
	

	
	
	msg = "ABC\r\n";
	while (1)
	{
		if(USART_RX_STA)
		{
			Delay_ms(500);
			printf("%s",USART_RX_BUF);
			if(strcmp(USART_RX_BUF,"Send\r\n") == 0)//对比数组
			{
				GPIO_SetBits(GPIOC, GPIO_Pin_13);//关灯
				CAN_Configuration();
				CAN_SendMessage(0xAA);
				CAN_ReceiveMessage();
				printf("Send\r\n");
				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//清空数组内存
			}
				
			
			if(strcmp(USART_RX_BUF,"change\r\n") == 0)//对比数组
			{
				GPIO_ResetBits(GPIOC, GPIO_Pin_13);//开灯
				printf("change\r\n");
				msg = "BCD\r\n";
				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//清空数组内存
			}
			
			if(strcmp(USART_RX_BUF,"show\r\n") == 0)//对比数组
			{
				GPIO_ResetBits(GPIOC, GPIO_Pin_13);//开灯
				printf("Can_Recevie len is %d\r\n",len);
				printf("%s\r\n",reviced);
				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//清空数组内存
			}
			
			memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//清空数组内存
			USART_RX_STA = 0;
		}
		Delay_ms(500);
	}
}
