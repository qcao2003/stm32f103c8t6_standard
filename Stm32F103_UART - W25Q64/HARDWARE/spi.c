#include "stm32f10x.h"                  // Device header
#include "spi.h"

/**
  * @brief  �ı�SS��ƽ
  * @param  BitValue	�ı��Ŀ��ֵ��0Ϊ�͵�ƽ��1Ϊ�ߵ�ƽ
  * @retval ��
  */
void MySPI_W_SS(uint8_t BitValue)
{
	GPIO_WriteBit(MySPI_SS_GPIO, MySPI_SS_GPIO_Pin, (BitAction)BitValue);
}

/**
  * @brief  �ı�SCLK��ƽ
  * @param  BitValue	�ı��Ŀ��ֵ��0Ϊ�͵�ƽ��1Ϊ�ߵ�ƽ
  * @retval ��
  */
void MySPI_W_SCLK(uint8_t BitValue)
{
	GPIO_WriteBit(MySPI_SS_GPIO, MySPI_SCLK_GPIO_Pin, (BitAction)BitValue);
}

/**
  * @brief  �ı�MOSI��ƽ
  * @param  BitValue	�ı��Ŀ��ֵ��0Ϊ�͵�ƽ��1Ϊ�ߵ�ƽ
  * @retval ��
  */
void MySPI_W_MOSI(uint8_t BitValue)
{
	GPIO_WriteBit(MySPI_SS_GPIO, MySPI_MOSI_GPIO_Pin, (BitAction)BitValue);
}

/**
  * @brief  ��ȡMISO��ƽ
  * @param  ��
  * @retval ��ȡ�����߼���ƽֵ
  */
uint8_t MySPI_R_MISO(void)
{
	return GPIO_ReadInputDataBit(MySPI_MISO_GPIO, MySPI_MISO_GPIO_Pin);
}

/**
  * @brief  ���SPI��GPIO��ʼ������������GPIOʱ����Ҫ�����ļ���ʼ�ĺ궨�弴��
  * @param  ��
  * @retval ��
  */
void MySPI_Init(void)
{
	RCC_APB2PeriphClockCmd(MySPI_MOSI_GPIO_CLK, ENABLE);
	RCC_APB2PeriphClockCmd(MySPI_MISO_GPIO_CLK, ENABLE);
	RCC_APB2PeriphClockCmd(MySPI_SCLK_GPIO_CLK, ENABLE);
	RCC_APB2PeriphClockCmd(MySPI_SS_GPIO_CLK, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		// �������룬��������
	GPIO_InitStructure.GPIO_Pin = MySPI_MISO_GPIO_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(MySPI_MISO_GPIO, &GPIO_InitStructure);
	
	// �����������ž�Ϊ�������
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
  * @brief  ����SPI����ʼ�ź�
  * @param  ��
  * @retval ��
  */
void MySPI_Start(void)
{
	MySPI_W_SS(0);
}

/**
  * @brief  ����SPI�Ľ����ź�
  * @param  ��
  * @retval ��
  */
void MySPI_Stop(void)
{
	MySPI_W_SS(1);
}

/**
  * @brief  �������ݺ���
  * @param  ByteSend	���͵��ӻ������ݣ�����Ϊһ���ֽ�
  * @retval ���յ������ݣ�����Ϊһ���ֽ�
  */
uint8_t MySPI_SwapByte(uint8_t ByteSend)
{
	uint8_t i, ByteReceive = 0x00;
	
	for (i = 0; i < 8; i ++)
	{
		MySPI_W_MOSI(ByteSend & (0x80 >> i));		// ���½��أ��������Ƶ�MOSI������
		
		MySPI_W_SCLK(1);					// �����ض�ȡ����
		if (MySPI_R_MISO() == 1)
		{
			ByteReceive |= (0x80 >> i);		// ������ȡ����
		}
		MySPI_W_SCLK(0);					// �½���
	}
	
	return ByteReceive;
}


