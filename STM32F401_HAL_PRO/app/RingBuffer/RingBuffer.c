#include "RingBuffer.h"
/**
 2* @brief  RingBuff_Init
 3* @param  void
 4* @return void
 5* @author �ܽ�
 6* @date   2018
 7* @version v1.0
 8* @note   ��ʼ�����λ�����
 9*/
void RingBuff_Init(RingBuff_t *const this)
{
   //��ʼ�������Ϣ
   this->Head = 0;
   this->Tail = 0;
   this->Lenght = 0;
}

/**
 2* @brief  Write_RingBuff
 3* @param  u8 data,
 4* @return FLASE:���λ�����������д��ʧ��;TRUE:д��ɹ�
 5* @author �ܽ�
 6* @date   2018
 7* @version v1.0
 8* @note   �����λ�����д��u8���͵�����
 9*/
uint8_t Write_RingBuff(RingBuff_t *const this,uint8_t data)
{
   if(this->Lenght >= RINGBUFF_LEN) //�жϻ������Ƿ�����
    {
      return FLASE;
    }
    this->Ring_Buff[this->Tail]=data;
//    ringBuff.Tail++;
    this->Tail = (this->Tail+1)%RINGBUFF_LEN;//��ֹԽ��Ƿ�����
    this->Lenght++;
    return TRUE;
}

/**
 2* @brief  Read_RingBuff
 3* @param  u8 *rData�����ڱ����ȡ������
 4* @return FLASE:���λ�����û�����ݣ���ȡʧ��;TRUE:��ȡ�ɹ�
 5* @author �ܽ�
 6* @date   2018
 7* @version v1.0
 8* @note   �ӻ��λ�������ȡһ��u8���͵�����
 9*/
uint8_t Read_RingBuff(RingBuff_t *const this,uint8_t *rData)
{
   if(this->Lenght == 0)//�жϷǿ�
    {
       return FLASE;
    }
   *rData = this->Ring_Buff[this->Head];//�Ƚ��ȳ�FIFO���ӻ�����ͷ��
//   ringBuff.Head++;
   this->Head = (this->Head+1)%RINGBUFF_LEN;//��ֹԽ��Ƿ�����
   this->Lenght--;
   return TRUE;
}

