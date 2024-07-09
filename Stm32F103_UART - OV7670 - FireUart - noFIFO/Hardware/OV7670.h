#ifndef __OV7670_H
#define __OV7670_H
#include "sys.h"
#include "SCCB.h"
#include "Delay.h"
#include "uart1.h"
#include "exti.h"

#define OV7670_PWDN_H  	GPIO_SetBits(GPIOB, GPIO_Pin_3);			//POWER DOWN控制信号 
#define OV7670_PWDN_L  	GPIO_ResetBits(GPIOB, GPIO_Pin_3);			//POWER DOWN控制信号 
#define OV7670_RST_H  	GPIO_SetBits(GPIOB, GPIO_Pin_4);			//复位控制信号 
#define OV7670_RST_L  	GPIO_ResetBits(GPIOB, GPIO_Pin_4);			//复位控制信号 

#define OV7670_MID				0X7FA2    
#define OV7670_PID				0X7673

u8 OV7670_Init(void);
void OV7670_Light_Mode(u8 mode);
void OV7670_Color_Saturation(u8 sat);
void OV7670_Brightness(u8 bright);
void OV7670_Contrast(u8 contrast);
void OV7670_Special_Effects(u8 eft);
void OV7670_Window_Set(u16 sx,u16 sy,u16 width,u16 height);
void OV7670_STM32GetPic(uint32_t i,uint32_t j);
void OV7670_config_window(u16 startx,u16 starty,u16 width,u16 height);
void OV7670_Self_Window_Set(void);
void FIFOgetPIC(void);
void Send_Fired_Uart_init(void);
#endif
