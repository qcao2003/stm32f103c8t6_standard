#ifndef __OV7670_H
#define __OV7670_H
#include "sys.h"
#define OV7670_PWDN  	PAout(13)			//POWER DOWN控制信号 
#define OV7670_RST  	PAout(14)			//复位控制信号 
#define OV7670_MID				0X7FA2    
#define OV7670_PID				0X7673

extern uint8_t OV7670_State;
extern uint8_t pic[128*160*2];
u8 OV7670_Init(void);
void OV7670_Light_Mode(u8 mode);
void OV7670_Color_Saturation(u8 sat);
void OV7670_Brightness(u8 bright);
void OV7670_Contrast(u8 contrast);
void OV7670_Special_Effects(u8 eft);
void OV7670_Window_Set(u16 sx,u16 sy,u16 width,u16 height);
void OV7670_STM32GetPic(void);
void OV7670_config_window(u16 startx,u16 starty,u16 width,u16 height);
void OV7670_Self_Window_Set(void);

#endif
