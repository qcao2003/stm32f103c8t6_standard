#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "led.h"
#include "uart1.h"
#include "OV7670.h"
#include "SCCB.h"
#include "exti.h"
#include "spi.h"
#include "mmc_sd.h"
#include "malloc.h"
#include "ff.h"
#include "diskio.h"
#include "exfuns.h"

uint8_t OV7670_State=0;
const unsigned char buffer[100] = "This is a new file, the data is just written in!";



int main(void)
{
	
	FRESULT res; // FatFs function common result code
	LED_Init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);// 设置中断优先级分组3
	uart1_init(9600);
	delay_init();
	while(SD_Initialize())
	{
		printf("SD Card Error\r\n");
		delay_ms(500);			
		printf("Please Check!\r\n");	
		delay_ms(500);
		LED=!LED;//PC13闪烁
	}
	OV7670_Init();
	EXTI_InitPA15();
	mem_init();				//初始化内存池
	exfuns_init();    //分配内存
	res = f_mount(fs[0],"0",1); 					//挂载SD卡
	if(res==FR_OK)
		printf("f_mount is ok\r\n");
	
	while (1)
	{
		if(USART_RX_STA)
		{
			delay_ms(500);
			printf("%s",USART_RX_BUF);
			if(strcmp(USART_RX_BUF,"Write\r\n") == 0)//对比数组
			{
				GPIO_SetBits(GPIOC, GPIO_Pin_13);//关灯
				printf("Write\r\n");
				res = f_open(file,"0:text1.txt", FA_WRITE | FA_CREATE_ALWAYS);
				printf("File is opened\r\n");
				if(res == FR_OK)
				{
					res = f_write(file, buffer, 100, &bw);//写入的时候三次初始化
				}
				f_close(file);
				printf("Write is done\r\n");				
				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//清空数组内存
			}
				
			
			if(strcmp(USART_RX_BUF,"Read\r\n") == 0)//对比数组
			{
				GPIO_ResetBits(GPIOC, GPIO_Pin_13);//开灯
				printf("Read\r\n");
				res = f_open(file,"0:text1.txt", FA_READ);
				res = f_read(file,fatbuf,100,&br);
				f_close(file);
				printf("%s\r\n",fatbuf);
				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//清空数组内存
			}
			
			if(strcmp(USART_RX_BUF,"Write_Pic\r\n") == 0)//对比数组
			{
				OV7670_State = 0;//重新进入到开启FIFO状态
				GPIO_ResetBits(GPIOC, GPIO_Pin_13);//开灯
				printf("Write_Pic\r\n");
				OV7670_STM32GetPictoSDCARD(240,320,file,res,bw,"0:test3.bmp");
				printf("Write_Pic is completed\r\n");
				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//清空数组内存
			}
			
			memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//清空数组内存
			
			USART_RX_STA = 0;
		}
	}
}
