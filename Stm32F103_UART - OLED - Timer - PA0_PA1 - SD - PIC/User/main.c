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

//BMP头文件
typedef __packed struct
{
    u16  bfType ;     //文件标志.只对'BM',用来识别BMP位图类型
    u32  bfSize ;     //文件大小,占四个字节
    u16  bfReserved1 ;//保留
    u16  bfReserved2 ;//保留
    u32  bfOffBits ;  //从文件开始到位图数据(bitmap data)开始之间的的偏移量
}BITMAPFILEHEADER ;
//BMP信息头
typedef __packed struct
{
    u32 biSize ;                //说明BITMAPINFOHEADER结构所需要的字数。
    long  biWidth ;         //说明图象的宽度，以象素为单位 
    long  biHeight ;        //说明图象的高度，以象素为单位 
    u16  biPlanes ;         //为目标设备说明位面数，其值将总是被设为1 
    u16  biBitCount ;       //说明比特数/象素，其值为1、4、8、16、24、或32
/*说明图象数据压缩的类型。其值可以是下述值之一：
BI_RGB：没有压缩；
BI_RLE8：每个象素8比特的RLE压缩编码，压缩格式由2字节组成(重复象素计数和颜色索引)；  
BI_RLE4：每个象素4比特的RLE压缩编码，压缩格式由2字节组成
BI_BITFIELDS：每个象素的比特由指定的掩码决定。*/
    u32 biCompression ;     
    u32 biSizeImage ;       //说明图象的大小，以字节为单位。当用BI_RGB格式时，可设置为0  
    long  biXPelsPerMeter ; //说明水平分辨率，用象素/米表示
    long  biYPelsPerMeter ; //说明垂直分辨率，用象素/米表示
    u32 biClrUsed ;         //说明位图实际使用的彩色表中的颜色索引数
    u32 biClrImportant ;    //说明对图象显示有重要影响的颜色索引的数目，如果是0，表示都重要。 
}BITMAPINFOHEADER ;
//彩色表 
typedef __packed struct 
{
    u8 rgbBlue ;    //指定蓝色强度
    u8 rgbGreen ;   //指定绿色强度 
    u8 rgbRed ;     //指定红色强度 
    u8 rgbReserved ;//保留，设置为0 
}RGBQUAD ;
//整体信息头
typedef __packed struct
{ 
    BITMAPFILEHEADER bmfHeader;
    BITMAPINFOHEADER bmiHeader;  
    RGBQUAD RGB_MASK[3];            //调色板用于存放RGB掩码.
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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	uart1_init(9600);
	while(SD_Initialize())
	{
		printf("SD Card Error!\r\n");
		Delay_ms(500);	
		printf("Please Check! \r\n");
		Delay_ms(500);
		LED=!LED;//PC13闪烁
	}
	OLED_ShowString(0,1,"SD Card OK",12);
	printf("SD Card OK\r\n");
	exfuns_init(); //为fatfs相关变量申请内存
	mem_init();
	f_mount(fs[0],"0:",1); 					//挂载SD卡
	while(exf_getfree("0",&total,&free))	//得到SD卡的总容量和剩余容量
	{
		printf("Fatfs Error!\r\n");
		Delay_ms(500);
		printf("Please Check! \r\n");
		Delay_ms(500);
		LED=!LED;//PC13闪烁
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
			if(strcmp(USART_RX_BUF,"Write_Word\r\n") == 0)//对比数组
			{
				GPIO_SetBits(GPIOC, GPIO_Pin_13);//关灯
				printf("OFF\r\n");
				res = f_open(&fsrc,"0:data.txt", FA_WRITE | FA_CREATE_ALWAYS);
				if(res == FR_OK)
				{
					res = f_write(&fsrc, buffer, 100, &bw);
				}
				while(bw < 100);
				f_close(&fsrc);	
				
				OLED_ShowString(0,6,"Write OK!",12);
				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//清空数组内存
			}
				
			
			if(strcmp(USART_RX_BUF,"Write_Pic\r\n") == 0)//对比数组
			{
				GPIO_ResetBits(GPIOC, GPIO_Pin_13);//开灯
				printf("ON\r\n");
				
				res = f_open(&fnew, "0:test1.bmp", FA_OPEN_ALWAYS | FA_WRITE | FA_READ);
				//文件打开成功
				if(res == FR_OK)
				{
						//填写文件信息头信息  
						bmp.bmfHeader.bfType = 0x4D42;          //bmp类型  "BM"
						bmp.bmfHeader.bfSize= 54 + 320*240*2;   //文件大小（信息结构体+像素数据）
						bmp.bmfHeader.bfReserved1 = 0x0000;     //保留，必须为0
						bmp.bmfHeader.bfReserved2 = 0x0000;             
						bmp.bmfHeader.bfOffBits=54;             //位图信息结构体所占的字节数

						//填写位图信息头信息  
						bmp.bmiHeader.biSize=40;            //位图信息头的大小
						bmp.bmiHeader.biWidth=320;          //位图的宽度
						bmp.bmiHeader.biHeight=240;         //图像的高度
						bmp.bmiHeader.biPlanes=1;           //目标设别的级别，必须是1
						bmp.bmiHeader.biBitCount=16;        //每像素位数
						bmp.bmiHeader.biCompression=3;      //RGB555格式
						bmp.bmiHeader.biSizeImage=320*240*2;//实际位图所占用的字节数（仅考虑位图像素数据）
						bmp.bmiHeader.biXPelsPerMeter=0;    //水平分辨率
						bmp.bmiHeader.biYPelsPerMeter=0;    //垂直分辨率
						bmp.bmiHeader.biClrImportant=0;     //说明图像显示有重要影响的颜色索引数目，0代表所有的颜色一样重要
						bmp.bmiHeader.biClrUsed=0;          //位图实际使用的彩色表中的颜色索引数，0表示使用所有的调色板项

						//RGB565格式掩码
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

						//写文件头进文件  
						res = f_write(&fnew, &bmp, sizeof(bmp), &fnum);
						
						for(i=0;i<240;i++)
						{
								for(j=0;j<320;j++)
								{               
//										GPIOB->CRL=0X88888888;
//										OV7725_RCK=0;
//										color=OV7725_DATA;  //读数据--高8位

//										OV7725_RCK=1; 
//										color<<=8;  
//										OV7725_RCK=0;
//										color|=OV7725_DATA; //读数据   --低8位       (高低8+8位合并成一个u16发送)                              
//										OV7725_RCK=1;
//										GPIOB->CRL=0X33333333;
										//LCD显示
//										LCD_WR_DATA(color);
										//写位图信息头进内存卡
										f_write(&fnew, &color, sizeof(color), &fnum);
								}
						}
						//while(fnum < sizeof(color));
						f_close(&fnew);	
				}
				memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//清空数组内存
			}
			
			memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));//清空数组内存
			
			USART_RX_STA = 0;
		}
	}
}

void TIM2_IRQHandler(void)  //TM2的中断函数
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		t1.num++;
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
