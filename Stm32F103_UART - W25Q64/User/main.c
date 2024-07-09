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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	uart1_init(9600);
	
	W25Q64_Init();
	W25Q64_ReadID(&MID, &DID);
	
	W25Q64_SectorErase(0x000000);		// 擦除扇区的起始地址
	W25Q64_PageProgram(0x000000, ArrayWrite, 4);	// 写入数据
	
	W25Q64_ReadData(0x000000, ArrayRead, 4);		// 读取数据

	//uart2_init(9600);
	//uart3_init(9600);
	
	while (1)
	{
		if(USART_RX_STA)
		{
			Delay_ms(500);
			printf("%s",USART_RX_BUF);
			if(strcmp(USART_RX_BUF,"LED_OFF\r\n") == 0)//对比数组
			{
				GPIO_SetBits(GPIOC, GPIO_Pin_13);//关灯
				printf("OFF\r\n");
				printf("The MID is %d \r\n",MID);
				printf("The DID is %d \r\n",DID);
				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//清空数组内存
			}
				
			
			if(strcmp(USART_RX_BUF,"LED_ON\r\n") == 0)//对比数组
			{
				GPIO_ResetBits(GPIOC, GPIO_Pin_13);//开灯
				printf("ON\r\n");
				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//清空数组内存
			}
			
			memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//清空数组内存
			
			USART_RX_STA = 0;
		}
		//GPIO_ResetBits(GPIOC, GPIO_Pin_13);
		//Delay_ms(500);
		//GPIO_SetBits(GPIOC, GPIO_Pin_13);
		//Delay_ms(500);
	}
}
