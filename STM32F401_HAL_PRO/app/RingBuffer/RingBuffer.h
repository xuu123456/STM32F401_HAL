#ifndef _RingBuffer_H_
#define _RingBuffer_H_

#include "include.h"

#define USER_RINGBUFF  1  //ʹ�û��λ�������ʽ��������
#if  USER_RINGBUFF
	/**���ʹ�û��λ�����ʽ���մ�������***/
	#define  RINGBUFF_LEN          50     //�����������ֽ��� 50
	#define  FLASE   1 
	#define  TRUE    0 

	typedef struct
	{
			uint16_t Head;           
			uint16_t Tail;
			uint16_t Lenght;
			uint8_t Ring_Buff[RINGBUFF_LEN];
	}RingBuff_t;
	//RingBuff_t ringBuff;//����һ��ringBuff�Ļ�����

	void RingBuff_Init(RingBuff_t *const this);
	uint8_t Write_RingBuff(RingBuff_t *const this,uint8_t data);
	uint8_t Read_RingBuff(RingBuff_t *const this,uint8_t *rData);
#endif


#endif
	
	
	
	
	