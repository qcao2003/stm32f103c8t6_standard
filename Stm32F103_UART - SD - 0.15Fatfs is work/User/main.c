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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// �����ж����ȼ�����2
	uart1_init(9600);
	mem_init();				//��ʼ���ڴ��	
	//uart2_init(9600);
	//uart3_init(9600);
	while(SD_Initialize())
	{
		printf("SD Card Error!\r\n");
		delay_ms(500);					
		printf("Please Check! \r\n");
		delay_ms(500);
		LED=!LED;//PC13��˸
	}
	//���SD���ɹ� 											    
	printf("SD Card OK\r\n");
	res = f_mount(&fatfs,"0",1); 					//����SD��
	if(res==FR_OK)
		printf("f_mount is ok\r\n");

	while (1)
	{
		if(USART_RX_STA)
		{
			delay_ms(500);
			printf("%s",USART_RX_BUF);
			if(strcmp(USART_RX_BUF,"Write\r\n") == 0)//�Ա�����
			{
				GPIO_SetBits(GPIOC, GPIO_Pin_13);//�ص�
				printf("Write\r\n");
				res = f_open(&fsrc,"0:text1.txt", FA_WRITE | FA_CREATE_ALWAYS);
				printf("File is opened\r\n");
				if(res == FR_OK)
				{
					res = f_write(&fsrc, buffer, 100, &bw);//д���ʱ�����γ�ʼ��
				}
				f_close(&fsrc);		
				printf("Write is done\r\n");
				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//��������ڴ�
			}
				
			
			if(strcmp(USART_RX_BUF,"Read\r\n") == 0)//�Ա�����
			{
				GPIO_ResetBits(GPIOC, GPIO_Pin_13);//����
				printf("Read\r\n");
				res = f_open(&fsrc,"0:text1.txt", FA_READ);
				res = f_read(&fsrc,buff0,100,&bw);
				f_close(&fsrc);
				printf("%s\r\n",(char*)buff0);
				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//��������ڴ�
			}
			
			memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//��������ڴ�
			
			USART_RX_STA = 0;
		}
	}
}
