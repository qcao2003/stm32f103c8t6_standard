
#ifndef __UART2_H
#define __UART2_H

#include "stdio.h"	
#include <stdarg.h>
#include <stdio.h>
#include <string.h>


//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart2_init(u32 bound);
void u2_printf(char* fmt, ...);
void USART2_IRQHandler(void);
 
 
#endif

