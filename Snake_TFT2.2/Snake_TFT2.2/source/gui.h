#ifndef _GUI_H_
#define _GUI_H_


//---显示图片，定义这个---//
#define PICTURE_SHOW

//---显示32*29的汉字---//
//#define CHAR32_SHOW

//---显示19x24的汉字---//
//#define CHAR14_SHOW

//---显示ASCII16x24码对应的字符---//
#define USE_ASCII16x24


//---定义全局变量---//
void GUI_Dot(uchar x, uint y, uint color);  //画点
void GUI_Box(uchar sx, uint sy, uchar ex, uint ey, uint color);   //画方框
void GUI_Line(uchar xStart, uint yStart, uchar xEnd, uint yEnd, uint color);//画线条
void GUI_Write32CnChar(uint x, uint y, uchar *cn, uint wordColor, uint backColor);	 
void GUI_Write14CnChar(uint x,uint y,uchar *cn,uint wordColor,uint backColor);	 
void GUI_ShowPicture(uchar x, uint y, uchar wide, uint high);
void GUI_WriteASCII16x24(uchar x, uchar y, uchar *p, uint wordColor, uint backColor);
 
#endif
