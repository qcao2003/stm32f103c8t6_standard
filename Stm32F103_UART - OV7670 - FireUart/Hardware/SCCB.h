#ifndef __SCCB_H
#define __SCCB_H

#include "sys.h" 
#include "uart1.h"
#include "Delay.h"
#include "led.h"

//IO操作函数	 
#define SCCB_SCL_H    		GPIO_SetBits(GPIOA, GPIO_Pin_12)	 	//SCL
#define SCCB_SCL_L    		GPIO_ResetBits(GPIOA, GPIO_Pin_12)	 	//SCL
#define SCCB_SDA_H    		GPIO_SetBits(GPIOA, GPIO_Pin_11) 		//SDA	 
#define SCCB_SDA_L    		GPIO_ResetBits(GPIOA, GPIO_Pin_11) 		//SDA	 

#define SCCB_READ_SDA    	GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11)  		//输入SDA    


//IO方向设置
#define SCCB_SDA_IN()  {GPIOA->CRH &= 0xFFFF0FFF;GPIOA->CRH |= 8 << ((11-8)*4);}		//PA11 输入
#define SCCB_SDA_OUT() {GPIOA->CRH &= 0xFFFF0FFF;GPIOA->CRH |= 3 << ((11-8)*4);} 	//PA11 输出

#define OV7670_ADDRESS 0x42

void SCCB_Init(void);
void SCCB_Start(void);
void SCCB_Stop(void);
void SCCB_No_Ack(void);
u8 SCCB_WR_Byte(u8 dat);
u8 SCCB_RD_Byte(void);
u8 SCCB_WR_Reg(u8 reg, u8 data);
u8 SCCB_RD_Reg(u8 reg);

#endif
