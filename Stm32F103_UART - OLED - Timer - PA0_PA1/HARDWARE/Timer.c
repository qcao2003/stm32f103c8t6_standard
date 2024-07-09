#include "stm32f10x.h"                  // Device header
 
void Timer_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//TIM2����APB2��
	
	TIM_InternalClockConfig(TIM2);//ʹ��RCC�ڲ�ʱ��72MHZ
	
	//��ʱ����ʼ��
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;  //  �Ƿ񻮷�72MHZʱ��Ƶ��
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼƴ�
	TIM_TimeBaseInitStructure.TIM_Period = 10000 - 1;  //  �Զ���װ����ֵ  ���Ϊ65535
	TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1;  //Ԥ��Ƶϵ��  ���Ϊ65535
	//  1/(72000000/7200-1+1)*10000=1s  Ҳ����˵ÿһ���һ���ж�
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;  //�Ƿ��ظ�����
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);  //�����־λ
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);  //���û��߽��ö�ʱ���жϣ��ж��������ģ�飺���������жϵ�NVIC��ͨ·
	//ͨ������TIM_ITConfig()�������ö�ʱ���ж����ͺ�ֻ��ʹ���˶�ʱ���ڲ����жϴ��������ǲ�û��ʹ���ⲿ�жϣ���û�н��ж������͵�NVIC��
	//Ҫʹ���ⲿ�жϣ���Ҫ��NVIC��ʹ�ܶ�Ӧ���ж��ߣ���������ʱ���жϴ���ʱ���ж�����ͻᱻ���͵�NVIC��
	//NVIC�ٸ����ж����ȼ�����ռ�����������Ƿ��ж������͵�CPU������ִ���жϴ�������
	
	//NVIC�ж����ȼ�����
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	//��ʱ��ʹ��
	TIM_Cmd(TIM2, ENABLE);
}
 
/*
void TIM2_IRQHandler(void)  //TM2���жϺ���
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
*/

