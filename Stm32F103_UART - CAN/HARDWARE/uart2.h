
#ifndef __UART2_H
#define __UART2_H

#include "stdio.h"	
#include <stdarg.h>
#include <stdio.h>
#include <string.h>


//如果想串口中断接收，请不要注释以下宏定义
void uart2_init(u32 bound);
void u2_printf(char* fmt, ...);
void USART2_IRQHandler(void);
 
 
#endif

