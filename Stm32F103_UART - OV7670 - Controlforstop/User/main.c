#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "led.h"
#include "uart1.h"
#include "OV7670.h"
#include "SCCB.h"
#include "exti.h"
#include <stdio.h>

uint8_t OV7670_State=0;

int main(void)
{
	LED_Init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);// �����ж����ȼ�����3
	uart1_init(256000);
	//uart2_init(9600);
	//uart3_init(9600);
	OV7670_Init();
	EXTI_InitPA15();
	while (1)
	{
		if(USART_RX_STA)
		{
			Delay_ms(500);
			printf("%s",USART_RX_BUF);
			if(strcmp(USART_RX_BUF,"LED_OFF\r\n") == 0)//�Ա�����
			{
				GPIO_SetBits(GPIOC, GPIO_Pin_13);//�ص�
				printf("OFF\r\n");

				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//��������ڴ�
			}
				
			
			if(strcmp(USART_RX_BUF,"LED_ON\r\n") == 0)//�Ա�����
			{
				GPIO_ResetBits(GPIOC, GPIO_Pin_13);//����
				printf("ON\r\n");
				
				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//��������ڴ�
			}
			
			if(strcmp(USART_RX_BUF,"Get_pic\r\n") == 0)//�Ա�����
			{
				GPIO_ResetBits(GPIOC, GPIO_Pin_13);//����
				printf("ON\r\n");
				OV7670_STM32GetPic(240,320);
				printf("Pic is completed\r\n");
				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//��������ڴ�
			}
			
			memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//��������ڴ�
			
			USART_RX_STA = 0;
		}
		OV7670_FIFOGetPic();//��һ�ν��뿪��FIFO
		Delay_ms(50);
		OV7670_FIFOGetPic();//�ٽ���һ�ιر�FIFO
		Delay_ms(50);
		OV7670_STM32GetPic(320,240);
	}
}
