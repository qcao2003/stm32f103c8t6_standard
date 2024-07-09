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
//��ȡSD����ָ�����������ݣ���ͨ������1���
//sec�����������ַ���
void SD_Read_Sectorx(u32 sec)
{
	u8 *buf;
	u16 i;
	buf=mymalloc(512);				//�����ڴ�
	if(SD_ReadDisk(buf,sec,1)==0)	//��ȡ0����������
	{	
		printf("SECTOR 0 DATA:\r\n");
		for(i=0;i<512;i++)printf("%x ",buf[i]);//��ӡsec��������    	   
		printf("\r\nDATA ENDED\r\n");
		printf("USART1 Send Data Over!\r\n");
	}
	myfree(buf);//�ͷ��ڴ�	
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
	exfuns_init(); //Ϊfatfs��ر��������ڴ�
	f_mount(fs[0],"0:",1); 					//����SD��
	while(exf_getfree("0",&total,&free))	//�õ�SD������������ʣ������
	{
		printf("Fatfs Error!\r\n");
		delay_ms(500);					
		printf("Please Check! \r\n");
		delay_ms(500);
		LED=!LED;//PC13��˸
	}
	while (1)
	{
		if(USART_RX_STA)
		{
			delay_ms(500);
			printf("%s",USART_RX_BUF);
			if(strcmp(USART_RX_BUF,"LED_OFF\r\n") == 0)//�Ա�����
			{
				GPIO_SetBits(GPIOC, GPIO_Pin_13);//�ص�
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
				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//��������ڴ�
			}
				
			
			if(strcmp(USART_RX_BUF,"LED_ON\r\n") == 0)//�Ա�����
			{
				GPIO_ResetBits(GPIOC, GPIO_Pin_13);//����
				printf("ON\r\n");
				sd_size=SD_GetSectorCount();//�õ�������
				printf("SD Card Size %d MB\r\n",sd_size>>11);
				printf("SD_CID is %d \r\n",SD_GetCID(cid));
				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//��������ڴ�
			}
			
			memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//��������ڴ�
			
			USART_RX_STA = 0;
		}
		//GPIO_ResetBits(GPIOC, GPIO_Pin_13);
		//Delay_ms(500);
		//GPIO_SetBits(GPIOC, GPIO_Pin_13);
		//Delay_ms(500);
	}
}
