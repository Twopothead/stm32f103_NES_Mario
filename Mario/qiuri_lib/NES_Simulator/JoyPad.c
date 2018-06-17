/**
  ******************************************************************************
  * @file    
  * @author  
  * @version 
  * @date    
  * @brief  
  ******************************************************************************
  * @attention
  *
  * 
  ******************************************************************************
  */  
#include "nes_main.h"


uint8 NES_GetJoyPadVlaue(int JoyPadNum)
{
	uint8 retval = 0;

	retval = (JoyPad[JoyPadNum].value >> JoyPad[JoyPadNum].index) & 0x01;	 //A, B, SELECT, START, UP, DOWN, LEFT, RIGHT
	JoyPad[JoyPadNum].index++;	

	return retval;
}

void NES_JoyPadReset(void)
{
	JoyPad[0].state = JOYPADENABLE;
    JoyPad[0].index = 0;
	
	JoyPad[1].state = JOYPADENABLE;
	JoyPad[0].index = 0;
}

void NES_JoyPadInit(void)
{
	JoyPad[0].state = JOYPADDISABLE;
    JoyPad[0].index = 0;
	JoyPad[0].value = 1 << 20;

	JoyPad[1].state = JOYPADDISABLE;
    JoyPad[1].index = 0;
	JoyPad[1].value = 1 << 19;
  JOYPAD_Init();
}

void NES_JoyPadDisable(void)
{
//   JoyPad[0].state = JOYPADDISABLE;
//   JoyPad[1].state = JOYPADDISABLE;
}

  /**
  * @}
  */


/*******************************END OF FILE***********************************/


// void update(int joy,int value,unsigned char code){
// //https://blog.csdn.net/apple_boys/article/details/41696481  
//       value = 1;//强制使用第一个手柄
//         if(value==1|value==2)  
//                 joypad[joy].value|=code;  
//         else if(value==0)  
//                 joypad[joy].value&=(~code);  
// }  
// void NES_UpdateJoyValue(){  
//         for(int ii=0;ii<5;ii++){  
//         int num=read(inputfd,&t,sizeof(t));  
//         read(0,buf,sizeof(buf));  
//         if(num==sizeof(t)){  
//          if(t.type==EV_KEY){  
//             switch(t.code){  
//                 case 17:break;  
//                 case 31:update(0,t.value,0x10);break;  
//                 case 30:update(0,t.value,0x40);break;  
//                 case 32:update(0,t.value,0x80);break;  
//                 case 36:update(0,t.value,0x01);break;//A  
//                 case 37:update(0,t.value,0x02);break;//B  
//                 case 2: update(0,t.value,0x04);break;//select  
//                 case 3: update(0,t.value,0x08);break;//start  
//                 case 4:nesruning=0;break;  
//                 default:break;  
//                 }  
//          }  
//         }  
//         }  
// }  