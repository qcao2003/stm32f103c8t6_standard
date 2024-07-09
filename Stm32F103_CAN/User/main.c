#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "led.h"
#include "uart1.h"
#include "uart2.h"
#include "uart3.h"
#include "exit.h"
#include "sja1000.h"

int main(void)
{
	LED_Init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	uart1_init(9600);

	while (1)
	{
		//sja1000_send_frame();
		
		//GPIO_ResetBits(GPIOC, GPIO_Pin_13);
		//Delay_ms(500);
		//GPIO_SetBits(GPIOC, GPIO_Pin_13);
		//Delay_ms(500);
	}
}
