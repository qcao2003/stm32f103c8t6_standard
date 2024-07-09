#include "can.h"
#include "led.h"
#include "Delay.h"
#include "uart1.h"



void CAN_Configuration(void) 
{
  CAN_InitTypeDef CAN_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
 
  // 使能CAN和GPIO时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
 
  // 配置CAN引脚
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
 
  // CAN初始化
  CAN_DeInit(CAN1);
  CAN_StructInit(&CAN_InitStructure);
 
  // 配置CAN参数
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
 
  // 设置消息ID和长度
  TxMessage.StdId = 0x123;
  TxMessage.IDE = CAN_Id_Standard;
  TxMessage.RTR = CAN_RTR_Data;
  TxMessage.DLC = 1;
 
  // 设置消息数据
  TxMessage.Data[0] = data;
 
  // 发送消息
  CAN_Transmit(CAN1, &TxMessage);
	printf("Send is finish\r\n");
}

void CAN_ReceiveMessage(void)
{
	uint8_t data;
  CanRxMsg RxMessage;
 
  // 等待接收到消息
  while (CAN_MessagePending(CAN1, CAN_FIFO0) == 0);
 
  // 接收消息 存入到FIFO0里面
  CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
  
  // 处理接收到的消息
  if (RxMessage.StdId == 0x123 && RxMessage.IDE == CAN_Id_Standard && RxMessage.DLC == 1) {
    data = RxMessage.Data[0];// 处理接收到的数据
		printf("Data is %d\r\n",data);
  }
}

