#ifndef __LCD_DRIVER_H
#define __LCD_DRIVER_H


//---����ʹ�õ�IO��---//
sbit	    TFT_RST = P3^3;   //��λ
sbit	    TFT_CS  = P3^2;	  //Ƭѡ
sbit	    TFT_RS  = P2^7;	  //��������ѡ���
sbit	    TFT_WR  = P2^6;	  //��д����
sbit      TFT_RD  = P2^5;   //��д����

#define   TFT_DATAPORTH  P1  //���ø�8λ����
#define   TFT_DATAPORTL  P0
//---�������Ĵ�С---//
#define TFT_XMAX ((uchar)175)//		//����TFT���Ĵ�С
#define TFT_YMAX ((uchar)219)//

//---������ɫ�ĺ�---//
#define WHITE          0xFFFF
#define BLACK          0x0000
#define BLUE           0x001F
#define RED            0xF800
#define MAGENTA        0xF81F
#define GREEN          0x07E0
#define CYAN           0x7FFF
#define YELLOW         0xFFE0		 //������ɫ�ĺ�

//-- ����ȫ�ֺ��� --//
void TFT_WriteCmd(uint cmd);
void TFT_WriteData(uint dat);
void TFT_Init(void);
void TFT_SetWindow(uchar xStart, uchar yStart, uchar xEnd, uchar yEnd);
void TFT_ClearScreen(uint color);

#endif
