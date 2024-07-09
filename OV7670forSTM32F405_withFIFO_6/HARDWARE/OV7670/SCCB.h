#ifndef __SCCB_H
#define __SCCB_H

#include "sys.h" 

//IO方向设置
#define SCCB_SDA_IN()  {GPIOA->MODER&=~(3<<(12*2));GPIOA->MODER|=0<<12*2;}	//PA12 输入
#define SCCB_SDA_OUT() {GPIOA->MODER&=~(3<<(12*2));GPIOA->MODER|=1<<12*2;} 	//PA12 输出

//IO操作函数	 
#define SCCB_SCL    		PAout(11)	 	//SCL
#define SCCB_SDA    		PAout(12) 		//SDA	 

#define SCCB_READ_SDA    	PAin(12)  		//输入SDA    

void SCCB_Init(void);
void SCCB_Start(void);
void SCCB_Stop(void);
void SCCB_No_Ack(void);
u8 SCCB_WR_Byte(u8 dat);
u8 SCCB_RD_Byte(void);
u8 SCCB_WR_Reg(u8 sccb_id, u8 reg, u8 data);
u8 SCCB_RD_Reg(u8 sccb_id, u8 reg);

#endif
