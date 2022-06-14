#include "MultiTimer.h"
#include <stdio.h>
#include "include.h"
/* Timer handle list head. */
static MultiTimer* timerList = NULL;

/* Timer tick */
static PlatformTicksFunction_t platformTicksFunction = NULL;

int MultiTimerInstall(PlatformTicksFunction_t ticksFunc)
{
    platformTicksFunction = ticksFunc;
    return 0;
}

int MultiTimerStart(MultiTimer* timer, uint64_t timing, MultiTimerCallback_t callback, void* userData)
{
    if (!timer || !callback ) {
        return -1;
    }
    MultiTimer** nextTimer = &timerList;
    /* Remove the existing target timer. */
    for (; *nextTimer; nextTimer = &(*nextTimer)->next) {
        if (timer == *nextTimer) {
            *nextTimer = timer->next; /* remove from list */
            break;
        }
    }

    /* Init timer. */
    timer->deadline = platformTicksFunction() + timing;
    timer->callback = callback;
    timer->userData = userData;

    /* Insert timer. */
    for (nextTimer = &timerList;; nextTimer = &(*nextTimer)->next) {
        if (!*nextTimer) {
            timer->next = NULL;
            *nextTimer = timer;
            break;
        }
        if (timer->deadline < (*nextTimer)->deadline) {
            timer->next = *nextTimer;
            *nextTimer = timer;
            break;
        }
    }
    return 0;
}

int MultiTimerStop(MultiTimer* timer)
{
    MultiTimer** nextTimer = &timerList;
    /* Find and remove timer. */
    for (; *nextTimer; nextTimer = &(*nextTimer)->next) {
        MultiTimer* entry = *nextTimer;
        if (entry == timer) {
            *nextTimer = timer->next;
            break;
        }
    }
    return 0;
}

int MultiTimerYield(void)
{
    MultiTimer* entry = timerList;
    for (; entry; entry = entry->next) {
        /* Sorted list, just process with the front part. */
        if (platformTicksFunction() < entry->deadline) {
            return (int)(entry->deadline - platformTicksFunction());
        }
        /* remove expired timer from list */
        timerList = entry->next;

        /* call callback */
        if (entry->callback) {
            entry->callback(entry, entry->userData);
        }
    }
    return 0;
}


/*
以下是我的接口函数

*/
extern RingBuff_t Key_ringBuff;
uint64_t _time_tick_ms_;
uint8_t key_cnt;//5s中按键按下次数
void timer_ticks(void) //1ms ticks，需要放在定时器中运行
{
	_time_tick_ms_++;
}
uint64_t Get_Tick(void)//获取时间戳
{
	return _time_tick_ms_;
}

void MyTimer1Callback(MultiTimer* timer, void *userData)
{
    HAL_GPIO_TogglePin (GPIOA,GPIO_PIN_5);	
    MultiTimerStart(timer, 1000, MyTimer1Callback, userData);	
		if(key_cnt > 0) 
		{
			HAL_GPIO_TogglePin (GPIOA,GPIO_PIN_6);
			key_cnt--;
		}
}
void MyTimer2Callback(MultiTimer* timer, void *userData)
{
	if(Get_Key_SW1_D2() == 0 || Get_Key_SW2_D3() == 0)
		Write_RingBuff(&Key_ringBuff,1);//在缓存区中写入一个按键值
	
  MultiTimerStart(timer, 50, MyTimer2Callback, userData);
}
void MyTimer3Callback(MultiTimer* timer, void *userData)
{
	uint8_t cnt,dat;
	while(Read_RingBuff(&Key_ringBuff,&dat) != FLASE) 
		cnt++;//读取缓冲区中按键按下的次数
	key_cnt = cnt;
    MultiTimerStart(timer, 5000, MyTimer3Callback, userData);
}
