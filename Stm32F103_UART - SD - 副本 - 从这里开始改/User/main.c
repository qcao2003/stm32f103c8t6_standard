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
#include "exfuns.h"
#include "fattester.h"

const unsigned char buffer[100] = "This is a new file, the data is just written in!";
//读取SD卡的指定扇区的内容，并通过串口1输出
//sec：扇区物理地址编号
void SD_Read_Sectorx(u32 sec)
{
	u8 *buf;
	u16 i;
	buf=mymalloc(512);				//申请内存
	if(SD_ReadDisk(buf,sec,1)==0)	//读取0扇区的内容
	{	
		printf("SECTOR 0 DATA:\r\n");
		for(i=0;i<512;i++)printf("%x ",buf[i]);//打印sec扇区数据    	   
		printf("\r\nDATA ENDED\r\n");
		printf("USART1 Send Data Over!\r\n");
	}
	myfree(buf);//释放内存	
}

int main(void)
{
	FIL fsrc; // file objects
	 
	FRESULT res; // FatFs function common result code
	 
	UINT bw; // File R/W coun
	u32 total,free;
	u32 sd_size;
	u8 cid[16];
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
	exfuns_init(); //为fatfs相关变量申请内存
	f_mount(fs[0],"0:",1); 					//挂载SD卡
	while(exf_getfree("0",&total,&free))	//得到SD卡的总容量和剩余容量
	{
		printf("Fatfs Error!\r\n");
		delay_ms(500);					
		printf("Please Check! \r\n");
		delay_ms(500);
		LED=!LED;//PC13闪烁
	}
	while (1)
	{
		if(USART_RX_STA)
		{
			delay_ms(500);
			printf("%s",USART_RX_BUF);
			if(strcmp(USART_RX_BUF,"LED_OFF\r\n") == 0)//对比数组
			{
				GPIO_SetBits(GPIOC, GPIO_Pin_13);//关灯
				printf("OFF\r\n");
				res = f_open(&fsrc,"data.txt", FA_WRITE | FA_CREATE_ALWAYS);
				if(res == FR_OK)
				{
					res = f_write(&fsrc, buffer, 100, &bw);
				}
				while(bw < 100);
				f_close(&fsrc);		
				printf("SD Total Size %d MB\r\n",total>>10);
				printf("SD Free Size %d MB\r\n",free>>10);
				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//清空数组内存
			}
				
			
			if(strcmp(USART_RX_BUF,"LED_ON\r\n") == 0)//对比数组
			{
				GPIO_ResetBits(GPIOC, GPIO_Pin_13);//开灯
				printf("ON\r\n");
				sd_size=SD_GetSectorCount();//得到扇区数
				printf("SD Card Size %d MB\r\n",sd_size>>11);
				printf("SD_CID is %d \r\n",SD_GetCID(cid));
				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//清空数组内存
			}
			
			memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//清空数组内存
			
			USART_RX_STA = 0;
		}
		//GPIO_ResetBits(GPIOC, GPIO_Pin_13);
		//Delay_ms(500);
		//GPIO_SetBits(GPIOC, GPIO_Pin_13);
		//Delay_ms(500);
	}
}
