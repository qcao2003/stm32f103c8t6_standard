#include "stm32f10x.h"                  // Device header
#include "spi.h"

/**
  * @brief  改变SS电平
  * @param  BitValue	改变的目标值，0为低电平，1为高电平
  * @retval 无
  */
void MySPI_W_SS(uint8_t BitValue)
{
	GPIO_WriteBit(MySPI_SS_GPIO, MySPI_SS_GPIO_Pin, (BitAction)BitValue);
}

/**
  * @brief  改变SCLK电平
  * @param  BitValue	改变的目标值，0为低电平，1为高电平
  * @retval 无
  */
void MySPI_W_SCLK(uint8_t BitValue)
{
	GPIO_WriteBit(MySPI_SS_GPIO, MySPI_SCLK_GPIO_Pin, (BitAction)BitValue);
}

/**
  * @brief  改变MOSI电平
  * @param  BitValue	改变的目标值，0为低电平，1为高电平
  * @retval 无
  */
void MySPI_W_MOSI(uint8_t BitValue)
{
	GPIO_WriteBit(MySPI_SS_GPIO, MySPI_MOSI_GPIO_Pin, (BitAction)BitValue);
}

/**
  * @brief  读取MISO电平
  * @param  无
  * @retval 读取到的逻辑电平值
  */
uint8_t MySPI_R_MISO(void)
{
	return GPIO_ReadInputDataBit(MySPI_MISO_GPIO, MySPI_MISO_GPIO_Pin);
}

/**
  * @brief  软件SPI的GPIO初始化函数，更换GPIO时仅需要更改文件开始的宏定义即可
  * @param  无
  * @retval 无
  */
void MySPI_Init(void)
{
	RCC_APB2PeriphClockCmd(MySPI_MOSI_GPIO_CLK, ENABLE);
	RCC_APB2PeriphClockCmd(MySPI_MISO_GPIO_CLK, ENABLE);
	RCC_APB2PeriphClockCmd(MySPI_SCLK_GPIO_CLK, ENABLE);
	RCC_APB2PeriphClockCmd(MySPI_SS_GPIO_CLK, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		// 主机输入，上拉输入
	GPIO_InitStructure.GPIO_Pin = MySPI_MISO_GPIO_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(MySPI_MISO_GPIO, &GPIO_InitStructure);
	
	// 其余三个引脚均为推挽输出
	// MOSI
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = MySPI_MOSI_GPIO_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(MySPI_MOSI_GPIO, &GPIO_InitStructure);
	
	// SCLK
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = MySPI_SCLK_GPIO_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(MySPI_SCLK_GPIO, &GPIO_InitStructure);
	
	// SS
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = MySPI_SS_GPIO_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(MySPI_SS_GPIO, &GPIO_InitStructure);
	
	MySPI_W_SS(1);
	MySPI_W_SCLK(0);
}

/**
  * @brief  生成SPI的起始信号
  * @param  无
  * @retval 无
  */
void MySPI_Start(void)
{
	MySPI_W_SS(0);
}

/**
  * @brief  生成SPI的结束信号
  * @param  无
  * @retval 无
  */
void MySPI_Stop(void)
{
	MySPI_W_SS(1);
}

/**
  * @brief  交换数据函数
  * @param  ByteSend	发送到从机的数据，长度为一个字节
  * @retval 接收到的数据，长度为一个字节
  */
uint8_t MySPI_SwapByte(uint8_t ByteSend)
{
	uint8_t i, ByteReceive = 0x00;
	
	for (i = 0; i < 8; i ++)
	{
		MySPI_W_MOSI(ByteSend & (0x80 >> i));		// 在下降沿，把数据移到MOSI总线上
		
		MySPI_W_SCLK(1);					// 上升沿读取数据
		if (MySPI_R_MISO() == 1)
		{
			ByteReceive |= (0x80 >> i);		// 掩码提取数据
		}
		MySPI_W_SCLK(0);					// 下降沿
	}
	
	return ByteReceive;
}


