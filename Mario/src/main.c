//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//
#include "stm32f10x.h"

#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"

#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "24cxx.h"
#include "w25qxx.h"
#include "touch.h"


#include "font.h"
// extern const unsigned char asc2_1206[95][12];

static u8 fac_us = 0;  //us延时倍乘数
static u16 fac_ms = 0; //ms

//总共：3个字符集（12*12、16*16和24*24），用户可以自行新增其他分辨率的字符集。
extern const unsigned char asc2_1206[95][12];
extern const unsigned char asc2_1608[95][16];
extern const unsigned char asc2_2412[95][36];

/************************************************
 ALIENTEK战舰STM32开发板实验27
 触摸屏 实验 
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/



//清空屏幕并在右上角显示"RST"
void Load_Drow_Dialog(void)
{
	LCD_Clear(WHITE);	//清屏   
 	POINT_COLOR=BLUE;	//设置字体为蓝色 
	LCD_ShowString(lcddev.width-24,0,200,16,16,"RST");//显示清屏区域
  	POINT_COLOR=RED;	//设置画笔蓝色 
}
////////////////////////////////////////////////////////////////////////////////
//电容触摸屏专有部分
//画水平线
//x0,y0:坐标
//len:线长度
//color:颜色
void gui_draw_hline(u16 x0,u16 y0,u16 len,u16 color)
{
	if(len==0)return;
	LCD_Fill(x0,y0,x0+len-1,y0,color);	
}
//画实心圆
//x0,y0:坐标
//r:半径
//color:颜色
void gui_fill_circle(u16 x0,u16 y0,u16 r,u16 color)
{											  
	u32 i;
	u32 imax = ((u32)r*707)/1000+1;
	u32 sqmax = (u32)r*(u32)r+(u32)r/2;
	u32 x=r;
	gui_draw_hline(x0-r,y0,2*r,color);
	for (i=1;i<=imax;i++) 
	{
		if ((i*i+x*x)>sqmax)// draw lines from outside  
		{
 			if (x>imax) 
			{
				gui_draw_hline (x0-i+1,y0+x,2*(i-1),color);
				gui_draw_hline (x0-i+1,y0-x,2*(i-1),color);
			}
			x--;
		}
		// draw lines from inside (center)  
		gui_draw_hline(x0-x,y0+i,2*x,color);
		gui_draw_hline(x0-x,y0-i,2*x,color);
	}
}  
//两个数之差的绝对值 
//x1,x2：需取差值的两个数
//返回值：|x1-x2|
u16 my_abs(u16 x1,u16 x2)
{			 
	if(x1>x2)return x1-x2;
	else return x2-x1;
}  
//画一条粗线
//(x1,y1),(x2,y2):线条的起始坐标
//size：线条的粗细程度
//color：线条的颜色
void lcd_draw_bline(u16 x1, u16 y1, u16 x2, u16 y2,u8 size,u16 color)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	if(x1<size|| x2<size||y1<size|| y2<size)return; 
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		gui_fill_circle(uRow,uCol,size,color);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}   
////////////////////////////////////////////////////////////////////////////////
 //5个触控点的颜色												 
const u16 POINT_COLOR_TBL[CT_MAX_TOUCH]={RED,GREEN,BLUE,BROWN,GRED};  
//电阻触摸屏测试函数
void rtp_test(void)
{
	u8 key;
	u8 i=0;	  
	while(1)
	{
	 	key=KEY_Scan(0);
		tp_dev.scan(0); 		 
		if(tp_dev.sta&TP_PRES_DOWN)			//触摸屏被按下
		{	
		 	if(tp_dev.x[0]<lcddev.width&&tp_dev.y[0]<lcddev.height)
			{	
				if(tp_dev.x[0]>(lcddev.width-24)&&tp_dev.y[0]<16)Load_Drow_Dialog();//清除
				else TP_Draw_Big_Point(tp_dev.x[0],tp_dev.y[0],RED);		//画图	  			   
			}
		}else delay_ms(10);	//没有按键按下的时候 	    
		if(key==KEY0_PRES)	//KEY0按下,则执行校准程序
		{
			LCD_Clear(WHITE);//清屏
		    TP_Adjust();  	//屏幕校准  
			Load_Drow_Dialog();
		}
		i++;
		if(i%20==0)LED0=!LED0;
	}
}
//电容触摸屏测试函数
void ctp_test(void)
{
	u8 t=0;
	u8 i=0;	  	    
 	u16 lastpos[5][2];		//记录最后一次的数据 
	while(1)
	{
		tp_dev.scan(0);
		for(t=0;t<CT_MAX_TOUCH;t++)
		{
			if((tp_dev.sta)&(1<<t))
			{
				if(tp_dev.x[t]<lcddev.width&&tp_dev.y[t]<lcddev.height)
				{
					if(lastpos[t][0]==0XFFFF)
					{
						lastpos[t][0] = tp_dev.x[t];
						lastpos[t][1] = tp_dev.y[t];
					}
					lcd_draw_bline(lastpos[t][0],lastpos[t][1],tp_dev.x[t],tp_dev.y[t],2,POINT_COLOR_TBL[t]);//画线
					lastpos[t][0]=tp_dev.x[t];
					lastpos[t][1]=tp_dev.y[t];
					if(tp_dev.x[t]>(lcddev.width-24)&&tp_dev.y[t]<16)
					{
						Load_Drow_Dialog();//清除
					}
				}
			}else lastpos[t][0]=0XFFFF;
		}
		
		delay_ms(5);i++;
		if(i%20==0)LED0=!LED0;
	}	
}




 


// int main(void)
// {


// 	delay_init();	    	 //延时函数初始化	  
//   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
// 	uart_init(115200);	 	//串口初始化为115200
	 
//  	LED_Init();			     //LED端口初始化

 	
// 	// LCD_Init();
// 	//!!!!!!!!!!!!!!LCD_Init does not work!!!!!!!!!!!!! 
// 	#include <fuck.txt>
// 	/****************************************************/
//  	LCD_Clear(GREEN);
//  	POINT_COLOR=RED;//设置字体为红色 
// 	LCD_ShowString(60,50,200,16,16,"please fuck my asshole");
// 	delay_ms(1500);	
// 	// LCD_Clear(YELLOW);
// 	tp_dev.init();
// }



/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>

#include "nes_main.h"
#include "stm32f10x.h"

// void LCD_Init(void);
// void LCD_Clear(unsigned int color);

// #ifdef __GNUC__
//   #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
// #else
//   #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
// #endif

void LED_Config(void)
{
}

void KeyPortConfig(void)
{
}

void USART1_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;  

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO | RCC_APB2Periph_USART1, ENABLE);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	  
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Configure USART Rx as input floating */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* USARTx configured as follow:
        - BaudRate = 115200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  USART_Init(USART1, &USART_InitStructure);
  USART_Cmd(USART1, ENABLE);
}

int main(void)
{


	delay_init();	    	 //延时函数初始化	  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
	 
 	LED_Init();			     //LED端口初始化

 	
	// LCD_Init();
	//!!!!!!!!!!!!!!LCD_Init does not work!!!!!!!!!!!!! 
	#include <fuck.txt>
	/****************************************************/
 // 	LCD_Clear(GREEN);
 // 	POINT_COLOR=RED;//设置字体为红色 
	// LCD_ShowString(60,50,200,16,16,"please fuck my asshole");
	// delay_ms(1500);	
	// LCD_Clear(YELLOW);
	// tp_dev.init();
// }
// int main(void)
// {
//   LCD_Init(); 
  //LED_Config();
  KeyPortConfig();
  USART1_Init();
  printf("\r\n");
  printf("nes emulator for stm32f103tb\r\n");

  if(SysTick_Config(72000000 / 60)){ // 60Hz
    /* Capture error */ 
    while (1);
  }

  // LCD_Clear(YELLOW);
  // gui_fill_circle(100,400,180,RED);
  
 nes_main();

// LCD_Clear(GREEN);
 // u32 tempp=10;
	//POINT_COLOR=RED;
	// LCD_ShowNum(80,50,tempp,32,32);
	//LCD_ShowNum(116,130,520,3,16);//显示键值
tp_dev.init();
///////////////////////////////////////////////////
// #include "nes_main.h"

// extern uint8 Continue=TRUE;
// extern int FrameCnt;

// #include "rom.h"
// NesHeader *neshreader = (NesHeader*)rom_file;

// 	if(strncmp(neshreader->filetype, "NES", 3) != 0){
// 		printf("invalid nes rom\r\n");
// 		return;
// 	}
//   else{
// 		printf("16kB  ROM: %d\r\n", neshreader->romnum);
// 		printf(" 8kB VROM: %d\r\n", neshreader->vromnum);
// 		if((neshreader->romfeature & 0x01) == 0){
// 			printf("romfeature 0x01\r\n");
// 		}

// 		if((neshreader->romfeature & 0x02) == 0){
// 			printf("romfeature 0x02\r\n");
// 		}

// 		if((neshreader->romfeature & 0x04) == 0){
// 			printf("romfeature 0x04\r\n");
// 		}

// 		if((neshreader->romfeature & 0x08) == 0){
// 			printf("romfeature 0x08\r\n");
// 		}
// 		printf("iNES Mapper Numbers: %d\r\n", (neshreader->rommappernum & 0xF0) | ( neshreader->romfeature >> 4));
// 	}
	
//   printf("intialize 6502...\r\n");
// 	init6502mem(0, 0, (&rom_file[0x10]), neshreader->romnum);
//   printf("intialize 6502 complete\r\n");
  
//   printf("reset 6502...\r\n");
// 	reset6502();
//   printf("reset 6502 complete\r\n");
  
//   printf("initialize ppu...\r\n");
// 	PPU_Init((&rom_file[0x10] + (neshreader->romnum * 0x4000)), (neshreader->romfeature & 0x01));
//   printf("initialize ppu complete\r\n");
  
//   printf("initialize joypad...\r\n");
// 	NES_JoyPadInit();
//   printf("initialize joypad complete\r\n");
  
//   printf("frame cycle...\r\n");
// 	NesFrameCycle();













////////////////////////////////////////////////////





  // tp_dev.init();
  while(1){
  }
}

// PUTCHAR_PROTOTYPE
// {
//   while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){
//   }

//   USART_SendData(USART1, (uint8_t)ch);	
//   return ch;
// }

// #ifdef USE_FULL_ASSERT

// void assert_failed(uint8_t* file, uint32_t line)
// { 
//   while(1){
//   }
// }
// #endif
