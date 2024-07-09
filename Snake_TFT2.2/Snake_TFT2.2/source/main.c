
#include "main.h"

long x, y;             //屏幕坐标x(0-175), y(0-219)
uchar scorceValue[5] = {0, 0, 0, 0, 0}; //成绩变量

uchar where, page = 1;  
bit f_startGame;       //开始游戏标志位
bit f_gameWait;        //游戏暂停标志位

#define N 50           //蛇的最大长度(N - 1)
const uchar H = 108;   //地图的高  
const uchar L = 174;   //地图的长  

struct Food         /*食物数据*/
{
   uchar fx;         /*食物的横坐标*/
   uchar fy;         /*食物的纵坐标*/
   uchar yes;        /*判断是否要出现食物的变量(1为需要生成食物)*/
}food;     

struct Snake        /*蛇每个节点的数据*/
{
   uchar sx;         /*蛇的坐标*/
   uchar sy;
   int   direction;  /*蛇移动方向1,2,3,4表示右,左,上,下四个方向*/
}Snake[N];

uchar SnakeNode;     /*蛇的长度(<= N - 1)*/


/****************************************************************************
*函数名：main
*输  入：无
*输  出：无
*功  能：主函数
****************************************************************************/

void main()
{
	uchar rst = 0;

  Uart_init();
	TFT_Init();
  //-- 触摸校正，默认使用开机不校正，如果触摸对对可以改正，该函数里面的校正点 --//
	//-- 修改LCD_TOUCH_TYPE来改变校正模式，定义在touch.h的45行 --//
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
				//--如果触摸跟显示发生偏移，可以根据显示AD值--//
				//--调整下面公式里面的数值--//
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
			
				if ((x > 150) && (y < 29))  //屏幕刷新
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

				if((x >= 45 && x <= 128) && (y >= 140 && y <= 164)) //start 识别区
				{
					if(page == 1)
					  where = 1; 
				}

				if((x >= 45 && x <= 128) && (y >= 170 && y <= 194)) //about 识别区
				{
					if(page == 1)
					  where = 2;
				}

				if(x <= 29 && y >= 190) //back 此按钮为隐藏屏幕左下角30*30 识别区
				{
					if(page == 3)
					  where = 3; 
					
					if(page == 2)
						where = 3;
					  f_startGame = 0;  //退出游戏识别区
				}

				if(x <= 88 && (y >= 55 && y <= 110))  //游戏暂停识别区
				{
					if(page == 2)
						where = 8;
				}

				GameButton();  //游戏控制按钮识别区
			}


      if(f_startGame && (~f_gameWait)) //开始游戏
      {
			  GamePlay();
			}
			
			
			switch(where)   //点了哪里，要做森么。
			{
				case 1:  //点击start
					  SendString("you touch start button\r\n");
						GameDis();

						/*游戏初始化*/
						f_startGame = 1;  
						food.yes = 1;  /*1表示需要出现新食物,0表示已经存在食物*/ 
						SnakeNode = 3;
						Snake[0].direction = 1;  /*蛇头方向往右*/ 
						
						Snake[0].sx = 141; Snake[0].sy = 164;   /*初始蛇的body */
						Snake[1].sx = 140; Snake[1].sy = 164;
						Snake[2].sx = 139; Snake[2].sy = 164;

						page = 2;
					break;
					
				case 2:	//点击about
			  		SendString("you touch about button\r\n");
				    AboutDis();
						page = 3;
					break;
					
				case 3:	//点击back
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

				case 8: //游戏开始(暂停)
				  f_gameWait = ~f_gameWait;
					DelayMs(1000);   //防止程序误判点2下，可以改大点
					break;
					
				default:
					break;
			} where = 0;
	}
}	


void GamePlay()  /*贪吃蛇游戏*/
{
	uint i;
	
  for(i = SnakeNode; i > 0; i--)  /*蛇的每个环节往前移动,也就是贪吃蛇 
                                                         的关键算法*/ /*Snake[SnakeNode]  用来去尾巴*/
  { 
    Snake[i].sx = Snake[i-1].sx; 
		Snake[i].sy = Snake[i-1].sy;
  } 
	

  if(food.yes == 1)  /*需要出现新食物*/ 
  { 
		food.fx = rand() % L + 1;    /*随机定义食物的横坐标*/ 
		food.fy = rand() % H + 111;  /*随机定义食物的纵坐标*/ 

		for(i = 0; i <= SnakeNode; i++)  /*新食物的坐标被蛇占用*/
		{
			if((Snake[i].sx == food.fx) && (Snake[i].sy == food.fy))
			{
			  food.yes = 1;
				break;
			}
		}
		
		food.yes = 0;  /*画面上有食物了*/ 
	} 
	
	if(food.yes == 0)  /*画面上有食物了就要显示*/ 
	{ 
		GUI_Dot(food.fx, food.fy, BLUE);  /*食物显示蓝色*/
	}
  #if 1
		SendString("食物坐标 : ");
		SendData(food.fx / 100 + 0x30);
		SendData(food.fx / 10 % 10 + 0x30);
		SendData(food.fx % 10 + 0x30);
		SendString("  ");
		SendData(food.fy / 100 + 0x30);
		SendData(food.fy / 10 % 10 + 0x30);
		SendData(food.fy % 10 + 0x30);
		SendString("\r\n");
	#endif



	switch(Snake[0].direction)  /*1,2,3,4表示右,左,上,下四个方向,通过这个判断 
                                                  来移动蛇头*/ 
  { 
    case 1: Snake[0].sx += 1; break; /*右*/ 
    case 2: Snake[0].sx -= 1; break; /*左*/ 
    case 3: Snake[0].sy -= 1; break; /*上*/ 
    case 4: Snake[0].sy += 1; break; /*下*/ 
  }
	
	#if 0
		SendString("direction : ");
		SendData(Snake[0].direction + 0x30);
		SendString("\r\n");
	#endif
 

	for(i = 0; i <= SnakeNode; i++)  /*屏幕显示*/
	{
    GUI_Dot(Snake[i].sx, Snake[i].sy, BLACK);
		if(i == SnakeNode)             /*去尾巴*/
			GUI_Dot(Snake[i].sx, Snake[i].sy, WHITE);
	}
	DelayMs(1000); 									 /*蛇的移动速度*/

	
  if((Snake[0].sx == food.fx) && (Snake[0].sy == food.fy))  /*蛇吃到食物*/
  {
		SnakeNode++;
		food.yes = 1; 
	}
	
  ScoreDis();                /*显示成绩*/



	for(i = 1;i < SnakeNode; i++)  /*判断蛇是否撞到自己*/
  { 
    if(Snake[i].sx == Snake[0].sx && Snake[i].sy == Snake[0].sy)  /*蛇头是否撞到身体*/ 
    { 
			f_startGame = 0;
      GameOver();                /*本次游戏结束*/ 
      break; 
    } 
  } 

	if(Snake[0].sx <= 1 || Snake[0].sx >= 174 ||  /*判断蛇是否撞到墙壁*/
		 Snake[0].sy <= 111 || Snake[0].sy >= 218)           
  { 
		f_startGame = 0;
    GameOver();                  /*本次游戏结束*/ 
  }
}



void GameButton()  /*游戏界面的控制按钮(位于右上角)*/
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


void SnakeDis()  /*开机显示*/
{
	TFT_ClearScreen(WHITE);
	GUI_WriteASCII16x24(48, 0, "SNAKE", RED, WHITE);
	GUI_Line(13, 29, 163, 29, RED);
	GUI_ShowPicture(13, 40, 150, 150);				
	GUI_WriteASCII16x24(48, 140, "start", RED, WHITE);
	GUI_WriteASCII16x24(48, 170, "about", RED, WHITE);
}

void GameDis()  /*点击start   后显示(游戏界面)*/
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

void AboutDis()  /*点击about   后显示(about界面)*/
{
	TFT_ClearScreen(WHITE);	
	GUI_WriteASCII16x24(48, 0, "ABOUT", BLACK, WHITE);	
	GUI_Line(13, 29, 163, 29, BLACK);	
	
	GUI_WriteASCII16x24(10, 34, "Author:", YELLOW, WHITE);
	GUI_WriteASCII16x24(10, 63, "   Jove   ", YELLOW, WHITE);
	GUI_WriteASCII16x24(10, 92, "Date:", YELLOW, WHITE);
	GUI_WriteASCII16x24(10, 121, "2016-09-18", YELLOW, WHITE);
}

void ScoreDis()  /*成绩显示*/
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

void GameOver()      /*游戏结束*/
{
	TFT_ClearScreen(WHITE);
	
	GUI_WriteASCII16x24(0, 0, "score", BLACK, WHITE);
	ScoreDis();
		
	GUI_WriteASCII16x24(56, 100, "Game", YELLOW, WHITE);
	GUI_WriteASCII16x24(56, 130, "Over", YELLOW, WHITE);
}

void DelayMs(uint a) {	  //延时x ms
	uchar i;
	while( --a != 0) {		
		for(i = 0; i < 125; i++); 
	}   				  
}


