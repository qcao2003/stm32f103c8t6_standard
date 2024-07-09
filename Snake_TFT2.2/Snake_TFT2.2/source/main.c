
#include "main.h"

long x, y;             //��Ļ����x(0-175), y(0-219)
uchar scorceValue[5] = {0, 0, 0, 0, 0}; //�ɼ�����

uchar where, page = 1;  
bit f_startGame;       //��ʼ��Ϸ��־λ
bit f_gameWait;        //��Ϸ��ͣ��־λ

#define N 50           //�ߵ���󳤶�(N - 1)
const uchar H = 108;   //��ͼ�ĸ�  
const uchar L = 174;   //��ͼ�ĳ�  

struct Food         /*ʳ������*/
{
   uchar fx;         /*ʳ��ĺ�����*/
   uchar fy;         /*ʳ���������*/
   uchar yes;        /*�ж��Ƿ�Ҫ����ʳ��ı���(1Ϊ��Ҫ����ʳ��)*/
}food;     

struct Snake        /*��ÿ���ڵ������*/
{
   uchar sx;         /*�ߵ�����*/
   uchar sy;
   int   direction;  /*���ƶ�����1,2,3,4��ʾ��,��,��,���ĸ�����*/
}Snake[N];

uchar SnakeNode;     /*�ߵĳ���(<= N - 1)*/


/****************************************************************************
*��������main
*��  �룺��
*��  ������
*��  �ܣ�������
****************************************************************************/

void main()
{
	uchar rst = 0;

  Uart_init();
	TFT_Init();
  //-- ����У����Ĭ��ʹ�ÿ�����У������������ԶԿ��Ը������ú��������У���� --//
	//-- �޸�LCD_TOUCH_TYPE���ı�У��ģʽ��������touch.h��45�� --//
	TOUCH_Adjust();
	rst = 1;

	while(1)
	{
		if(rst == 1)
		{	
			SnakeDis();  
			rst = 0;
		}

      
			if(TOUCH_XPT_ReadXY() == 1)
			{
				//--�����������ʾ����ƫ�ƣ����Ը�����ʾADֵ--//
				//--�������湫ʽ�������ֵ--//
							x = (xpt_xy.x * TouchAdj.xFactor / LCD_ADJ_MUX + TouchAdj.xOffset);
							y = (xpt_xy.y * TouchAdj.yFactor / LCD_ADJ_MUX + TouchAdj.yOffset);
			
				if (x > 175 )
				{
					x = 175;
				}
				
				if (y > 219)
				{
					y = 219;
				}     
			
				if ((x > 150) && (y < 29))  //��Ļˢ��
				{
					rst = 1;
				}
				else
				{
					//GUI_Dot((uint)x , (uint)y, BLUE);  
					
					#if 1
					SendString("Now x zoubiao: ");
					SendData(x / 100 + 0x30);
					SendData(x / 10 % 10 + 0x30);
					SendData(x % 10 + 0x30);
					SendString("\r\n");
					
					SendString("Now y zoubiao: ");
					SendData(y / 100 + 0x30);
					SendData(y / 10 % 10 + 0x30);
					SendData(y % 10 + 0x30);
					SendString("\r\n");
					#endif

				}

				if((x >= 45 && x <= 128) && (y >= 140 && y <= 164)) //start ʶ����
				{
					if(page == 1)
					  where = 1; 
				}

				if((x >= 45 && x <= 128) && (y >= 170 && y <= 194)) //about ʶ����
				{
					if(page == 1)
					  where = 2;
				}

				if(x <= 29 && y >= 190) //back �˰�ťΪ������Ļ���½�30*30 ʶ����
				{
					if(page == 3)
					  where = 3; 
					
					if(page == 2)
						where = 3;
					  f_startGame = 0;  //�˳���Ϸʶ����
				}

				if(x <= 88 && (y >= 55 && y <= 110))  //��Ϸ��ͣʶ����
				{
					if(page == 2)
						where = 8;
				}

				GameButton();  //��Ϸ���ư�ťʶ����
			}


      if(f_startGame && (~f_gameWait)) //��ʼ��Ϸ
      {
			  GamePlay();
			}
			
			
			switch(where)   //�������Ҫ��ɭô��
			{
				case 1:  //���start
					  SendString("you touch start button\r\n");
						GameDis();

						/*��Ϸ��ʼ��*/
						f_startGame = 1;  
						food.yes = 1;  /*1��ʾ��Ҫ������ʳ��,0��ʾ�Ѿ�����ʳ��*/ 
						SnakeNode = 3;
						Snake[0].direction = 1;  /*��ͷ��������*/ 
						
						Snake[0].sx = 141; Snake[0].sy = 164;   /*��ʼ�ߵ�body */
						Snake[1].sx = 140; Snake[1].sy = 164;
						Snake[2].sx = 139; Snake[2].sy = 164;

						page = 2;
					break;
					
				case 2:	//���about
			  		SendString("you touch about button\r\n");
				    AboutDis();
						page = 3;
					break;
					
				case 3:	//���back
					  SendString("you touch back button\r\n");
					  SnakeDis();
						page = 1;
					break;

				case 4: //up
					SendString("up\r\n");
					Snake[0].direction = 3;
					break;

				case 5: //down
					SendString("down\r\n");
					Snake[0].direction = 4;
					break;

				case 6: //left
					SendString("left\r\n");
					Snake[0].direction = 2;
					break;

				case 7: //right
					SendString("right\r\n");
					Snake[0].direction = 1;
					break;

				case 8: //��Ϸ��ʼ(��ͣ)
				  f_gameWait = ~f_gameWait;
					DelayMs(1000);   //��ֹ�������е�2�£����ԸĴ��
					break;
					
				default:
					break;
			} where = 0;
	}
}	


void GamePlay()  /*̰������Ϸ*/
{
	uint i;
	
  for(i = SnakeNode; i > 0; i--)  /*�ߵ�ÿ��������ǰ�ƶ�,Ҳ����̰���� 
                                                         �Ĺؼ��㷨*/ /*Snake[SnakeNode]  ����ȥβ��*/
  { 
    Snake[i].sx = Snake[i-1].sx; 
		Snake[i].sy = Snake[i-1].sy;
  } 
	

  if(food.yes == 1)  /*��Ҫ������ʳ��*/ 
  { 
		food.fx = rand() % L + 1;    /*�������ʳ��ĺ�����*/ 
		food.fy = rand() % H + 111;  /*�������ʳ���������*/ 

		for(i = 0; i <= SnakeNode; i++)  /*��ʳ������걻��ռ��*/
		{
			if((Snake[i].sx == food.fx) && (Snake[i].sy == food.fy))
			{
			  food.yes = 1;
				break;
			}
		}
		
		food.yes = 0;  /*��������ʳ����*/ 
	} 
	
	if(food.yes == 0)  /*��������ʳ���˾�Ҫ��ʾ*/ 
	{ 
		GUI_Dot(food.fx, food.fy, BLUE);  /*ʳ����ʾ��ɫ*/
	}
  #if 1
		SendString("ʳ������ : ");
		SendData(food.fx / 100 + 0x30);
		SendData(food.fx / 10 % 10 + 0x30);
		SendData(food.fx % 10 + 0x30);
		SendString("  ");
		SendData(food.fy / 100 + 0x30);
		SendData(food.fy / 10 % 10 + 0x30);
		SendData(food.fy % 10 + 0x30);
		SendString("\r\n");
	#endif



	switch(Snake[0].direction)  /*1,2,3,4��ʾ��,��,��,���ĸ�����,ͨ������ж� 
                                                  ���ƶ���ͷ*/ 
  { 
    case 1: Snake[0].sx += 1; break; /*��*/ 
    case 2: Snake[0].sx -= 1; break; /*��*/ 
    case 3: Snake[0].sy -= 1; break; /*��*/ 
    case 4: Snake[0].sy += 1; break; /*��*/ 
  }
	
	#if 0
		SendString("direction : ");
		SendData(Snake[0].direction + 0x30);
		SendString("\r\n");
	#endif
 

	for(i = 0; i <= SnakeNode; i++)  /*��Ļ��ʾ*/
	{
    GUI_Dot(Snake[i].sx, Snake[i].sy, BLACK);
		if(i == SnakeNode)             /*ȥβ��*/
			GUI_Dot(Snake[i].sx, Snake[i].sy, WHITE);
	}
	DelayMs(1000); 									 /*�ߵ��ƶ��ٶ�*/

	
  if((Snake[0].sx == food.fx) && (Snake[0].sy == food.fy))  /*�߳Ե�ʳ��*/
  {
		SnakeNode++;
		food.yes = 1; 
	}
	
  ScoreDis();                /*��ʾ�ɼ�*/



	for(i = 1;i < SnakeNode; i++)  /*�ж����Ƿ�ײ���Լ�*/
  { 
    if(Snake[i].sx == Snake[0].sx && Snake[i].sy == Snake[0].sy)  /*��ͷ�Ƿ�ײ������*/ 
    { 
			f_startGame = 0;
      GameOver();                /*������Ϸ����*/ 
      break; 
    } 
  } 

	if(Snake[0].sx <= 1 || Snake[0].sx >= 174 ||  /*�ж����Ƿ�ײ��ǽ��*/
		 Snake[0].sy <= 111 || Snake[0].sy >= 218)           
  { 
		f_startGame = 0;
    GameOver();                  /*������Ϸ����*/ 
  }
}



void GameButton()  /*��Ϸ����Ŀ��ư�ť(λ�����Ͻ�)*/
{
	float k = 1.25;
	float y1 = 0.0, y2 = 0.0;
	y1 = k * x - 110;
	y2 = -(k * x) + 220;
	if(x >= 88 && y <= 110 && page == 2) 
	{
		if(y < y1 && y < y2)
		{
			where = 4;
		}
		
		if(y < y1 && y > y2)
		{
			where = 7;
		}
		
		if(y > y1 && y > y2)
		{
			where = 5;
		}
		
		if(y > y1 && y < y2)
		{
			where = 6;
		}
	}
}


void SnakeDis()  /*������ʾ*/
{
	TFT_ClearScreen(WHITE);
	GUI_WriteASCII16x24(48, 0, "SNAKE", RED, WHITE);
	GUI_Line(13, 29, 163, 29, RED);
	GUI_ShowPicture(13, 40, 150, 150);				
	GUI_WriteASCII16x24(48, 140, "start", RED, WHITE);
	GUI_WriteASCII16x24(48, 170, "about", RED, WHITE);
}

void GameDis()  /*���start   ����ʾ(��Ϸ����)*/
{
	TFT_ClearScreen(WHITE);
	
	GUI_Line(0, 55, 87, 55, RED);
	GUI_Line(87, 0, 87, 110, RED);
	
	GUI_WriteASCII16x24(0, 0, "score", BLACK, WHITE);
	GUI_WriteASCII16x24(12, 70, "STOP", MAGENTA, WHITE);

  GUI_Line(0, 110, 175, 110, RED);
	GUI_Line(0, 110, 0, 219, RED);
	GUI_Line(175, 110, 175, 219, RED);
	GUI_Line(0, 219, 175, 219, RED);

  GUI_Line(88, 110, 175, 0, GREEN);
	GUI_Line(88, 0, 175, 110, GREEN);

}

void AboutDis()  /*���about   ����ʾ(about����)*/
{
	TFT_ClearScreen(WHITE);	
	GUI_WriteASCII16x24(48, 0, "ABOUT", BLACK, WHITE);	
	GUI_Line(13, 29, 163, 29, BLACK);	
	
	GUI_WriteASCII16x24(10, 34, "Author:", YELLOW, WHITE);
	GUI_WriteASCII16x24(10, 63, "   Jove   ", YELLOW, WHITE);
	GUI_WriteASCII16x24(10, 92, "Date:", YELLOW, WHITE);
	GUI_WriteASCII16x24(10, 121, "2016-09-18", YELLOW, WHITE);
}

void ScoreDis()  /*�ɼ���ʾ*/
{
	scorceValue[0] = (SnakeNode - 3) / 1000 + 0x30;
	scorceValue[1] = (SnakeNode - 3) / 100 % 10 + 0x30;
	scorceValue[2] = (SnakeNode - 3) / 10 % 10 + 0x30;
	scorceValue[3] = (SnakeNode - 3) % 10 + 0x30;
	
  #if 0
    SendString("SnakeNode : ");
		SendData((SnakeNode - 3) / 1000 + 0x30);
		SendData((SnakeNode - 3) / 100 % 10 + 0x30);
		SendData((SnakeNode - 3) / 10 % 10 + 0x30);		
		SendData((SnakeNode - 3) % 10 + 0x30);
		SendString("\r\n");
	#endif
	
  GUI_WriteASCII16x24(12, 27, scorceValue, MAGENTA, WHITE);
}

void GameOver()      /*��Ϸ����*/
{
	TFT_ClearScreen(WHITE);
	
	GUI_WriteASCII16x24(0, 0, "score", BLACK, WHITE);
	ScoreDis();
		
	GUI_WriteASCII16x24(56, 100, "Game", YELLOW, WHITE);
	GUI_WriteASCII16x24(56, 130, "Over", YELLOW, WHITE);
}

void DelayMs(uint a) {	  //��ʱx ms
	uchar i;
	while( --a != 0) {		
		for(i = 0; i < 125; i++); 
	}   				  
}


