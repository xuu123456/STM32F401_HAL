#ifndef _Hw_IsrManager_H_
#define _Hw_IsrManager_H_
#include "Include.h"

void Exit_Null(void);
extern void (*Exit_isr[])(void);
extern void (*Dma_Channel_isr[])(void); 
 
void Timer_AdcSample_Start(void);
void Timer_AdcSample_End(void);
void Timer1_Isr(uint32_t tim_sr);
void Timer2_Isr(uint32_t tim_sr);
void Timer3_Isr(uint32_t tim_sr);
void Timer4_Isr(uint32_t tim_sr);
void Timer5_Isr(uint32_t tim_sr);
void Timer9_Isr(uint32_t tim_sr);
void Timer10_Isr(uint32_t tim_sr);
void Timer11_Isr(uint32_t tim_sr);
void Rtc_Isr(void);
#endif

