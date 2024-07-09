#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "led.h"
#include "uart1.h"
#include "uart2.h"
#include "uart3.h"
#include "oled.h"
#include "rtc.h"
#include "Timer.h"
#include "mmc_sd.h"
#include "malloc.h" 
#include "ff.h"
#include "exfuns.h"
#include "fattester.h"

const unsigned char buffer[100] = "This is a new file, the data is just written in!";
	
struct tim
{
	u16 set_year;
	u8 set_month;
	u8 set_day;
	u8 set_hours;
	u8 set_mins;
	u8 set_secs;
	u8 week_num;
	char* Weeks;
	u8 num;
};

struct tim t1;

//BMPͷ�ļ�
typedef __packed struct
{
    u16  bfType ;     //�ļ���־.ֻ��'BM',����ʶ��BMPλͼ����
    u32  bfSize ;     //�ļ���С,ռ�ĸ��ֽ�
    u16  bfReserved1 ;//����
    u16  bfReserved2 ;//����
    u32  bfOffBits ;  //���ļ���ʼ��λͼ����(bitmap data)��ʼ֮��ĵ�ƫ����
}BITMAPFILEHEADER ;
//BMP��Ϣͷ
typedef __packed struct
{
    u32 biSize ;                //˵��BITMAPINFOHEADER�ṹ����Ҫ��������
    long  biWidth ;         //˵��ͼ��Ŀ�ȣ�������Ϊ��λ 
    long  biHeight ;        //˵��ͼ��ĸ߶ȣ�������Ϊ��λ 
    u16  biPlanes ;         //ΪĿ���豸˵��λ��������ֵ�����Ǳ���Ϊ1 
    u16  biBitCount ;       //˵��������/���أ���ֵΪ1��4��8��16��24����32
/*˵��ͼ������ѹ�������͡���ֵ����������ֵ֮һ��
BI_RGB��û��ѹ����
BI_RLE8��ÿ������8���ص�RLEѹ�����룬ѹ����ʽ��2�ֽ����(�ظ����ؼ�������ɫ����)��  
BI_RLE4��ÿ������4���ص�RLEѹ�����룬ѹ����ʽ��2�ֽ����
BI_BITFIELDS��ÿ�����صı�����ָ�������������*/
    u32 biCompression ;     
    u32 biSizeImage ;       //˵��ͼ��Ĵ�С�����ֽ�Ϊ��λ������BI_RGB��ʽʱ��������Ϊ0  
    long  biXPelsPerMeter ; //˵��ˮƽ�ֱ��ʣ�������/�ױ�ʾ
    long  biYPelsPerMeter ; //˵����ֱ�ֱ��ʣ�������/�ױ�ʾ
    u32 biClrUsed ;         //˵��λͼʵ��ʹ�õĲ�ɫ���е���ɫ������
    u32 biClrImportant ;    //˵����ͼ����ʾ����ҪӰ�����ɫ��������Ŀ�������0����ʾ����Ҫ�� 
}BITMAPINFOHEADER ;
//��ɫ�� 
typedef __packed struct 
{
    u8 rgbBlue ;    //ָ����ɫǿ��
    u8 rgbGreen ;   //ָ����ɫǿ�� 
    u8 rgbRed ;     //ָ����ɫǿ�� 
    u8 rgbReserved ;//����������Ϊ0 
}RGBQUAD ;
//������Ϣͷ
typedef __packed struct
{ 
    BITMAPFILEHEADER bmfHeader;
    BITMAPINFOHEADER bmiHeader;  
    RGBQUAD RGB_MASK[3];            //��ɫ�����ڴ��RGB����.
}BITMAPINFO; 

int main(void)
{
	u32 total,free,i,j;

	FIL fsrc,fnew; // file objects
	 
	FRESULT res; // FatFs function common result code
	
	BITMAPINFO bmp;
	 
	UINT bw,fnum; // File R/W coun
	//u32 sd_size;
	//u8 cid[16];
	char Data[50],Time[50],Week[50];
	u16 color = 0XC080;
	t1.set_year = 2023;
	t1.set_month = 12;
	t1.set_day = 16;
	t1.set_hours = 0;
	t1.set_mins = 0;
	t1.set_secs = 0;
	LED_Init();
	OLED_Init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// �����ж����ȼ�����2
	uart1_init(9600);
	while(SD_Initialize())
	{
		printf("SD Card Error!\r\n");
		Delay_ms(500);	
		printf("Please Check! \r\n");
		Delay_ms(500);
		LED=!LED;//PC13��˸
	}
	OLED_ShowString(0,1,"SD Card OK",12);
	printf("SD Card OK\r\n");
	exfuns_init(); //Ϊfatfs��ر��������ڴ�
	mem_init();
	f_mount(fs[0],"0:",1); 					//����SD��
	while(exf_getfree("0",&total,&free))	//�õ�SD������������ʣ������
	{
		printf("Fatfs Error!\r\n");
		Delay_ms(500);
		printf("Please Check! \r\n");
		Delay_ms(500);
		LED=!LED;//PC13��˸
	}
	Timer_Init();
	OLED_Clear();
	while(RTC_Init());
	RTC_Set(t1.set_year,t1.set_month,t1.set_day,t1.set_hours,t1.set_mins,t1.set_secs);
	t1.week_num = RTC_Get_Week(t1.set_year,t1.set_month,t1.set_day);
	
	while (1)
	{
		if(t1.num)
		{
			OLED_Clear();
			t1.num = 0;
		}
		sprintf(Data,"%d%s%d%s%d",calendar.w_year,"/",calendar.w_month,"/",calendar.w_date);
		sprintf(Time,"%s%d%s%d%s%d","Time is ",calendar.hour,":",calendar.min,":",calendar.sec);
		sprintf(Week,"%s%s","Today's ",WEEKS[t1.week_num-1]);
		OLED_ShowString(0,0,(u8*)Data,12);
		OLED_ShowString(0,2,(u8*)Time,12);
		OLED_ShowString(0,4,(u8*)Week,12);
		OLED_ShowString(0,6,"Write Completed",12);
		if(USART_RX_STA)
		{
			Delay_ms(500);
			printf("%s",USART_RX_BUF);
			if(strcmp(USART_RX_BUF,"Write_Word\r\n") == 0)//�Ա�����
			{
				GPIO_SetBits(GPIOC, GPIO_Pin_13);//�ص�
				printf("OFF\r\n");
				res = f_open(&fsrc,"0:data.txt", FA_WRITE | FA_CREATE_ALWAYS);
				if(res == FR_OK)
				{
					res = f_write(&fsrc, buffer, 100, &bw);
				}
				while(bw < 100);
				f_close(&fsrc);	
				
				OLED_ShowString(0,6,"Write OK!",12);
				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//��������ڴ�
			}
				
			
			if(strcmp(USART_RX_BUF,"Write_Pic\r\n") == 0)//�Ա�����
			{
				GPIO_ResetBits(GPIOC, GPIO_Pin_13);//����
				printf("ON\r\n");
				
				res = f_open(&fnew, "0:test1.bmp", FA_OPEN_ALWAYS | FA_WRITE | FA_READ);
				//�ļ��򿪳ɹ�
				if(res == FR_OK)
				{
						//��д�ļ���Ϣͷ��Ϣ  
						bmp.bmfHeader.bfType = 0x4D42;          //bmp����  "BM"
						bmp.bmfHeader.bfSize= 54 + 320*240*2;   //�ļ���С����Ϣ�ṹ��+�������ݣ�
						bmp.bmfHeader.bfReserved1 = 0x0000;     //����������Ϊ0
						bmp.bmfHeader.bfReserved2 = 0x0000;             
						bmp.bmfHeader.bfOffBits=54;             //λͼ��Ϣ�ṹ����ռ���ֽ���

						//��дλͼ��Ϣͷ��Ϣ  
						bmp.bmiHeader.biSize=40;            //λͼ��Ϣͷ�Ĵ�С
						bmp.bmiHeader.biWidth=320;          //λͼ�Ŀ��
						bmp.bmiHeader.biHeight=240;         //ͼ��ĸ߶�
						bmp.bmiHeader.biPlanes=1;           //Ŀ�����ļ��𣬱�����1
						bmp.bmiHeader.biBitCount=16;        //ÿ����λ��
						bmp.bmiHeader.biCompression=3;      //RGB555��ʽ
						bmp.bmiHeader.biSizeImage=320*240*2;//ʵ��λͼ��ռ�õ��ֽ�����������λͼ�������ݣ�
						bmp.bmiHeader.biXPelsPerMeter=0;    //ˮƽ�ֱ���
						bmp.bmiHeader.biYPelsPerMeter=0;    //��ֱ�ֱ���
						bmp.bmiHeader.biClrImportant=0;     //˵��ͼ����ʾ����ҪӰ�����ɫ������Ŀ��0�������е���ɫһ����Ҫ
						bmp.bmiHeader.biClrUsed=0;          //λͼʵ��ʹ�õĲ�ɫ���е���ɫ��������0��ʾʹ�����еĵ�ɫ����

						//RGB565��ʽ����
						bmp.RGB_MASK[0].rgbBlue = 0;
						bmp.RGB_MASK[0].rgbGreen = 0xF8;
						bmp.RGB_MASK[0].rgbRed = 0;
						bmp.RGB_MASK[0].rgbReserved = 0;

						bmp.RGB_MASK[1].rgbBlue = 0xE0;
						bmp.RGB_MASK[1].rgbGreen = 0x07;
						bmp.RGB_MASK[1].rgbRed = 0;
						bmp.RGB_MASK[1].rgbReserved = 0;

						bmp.RGB_MASK[2].rgbBlue = 0x1F;
						bmp.RGB_MASK[2].rgbGreen = 0;
						bmp.RGB_MASK[2].rgbRed = 0;
						bmp.RGB_MASK[2].rgbReserved = 0;

						//д�ļ�ͷ���ļ�  
						res = f_write(&fnew, &bmp, sizeof(bmp), &fnum);
						
						for(i=0;i<240;i++)
						{
								for(j=0;j<320;j++)
								{               
//										GPIOB->CRL=0X88888888;
//										OV7725_RCK=0;
//										color=OV7725_DATA;  //������--��8λ

//										OV7725_RCK=1; 
//										color<<=8;  
//										OV7725_RCK=0;
//										color|=OV7725_DATA; //������   --��8λ       (�ߵ�8+8λ�ϲ���һ��u16����)                              
//										OV7725_RCK=1;
//										GPIOB->CRL=0X33333333;
										//LCD��ʾ
//										LCD_WR_DATA(color);
										//дλͼ��Ϣͷ���ڴ濨
										f_write(&fnew, &color, sizeof(color), &fnum);
								}
						}
						//while(fnum < sizeof(color));
						f_close(&fnew);	
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
