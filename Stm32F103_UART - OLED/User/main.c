#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "led.h"
#include "uart1.h"
#include "uart2.h"
#include "uart3.h"
#include "oled.h"
#include "rtc.h"

struct tim
{
	u16 set_year;
	u8 set_month;
	u8 set_day;
	u8 set_hours;
	u8 set_mins;
	u8 set_secs;
};

int main(void)
{
	char Data[50],Time[50];
	struct tim t1;
	t1.set_year = 2023;
	t1.set_month = 12;
	t1.set_day = 11;
	t1.set_hours = 16;
	t1.set_mins = 59;
	t1.set_secs = 59;
	LED_Init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	uart1_init(9600);
	OLED_Init();
	//uart2_init(9600);
	//uart3_init(9600);
	OLED_Clear();
	while(RTC_Init());
	RTC_Set(t1.set_year,t1.set_month,t1.set_day,t1.set_hours,t1.set_mins,t1.set_secs);
	while (1)
	{
		if(calendar.w_month < 10 ||calendar.w_date < 10 ||calendar.hour < 10||calendar.min < 10||calendar.sec < 10) //当这数字个位的时候就刷新
			OLED_Clear();
		sprintf(Data,"%d%s%d%s%d",calendar.w_year,"/",calendar.w_month,"/",calendar.w_date);
		sprintf(Time,"%s%d%s%d%s%d","Time is ",calendar.hour,":",calendar.min,":",calendar.sec);
		OLED_ShowString(0,0,(u8*)Data,12);
		OLED_ShowString(0,2,(u8*)Time,12);
		if(USART_RX_STA)
		{
			Delay_ms(500);
			printf("%s",USART_RX_BUF);
			if(strcmp(USART_RX_BUF,"LED_OFF\r\n") == 0)//对比数组
			{
				GPIO_SetBits(GPIOC, GPIO_Pin_13);//关灯
				printf("OFF\r\n");
				OLED_ShowString(0,1,"ALAN WAKE",12);
				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//清空数组内存
			}
				
			
			if(strcmp(USART_RX_BUF,"LED_ON\r\n") == 0)//对比数组
			{
				RTC_Get();
//				sprintf(Data,"%s%d%s%d%s%d%s","Today is ",calendar.w_year,"/",calendar.w_month,"/",calendar.w_date,"\r\n");
//				sprintf(Time,"%s%d%s%d%s%d%s","Time is ",calendar.hour,":",calendar.min,":",calendar.sec,"\r\n");
				GPIO_ResetBits(GPIOC, GPIO_Pin_13);//开灯
				printf("ON\r\n");
				OLED_ShowString(0,1,"John Sherry",12);
				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//清空数组内存
			}
			
			if(strcmp(USART_RX_BUF,"Get_time\r\n") == 0)//对比数组
			{
				RTC_Get();
//				sprintf(Data,"%s%d%s%d%s%d","Today is ",calendar.w_year,"/",calendar.w_month,"/",calendar.w_date);
//				sprintf(Time,"%s%d%s%d%s%d","Time is ",calendar.hour,":",calendar.min,":",calendar.sec);
				GPIO_ResetBits(GPIOC, GPIO_Pin_13);//开灯
				printf("ON\r\n");
//				OLED_ShowString(0,0,(u8*)Data,12);
//				OLED_ShowString(0,2,(u8*)Time,12);
				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//清空数组内存
			}
			memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//清空数组内存
			
			USART_RX_STA = 0;
		}
	}
}
