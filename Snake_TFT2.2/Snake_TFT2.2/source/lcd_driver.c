#include "main.h"


/****************************************************************************
*��������TFT_WriteCmd
*��  �룺cmd
*��  ����
*��  �ܣ�д���
****************************************************************************/	  

void TFT_WriteCmd(uint cmd)
{
	uchar cmdH, cmdL;
	cmdH = cmd >> 8;
	cmdL = cmd & 0x00FF;

	TFT_WR = 1;	 		  //��ʼ��WR
	TFT_CS = 0;			  //��Ƭѡ
	
	TFT_RD = 1;           //ѡ��д
	TFT_RS = 0;			  //ѡ������
		
	TFT_DATAPORTH = cmdH; //������������
	TFT_DATAPORTL = cmdL; //������������

	TFT_WR = 0;			  //д��ʱ��
	TFT_WR = 1;
	
	TFT_CS = 1;			  //�ر�Ƭѡ
}

/****************************************************************************
*��������TFT_WriteCmdData
*��  �룺cmd, dat
*��  ����
*��  �ܣ�д��� д���ݡ�
****************************************************************************/	  

void TFT_WriteData(uint dat)
{
    uchar datH, datL;
	datH = dat >> 8;
	datL = dat & 0x00FF;

	TFT_WR = 1;	 		  //��ʼ��WR
	TFT_CS = 0;			  //��Ƭѡ
	
	TFT_RD = 1;           //ѡ��д
	TFT_RS = 1;			  //ѡ��������
		
	TFT_DATAPORTH = datH;  //������������
	TFT_DATAPORTL = datL;  //������������

	TFT_WR = 0;			  //д��ʱ��
	TFT_WR = 1;
	
	TFT_CS = 1;			  //�ر�Ƭѡ
}

/****************************************************************************
*��������TFT_Init
*��  �룺
*��  ����
*��  �ܣ���ʼ��TFT��
****************************************************************************/	  

void TFT_Init(void)
{
	uint i;

	TFT_RST = 1;
	for(i=500; i>0; i--);
	TFT_RST = 0;
	for(i=500; i>0; i--);
	TFT_RST = 1;
	for(i=5000; i>0; i--);
	
	TFT_CS = 0;
	TFT_WriteCmd(0x11);

	TFT_WriteCmd(0xc5);
	TFT_WriteData(0xc5);

	TFT_WriteCmd(0xf0);
	TFT_WriteData(0x5a);

	TFT_WriteCmd(0xf2);
	TFT_WriteData(0x01);

	TFT_WriteCmd(0xe0);
	TFT_WriteData(0x00);
	TFT_WriteData(0x01);
	TFT_WriteData(0x06);
	TFT_WriteData(0x2e);
	TFT_WriteData(0x2b);
	TFT_WriteData(0x0b);
	TFT_WriteData(0x1a);
	TFT_WriteData(0x02);
	TFT_WriteData(0x06);
	TFT_WriteData(0x05);
	TFT_WriteData(0x0c);
	TFT_WriteData(0x0d);
	TFT_WriteData(0x00);
	TFT_WriteData(0x05);
	TFT_WriteData(0x02);
	TFT_WriteData(0x05);

	TFT_WriteCmd(0xe1);
	TFT_WriteData(0x06);
	TFT_WriteData(0x23);
	TFT_WriteData(0x25);
	TFT_WriteData(0x0f);
	TFT_WriteData(0x0a);
	TFT_WriteData(0x04);
	TFT_WriteData(0x02);
	TFT_WriteData(0x1a);
	TFT_WriteData(0x05);
	TFT_WriteData(0x03);
	TFT_WriteData(0x06);
	TFT_WriteData(0x01);
	TFT_WriteData(0x0c);
	TFT_WriteData(0x0b);
	TFT_WriteData(0x05);
	TFT_WriteData(0x05);

	TFT_WriteCmd(0x3a);
	TFT_WriteData(0x05);

    TFT_WriteCmd(0x36);
    TFT_WriteData(0xC8);

	TFT_WriteCmd(0x29);
}

/****************************************************************************
*��������TFT_SetWindow
*��  �룺xStart, yStart, xEnd, yEnd
*��  ����
*��  �ܣ�����Ҫ�����Ĵ��ڡ�
****************************************************************************/	  

void TFT_SetWindow(uchar xStart, uchar yStart, uchar xEnd, uchar yEnd)
{
    TFT_WriteCmd(0x2A);
    TFT_WriteData(0x00);
    TFT_WriteData(xStart);
    TFT_WriteData(0x00);
    TFT_WriteData(xEnd);

    TFT_WriteCmd(0x2b);
    TFT_WriteData(0x00);
    TFT_WriteData(yStart);
    TFT_WriteData(0x00);
    TFT_WriteData(yEnd);
    
    TFT_WriteCmd(0x2c); 
}


/****************************************************************************
*��������GUI_Clearcreen
*��  �룺backColor
*��  ����
*��  �ܣ�������Ϳ����ɫ��
****************************************************************************/
	  
void TFT_ClearScreen(uint color)
{
  	uint i, j;

	TFT_SetWindow(0, 0, TFT_XMAX, TFT_YMAX);	 //��������
  	for(i=0; i<=TFT_XMAX; i++)
	{
		for (j=0; j<=TFT_YMAX; j++)
		{
			TFT_WriteData(color);
		}
	}
}


