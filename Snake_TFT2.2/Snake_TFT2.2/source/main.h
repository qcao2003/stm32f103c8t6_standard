#ifndef __MAIN_H__
#define __MAIN_H__


#include "stc12c5a60s2.h"
#include "typedef.h"
#include "intrins.h"
#include "stdlib.h"         //rand()Ëæ»úº¯Êý
#include "lcd_driver.h"
#include "gui.h"
#include "touch.h"
#include "Uart.h"

void GameButton();

void GamePlay();

void GameDis();
void SnakeDis();
void AboutDis();
void ScoreDis();
void GameOver();

void DelayMs(uint);


#endif

