#include "servo.h"

#include "stm32f4xx_hal.h"

extern TIM_HandleTypeDef htim2; // 定义定时器句柄

void Set_servo_angle(uint16_t angle1,uint16_t angle2)
{
    uint32_t pulse_width = (angle1 * 10) + 500; // 500us ~ 2500us
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, pulse_width);
}

