#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "led.h"
#include "uart1.h"
#include "uart2.h"
#include "uart3.h"
#include "can.h"
#include "W25Q64.h"

int main(void)
{
	uint8_t MID;
	uint16_t DID;

	uint8_t ArrayWrite[] = {0x55, 0x66, 0x77, 0x88};
	uint8_t ArrayRead[4] = {0};
	LED_Init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// �����ж����ȼ�����2
	uart1_init(9600);
	
	W25Q64_Init();
	W25Q64_ReadID(&MID, &DID);
	
	W25Q64_SectorErase(0x000000);		// ������������ʼ��ַ
	W25Q64_PageProgram(0x000000, ArrayWrite, 4);	// д������
	
	W25Q64_ReadData(0x000000, ArrayRead, 4);		// ��ȡ����

	//uart2_init(9600);
	//uart3_init(9600);
	
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
				printf("The MID is %d \r\n",MID);
				printf("The DID is %d \r\n",DID);
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
		//GPIO_ResetBits(GPIOC, GPIO_Pin_13);
		//Delay_ms(500);
		//GPIO_SetBits(GPIOC, GPIO_Pin_13);
		//Delay_ms(500);
	}
}
