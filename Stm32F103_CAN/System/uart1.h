#ifndef __UART1_H
#define __UART1_H

#include "stdio.h"	
 
#define USART_REC_LEN  			200  	//�����������ֽ��� 200
	  	
extern char USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	
//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart1_init(u32 bound);
 
void USART1_IRQHandler(void);
 
 
#endif

