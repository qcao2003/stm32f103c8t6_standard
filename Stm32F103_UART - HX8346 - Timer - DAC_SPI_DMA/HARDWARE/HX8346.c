#include "HX8346.h"



void SPI_init()
{
	SPI_InitTypeDef  SPI_InitStructure;
	//SPI1口初始化
	/* Configure SPI1 pins: SCK, MISO and MOSI */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //SPI1设置为两线全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		                //设置SPI1为主模式
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		            //SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		                    //串行时钟在不操作时，时钟为低电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	                    //第一个时钟沿开始采样数据
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		                    //NSS信号由软件（使用SSI位）管理
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;  //SPI波特率预分频值为8
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	                //数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	                        //CRC值计算的多项式
	SPI_Init(SPI1, &SPI_InitStructure);                                 //根据SPI_InitStruct中指定的参数初始化外设SPI1寄存器
	
	/* Enable SPI1  */
	SPI_Cmd(SPI1, ENABLE);                                              //使能SPI1外设
}
void SPI_DAM_init()
{
	DMA_InitTypeDef DMA_InitStructure;
	DMA_DeInit(DMA1_Channel3);  
	//DMA_InitStructure.DMA_PeripheralBaseAddr = 0x4001300C;                          	//设置  接收外设(0x4001300C) 地址(源地址)
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&SPI1->DR;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)g_cSbuff;                    	//设置 SRAM 存储地址(源地址)
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;                                //传输方向 内存-外设
	DMA_InitStructure.DMA_BufferSize = DMA_Buff;                           						//设置 SPI1 接收长度
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;                  //外设地址增量(不变)
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                           //内存地址增量(变化)
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;           //外设传输宽度(字节)
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;                   //内存传输宽度(字节)
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                                     //传输方式,一次传输完停止,不重新加载
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;                           //中断方式-高(三级)
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                                      //内存到内存方式禁止
	DMA_Init(DMA1_Channel3, &DMA_InitStructure);
 
	DMA_ITConfig(DMA1_Channel3, DMA_IT_TC, DISABLE);                                   //开启 DMA1_Channel3 传输完成中断
	DMA_ITConfig(DMA1_Channel3, DMA_IT_TE, DISABLE);                                   //开启 DMA1_Channel3 传输错误中断
	/* Enable SPI1 DMA TX request */
	SPI1->CR2 |= 1<<1;                                                                //发送缓冲区DMA使能
	DMA_Cmd(DMA1_Channel3, DISABLE);                                                  //开启 DMA 通道 DMA1_Channel3
}

/*******************************************************************************
* Function Name  : SPI1_Send
* Description    : SPI1的DMA方式发送
* Input          : SPI1_TX_Buff[SPI1_SendBufferSize]
* Output         : None
* Return         : None
* Attention             : 关闭DMA通道3之前必须等待TXE为1，等待忙标志为0
*******************************************************************************/
void SPI_DMA_TXD(u32 Num)
{
 
    //DMA1_Channel3->CPAR = 0x4001300C; //外设地址
		DMA1_Channel3->CPAR = (uint32_t)&SPI1->DR; //外设地址
    DMA1_Channel3->CMAR = (u32) g_cSbuff; //mem地址
    DMA1_Channel3->CNDTR = Num ; //传输长度
    DMA1_Channel3->CCR = (0 << 14) | // 非存储器到存储器模式
            (2 << 12) | // 通道优先级高
            (0 << 11) | // 存储器数据宽度8bit
            (0 << 10) | // 存储器数据宽度8bit
            (0 <<  9) | // 外设数据宽度8bit
            (0 <<  8) | // 外设数据宽度8bit
            (1 <<  7) | // 存储器地址增量模式
            (0 <<  6) | // 外设地址增量模式(不增)
            (0 <<  5) | // 非循环模式
            (1 <<  4) | // 从存储器读
 //           (1 <<  3) | // 允许传输错误中断
//            (0 <<  2) | // 允许半传输中断
//            (1 <<  1) | // 允许传输完成中断
            (1);        // 通道开启
	while(DMA_GetFlagStatus(DMA1_FLAG_TC3 )==RESET);//等待传输完成
	DMA_ClearFlag(DMA1_FLAG_TC3);//清除传输完成标志位
	DMA_Cmd(DMA1_Channel3, DISABLE);//关闭通道
	myfree(0,g_cSbuff);//释放内存
}
 
/**********************************************
函数名：Lcd填充函数
功能：填充Lcd上指定的矩形区域
 
注意：xStart和 yStart随着屏幕的旋转而改变，位置是矩形框的四个角
 
入口参数：xStart x方向的起始点
          ySrart y方向的终止点
          xLong  要选定矩形的x方向长度
          yLong  要选定矩形的y方向长度
					Color  颜色
返回值：无
***********************************************/
void Lcd_ColorBox(u16 xStart,u16 yStart,u16 xLong,u16 yLong,u16 Color)
{
	u32 i,j;
	i = xLong*yLong;//输入的点数
	j = DMA_Buff/2;//最大点数
	BlockWrite(xStart,xStart+xLong-1,yStart,yStart+yLong-1);
	do
	{
		if(i>j)
		{
			SPI_WriteNumber(Color,j);
			i-=j;
		}
		else
		{
			SPI_WriteNumber(Color,i);
			break;
		}
	}
	while(1);
}
 
void SPI_WriteNumber(u16 Color,u32 Num)//写入num个点
{
	unsigned int i;
	if(Num <= DMA_Buff)
	{
		Num<<=1;//Num*2
		g_cSbuff = mymalloc(0,Num);//申请内存
		for(i=0;i<Num;i++,i++)
		{
			*(g_cSbuff+i) = Color>>8;
			*(g_cSbuff+i+1) = Color;//写数据
		}
		SPI_CS(0);	
		LCD_RS(1);
		SPI_DMA_TXD(Num);//Num*2
		SPI_CS(1);
	}
}
 