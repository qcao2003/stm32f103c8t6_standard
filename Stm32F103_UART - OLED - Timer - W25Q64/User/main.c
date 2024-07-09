#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "led.h"
#include "uart1.h"
#include "uart2.h"
#include "uart3.h"
#include "oled.h"
#include "rtc.h"
#include "Timer.h"
#include "spi.h"
#include "flash.h"

struct tim
{
	u16 set_year;
	u8 set_month;
	u8 set_day;
	u8 set_hours;
	u8 set_mins;
	u8 set_secs;
	u8 num;
};

struct tim t1;

int main(void)
{
	u16 id = 0;
	char Data[50],Time[50],datatemp[50];
	u32 FLASH_SIZE,i;
	t1.set_year = 2023;
	t1.set_month = 12;
	t1.set_day = 12;
	t1.set_hours = 21;
	t1.set_mins = 12;
	t1.set_secs = 30;
	LED_Init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// �����ж����ȼ�����2
	uart1_init(9600);
	OLED_Init();
	Timer_Init();
	SPI_Flash_Init();  		//SPI FLASH ��ʼ��
	OLED_Clear();
	while(1)
	{
		id = SPI_Flash_ReadID();
		printf("The W25Q64 id is 0x%X \r\n",id);
		if (id == W25Q64 || id == NM25Q64)
			break;
	}
	while(RTC_Init());
	RTC_Set(t1.set_year,t1.set_month,t1.set_day,t1.set_hours,t1.set_mins,t1.set_secs);
	while (1)
	{
		if(t1.num)
		{
			OLED_Clear();
			t1.num = 0;
		}
		sprintf(Data,"%d%s%d%s%d",calendar.w_year,"/",calendar.w_month,"/",calendar.w_date);
		sprintf(Time,"%s%d%s%d%s%d","Time is ",calendar.hour,":",calendar.min,":",calendar.sec);
		OLED_ShowString(0,0,(u8*)Data,12);
		OLED_ShowString(0,2,(u8*)Time,12);
		if(USART_RX_STA)
		{
			Delay_ms(500);
			printf("%s",USART_RX_BUF);
			if(strcmp(USART_RX_BUF,"LED_OFF\r\n") == 0)//�Ա�����
			{
				GPIO_SetBits(GPIOC, GPIO_Pin_13);//�ص�
				printf("OFF\r\n");
				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//��������ڴ�
			}
				
			
			if(strcmp(USART_RX_BUF,"LED_ON\r\n") == 0)//�Ա�����
			{
				GPIO_ResetBits(GPIOC, GPIO_Pin_13);//����
				printf("ON\r\n");
				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//��������ڴ�
			}
			
			if(strcmp(USART_RX_BUF,"Write\r\n") == 0)//�Ա�����
			{
				GPIO_ResetBits(GPIOC, GPIO_Pin_13);//����
				for(i = 0;i < FLASH_SIZE;i=i+100)
					{
						GPIO_SetBits(GPIOC, GPIO_Pin_13);//�ص�
						RTC_Get();
						sprintf(Data,"%s%d%s%d%s%d%s","Today is ",calendar.w_year,"/",calendar.w_month,"/",calendar.w_date,"\r\n");
						sprintf(Time,"%s%d%s%d%s%d%s","Time is ",calendar.hour,":",calendar.min,":",calendar.sec,"\r\n");
						Write_page_with_check_Sector((u8*)Data,i,50);		//д�뵽1000����ַ�У�������50���ֽ�
						Write_page_with_check_Sector((u8*)Time,i+50,50);		//д�뵽1500����ַ�У�������50���ֽ�
						GPIO_ResetBits(GPIOC, GPIO_Pin_13);//����
						SPI_Flash_Read((u8*)datatemp,i,50);
						printf("The %d Address is %s\r\n",i,datatemp);
						SPI_Flash_Read((u8*)datatemp,i+50,50);
						printf("The %d Address is %s\r\n",i+50,datatemp);
					}
				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//��������ڴ�
			}
			memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//��������ڴ�
			
			USART_RX_STA = 0;
		}
	}
}

void TIM2_IRQHandler(void)  //TM2���жϺ���
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		t1.num++;
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
