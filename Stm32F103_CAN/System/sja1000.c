#include "sja1000.h"

void adio_out_change(void)//��������
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = 0xFF; //��GPIOA0��GPIOA7
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void adio_in_change(void)//��������
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = 0xFF;	//��GPIOA0��GPIOA7
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void sja1000_IO_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_15; //PA12 SJA1000_CS PA15 SAJ1000_ALE
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA12
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5; //PB3 SJA1000_WR PB4 SAJ1000_RD PB5 SJA1000_RESET
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
}

void MCU_Init(void)
{
	//CPU��ʼ��
	CAN_RESET_L;  //SJA1000��λ��Ч
	Delay_ms(10);
	CAN_RESET_H;  //CAN���߸�λ�ܽ�,��λ��Ч
  CAN_CS_L;  //CAN����Ƭѡ��Ч
	EXTIX_Init(); //���ж�
	
}

void Peli_Init(void)
{//SJA1000 �ĳ�ʼ��
    uint8 bdata Status;
    do
    {//.0=1---reset MODRe,���븴λģʽ���Ա�������Ӧ�ļĴ���
     //��ֹδ���븴λģʽ���ظ�д��
      SJA_MOD = RM_BIT |AFM_BIT;
			Status = SJA_MOD ;
    }
    while(!(Status & RM_BIT));

    SJA_CDR  = CANMode_BIT|CLKOff_BIT;// CDR.3=1--ʱ�ӹر�, .7=0---basic CAN, .7=1---Peli CAN
    SJA_BTR0 = 0x03;
    SJA_BTR1 = 0x1c;//16M���񣬲�����125Kbps
    SJA_IER  = RIE_BIT;// .0=1--�����ж�ʹ�ܣ�  .1=0--�رշ����ж�ʹ��
    SJA_OCR  = NormalMode|Tx0PullDn|OCPOL1_BIT|Tx1PullUp;// ����������ƼĴ���
    SJA_CMR  = RRB_BIT;//�ͷŽ��ջ�����

    SJA_ACR0  = 0x11;
    SJA_ACR1  = 0x22;
    SJA_ACR2  = 0x33;
    SJA_ACR3  = 0x44;//��ʼ����ʾ��

    SJA_AMR0  = 0xff;
    SJA_AMR1  = 0xff;
    SJA_AMR2  = 0xff;
    SJA_AMR3  = 0xff;//��ʼ������

    do
    {//ȷ�������Խ���ģʽ
			SJA_MOD   = STM_BIT;
			Status  = SJA_MOD;
     }
    while( !(Status & STM_BIT) );

}//SJA1000 �ĳ�ʼ��


