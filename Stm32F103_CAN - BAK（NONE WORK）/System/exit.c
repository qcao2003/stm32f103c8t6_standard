#include "exit.h"
//外部中断0服务程序

uint8_t msgbuf[8]={0};
 
void EXTI15_10_IRQHandler(void)
{

	printf("PA11_Press\r\n"); 
	uint8_t data=0,i;
	
	data = sja1000_read_byte(0x03);
	
	if(data & 0x01)
	{
		for(i=0;i<8;i++)
		{
			msgbuf[i] = sja1000_read_byte(22+i);
			printf("msgbuf[%d]",i);
			printf("%X\r\n",msgbuf[i]);
		}
		
		sja1000_write_cmd(0x01,0x04); //清除接收husn
		
		sja1000_send_frame();
	}
	EXTI_ClearITPendingBit(EXTI_Line11); //清除LINE0上的中断标志位 

}	


void EXTIX_Init(void)
{

	KEY_InitPA11();
	
	
	EXTIPA11_Init();

}



void EXTIPA11_Init(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能SYSCFG时钟

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource11);//PA11 连接到中断线1
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);//设置系统中断优先级分组2

	/* 配置EXTI_Line1 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line11;//LINE11
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //上升沿触发 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能LINE1
	EXTI_Init(&EXTI_InitStructure);//配置

	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;//外部中断11
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;//子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);//配置
}

//按键初始化函数
void KEY_InitPA11(void)
{

	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能GPIOA时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; //PA11 对应引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50M

	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA11
	
 
} 



