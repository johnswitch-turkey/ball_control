#ifndef __KEY_H
#define __KEY_H

#include "stm32f4xx_hal.h"

extern uint8_t key_state; // 目标位置 // 1:左上 2:右上 3:左下 4:右下

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

#endif
