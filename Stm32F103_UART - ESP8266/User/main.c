#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "led.h"
#include "uart1.h"
#include "uart2.h"
#include "uart3.h"
#include "dht11.h"

int main(void)
{
	LED_Init();
	Delay_ms(20000);	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	uart1_init(9600);
	uart2_init(9600);
	//uart3_init(9600);
	u2_printf("AT+CIPMUX=1\r\n");
	Delay_ms(5000);

	u2_printf("AT+CIPSERVER=1,8080\r\n");
	Delay_ms(5000);
	GPIO_SetBits(GPIOC, GPIO_Pin_13); //关灯
	while (1)
	{

		if(USART2_RX_STA)
		{
			Delay_ms(1000);
			u2_printf("AT+CIPSEND=0,30\r\n");
			Delay_ms(1000);
			if(DHT_Read())
			{
				u2_printf("Humidity:%d.%d,Temperature:%d.%d\r\n",dat[0],dat[1],dat[2],dat[3]);
				Delay_ms(1000);
			}
			//u2_printf("ABC\r\n");
			memset(USART2_RX_BUF,0,sizeof(USART2_RX_BUF));//清空数组内存
			USART2_RX_STA = 0;
		}

	}
		//GPIO_ResetBits(GPIOC, GPIO_Pin_13);
		//Delay_ms(500);
		//GPIO_SetBits(GPIOC, GPIO_Pin_13);
		//Delay_ms(500);
		//u2_printf("USART2 is working\r\n"); //Uart2是可以发送的
}
