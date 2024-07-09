#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "led.h"
#include "uart1.h"
#include "uart2.h"
#include "uart3.h"
#include "oled.h"
#include "rtc.h"
#include "Timer.h"
#include "HX8346.h"

uint16_t g_cSbuff = 0;

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
	char Data[50],Time[50];
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
	OLED_Clear();
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
				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//��������ڴ�
			}
				
			
			if(strcmp(USART_RX_BUF,"LED_ON\r\n") == 0)//�Ա�����
			{
				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//��������ڴ�
			}
			
			if(strcmp(USART_RX_BUF,"Get_time\r\n") == 0)//�Ա�����
			{
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