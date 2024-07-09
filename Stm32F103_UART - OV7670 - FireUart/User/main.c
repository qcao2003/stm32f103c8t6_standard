#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "led.h"
#include "uart1.h"
#include "uart2.h"
#include "uart3.h"
#include "OV7670.h"
#include "SCCB.h"
#include "exti.h"

int main(void)
{
	LED_Init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);// 设置中断优先级分组3
	uart1_init(460800);
	//uart2_init(9600);
	//uart3_init(9600);
	//初始化OV7670
	while(OV7670_Init())
	{
		printf("OV7670_Init ERROR\r\n");
	}
	EXTI_InitPA15();
	Send_Fired_Uart_init();
	while (1)
	{
		OV7670_STM32GetPic(240,320);
	}
}
