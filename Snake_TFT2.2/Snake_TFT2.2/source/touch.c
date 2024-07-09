#include"main.h"

PosTypeDef TouchAdj; //����һ��������������У������
XPT_XY xpt_xy;	     //����һ��ȫ�ֱ�������X��Y��ֵ
#define XY_READ_TIMS  10		   //��ȡ�Ĵ���

/****************************************************************************
*��������TOUCH_SPI_Start
*��  �룺��
*��  ������
*��  �ܣ���ʼ������SPI
****************************************************************************/

void TOUCH_SPI_Start(void)
{
	TOUCH_CLK = 0;
	TOUCH_CS  = 1;
	TOUCH_DIN = 1;
	TOUCH_CLK = 1;
	TOUCH_CS  = 0;		
}

/****************************************************************************
*��������TOUCH_SPI_Write
*��  �룺dat��д������
*��  ������
*��  �ܣ�ʹ��SPIд������
****************************************************************************/

void TOUCH_SPI_Write(uchar dat)
{
	uchar i;
	TOUCH_CLK = 0;
	for(i=0; i<8; i++)
	{
		TOUCH_DIN = dat >> 7;  	//�������λ
		dat <<= 1;
		TOUCH_CLK = 0;			//�����ط�������

		TOUCH_CLK = 1;

	}
}

/****************************************************************************
*��������TOUCH_SPI_Read
*��  �룺��
*��  ����dat����ȡ ��������
*��  �ܣ�ʹ��SPI��ȡ����
****************************************************************************/

uint TOUCH_SPI_Read(void)
{
	uint i, dat=0;
	TOUCH_CLK = 0;
	for(i=0; i<12; i++)		//����12λ����
	{
		dat <<= 1;

		TOUCH_CLK = 1;
		TOUCH_CLK = 0;

		dat |= TOUCH_DOUT;

	}
	return dat;	
}

/****************************************************************************
*��������TOUCH_XPT_ReadData
*��  �룺cmd����ȡ��X����Y
*��  ����endValue�������źŴ���󷵻ص�ֵ
*��  �ܣ���ȡ��������
****************************************************************************/

uint TOUCH_XPT_ReadData(uchar cmd)
{
	uchar i, j;
	uint readValue[XY_READ_TIMS];
	long endValue;

	TOUCH_CLK = 0;    //������ʱ��
	TOUCH_CS  = 0;    //ѡ��оƬ
	
	for(i=0; i<XY_READ_TIMS; i++)	 	//��ȡXY_READ_TIMS�ν��
	{	
		TOUCH_SPI_Write(cmd);  //����ת������
		//Delay_6us();
		for(j=6; j>0; j--); 	//��ʱ�ȴ�ת�����
		TOUCH_CLK = 1;	  //����һ��ʱ�����ڣ����BUSY
		_nop_();
		_nop_();
		TOUCH_CLK = 0;
		_nop_();
		_nop_();
	
		readValue[i] = TOUCH_SPI_Read();
	}
	TOUCH_CS = 1; //�ͷ�Ƭѡ

	//---����˲�---//
	//---�ȴ�С���򣬳�ȥ���ֵ����ȥ���ֵ������ƽ��ֵ---//
   	for(i=0; i<XY_READ_TIMS - 1; i++)	//�Ӵ�С����
	{
		for(j= i+1; j<XY_READ_TIMS; j++)
		{
			if(readValue[i] < readValue[j])
			{
				endValue = readValue[i];
				readValue[i] = readValue[j];
				readValue[j] = endValue;
			}  
		}
	}
//	if((readValue[2] - readValue[3]) > 5)
//	{
//		return 0;
//	}
	endValue = 0;
	for(i=2; i<XY_READ_TIMS-2; i++)		   
	{
		endValue += readValue[i]; 
	}
	endValue = endValue/ (XY_READ_TIMS - 4);//��ƽ��ֵ

	return endValue;
}

/****************************************************************************
*��������TOUCH_XPT_ReadXY
*��  �룺��
*��  ����1���ɹ� 0��ʧ��
*��  �ܣ���ȡXY��ADֵ
****************************************************************************/

uchar TOUCH_XPT_ReadXY(void)
{	
	uint x1, x2, x, y1, y2, y;

	TOUCH_SPI_Start();
	//---�ֱ������Xֵ��Yֵ, �����Ŷ��������һЩ��ȡ����---//
	x1 = TOUCH_XPT_ReadData(XPT_CMD_X);
	y1 = TOUCH_XPT_ReadData(XPT_CMD_Y);
	x2 = TOUCH_XPT_ReadData(XPT_CMD_X);
	y2 = TOUCH_XPT_ReadData(XPT_CMD_Y);

	//---��ȡXֵ�Ĳ�ֵ---//
	if (x1 > x2)
	{
		x = x1 - x2;
	}
	else
	{
		x = x2 - x1;
	}

	//---��ȡYֵ�Ĳ�ֵ---//
	if (y1 > y2)
	{
		y = y1 - y2;
	}
	else
	{
		y = y2 - y1;
	}

	//---�жϲ�ֵ�Ƿ����50�����ھͷ���0����ʾ��ȡʧ��---//
	if((x > 50) || (y > 50))  
	{
		return 0;
	}

	//---��ȡ���ζ�ȡֵ��ƽ������Ϊ��ȡ����XYֵ---//
	xpt_xy.x = (x1 + x2) / 2;
	xpt_xy.y = (y1 + y2) / 2;

	xpt_xy.x &= 0xFFF0;	  //ȥ������λ
	xpt_xy.y &= 0xFFF0;

	//---ȷ��XYֵ�ķ�Χ�����ڴ���������TFTʱ---//
	if((xpt_xy.x < 100) || (xpt_xy.y > 3600))
	{
		return 0;
	}

	return 1;  // ����1����ʾ��ȡ�ɹ�
}


/****************************************************************************
*��������TOUCH_ReadAdj
*��  �룺px����ȡ����X����ֵ
*      *           py����ȡ����Y����ֵ
*��  ������
*��  �ܣ�����ĻУ������ȡ�ĸ���
****************************************************************************/	  
#if LCD_TOUCH_TYPE == 0

void TOUCH_ReadAdj(uint *px, uint *py)
{
    ulong i = 0;
    XPT_XY pot[4];

    /* ��ȡ�ĸ���λ������� */
    TFT_ClearScreen(0x0000);
    GUI_DrowSign(LCD_ADJX_MIN, LCD_ADJY_MIN, 0x0FF0);
    while(1)
    {
        if(TOUCH_XPT_ReadXY())
        {
            i++;
            if(i > 10)
            {
                pot[0].x = xpt_xy.x;
                pot[0].y = xpt_xy.y;
                break;
            }
        }   
    }
    GUI_DrowSign(LCD_ADJX_MIN, LCD_ADJY_MIN, 0x0000);
    ADJ_CHECK_DELAY;
    GUI_DrowSign(LCD_ADJX_MIN, LCD_ADJY_MAX, 0x0FF0);
    i = 0;
    while(1)
    {
        if(TOUCH_XPT_ReadXY())
        {
            i++;
            if(i > 10)
            {
                pot[1].x = xpt_xy.x;
                pot[1].y = xpt_xy.y;
                break;
            }
        }   
    }
    GUI_DrowSign(LCD_ADJX_MIN, LCD_ADJY_MAX, 0x0000);
    ADJ_CHECK_DELAY;
    GUI_DrowSign(LCD_ADJX_MAX, LCD_ADJY_MIN, 0x0FF0);
    i = 0;
    while(1)
    {
        if(TOUCH_XPT_ReadXY())
        {
            i++;
            if(i > 10)
            {
                pot[2].x = xpt_xy.x;
                pot[2].y = xpt_xy.y;
                break;
            }
        }   
    }
    GUI_DrowSign(LCD_ADJX_MAX, LCD_ADJY_MIN, 0x0000);
    ADJ_CHECK_DELAY;
    GUI_DrowSign(LCD_ADJX_MAX, LCD_ADJY_MAX, 0x0FF0);
    i = 0;
    while(1)
    {
        if(TOUCH_XPT_ReadXY())
        {
            i++;
            if(i > 10)
            {
                pot[3].x = xpt_xy.x;
                pot[3].y = xpt_xy.y;
                break;
            }
        }   
    }      
    GUI_DrowSign(LCD_ADJX_MAX, LCD_ADJY_MAX, 0x0000);

    /* �����ȡ�����ĸ�������ݣ����ϳɶԽǵ������� */

    px[0] = (pot[0].x + pot[1].x) / 2;
    py[0] = (pot[0].y + pot[2].y) / 2;
    px[1] = (pot[3].x + pot[2].x) / 2;
    py[1] = (pot[3].y + pot[1].y) / 2;
}

#endif

/****************************************************************************
*��������TOUCH_Adjust
*��  �룺��
*��  ������
*��  �ܣ�������У��
****************************************************************************/	  
 
void TOUCH_Adjust(void)
{
    uint px[2], py[2];
    float xFactor, yFactor;
    
    /* ��λȡ�� */
#if LCD_TOUCH_TYPE == 0
    TOUCH_ReadAdj(px, py);
#else

	/* ��(10,10)�ͣ�TFT_XMAX - 10, TFT_YMAX - 10�� ���������ϾͲ���ÿ�ο�����ҪУ����*/
	px[0] = 480;
    py[0] = 368;
    px[1] = 3660;
    py[1] = 3360;
	
#endif	

#if LCD_TOUCH_TYPE == 0
    /* ����������� */
    xFactor = (float)LCD_ADJx / (px[0] - px[1]);
    yFactor = (float)LCD_ADJy / (py[0] - py[1]);  
    
    /* ���ƫ���� */
    TouchAdj.xOffset = (int)LCD_ADJX_MAX - ((float)px[0] * xFactor + 0.5);
    TouchAdj.yOffset = (int)LCD_ADJY_MAX - ((float)py[0] * yFactor + 0.5);

    /* �����������������ݴ���Ȼ�󱣴� */
    TouchAdj.xFactor = xFactor * LCD_ADJ_MUX;
    TouchAdj.yFactor = yFactor * LCD_ADJ_MUX;
#endif

#if LCD_TOUCH_TYPE == 1  
  
    /* ����������� */
    xFactor = (float)LCD_ADJx / (px[1] - px[0]);
    yFactor = (float)LCD_ADJy / (py[1] - py[0]);

    /* ���ƫ���� */
    TouchAdj.xOffset = (int)LCD_ADJX_MAX - ((float)px[1] * xFactor + 0.5);
    TouchAdj.yOffset = (int)LCD_ADJY_MAX - ((float)py[1] * yFactor + 0.5);

    /* �����������������ݴ���Ȼ�󱣴� */
    TouchAdj.xFactor = xFactor * LCD_ADJ_MUX;
    TouchAdj.yFactor = yFactor * LCD_ADJ_MUX;

#endif
}


