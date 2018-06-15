#ifndef platform_h
#define platform_h

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"

#define USART USART2
#define USART_GPIO GPIOA
#define USART_BAUD 115200
#define USART_RD_BUF_LEN 256
#define USART_GPIO_AF GPIO_AF_7
#define USART_RX_PIN GPIO_Pin_2
#define USART_TX_PIN GPIO_Pin_3
#define USART_RX_SRC GPIO_PinSource2
#define USART_TX_SRC GPIO_PinSource3
#define USART_IRQn   USART2_IRQn
#define USART_Handler() void USART2_IRQHandler(void)
#define USART_ClkEn() \
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE)


#endif


