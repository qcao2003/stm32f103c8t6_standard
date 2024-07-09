#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "led.h"
#include "uart1.h"
#include "uart2.h"
#include "uart3.h"


int main(void)
{
	LED_Init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// �����ж����ȼ�����2
	uart1_init(256000);
	//uart2_init(9600);
	//uart3_init(9600);
	
	while (1)
	{
		//��ͷ
		Serial_SendByte(0x53);
		Serial_SendByte(0x5A);
		Serial_SendByte(0x48);
		Serial_SendByte(0x59);
		//�豸��ַ
		Serial_SendByte(0x00);
		//������
		Serial_SendByte(0x17);
		Serial_SendByte(0x00);
		Serial_SendByte(0x00);
		Serial_SendByte(0x00);
		//ָ��
		Serial_SendByte(0x01);
		//ͼ���ʽ
		Serial_SendByte(0x04);
		//��
		Serial_SendByte(0x40);
		Serial_SendByte(0x01);
		//��
		Serial_SendByte(0xF0);
		Serial_SendByte(0x00);
		//У�� CRC-16��У�� ��������ֵ
		Serial_SendByte(0xFF);
		Serial_SendByte(0xFF);
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
			
			if(strcmp(USART_RX_BUF,"Send\r\n") == 0)//�Ա�����
			{
				GPIO_ResetBits(GPIOC, GPIO_Pin_13);//����

				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//��������ڴ�
			}
			
			memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//��������ڴ�
			
			USART_RX_STA = 0;
		}
	}
}
