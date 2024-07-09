#ifndef __SPI_H_
#define __SPI_H_
#include "sys.h"

#define 	MySPI_MOSI_GPIO_CLK			RCC_APB2Periph_GPIOA
#define 	MySPI_MOSI_GPIO				GPIOA
#define 	MySPI_MOSI_GPIO_Pin			GPIO_Pin_7

#define 	MySPI_MISO_GPIO_CLK			RCC_APB2Periph_GPIOA
#define 	MySPI_MISO_GPIO				GPIOA
#define 	MySPI_MISO_GPIO_Pin			GPIO_Pin_6

#define 	MySPI_SCLK_GPIO_CLK			RCC_APB2Periph_GPIOA
#define 	MySPI_SCLK_GPIO				GPIOA
#define 	MySPI_SCLK_GPIO_Pin			GPIO_Pin_5

#define		MySPI_SS_GPIO_CLK			RCC_APB2Periph_GPIOA
#define 	MySPI_SS_GPIO				GPIOA
#define 	MySPI_SS_GPIO_Pin			GPIO_Pin_4

void MySPI_Init(void);
void MySPI_Start(void);
void MySPI_Stop(void);
uint8_t MySPI_SwapByte(uint8_t ByteSend);

#endif


