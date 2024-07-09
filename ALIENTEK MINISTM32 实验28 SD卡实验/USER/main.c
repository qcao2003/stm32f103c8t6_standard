#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "lcd.h"
#include "key.h"
#include "malloc.h"  
#include "MMC_SD.h"   
//ALIENTEK Mini STM32开发板范例代码28
//SD卡实验  
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司 

//读取SD卡的指定扇区的内容，并通过串口1输出
//sec：扇区物理地址编号
void SD_Read_Sectorx(u32 sec)
{
	u8 *buf;
	u16 i;
	buf=mymalloc(512);				//申请内存
	if(SD_ReadDisk(buf,sec,1)==0)	//读取0扇区的内容
	{	
		LCD_ShowString(60,190,200,16,16,"USART1 Sending Data...");
		printf("SECTOR 0 DATA:\r\n");
		for(i=0;i<512;i++)printf("%x ",buf[i]);//打印sec扇区数据    	   
		printf("\r\nDATA ENDED\r\n");
		LCD_ShowString(60,190,200,16,16,"USART1 Send Data Over!");
	}
	myfree(buf);//释放内存	
}
	
//ALIENTEK Mini STM32开发板范例代码11
//TFTLCD显示实验   
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司 
 int main(void)
 { 
//	u8 key;		 
	u32 sd_size;
//	u8 t=0;	 
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	delay_init();	    	 //延时函数初始化	  
	uart_init(9600);	 	//串口初始化为9600
	LED_Init();		  		//初始化与LED连接的硬件接口
//	LCD_Init();			   	//初始化LCD	
	printf("Welcome\r\n");
// 	KEY_Init();				//按键初始化  
 	mem_init();				//初始化内存池	    
//while(SD_Initialize())//检测不到SD卡
	while(1)//检测不到SD卡
	{
		printf("SD Card Error!\r\n");
		delay_ms(500);					
		printf("Please Check! \r\n");
		delay_ms(500);
		LED0=!LED0;//DS0闪烁
	}
 	POINT_COLOR=BLUE;//设置字体为蓝色 
	//检测SD卡成功 											    
	printf("SD Card OK\r\n");
	printf("SD Card Size %d MB\r\n",sd_size>>11);
//	sd_size=SD_GetSectorCount();//得到扇区数
	while(1)
	{
//		key=KEY_Scan(0);
//		if(key==KEY0_PRES)SD_Read_Sectorx(0);//KEY0按,读取SD卡扇区0的内容
//		t++;
//		delay_ms(10);
//		if(t==20)
//		{
//			LED0=!LED0;
//			t=0;
//		}
	}    	   
}






