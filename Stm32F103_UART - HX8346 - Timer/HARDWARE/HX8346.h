#ifndef __HX8346_H
#define __HX8346_H	 
#include "sys.h"	    
#include "stm32f10x.h" 

extern uint16_t g_cSbuff;

void SPI_init(void);
void SPI_DAM_init(void);
void SPI_DMA_TXD(u32 Num);
void Lcd_ColorBox(u16 xStart,u16 yStart,u16 xLong,u16 yLong,u16 Color);
void SPI_WriteNumber(u16 Color,u32 Num);

#endif