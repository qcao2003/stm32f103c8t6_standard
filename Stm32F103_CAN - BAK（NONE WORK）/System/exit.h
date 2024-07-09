#ifndef __EXTI_H
#define __EXTI_H
#include "sys.h"

#include "led.h"
#include "delay.h"
#include "can.h"

extern uint8_t msgbuf[8];

void EXTIX_Init(void);

void EXTI15_10_IRQHandler(void);
void EXTIPA11_Init(void);
void KEY_InitPA11(void);

#endif

