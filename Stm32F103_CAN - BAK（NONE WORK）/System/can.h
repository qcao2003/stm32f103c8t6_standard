#ifndef __CAN_H
#define __CAN_H
#include "sys.h"
#include "delay.h"
#include "stm32f10x.h"  
#include "uart1.h"

#define CAN_CS_H GPIO_SetBits(GPIOA, GPIO_Pin_12);
#define CAN_CS_L GPIO_ResetBits(GPIOA, GPIO_Pin_12);

#define CAN_ALE_H GPIO_SetBits(GPIOA, GPIO_Pin_15);
#define CAN_ALE_L GPIO_ResetBits(GPIOA, GPIO_Pin_15);

#define CAN_WR_H GPIO_SetBits(GPIOB, GPIO_Pin_3);
#define CAN_WR_L GPIO_ResetBits(GPIOB, GPIO_Pin_3);

#define CAN_RD_H GPIO_SetBits(GPIOB, GPIO_Pin_4);
#define CAN_RD_L GPIO_ResetBits(GPIOB, GPIO_Pin_4);

#define CAN_RESET_H GPIO_SetBits(GPIOB, GPIO_Pin_5);
#define CAN_RESET_L GPIO_ResetBits(GPIOB, GPIO_Pin_5);

#define CAN_AD_Port GPIOA
extern uint16_t adIOPin[];

extern uint8_t can_filter[8];
extern uint8_t msg[10];
void sja1000_send_frame(void);
void sja1000_reg_Init(void);
uint8_t sja1000_read_byte(uint8_t addr);
void sja1000_write_cmd(uint8_t addr, uint8_t cmd);
void adio_in_change(void);
void PA0_7_init(void);
#endif

