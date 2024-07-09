#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "led.h"
#include "uart1.h"
#include "uart2.h"
#include "uart3.h"

const char* CIPMUX = "AT+CIPMUX=1\r\n";
const char* CIPSERVER = "AT+CIPSERVER=1,8080\r\n";

int main(void)
{
	LED_Init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// �����ж����ȼ�����2
	uart1_init(9600);
	uart2_init(9600);
	//uart3_init(9600);
	Delay_ms(20000);
	//printf("%s",CIPMUX);
	printf("AT+CIPMUX=1\r\n");
	Delay_ms(5000);
	//printf("%s",CIPSERVER);
	printf("AT+CIPSERVER=1,8080\r\n");
	Delay_ms(5000);
	
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
				Delay_ms(500);
				u2_printf("%s",CIPMUX);
				Delay_ms(1000);
				if(USART2_RX_STA)
				{
					Delay_ms(500);
					printf("%s",USART2_RX_BUF);
					memset(USART2_RX_BUF,0,sizeof(USART2_RX_BUF));//��������ڴ�
					USART2_RX_STA = 0;
				}
				Delay_ms(500);
				u2_printf("%s",CIPSERVER);
				Delay_ms(1000);
				if(USART2_RX_STA)
				{
					Delay_ms(500);
					printf("%s",USART2_RX_BUF);
					memset(USART2_RX_BUF,0,sizeof(USART2_RX_BUF));//��������ڴ�
					USART2_RX_STA = 0;
				}
				Delay_ms(1000);
				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//��������ڴ�
			}
				
			
			if(strcmp(USART_RX_BUF,"LED_ON\r\n") == 0)//�Ա�����
			{
				GPIO_ResetBits(GPIOC, GPIO_Pin_13);//����
				printf("ON\r\n");
				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//��������ڴ�
			}
			
			memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//��������ڴ�
			USART_RX_STA = 0;
		}
		if(USART2_RX_STA)
		{	
			Delay_ms(500);
			u2_printf("%s",USART2_RX_BUF);
			if(strcmp(USART2_RX_BUF,"LED_ON\r\n") == 0)//�Ա�����
			{
				GPIO_ResetBits(GPIOC, GPIO_Pin_13);//����
				u2_printf("USART2_ON\r\n");
				memset(USART2_RX_BUF,0,sizeof(USART2_RX_BUF));//��������ڴ�
			}
			
			if(strcmp(USART2_RX_BUF,"LED_OFF\r\n") == 0)//�Ա�����
			{
				GPIO_SetBits(GPIOC, GPIO_Pin_13);//�ص�
				u2_printf("USART2_OFF\r\n");
				memset(USART2_RX_BUF,0,sizeof(USART2_RX_BUF));//��������ڴ�
			}
			
			memset(USART2_RX_BUF,0,sizeof(USART2_RX_BUF));//��������ڴ�
			USART2_RX_STA = 0;
		}
			
	}
		//GPIO_ResetBits(GPIOC, GPIO_Pin_13);
		//Delay_ms(500);
		//GPIO_SetBits(GPIOC, GPIO_Pin_13);
		//Delay_ms(500);
		//u2_printf("USART2 is working\r\n"); //Uart2�ǿ��Է��͵�
}
