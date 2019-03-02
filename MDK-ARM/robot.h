#ifndef __ROBOT_H
#define __ROBOT_H

/* 	In robot.h & robot.c define:
		struct ROBOT includes ALL of the whole robot
		ALL interruption behavior													*/

#include "hardware.h"

struct ROBOT
{
	struct HARDWARE hardware;
};

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
	

#endif
