#include "HW_Api.h"

uint8_t Get_Key_SW1_D2(void)
{
	return HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_4);
}

uint8_t Get_Key_SW2_D3(void)
{
	return HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_5);
}

