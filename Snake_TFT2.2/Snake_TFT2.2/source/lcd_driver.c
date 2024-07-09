#include "main.h"


/****************************************************************************
*函数名：TFT_WriteCmd
*输  入：cmd
*输  出：
*功  能：写命令。
****************************************************************************/	  

void TFT_WriteCmd(uint cmd)
{
	uchar cmdH, cmdL;
	cmdH = cmd >> 8;
	cmdL = cmd & 0x00FF;

	TFT_WR = 1;	 		  //初始化WR
	TFT_CS = 0;			  //打开片选
	
	TFT_RD = 1;           //选择写
	TFT_RS = 0;			  //选择命令
		
	TFT_DATAPORTH = cmdH; //放置命令数据
	TFT_DATAPORTL = cmdL; //放置命令数据

	TFT_WR = 0;			  //写入时序
	TFT_WR = 1;
	
	TFT_CS = 1;			  //关闭片选
}

/****************************************************************************
*函数名：TFT_WriteCmdData
*输  入：cmd, dat
*输  出：
*功  能：写命令， 写数据。
****************************************************************************/	  

void TFT_WriteData(uint dat)
{
    uchar datH, datL;
	datH = dat >> 8;
	datL = dat & 0x00FF;

	TFT_WR = 1;	 		  //初始化WR
	TFT_CS = 0;			  //打开片选
	
	TFT_RD = 1;           //选择写
	TFT_RS = 1;			  //选择发送数据
		
	TFT_DATAPORTH = datH;  //放置命令数据
	TFT_DATAPORTL = datL;  //放置命令数据

	TFT_WR = 0;			  //写入时序
	TFT_WR = 1;
	
	TFT_CS = 1;			  //关闭片选
}

/****************************************************************************
*函数名：TFT_Init
*输  入：
*输  出：
*功  能：初始化TFT。
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
*函数名：TFT_SetWindow
*输  入：xStart, yStart, xEnd, yEnd
*输  出：
*功  能：设置要操作的窗口。
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
*函数名：GUI_Clearcreen
*输  入：backColor
*输  出：
*功  能：清屏并涂上颜色。
****************************************************************************/
	  
void TFT_ClearScreen(uint color)
{
  	uint i, j;

	TFT_SetWindow(0, 0, TFT_XMAX, TFT_YMAX);	 //作用区域
  	for(i=0; i<=TFT_XMAX; i++)
	{
		for (j=0; j<=TFT_YMAX; j++)
		{
			TFT_WriteData(color);
		}
	}
}


