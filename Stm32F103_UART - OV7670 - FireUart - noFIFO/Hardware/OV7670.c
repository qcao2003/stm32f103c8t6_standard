#include "OV7670.h"
#include "ov7670config.h"

//uint8_t OV7670_State;

uint8_t OV7670_VS(void)
{
	return GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13);
}


uint8_t OV7670_HREF(void)
{
    return GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14);
}

uint8_t OV7670_PCLK(void)
{
    return GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15);
}
/*  
初始化GPIO口
*/
void OV7670_GPIO_Init(void)
{
		//设置IO     	   
	GPIO_InitTypeDef  GPIO_InitStruct;                   /* 结构体定义 */
	//PB3 PWDN PB4 RST
	//@RST                3.3V                     0：RESET模式    1：一般模式
	//@PWNN                GND                        0：工作         1：POWER DOWN
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	//GPIOB3,4初始化设置
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4;//PB3,4推挽输出
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//50MHz
	GPIO_Init(GPIOB, &GPIO_InitStruct);//初始化
	
	/* VS HREF 和 PCLK IO口初始化 */
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);        /* GPIOB使能 */
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource15);	//PA15 连接到中断线1

	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;                   /* PB13,14,15 */
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;                   /* 上拉输入 */
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;                   /*  50MHz*/
	GPIO_Init(GPIOB,&GPIO_InitStruct); 
	
	/* D0-D7 IO口初始化 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);        /* GPIOA使能 */

	GPIO_InitStruct.GPIO_Pin=0xFF;                   /* PA0-PA7 */
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;                   /* 上拉输入 */
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;                   /*  50MHz*/
	GPIO_Init(GPIOA,&GPIO_InitStruct); 
	
	GPIO_WriteBit(GPIOB, GPIO_Pin_3, Bit_RESET); //PWNN                GND
	GPIO_WriteBit(GPIOB, GPIO_Pin_4, Bit_SET);	//RST                3.3V 
}
//初始化OV7670
//配置完以后,默认输出是320*240尺寸的图片!! 
//返回值:0,成功
//    其他,错误代码
u8 OV7670_Init(void)
{ 
	u16 i=0;
	u16 reg;
	OV7670_GPIO_Init();
	OV7670_PWDN_L;	//POWER ON
	Delay_ms(10);
	OV7670_RST_L;	//复位OV7670
	Delay_ms(10);
	OV7670_RST_H;	//结束复位 
	SCCB_Init();        		//初始化SCCB 的IO口	 
	SCCB_WR_Reg(0X12, 0x80);	//软复位OV7670
	Delay_ms(50); 
	reg=SCCB_RD_Reg(0X1C);	//读取厂家ID 高八位
	reg<<=8;
	reg|=SCCB_RD_Reg(0x1D);	//读取厂家ID 低八位
	
	if(reg!=OV7670_MID)
	{
		printf("MID:0X%X\r\n",reg);
		return 1;
	}
	reg=SCCB_RD_Reg(0x0A);	//读取厂家ID 高八位
	reg<<=8;
	reg|=SCCB_RD_Reg(0x0B);	//读取厂家ID 低八位
	if(reg!=OV7670_PID)
	{
		printf("HID:0X%X\r\n",reg);
		return 2;
	}   
	
	//初始化 OV7670,采用QVGA分辨率(320*240)  
	for(i=0;i<sizeof(ov7670_init_reg_tb1)/sizeof(ov7670_init_reg_tb1[0]);i++)
	{
	   	SCCB_WR_Reg(ov7670_init_reg_tb1[i][0], ov7670_init_reg_tb1[i][1]);
			
 	} 
//	OV7670_Light_Mode(3);
//	OV7670_Color_Saturation(2);
//	OV7670_Brightness(2);
//	OV7670_Contrast(2);
//	OV7670_Special_Effects(0);
	
//	OV7670_config_window(200,50,160,128);
	
//	OV7670_Self_Window_Set();
	
//	OV7670_config_window(4,0,320,240);
	
	printf("OV7670init is Successful\r\n");
	
	return 0x00; 	//ok
} 

////////////////////////////////////////////////////////////////////////////
//OV7670功能设置
//白平衡设置
//0:自动
//1:太阳sunny
//2,阴天cloudy
//3,办公室office
//4,家里home
void OV7670_Light_Mode(u8 mode)
{
	u8 reg13val=0XE7;//默认就是设置为自动白平衡
	u8 reg01val=0;
	u8 reg02val=0;
	switch(mode)
	{
		case 1://sunny
			reg13val=0XE5;
			reg01val=0X5A;
			reg02val=0X5C;
			break;	
		case 2://cloudy
			reg13val=0XE5;
			reg01val=0X58;
			reg02val=0X60;
			break;	
		case 3://office
			reg13val=0XE5;
			reg01val=0X84;
			reg02val=0X4c;
			break;	
		case 4://home
			reg13val=0XE5;
			reg01val=0X96;
			reg02val=0X40;
			break;	
	}
	SCCB_WR_Reg(0X13,reg13val);//COM8设置 
	SCCB_WR_Reg(0X01,reg01val);//AWB蓝色通道增益 
	SCCB_WR_Reg(0X02,reg02val);//AWB红色通道增益 
}				  
//色度设置
//0:-2
//1:-1
//2,0
//3,1
//4,2
void OV7670_Color_Saturation(u8 sat)
{
	u8 reg4f5054val=0X80;//默认就是sat=2,即不调节色度的设置
 	u8 reg52val=0X22;
	u8 reg53val=0X5E;
 	switch(sat)
	{
		case 0://-2
			reg4f5054val=0X40;  	 
			reg52val=0X11;
			reg53val=0X2F;	 	 
			break;	
		case 1://-1
			reg4f5054val=0X66;	    
			reg52val=0X1B;
			reg53val=0X4B;	  
			break;	
		case 3://1
			reg4f5054val=0X99;	   
			reg52val=0X28;
			reg53val=0X71;	   
			break;	
		case 4://2
			reg4f5054val=0XC0;	   
			reg52val=0X33;
			reg53val=0X8D;	   
			break;	
	}
	SCCB_WR_Reg(0X4F,reg4f5054val);	//色彩矩阵系数1
	SCCB_WR_Reg(0X50,reg4f5054val);	//色彩矩阵系数2 
	SCCB_WR_Reg(0X51,0X00);			//色彩矩阵系数3  
	SCCB_WR_Reg(0X52,reg52val);		//色彩矩阵系数4 
	SCCB_WR_Reg(0X53,reg53val);		//色彩矩阵系数5 
	SCCB_WR_Reg(0X54,reg4f5054val);	//色彩矩阵系数6  
	SCCB_WR_Reg(0X58,0X9E);			//MTXS 
}
//亮度设置
//0:-2
//1:-1
//2,0
//3,1
//4,2
void OV7670_Brightness(u8 bright)
{
	u8 reg55val=0X00;//默认就是bright=2
  	switch(bright)
	{
		case 0://-2
			reg55val=0XB0;	 	 
			break;	
		case 1://-1
			reg55val=0X98;	 	 
			break;	
		case 3://1
			reg55val=0X18;	 	 
			break;	
		case 4://2
			reg55val=0X30;	 	 
			break;	
	}
	SCCB_WR_Reg(0X55,reg55val);	//亮度调节 
}
//对比度设置
//0:-2
//1:-1
//2,0
//3,1
//4,2
void OV7670_Contrast(u8 contrast)
{
	u8 reg56val=0X40;//默认就是contrast=2
  	switch(contrast)
	{
		case 0://-2
			reg56val=0X30;	 	 
			break;	
		case 1://-1
			reg56val=0X38;	 	 
			break;	
		case 3://1
			reg56val=0X50;	 	 
			break;	
		case 4://2
			reg56val=0X60;	 	 
			break;	
	}
	SCCB_WR_Reg(0X56,reg56val);	//对比度调节 
}
//特效设置
//0:普通模式    
//1,负片
//2,黑白   
//3,偏红色
//4,偏绿色
//5,偏蓝色
//6,复古	    
void OV7670_Special_Effects(u8 eft)
{
	u8 reg3aval=0X04;//默认为普通模式
	u8 reg67val=0XC0;
	u8 reg68val=0X80;
	switch(eft)
	{
		case 1://负片
			reg3aval=0X24;
			reg67val=0X80;
			reg68val=0X80;
			break;	
		case 2://黑白
			reg3aval=0X14;
			reg67val=0X80;
			reg68val=0X80;
			break;	
		case 3://偏红色
			reg3aval=0X14;
			reg67val=0Xc0;
			reg68val=0X80;
			break;	
		case 4://偏绿色
			reg3aval=0X14;
			reg67val=0X40;
			reg68val=0X40;
			break;	
		case 5://偏蓝色
			reg3aval=0X14;
			reg67val=0X80;
			reg68val=0XC0;
			break;	
		case 6://复古
			reg3aval=0X14;
			reg67val=0XA0;
			reg68val=0X40;
			break;	 
	}
	SCCB_WR_Reg(0X3A,reg3aval);//TSLB设置 
	SCCB_WR_Reg(0X68,reg67val);//MANU,手动U值 
	SCCB_WR_Reg(0X67,reg68val);//MANV,手动V值 
}	


void OV7670_config_window(u16 startx,u16 starty,u16 width,u16 height)
{
	u16 endx =(startx+width*2);
	u16 endy =(starty+height*2);
	u8 x_reg,y_reg;
	u8 temp;
	if(endx>784)endx = endx % 784;
	if(endy>784)endy = endy % 784;
	x_reg=SCCB_RD_Reg(0x32);
	x_reg&=0xC0;
	y_reg=SCCB_RD_Reg(0x03);
	y_reg&=0xF0;
	//设置HREF
	temp=x_reg|((endx&0x7)<<3)|(startx&0x7);
	SCCB_WR_Reg(0x32,temp);
	temp=(startx&0x7F8)>>3;
	SCCB_WR_Reg(0x17,temp);
	temp=(endx&0x7F8)>>3;
	SCCB_WR_Reg(0x18,temp);
	//设置VREF
	temp=y_reg|((endy&0x3)<<2)|(starty&0x3);
	SCCB_WR_Reg(0x03,temp);
	temp=(starty&0x3FC)>>2;
	SCCB_WR_Reg(0x19,temp);
	temp=(endy&0x3FC)>>2;
	SCCB_WR_Reg(0x1A,temp);
}
//320*240尺寸的摄像头串口输出
void OV7670_Self_Window_Set(void)
{
	SCCB_WR_Reg(0x32,0x80);
	SCCB_WR_Reg(0x17,0x17);
	SCCB_WR_Reg(0x18,0x05);
	SCCB_WR_Reg(0x03,0x0A);
	SCCB_WR_Reg(0x19,0x02);
	SCCB_WR_Reg(0x1A,0x7B);
}

void OV7670_STM32GetPic(uint32_t Width,uint32_t height)
{

	uint32_t i,j;
	//发送图像数据包
	//包头
	Serial_SendByte(0x53);
	Serial_SendByte(0x5A);
	Serial_SendByte(0x48);
	Serial_SendByte(0x59);
	//设备地址
	Serial_SendByte(0x00);
	//包长度
	Serial_SendByte(0x0C);
	Serial_SendByte(0x58);
	Serial_SendByte(0x02);
	Serial_SendByte(0x00);
	//指令
	Serial_SendByte(0x02);
	
	while(OV7670_VS()==0)
	{
		i++;
	}

	
  while(OV7670_VS()==1);
	//后列
	for(i = 0; i < height; i++) 
	{
		while(OV7670_HREF()==0);
		//先行
		for(j = 0; j < Width; j++) 
		{     
			while(OV7670_PCLK()==0);
			Serial_SendByte(GPIOA->IDR&0xFF);//只有使用Serial_SendByte才可以被串口客户端识别
			while(OV7670_PCLK()==1);
						
			while(OV7670_PCLK()==0);
			Serial_SendByte(GPIOA->IDR&0xFF);
			while(OV7670_PCLK()==1);
		}
		while(OV7670_HREF()==1);
	}
	while(OV7670_VS()==0);
	//校验 CRC-16不校验 可以任意值
	Serial_SendByte(0xFF);
	Serial_SendByte(0xFF);
		
	Delay_ms(500);
}
//初始化串口发出的数据
void Send_Fired_Uart_init(void)
{
		//发送RGR656的信息
	//包头
	Serial_SendByte(0x53);
	Serial_SendByte(0x5A);
	Serial_SendByte(0x48);
	Serial_SendByte(0x59);
	//设备地址
	Serial_SendByte(0x00);
	//包长度
	Serial_SendByte(0x17);
	Serial_SendByte(0x00);
	Serial_SendByte(0x00);
	Serial_SendByte(0x00);
	//指令
	Serial_SendByte(0x01);
	//图像格式
	Serial_SendByte(0x04);
	//宽
	Serial_SendByte(0x40);
	Serial_SendByte(0x01);
	//高
	Serial_SendByte(0xF0);
	Serial_SendByte(0x00);
	//校验 CRC-16不校验 可以任意值
	Serial_SendByte(0xFF);
	Serial_SendByte(0xFF);
	
}

//void EXTI15_10_IRQHandler(void)
//{
//	if(OV7670_State<2)
//	{
//		if(OV7670_State==0)
//		{
//			OV7670_W_WRST(0);//写指针复位
//			OV7670_W_WRST(1);
//			OV7670_W_WEN(1);//写使能
//		}
//		else
//		{
//			OV7670_W_WEN(0);//禁止写入FIFO
//			OV7670_W_WRST(0);//复位写指针
//			OV7670_W_WRST(1);
//		}
//		OV7670_State++;
//	}
//	EXTI_ClearITPendingBit(EXTI_Line15);
//}	
