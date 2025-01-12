#ifndef __UART1_H
#define __UART1_H

#include "stdio.h"	
 
#define USART_REC_LEN  			200  	//定义最大接收字节数 200
	  	
extern char USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         		//接收状态标记	
//如果想串口中断接收，请不要注释以下宏定义
void uart1_init(u32 bound);

void USART1_IRQHandler(void);
void Serial_SendByte(uint8_t Byte);
 
#endif

