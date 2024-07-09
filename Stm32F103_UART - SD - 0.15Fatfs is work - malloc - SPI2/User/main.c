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
#include "exfuns.h"
#include "oled.h"

const unsigned char buffer[100] = "This is a new file, the data is just written in!";

int main(void)
{
	FRESULT res; // FatFs function common result code
	UINT bw; // File R/W coun
	LED_Init();
	delay_init();
	OLED_Init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// �����ж����ȼ�����2
	uart1_init(9600);
		
	//uart2_init(9600);
	//uart3_init(9600);
	while(SD_Initialize())
	{
		OLED_Clear();
		OLED_ShowString(0,0,"SD Card Error",12);
		delay_ms(500);			
		OLED_ShowString(0,2,"Please Check!",12);		
		delay_ms(500);
		LED=!LED;//PC13��˸
	}
	//���SD���ɹ� 					
	OLED_Clear();
	OLED_ShowString(0,0,"SD Card is OK",12);
	mem_init();				//��ʼ���ڴ��
	exfuns_init();    //�����ڴ�
	res = f_mount(fs[0],"0",1); 					//����SD��
	if(res==FR_OK)
		OLED_ShowString(0,2,"f_mount is ok",12);

	while (1)
	{
		if(USART_RX_STA)
		{
			delay_ms(500);
			printf("%s",USART_RX_BUF);
			if(strcmp(USART_RX_BUF,"Write\r\n") == 0)//�Ա�����
			{
				OLED_Clear();
				GPIO_SetBits(GPIOC, GPIO_Pin_13);//�ص�
				printf("Write\r\n");
				res = f_open(file,"0:text1.txt", FA_WRITE | FA_CREATE_ALWAYS);
				OLED_ShowString(0,0,"File is opened",12);
				if(res == FR_OK)
				{
					res = f_write(file, buffer, 100, &bw);//д���ʱ�����γ�ʼ��
				}
				f_close(file);
				OLED_ShowString(0,3,"Write is done",12);				
				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//��������ڴ�
			}
				
			
			if(strcmp(USART_RX_BUF,"Read\r\n") == 0)//�Ա�����
			{
				GPIO_ResetBits(GPIOC, GPIO_Pin_13);//����
				printf("Read\r\n");
				res = f_open(file,"0:text1.txt", FA_READ);
				res = f_read(file,fatbuf,100,&bw);
				f_close(file);
				OLED_ShowString(0,5,fatbuf,12);
				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//��������ڴ�
			}
			
			memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//��������ڴ�
			
			USART_RX_STA = 0;
		}
	}
}
