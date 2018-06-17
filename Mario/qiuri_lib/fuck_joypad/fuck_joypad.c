#include "joypad.h"
#include "fuck_joypad.h"
#include "nes_main.h"
#include "lcd.h"
void fuck_NES_JoyPadUpdateValue()	 //systick 中断读取按键值
{


	u32 temp=0;
 	u8 t;
	JOYPAD_LAT=1;	//锁存当前状态
 	JOYPAD_Delay(80);
	JOYPAD_LAT=0; 
	for(t=0;t<20;t++)
	{
		temp>>=1;	 
		if(JOYPAD_DAT==0)temp|=0x80000;//LOAD之后，就得到第一个数据
		
		JOYPAD_CLK=1;			   	//每给一次脉冲，收到一个数据
		JOYPAD_Delay(80);
		JOYPAD_CLK=0;	
		JOYPAD_Delay(80); 
	}
	//POINT_COLOR=RED;
	// LCD_ShowNum(116,130,temp,3,16);
	
	
	JoyPad[0].value=temp;
	JoyPad[1].value=temp;

}