#ifndef __EXTI_H
#define __EXTI_H
#include "sys.h"
#include "led.h"
#include "delay.h"
#include "uart1.h"

void EXTI_InitPA15(void);
void NVIC_Close_Configuration(void);
void NVIC_Open_Configuration(void);

#endif
