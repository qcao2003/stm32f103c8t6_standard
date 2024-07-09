#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "led.h"
#include "uart1.h"
#include "uart2.h"
#include "uart3.h"
#include "can.h"
#include "spi.h"
#include "flash.h"
#include "image.h"
#include "rtc.h"

//要写入到W25Q64的字符串数组
const u8 TEXT_Buffer_First[]={"MiniSTM32 SPI TEST"};
#define First_SIZE sizeof(TEXT_Buffer_First)	 
	
const u8 TEXT_Buffer_Second[]={"China has been Well"};
#define Second_Size sizeof(TEXT_Buffer_Second)

#define Third_Size sizeof(image)
	
char Data[50],Time[50];

int main(void)
{
	u16 id = 0;
	u32 FLASH_SIZE,i;
	u8 datatemp[256];
	LED_Init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	uart1_init(9600);
	SPI_Flash_Init();  		//SPI FLASH 初始化
	
	//uart2_init(9600);
	//uart3_init(9600);
	while(1)
	{
		id = SPI_Flash_ReadID();
		printf("The W25Q64 id is 0x%X \r\n",id);
		if (id == W25Q64 || id == NM25Q64)
			break;
	}
	while(RTC_Init())
	{
		printf("RTC_Init_Error\r\n");
	}
	FLASH_SIZE=8*1024*1024;	//FLASH 大小为8M字节 一共有2048个扇区
	
	while (1)
	{
		if(USART_RX_STA)
		{
			Delay_ms(500);
			printf("%s",USART_RX_BUF);
			if(strcmp(USART_RX_BUF,"First_Write\r\n") == 0)//对比数组
			{
				GPIO_SetBits(GPIOC, GPIO_Pin_13);//关灯
				printf("OFF\r\n");
				//SPI_Flash_Write((u8*)TEXT_Buffer,FLASH_SIZE-100,SIZE);		//从倒数第100个地址处开始,写入SIZE长度的数据
				Write_page_with_check_Sector((u8*)TEXT_Buffer_First,FLASH_SIZE-100,First_SIZE);		//写入到倒数100个地址中，长度是256个字节
				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//清空数组内存
			}
				
			if(strcmp(USART_RX_BUF,"Second_Write\r\n") == 0)//对比数组
			{
				GPIO_SetBits(GPIOC, GPIO_Pin_13);//关灯
				printf("OFF\r\n");
				//SPI_Flash_Write((u8*)TEXT_Buffer_Second,FLASH_SIZE-100,Second_Size);		//从倒数第100个地址处开始,写入SIZE长度的数据
				Write_page_with_check_Sector((u8*)TEXT_Buffer_Second,FLASH_SIZE-80,Second_Size);		//写入到倒数80个地址中，长度是256个字节
				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//清空数组内存
			}
			
			if(strcmp(USART_RX_BUF,"Third_Write\r\n") == 0)//对比数组
			{
				GPIO_SetBits(GPIOC, GPIO_Pin_13);//关灯
				printf("OFF\r\n");
				SPI_Flash_Erase_Sector((FLASH_SIZE-4096*200)/4096);
				SPI_Flash_Erase_Sector((FLASH_SIZE-4096*199)/4096);
				SPI_Flash_Erase_Sector((FLASH_SIZE-4096*198)/4096);
				SPI_Flash_Write_NoCheck((u8*)image,FLASH_SIZE-4096*200,Third_Size);	//写入倒数200个扇区	也就是2048-200个扇区

				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//清空数组内存
			}
			
			if(strcmp(USART_RX_BUF,"Fourth_Write\r\n") == 0)
			{
				RTC_Get();
				sprintf(Data,"%s%d%s%d%s%d%s","Today is ",calendar.w_year,"/",calendar.w_month,"/",calendar.w_date,"\r\n");
				sprintf(Time,"%s%d%s%d%s%d%s","Time is ",calendar.hour,":",calendar.min,":",calendar.sec,"\r\n");
				printf("%s",Data);
				printf("%s",Time);
				Write_page_with_check_Sector((u8*)Data,500,256);		//写入到0个地址中，长度是256个字节
				//Write_page_with_check_Sector((u8*)Time,1000,256);		//写入到100个地址中，长度是256个字节
				SPI_Flash_Read(datatemp,500,256);
				printf("The 500 Address is %s\r\n",datatemp);
				//SPI_Flash_Read(datatemp,1000,256);
				//printf("The 1000 Address is %s\r\n",datatemp);
			}
			
			if(strcmp(USART_RX_BUF,"First_Read\r\n") == 0)//对比数组
			{
				GPIO_ResetBits(GPIOC, GPIO_Pin_13);//开灯
				printf("ON\r\n");
				SPI_Flash_Read(datatemp,FLASH_SIZE-100,First_SIZE);		//从倒数第100个地址处开始,读出SIZE个字节
				printf("The 100th reciprocal is %s\r\n",datatemp);
				SPI_Flash_Read(datatemp,FLASH_SIZE-99,First_SIZE);		//从倒数第99个地址处开始,读出SIZE个字节  说明这里1个地址是存一个字节的数据
				printf("The 99th reciprocal is %s\r\n",datatemp);
				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//清空数组内存
			}
			
			if(strcmp(USART_RX_BUF,"Second_Read\r\n") == 0)//对比数组
			{
				GPIO_ResetBits(GPIOC, GPIO_Pin_13);//开灯
				printf("ON\r\n");
				SPI_Flash_Read(datatemp,FLASH_SIZE-80,Second_Size);		//从倒数第80个地址处开始,读出SIZE个字节
				printf("%s\r\n",datatemp);
				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//清空数组内存
			}
			
			if(strcmp(USART_RX_BUF,"Third_Read\r\n") == 0)//对比数组
			{
				GPIO_ResetBits(GPIOC, GPIO_Pin_13);//开灯
				printf("ON\r\n");
				Serial_SendByte(0x01);
				Serial_SendByte(0xFE);
				SPI_Flash_Read(datatemp,FLASH_SIZE-4096*200,Third_Size);		//从倒数第100个地址处开始,读出SIZE个字节
				//printf("%s\r\n",datatemp);
				for(i=0;i<sizeof(datatemp);i++)
				{
					Serial_SendByte(datatemp[i]);
					i++;
				}
				Serial_SendByte(0xFE);
				Serial_SendByte(0x01);
				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//清空数组内存
			}
			
			if(strcmp(USART_RX_BUF,"Fourth_Read\r\n") == 0)//对比数组
			{
				memset(datatemp,0,sizeof(datatemp));
				SPI_Flash_Read(datatemp,500,256);
				printf("The 500 Address is %s\r\n",datatemp);
				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//清空数组内存
			}
			
			if(strcmp(USART_RX_BUF,"Erase_Sector\r\n") == 0)//对比数组
			{
				GPIO_ResetBits(GPIOC, GPIO_Pin_13);//开灯
				printf("Erasing\r\n");
				SPI_Flash_Erase_Sector((FLASH_SIZE-100)/4096);				//擦除这个扇区
				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//清空数组内存
			}
			
			if(strcmp(USART_RX_BUF,"ReadSR\r\n") == 0)//对比数组
			{
				GPIO_ResetBits(GPIOC, GPIO_Pin_13);//开灯
				printf("The State Register is 0x%X\r\n",SPI_Flash_ReadSR());				//读取SR寄存器
				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//清空数组内存
			}
			
			if(strcmp(USART_RX_BUF,"Erase_All\r\n") == 0)//对比数组
			{
				GPIO_ResetBits(GPIOC, GPIO_Pin_13);//开灯
				SPI_Flash_Erase_Chip();
				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//清空数组内存
			}
			
			if(strcmp(USART_RX_BUF,"ReadID\r\n") == 0)//对比数组
			{
				GPIO_ResetBits(GPIOC, GPIO_Pin_13);//开灯
				printf("The JedecDeviceID is 0x%X\r\n",SPI_Flash_Read_JedecDeviceID());				//读取JedecID
				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//清空数组内存
			}
			memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//清空数组内存
			
			USART_RX_STA = 0;
		}
//		GPIO_ResetBits(GPIOC, GPIO_Pin_13);
//		Delay_ms(500);
//		GPIO_SetBits(GPIOC, GPIO_Pin_13);
//		Delay_ms(500);
//		Serial_SendByte(0x01);
//		Serial_SendByte(0xFE);
//		SPI_Flash_Read(datatemp,FLASH_SIZE-4096*200,Third_Size);		//从倒数第100个地址处开始,读出SIZE个字节
//		//printf("%s\r\n",datatemp);
//		for(i=0;i<sizeof(datatemp);i++)
//		{
//			Serial_SendByte(datatemp[i]);
//			i++;
//		}
//		Serial_SendByte(0xFE);
//		Serial_SendByte(0x01);
	}

}
