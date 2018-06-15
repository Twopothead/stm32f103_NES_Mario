#include "ringbuffer.h"

int rbput(struct ringbuffer *rb,uint8_t add)
{
	if(rbfull(rb)) return 0;
	rb->bf[rb->putidx]=add;
	rb->putidx=(rb->putidx+1)%rb->len;
	return 1;
}

int rbget(struct ringbuffer *rb)
{
	int result;
	if(rbempty(rb)) return -1;
	result=rb->bf[rb->getidx];
	rb->getidx=(rb->getidx+1)%rb->len;
	return result;
}
