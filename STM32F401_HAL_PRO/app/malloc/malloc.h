#ifndef _MOLLOC_H
#define _MOLLOC_H

	#include "main.h"
	
	#ifndef NULL
		#define NULL 0
	#endif

	//���������ڴ��
	#define SRAMIN	 0		//�ڲ��ڴ��
	#define SRAMEX   1		//�ⲿ�ڴ��(��ӢSTM32�����岻֧���ⲿ�ڴ�) 
  //�����ֶ��ٸ�SRAM�ɹ���
	#define SRAMBANK 	2	//����֧�ֵ�SRAM����. ��Ӣ��ʵ����ֻ֧��1���ڴ�����,���ڲ��ڴ�.	
	
	//mem1�ڴ�����趨.mem1��ȫ�����ڲ�SRAM����.(�����ڲ�SARM���ڴ�غ��ڴ��Ĳ���)
	#define MEM1_BLOCK_SIZE			32  	  						              //һ���ڴ���СΪ32�ֽ�
	#define MEM1_MAX_SIZE			2*1024  						                //�������ڴ� 20K  (��������ڴ����ϵͳ���ڲ�SRAM�ɿ��Ƶ��ڴ��С)
	#define MEM1_ALLOC_TABLE_SIZE	MEM1_MAX_SIZE/MEM1_BLOCK_SIZE 	//�ڴ���С(�ж��ٿ��ڴ��)

	//mem2�ڴ�����趨.mem2���ڴ�ش����ⲿSRAM����
	#define MEM2_BLOCK_SIZE			32  	  						              //һ���ڴ���СΪ32�ֽ�
	#define MEM2_MAX_SIZE			1 *32  							                //��Ϊ��Ӣ��û�������ڴ�,����������һ����Сֵ
	#define MEM2_ALLOC_TABLE_SIZE	MEM2_MAX_SIZE/MEM2_BLOCK_SIZE 	//�ڴ���С 
		
	//�ڴ����������ṹ��
	//ע�⣺�ڴ�������ڴ�غ��ڴ��б����
	//SRAMBANK��SARM������һ�����ڲ�SRAM���ⲿSRAM��CCM
	struct _m_mallco_dev
	{
		void (*init)(uint8_t);					    //��ʼ��
		uint8_t (*perused)(uint8_t);		  	    //�ڴ�ʹ����
		uint8_t 	*membase[SRAMBANK];				//�ڴ�� ����SRAMBANK��������ڴ�
		uint16_t *memmap[SRAMBANK]; 				//�ڴ����״̬��
		uint8_t  memrdy[SRAMBANK]; 				//�ڴ�����Ƿ����
	};
	
	void my_mem_init(uint8_t memx);
	uint8_t my_mem_perused(uint8_t memx);

	void *mymalloc(uint8_t memx,uint32_t size);
	void myfree(uint8_t memx,void *ptr);
	void *myrealloc(uint8_t memx,void *ptr,uint32_t size);
	
#endif











