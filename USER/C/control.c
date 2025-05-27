#include "control.h"

#include "pid.h"
#include "uart.h"
#include "led.h"
#include "servo.h"
#include "main.h"

Tar_Position target_position;


uint16_t ball_site[4][2] = {
    {0, 0},  {0, 0},
    {0, 0},  {0, 0}
};


//extern UART_HandleTypeDef huart4;  // 声明外部变量
extern uint8_t uart1_rxbuff[];


float g_fSpeedControlOut_X,g_fSpeedControlOutOld_X;//X方向速度环输出
float g_fSpeedControlOut_Y,g_fSpeedControlOutOld_Y;//X方向速度环输出
int g_nSpeedControlPeriod;//速度环控制周期计算量

float x_angle, y_angle;

void control(void){
    static uint32_t position_timer = 0;    // 位置环周期

    static float cont_val_X = 0.0, cont_val_Y = 0.0;                  // 当前X,Y方向控制值（位置环、速度环共用）
    static int pre_Cx,pre_Cy;

    static int actual_speedX,actual_speedY;
	
	g_nSpeedControlPeriod++;

	if(g_nSpeedControlPeriod >= 3)
	{
	
	/***********************************位置外环**************************************/
							if (position_timer++%2 == 0)           //位置环(外环)控制频率 是 速度环(内环)控制频率 的一半
							{
								
								set_pid_target(&pid_positionX, target_position.target_position_x);       //得到openmv采集到的小球坐标数据，有没有必要用一下滤波？低通滤波啥的。例如这一次的坐标值为上一次的70%加上这一次数据的%30
								set_pid_target(&pid_positionY, target_position.target_position_y);
								
								cont_val_X = location_pid_realize(&pid_positionX, Cx);    // 进行 PID 计算
								cont_val_Y = location_pid_realize(&pid_positionY, Cy);
								
								/* 目标速度上限处理 */    //如果想3s内将球从一个角落滚动到另一个角落，那么在以100ms为单位时间的情况下，那么每次采样时实际坐标值差值大概为 80*0.03=2.4
								Limit_speed (&cont_val_X, &cont_val_Y);   // 速度上限处理
								
								set_pid_target(&pid_speedX, cont_val_X);    // 设定速度的目标值 ，位置环的输出给速度内环当目标值
								set_pid_target(&pid_speedY, cont_val_Y);
								
								 //速度内环调试代码，要调试速度内环时取消注释，只要将小球放在板子上能让小球立刻速度为0，则
//								set_pid_target(&pid_speedX, 0);
//								set_pid_target(&pid_speedY, 0);
							}
							
					/***********************************速度内环**************************************/
						//下面五句是通过一定时间的中断捕获小球位置，作差得到的速度
						//这里还要考虑openmv的传送数据的速度吧，不然要是这次减去上次的偏差不直接为0了吗？加上灯光可以在20Hz左右
						//这个应该等待时间长一点，否则数值变化很小。  但1000/20=50 , 50ms也挺大的了。  反正采样周期是一样的，除不除以时间都无所谓了，只是数值大小不一样而已
						//舵机控制信号周期可以10ms，也可以20ms。就用10ms的吧

							//获取速度
							actual_speedX = Cx-pre_Cx;       //因为是速度，所以用这一次减去上一次  
							actual_speedY = Cy-pre_Cy;
							pre_Cx = Cx;
							pre_Cy = Cy;
							
							
							cont_val_X = speed_pid_realize(&pid_speedX, actual_speedX);    // 进行 PID 计算   
							cont_val_Y = speed_pid_realize(&pid_speedY, actual_speedY); 
							
									 /*     舵机角度上限处理   */ 
							//如果速度环PID控制器输出值当成是角度的话， 下面要是直接赋值给角度，理论上极限值是-90~90。
							
							//将这个数值转换为角度其实也是可以的
							//只要控制周期足够短，小球加速度 就可看成 一个系数 乘上 平板倾角（小球速度与平板倾角近似线性关系），当然也就可以直接转换为舵机的转动角度。  
							//可直接将此值作为小球倾角使用，相应系数包含在了比例P中
							
							//反正位置到了就行，速度控制不用很精确，所以把数据处理一下换成舵机角度上限处理吧，更加直观地，更好防止舵机打舵过度
							
							//理论上甚至可以拿个六轴传感器来测板子实际倾角，再搞个闭环，但没必要。
							
							g_fSpeedControlOutOld_X = x_angle;  //保存上一次输出  
							g_fSpeedControlOutOld_Y = y_angle;
							//水平调试
					//		cont_val_X = 0;  
					//		cont_val_Y = 0;
							
							
							x_angle = limit_angle(cont_val_X);  // 转换量程，板子倾斜角度限幅 （90度正反转动一个值）
							y_angle = limit_angle(cont_val_Y);

							g_nSpeedControlPeriod = 0;

}

AngleControlOutput();

							Set_servo_angle(g_fSpeedControlOut_X, g_fSpeedControlOut_Y);  // 舵机控制
}



void Limit_speed (float * val_x , float * val_y)
{
    if (* val_x > TARGET_SPEED_MAX)
								{
									* val_x = TARGET_SPEED_MAX;   
								}
								else if (* val_x < -TARGET_SPEED_MAX)
								{
									* val_x = -TARGET_SPEED_MAX;
								}
								
								 if (* val_y > TARGET_SPEED_MAX)
								{
									* val_y = TARGET_SPEED_MAX;   
								}
								else if (* val_y < -TARGET_SPEED_MAX)
								{
									* val_y = -TARGET_SPEED_MAX;
								}
}


float limit_angle(float angle)
{
    if(angle > 90.0f)
        return 90.0f;
    else if(angle < -90.0f)
        return -90.0f;
    return angle;
}


void AngleControlOutput(void)    //平滑输出，将一次速度环控制拆分为三次输出，每次两个10ms周期的PWM
{
	float fValue_X,fValue_Y;
	fValue_X = x_angle - g_fSpeedControlOutOld_X ;//速度计算量差值=本次速度计算量-上次速度计算量
	g_fSpeedControlOut_X = fValue_X * (g_nSpeedControlPeriod + 1) / SPEED_CONTROL_PERIOD + g_fSpeedControlOutOld_X;//x方向速度计算量差值
	
	fValue_Y = y_angle - g_fSpeedControlOutOld_Y ;//速度计算量差值=本次速度计算量-上次速度计算量
	g_fSpeedControlOut_Y = fValue_Y * (g_nSpeedControlPeriod + 1) / SPEED_CONTROL_PERIOD + g_fSpeedControlOutOld_Y;//Y方向速度计算量差值
}

