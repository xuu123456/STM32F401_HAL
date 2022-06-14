#include "RingBuffer.h"
/**
 2* @brief  RingBuff_Init
 3* @param  void
 4* @return void
 5* @author 杰杰
 6* @date   2018
 7* @version v1.0
 8* @note   初始化环形缓冲区
 9*/
void RingBuff_Init(RingBuff_t *const this)
{
   //初始化相关信息
   this->Head = 0;
   this->Tail = 0;
   this->Lenght = 0;
}

/**
 2* @brief  Write_RingBuff
 3* @param  u8 data,
 4* @return FLASE:环形缓冲区已满，写入失败;TRUE:写入成功
 5* @author 杰杰
 6* @date   2018
 7* @version v1.0
 8* @note   往环形缓冲区写入u8类型的数据
 9*/
uint8_t Write_RingBuff(RingBuff_t *const this,uint8_t data)
{
   if(this->Lenght >= RINGBUFF_LEN) //判断缓冲区是否已满
    {
      return FLASE;
    }
    this->Ring_Buff[this->Tail]=data;
//    ringBuff.Tail++;
    this->Tail = (this->Tail+1)%RINGBUFF_LEN;//防止越界非法访问
    this->Lenght++;
    return TRUE;
}

/**
 2* @brief  Read_RingBuff
 3* @param  u8 *rData，用于保存读取的数据
 4* @return FLASE:环形缓冲区没有数据，读取失败;TRUE:读取成功
 5* @author 杰杰
 6* @date   2018
 7* @version v1.0
 8* @note   从环形缓冲区读取一个u8类型的数据
 9*/
uint8_t Read_RingBuff(RingBuff_t *const this,uint8_t *rData)
{
   if(this->Lenght == 0)//判断非空
    {
       return FLASE;
    }
   *rData = this->Ring_Buff[this->Head];//先进先出FIFO，从缓冲区头出
//   ringBuff.Head++;
   this->Head = (this->Head+1)%RINGBUFF_LEN;//防止越界非法访问
   this->Lenght--;
   return TRUE;
}

