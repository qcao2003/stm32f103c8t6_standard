#include"main.h"

PosTypeDef TouchAdj; //定义一阵数据用来保存校正因数
XPT_XY xpt_xy;	     //定义一个全局变量保存X、Y的值
#define XY_READ_TIMS  10		   //读取的次数

/****************************************************************************
*函数名：TOUCH_SPI_Start
*输  入：无
*输  出：无
*功  能：初始化触摸SPI
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
*函数名：TOUCH_SPI_Write
*输  入：dat：写入数据
*输  出：无
*功  能：使用SPI写入数据
****************************************************************************/

void TOUCH_SPI_Write(uchar dat)
{
	uchar i;
	TOUCH_CLK = 0;
	for(i=0; i<8; i++)
	{
		TOUCH_DIN = dat >> 7;  	//放置最高位
		dat <<= 1;
		TOUCH_CLK = 0;			//上升沿放置数据

		TOUCH_CLK = 1;

	}
}

/****************************************************************************
*函数名：TOUCH_SPI_Read
*输  入：无
*输  出：dat：读取 到的数据
*功  能：使用SPI读取数据
****************************************************************************/

uint TOUCH_SPI_Read(void)
{
	uint i, dat=0;
	TOUCH_CLK = 0;
	for(i=0; i<12; i++)		//接收12位数据
	{
		dat <<= 1;

		TOUCH_CLK = 1;
		TOUCH_CLK = 0;

		dat |= TOUCH_DOUT;

	}
	return dat;	
}

/****************************************************************************
*函数名：TOUCH_XPT_ReadData
*输  入：cmd：读取的X或者Y
*输  出：endValue：最终信号处理后返回的值
*功  能：读取触摸数据
****************************************************************************/

uint TOUCH_XPT_ReadData(uchar cmd)
{
	uchar i, j;
	uint readValue[XY_READ_TIMS];
	long endValue;

	TOUCH_CLK = 0;    //先拉低时间
	TOUCH_CS  = 0;    //选中芯片
	
	for(i=0; i<XY_READ_TIMS; i++)	 	//读取XY_READ_TIMS次结果
	{	
		TOUCH_SPI_Write(cmd);  //发送转换命令
		//Delay_6us();
		for(j=6; j>0; j--); 	//延时等待转换结果
		TOUCH_CLK = 1;	  //发送一个时钟周期，清除BUSY
		_nop_();
		_nop_();
		TOUCH_CLK = 0;
		_nop_();
		_nop_();
	
		readValue[i] = TOUCH_SPI_Read();
	}
	TOUCH_CS = 1; //释放片选

	//---软件滤波---//
	//---先大到小排序，除去最高值，除去最低值，求其平均值---//
   	for(i=0; i<XY_READ_TIMS - 1; i++)	//从大到小排序
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
	endValue = endValue/ (XY_READ_TIMS - 4);//求平均值

	return endValue;
}

/****************************************************************************
*函数名：TOUCH_XPT_ReadXY
*输  入：无
*输  出：1：成功 0：失败
*功  能：读取XY的AD值
****************************************************************************/

uchar TOUCH_XPT_ReadXY(void)
{	
	uint x1, x2, x, y1, y2, y;

	TOUCH_SPI_Start();
	//---分别读两次X值和Y值, 交叉着读可以提高一些读取精度---//
	x1 = TOUCH_XPT_ReadData(XPT_CMD_X);
	y1 = TOUCH_XPT_ReadData(XPT_CMD_Y);
	x2 = TOUCH_XPT_ReadData(XPT_CMD_X);
	y2 = TOUCH_XPT_ReadData(XPT_CMD_Y);

	//---求取X值的差值---//
	if (x1 > x2)
	{
		x = x1 - x2;
	}
	else
	{
		x = x2 - x1;
	}

	//---求取Y值的差值---//
	if (y1 > y2)
	{
		y = y1 - y2;
	}
	else
	{
		y = y2 - y1;
	}

	//---判断差值是否大于50，大于就返回0，表示读取失败---//
	if((x > 50) || (y > 50))  
	{
		return 0;
	}

	//---求取两次读取值的平均数作为读取到的XY值---//
	xpt_xy.x = (x1 + x2) / 2;
	xpt_xy.y = (y1 + y2) / 2;

	xpt_xy.x &= 0xFFF0;	  //去掉低四位
	xpt_xy.y &= 0xFFF0;

	//---确定XY值的范围，用在触摸屏大于TFT时---//
	if((xpt_xy.x < 100) || (xpt_xy.y > 3600))
	{
		return 0;
	}

	return 1;  // 返回1，表示读取成功
}


/****************************************************************************
*函数名：TOUCH_ReadAdj
*输  入：px：读取到的X坐标值
*      *           py：读取到的Y坐标值
*输  出：无
*功  能：给屏幕校正函数取四个点
****************************************************************************/	  
#if LCD_TOUCH_TYPE == 0

void TOUCH_ReadAdj(uint *px, uint *py)
{
    ulong i = 0;
    XPT_XY pot[4];

    /* 读取四个定位点的坐标 */
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

    /* 处理读取到的四个点的数据，整合成对角的两个点 */

    px[0] = (pot[0].x + pot[1].x) / 2;
    py[0] = (pot[0].y + pot[2].y) / 2;
    px[1] = (pot[3].x + pot[2].x) / 2;
    py[1] = (pot[3].y + pot[1].y) / 2;
}

#endif

/****************************************************************************
*函数名：TOUCH_Adjust
*输  入：无
*输  出：无
*功  能：触摸屏校正
****************************************************************************/	  
 
void TOUCH_Adjust(void)
{
    uint px[2], py[2];
    float xFactor, yFactor;
    
    /* 定位取点 */
#if LCD_TOUCH_TYPE == 0
    TOUCH_ReadAdj(px, py);
#else

	/* 将(10,10)和（TFT_XMAX - 10, TFT_YMAX - 10） 的坐标填上就不用每次开机都要校正了*/
	px[0] = 480;
    py[0] = 368;
    px[1] = 3660;
    py[1] = 3360;
	
#endif	

#if LCD_TOUCH_TYPE == 0
    /* 求出比例因数 */
    xFactor = (float)LCD_ADJx / (px[0] - px[1]);
    yFactor = (float)LCD_ADJy / (py[0] - py[1]);  
    
    /* 求出偏移量 */
    TouchAdj.xOffset = (int)LCD_ADJX_MAX - ((float)px[0] * xFactor + 0.5);
    TouchAdj.yOffset = (int)LCD_ADJY_MAX - ((float)py[0] * yFactor + 0.5);

    /* 将比例因数进行数据处理，然后保存 */
    TouchAdj.xFactor = xFactor * LCD_ADJ_MUX;
    TouchAdj.yFactor = yFactor * LCD_ADJ_MUX;
#endif

#if LCD_TOUCH_TYPE == 1  
  
    /* 求出比例因数 */
    xFactor = (float)LCD_ADJx / (px[1] - px[0]);
    yFactor = (float)LCD_ADJy / (py[1] - py[0]);

    /* 求出偏移量 */
    TouchAdj.xOffset = (int)LCD_ADJX_MAX - ((float)px[1] * xFactor + 0.5);
    TouchAdj.yOffset = (int)LCD_ADJY_MAX - ((float)py[1] * yFactor + 0.5);

    /* 将比例因数进行数据处理，然后保存 */
    TouchAdj.xFactor = xFactor * LCD_ADJ_MUX;
    TouchAdj.yFactor = yFactor * LCD_ADJ_MUX;

#endif
}


