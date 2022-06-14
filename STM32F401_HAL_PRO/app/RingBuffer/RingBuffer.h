#ifndef _RingBuffer_H_
#define _RingBuffer_H_

#include "include.h"

#define USER_RINGBUFF  1  //使用环形缓冲区形式接收数据
#if  USER_RINGBUFF
	/**如果使用环形缓冲形式接收串口数据***/
	#define  RINGBUFF_LEN          50     //定义最大接收字节数 50
	#define  FLASE   1 
	#define  TRUE    0 

	typedef struct
	{
			uint16_t Head;           
			uint16_t Tail;
			uint16_t Lenght;
			uint8_t Ring_Buff[RINGBUFF_LEN];
	}RingBuff_t;
	//RingBuff_t ringBuff;//创建一个ringBuff的缓冲区

	void RingBuff_Init(RingBuff_t *const this);
	uint8_t Write_RingBuff(RingBuff_t *const this,uint8_t data);
	uint8_t Read_RingBuff(RingBuff_t *const this,uint8_t *rData);
#endif


#endif
	
	
	
	
	