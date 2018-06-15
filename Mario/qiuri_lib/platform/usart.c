#include "platform.h"
#include "ringbuffer.h"

DECLARE_RB(rb_usart,USART_RD_BUF_LEN);

int __io_putchar(int ch)
{
	while(USART_GetFlagStatus(USART,USART_FLAG_TXE)!=SET);
	USART_SendData(USART,ch);
	return ch;
}

int __io_getchar(void)
{
	signed char ch;
	while(rbempty(&rb_usart)) continue;
	ch=rbget(&rb_usart);
	switch(ch)
	{
		case '\r':ch='\n';return __io_putchar(ch);
		case 0x1b:ch='^'; return __io_putchar(ch);
		case 0x08:/* backspace in minicom or putty */
		case 0x7F: 
			/* earse char in screen(replace with space
			 and return the backspace code */
			__io_putchar(ch); __io_putchar(' ');__io_putchar(ch);
			return ch;
		default: return __io_putchar(ch);
	}
	
	/* echo it */
	return __io_putchar(ch);
}

int __io_puts(const char *str)
{
	const char *tmp=str;
	while(*tmp!='\0'){
		__io_putchar(*tmp);
		tmp++;
	}
	return tmp-str;
}

/** gets one line from serial.Save all chars into buf,
 ** the length of buf is 'len'.
 ** return read string
 */
char * __io_gets(char *buf,int len)
{
	int idx=0;
	signed char ch;
	while(idx<len-1){ /* 1 is for '\0' */
		switch(ch=__io_getchar()){
		case '\n':buf[idx]='\0';return buf;
		case 0x08:
		case 0x7F: /* backspace in minicom or putty */
			if(idx>0) idx--;break;
		default:
			buf[idx]=ch;idx++;break;
		}
	}
	buf[len-1]='\0';/* data too long,buf is not enough */
	return buf;
}

/* USART interrupt handler */
USART_Handler()
{
	if(USART_GetITStatus(USART,USART_IT_RXNE)==SET){
		rbput(&rb_usart,USART_ReceiveData(USART));
	}
}
