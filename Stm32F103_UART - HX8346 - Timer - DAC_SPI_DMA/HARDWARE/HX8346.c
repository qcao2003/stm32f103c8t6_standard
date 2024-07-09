#include "HX8346.h"



void SPI_init()
{
	SPI_InitTypeDef  SPI_InitStructure;
	//SPI1�ڳ�ʼ��
	/* Configure SPI1 pins: SCK, MISO and MOSI */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //SPI1����Ϊ����ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		                //����SPI1Ϊ��ģʽ
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		            //SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		                    //����ʱ���ڲ�����ʱ��ʱ��Ϊ�͵�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	                    //��һ��ʱ���ؿ�ʼ��������
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		                    //NSS�ź��������ʹ��SSIλ������
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;  //SPI������Ԥ��ƵֵΪ8
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	                //���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	                        //CRCֵ����Ķ���ʽ
	SPI_Init(SPI1, &SPI_InitStructure);                                 //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPI1�Ĵ���
	
	/* Enable SPI1  */
	SPI_Cmd(SPI1, ENABLE);                                              //ʹ��SPI1����
}
void SPI_DAM_init()
{
	DMA_InitTypeDef DMA_InitStructure;
	DMA_DeInit(DMA1_Channel3);  
	//DMA_InitStructure.DMA_PeripheralBaseAddr = 0x4001300C;                          	//����  ��������(0x4001300C) ��ַ(Դ��ַ)
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&SPI1->DR;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)g_cSbuff;                    	//���� SRAM �洢��ַ(Դ��ַ)
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;                                //���䷽�� �ڴ�-����
	DMA_InitStructure.DMA_BufferSize = DMA_Buff;                           						//���� SPI1 ���ճ���
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;                  //�����ַ����(����)
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                           //�ڴ��ַ����(�仯)
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;           //���贫����(�ֽ�)
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;                   //�ڴ洫����(�ֽ�)
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                                     //���䷽ʽ,һ�δ�����ֹͣ,�����¼���
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;                           //�жϷ�ʽ-��(����)
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                                      //�ڴ浽�ڴ淽ʽ��ֹ
	DMA_Init(DMA1_Channel3, &DMA_InitStructure);
 
	DMA_ITConfig(DMA1_Channel3, DMA_IT_TC, DISABLE);                                   //���� DMA1_Channel3 ��������ж�
	DMA_ITConfig(DMA1_Channel3, DMA_IT_TE, DISABLE);                                   //���� DMA1_Channel3 ��������ж�
	/* Enable SPI1 DMA TX request */
	SPI1->CR2 |= 1<<1;                                                                //���ͻ�����DMAʹ��
	DMA_Cmd(DMA1_Channel3, DISABLE);                                                  //���� DMA ͨ�� DMA1_Channel3
}

/*******************************************************************************
* Function Name  : SPI1_Send
* Description    : SPI1��DMA��ʽ����
* Input          : SPI1_TX_Buff[SPI1_SendBufferSize]
* Output         : None
* Return         : None
* Attention             : �ر�DMAͨ��3֮ǰ����ȴ�TXEΪ1���ȴ�æ��־Ϊ0
*******************************************************************************/
void SPI_DMA_TXD(u32 Num)
{
 
    //DMA1_Channel3->CPAR = 0x4001300C; //�����ַ
		DMA1_Channel3->CPAR = (uint32_t)&SPI1->DR; //�����ַ
    DMA1_Channel3->CMAR = (u32) g_cSbuff; //mem��ַ
    DMA1_Channel3->CNDTR = Num ; //���䳤��
    DMA1_Channel3->CCR = (0 << 14) | // �Ǵ洢�����洢��ģʽ
            (2 << 12) | // ͨ�����ȼ���
            (0 << 11) | // �洢�����ݿ��8bit
            (0 << 10) | // �洢�����ݿ��8bit
            (0 <<  9) | // �������ݿ��8bit
            (0 <<  8) | // �������ݿ��8bit
            (1 <<  7) | // �洢����ַ����ģʽ
            (0 <<  6) | // �����ַ����ģʽ(����)
            (0 <<  5) | // ��ѭ��ģʽ
            (1 <<  4) | // �Ӵ洢����
 //           (1 <<  3) | // ����������ж�
//            (0 <<  2) | // ����봫���ж�
//            (1 <<  1) | // ����������ж�
            (1);        // ͨ������
	while(DMA_GetFlagStatus(DMA1_FLAG_TC3 )==RESET);//�ȴ��������
	DMA_ClearFlag(DMA1_FLAG_TC3);//���������ɱ�־λ
	DMA_Cmd(DMA1_Channel3, DISABLE);//�ر�ͨ��
	myfree(0,g_cSbuff);//�ͷ��ڴ�
}
 
/**********************************************
��������Lcd��亯��
���ܣ����Lcd��ָ���ľ�������
 
ע�⣺xStart�� yStart������Ļ����ת���ı䣬λ���Ǿ��ο���ĸ���
 
��ڲ�����xStart x�������ʼ��
          ySrart y�������ֹ��
          xLong  Ҫѡ�����ε�x���򳤶�
          yLong  Ҫѡ�����ε�y���򳤶�
					Color  ��ɫ
����ֵ����
***********************************************/
void Lcd_ColorBox(u16 xStart,u16 yStart,u16 xLong,u16 yLong,u16 Color)
{
	u32 i,j;
	i = xLong*yLong;//����ĵ���
	j = DMA_Buff/2;//������
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
 
void SPI_WriteNumber(u16 Color,u32 Num)//д��num����
{
	unsigned int i;
	if(Num <= DMA_Buff)
	{
		Num<<=1;//Num*2
		g_cSbuff = mymalloc(0,Num);//�����ڴ�
		for(i=0;i<Num;i++,i++)
		{
			*(g_cSbuff+i) = Color>>8;
			*(g_cSbuff+i+1) = Color;//д����
		}
		SPI_CS(0);	
		LCD_RS(1);
		SPI_DMA_TXD(Num);//Num*2
		SPI_CS(1);
	}
}
 