#include "exti.h"



void EXTI_InitPA15(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);

	/* 配置EXTI_Line15 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line15;						//LINE15
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;				//中断事件
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; 			//上升沿触发 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;							//使能LINE15
	EXTI_Init(&EXTI_InitStructure);//配置

	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;//外部中断10-15
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);//配置
}


void NVIC_Close_Configuration(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;//外部中断10-15
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);//配置
}

void NVIC_Open_Configuration(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;//外部中断10-15
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);//配置
}

