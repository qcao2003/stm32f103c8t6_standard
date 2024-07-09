#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "led.h"
#include "uart1.h"
#include "uart2.h"
#include "uart3.h"
#include "spi.h"
#include "mmc_sd.h"
#include "malloc.h" 
#include "ff.h"
#include "diskio.h"

FATFS fatfs;

const unsigned char buffer[100] = "This is a new file, the data is just written in!";
unsigned char buff0[100];

int main(void)
{
	FIL fsrc; // file objects
	 
	FRESULT res; // FatFs function common result code
	 
	UINT bw; // File R/W coun
	LED_Init();
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	uart1_init(9600);
	mem_init();				//初始化内存池	
	//uart2_init(9600);
	//uart3_init(9600);
	while(SD_Initialize())
	{
		printf("SD Card Error!\r\n");
		delay_ms(500);					
		printf("Please Check! \r\n");
		delay_ms(500);
		LED=!LED;//PC13闪烁
	}
	//检测SD卡成功 											    
	printf("SD Card OK\r\n");
	res = f_mount(&fatfs,"0",1); 					//挂载SD卡
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
				res = f_open(&fsrc,"0:text1.txt", FA_WRITE | FA_CREATE_ALWAYS);
				printf("File is opened\r\n");
				if(res == FR_OK)
				{
					res = f_write(&fsrc, buffer, 100, &bw);//写入的时候三次初始化
				}
				f_close(&fsrc);		
				printf("Write is done\r\n");
				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//清空数组内存
			}
				
			
			if(strcmp(USART_RX_BUF,"Read\r\n") == 0)//对比数组
			{
				GPIO_ResetBits(GPIOC, GPIO_Pin_13);//开灯
				printf("Read\r\n");
				res = f_open(&fsrc,"0:text1.txt", FA_READ);
				res = f_read(&fsrc,buff0,100,&bw);
				f_close(&fsrc);
				printf("%s\r\n",(char*)buff0);
				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//清空数组内存
			}
			
			memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//清空数组内存
			
			USART_RX_STA = 0;
		}
	}
}
