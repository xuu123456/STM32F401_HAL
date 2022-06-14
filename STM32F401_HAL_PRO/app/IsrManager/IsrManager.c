#include "IsrManager.h"
extern TIM_HandleTypeDef htim10;

void Exit_Null(void){};
void Dma_Null(void){};
void Timer_Null(void){};

	void (*Exit_isr[])(void)=
	{
		Exit_Null,                 //0
		Exit_Null,  //1
		Exit_Null,  //2
		Exit_Null,      //3
		Exit_Null,      //4
		Exit_Null,                 //5
		Exit_Null,   //6
		Exit_Null,                 //7	
		Exit_Null,                 //8
		Exit_Null,                 //9
		Exit_Null,                 //10
		Exit_Null,                 //11 
		Exit_Null,                 //12
		Exit_Null,      //13
		Exit_Null,                 //14
		Exit_Null,                 //15
	};
	
	


void GPIO_PinReverse(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GPIO_PIN(GPIO_Pin));
  
  GPIOx->ODR ^=  GPIO_Pin;
}
void Timer1_Isr(uint32_t tim_sr)
{
	if (tim_sr & TIM_SR_UIF)
	{		
	}
}

void Timer2_Isr(uint32_t tim_sr)
{
	if (tim_sr & 0x0008)
	{
	}
	if (tim_sr & 0x0004)
	{
	}

}


void Timer3_Isr(uint32_t tim_sr)
{

}


void Timer4_Isr(uint32_t tim_sr)
{
	
}

void Timer5_Isr(uint32_t tim_sr)
{
	
}


void Timer9_Isr(uint32_t tim_sr)
{	
//	#ifndef BOOTLOADER
//	if (tim_sr & TIM_IT_Update)
//	{		
//		HwLed_Breathing_Process();
//		Beep_It_Control();
//	} 
//	#endif
}



void Timer10_Isr(uint32_t tim_sr)
{
	//给multi timer提供时基信号
   timer_ticks(); //1ms ticks
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)//重写定时器更新中断回调函数
{
	if(htim == &htim10)
	{
		const uint32_t sr = (TIM10->SR & 0x1f);
		//TIM10->SR = ~sr;
		Timer10_Isr(sr);
	}
}

void Rtc_Isr(void)
{
	
}

void (*Dma_Channel_isr[])(void) = 
{
	Dma_Null,              //  0 no use
	Dma_Null,              //  1   
	Dma_Null,         //  2
	Dma_Null,              //  3
	Dma_Null,//  4
	Dma_Null,              //  5
	Dma_Null,              //  6
	Dma_Null,              //  7	
};
