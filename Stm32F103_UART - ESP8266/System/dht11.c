#include "stm32f10x.h"
#include "dht11.h"
#include "sys.h"
#include "Delay.h"

uint8_t  dat[5]={0x00,0x00,0x00,0x00,0x00};    //存储读取的温湿度信息
uint32_t sum=0;         //存放校验时的求和
 
void DHT_GPIO_Config_Output( void )
{		
	GPIO_InitTypeDef GPIO_InitStruct;	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); 
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}
 
void DHT_GPIO_Config_Input( void )
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); 
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}
 
 
uint8_t DHT_Read_Byte(void)
{
	uint8_t temp;  
	uint8_t ReadDat=0; 
	uint8_t t = 0;
	uint8_t i; 
 
	for(i=0;i<8;i++)
	{
		while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==0&&t<100)  
		{		
			Delay_us(1);
			t++;  //防止卡死
		}
		t=0;
		//由于‘0’代码高电平时间26~28us，1代码高电平时间70us，延时30us，可判断高低电平，数字0读取到的是低电平，高电平则反之
		Delay_us(30);
		temp=0;
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==1) temp=1;		
			
		while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==1&&t<100)
		{		
			Delay_us(1);
			t++;
		}
		t=0;
		ReadDat<<=1; 
		ReadDat|=temp;
	}	
	return ReadDat;
}
 
uint8_t DHT_Read(void)
{
	uint8_t i;
	uint8_t t = 0;
	DHT_GPIO_Config_Output();
	DHT11_LOW ;  //拉低
	Delay_ms(18);
	DHT11_HIGH;  //拉高
	Delay_us(40);
	DHT_GPIO_Config_Input();
	Delay_us(20);
	//延时20us，低电平80us，还剩60us，检查是否是低电平以确定是否有响应信号
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==0)  //如果读取到低电平，证明DHT11响应
	{
		while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==0&&t<100)//接收响应信号低电平剩余60us，等待变高电平
		{
			Delay_us(1);
			t++;			
		}
		t=0;//超过100us自动向下运行，以免卡死
		while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==1&&t<100)//接收响应信号高电平80us，等待变低电平
			{
				Delay_us(1);
				t++;			
			}
		t=0;
		for(i=0;i<5;i++)  //接收40位数据
			{
				dat[i]=DHT_Read_Byte();//读出1个字节
			}
		Delay_us(50);//结束信号
	}
	sum=dat[0]+dat[1]+dat[2]+dat[3];
	if(dat[4]==(u8)sum)  //校验  
	{
		return 1;    
	}
	else
		return 0;    
}

