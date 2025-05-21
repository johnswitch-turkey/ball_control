#ifndef __PID_H
#define __PID_H
#ifndef __BSP_PID_H
#define	__BSP_PID_H
#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct
{
    float target_val;               //目标值
    float actual_val;        		//实际值
    float err;             			//定义偏差值
    float err_last;          		//定义上一个偏差值
    float Kp,Ki,Kd;          		//定义比例、积分、微分系数
    float integral;          		//定义积分值
}_pid;

extern _pid pid_positionX, pid_positionY, pid_speedX, pid_speedY;

extern void PID_param_init(void);

extern void set_pid_target(_pid *pid, float temp_val);
extern float get_pid_target(_pid *pid);

extern void set_p_i_d(_pid *pid, float p, float i, float d);

extern float location_pid_realize(_pid *pid, float actual_val);
extern float speed_pid_realize(_pid *pid, float actual_val);

#endif











#endif
