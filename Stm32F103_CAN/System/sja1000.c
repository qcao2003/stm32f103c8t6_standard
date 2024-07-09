#include "sja1000.h"

void adio_out_change(void)//输入设置
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = 0xFF; //从GPIOA0到GPIOA7
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void adio_in_change(void)//输入设置
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = 0xFF;	//从GPIOA0到GPIOA7
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void sja1000_IO_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_15; //PA12 SJA1000_CS PA15 SAJ1000_ALE
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA12
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5; //PB3 SJA1000_WR PB4 SAJ1000_RD PB5 SJA1000_RESET
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
}

void MCU_Init(void)
{
	//CPU初始化
	CAN_RESET_L;  //SJA1000复位有效
	Delay_ms(10);
	CAN_RESET_H;  //CAN总线复位管脚,复位无效
  CAN_CS_L;  //CAN总线片选有效
	EXTIX_Init(); //打开中断
	
}

void Peli_Init(void)
{//SJA1000 的初始化
    uint8 bdata Status;
    do
    {//.0=1---reset MODRe,进入复位模式，以便设置相应的寄存器
     //防止未进入复位模式，重复写入
      SJA_MOD = RM_BIT |AFM_BIT;
			Status = SJA_MOD ;
    }
    while(!(Status & RM_BIT));

    SJA_CDR  = CANMode_BIT|CLKOff_BIT;// CDR.3=1--时钟关闭, .7=0---basic CAN, .7=1---Peli CAN
    SJA_BTR0 = 0x03;
    SJA_BTR1 = 0x1c;//16M晶振，波特率125Kbps
    SJA_IER  = RIE_BIT;// .0=1--接收中断使能；  .1=0--关闭发送中断使能
    SJA_OCR  = NormalMode|Tx0PullDn|OCPOL1_BIT|Tx1PullUp;// 配置输出控制寄存器
    SJA_CMR  = RRB_BIT;//释放接收缓冲器

    SJA_ACR0  = 0x11;
    SJA_ACR1  = 0x22;
    SJA_ACR2  = 0x33;
    SJA_ACR3  = 0x44;//初始化标示码

    SJA_AMR0  = 0xff;
    SJA_AMR1  = 0xff;
    SJA_AMR2  = 0xff;
    SJA_AMR3  = 0xff;//初始化掩码

    do
    {//确保进入自接收模式
			SJA_MOD   = STM_BIT;
			Status  = SJA_MOD;
     }
    while( !(Status & STM_BIT) );

}//SJA1000 的初始化


