#include "exit.h"
//�ⲿ�ж�0�������

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
		
		sja1000_write_cmd(0x01,0x04); //�������husn
		
		sja1000_send_frame();
	}
	EXTI_ClearITPendingBit(EXTI_Line11); //���LINE0�ϵ��жϱ�־λ 

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
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//ʹ��SYSCFGʱ��

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource11);//PA11 ���ӵ��ж���1
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);//����ϵͳ�ж����ȼ�����2

	/* ����EXTI_Line1 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line11;//LINE11
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //�����ش��� 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ��LINE1
	EXTI_Init(&EXTI_InitStructure);//����

	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;//�ⲿ�ж�11
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;//�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);//����
}

//������ʼ������
void KEY_InitPA11(void)
{

	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; //PA11 ��Ӧ����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//��������ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50M

	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA11
	
 
} 



