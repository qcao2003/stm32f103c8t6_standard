#ifndef _GUI_H_
#define _GUI_H_


//---��ʾͼƬ���������---//
#define PICTURE_SHOW

//---��ʾ32*29�ĺ���---//
//#define CHAR32_SHOW

//---��ʾ19x24�ĺ���---//
//#define CHAR14_SHOW

//---��ʾASCII16x24���Ӧ���ַ�---//
#define USE_ASCII16x24


//---����ȫ�ֱ���---//
void GUI_Dot(uchar x, uint y, uint color);  //����
void GUI_Box(uchar sx, uint sy, uchar ex, uint ey, uint color);   //������
void GUI_Line(uchar xStart, uint yStart, uchar xEnd, uint yEnd, uint color);//������
void GUI_Write32CnChar(uint x, uint y, uchar *cn, uint wordColor, uint backColor);	 
void GUI_Write14CnChar(uint x,uint y,uchar *cn,uint wordColor,uint backColor);	 
void GUI_ShowPicture(uchar x, uint y, uchar wide, uint high);
void GUI_WriteASCII16x24(uchar x, uchar y, uchar *p, uint wordColor, uint backColor);
 
#endif
