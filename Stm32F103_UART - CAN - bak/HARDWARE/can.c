#include "can.h"
#include "led.h"
#include "Delay.h"
#include "uart1.h"



void CAN_Configuration(void) 
{
  CAN_InitTypeDef CAN_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
 
  // ʹ��CAN��GPIOʱ��
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
 
  // ����CAN����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
 
  // CAN��ʼ��
  CAN_DeInit(CAN1);
  CAN_StructInit(&CAN_InitStructure);
 
  // ����CAN����
  CAN_InitStructure.CAN_TTCM = DISABLE;
  CAN_InitStructure.CAN_ABOM = DISABLE;
  CAN_InitStructure.CAN_AWUM = DISABLE;
  CAN_InitStructure.CAN_NART = DISABLE;
  CAN_InitStructure.CAN_RFLM = DISABLE;
  CAN_InitStructure.CAN_TXFP = DISABLE;
  CAN_InitStructure.CAN_Mode = CAN_Mode_LoopBack;
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
  CAN_InitStructure.CAN_BS1 = CAN_BS1_4tq;
  CAN_InitStructure.CAN_BS2 = CAN_BS2_3tq;
  CAN_InitStructure.CAN_Prescaler = 6;
  CAN_Init(CAN1, &CAN_InitStructure);
	printf("Can init is finish\r\n");
}

void CAN_SendMessage(uint8_t data) 
{
  CanTxMsg TxMessage;
 
  // ������ϢID�ͳ���
  TxMessage.StdId = 0x123;
  TxMessage.IDE = CAN_Id_Standard;
  TxMessage.RTR = CAN_RTR_Data;
  TxMessage.DLC = 1;
 
  // ������Ϣ����
  TxMessage.Data[0] = data;
 
  // ������Ϣ
  CAN_Transmit(CAN1, &TxMessage);
	printf("Send is finish\r\n");
}

void CAN_ReceiveMessage(void)
{
	uint8_t data;
  CanRxMsg RxMessage;
 
  // �ȴ����յ���Ϣ
  while (CAN_MessagePending(CAN1, CAN_FIFO0) == 0);
 
  // ������Ϣ ���뵽FIFO0����
  CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
  
  // ������յ�����Ϣ
  if (RxMessage.StdId == 0x123 && RxMessage.IDE == CAN_Id_Standard && RxMessage.DLC == 1) {
    data = RxMessage.Data[0];// ������յ�������
		printf("Data is %d\r\n",data);
  }
}

