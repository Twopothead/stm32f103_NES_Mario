#ifndef ringbuffer_h
#define ringbuffer_h

#include <stdint.h>

struct ringbuffer
{
	uint8_t *bf;
	int len;
	
	volatile int count;
	volatile int putidx; /* read index and put index */
	volatile int getidx;
};

/**      ^rdidx,read from there
 **      |
 **  xxxx123456xxxx
 **            ^
 **            |ptidx,write to there
 */
 
#define DECLARE_RB(name,len) \
	static uint8_t name##_buff[len]; \
	struct ringbuffer name = { \
		name##_buff,len,0,0,0 }
		
/**
 ** Init a ringbuffer using 'bf' and 'len'.
 ** set head and tail as zero
 **/
#define rbinit(prb,bf,len) do { \
	(prb)->bf=bf;(prb)->len=len;} \
	while(0)

#define rbfull(prb) ( ((prb)->putidx+1)%(prb)->len == (prb)->getidx )
#define rbempty(prb) ( (prb)->getidx == (prb)->putidx )

/* if ringbuffer is full,return 0.else return 1 */
int rbput(struct ringbuffer *prb,uint8_t add);
/* if ringbuffer is empty,return -1.else return bf[tail] */
int rbget(struct ringbuffer *prb);

#endif 


