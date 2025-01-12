#ifndef __LCD_DRIVER_H
#define __LCD_DRIVER_H


//---定义使用的IO口---//
sbit	    TFT_RST = P3^3;   //复位
sbit	    TFT_CS  = P3^2;	  //片选
sbit	    TFT_RS  = P2^7;	  //数据命令选择端
sbit	    TFT_WR  = P2^6;	  //读写控制
sbit      TFT_RD  = P2^5;   //读写控制

#define   TFT_DATAPORTH  P1  //放置高8位数据
#define   TFT_DATAPORTL  P0
//---定义屏的大小---//
#define TFT_XMAX ((uchar)175)//		//设置TFT屏的大小
#define TFT_YMAX ((uchar)219)//

//---定义颜色的宏---//
#define WHITE          0xFFFF
#define BLACK          0x0000
#define BLUE           0x001F
#define RED            0xF800
#define MAGENTA        0xF81F
#define GREEN          0x07E0
#define CYAN           0x7FFF
#define YELLOW         0xFFE0		 //定义颜色的宏

//-- 声明全局函数 --//
void TFT_WriteCmd(uint cmd);
void TFT_WriteData(uint dat);
void TFT_Init(void);
void TFT_SetWindow(uchar xStart, uchar yStart, uchar xEnd, uchar yEnd);
void TFT_ClearScreen(uint color);

#endif

