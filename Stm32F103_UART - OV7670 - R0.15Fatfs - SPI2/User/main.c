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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);// �����ж����ȼ�����3
	uart1_init(9600);
	delay_init();
	while(SD_Initialize())
	{
		printf("SD Card Error\r\n");
		delay_ms(500);			
		printf("Please Check!\r\n");	
		delay_ms(500);
		LED=!LED;//PC13��˸
	}
	OV7670_Init();
	EXTI_InitPA15();
	mem_init();				//��ʼ���ڴ��
	exfuns_init();    //�����ڴ�
	res = f_mount(fs[0],"0",1); 					//����SD��
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
				res = f_open(file,"0:text1.txt", FA_WRITE | FA_CREATE_ALWAYS);
				printf("File is opened\r\n");
				if(res == FR_OK)
				{
					res = f_write(file, buffer, 100, &bw);//д���ʱ�����γ�ʼ��
				}
				f_close(file);
				printf("Write is done\r\n");				
				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//��������ڴ�
			}
				
			
			if(strcmp(USART_RX_BUF,"Read\r\n") == 0)//�Ա�����
			{
				GPIO_ResetBits(GPIOC, GPIO_Pin_13);//����
				printf("Read\r\n");
				res = f_open(file,"0:text1.txt", FA_READ);
				res = f_read(file,fatbuf,100,&br);
				f_close(file);
				printf("%s\r\n",fatbuf);
				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//��������ڴ�
			}
			
			if(strcmp(USART_RX_BUF,"Write_Pic\r\n") == 0)//�Ա�����
			{
				OV7670_State = 0;//���½��뵽����FIFO״̬
				GPIO_ResetBits(GPIOC, GPIO_Pin_13);//����
				printf("Write_Pic\r\n");
				OV7670_STM32GetPictoSDCARD(240,320,file,res,bw,"0:test3.bmp");
				printf("Write_Pic is completed\r\n");
				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//��������ڴ�
			}
			
			memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//��������ڴ�
			
			USART_RX_STA = 0;
		}
	}
}
