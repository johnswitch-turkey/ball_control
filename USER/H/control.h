#ifndef __CONTROL_H
#define __CONTROL_H

#include "stm32f4xx_hal.h"


#define TARGET_SPEED_MAX   2 // 目标速度的上限值 pixels/100ms  在stm32能每100ms收到一次openmv采集的小球位置坐标的情况下，走过的像素点数,不要调太快

#define SPEED_CONTROL_PERIOD 3 //速度环控制周期



typedef struct
{
    uint16_t target_position_x;             //x目标位置
    uint16_t target_position_y;        		//y目标位置
}Tar_Position;

extern Tar_Position target_position;

extern uint16_t ball_site[4][2]; //小球位置

void control(void);
void Limit_speed (float * val_x , float * val_y);
float limit_angle(float angle);
void AngleControlOutput(void);

#endif
