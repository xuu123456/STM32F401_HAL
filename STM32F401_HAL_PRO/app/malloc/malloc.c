#include "malloc.h"



/*****************************************************************************
*                           ����C����                                         *
******************************************************************************
*__align ����     ��������������ڴ��е�λ���йأ�Ҳ�����ڴ��ֽڶ��롣�е�
*                   ����⣬��Ҫͨ�����¾��������
*__attribute__���ã��������ú������ԡ��������Ժ��������ԡ�����������������
*                   �Զ�λ��ַ����ר��ָ���ڴ��ַ
*
*ʵ��һ��
*     __align(32) uint8_t mem2base[MEM2_MAX_SIZE] __attribute__((at(0X68000000)));
*   ��˼������һ�����飬�����СΪMEM2_MAX_SIZE��������ռ�Ŀռ��ܱ�32������������
*         ����ʼ�ڴ��ַΪ0X68000000��
*   ���MEM2_MAX_SIZEΪ2���������ڴ�ռ��СΪ32�ֽڣ����MEM2_MAX_SIZEΪ33������
*   ���ڴ�ռ��СΪ64�ֽڡ�
*
*ʵ������
struct A{
	char a;
	unsigned int b;
	unsigned char c;
	char d;
};

��һ���ṹ����:
structB{
	char a;
	unsigned int b;
	unsigned char c;
	char d;
}__attribute__((align(8)));

sizeof(A) = 12(�ڴ�ռ��С12���ֽ�) 
sizeof(B) = 8(�ڴ�ռ��С8���ֽ�)
********************************************************************************/
//�ڴ��(32�ֽڶ���)
//�ɿ��Ƶ��ڴ��С
__align(32) uint8_t mem1base[MEM1_MAX_SIZE];	   												                   //�ڲ�SRAM�ڴ��
__align(32) uint8_t mem2base[MEM2_MAX_SIZE] __attribute__((at(0X68000000)));					     //�ⲿSRAM�ڴ��
//�ڴ�����
//�ɿ��Ƶ��ڴ���ƿ����(ÿ���ڴ���СΪ32�ֽ�)
uint16_t mem1mapbase[MEM1_ALLOC_TABLE_SIZE];													                      //�ڲ�SRAM�ڴ��MAP
uint16_t mem2mapbase[MEM2_ALLOC_TABLE_SIZE] __attribute__((at(0X68000000+MEM2_MAX_SIZE)));	//�ⲿSRAM�ڴ��MAP

//�ڴ�������	
//const ����ı�����ֵ�ǲ�����ı��
//��Ϊ���ڲ�SRAM���ⲿSRAM������������
const uint32_t memtblsize[SRAMBANK]={MEM1_ALLOC_TABLE_SIZE,MEM2_ALLOC_TABLE_SIZE};			//�ڴ���С(�����ƶ����ڴ��)
const uint32_t memblksize[SRAMBANK]={MEM1_BLOCK_SIZE,MEM2_BLOCK_SIZE};						      //�ڴ�ֿ��С(һ���ڴ��ռ�����ֽ��ڴ�ռ�)
const uint32_t memsize[SRAMBANK]={MEM1_MAX_SIZE,MEM2_MAX_SIZE};	                      //�ڴ�ش�С(�����Է�����ڴ�ռ��С)

//�ڴ���������
struct _m_mallco_dev mallco_dev=
{
	my_mem_init,				     //�ڴ��ʼ��
	my_mem_perused,				   //�ڴ�ʹ����
	mem1base,mem2base,			 //�ڴ��
	mem1mapbase,mem2mapbase, //�ڴ����״̬��
	0,0,  		 				       //�ڴ����δ����
};

/*******************************************
*��������  �������ڴ����������(��һ���ڴ�ռ��������ݵ���һ�ڴ�ռ���)
*������    ��mymemcpy
*��������  ��void *des   void *src   uint32_t n
*��������ֵ��void
*����      ��
*            *des ��Ŀ���ַ
*            *src ��Դ��ַ
*              n  ��Ҫ���Ƶĳ���(�ֽ�Ϊ��λ)
*********************************************/
void mymemcpy(void *des,void *src,uint32_t n)  
{  
	//һ�����ǲ����Ҫ�����Ĳ���ָ����в���
	//����ͨ��һ������ָ����Ϊ�н飬������Ϊ�˳��ڰ�ȫ��֤
	uint8_t *xdes=des;
	uint8_t *xsrc=src; 
	//������++֮ǰ�������ã���++
	//������++֮����++����ʹ��
	while(n--)*xdes++=*xsrc++;  
}  

/*****************************************************
*��������  �������ڴ�(�����ڴ�ռ��ֵ��һ�������Կռ���0)
*������    ��mymemset
*��������  ��void *s    uint8_t c   uint32_t count
*��������ֵ��void
*����      ��
*            *s    ���ڴ��׵�ַ
*            c     ��Ҫ���õ�ֵ
*            count ����Ҫ���õ��ڴ��С(�ֽ�Ϊ��λ)
******************************************************/
void mymemset(void *s,uint8_t c,uint32_t count)  
{  
	//һ�����ǲ����Ҫ�����Ĳ���ָ����в���
	//����ͨ��һ������ָ����Ϊ�н飬������Ϊ�˳��ڰ�ȫ��֤
	uint8_t *xs = s;  
	//������++֮ǰ�������ã���++
	//������++֮����++����ʹ��
	while(count--)*xs++=c;  
}

/*****************************************************************
*��������  ���ڴ�����ʼ��
*������    ��my_mem_init
*��������  ��uint8_t memx
*��������ֵ��void
*����      ��
*           memx:�����ڴ�飬�����ڲ�SRAM�����ⲿSRAM���ڴ��
*
*    ��ʵ��ν�ĳ�ʼ�����ǰ��ڴ�غ��ڴ��(���ǵı��ʾ�������)��0
******************************************************************/
void my_mem_init(uint8_t memx)  
{  
  mymemset(mallco_dev.memmap[memx], 0,memtblsize[memx]*2);  //�ڴ�״̬����������  
	mymemset(mallco_dev.membase[memx], 0,memsize[memx]);	    //�ڴ��������������  
	mallco_dev.memrdy[memx]=1;								                //�ڴ�����ʼ��OK�����ڴ�غ��ڴ����0��  
}  

/*****************************************************************
*��������  ����ȡ�ڴ�ʹ����
*������    ��my_mem_perused
*��������  ��uint8_t memx
*��������ֵ��void
*����      ��
*           memx:�����ڴ�飬�����ڲ�SRAM�����ⲿSRAM���ڴ��
*
*    �Ƿ�ռ����ͨ���ж�mem1mapbase��mem2mapbase�������Ա�Ƿ��0�����
* ��0��ռ�ã�֮�������Աֵ��һ�����壬����ռ�˶��ٿ飬��ֵΪ10�����ʾ
* ������������10���ڴ��
******************************************************************/
uint8_t my_mem_perused(uint8_t memx)  
{  
	uint32_t used=0;  
	uint32_t i;  
  //memtblsize���ڴ���С(һ���ڴ����)
	//�����ڴ������
	for(i=0;i<memtblsize[memx];i++)  
	{  
		//mallco_dev.memmap[memx]���ڴ������
		//ȡ��ÿ����Ա�ж��Ƿ��0
		//��0��������
		if(mallco_dev.memmap[memx][i])used++; 
	} 
	
	//ʹ������/��������*100 = ʹ����
	return (used*100)/(memtblsize[memx]);  
}

/*****************************************************************
*��������  ���ڴ����(�ڲ�����)------ȷ�����ڴ�ص�ƫ����
*������    ��my_mem_malloc
*��������  ��uint8_t memx,uint32_t size
*��������ֵ��uint32_t
*����      ��
*           memx:�����ڴ�飬�����ڲ�SRAM�����ⲿSRAM���ڴ��
*           size:Ҫ������ڴ��С(�ֽ�)
*           ����ֵ:0XFFFFFFFF,�������;����,�ڴ�ƫ�Ƶ�ַ
*
*           ע�⣺�ڴ��ı����ǴӺ���ǰ��
******************************************************************/
uint32_t my_mem_malloc(uint8_t memx,uint32_t size)  
{  
	signed long offset=0;  //ƫ�������� 
	uint32_t nmemb;	           //��Ҫ���ڴ����  
	uint32_t cmemb=0;           //�������ڴ��������֤����������ڴ����������
	uint32_t i;  
	//�ж��Ƿ���ִ���˳�ʼ��
	if(!mallco_dev.memrdy[memx]) mallco_dev.init(memx);//δ��ʼ��,��ִ�г�ʼ�� 
	if(size==0)return 0XFFFFFFFF;//����Ҫ����
  //�ڴ���� = ����ռ��С(�ֽڵ�λ) / tһ���ڴ���С(�ֽڵ�λ)
	nmemb=size/memblksize[memx];  	//��ȡ��Ҫ����������ڴ����
	//����ռ��С(�ֽڵ�λ) / tһ���ڴ���С(�ֽڵ�λ) ��= 0
	//�����0��Ҫ������һ���ڴ��
	if(size%memblksize[memx])nmemb++;  
	//�ڴ��ı����ǴӺ���ǰ��
	for(offset=memtblsize[memx]-1;offset>=0;offset--)//���������ڴ������  
	{ 
    //�жϸ��ڴ���Ƿ�ռ����   
		if(!mallco_dev.memmap[memx][offset])cmemb++;//�������ڴ��������
		//��֤�ڴ���������
		else cmemb=0;								                //�����ڴ������
		//ȷ���������ڴ��λ�ú�
		if(cmemb==nmemb)							              //�ҵ�������nmemb�����ڴ��
		{
			for(i=0;i<nmemb;i++)  				     	//��ע�ڴ��ǿ� 
			{  
				//��ʼ���ڴ�����ڴ�������λ�ñ�Ǹ��ڴ�鱻ռ��
				mallco_dev.memmap[memx][offset+i]=nmemb;  
			} 
			
			//ȷ������ռ����ڴ������λ��    ���ڴ������λ��*һ���ڴ���С(32�ֽ�) 
			return (offset*memblksize[memx]);   //����ƫ�Ƶ�ַ  
		}
	}  
	return 0XFFFFFFFF;//δ�ҵ����Ϸ����������ڴ��  
}  

/*****************************************************************
*��������  ���ͷ��ڴ�(�ڲ�����)------�ڴ��ƫ�����������ռ����ڴ���ռ�ñ�־
*������    ��my_mem_free
*��������  ��uint8_t memx,uint32_t offset
*��������ֵ��uint32_t
*����      ��
*           memx:�����ڴ�飬�����ڲ�SRAM�����ⲿSRAM���ڴ��
*           size:�ڴ��ַƫ��(�ֽ�)--------Ҳ�������ڴ�������λ��
*           ����ֵ:0,�ͷųɹ�;1,�ͷ�ʧ��;
*
******************************************************************/
uint8_t my_mem_free(uint8_t memx,uint32_t offset)  
{  
	int i;
	int index;
	int nmemb;
	
	//�ж��Ƿ��ʼ��
	if(!mallco_dev.memrdy[memx])//δ��ʼ��,��ִ�г�ʼ��
	{
		mallco_dev.init(memx);    
    return 1;//δ��ʼ��  
  }
  
	//�ж����ƫ�����Ƿ񳬳����ڴ�صĴ�С
	if(offset<memsize[memx])//ƫ�����ڴ����. 
	{ 
		//�ڴ��ƫ���� = �ڴ��ƫ����/һ���ڴ���С
		index=offset/memblksize[memx];			  //ƫ�������ڴ�����  
		//�ڴ�������Ա��ֵ��������Ŀ���
		nmemb=mallco_dev.memmap[memx][index];	//�ڴ������
		
		for(i=0;i<nmemb;i++)  						        //�ڴ������
		{  
			//�������ռ����ڴ��ı��
			mallco_dev.memmap[memx][index+i]=0;  
		}  
		return 0;  
	}else return 1;//ƫ�Ƴ�����.  
}  

/*****************************************************************
*��������  �������ڴ�(�ⲿ����)
*������    ��mymalloc
*��������  ��uint8_t memx,uint32_t size
*��������ֵ��void *
*����      ��
*           memx:�����ڴ�飬�����ڲ�SRAM�����ⲿSRAM���ڴ��
*           size:�ڴ��С(�ֽ�)
*           ����ֵ:���䵽���ڴ��׵�ַ
******************************************************************/
void *mymalloc(uint8_t memx,uint32_t size)  
{  
	uint32_t offset;    //���ڴ�������ƫ��������  
	//��ȡ���ڴ�������ƫ����
	offset=my_mem_malloc(memx,size); 
  //�����������򷵻ؿյ�ַ	
	if(offset==0XFFFFFFFF)return NULL;  
	//�������ɹ����򷵻�����ռ��׵�ַ
	else return (void*)((uint32_t)mallco_dev.membase[memx]+offset);  
}  

/*****************************************************************
*��������  ���ͷ��ڴ�(�ⲿ����)
*������    ��myfree
*��������  ��uint8_t memx,void *ptr
*��������ֵ��uint32_t
*����      ��
*           memx:�����ڴ�飬�����ڲ�SRAM�����ⲿSRAM���ڴ��
*           ptr :Ҫ�ͷŵ��ڴ�ռ��׵�ַ 
******************************************************************/
void myfree(uint8_t memx,void *ptr)  
{  
	uint32_t offset;
  uint32_t n;     //��Ҫ�ͷŵĿռ�Ŀռ��С	
	if(ptr==NULL)return;//��ַΪ0. 
  //ȷ������ռ���ڴ��ƫ����	
 	offset=(uint32_t)ptr-(uint32_t)mallco_dev.membase[memx]; 
	//�ռ�ռ�ڴ�ؿռ�Ĵ�С
	n=mallco_dev.memmap[memx][offset/memblksize[memx]] * memblksize[memx];	
	//�ͷ��ڴ�ض�Ӧ�ռ������
	mymemset(ptr,0,n);
  //�ͷ��ڴ��	
  my_mem_free(memx,offset);	//�ͷ��ڴ�      
}  


/*****************************************************************
*��������  �����·����ڴ�(�ⲿ����)
*������    ��myfree
*��������  ��uint8_t memx,void *ptr
*��������ֵ��uint32_t
*����      ��
*           memx:�����ڴ�飬�����ڲ�SRAM�����ⲿSRAM���ڴ��
*           ptr :���ڴ�ռ��ַ�׵�ַ
*           size:Ҫ���·�����ڴ��С(�ֽ�)
******************************************************************/
void *myrealloc(uint8_t memx,void *ptr,uint32_t size)  
{  
	uint32_t offset; 
	
	//����һ���µĿռ�
	offset=my_mem_malloc(memx,size);   	
	if(offset==0XFFFFFFFF)return NULL;     
	else  
	{ 
    //�Ѿɿռ�����ݸ��Ƶ��¿ռ���		
		mymemcpy((void*)((uint32_t)mallco_dev.membase[memx]+offset),ptr,size);	//�������ڴ����ݵ����ڴ�  
		//ɾ���ɿռ�
		myfree(memx,ptr);                                              		//�ͷž��ڴ�
		//�����¿ռ��ַ
		return (void*)((uint32_t)mallco_dev.membase[memx]+offset);  				    //�������ڴ��׵�ַ
	}    
}



