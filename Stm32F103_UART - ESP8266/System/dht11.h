#ifndef  __DHT11_H
#define  __DHT11_H
 
#include "sys.h"
#include "stm32f10x.h"
 

#define DHT11_PORT      GPIOA
#define DHT11_CLK       RCC_APB2Periph_GPIOA
#define DHT11_TRIG      GPIO_Pin_1
#define DHT11           GPIO_ReadInputDataBit(DHT11_PORT,DHT11_TRIG)
 
#define DHT11_HIGH      GPIO_SetBits(GPIOA, GPIO_Pin_1)
#define DHT11_LOW       GPIO_ResetBits(GPIOA, GPIO_Pin_1)
 
extern uint8_t  dat[5];

void DHT11_GPIO_OUT(void);
void DHT11_GPIO_IN(void);
uint8_t DHT_Read_Byte(void);
uint8_t DHT_Read(void);
 
#endif

