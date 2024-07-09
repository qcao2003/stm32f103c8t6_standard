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

//Ҫд�뵽W25Q64���ַ�������
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
	u8 datatemp[50];
	LED_Init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// �����ж����ȼ�����2
	uart1_init(9600);
	SPI_Flash_Init();  		//SPI FLASH ��ʼ��
	
	//uart2_init(9600);
	//uart3_init(9600);
//	while(1)
//	{
//		id = SPI_Flash_ReadID();
//		printf("The W25Q64 id is 0x%X \r\n",id);
//		if (id == W25Q64 || id == NM25Q64)
//			break;
//	}
	while(RTC_Init())
	{
		printf("RTC_Init_Error\r\n");
	}
	FLASH_SIZE=8*1024*1024;	//FLASH ��СΪ8M�ֽ� һ����2048������
	
	while (1)
	{
		if(USART_RX_STA)
		{
			Delay_ms(500);
			printf("%s",USART_RX_BUF);
			if(strcmp(USART_RX_BUF,"First_Write\r\n") == 0)//�Ա�����
			{
				GPIO_SetBits(GPIOC, GPIO_Pin_13);//�ص�
				printf("OFF\r\n");
				//SPI_Flash_Write((u8*)TEXT_Buffer,FLASH_SIZE-100,SIZE);		//�ӵ�����100����ַ����ʼ,д��SIZE���ȵ�����
				Write_page_with_check_Sector((u8*)TEXT_Buffer_First,FLASH_SIZE-100,First_SIZE);		//д�뵽����100����ַ�У�������256���ֽ�
				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//��������ڴ�
			}
				
			if(strcmp(USART_RX_BUF,"Second_Write\r\n") == 0)//�Ա�����
			{
				GPIO_SetBits(GPIOC, GPIO_Pin_13);//�ص�
				printf("OFF\r\n");
				//SPI_Flash_Write((u8*)TEXT_Buffer_Second,FLASH_SIZE-100,Second_Size);		//�ӵ�����100����ַ����ʼ,д��SIZE���ȵ�����
				Write_page_with_check_Sector((u8*)TEXT_Buffer_Second,FLASH_SIZE-80,Second_Size);		//д�뵽����80����ַ�У�������256���ֽ�
				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//��������ڴ�
			}
			
			if(strcmp(USART_RX_BUF,"Third_Write\r\n") == 0)//�Ա�����
			{
				for(i = 0;i < FLASH_SIZE;i=i+100)
				{
					GPIO_SetBits(GPIOC, GPIO_Pin_13);//�ص�
					RTC_Get();
					sprintf(Data,"%s%d%s%d%s%d%s","Today is ",calendar.w_year,"/",calendar.w_month,"/",calendar.w_date,"\r\n");
					sprintf(Time,"%s%d%s%d%s%d%s","Time is ",calendar.hour,":",calendar.min,":",calendar.sec,"\r\n");
					Write_page_with_check_Sector((u8*)Data,i,50);		//д�뵽1000����ַ�У�������50���ֽ�
					Write_page_with_check_Sector((u8*)Time,i+50,50);		//д�뵽1500����ַ�У�������50���ֽ�
					GPIO_ResetBits(GPIOC, GPIO_Pin_13);//����
					SPI_Flash_Read(datatemp,i,50);
					printf("The %d Address is %s\r\n",i,datatemp);
					SPI_Flash_Read(datatemp,i+50,50);
					printf("The %d Address is %s\r\n",i+50,datatemp);
				}
				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//��������ڴ�
			}
			
			if(strcmp(USART_RX_BUF,"Fourth_Write\r\n") == 0)
			{
				RTC_Get();
				sprintf(Data,"%s%d%s%d%s%d%s","Today is ",calendar.w_year,"/",calendar.w_month,"/",calendar.w_date,"\r\n");
				sprintf(Time,"%s%d%s%d%s%d%s","Time is ",calendar.hour,":",calendar.min,":",calendar.sec,"\r\n");
				printf("%s",Data);
				printf("%s",Time);
				Write_page_with_check_Sector((u8*)Data,1000,256);		//д�뵽1000����ַ�У�������256���ֽ�
				Write_page_with_check_Sector((u8*)Time,1500,256);		//д�뵽1500����ַ�У�������256���ֽ�
				SPI_Flash_Read(datatemp,1000,256);
				printf("The 1000 Address is %s\r\n",datatemp);
				SPI_Flash_Read(datatemp,1500,256);
				printf("The 1500 Address is %s\r\n",datatemp);
			}
			
			if(strcmp(USART_RX_BUF,"First_Read\r\n") == 0)//�Ա�����
			{
				GPIO_ResetBits(GPIOC, GPIO_Pin_13);//����
				printf("ON\r\n");
				SPI_Flash_Read(datatemp,FLASH_SIZE-100,First_SIZE);		//�ӵ�����100����ַ����ʼ,����SIZE���ֽ�
				printf("The 100th reciprocal is %s\r\n",datatemp);
				SPI_Flash_Read(datatemp,FLASH_SIZE-99,First_SIZE);		//�ӵ�����99����ַ����ʼ,����SIZE���ֽ�  ˵������1����ַ�Ǵ�һ���ֽڵ�����
				printf("The 99th reciprocal is %s\r\n",datatemp);
				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//��������ڴ�
			}
			
			if(strcmp(USART_RX_BUF,"Second_Read\r\n") == 0)//�Ա�����
			{
				GPIO_ResetBits(GPIOC, GPIO_Pin_13);//����
				printf("ON\r\n");
				SPI_Flash_Read(datatemp,FLASH_SIZE-80,Second_Size);		//�ӵ�����80����ַ����ʼ,����SIZE���ֽ�
				printf("%s\r\n",datatemp);
				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//��������ڴ�
			}
			
			if(strcmp(USART_RX_BUF,"Third_Read\r\n") == 0)//�Ա�����
			{
				GPIO_ResetBits(GPIOC, GPIO_Pin_13);//����
				printf("ON\r\n");
				
				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//��������ڴ�
			}
			
			if(strcmp(USART_RX_BUF,"Fourth_Read\r\n") == 0)//�Ա�����
			{
				memset(datatemp,0,sizeof(datatemp));
				SPI_Flash_Read(datatemp,500,256);
				printf("The 500 Address is %s\r\n",datatemp);
				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//��������ڴ�
			}
			
			if(strcmp(USART_RX_BUF,"Erase_Sector\r\n") == 0)//�Ա�����
			{
				GPIO_ResetBits(GPIOC, GPIO_Pin_13);//����
				printf("Erasing\r\n");
				SPI_Flash_Erase_Sector((FLASH_SIZE-100)/4096);				//�����������
				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//��������ڴ�
			}
			
			if(strcmp(USART_RX_BUF,"ReadSR\r\n") == 0)//�Ա�����
			{
				GPIO_ResetBits(GPIOC, GPIO_Pin_13);//����
				printf("The State Register is 0x%X\r\n",SPI_Flash_ReadSR());				//��ȡSR�Ĵ���
				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//��������ڴ�
			}
			
			if(strcmp(USART_RX_BUF,"Erase_All\r\n") == 0)//�Ա�����
			{
				GPIO_ResetBits(GPIOC, GPIO_Pin_13);//����
				SPI_Flash_Erase_Chip();
				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//��������ڴ�
			}
			
			if(strcmp(USART_RX_BUF,"ReadID\r\n") == 0)//�Ա�����
			{
				GPIO_ResetBits(GPIOC, GPIO_Pin_13);//����
				printf("The JedecDeviceID is 0x%X\r\n",SPI_Flash_Read_JedecDeviceID());				//��ȡJedecID
				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//��������ڴ�
			}
			memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//��������ڴ�
			
			USART_RX_STA = 0;
		}
	}
}
