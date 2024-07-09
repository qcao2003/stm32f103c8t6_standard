//////////////////////////////////////////////////////////////////////////////////	 
//�����콢�� 1.8����ֿ�TFT���Գ���   
//���̵�ַ��http://tmall.lvsn.com
//����ƽ̨��STM32F405RGT6
//              ----------------------------------------------------------------
//              GND   ��Դ��
//              VCC   ��Դ������3.3-5V��
//              SCL   PB9��SCLK��
//              SDA   PB8��MOSI��
//              CS    PB7
//              SDO   PB6��MISO��
//              DC    PB5
//              BLK   PB4
//              ----------------------------------------------------------------
//******************************************************************************/
#include "delay.h"
#include "sys.h"
#include "led.h"
#include "lcd_init.h"
#include "lcd.h"
#include "OV7670.h"
#include "SCCB.h"
#include "usart.h"
#include "pic.h"

uint8_t OV7670_State=0;
uint8_t pic[128*160*2];
int main(void)
{ 
	uart_init(115200);
	delay_init(168);
	LED_Init();//LED��ʼ��
	LCD_Init();//LCD��ʼ��
	LCD_Fill(0,0,128,160,WHITE);
	OV7670_Init();
	while(1) 
	{
		OV7670_STM32GetPic();
		//LCD_ShowPicture(65,80,40,40,gImage_1);
		LCD_ShowPicture(0,0,128,160,pic);
	}
}


