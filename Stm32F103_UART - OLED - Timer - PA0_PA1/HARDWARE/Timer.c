#include "stm32f10x.h"                  // Device header
 
void Timer_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//TIM2挂在APB2上
	
	TIM_InternalClockConfig(TIM2);//使用RCC内部时钟72MHZ
	
	//定时器初始化
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;  //  是否划分72MHZ时钟频率
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计次
	TIM_TimeBaseInitStructure.TIM_Period = 10000 - 1;  //  自动重装器的值  最大为65535
	TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1;  //预分频系数  最大为65535
	//  1/(72000000/7200-1+1)*10000=1s  也就是说每一秒进一次中断
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;  //是否重复计数
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);  //清除标志位
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);  //启用或者禁用定时器中断，中断输出控制模块：开启更新中断到NVIC的通路
	//通过调用TIM_ITConfig()函数配置定时器中断类型后，只是使能了定时器内部的中断触发，但是并没有使能外部中断，即没有将中断请求发送到NVIC。
	//要使能外部中断，需要在NVIC中使能对应的中断线，这样当定时器中断触发时，中断请求就会被发送到NVIC，
	//NVIC再根据中断优先级和抢占规则来决定是否将中断请求发送到CPU，进而执行中断处理函数。
	
	//NVIC中断优先级设置
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	//定时器使能
	TIM_Cmd(TIM2, ENABLE);
}
 
/*
void TIM2_IRQHandler(void)  //TM2的中断函数
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
*/

