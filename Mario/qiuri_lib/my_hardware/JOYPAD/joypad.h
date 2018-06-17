#ifndef __JOYPAD_H
#define __JOYPAD_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板V3
//游戏手柄驱动 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2015/1/16
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

//手柄连接引脚
#define JOYPAD_CLK PDout(3)  	//时钟		PD3
#define JOYPAD_LAT PBout(11)  	//锁存     	PB11
#define JOYPAD_DAT PBin(10)	 	//数据     	PB10    

void JOYPAD_Init(void);			//初始化	
void JOYPAD_Delay(u16 t);		//延时
u8 JOYPAD_Read(void);			//读取键值	 				    
#endif
















