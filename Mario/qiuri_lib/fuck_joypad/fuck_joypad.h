#ifndef __FUCK_JOYPAD_H
#define __FUCK_JOYPAD_H	
#include "stm32f10x.h"

#ifdef __cplusplus
 extern "C" {
#endif

void fuck_NES_JoyPadUpdateValue();	 //systick 中断读取按键值

#ifdef __cplusplus
}
#endif

#endif
