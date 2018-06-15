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

JoyPadType JoyPad[2];

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
